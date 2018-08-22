#ifndef STMYTRACKPTCUT
#define STMYTRACKPTCUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackPtMinCut : public TObject{
 public:
 StMyTrackPtMinCut(): _ptmin(0.2){}
  bool operator()(const StMuTrack *track){
    double pt = track->pt();
    return pt < _ptmin;
  }
  //private:
  double _ptmin;
  
  ClassDef(StMyTrackPtMinCut, 0);
};

class StMyTrackPtMaxCut : public TObject{
 public:
 StMyTrackPtMaxCut(): _ptmax(30){}
  bool operator()(const StMuTrack *track){
    double pt = track->pt();
    return pt > _ptmax;
  }
  //private:
  double _ptmax;

  ClassDef(StMyTrackPtMaxCut, 0);
};

#endif
