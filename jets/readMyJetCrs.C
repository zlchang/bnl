Int_t readMyJetCrs(const char* filelist="jets.list", const char* outfile="jets.crs.root")
{
  gSystem->Load("StJetEvent.so");
  gSystem->Load("StJetSkimEvent.so");
  gSystem->Load("StUeEvent.so");
  gSystem->Load("StMyDef.so");
  gSystem->Load("StMyHistUtil.so");
  gSystem->Load("StMyJetSpin.so");
  gSystem->Load("StMyJet.so");

  //Add jet tree into StJetEvent
  TChain jetchn("jet");
  //Add skim tree into StJetSkimEvent
  TChain skimchn("jetSkimTree");
  //Add ue tree
  //TChain uechn("ue");
  
  char jetfile[256];
  ifstream in(filelist);
  while(in >> jetfile){
    jetchn.Add(jetfile);    
    cout<<jetfile<<" "<<jetchn.GetEntries()<<endl; 

    TString skimfile(jetfile);
    skimfile.Replace(skimfile.Index("jets"),4,"skim");
    
    //TString dir(gSystem->DirName(jetfile));
    //cout<<dir<<endl;
    //dir.Replace(dir.Index("jet"),3,"skim");
    //TString skimfile(gSystem->BaseName(jetfile));
    //skimfile.Replace(skimfile.Index("jets"),4,"skim");
    //skimfile = dir + "/" + skimfile;
    
    skimchn.Add(skimfile);
    cout<<skimfile<<" "<<skimchn.GetEntries()<<endl;
    

    //TString uefile(jetfile);
    //uefile.Replace(uefile.Index("jets"),4,"ueoc");
    //uechn.Add(uefile);
    //cout<<uefile<<" "<<uechn.GetEntries()<<endl;
  }

  StJetEvent *jetevnt= new StJetEvent;
  //jetchn.SetBranchAddress("AntiKtR050NHits12_7pct",&jetevnt);
  jetchn.SetBranchAddress("AntiKtR050NHits12",&jetevnt);
  //StJetEvent *parevnt= new StJetEvent;
  //jetchn.SetBranchAddress("AntiKtR050Parton",&parevnt);
  //StJetEvent *particleevnt= new StJetEvent;
  //jetchn.SetBranchAddress("AntiKtR050Particle",&particleevnt);

  Int_t njetevnt = jetchn.GetEntries();

  cout<<njetevnt<<endl;
  
  StJetSkimEvent *skimevnt = new StJetSkimEvent;

  skimchn.SetBranchAddress("skimEventBranch",&skimevnt);
  Int_t nskimevnt = skimchn.GetEntries();
  cout<<nskimevnt<<endl;

  //StUeOffAxisConesEvent *ueocevnt = new StUeOffAxisConesEvent;
  //uechn.SetBranchAddress("AntiKtR050NHits12_7pctOffAxisConesR050", &ueocevnt);
  //uechn.SetBranchAddress("AntiKtR050NHits12OffAxisConesR050", &ueocevnt);
  //Int_t nueocevnt = uechn.GetEntries();

  //TFile *fout = new TFile(outfile, "recreate");
  ReaderCrs *reader = new ReaderCrs(jetevnt, 0x0, 0x0, skimevnt, 0x0, 0x0);
  //cout<<nueocevnt<<endl;

  assert(outfile);
  reader->SetOutput(outfile);
  reader->SetMC(false);
  reader->SetUE(true);
  reader->Init();
  for(Int_t iEvt = 0; iEvt < njetevnt; iEvt++)
  //for(Int_t iEvt = 0; iEvt < 10; iEvt++)
    {
      jetchn.GetEntry(iEvt);
      skimchn.GetEntry(iEvt);
      //uechn.GetEntry(iEvt);
      if(skimevnt->eventId() != jetevnt->eventId())// || ueocevnt->eventId() != jetevnt->eventId())
      {
         cout<<"skim:"<<skimevnt->eventId()<<" jet:"<<jetevnt->eventId()<<endl;//" ueoc:"<<ueocevnt->eventId()<<endl;  
         continue;
      }
      //if(iEvt == 308) continue;
      if(reader->Make(iEvt) == 0) continue;
    }
//  reader->getJetMC("JP0")->Write("JP0JetMC");
//  reader->getJetMC("JP1")->Write("JP1JetMC");
//  reader->getJetMC("JP2")->Write("JP2JetMC");
  reader->Finish();
//  fout->Write();
//  fout->Close();
  return 0;
}
