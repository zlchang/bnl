#ifndef STMYTRACKPROJEMCPTRADIUS
#define STMYTRACKPROJEMCPTRADIUS

#include "StMyTrackProjEmc.h"

class StMyTrackProjEmcPtRadius : public StMyTrackProjEmc
{
 public:
 StMyTrackProjEmcPtRadius() : StMyTrackProjEmc(){}
  int findTower(const StMuTrack *track, double mag);
  int findTower(const StMcTrack *track, double mag);
  double getRadius();
 private:
  double mRadius;
  ClassDef(StMyTrackProjEmcPtRadius, 1);
};
#endif
