#ifndef STMYTRACKPROJEMCSAMETOWER
#define STMYTRACKPROJEMCSAMETOWER

#include "StMyTrackProjEmc.h"

class StMyTrackProjEmcSameTower : public StMyTrackProjEmc
{
 public:
  StMyTrackProjEmcSameTower() : StMyTrackProjEmc(){
    mRadius = 225.405;
    mDepth = 30;
  }
  int findTower(const StMuTrack *track, double mag);
  int findTower(const StMcTrack *track, double mag);
  void setRadius(double rr){mRadius = rr;}
  void setDepth(double dd) {mDepth = dd;}
  double getRadius(){ return mRadius+mDepth/2.; }
  StThreeVectorD mExitPositionAt;
  StThreeVectorD mExitMomentumAt;
 private:
  double mRadius;
  double mDepth;
  ClassDef(StMyTrackProjEmcSameTower, 1);
};
#endif
