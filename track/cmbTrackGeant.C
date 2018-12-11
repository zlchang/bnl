const char* name[] = {"Reco", "Mc", "McPiP", "McPiM", "McKP", "McKM"};
const int Nn = 6;

int cmbTrackGeant(char *ptbin = "pt5_7",
		  char* dir_base = "output/trackGeant_eta0.9",
		  char* mcfiletype = "run12.b.trackGeant",
		  int ver = 1)
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
    yFile = TFile::Open(Form("%s/%s/%s.trackGeant.%d.root", dir_base, ptbin, ptbin, ii));
    
    cout<<Form("%s/%s/%s.trackGeant.%d.root", dir_base, ptbin, ptbin, ii)<<endl;
    
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
      AddHist(pp->mMatchVsPhi, Form("%sMatchVsPhi", name[it]), ww);
      AddHist(pp->mQaVsPhi, Form("%sQaVsPhi", name[it]), ww);
      AddHist(pp->mPhiMcVsMu, Form("%sPhiMcVsMu", name[it]), ww);
      //
      AddHist(pp->mNHitsMcVsMu, Form("%sNHitsMcVsMu", name[it]), ww);
      //
      AddHist(pp->mDvZ, Form("%sDvZ", name[it]), ww);
      AddHist(pp->mMcTrack->mHist, Form("Mc%sTrack", name[it]), ww);
      AddHist(pp->mMcTrack->mHistPhi, Form("Mc%sTrackPhi", name[it]), ww);
      AddHist(pp->mMcTrack->mHistEta, Form("Mc%sTrackEta", name[it]), ww);
      AddHist(pp->mMcTrack->mHistNHits, Form("Mc%sTrackNHits", name[it]), ww);
      //
      AddHist(pp->mTrack->mHist, Form("%sTrack", name[it]), ww);
      AddHist(pp->mTrack->mHistPhi, Form("%sTrackPhi", name[it]), ww);
      AddHist(pp->mTrack->mHistEta, Form("%sTrackEta", name[it]), ww);
      AddHist(pp->mTrack->mHistNHits, Form("%sTrackNHits", name[it]), ww);
      //AddHist(pp->mTrack->mHistNHitsFit, Form("%sTrackNHitsFit", name[it]), ww);
      AddHist(pp->mTrack->mHistNHitsFitRatio, Form("%sTrackNHitsFitRatio", name[it]), ww);
      AddHist(pp->mTrack->mHistDcaPt, Form("%sTrackDcaPt", name[it]), ww);
      AddHist(pp->mTrack->mHistFrac, Form("%sTrackFrac", name[it]), ww);
      /*
      AddHist(pp->mTrack->mHistNSigmaElectron, Form("%sNSigmaPion", name[it]), ww);
      AddHist(pp->mTrack->mHistNSigmaPion, Form("%sNSigmaPion", name[it]), ww);
      AddHist(pp->mTrack->mHistNSigmaKaon, Form("%sNSigmaKaon", name[it]), ww);
      AddHist(pp->mTrack->mHistNSigmaProton, Form("%sNSigmaProton", name[it]), ww);
      */

    }
  }
  
  fout->Write();
  fout->Close();
  return 1;
}
