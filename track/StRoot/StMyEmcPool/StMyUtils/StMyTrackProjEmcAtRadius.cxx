#include "StMyTrackProjEmcAtRadius.h"

int StMyTrackProjEmcAtRadius::findTower(const StMuTrack *track, double mag)
{
  int exitTowerId = 0;
  if(mEmcPosition.trackOnEmc(&mPositionAt, &mMomentumAt, track, mag, mRadius)){
    double exitEta = mPositionAt.pseudoRapidity();
    double exitPhi = mPositionAt.phi();
    mGeom->getId(exitPhi, exitEta, exitTowerId);
  }
  return exitTowerId;
}

int StMyTrackProjEmcAtRadius::findTower(const StMcTrack *track, double mag)
{
  int exitTowerId = 0;
  //Printf("mRadius = %lf", mRadius);
  if(mEmcPosition.trackOnEmc(&mPositionAt, &mMomentumAt, track, mag, mRadius)){
    float exitEta = mPositionAt.pseudoRapidity();
    float exitPhi = mPositionAt.phi();
    //if(mGeom) mGeom->getId(exitEta, exitPhi, exitTowerId);
    //else Printf("mGeom not initilized");
    mGeom->getId(exitPhi, exitEta, exitTowerId);
    //Printf("exitEta =%lf exitPhi = %lf id = %d", exitEta, exitPhi, exitTowerId);
    //int m = -1, e = -1, s = -1;
    //int r = mGeom->getBin(exitEta, exitPhi, m, e, s);
    //Printf("m = %d e = %d s = %d r = %d", m, e, s, r);
  }
  return exitTowerId;
}
