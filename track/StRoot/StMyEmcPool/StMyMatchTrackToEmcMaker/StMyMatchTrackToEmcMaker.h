#ifndef STMYMATCHTRACKTOEMCMAKER
#define STMYMATCHTRACKTOEMCMAKER

#include "StMaker.h"

class StMyMatchTrackToEmcHist;
class TFile;
class StEmcGeom;
class StMyTrack;

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
  void fillHists(const StMyTrack &track, StMyMatchTrackToEmcHist *hist);
 private:
  const char *mFileName;
  TFile *mFile;
  StMyMatchTrackToEmcHist *mHistNoCut;
  StMyMatchTrackToEmcHist *mHist;
  StMyMatchTrackToEmcHist *mHistPos;
  StMyMatchTrackToEmcHist *mHistNeg;

  ClassDef(StMyMatchTrackToEmcMaker, 0);
};
#endif
