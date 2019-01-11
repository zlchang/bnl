#ifndef STMYMATCHTRACKTOEMCMAKER
#define STMYMATCHTRACKTOEMCMAKER

#include "StMaker.h"

#include <vector>
#include <string>

class TFile;
class StEmcGeom;
class StBemcTables;
class StMyTrackMatch;
class StMyTower;
class StMyCluster;
class StMyTowerHist;
class StMyClusterHist;
class StMyMatchTrackToEmcHist;
class StMyTrackCut;
class StMyVertexCut;
class StMyTrackProjEmc;

class StMyMatchTrackToEmcMaker : public StMaker{

public:
 StMyMatchTrackToEmcMaker(const char* name = "StMyMatchTrackToEmcMaker"):StMaker(name){
   mFileName = "MyMatchTrackToEmc.root";
   mFlagMc = false;
  }
  int Init();
  int Make();
  int Finish();
  void addTrackCut(StMyTrackCut* tcut){ mTrackCuts.push_back(tcut); }
  void addVertexCut(StMyVertexCut* vcut){ mVertexCuts.push_back(vcut); }
  void setTrackProjEmc(StMyTrackProjEmc* proj){ mTrackProj = proj; }
  void setMc(bool flag) {mFlagMc = flag;}
  void SetOutfile(const char* file){mFileName =  file; }  
 protected:
  StEmcGeom *mBemcGeom;
  StBemcTables *mBemcTable;
  void fillHist(const StMyTrackMatch &track, StMyMatchTrackToEmcHist *hist);
  void fillHistCluster(const StMyCluster &cluster, StMyClusterHist *hist);
  void fillHistTower(const StMyTower &tower, StMyTowerHist *hist);
 private:
  const char *mFileName;
  TFile *mFile;
  bool mFlagMc;
  vector<StMyMatchTrackToEmcHist *>mHistNoCutVec;
  vector<StMyMatchTrackToEmcHist *>mHistVec;
  vector<StMyMatchTrackToEmcHist *>mHistPosVec;
  vector<StMyMatchTrackToEmcHist *>mHistNegVec;

  vector<StMyTowerHist *> mHistTowerVec;
  vector<StMyClusterHist *>mHistClusterVec;
  
  vector<StMyTrackCut*> mTrackCuts;
  vector<StMyVertexCut*> mVertexCuts;
  vector<string> mTriggers;
  StMyTrackProjEmc *mTrackProj;
  ClassDef(StMyMatchTrackToEmcMaker, 0);
};
#endif
