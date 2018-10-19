const unsigned long nevents = 100;
void readGeant(const char* geantfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.geant.root")
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StEEmcUtil");
	
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

  chain->Init(); 
  for (int iEvent = 1; iEvent <= nevents; ++iEvent) {
    chain->Clear();

    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    // Get StMcEvent
    StMcEvent* mcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mcEvent) continue;

    StMcVertex *vertex = mcEvent->primaryVertex();
    float vZ = vertex->position().z();
    
    Printf("vZ=%f\n", vZ);
    int nprim = mcEvent->numberOfPrimaryTracks();
    int ndgts = vertex->numberOfDaughters();
    int ntrks = mcEvent->tracks().size();
    int nfnl = mcEvent->eventGeneratorFinalStateTracks();
    Printf("nprim=%d, ndgts=%d, ntrks=%d, nfnl=%d\n", nprim, ndgts, ntrks, nfnl);

    if(ndgts == nprim) continue;
    StPtrVecMcTrack& daughters = vertex->daughters();
    for(int idt = 0; idt < daughters.size(); idt++){
       StMcTrack *dtrk = daughters[idt];
       long dgid = dtrk->geantId();
       float dpt = dtrk->pt();
       float deta = dtrk->momentum().pseudoRapidity();
       unsigned int dntpc = dtrk->tpcHits().size();
       Printf("did=%d dpt=%f deta=%f dgid=%d dntpc=%d\n", idt, dpt, deta, dgid, dntpc);
    }

    StPtrVecMcTrack& tracks = mcEvent->tracks();
    int counter = 0;
    int countertpc = 0;
    for(int imt = 0; imt < tracks.size(); imt++){
      StMcTrack *mtrk = tracks[imt];

      StMcVertex *mvtx = mtrk->startVertex();
      if(mvtx != vertex) continue;
      long mgid = mtrk->geantId();
      float mpt = mtrk->pt();
      float meta = mtrk->momentum().pseudoRapidity();
      unsigned int mntpc = mtrk->tpcHits().size();
      if(mntpc > 0) countertpc++;
      counter++;
      Printf("mid=%d mpt=%f meta=%f mgid=%d mntpc=%d\n", imt, mpt, meta, mgid, mntpc);
    }
    Printf("%d tracks counted tracks with tpc hits: %d\n", counter, countertpc);
    /*
    StMcEmcHitCollection *emcHits = mcEvent->bemcHitCollection();
    unsigned long nhits = emcHits->numberOfHits();
    unsigned int nmodules = emcHits->numberOfModules();

    unsigned long nsum = 0;
    for(unsigned int im = 0; im < nmodules; im++){
      StMcEmcModuleHitCollection *module = emcHits->module(im+1);
      if(!module) { Printf("module %d not found\n", im); continue; }
      unsigned long nh = module->numberOfHits();
      unsigned long ndh = module->numberOfDetectorHits();
      if(nh != ndh) Printf("hn = %d, hdh = %d \n", nh, ndh);
      nsum += nh;

      StSPtrVecMcCalorimeterHit &hits = module->hits();
      Printf("size: %d\n", hits.size());
      for(unsigned long ih = 0; ih < hits.size(); ih++){
	StMcCalorimeterHit *hh = hits[ih];
	if(!hh) continue;
	float dE = hh->dE();
	if(dE > 0) Printf("dE = %f\n", dE);
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
	  if(id == 7 || id == 1) Printf("r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n, ", r, pt, ishw, pdg, id, key);
	}
      }
    }
    Printf("nhits=%d, sum=%d, nmodules=%d \n", nhits, nsum, nmodules);
    */
  }
}
