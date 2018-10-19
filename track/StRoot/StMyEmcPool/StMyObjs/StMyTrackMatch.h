#ifndef STMYTRACKMatch
#define STMYTRACKMatch
#include "StMyTrack.h"
#include "StMyTower.h"
#include "StMyCluster.h"

class StMyTrackMatch : public StMyTrack{
 public:
  StMyTrackMatch(){StMyTrack(); mMatch=false;};
  bool mMatch;
  double mDist;
  StMyTower mTower;
  StMyCluster mCluster;
  ClassDef(StMyTrackMatch, 0);
};

#endif
