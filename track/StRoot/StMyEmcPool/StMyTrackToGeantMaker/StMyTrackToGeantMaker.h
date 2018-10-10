#ifndef STMYTRACKTOGEANTMAKER
#define STMYTRACKTOGEANTMAKER

#include "StMaker.h"

#include <vector>

class TFile;
class StMyTrackGeant;
class StMyMcTrack;
class StMyTrackToGeantHist;
class StMyTrackCut;
class StMyVertexCut;

class StMyTrackToGeantMaker : public StMaker{

public:
 StMyTrackToGeantMaker(const char* name = "StMyTrackToGeantMaker"):StMaker(name){
   mFileName = "MyTrackToGeant.root";
  }
  int Init();
  int Make();
  int Finish();
  void addTrackCut(StMyTrackCut* tcut){ mTrackCuts.push_back(tcut); }
  void addVertexCut(StMyVertexCut* vcut){ mVertexCuts.push_back(vcut); }
  void SetOutfile(const char* file){mFileName =  file; }  
 protected:
  void fillHist(const StMyTrackGeant &track, const StMyMcTrack &mcTrack, StMyTrackToGeantHist *hist);
 private:
  const char *mFileName;
  TFile *mFile;

  StMyTrackToGeantHist *mHist;

  vector<StMyTrackCut*> mTrackCuts;
  vector<StMyVertexCut*> mVertexCuts;
  ClassDef(StMyTrackToGeantMaker, 0);
};
#endif
