#ifndef STMYTRACKPROJEMC
#define STMYTRACKPROJEMC

#include "TObject.h"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMcEvent/StMcTrack.hh"

class StMyTrackProjEmc: public TObject
{
 public:
  StMyTrackProjEmc(){
    mMomentumAt = StThreeVectorD(0, 0, 0);
    mPositionAt = StThreeVectorD(0, 0, 0);
    mGeom = StEmcGeom::instance("bemc");
    mEmcPosition = StEmcPosition();
  };
  
  virtual int findTower(const StMuTrack *track, double mag) = 0;
  virtual int findTower(const StMcTrack *track, double mag) = 0;

  virtual double getRadius() = 0;

  StThreeVectorD mMomentumAt;
  StThreeVectorD mPositionAt;
  StEmcGeom *mGeom;
  StEmcPosition mEmcPosition;
 private:
  ClassDef(StMyTrackProjEmc, 0);
};
#endif
