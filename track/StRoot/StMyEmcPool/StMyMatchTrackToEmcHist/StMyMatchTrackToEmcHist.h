#ifndef STMYMATCHTRACKTOEMCHIST
#define STMYMATCHTRACKTOEMCHIST

#include "TObject.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TH2D.h"

#include "StMyTowerHist.h"
#include "StMyClusterHist.h"

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

class StMyMatchTrackToEmcHist : public TObject
{
public:
  StMyMatchTrackToEmcHist(){}
  
  StMyMatchTrackToEmcHist(const char *name){
    int nbins = 55;
    double min = -1;
    double max = 10;
    TH1::SetDefaultSumw2(kTRUE); 
    mHistTrack = new TH1D(Form("%sTrack", name), "; track p_{T} [GeV]", nbins, min, max); 
    mHistTrackFrac = new StMyProfile(Form("%sTrackFrac", name), "; track p_{T} [GeV]", nbins, min, max);

    mHistTower = new TH1D(Form("%sTower", name), "; tower E [GeV]", nbins, min, max); 

    mHistNSigmaElectron = new TH1D(Form("%sNSigmaElectron", name), ";n#sigma_{e}", 200, -10, 10);
    mHistNSigmaPion = new TH1D(Form("%sNSigmaPion", name), ";n#sigma_{e}", 200, -10, 10);
    
    //mHistEpVsP = new StMyHist2D(Form("%sEpVsP", name), ";track p [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);
    mHistEptVsPt = new StMyHist2D(Form("%sEptVsPt", name), ";track p_{T} [GeV]; E/p_{T}", nbins, min, max, 150, -0.1, 1.4);

    //mHistEpVsPCluster = new StMyHist2D(Form("%sEpVsPCluster", name), ";track p [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);
    mHistEptVsPtCluster = new StMyHist2D(Form("%sEptVsPtCluster", name), ";track p_{T} [GeV]; E/p_{T}", nbins, min, max, 150, -0.1, 1.4);
    
    mHistHitTowerFracCluster = new StMyHist2D(Form("%sHitTowerFracCluster", name), ";track p_{T} [GeV]; hit frac.", nbins, min, max, 150, -0.1, 1.4);
    mHistMaxTowerFracCluster = new StMyHist2D(Form("%sMaxTowerFracCluster", name), ";track p_{T} [GeV]; hit frac.", nbins, min, max, 150, -0.1, 1.4);  
    mHistEptVsDist = new TProfile(Form("%sEptVsDist", name), ";d;E/p_{T}", 9, -0.01, 0.08);

    mTower = new StMyTowerHist(Form("%sTower", name));
    mCluster = new StMyClusterHist(Form("%sCluster", name));
  }
  ~StMyMatchTrackToEmcHist(){
    delete mHistTrack;
    delete mHistTrackFrac;
    delete mHistTower;
    delete mHistNSigmaElectron;
    delete mHistNSigmaPion;

    //delete mHistEpVsP;
    delete mHistEptVsPt;

    //delete mHistEpVsPCluster;
    delete mHistEptVsPtCluster;

    delete mHistHitTowerFracCluster;
    delete mHistMaxTowerFracCluster;
    delete mHistEptVsDist;

    delete mTower;
    delete mCluster;
  }
  
  
  TH1D *mHistTrack;
  StMyProfile *mHistTrackFrac;
  TH1D *mHistTower;
  TH1D *mHistNSigmaElectron;
  TH1D *mHistNSigmaPion;
  
  //StMyHist2D *mHistEpVsP;
  StMyHist2D *mHistEptVsPt;

  //StMyHist2D *mHistEpVsPCluster;
  StMyHist2D *mHistEptVsPtCluster;

  StMyHist2D *mHistHitTowerFracCluster;
  StMyHist2D *mHistMaxTowerFracCluster;

  TProfile *mHistEptVsDist;

  StMyTowerHist *mTower;
  StMyClusterHist *mCluster;
  ClassDef(StMyMatchTrackToEmcHist, 1);
};
#endif
