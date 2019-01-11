#ifndef STMYMATCHTRACKTOEMCFILE
#define STMYMATCHTRACKTOEMCFILE
#include "StMyEmcPool/StMyObjs/StMyFile.h"
class StMyMatchTrackToEmcHist;

class StMyMatchTrackToEmcFile : public StMyFile
{
 public:
  StMyMatchTrackToEmcFile(){}
 StMyMatchTrackToEmcFile(TFile *fl) : StMyFile(fl){}
  void Add(StMyMatchTrackToEmcHist *pp, char* name, double ww);
 private:
  ClassDef(StMyMatchTrackToEmcFile, 0);
};
#endif
