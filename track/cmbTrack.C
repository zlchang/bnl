const char* type[] = {"All", "Pos", "Neg", "NoCut"};
const int Ntype = 4;
const char* trg[] = {"JP0", "JP1", "JP2"};
const int Ntrg = 3;

const int Npt = 13;

double weight[Npt];
double fudge[Npt];
void loadFudge(const char* runlist = "text.full")
{
  TFile *finfo = TFile::Open(Form("analyze_logs/ptbin.info.%s.root", runlist));
  TH1F *hweight = (TH1F *)finfo->Get("weight");
  //double weight[13];
  for(int ipt = 0; ipt < 13; ipt++){
    weight[ipt] = hweight->GetBinContent(ipt+1);
    //weight[ipt] = 1.;
  }
  finfo->Close();
  //float fudge[13];
  ifstream in_fudge(Form("fudge/fudge.ptbin.list.%s.rebin.5.txt", runlist));
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
}

//test
double sumw2 = 0; 
double sumw = 0;
double sumxw = 0, sumx2w = 0; 
void test(TProfile *profile, const char* name);

int cmbTrack(
	     char *file = "ptbin.list",
	     bool mode = 1,
	     int index = 10,
	     //char *file = "run12.final.v2.list",
	     //bool mode = 0,
	     //int index = 1,
	     char* dir_base = "output/track_filt", 
	     char* mcfilepre = "run12.ef.track",
	     int ver = 0)
{
  //TH1::SetDefaultSumw2(kTRUE);
  gSystem->Load("StMyObjs.so");
  gSystem->Load("StMyMatchTrackToEmcHist.so");

  TFile *fout = new TFile(Form("%s.%s.v%d.w.track.root", file, mcfilepre, ver), "recreate");
  //
  StMyMatchTrackToEmcHist *hist[Ntype][Ntrg];
  for(int it = 0; it < Ntype; it++){
     for(int jt = 0; jt < Ntrg; jt++){
        hist[it][jt] = new StMyMatchTrackToEmcHist(Form("Cmb%s%s", trg[jt], type[it]));
        //hist[it][jt] = new StMyMatchTrackToEmcHist(Form("Cmb%s%s", trg[jt], type[it]));
     }
  }
  StMyTowerHist *histTower[Ntrg];
  for(int it = 0; it < Ntrg; it++){
    histTower[it] = new StMyTowerHist(Form("Cmb%sTower", type[it]));
  }
  StMyClusterHist *histCluster[Ntrg];
  for(int it = 0; it < Ntrg; it++){
    histCluster[it]  = new StMyClusterHist(Form("Cmb%sCluster", type[it]));
  }
  if(mode) loadFudge();

  ifstream in(file);
  char ptbin[16];
  //set counter
  int counter = 0;
  while(in >> ptbin){
    double ww = 1;
    if(mode) ww = fudge[counter]*weight[counter];
    for(int ii = 0; ii < index; ii++){
      //yFile = TFile::Open(Form("%s/%s/%s.track.%d.root", dir_base, ptbin, ptbin, ii));
      TFile *fin = TFile::Open(Form("%s/%s/%s.track.%d.root", dir_base, ptbin, ptbin, ii));
      StMyMatchTrackToEmcFile ff(fin);
      cout<<Form("%s/%s/%s.track.%d.root", dir_base, ptbin, ptbin, ii)<<endl;
      for(int it = 0; it < Ntype; it++){
	for(int jt = 0; jt < Ntrg; jt++){
	  const char* nn = type[it];
	  const char* mm = trg[jt];
          if(jt == 2){
		if(mode == 1 && counter < 4) continue;
          }
	  StMyMatchTrackToEmcHist *pp = hist[it][jt];
	  ff.Add(pp, Form("%s%s", mm, nn), ww);
	}
      }
      for(int jt = 0; jt < Ntrg; jt++){
	const char* mm = trg[jt];
	StMyTowerFile ftwr(fin);
	ftwr.Add(histTower[jt], Form("%s", mm), ww);
	//cluster
	StMyClusterFile fcls(fin);
	fcls.Add(histCluster[jt], Form("%s", mm), ww);
      }
      fin->Close();
    }
    counter++;
  }
  
  fout->Write();
  fout->Close();
  return 1;
}
void test(TProfile *profile, const char* name, double ww)
{
  double cs = profile->GetBinContent(50);
  double ch = ((TProfile*)yFile->Get(name))->GetBinContent(50);
  double ens = profile->GetBinEntries(50);
  double enh = ((TProfile*)yFile->Get(name))->GetBinEntries(50);
  double es = profile->GetBinError(50);
  double eh = ((TProfile*)yFile->Get(name))->GetBinError(50);
  //double ees = pp->mHistEptVsPt->mProfile->GetBinEffectiveEntries(50);
  //double eeh = ((TProfile*)yFile->Get(Form("%sEptVsPtProf", nn)))->GetBinEffectiveEntries(50);
  double ees = profile->GetSumw2()->At(50);
  double eeh = ((TProfile*)yFile->Get(name))->GetSumw2()->At(50);
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
    if(effs > 0) errs = 1/effs*(sumx2w/sumw-TMath::Power(sumxw/sumw,2));
    rmss = (sumx2w/sumw-TMath::Power(sumxw/sumw,2))/sumw;
  }
  errs = TMath::Sqrt(errs);
  rmss = TMath::Sqrt(rmss);
  Printf("sumw2=%lg, sumw=%lg, sumxw=%lg, sumx2w=%lg, cons=%lg, effs=%lg, err=%lg, rmss=%lg\n", sumw2, sumw, sumxw, sumx2w, cons, effs, errs, rmss);
  
}
/*
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
*/
