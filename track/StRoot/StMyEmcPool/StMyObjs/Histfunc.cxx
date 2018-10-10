#include "Histfunc.h"
TFile *yFile;
void AddHist(StMyHist2D *hist, const char* name, double ww)
{
  AddHist(hist->mProfile, Form("%sProf", name), ww);
  AddHist(hist->mProfileW2, Form("%sProfW2", name), ww*ww);
  AddHist(hist->mScatter, Form("%sScatter", name), ww);
}
void AddHist(StMyProfile *hist, const char* name, double ww)
{
  AddHist(hist->mProfile, Form("%sProf", name), ww);
  AddHist(hist->mProfileW2, Form("%sProfW2", name), ww*ww);
}
void AddHist(TH1D *hist, const char* name, double w)
{
  TH1D* hh = (TH1D*) yFile->Get(name);
  if(hh)
    hist->Add(hh, w);
  else
    Printf("%s not found\n", name);
}
void AddHist(TH2D *hist, const char* name, double w)
{
  TH2D* hh = (TH2D*) yFile->Get(name);
  if(hh)
    hist->Add(hh, w);
  else
    Printf("%s not found\n", name);
}
void AddHist(TProfile *hist, const char* name, double w)
{
  TProfile* hh = (TProfile*) yFile->Get(name);
  //double sumw = hist->GetBinEntries();
  //double sumxw = hist->GetBinContent()*sumw;
  //double 
  if(hh)
    hist->Add(hh, w);
  else
    Printf("%s not found\n", name);
}
