#ifndef STMYTRACKDCACUT
#define STMYTRACKDCACUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackDcaCut : public StMyTrackCut{
 public:
 StMyTrackDcaCut(): _dcamax(3.0){}
  bool operator()(const StMuTrack *track){
    double dca = track->dcaGlobal().mag();
    return dca > _dcamax;
  }
  //private:
  double _dcamax;
  
  ClassDef(StMyTrackDcaCut, 0);
};
#endif
