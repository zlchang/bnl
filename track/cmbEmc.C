TFile *mFile;
long ids[] = {1, 2, 3,
	      7, 8, 9,
	      10, 11, 12,
	      13, 14, 15,
	      16};
const int Nids = 13;

int cmbEmc(char *file = "ptbin.list", char* runlist = "text.full",
	     char* dir_base = "output/MyReadEmcGeant", char* mcfilepre = "run12.b.emc",
	     int ver = 0, int first = 0)
{
  TH1::SetDefaultSumw2(kTRUE);
  gSystem->Load("StMyMatchTrackToEmcHist.so");
  gSystem->Load("StMyEmcFromGeantHist.so");

  TFile *fout = new TFile(Form("%s.%s.v%d.w.root", file, mcfilepre, ver), "recreate");
  //
  StMyMcTrackHist *hist[Nids];
  StMyGeantId geant;
  for(int it = 0; it < Nids; it++){
    hist[it] = new StMyMcTrackHist(Form("%s", geant.getName(ids[it])));
  }

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
      mFile = TFile::Open(Form("%s/%s/%s.%d.MyReadEmcGeant.root", dir_base, ptbin, ptbin, ii));
      //mFile = TFile::Open(Form("%s/%s/%s.mudst.subset.%d.list.emcGeant.root", dir_base, ptbin, ptbin, ii));
      //mFile = TFile::Open(Form("%s/%s/%s.emcGeant.%d.root", dir_base, ptbin, ptbin, ii));
      cout<<Form("%s/%s/%s.MyReadEmcGeant.%d.root", dir_base, ptbin, ptbin, ii)<<endl;

      for(int it = 0; it < Nids; it++){
	const char* nn = geant.getName(ids[it]);
	StMyMcTrackHist *pp = hist[it];
	AddHist(pp->mEptVsPt->mProfile, Form("%sEptVsptProf", nn), ww);
	AddHist(pp->mEptVsPt->mProfileW2, Form("%sEptVsptProfW2", nn), ww*ww);
	AddHist(pp->mEptVsPt->mScatter, Form("%sEptVsptScatter", nn), ww);
	//
	AddHist(pp->mNVsPt->mProfile, Form("%sNVsptProf", nn), ww);
	AddHist(pp->mNVsPt->mProfileW2, Form("%sNVsptProfW2", nn), ww*ww);
	AddHist(pp->mNVsPt->mScatter, Form("%sNVsptScatter", nn), ww);
      }
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
  //Printf("%s 6th bin: %lf\n", hh->GetName(), hh->GetBinContent(6));
  //double sumw = hist->GetBinEntries();
  //double sumxw = hist->GetBinContent()*sumw;
  //double 
  if(hh)
  hist->Add(hh, w);
  else
  Printf("%s not found\n", name);
}
