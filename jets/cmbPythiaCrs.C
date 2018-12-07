int cmbPythiaCrs(char *file = "ptbin.list", char* runlist = "text.full", 
	  char* dir_base = "pythia", char* mcfilepre = "run12.l.ue", 
	  int ver = 1, int first = 0)
{
  //gSystem->Load("libMyJetMC.so");
  gSystem->Load("StMyDef.so");
  gSystem->Load("StMyHistUtil.so");
  gSystem->Load("StMyJetSpin.so");
  ifstream in(file);
  char ptbin[16];
  const int Ntrg = 3;
  //  const int Nver = 2;
  char *trg[Ntrg] = {"JP0", "JP1", "JP2"};

  //  char dir_base[] = "output/mc";
  //  char dir[128];

  TFile *fout = new TFile(Form("%s.%s.v%d.w.cmb.pmc.root", file, mcfilepre, ver), "recreate");
  //test
  const int NX = 150;
  double xx[NX+1];
  for(int ibin = 0; ibin <= NX; ibin++){
    xx[ibin] = TMath::Power(10, -3+3*(ibin+0.)/(NX+0.));
  }
  //TH1F *hxg[yNPtBins];
  //for(int ib = 0; ib < yNPtBins; ib++){
    //hxg[ib] = new TH1F(Form("newXgCmbBin%d", ib), ";x_{g}", NX, xx);
  //}
  //
  
  //MyJetPar *jetparton = new MyJetPar("Parton");
  //MyJetPar *jetparticle = new MyJetPar("Particle");
  //yield
  MyJetYieldPar *jetyldparton = new MyJetYieldPar("Parton");
  MyJetYieldPar *jetyldparticle = new MyJetYieldPar("Particle");
  //
  TFile *finfo = TFile::Open(Form("analyze_logs/ptbin.info.%s.root", runlist));
  TH1F *hweight = (TH1F *)finfo->Get("weight");
  double weight[13];
  for(int ipt = 0; ipt < 13; ipt++){
    weight[ipt] = hweight->GetBinContent(ipt+1);
    //weight[ipt] = 1.;
  }
  finfo->Close();
  float fudge[13];
  ifstream in_fudge(Form("fudge/fudge.%s.%s.rebin.5.txt", file, runlist));
  if(!in_fudge) cout<<"can't not open fudge factor file"<<endl;
  float ptmin, ptmax, ff, ffee;
  int counter = 0;
  while(in_fudge >> ptmin >> ptmax >> ff >> ffee){
    cout<<ptmin<<" "<<ff<<endl;
    //if(counter <= 4)
      fudge[counter] = ff;
    //else fudge[counter] = 1.;
    counter++;
  }
  in_fudge.close();
  counter = 0;
  //
  while(in >> ptbin){
    //if(counter == 0) { counter++; continue; }
    float ww = fudge[counter]*weight[counter];
    for(int ii = 0; ii < 5; ii++){
      TFile *fin = TFile::Open(Form("%s/%s/%s.%d.crs.root", dir_base, ptbin, ptbin, ii));
      cout<<Form("%s/%s/%s.%d.crs.root", dir_base, ptbin, ptbin, ii)<<endl;
//      TFile *fin = TFile::Open(Form("%s/%s/%s.%s.mc.root", dir_base, mcfilepre, mcfilepre, ptbin));
//      cout<<Form("%s/%s/%s.%s.mc.root", dir_base, mcfilepre, mcfilepre, ptbin)<<endl;
      //MyJetPar *pa = (MyJetPar *)fin->Get("PartonMc");
      //jetparton->Add(pa, ww);

      //MyJetPar *pb = (MyJetPar *)fin->Get("ParticleMc");
      //jetparticle->Add(pb, ww);
      //yield
      MyJetYieldPar *pya = (MyJetYieldPar *)fin->Get("YieldPartonMc");
      jetyldparton->Add(pya, ww);

      MyJetYieldPar *pyb = (MyJetYieldPar *)fin->Get("YieldParticleMc");
      jetyldparticle->Add(pyb, ww);
    
      //for(int ib = 0; ib < yNPtBins; ib++){
	//TH1F *hh = (TH1F*)fin->Get(Form("newXgBin%d", ib));
	//hxg[ib]->Add(hh, ww);
      //}
      fin->Close();
    }
    cout<<counter<<" "<<ptbin<<" w: "<<weight[counter]<<" f: "<<fudge[counter]<<" wf: "<<ww<<endl;
    counter++;
  }

  fout->Write();
  fout->Close();
  return 1;
}
