#ifndef STMYTRACK
#define STMYTRACK
#include "StMyTower.h"
#include "StMyCluster.h"

class StMyTrack : public TObject{
 public:
  StMyTrack(){};
  double mPt;
  int mCharge;
  //double mE;
  double mDist;
  double mNSigmaPion;
  double mNSigmaElectron;
  //double mECluster;
  //double mHitFrac;
  //double mHitMax;
  StMyTower mTower;
  StMyCluster mCluster;
  ClassDef(StMyTrack, 0);
};
#endif
