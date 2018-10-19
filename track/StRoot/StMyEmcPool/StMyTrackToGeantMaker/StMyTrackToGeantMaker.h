#ifndef STMYTRACKTOGEANTMAKER
#define STMYTRACKTOGEANTMAKER

#include "StMaker.h"

#include <vector>
#include <map>


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
  void fillHist(const StMyMcTrack &mc, map<long, vector<StMyTrackGeant>> recoMap, StMyTrackToGeantHist *hist);
 private:
  const char *mFileName;
  TFile *mFile;

  StMyTrackToGeantHist *mHist;
  StMyTrackToGeantHist *mHistMc;
  StMyTrackToGeantHist *mHistMcPiP;
  StMyTrackToGeantHist *mHistMcPiM;
  StMyTrackToGeantHist *mHistMcKP;
  StMyTrackToGeantHist *mHistMcKM;

  vector<StMyTrackCut*> mTrackCuts;
  vector<StMyVertexCut*> mVertexCuts;
  ClassDef(StMyTrackToGeantMaker, 0);
};
#endif
