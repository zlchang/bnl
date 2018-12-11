const unsigned long nevents = 10000;
/*double samplingFraction(double *x, double *par){
  double xx = TMath::Abs(x[0]);
  return 1./(par[0]+par[1]*xx+par[2]*xx*xx);
}
*/
void MyReadEmcGeant(//const char* mudstfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root",
	     const char* geantfile = "test.emb.list",///global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.geant.root"
             const char* outfile = "emc.geant.root")
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  //gROOT->Macro("LoadJetMaker.C");

  ///*
  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StEEmcUtil");
  
  gSystem->Load("StEmcRawMaker");
  gSystem->Load("StEmcADCtoEMaker");
  
  gSystem->Load("StDbBroker");
  gSystem->Load("St_db_Maker");	
  //*/
  //emc simulator
  gSystem->Load("StEmcSimulatorMaker");
  gSystem->Load("StMyObjs");
  gSystem->Load("StMyUtils");
  //gSystem->Load("StMyMatchTrackToEmcHist");
  gSystem->Load("StMyEmcFromGeantHist");
  gSystem->Load("StMyEmcFromGeantMaker");
  // Create chain
  //
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
  in.close();
  // GEANT reader
  StIOMaker* ioMaker = new StIOMaker;
  //ioMaker->SetFile(geantfile);
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
  StMyTrackProjEmcPtRadius *proj = new StMyTrackProjEmcPtRadius;
  StMyEmcFromGeantMaker *emcGeant = new StMyEmcFromGeantMaker;
  emcGeant->SetOutfile(outfile);
  emcGeant->addVertexCut(new StMyMcVertexZCut);
  emcGeant->setTrackProjEmc(proj);

  chain->Init();
  
  for (int iEvent = 0; iEvent < nevents; ++iEvent) {
    chain->Clear();    
    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;
  }
  chain->Finish();
}
