const unsigned long nevents = 10000;
const double MipPhotoElectrons = 63;
const double MipEnergyDeposit = 0.0198;
double mSF[3] = {14.356, -0.512, 0.668};
double samplingFraction(double eta) {
  double x = TMath::Abs(eta);
  return mSF[0]+mSF[1]*x+mSF[2]*x*x;
}

void readTestEmcSimu(//const char* mudstfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root",
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
  
  //emc simulator
  gSystem->Load("StEmcSimulatorMaker");
  // Create chain
  StChain* chain = new StChain;
  //
  // GEANT reader
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
  
  //StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",mudstfile,"",1000);
  //StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");
  

  //StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  //adc->saveAllStEvent(true);

  //StEmcSimulatorMaker *emcSimu = new StEmcSimulatorMaker;


  StBemcTables *emcTables = new StBemcTables;

  TFile *fout = new TFile("simu.emc.root", "recreate");
  TH2D *h = new TH2D("hsimu", ";geant;simu", 50, 0, 0.05, 250, -0.5, 2);
  
  chain->Init();
  
  for (int iEvent = 0; iEvent < nevents; ++iEvent) {
    chain->Clear();
    
    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    emcTables->loadTables(chain);
    
    //print flag
    bool flag_print = (iEvent%100 == 0);
    //bool flag = chain->GetMaker("Eread") || chain->GetMaker("emcRaw");
    //Printf("embedding flag = %d\n", flag);

    //emc simulator
    /*
    //bool flag_bfc = chain->InheritsFrom("StBFChain");
    //Printf("bfc flag = %d\n", flag_bfc);
    StEvent *event = (StEvent*) chain->GetDataSet("StEvent");
    if(!event) {Printf("event %d not found\n", iEvent); continue;}
    //StMuDst *dst = (StMuDst*)chain->GetDataSet("StMuDst");
    //if(!dst) Printf("mudst not found\n");
    StEmcCollection *emc = event->emcCollection();
    StEmcDetector *bemc = emc->detector(kBarrelEmcTowerId);
    if (bemc) {
      for(unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
	StSPtrVecEmcRawHit& rawHits = bemc->module(m)->hits();
	//Printf("MuDst module %d hits %d\n", m, rawHits.size());
	for(size_t k = 0; k < rawHits.size(); ++k) {
	  StEmcRawHit *hit = rawHits[k];
	  float ee = hit->energy();
	  unsigned int adc = hit->adc();
	  int towerId = 0;
	  emcGeom->getId(hit->module(), hit->eta(), hit->sub(), towerId);
	  if(flag_print) Printf("id=%d ee=%f adc=%u\n", towerId, ee, adc);
	}
      }
    }else{
      Printf("bemc not found\n");
    }
    */
    //MuDst
    /*StEmcCollection *emc = StMuDst::emcCollection();
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
    */
    // Get StMcEvent
    StMcEvent* mcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mcEvent) continue;

    float vZ = mcEvent->primaryVertex()->position().z();
    
    Printf("vZ=%f\n", vZ);

    StMcEmcHitCollection *emcHits = mcEvent->bemcHitCollection();
    unsigned long nhits = emcHits->numberOfHits();
    unsigned int nmodules = emcHits->numberOfModules();

    //vector<float> mcDe(4800);
    
      unsigned long nsum = 0;
      for(unsigned int im = 1; im <= nmodules; im++){
	StMcEmcModuleHitCollection *module = emcHits->module(im);
	if(!module) { Printf("module %d not found\n", im); continue; }
	unsigned long nh = module->numberOfHits();
	nsum += nh;
	unsigned long ndh = module->numberOfDetectorHits();
	Printf("module = %d hn = %d, hdh = %d \n", im+1, nh, ndh);
	/*
	  StSPtrVecMcCalorimeterHit &hits = module->hits();
	  Printf("geant hit size: %d\n", hits.size());
	  for(unsigned long ih = 0; ih < hits.size(); ih++){
	  StMcCalorimeterHit *hh = hits[ih];
	  if(!hh) continue;
	  float dE = hh->dE();
	  int towerId;
	  int sub = hh->sub();
	  //if(sub < 0) Print("sub = %d\n", sub);
	  emcGeom->getId(im, hh->eta(), sub, towerId);
	  if(flag_print) Printf("id=%d e=%f\n", towerId, dE);
	  //mcDe[towerId-1] += dE;
	  ////float ee = energy[towerId-1];
	////if(dE > 0) Printf("towerId = %d, dE = %f, ee = %f\n", towerId, dE, ee);
	//if(dE > 0) Printf("towerId = %d, dE = %f\n", towerId, dE);
	StMcTrack *trk = hh->parentTrack();
	if(!trk){ 
	  Printf("track not found for dE = %f\n", dE); 
	}
	else{
	  float pt = trk->pt();
	  float r = 0;
	  if(pt > 0) r = dE/pt;
          int ishw = trk->isShower();
          long pdg = trk->pdgId();
          long id = trk->geantId();
          long key = trk->key();
	  //if(id == 7 || id == 1) Printf("towerid = %d r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n, ", towerId, r, pt, ishw, pdg, id, key);
	  if(flag_print) Printf("id=%d e=%f pt=%f r=%f ishw=%d, pdg=%ld geantId=%ld key=%ld\n", towerId, dE, pt, r, ishw, pdg, id, key);
	}
      }
      */
	//detector hits
	StSPtrVecMcCalorimeterHit &dets = module->detectorHits();
	Printf("detector hit size: %d\n", dets.size());
	for(unsigned long ih = 0; ih < dets.size(); ih++){
	  StMcCalorimeterHit *hh = dets[ih];
	  if(!hh) continue;
	  float dE = hh->dE();
	  int towerId;
	  int sub = hh->sub();
	  //if(sub < 0) Print("sub = %d\n", sub);
	  emcGeom->getId(im, hh->eta(), sub, towerId);
	  //if(flag_print) Printf("detector hits: id=%d e=%f\n", towerId, dE);
	  //mcDe[towerId-1] += dE;
	  ////float ee = energy[towerId-1];
	  ////if(dE > 0) Printf("towerId = %d, dE = %f, ee = %f\n", towerId, dE, ee);
	  //if(dE > 0) Printf("towerId = %d, dE = %f\n", towerId, dE);

	  //test
	  float ped = emcTables->pedestal(1, towerId);
	  float rms = emcTables->pedestalRMS(1, towerId);
	  float calib = emcTables->calib(1, towerId);
	  //if(flag_print) 
	  Printf("id=%d ped=%f, rms=%f, calib=%f\n", towerId, ped, rms, calib);
	  //double f_photo = MipPhotoElectrons/MipEnergyDeposit;
	  //double n_photo = dE * f_photo;
	  float eta; emcGeom->getEta(im, hh->eta(), eta);
	  Printf("sub eta=%d, eta=%f\n", hh->eta(), eta);
	  double f_sample = samplingFraction(eta);
	  Printf("f_sample = %f\n", f_sample);
	  //StEmcPmtSimulator simu(kBarrelEmcTowerId, StEmcVirtualSimulator::kPrimarySecondaryFullMode);
	  StEmcPmtSimulator simu(kBarrelEmcTowerId, StEmcVirtualSimulator::kPrimaryOnlyMode);
	  simu.setEmbeddingMode(false);
	  simu.setTables(emcTables);
	  simu.setCalibScale(1.0);
	  simu.setCalibSpread(0);
	  simu.setMaximumAdc(4095);
	  simu.setMaximumAdcSpread(0);
	  
	  StEmcRawHit *raw = simu.makeRawHit(hh);
	  float ee = raw->energy();
	  h->Fill(dE, ee);
	  Printf("id=%d dE=%f ee=%f\n", towerId, dE, ee);
	  //StPmtSignal pmt;
	}
      }
      Printf("nhits=%d, sum=%d, nmodules=%d \n", nhits, nsum, nmodules);
      /*
	for(int it = 0; it < 4800; it++){
	float diff = energy[it] - mcDe[it];
	if(mcDe[it] > 0) Printf("towerId = %d, mu = %f, mc = %f, diff = %f\n", it+1, energy[it], mcDe[it], diff);
	}
      */
  }
  fout->Write();
  fout->Close();
}
