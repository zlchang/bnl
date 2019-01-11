#include "StMyFile.h"
#include "StMyHist.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

void StMyFile::AddHist(StMyHist2D *hist, const char* name, double ww)
{
  AddHist(hist->mProfile, Form("%sProf", name), ww);
  AddHist(hist->mProfileW2, Form("%sProfW2", name), ww*ww);
  AddHist(hist->mScatter, Form("%sScatter", name), ww);
}
void StMyFile::AddHist(StMyProfile *hist, const char* name, double ww)
{
  AddHist(hist->mProfile, Form("%sProf", name), ww);
  AddHist(hist->mProfileW2, Form("%sProfW2", name), ww*ww);
}
void StMyFile::AddHist(TH1D *hist, const char* name, double w)
{
  TH1D* hh = (TH1D*) mFile->Get(name);
  if(hh)
    hist->Add(hh, w);
  else
    Printf("%s not found\n", name);
}
void StMyFile::AddHist(TH2D *hist, const char* name, double w)
{
  TH2D* hh = (TH2D*) mFile->Get(name);
  if(hh)
    hist->Add(hh, w);
  else
    Printf("%s not found\n", name);
}
void StMyFile::AddHist(TProfile *hist, const char* name, double w)
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
