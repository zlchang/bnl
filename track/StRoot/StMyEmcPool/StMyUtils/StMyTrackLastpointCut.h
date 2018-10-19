#ifndef STMYTRACKLASTPOINTCUT
#define STMYTRACKLASTPOINTCUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackLastpointCut : public StMyTrackCut{
 public:
 StMyTrackLastpointCut(): _lastpoint(125.0){}
  bool operator()(const StMuTrack *track){
    double lastpoint = track->lastPoint().perp();
    return lastpoint < _lastpoint;
  }
  //private:
  double _lastpoint;
  
  ClassDef(StMyTrackLastpointCut, 0);
};
#endif
