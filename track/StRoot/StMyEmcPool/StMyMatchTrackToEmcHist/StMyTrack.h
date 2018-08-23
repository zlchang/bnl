#ifndef STMYTRACK
#define STMYTRACK
#include "StMyTower.h"
#include "StMyCluster.h"

class StMyTrack : public TObject{
 public:
  StMyTrack(){mMatch = false;};
  double mPt;
  int mCharge;
  double mNSigmaPion;
  double mNSigmaElectron;
  bool mMatch;
  ClassDef(StMyTrack, 0);
};
class StMyTrackMatch : public StMyTrack{
 public:
  StMyTrackMatch(){StMyTrack();};

  double mDist;
  StMyTower mTower;
  StMyCluster mCluster;
  ClassDef(StMyTrackMatch, 0);
};

#endif
