const unsigned long nevents = 100;
const double MipPhotoElectrons = 63;
const double MipEnergyDeposit = 0.0198;
double mSF[3] = {14.356, -0.512, 0.668};
double samplingFraction(double *x, double *par){
  double xx = TMath::Abs(x[0]);
  return 1./(par[0]+par[1]*xx+par[2]*xx*xx);
}
/*
  class myTower{//: public TObject{
  public:
  float dE;
  float eta;
  float e;
  float id;
  };
  class myTrack{// : public TObject{
public:
  float pt;
  long id;
  long geantId;
  vector<myTower> towers;
};
*/
void testPion(const char* geantfile = "test.list",
	      const char* ver = "v1"
	      )
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
  StFile *fileset = new StFile;
  const char file[256];
  ifstream in(geantfile);
  while(in >> file){
    TString str(file);
    str.ReplaceAll(".MuDst.root", ".geant.root");
    fileset->AddFile(str.Data());
    Printf("%s\n", file);
  }

  // GEANT reader
  StIOMaker* ioMaker = new StIOMaker;
  ioMaker->SetFile(geantfile);
  ioMaker->SetFileSet(fileset);
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

  //TFile *fout = new TFile(Form("simu.emc.%s.root", ver), "recreate");

  TF1 *f1 = new TF1("samplingFraction", samplingFraction, -2, 2, 3);
  f1->SetParameters(mSF);

  chain->Init();
  
  for (int iEvent = 0; iEvent < nevents; ++iEvent) {
    chain->Clear();
    
    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    emcTables->loadTables(chain);
    
    //print flag
    bool flag_print = (iEvent%100 == 0);
    // Get StMcEvent
    StMcEvent* mcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mcEvent) continue;
    unsigned long eid = mcEvent->eventNumber();
    
    if(eid != 37865) continue;
    float vX = mcEvent->primaryVertex()->position().x(); 
    float vY = mcEvent->primaryVertex()->position().y(); 
    float vZ = mcEvent->primaryVertex()->position().z(); 
    Printf("vX=%f, vY=%f, vZ=%f\n", vX, vY, vZ);
    
    StSPtrVecMcTrack mTracks = mcEvent->tracks();
    for(int igt = 0; igt < mTracks.size(); igt++){
      StMcTrack *trk_m = mTracks[igt];
      long id_m = trk_m->geantId();
      float pt_m = trk_m->pt();
      float eta_m = trk_m->pseudoRapidity();
      long key_m = trk_m->key();
      //double q_m = trk_m->particleDefinition()->charge();
      
      if(id_m != 0){
	Printf("id_m=%d key=%d pt_m=%f eta_m=%f\n", id_m, key_m, pt_m, eta_m);
	
	StMcVertex *startv = trk_m->startVertex();
	float startvx = startv->position().x();
	float startvy = startv->position().y();
	float startvz = startv->position().z();
	Printf("start x=%f, y=%f, z=%f\n", startvx, startvy, startvz);

	StMcVertex *stopv = trk_m->stopVertex();
	if(stopv){
	  float stopvx = stopv->position().x();
	  float stopvy = stopv->position().y();
	  float stopvz = stopv->position().z();
	  Printf("stop x=%f, y=%f, z=%f\n", stopvx, stopvy, stopvz);
	}

	StMcTrack *prnt = trk_m->parent();
	if(prnt){
	  long key_p = prnt->key();
	  Printf("parent key=%d\n", key_p);
	}

	StPtrVecMcCalorimeterHit hits = trk_m->bemcHits();
	
	float sdE = 0;
	for(int ih = 0; ih < hits.size(); ih++){
	  StMcCalorimeterHit *hit = hits[ih];
	  float dE = hit->dE();
	  double f_sample = f1->Eval(0);
	  //Printf("sf=%f\n", f_sample);
	  //return;
	  //dE = dE/f_sample;
	  sdE += dE/f_sample;
	  Printf("ih=%d, dE=%f, sum=%f\n", ih, dE, sdE);
	}
      }
    }
  }
  //  fout->Write();
  // fout->Close();
}
