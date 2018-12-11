void MyTrackGeant(
const char* MuDst = "test.emb.list",
const char* Outfile = "test.track.root"
//const char* Outfile = "MyMatchTrackToEmc.root"
)
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gROOT->Macro("LoadJetMaker.C");

  gSystem->Load("StMyObjs");
  gSystem->Load("StMyUtils");
  gSystem->Load("StMyTrackToGeantHist");
  gSystem->Load("StMyTrackToGeantMaker");

  StChain *chain = new StChain;

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",MuDst,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");

  StFile *fileset = new StFile;
  const char file[256];
  ifstream in(MuDst);
  while(in >> file){
    TString str(file);
    str.ReplaceAll(".MuDst.root", ".geant.root");
    fileset->AddFile(str.Data());
    Printf("%s\n", file);
  }
  in.close();

  StIOMaker* ioMaker = new StIOMaker;
  //ioMaker->SetFile(geantfile);
  ioMaker->SetFileSet(fileset);
  ioMaker->SetIOMode("r");
  ioMaker->SetBranch("*",0,"0");             // Deactivate all branches
  ioMaker->SetBranch("geantBranch",0,"r");   // Activate geant branch

  // Monte Carlo event maker
  StMcEventMaker* mcEventMaker = new StMcEventMaker;
  mcEventMaker->doPrintEventInfo  = false;
  mcEventMaker->doPrintMemoryInfo = false;

  StMyTrackToGeantMaker *geantMaker = new StMyTrackToGeantMaker;
  //matchMaker->addVertexCut(new StMyVertexZCut(30.));
  geantMaker->SetOutfile(Outfile);
  chain->Init();
  //for(int iEvent = 0; iEvent < 2; iEvent++)
  for(int iEvent = 0; iEvent < 10000; iEvent++)
    {
      chain->Clear();
      int status = chain->Make(iEvent);
      if(status == kStSkip) continue;
      if(status % 10 == kStEOF || status % 10 == kStFatal) break;
    }
  //chain->Finish();
}
