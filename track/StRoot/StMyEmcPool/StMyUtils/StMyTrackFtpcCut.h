#ifndef STMYTRACKFTPCCUT
#define STMYTRACKFTPCCUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackFtpcCut : public StMyTrackCut{
 public:
  StMyTrackFtpcCut(){}
  bool operator()(const StMuTrack *track){
    return track->topologyMap().trackFtpcEast() || track->topologyMap().trackFtpcWest();
  }
  //private:
  
  ClassDef(StMyTrackFtpcCut, 0);
};
#endif
