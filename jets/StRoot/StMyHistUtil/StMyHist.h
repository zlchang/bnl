#ifndef STMYHIST
#define STMYHIST

#include "TObject.h"
#include "TProfile.h"
#include "TH2D.h"
#include "TProfile2D.h"

class StMyHist2D : public TObject
{
 public:
  StMyHist2D(){}
  ~StMyHist2D(){
    delete mProfile;
    delete mProfileW2;
    delete mScatter;
  }
  StMyHist2D(const char* histname, const char* titlename, int nx, double xmin, double xmax, int ny, double ymin, double ymax){
    mProfile = new TProfile(Form("%sProf", histname), titlename, nx, xmin, xmax);
    mProfileW2 = new TProfile(Form("%sProfW2", histname), titlename, nx, xmin, xmax);
    mScatter = new TH2D(Form("%sScatter", histname), titlename, nx, xmin, xmax, ny, ymin, ymax);
  }

  void Fill(double x, double y, double w = 1.0){
    mProfile->Fill(x, y, w); 
    mProfileW2->Fill(x, y, w*w); 
    mScatter->Fill(x, y, w);
  }
  void Add(StMyHist2D* hist, double w = 1.0){
    mProfile->Add(hist->mProfile, w);
    mProfileW2->Add(hist->mProfile, w*w);
    mScatter->Add(hist->mScatter, w);
  }
  TProfile *mProfile;
  TProfile *mProfileW2;
  TH2D *mScatter;
  ClassDef(StMyHist2D, 1);
};

class StMyProfile : public TObject
{
 public:
  StMyProfile(){}
  ~StMyProfile(){
    delete mProfile;
    delete mProfileW2;
  }
  StMyProfile(const char* histname, const char* titlename, int nx, double xmin, double xmax){
    mProfile = new TProfile(Form("%sProf", histname), titlename, nx, xmin, xmax);
    mProfileW2 = new TProfile(Form("%sProfW2", histname), titlename, nx, xmin, xmax);
  }

  StMyProfile(const char* histname, const char* titlename, int nx, const double *xbins){
    mProfile = new TProfile(Form("%sProf", histname), titlename, nx, xbins);
    mProfileW2 = new TProfile(Form("%sProfW2", histname), titlename, nx, xbins);
  }

  void Fill(double x, double y, double w = 1.0){
    mProfile->Fill(x, y, w);
    mProfileW2->Fill(x, y, w*w);
  }
  void Add(StMyProfile* hist, double w = 1.0){
    mProfile->Add(hist->mProfile, w);
    mProfileW2->Add(hist->mProfile, w*w);
  }
  TProfile *mProfile;
  TProfile *mProfileW2;
  ClassDef(StMyProfile, 1);
};

class StMyProfile2D : public TObject
{
 public:
  StMyProfile2D(){}
  ~StMyProfile2D(){
    delete mProfile;
    delete mProfileW2;
  }
  StMyProfile2D(const char* histname, const char* titlename, int nx, const double *xbins, int ny, const double *ybins){
    mProfile = new TProfile2D(Form("%sProf", histname), titlename, nx, xbins, ny, ybins);
    mProfileW2 = new TProfile2D(Form("%sProfW2", histname), titlename, nx, xbins, ny, ybins);
  }

  StMyProfile2D(const char* histname, const char* titlename, int nx, double xmin, double xmax, int ny, double ymin, double ymax){
    mProfile = new TProfile2D(Form("%sProf", histname), titlename, nx, xmin, xmax, ny, ymin, ymax);
    mProfileW2 = new TProfile2D(Form("%sProfW2", histname), titlename, nx, xmin, xmax, ny, ymin, ymax);
  }

  void Fill(double x, double y, double z, double w = 1.0){
    mProfile->Fill(x, y, z, w);
    mProfileW2->Fill(x, y, z, w*w);
  }
  void Add(StMyProfile2D* hist, double w = 1.0){
    mProfile->Add(hist->mProfile, w);
    mProfileW2->Add(hist->mProfile, w*w);
  }
  TProfile2D *mProfile;
  TProfile2D *mProfileW2;
  ClassDef(StMyProfile2D, 1);
};
#endif
