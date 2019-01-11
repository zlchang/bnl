#ifndef STMYTRACKPROJEMCATRADIUS
#define STMYTRACKPROJEMCATRADIUS

#include "StMyTrackProjEmc.h"

class StMyTrackProjEmcAtRadius : public StMyTrackProjEmc
{
 public:
 StMyTrackProjEmcAtRadius() : StMyTrackProjEmc(){ mRadius = 225.405;}
  int findTower(const StMuTrack *track, double mag);
  int findTower(const StMcTrack *track, double mag);
  void setRadius(double r) {mRadius = r;}
  double getRadius() {return mRadius;}
 private:
  double mRadius;
  ClassDef(StMyTrackProjEmcAtRadius, 1);
};
#endif
