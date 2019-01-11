#include "StMyTrackProjEmcPtRadius.h"
#include "StMyEmcPool/StMyObjs/SFfunc.h"

double StMyTrackProjEmcPtRadius::getRadius()
{
  return mRadius;
}
int StMyTrackProjEmcPtRadius::findTower(const StMuTrack *track, double mag)
{
  int exitTowerId = 0;
  double pt = track->pt();
  mRadius = getRadiusLinear(pt);
  //Printf("radius = %lf\n", mRadius);
  if(mEmcPosition.trackOnEmc(&mPositionAt, &mMomentumAt, track, mag, mRadius)){
    double exitEta = mPositionAt.pseudoRapidity();
    double exitPhi = mPositionAt.phi();
    mGeom->getId(exitPhi, exitEta, exitTowerId);
    //Printf("PtRadius: exitEta=%lf, exitPhi=%lf, id=%d\n", exitEta, exitPhi, exitTowerId);
  }
  return exitTowerId;
}
int StMyTrackProjEmcPtRadius::findTower(const StMcTrack *track, double mag)
{
  int exitTowerId = 0;
  double pt = track->pt();
  mRadius = getRadiusLinear(pt);
  //Printf("radius = %lf\n", mRadius);
  if(mEmcPosition.trackOnEmc(&mPositionAt, &mMomentumAt, track, mag, mRadius)){
    double exitEta = mPositionAt.pseudoRapidity();
    double exitPhi = mPositionAt.phi();
    Printf("exitEta = %lf exitPhi = %lf", exitEta, exitPhi);
    mGeom->getId(exitPhi, exitEta, exitTowerId);
    //Printf("PtRadius: exitEta=%lf, exitPhi=%lf, id=%d\n", exitEta, exitPhi, exitTowerId);
  }
  return exitTowerId;
}
