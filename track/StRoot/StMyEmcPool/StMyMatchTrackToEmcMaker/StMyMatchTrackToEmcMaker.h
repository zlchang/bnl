#ifndef STMYMATCHTRACKTOEMCMAKER
#define STMYMATCHTRACKTOEMCMAKER

#include "StMaker.h"

#include <vector>

class TFile;
class StEmcGeom;
class StMyTrackMatch;
class StMyTower;
class StMyCluster;
class StMyTowerHist;
class StMyClusterHist;
class StMyMatchTrackToEmcHist;
class StMyTrackCut;

class StMyMatchTrackToEmcMaker : public StMaker{

public:
 StMyMatchTrackToEmcMaker(const char* name = "StMyMatchTrackToEmcMaker"):StMaker(name){
   mFileName = "MyMatchTrackToEmc.root";
  }
  int Init();
  int Make();
  int Finish();
  void SetOutfile(const char* file){mFileName =  file; }  
 protected:
  StEmcGeom *mBemcGeom;
  void fillHist(const StMyTrackMatch &track, StMyMatchTrackToEmcHist *hist);
  void fillHistCluster(const StMyCluster &cluster, StMyClusterHist *hist);
  void fillHistTower(const StMyTower &tower, StMyTowerHist *hist);
 private:
  const char *mFileName;
  TFile *mFile;
  StMyMatchTrackToEmcHist *mHistNoCut;
  StMyMatchTrackToEmcHist *mHist;
  StMyMatchTrackToEmcHist *mHistPos;
  StMyMatchTrackToEmcHist *mHistNeg;

  StMyTowerHist *mHistTower;
  StMyClusterHist *mHistCluster;
  vector<StMyTrackCut*> mTrackCuts;
  ClassDef(StMyMatchTrackToEmcMaker, 0);
};
#endif
