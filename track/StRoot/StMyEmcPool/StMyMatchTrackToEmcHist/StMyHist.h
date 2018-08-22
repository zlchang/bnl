#ifndef STMYHIST
#define STMYHIST

#include "TObject.h"
#include "TProfile.h"
#include "TH2D.h"
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

  void Fill(double x, double y, double w = 1.0){
    mProfile->Fill(x, y, w);
    mProfileW2->Fill(x, y, w*w);
  }
  TProfile *mProfile;
  TProfile *mProfileW2;
  ClassDef(StMyProfile, 1);
};
#endif
