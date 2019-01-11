#ifndef STMYMCTOWER
#define STMYMCTOWER

#include "TObject.h"

class StMyMcTower : public TObject{
 public:
  StMyMcTower(){}
  float dE;
  float eta;
  //float e;
  int id;
  ClassDef(StMyMcTower, 1);
};
#endif
