int cmbCrs(char *file = "run12.final.v2.list", 
	  char* dir_base = "output", 
	  char* ver = "ue.v1.0")
{
  gSystem->Load("StMyDef.so");
  gSystem->Load("StMyHistUtil.so");
  gSystem->Load("StMyJetSpin.so");
  ifstream in(file);
  char run[16];
  //char ptbin[16];
  const int Ntrg = 3;
  //  const int Nver = 2;
  char *trg[Ntrg] = {"JP0", "JP1", "JP2"};

  //  char dir_base[] = "output/mc";
  //  char dir[128];

  TFile *fout = new TFile(Form("%s.%s.crs.root", file, ver), "recreate");

  //MyJetMC *jetmc[Ntrg];
  MyJetYield *jetyld[Ntrg];
  MyJetTest *jettest[Ntrg];
  for(int itrg = 0; itrg < Ntrg; itrg++){
    //jetmc[itrg] = new MyJetMC(trg[itrg]);
    jetyld[itrg] = new MyJetYield(trg[itrg]);
    jettest[itrg] = new MyJetTest(Form("Test%s", trg[itrg]));
  }
  MyJetYield *jetyldcmb = new MyJetYield("Cmb");
  MyJetTest *jettestcmb = new MyJetTest("TestCmb");
  //
  MyJetQA *jetqa[Ntrg];
  for(int itrg = 0; itrg < Ntrg; itrg++){
    jetqa[itrg] = new MyJetQA(trg[itrg]);
  }
  
//  MyJetPar *jetparton = new MyJetPar("parton");
//  MyJetPar *jetparticle = new MyJetPar("particle");
//  MyJetYieldPar *jetyldparton = new MyJetYieldPar("Parton");
//  MyJetYieldPar *jetyldparticle = new MyJetYieldPar("Particle");
//  TFile *finfo = TFile::Open(Form("analyze_logs/ptbin.info.%s.root", runlist));
//  TH1F *hweight = (TH1F *)finfo->Get("weight");
//  double weight[13];
//  for(int ipt = 0; ipt < 13; ipt++){
//    weight[ipt] = hweight->GetBinContent(ipt+1);
    //weight[ipt] = 1.;
//  }
//  finfo->Close();
//  float fudge[13];
//  ifstream in_fudge(Form("fudge/fudge.%s.%s.rebin.5.txt", file, runlist));
//  if(!in_fudge) cout<<"can't not open fudge factor file"<<endl;
//  float ptmin, ptmax, ff, ffee;
  int counter = 0;
//  while(in_fudge >> ptmin >> ptmax >> ff >> ffee){
//    cout<<ptmin<<" "<<ff<<endl;
    //if(counter <= 4)
//      fudge[counter] = ff;
    //else fudge[counter] = 1.;
//    counter++;
//  }
//  in_fudge.close();
//  counter = 0;

  while(in >> run){
    //if(counter == 0) { counter++; continue; }
    //float ww = fudge[counter]*weight[counter];
    //
    ifstream inps(Form("prescales/lists/%s.ps.txt", run));
    cout<<Form("loading prescales: prescales/lists/%s.ps.txt", run)<<endl;
    double ps[Ntrg];
    for(int itrg = 0; itrg < Ntrg; itrg++) ps[itrg] = 1.0;
    for(int itrg = 0; itrg < Ntrg; itrg++){
      if(!(inps >> (ps[itrg]))) break;
      cout<<Form("trg %d: prescale: %lf", itrg, ps[itrg])<<endl;
    } 
    inps.close();
    //
    double ww = 1.0;
    for(int ii = 0; ii < 3; ii++){
      //if(ii%2 == 1) continue;
      TFile *fin = TFile::Open(Form("%s/%s/%s.%d.crs.root", dir_base, run, run, ii));
      cout<<Form("%s/%s/%s.%d.crs.root", dir_base, run, run, ii)<<endl;
//      TFile *fin = TFile::Open(Form("%s/%s/%s.%s.mc.root", dir_base, mcfilepre, mcfilepre, ptbin));
//      cout<<Form("%s/%s/%s.%s.mc.root", dir_base, mcfilepre, mcfilepre, ptbin)<<endl;
      for(int itrg = 0; itrg < Ntrg; itrg++){
        //if(counter <= 1 && itrg >= 1) continue;
        //MyJetMC *pmc = (MyJetMC *)fin->Get(Form("%sJetMc", trg[itrg]));
        //pmc->GetJetMC()->Print();
        //jetmc[itrg]->Add(pmc, ww);
        //yield
        MyJetYield *pyld = (MyJetYield *)fin->Get(Form("%sJetYield", trg[itrg]));
        jetyld[itrg]->Add(pyld, ww);
        //combine with prescales
        jetyldcmb->Add(pyld, ww*ps[itrg]);
        delete pyld;
        //
        MyJetTest *ptst = (MyJetTest *)fin->Get(Form("%sJetTest", trg[itrg]));
        jettest[itrg]->Add(ptst, ww);
        //combine with prescales
        jettestcmb->Add(ptst, ww*ps[itrg]);
        delete ptst;
      }
      for(int itrg = 0; itrg < Ntrg; itrg++){
        MyJetQA *pqa = (MyJetQA *)fin->Get(Form("%sJetQA", trg[itrg]));
        jetqa[itrg]->Add(pqa, ww);
        delete pqa;
      }
      //MyJetPar *pa = (MyJetPar *)fin->Get("JetMcParton");
      //jetparton->Add(pa, ww);

      //MyJetPar *pb = (MyJetPar *)fin->Get("JetMcParticle");
      //jetparticle->Add(pb, ww);
      //cmb
      //MyJetYield *pyldcmb = (MyJetYield *)fin->Get(Form("%sJetYield", "Cmb"));
      //jetyldcmb->Add(pyldcmb, ww);
      //cmb
      //MyJetTest *ptstcmb = (MyJetTest *)fin->Get(Form("%sJetTest", "Cmb"));
      //jettestcmb->Add(ptstcmb, ww);
      //MyJetYieldPar *pya = (MyJetYieldPar *)fin->Get("YieldPartonMc");
      //jetyldparton->Add(pya, ww);

      //MyJetYieldPar *pyb = (MyJetYieldPar *)fin->Get("YieldParticleMc");
      //jetyldparticle->Add(pyb, ww);
      
      fin->Close();
    }
    //cout<<counter<<" "<<ptbin<<" w: "<<weight[counter]<<" f: "<<fudge[counter]<<" wf: "<<ww<<endl;
    cout<<counter<<" "<<run<<endl;
    counter++;
  }

  fout->Write();
  fout->Close();
  return 1;
}
