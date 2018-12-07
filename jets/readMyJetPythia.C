Int_t readMyJetPythia(const char* filelist="test.pyjets.list", const char* outfile="test.py.root")
{
  gSystem->Load("StJetEvent.so");
  gSystem->Load("StJetSkimEvent.so");
  //gSystem->Load("StUeEvent.so");
  //gSystem->Load("StMyJetDef.so");
  gSystem->Load("StMyJetSpin.so");
  gSystem->Load("StMyJet.so");
  gSystem->Load("StMyDef.so");

  //Add jet tree into StJetEvent
  TChain jetchn("jet");
  //Add skim tree into StJetSkimEvent
  TChain pychn("PythiaTree");
  //Add ue tree
  //TChain uechn("ue");
  
  char jetfile[256];
  ifstream in(filelist);
  while(in >> jetfile){
    jetchn.Add(jetfile);    
    cout<<jetfile<<" "<<jetchn.GetEntries()<<endl; 

    TString pyfile(jetfile);
    pyfile.Replace(pyfile.Index("jets"),4,"asym");
    pychn.Add(pyfile);
//    TString dir(gSystem->DirName(jetfile));
//    dir.Replace(dir.Index("pythia"),6,"pythia/skim");
//    TString pyfile(gSystem->BaseName(jetfile));
//    pyfile.Replace(pyfile.Index("jets"),4,"asym");
//    pyfile = dir + "/" + pyfile;
//    pychn.Add(pyfile);

    cout<<pyfile<<" "<<pychn.GetEntries()<<endl;

//    TString uefile(jetfile);
//    uefile.Replace(uefile.Index("jets"),4,"ueoc");
//    uechn.Add(uefile);
//    cout<<uefile<<endl;
  }

  StJetEvent *parevnt= new StJetEvent;
  jetchn.SetBranchAddress("AntiKtR050Parton",&parevnt);
  StJetEvent *particleevnt= new StJetEvent;
  jetchn.SetBranchAddress("AntiKtR050Particle",&particleevnt);

  Int_t njetevnt = jetchn.GetEntries();

  cout<<njetevnt<<endl;
  
  StPythiaEvent *pyevnt = new StPythiaEvent;

  pychn.SetBranchAddress("PythiaBranch",&pyevnt);
  Int_t npyevnt = pychn.GetEntries();
  cout<<npyevnt<<endl;

//  StUeOffAxisConesEvent *ueocevnt = new StUeOffAxisConesEvent;
//  uechn.SetBranchAddress("AntiKtR050NHits12OffAxisConesR050", &ueocevnt);
//  Int_t nueocevnt = uechn.GetEntries();
//  cout<<nueocevnt<<endl;

  //TFile *fout = new TFile(outfile, "recreate");
  ReaderPythia *reader = new ReaderPythia(parevnt, particleevnt, pyevnt);

  assert(outfile);
  reader->SetOutput(outfile);
  reader->Init();
  //test
  double ptbins[yNPtBins] = {0., 0.54, 0.41, 1.04, 1.21, 1.30, 1.52, 2.47, 2.88,
			     3.14, 3.30, 3.56, 3.72, 4.26, 4.67};
  const int NX = 150;
  double xx[NX+1];
  for(int ibin = 0; ibin <= NX; ibin++){
    xx[ibin] = TMath::Power(10, -3+3*(ibin+0.)/(NX+0.));
  }
  TH1F *hxg[yNPtBins];
  for(int ib = 0; ib < yNPtBins; ib++){
     hxg[ib] = new TH1F(Form("newXgBin%d", ib), ";x_{g}", NX, xx);
  }
  //
  
  for(Int_t iEvt = 0; iEvt < njetevnt; iEvt++)
    {
      jetchn.GetEntry(iEvt);
      pychn.GetEntry(iEvt);
      //uechn.GetEntry(iEvt);
      //if(pyevnt->eventId() != parevnt->eventId() || ueocevnt->eventId() != jetevnt->eventId()) continue;
      if(pyevnt->eventId() != parevnt->eventId() || parevnt->eventId() != particleevnt->eventId()) continue;
      if(reader->Make(iEvt) == 0) continue;
      //test
      float pt = pyevnt->pt();
      float wgt = partonicWeight(pt);
      float x1 = pyevnt->x1();
      float x2 = pyevnt->x2();
      int flavor1 = pyevnt->particle(4)->GetPdgCode();
      flavor1 = (flavor1 == 21) ? 0 : flavor1;
      int flavor2 = pyevnt->particle(5)->GetPdgCode();
      flavor2 = (flavor2 == 21) ? 0 : flavor2;
      int NJets = parevnt->vertex()->numberOfJets();
      for(int iJet = 0; iJet < NJets; iJet++){
             StJetCandidate *jetcnd = parevnt->vertex()->jet(iJet);
             float pt = jetcnd->pt();
             float eta = jetcnd->eta();
             if(eta > -0.9 && eta < 0.9){
                for(int ib = 0; ib < yNPtBins; ib++){
                   if(pt >= yPtBins[ib]+ptbins[ib] && pt < yPtBins[ib+1]+ptbins[ib]){
                      //cout<<ib<<" "<<pt<<endl;
                      float waLL = pyevnt->partonALL();//aLLNNPDF(0);
                       //cout<<waLL<<endl;
                      if(flavor1 == 0) hxg[ib]->Fill(x1, waLL*wgt);
                      if(flavor2 == 0) hxg[ib]->Fill(x2, waLL*wgt);
                   }
                }
             }
       }
    }
  reader->Finish();
  return 0;
}
