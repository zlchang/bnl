#include "StMyTrackProjEmcSameTower.h"

int StMyTrackProjEmcSameTower::findTower(const StMuTrack *track, double mag)
{
  int enterTowerId = 0;
  double rmin = mRadius;
  if(mEmcPosition.trackOnEmc(&mPositionAt, &mMomentumAt, track, mag, rmin)){
    double enterEta = mPositionAt.pseudoRapidity();
    double enterPhi = mPositionAt.phi();
    mGeom->getId(enterPhi, enterEta, enterTowerId);
  }


  int exitTowerId = 0;
  double rmax = mRadius+mDepth;
  if(mEmcPosition.trackOnEmc(&mExitPositionAt, &mExitMomentumAt, track, mag, rmax)){
    double exitEta = mExitPositionAt.pseudoRapidity();
    double exitPhi = mExitPositionAt.phi();
    mGeom->getId(exitPhi, exitEta, exitTowerId);
  }

  if(enterTowerId == exitTowerId)
    return exitTowerId;
  else
    return -1;
}
int StMyTrackProjEmcSameTower::findTower(const StMcTrack *track, double mag)
{
  int enterTowerId = 0;
  double rmin = mRadius;
  if(mEmcPosition.trackOnEmc(&mPositionAt, &mMomentumAt, track, mag, rmin)){
    double enterEta = mPositionAt.pseudoRapidity();
    double enterPhi = mPositionAt.phi();
    mGeom->getId(enterPhi, enterEta, enterTowerId);
  }


  int exitTowerId = 0;
  double rmax = mRadius+mDepth;
  if(mEmcPosition.trackOnEmc(&mExitPositionAt, &mExitMomentumAt, track, mag, rmax)){
    double exitEta = mExitPositionAt.pseudoRapidity();
    double exitPhi = mExitPositionAt.phi();
    mGeom->getId(exitPhi, exitEta, exitTowerId);
  }

  if(enterTowerId == exitTowerId)
    return exitTowerId;
  else
    return -1;
}
