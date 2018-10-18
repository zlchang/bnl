//TFile *mFile;
const char* name[] = {"Reco", "Mc", "McPiP", "McPiM", "McKP", "McKM"};
const int Nn = 6;

int cmbTrackGeant(char *ptbin = "pt5_7",
		  char* dir_base = "output/MyTrackGeant",
		  char* mcfiletype = "run12.a.trackgeant",
		  int ver = 0)
{
  //  TH1::SetDefaultSumw2(kTRUE);
  //  gSystem->Load("StMyMatchTrackToEmcHist.so");
  //  gSystem->Load("StMyEmcFromGeantHist.so");

  gSystem->Load("StMyObjs.so");
  gSystem->Load("StMyTrackToGeantHist.so");
  
  TFile *fout = new TFile(Form("%s.%s.v%d.w.root", ptbin, mcfiletype, ver), "recreate");
  //
  StMyTrackToGeantHist *hist[Nn];
  for(int it = 0; it < Nn; it++){
    hist[it] = new StMyTrackToGeantHist(Form("Cmb%s", name[it]));
  }
  for(int ii = 0; ii < 10; ii++){
    double ww = 1;
    yFile = TFile::Open(Form("%s/%s/%s.%d.MyTrackGeant.root", dir_base, ptbin, ptbin, ii));
    
    cout<<Form("%s/%s/%s.MyTrackGeant.%d.root", dir_base, ptbin, ptbin, ii)<<endl;
    
    for(int it = 0; it < Nn; it++){
      StMyTrackToGeantHist *pp = hist[it];
      AddHist(pp->mMatchVsPt, Form("%sMatchVsPt", name[it]), ww);
      AddHist(pp->mQaVsPt, Form("%sQaVsPt", name[it]), ww);
      AddHist(pp->mPtMcVsMu, Form("%sPtMcVsMu", name[it]), ww);
      //
      AddHist(pp->mMatchVsEta, Form("%sMatchVsEta", name[it]), ww);
      AddHist(pp->mQaVsEta, Form("%sQaVsEta", name[it]), ww);
      AddHist(pp->mEtaMcVsMu, Form("%sEtaMcVsMu", name[it]), ww);
      //
      AddHist(pp->mDvZ, Form("%sDvZ", name[it]), ww);
    }
  }
  
  fout->Write();
  fout->Close();
  return 1;
}
