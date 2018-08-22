#ifndef STMYTRACKFLAGCUT
#define STMYTRACKFLAGCUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackFlagCut : public StMyTrackCut{
 public:
 StMyTrackFlagCut(): _flag(0){}
  bool operator()(const StMuTrack *track){
    int flag = track->flag();
    return flag < _flag;
  }
  //private:
  int _flag;
  
  ClassDef(StMyTrackFlagCut, 0);
};
#endif
