#ifndef FUNCEMCFROMGEANT
#define FUNCEMCFROMGEANT

#include "StarClassLibrary/StThreeVectorD.hh"

class StMcVertex;
class StMuTrack;
class StEmcGeom;
class StMcTrack;

float dcaT(StMcVertex *a, StMcVertex *b);
int projTowerId(const StMuTrack *muTrack, StThreeVectorD *momentum, StThreeVectorD *position, double mag, double bemcRadius, const StEmcGeom* geom);
StThreeVectorD projTrack(const StMcTrack *mcTrack, double mag, double radius);
#endif
