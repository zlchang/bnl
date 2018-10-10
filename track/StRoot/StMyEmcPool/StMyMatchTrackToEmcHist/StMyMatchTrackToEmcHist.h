#ifndef STMYMATCHTRACKTOEMCHIST
#define STMYMATCHTRACKTOEMCHIST

#include "StMyTowerHist.h"
#include "StMyClusterHist.h"
#include "StMyTrackHist.h"
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

    mHistTower = new TH1D(Form("%sTower", name), "; tower E [GeV]", nbins, min, max); 
    
    //mHistEpVsP = new StMyHist2D(Form("%sEpVsP", name), ";track p [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);
    mHistEptVsPt = new StMyHist2D(Form("%sEptVsPt", name), ";track p_{T} [GeV]; E/p_{T}", nbins, min, max, 150, -0.1, 1.4);

    //mHistEpVsPCluster = new StMyHist2D(Form("%sEpVsPCluster", name), ";track p [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);
    mHistEptVsPtCluster = new StMyHist2D(Form("%sEptVsPtCluster", name), ";track p_{T} [GeV]; E/p_{T}", nbins, min, max, 150, -0.1, 1.4);
    
    mHistHitTowerFracCluster = new StMyHist2D(Form("%sHitTowerFracCluster", name), ";track p_{T} [GeV]; hit frac.", nbins, min, max, 150, -0.1, 1.4);
    mHistMaxTowerFracCluster = new StMyHist2D(Form("%sMaxTowerFracCluster", name), ";track p_{T} [GeV]; hit frac.", nbins, min, max, 150, -0.1, 1.4);  
    mHistEptVsDist = new StMyProfile(Form("%sEptVsDist", name), ";d;E/p_{T}", 9, -0.01, 0.08);

    //mTower = new StMyTowerHist(Form("%sTower", name));
    //mCluster = new StMyClusterHist(Form("%sCluster", name));
  }
  ~StMyMatchTrackToEmcHist(){
    delete mHistTower;

    //delete mHistEpVsP;
    delete mHistEptVsPt;

    //delete mHistEpVsPCluster;
    delete mHistEptVsPtCluster;

    delete mHistHitTowerFracCluster;
    delete mHistMaxTowerFracCluster;
    delete mHistEptVsDist;
    //delete mTower;
    //delete mCluster;
    //StMyTrackHist::~StMyTrackHist();
  }
  
  
  TH1D *mHistTower;

  
  //StMyHist2D *mHistEpVsP;
  StMyHist2D *mHistEptVsPt;

  //StMyHist2D *mHistEpVsPCluster;
  StMyHist2D *mHistEptVsPtCluster;

  StMyHist2D *mHistHitTowerFracCluster;
  StMyHist2D *mHistMaxTowerFracCluster;

  StMyProfile *mHistEptVsDist;

  //StMyTowerHist *mTower;
  //StMyClusterHist *mCluster;

  ClassDef(StMyMatchTrackToEmcHist, 1);
};
#endif
