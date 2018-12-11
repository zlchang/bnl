#ifndef STMYTRACKMatch
#define STMYTRACKMatch
#include "StMyTrack.h"
#include "StMyTower.h"
#include "StMyCluster.h"

class StMyTrackMatch : public StMyTrack{
 public:
  StMyTrackMatch(){StMyTrack(); mMatch=false; mDist = -1.0; mRadius = 0.; mTower = StMyTower(); mCluster = StMyCluster();};
  bool mMatch;
  double mDist;
  double mRadius;
  StMyTower mTower;
  StMyCluster mCluster;
  ClassDef(StMyTrackMatch, 0);
};

#endif
