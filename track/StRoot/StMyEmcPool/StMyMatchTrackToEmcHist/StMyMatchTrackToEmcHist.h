#ifndef STMYMATCHTRACKTOEMCHIST
#define STMYMATCHTRACKTOEMCHIST

#include "StMyEmcPool/StMyObjs/StMyTowerHist.h"
#include "StMyEmcPool/StMyObjs/StMyClusterHist.h"
#include "StMyEmcPool/StMyObjs/StMyTrackHist.h"
#include "StMyEmcPool/StMyObjs/StMyHist.h"

class StMyMatchTrackToEmcHist : public StMyTrackHist
{
public:
  StMyMatchTrackToEmcHist(){StMyTrackHist();}
  
  StMyMatchTrackToEmcHist(const char *name) : StMyTrackHist(name){
    int nbins = 55;
    double min = -1;
    double max = 10;
    TH1::SetDefaultSumw2(kTRUE); 

    mHistTower = new TH2D(Form("%sTower", name), "track p_{T} [GeV]; tower E [GeV]", nbins, min, max, nbins, min, max); 
    
    mHistEpVsPt = new StMyHist2D(Form("%sEpVsPt", name), ";track p_{T} [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);

    mHistEpVsPtCluster = new StMyHist2D(Form("%sEpVsPtCluster", name), ";track p_{T} [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);
    
    mHistHitTowerFracCluster = new StMyHist2D(Form("%sHitTowerFracCluster", name), ";track p_{T} [GeV]; hit frac.", nbins, min, max, 150, -0.1, 1.4);
    mHistMaxTowerFracCluster = new StMyHist2D(Form("%sMaxTowerFracCluster", name), ";track p_{T} [GeV]; hit frac.", nbins, min, max, 150, -0.1, 1.4);  
    mHistEpVsDist = new StMyProfile(Form("%sEpVsDist", name), ";d;E/p", 9, -0.01, 0.08);

    //mTower = new StMyTowerHist(Form("%sTower", name));
    //mCluster = new StMyClusterHist(Form("%sCluster", name));
    mHRadius = new TH1D(Form("%sRadius", name), "; radius [cm]", 300, 225, 255);
    mHistMult = new TH1D(Form("%sTrackMult", name), "; track mult", 100, 0, 100);
  }
  ~StMyMatchTrackToEmcHist(){
    delete mHistTower;

    delete mHistEpVsPt;

    //delete mHistEpVsPCluster;
    delete mHistEpVsPtCluster;

    delete mHistHitTowerFracCluster;
    delete mHistMaxTowerFracCluster;
    delete mHistEpVsDist;
    //delete mTower;
    //delete mCluster;
    //StMyTrackHist::~StMyTrackHist();
    delete mHRadius;
    delete mHistMult;
  }
  
  
  TH2D *mHistTower;

  
  StMyHist2D *mHistEpVsPt;

  StMyHist2D *mHistEpVsPtCluster;

  StMyHist2D *mHistHitTowerFracCluster;
  StMyHist2D *mHistMaxTowerFracCluster;

  StMyProfile *mHistEpVsDist;

  //StMyTowerHist *mTower;
  //StMyClusterHist *mCluster;

  TH1D *mHRadius;
  TH1D *mHistMult;
  ClassDef(StMyMatchTrackToEmcHist, 1);
};
#endif
