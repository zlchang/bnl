#include "func.h"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcTrack.hh"

#include "StEmcUtil/projection/StEmcPosition.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

//#include "StTpcDb/StTpcDb.h"


#include "StarClassLibrary/SystemOfUnits.h"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "StarClassLibrary/StParticleTable.hh"
#include "StarClassLibrary/StParticleDefinition.hh"
#include "StarClassLibrary/StPhysicalHelix.hh"

StThreeVectorD projTrack(const StMcTrack *mcTrack, double mag, double radius)
{
  StThreeVectorD mom = mcTrack->momentum();
  StThreeVectorD vtx = mcTrack->startVertex()->position();

  int id = mcTrack->geantId();
  StParticleTable *table = StParticleTable::instance();
  StParticleDefinition *def = table->findParticleByGeantId(id);
  double chrg = def->charge();

  //StTpcDb *tpcdb = StTpcDb::instance();
  //double radius = tpcdb->Dimensions()->ifcRadius();
  if(round(chrg) != 0){
    StPhysicalHelix hx(mom, vtx, mag*tesla, chrg);
    pairD pathLength = hx.pathLength(radius);
    double s1 = pathLength.first;
    double s2 = pathLength.second;
    if(s1*s2 > 0){
      Printf("s1=%f s2=%f chrg=%.1f pt=%f eta=%f\n", s1, s2, chrg, mcTrack->pt(), mcTrack->momentum().pseudoRapidity());
      return StThreeVectorD(999, 999, 999);
    }
    double ss = s1 > 0 ? s1 : s2;
    StThreeVectorD pos = hx.at(ss);
    return pos;
  }
  return StThreeVectorD(999, 999, 999);
}

float dcaT(StMcVertex *a, StMcVertex *b)
{
  StThreeVectorF av = a->position();
  StThreeVectorF bv = b->position();

  StThreeVectorF cv = av - bv;
  return cv.mag();
}

int projTowerId(const StMuTrack *muTrack,   StThreeVectorD *momentumAt, StThreeVectorD *positionAt, double mag, double bemcRadius, const StEmcGeom* geom)
{
  StEmcPosition EmcPosition;

  int exitTowerId = 0;
  
  if(EmcPosition.trackOnEmc(positionAt, momentumAt, muTrack, mag, bemcRadius)){
    double exitEta = positionAt->pseudoRapidity();
    double exitPhi = positionAt->phi();
    //Printf("tower Id %d, exit eta: %.2lf, phi: %.2lf", exitTowerId, exitEta, exitPhi);
    //StEmcGeom::instance("bemc")->getId(exitPhi, exitEta, exitTowerId);
    geom->getId(exitPhi, exitEta, exitTowerId);
  }
  return exitTowerId;
}
