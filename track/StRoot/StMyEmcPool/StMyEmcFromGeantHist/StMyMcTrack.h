#ifndef STMYMCTRACK
#define STMYMCTRACK

#include "TObject.h"
#include <vector>
#include "StMyMcTower.h"

class StMyMcTrack : public TObject{
 public:
  StMyMcTrack(){}
  float pt;
  long id;
  long  geantid;
  unsigned int size() const{ return towers.size(); }
  float sumE() const{ 
    float sum = 0; 
    for(vector<StMyMcTower>::const_iterator it = towers.begin(); it != towers.end(); it++){
      sum += (*it).e;
    }
    return sum;
  }
  vector<StMyMcTower> towers;
  ClassDef(StMyMcTrack, 1);
};
#endif
