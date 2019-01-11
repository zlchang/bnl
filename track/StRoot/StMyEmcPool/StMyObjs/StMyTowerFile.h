#ifndef STMYTOWERFILE
#define STMYTOWERFILE
#include "StMyFile.h"
class StMyTowerHist;

class StMyTowerFile : public StMyFile
{
 public:
  StMyTowerFile(){}
 StMyTowerFile(TFile *fl) : StMyFile(fl){}
  void Add(StMyTowerHist *pp, char* name, double ww);
 private:
  ClassDef(StMyTowerFile, 0);
};
#endif
