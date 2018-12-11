#ifndef STEMCFROMGEANTMAKER
#define STEMCFROMGEANTMAKER

#include "StMaker.h"

#include <vector>
#include <map>

class TFile;
class StEmcGeom;
class StBemcTables;
class StMyEmcTrackHist;
class StMyEmcTrackMatchHist;
class StMyMcVertexCut;
class StMyTrackProjEmc;

class StMyEmcFromGeantMaker : public StMaker{
  
 public:
 StMyEmcFromGeantMaker(const char* name = "StMyEmcFromGeantMaker"):StMaker(name){
    mFileName = "EmcFromGeant.root";
  }
  int Init();
  int Make();
  int Finish();
  void SetOutfile(const char* file){mFileName =  file; } 
  void addVertexCut(StMyMcVertexCut* vcut){ mVertexCuts.push_back(vcut); } 
  void setTrackProjEmc(StMyTrackProjEmc* proj){ mTrackProj = proj; }
 protected:
  StEmcGeom *mBemcGeom;
  StBemcTables *mBemcTable;
 private:
  const char *mFileName;
  TFile *mFile;
  
  vector<long> mIds;
  map<long, StMyEmcTrackHist*> mMapHists;
  map<long, StMyEmcTrackMatchHist*> mMapTrackHists;
  vector<StMyMcVertexCut*> mVertexCuts;
  StMyTrackProjEmc *mTrackProj;
  ClassDef(StMyEmcFromGeantMaker, 0);
};
#endif
