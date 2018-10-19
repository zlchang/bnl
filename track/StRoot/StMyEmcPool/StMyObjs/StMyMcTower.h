#ifndef STMYMCTOWER
#define STMYMCTOWER

#include "TObject.h"

class StMyMcTower : public TObject{
 public:
  StMyMcTower(){}
  float dE;
  float eta;
  //float e;
  float id;
  ClassDef(StMyMcTower, 1);
};
#endif
