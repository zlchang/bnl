#ifndef STEMCFROMGEANTMAKER
#define STEMCFROMGEANTMAKER

#include "StMaker.h"

#include <vector>
#include <map>

class TFile;
class StEmcGeom;
class StBemcTables;
class StMyMcTrackHist;

class StMyEmcFromGeantMaker : public StMaker{
  
 public:
 StMyEmcFromGeantMaker(const char* name = "StMyEmcFromGeantMaker"):StMaker(name){
    mFileName = "EmcFromGeant.root";
  }
  int Init();
  int Make();
  int Finish();
  void SetOutfile(const char* file){mFileName =  file; }  
 protected:
  StEmcGeom *mBemcGeom;
  StBemcTables *mBemcTable;
 private:
  const char *mFileName;
  TFile *mFile;

  vector<long> mIds;
  map<long, StMyMcTrackHist*> mMapHists;
  ClassDef(StMyEmcFromGeantMaker, 0);
};
#endif
