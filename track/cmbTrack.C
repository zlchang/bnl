TFile *mFile;
const char* type[] = {"All", "Pos", "Neg", "NoCut"};
const int Ntype = 4;
int cmbTrack(char *file = "ptbin.list", char* runlist = "text.full",
	     char* dir_base = "output/track", char* mcfilepre = "run12.a.track",
	     int ver = 0, int first = 0)
{
  TH1::SetDefaultSumw2(kTRUE);
  gSystem->Load("StMyMatchTrackToEmcHist.so");

  TFile *fout = new TFile(Form("%s.%s.v%d.w.track.root", file, mcfilepre, ver), "recreate");
  //
  StMyMatchTrackToEmcHist *hist[Ntype];
  for(int it = 0; it < Ntype; it++){
    hist[it] = new StMyMatchTrackToEmcHist(Form("Cmb%s", type[it]));
  }
  StMyTowerHist *histTower = new StMyTowerHist("CmbTower");
  StMyClusterHist *histCluster = new StMyClusterHist("CmbCluster");

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

  ifstream in(file);
  char ptbin[16];
  //set counter
  counter = 0;
  //test
  double sumw2 = 0; 
  double sumw = 0;
  double sumxw = 0, sumx2w = 0; 
  while(in >> ptbin){
    float ww = fudge[counter]*weight[counter];
    //ww = 1;
    for(int ii = 0; ii < 10; ii++){
      //if(ii%2 == 0) continue;
      mFile = TFile::Open(Form("%s/%s/%s.track.%d.root", dir_base, ptbin, ptbin, ii));
      cout<<Form("%s/%s/%s.track.%d.root", dir_base, ptbin, ptbin, ii)<<endl;
      for(int it = 0; it < Ntype; it++){
	const char* nn = type[it];
	StMyMatchTrackToEmcHist *pp = hist[it];
	AddHist(pp->mHistTrack, Form("%sTrack", nn), ww);
	AddHist(pp->mHistTrackFrac->mProfile, Form("%sTrackFracProf", nn), ww);
	AddHist(pp->mHistTrackFrac->mProfileW2, Form("%sTrackFracProfW2", nn), ww*ww);
	AddHist(pp->mHistTower, Form("%sTower", nn), ww);
	AddHist(pp->mHistNSigmaElectron, Form("%sNSigmaElectron", nn), ww);
	AddHist(pp->mHistNSigmaPion, Form("%sNSigmaPion", nn), ww);
	AddHist(pp->mHistEptVsPt->mProfile, Form("%sEptVsPtProf", nn), ww);
	AddHist(pp->mHistEptVsPt->mProfileW2, Form("%sEptVsPtProfW2", nn), ww*ww);
        if(it == 0){
        double cs = pp->mHistEptVsPt->mProfile->GetBinContent(50);
        double ch = ((TProfile*)mFile->Get(Form("%sEptVsPtProf", nn)))->GetBinContent(50);
        double ens = pp->mHistEptVsPt->mProfile->GetBinEntries(50);
        double enh = ((TProfile*)mFile->Get(Form("%sEptVsPtProf", nn)))->GetBinEntries(50);
        double es = pp->mHistEptVsPt->mProfile->GetBinError(50);
        double eh = ((TProfile*)mFile->Get(Form("%sEptVsPtProf", nn)))->GetBinError(50);
        //double ees = pp->mHistEptVsPt->mProfile->GetBinEffectiveEntries(50);
        //double eeh = ((TProfile*)mFile->Get(Form("%sEptVsPtProf", nn)))->GetBinEffectiveEntries(50);
        double ees = pp->mHistEptVsPt->mProfile->GetSumw2()->At(50);
        double eeh = ((TProfile*)mFile->Get(Form("%sEptVsPtProf", nn)))->GetSumw2()->At(50);
        sumw2 += enh*ww*ww;
	sumw += enh*ww;
        sumxw += enh*ch*ww;
        sumx2w += (eh*eh*enh+ch*ch)*enh*ww;
        Printf("cs=%lf, ens=%lf, ch=%lf, enh=%lf, es=%lf, eh =%lf, ww=%lf\n", cs,ens,ch,enh, es, eh, ww);
	Printf("ees=%lg, eeh=%lg, sumw2=%lg\n", ees, eeh, sumw2);
	double cons = 0;
	double effs = 0;
	double errs = 0,rmss = 0;
        if(sumw2 > 0){ 
		cons = sumxw/sumw;
		effs = ens*ens/sumw2;
		errs = 1/effs*(sumx2w/sumw-TMath::Power(sumxw/sumw,2));
                rmss = (sumx2w/sumw-TMath::Power(sumxw/sumw,2))/sumw;
        }
        errs = TMath::Sqrt(errs);
        rmss = TMath::Sqrt(rmss);
	Printf("sumw2=%lg, sumw=%lg, sumxw=%lg, sumx2w=%lg, cons=%lg, effs=%lg, err=%lg, rmss=%lg\n", sumw2, sumw, sumxw, sumx2w, cons, effs, errs, rmss);
       }
	AddHist(pp->mHistEptVsPt->mScatter, Form("%sEptVsPtScatter", nn), ww);
	AddHist(pp->mHistEptVsPtCluster->mProfile, Form("%sEptVsPtClusterProf", nn), ww);
	AddHist(pp->mHistEptVsPtCluster->mProfileW2, Form("%sEptVsPtClusterProfW2", nn), ww*ww);
	AddHist(pp->mHistEptVsPtCluster->mScatter, Form("%sEptVsPtClusterScatter", nn), ww);
	//hit frac
	AddHist(pp->mHistHitTowerFracCluster->mProfile, Form("%sHitTowerFracClusterProf", nn), ww);
	AddHist(pp->mHistHitTowerFracCluster->mProfileW2, Form("%sHitTowerFracClusterProfW2", nn), ww*ww);
	AddHist(pp->mHistHitTowerFracCluster->mScatter, Form("%sHitTowerFracClusterScatter", nn), ww);
	//max frac
	AddHist(pp->mHistMaxTowerFracCluster->mProfile, Form("%sMaxTowerFracClusterProf", nn), ww);
	AddHist(pp->mHistMaxTowerFracCluster->mProfileW2, Form("%sMaxTowerFracClusterProfW2", nn), ww*ww);
	AddHist(pp->mHistMaxTowerFracCluster->mScatter, Form("%sMaxTowerFracClusterScatter", nn), ww);
	
	//AddHist(pp->mHistEptVsDist->mProfile, Form("%sEptVsDistProf", nn), ww);
	//AddHist(pp->mHistEptVsDist->mProfileW2, Form("%sEptVsDistProfW2", nn), ww*ww);
      }
      AddHist(histTower->hE, "TowerE", ww);
      AddHist(histTower->hHits, "TowerHits", ww);
      AddHist(histTower->hNHits, "TowerNHits", ww);
      //cluster
      AddHist(histCluster->hE, "ClusterE", ww);
      AddHist(histCluster->hNHits, "ClusterNHits", ww);
      AddHist(histCluster->hMaxFrac, "ClusterMaxFrac", ww);
    }
    counter++;
  }

  fout->Write();
  fout->Close();
  return 1;
}

void AddHist(TH1D *hist, const char* name, double w)
{
  TH1D* hh = (TH1D*) mFile->Get(name);
  if(hh)
  hist->Add(hh, w);
  else
  Printf("%s not found\n", name);
}
void AddHist(TH2D *hist, const char* name, double w)
{
  TH2D* hh = (TH2D*) mFile->Get(name);
  if(hh)
  hist->Add(hh, w);
  else
  Printf("%s not found\n", name);
}
void AddHist(TProfile *hist, const char* name, double w)
{
  TProfile* hh = (TProfile*) mFile->Get(name);
  //double sumw = hist->GetBinEntries();
  //double sumxw = hist->GetBinContent()*sumw;
  //double 
  if(hh)
  hist->Add(hh, w);
  else
  Printf("%s not found\n", name);
}
