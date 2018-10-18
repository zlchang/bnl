class StMcEvent;

const unsigned long nevents = 100000;
StMcEvent *mMcEvent;	

TH1D *mHsf;
TFile *mFile;
void readFzd(const char* fzdfile = "kinematics.starsim.em.fzd")
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gSystem->Load("StTpcDb");
  gSystem->Load("St_g2t");
  gSystem->Load("St_geant_Maker");
  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StEEmcUtil");

  // Create chain
  StChain* chain = new StChain;
  //
  //     // GEANT reader
  /*StIOMaker* ioMaker = new StIOMaker;
  ioMaker->SetFile(geantfile);
  ioMaker->SetIOMode("r");
  ioMaker->SetBranch("*",0,"0");             // Deactivate all branches
  ioMaker->SetBranch("geantBranch",0,"r");   // Activate geant branch
  */

  St_geant_Maker* geant = new St_geant_Maker;
  geant->SetInputFile(fzdfile);

  StEmcGeom *emcGeom = StEmcGeom::instance("bemc"); 
  // Monte Carlo event maker
  StMcEventMaker* mcEventMaker = new StMcEventMaker;
  mcEventMaker->doPrintEventInfo  = false;
  mcEventMaker->doPrintMemoryInfo = false;

  TString outfile(fzdfile);
  outfile.ReplaceAll(".fzd", ".qa.root");
  mFile = new TFile(outfile.Data(), "recreate");
  mHsf = new TH1D("hsf", ";sf; counts", 40, -0.1, 0.3);
  /*
  TProfile *hsfeta = new TProfile("hsfeta", ";sf; counts", 60, -3.0, 3.0);//, 150, -0.1, 1.4);

  TH1D *hsf_sum = new TH1D("hsfsum", ";sf; counts", 40, -0.1, 0.3);
  TProfile *hsfeta_sum = new TProfile("hsfetasum", ";sf; counts", 60, -3.0, 3.0);//, 150, -0.1, 1.4);
  */
  chain->Init(); 
  for (int iEvent = 1; iEvent <= nevents; ++iEvent) {
    chain->Clear();

    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    // Get StMcEvent
    mMcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mMcEvent) continue;

    float vZ = mMcEvent->primaryVertex()->position().z();
    Printf("vZ=%f\n", vZ);

    readEmc();
    ///*
    //*/
  }
  mFile->Write();
  mFile->Close();
}
int readEmc()
{
  StMcEmcHitCollection *emcHits = mMcEvent->bemcHitCollection();
  unsigned long nEmcHits = emcHits->numberOfHits();
  unsigned int nmodules = emcHits->numberOfModules();
  unsigned long nsum = 0;
  float sumdE = 0;
  for(unsigned int im = 0; im < nmodules; im++){
    StMcEmcModuleHitCollection *module = emcHits->module(im+1);
    if(!module) { Printf("module %d not found\n", im); continue; }
    unsigned long nh = module->numberOfHits();
    //unsigned long ndh = module->numberOfDetectorHits();
    //if(nh != ndh) Printf("hn = %d, hdh = %d \n", nh, ndh);
    nsum += nh;
    if(nh == 0) continue;
    //Printf("nh=%d, size: %d\n", nh, module->hits().size());
    StSPtrVecMcCalorimeterHit hits = module->hits();
    
    for(unsigned long ih = 0; ih < nh; ih++){
      StMcCalorimeterHit *hh = hits[ih];
      if(!hh) continue;
      float dE = hh->dE();
      Printf("dE = %f\n", dE);
      sumdE += dE;
      //StMcTrack *trk = hh->parentTrack();
      //if(!trk){ Printf("track not found for dE = %f\n", dE); }
      //else{
      //  float pt = trk->pt();
      //  float r = 0;
      //  if(pt > 0) r = dE/pt;
      //  int ishw = trk->isShower();
      //  long pdg = trk->pdgId();
      //  long id = trk->geantId();
      //  long key = trk->key();
      //  Printf("r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n", r, pt, ishw, pdg, id, key);
      //}
    }
    
    //*/
    
  }
  Printf("sumdE=%f, nEmcHits=%d, sum=%d, nmodules=%d \n", sumdE, nEmcHits, nsum, nmodules);

  float pt_track = 0;
  float mom_track = 0;
  float eta_track = 0;    

  const StSPtrVecMcTrack &tracks = mMcEvent->tracks();
  for(unsigned int it = 0; it < tracks.size(); it++){
      StMcTrack *trk = tracks[it];
      float pt = trk->pt();
      float mom = trk->momentum().mag();
      float eta = trk->pseudoRapidity();
      int ishw = trk->isShower();
      long pdg = trk->pdgId();
      long id = trk->geantId();
      long key = trk->key();
      if(key != 1) continue;
      Printf("pt=%f eta=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n", pt, eta, ishw, pdg, id, key);
      //if(ishw == 1) continue;	 
      StMcVertex *startv = trk->startVertex();
      float startVz = startv->position().z(); 
      Printf("startVx=%lf\n", startVz);
      StMcTrack *parent = trk->parent();
      if(parent) continue;
      //Printf("parent id = %u\n", parent->geantId());
      pt_track = pt;
      mom_track = mom;
      eta_track = eta;
  }
  mHsf->Fill(sumdE/mom_track);
  //*/
  //hsf_sum->Fill(sumdE/pt_track);
  //hsfeta_sum->Fill(eta_track, sumdE/pt_track);
  return 1;
}
int readTrack()
{
   float pt_track = 0;
   float eta_track = 0;    
   const StSPtrVecMcTrack &tracks = mMcEvent->tracks();
   for(unsigned int it = 0; it < tracks.size(); it++){
     StMcTrack *trk = tracks[it];
     float pt = trk->pt();
     float eta = trk->pseudoRapidity();
     int ishw = trk->isShower();
      long pdg = trk->pdgId();
      long id = trk->geantId();
      long key = trk->key();
      Printf("pt=%f eta=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n", pt, eta, ishw, pdg, id, key);
      //if(ishw == 1) continue;	 
      //if(id != 1) continue;
      StMcVertex *startv = trk->startVertex();
      float startVz = startv->position().z(); 
      Printf("startVx=%lf\n", startVz);
      //if(startVz > 0) continue;
      StSPtrVecMcCalorimeterHit &hits = trk->bemcHits();
      float sum = 0;
      unsigned int nhits = hits.size();
      Printf("bemc hit size: %d\n", nhits);
      for(unsigned long ih = 0; ih < nhits; ih++){
	StMcCalorimeterHit *hh = hits[ih];
	if(!hh) continue;
	float dE = hh->dE();
	sum += dE;
	Printf("hit=%d dE = %f\n", ih, dE);
      }
      Printf("dE_sum=%f\n", sum);
      //if(nhits){
      //hsf->Fill(sum/pt);
      //hsfeta->Fill(eta, sum/pt);
      //}
      StMcTrack *parent = trk->parent();
      if(parent) continue;
      //Printf("parent id = %u\n", parent->geantId());
      pt_track = pt;
      eta_track = eta;
    }
    return 1;
}
