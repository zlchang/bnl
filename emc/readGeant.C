const unsigned long nevents = 10000;
void readGeant(const char* geantfile = "gtrack.geant.root")
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

    float vZ = mcEvent->primaryVertex()->position().z();
    
    Printf("vZ=%f\n", vZ);

    StMcEmcHitCollection *emcHits = mcEvent->bemcHitCollection();
    unsigned long nhits = emcHits->numberOfHits();
    unsigned int nmodules = emcHits->numberOfModules();

    unsigned long nsum = 0;
    for(unsigned int im = 0; im < nmodules; im++){
      StMcEmcModuleHitCollection *module = emcHits->module(im+1);
      if(!module) { Printf("module %d not found\n", im); continue; }
      unsigned long nh = module->numberOfHits();
      //unsigned long ndh = module->numberOfDetectorHits();
      //if(nh != ndh) Printf("hn = %d, hdh = %d \n", nh, ndh);
      nsum += nh;

      StSPtrVecMcCalorimeterHit &hits = module->hits();
      //Printf("size: %d\n", hits.size());
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
	  Printf("r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n", r, pt, ishw, pdg, id, key);
	}
      }
    }
    Printf("nhits=%d, sum=%d, nmodules=%d \n", nhits, nsum, nmodules);
  }
}
