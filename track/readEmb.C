const unsigned long nevents = 10000;
void readEmb(const char* mudstfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root",
	     const char* geantfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.geant.root")
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  //gROOT->Macro("LoadJetMaker.C");

  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StEEmcUtil");
  
  gSystem->Load("StEmcRawMaker");
  gSystem->Load("StEmcADCtoEMaker");
  
  gSystem->Load("StDbBroker");
  gSystem->Load("St_db_Maker");	
  
  // Create chain
  StChain* chain = new StChain;
  //
  //     // GEANT reader
  StIOMaker* ioMaker = new StIOMaker;
  ioMaker->SetFile(geantfile);
  ioMaker->SetIOMode("r");
  ioMaker->SetBranch("*",0,"0");             // Deactivate all branches
  ioMaker->SetBranch("geantBranch",0,"r");   // Activate geant branch
 
  StEmcGeom *emcGeom = StEmcGeom::instance("bemc"); 
  // Monte Carlo event maker
  StMcEventMaker* mcEventMaker = new StMcEventMaker;
  mcEventMaker->doPrintEventInfo  = false;
  mcEventMaker->doPrintMemoryInfo = false;

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",mudstfile,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");


  StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  //adc->saveAllStEvent(true);

  chain->Init();
  for (int iEvent = 1; iEvent <= nevents; ++iEvent) {
    chain->Clear();

    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    //MuDst
    StEmcCollection *emc = StMuDst::emcCollection();
    StEmcDetector* bemc = emc->detector(kBarrelEmcTowerId);

    vector<float> energy(4800);
    if (bemc) {
      for(unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
	StSPtrVecEmcRawHit& rawHits = bemc->module(m)->hits();
	//Printf("MuDst module %d hits %d\n", m, rawHits.size());
	for(size_t k = 0; k < rawHits.size(); ++k) {
	  StEmcRawHit *hit = rawHits[k];
	  float ee = hit->energy();
	  int towerId = 0;
	  emcGeom->getId(hit->module(), hit->eta(), abs(hit->sub()), towerId);
	  energy[towerId-1] = ee;
	  //if(towerId >=1 && towerId <= 4800){
	  //energy[towerId-1] = hit->energy();
	  //}
	}
      }
      Printf("mudst hit size: %d\n", energy.size());
    }else{
      Printf("bemc not found\n");
    }

    // Get StMcEvent
    StMcEvent* mcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mcEvent) continue;

    float vZ = mcEvent->primaryVertex()->position().z();
    
    Printf("vZ=%f\n", vZ);

    StMcEmcHitCollection *emcHits = mcEvent->bemcHitCollection();
    unsigned long nhits = emcHits->numberOfHits();
    unsigned int nmodules = emcHits->numberOfModules();

    unsigned long nsum = 0;
    vector<float> mcDe(4800, 0);
    for(unsigned int im = 1; im <= nmodules; im++){
      StMcEmcModuleHitCollection *module = emcHits->module(im);
      if(!module) { Printf("module %d not found\n", im); continue; }
      unsigned long nh = module->numberOfHits();
      unsigned long ndh = module->numberOfDetectorHits();
      if(nh != ndh) Printf("module = %d hn = %d, hdh = %d \n", im+1, nh, ndh);
      nsum += nh;

      StSPtrVecMcCalorimeterHit &hits = module->hits();
      Printf("size: %d\n", hits.size());
      for(unsigned long ih = 0; ih < hits.size(); ih++){
	StMcCalorimeterHit *hh = hits[ih];
	if(!hh) continue;
	float dE = hh->dE();
	int towerId;
	int sub = hit->sub();
	if(sub < 0) Print("sub = %d\n", sub);
	emcGeom->getId(im, hh->eta(), sub, towerId);
	mcDe[towerId-1] += dE;
	////float ee = energy[towerId-1];
	////if(dE > 0) Printf("towerId = %d, dE = %f, ee = %f\n", towerId, dE, ee);
	//if(dE > 0) Printf("towerId = %d, dE = %f\n", towerId, dE);
	StMcTrack *trk = hh->parentTrack();
	if(!trk){ Printf("track not found for dE = %f\n", dE); }
	else{
	  float pt = trk->pt();
	  float r = 0;
	  if(pt > 0) r = dE/pt;
          int ishw = trk->isShower();
          long pdg = trk->pdgId();
          long id = trk->geantId();
          long key = trk->key();
	  if(id == 7 || id == 1) Printf("towerid = %d r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n, ", towerId, r, pt, ishw, pdg, id, key);
	}
      }
    }
    Printf("nhits=%d, sum=%d, nmodules=%d \n", nhits, nsum, nmodules);
    for(int it = 0; it < 4800; it++){
      float diff = energy[it] - mcDe[it];
      if(mcDe[it] > 0) Printf("towerId = %d, mu = %f, mc = %f, diff = %f\n", it+1, energy[it], mcDe[it], diff);
    }
  }
}
