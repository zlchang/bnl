#ifndef FUNCEMCFROMGEANT
#define FUNCEMCFROMGEANT

#include "StMcEvent/StMcVertex.hh"

float dcaT(StMcVertex *a, StMcVertex *b)
{
  StThreeVectorF av = a->position();
  StThreeVectorF bv = b->position();

  StThreeVectorF cv = av - bv;
  return cv.mag();
}

#endif
