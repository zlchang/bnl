#ifndef STMYCLUSTERFILE
#define STMYCLUSTERFILE
#include "StMyFile.h"
class StMyClusterHist;

class StMyClusterFile : public StMyFile
{
 public:
  StMyClusterFile(){}
 StMyClusterFile(TFile *fl) : StMyFile(fl){}
  void Add(StMyClusterHist *pp, char* name, double ww);
 private:
  ClassDef(StMyClusterFile, 0);
};
#endif
