#ifndef STMYTRACKETACUT
#define STMYTRACKETACUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackEtaMinCut : public StMyTrackCut{
 public:
 StMyTrackEtaMinCut(): _etamin(-2.5){}
  bool operator()(const StMuTrack *track){
    double eta = track->eta();
    return eta < _etamin;
  }
  //private:
  double _etamin;
  
  ClassDef(StMyTrackEtaMinCut, 0);
};

class StMyTrackEtaMaxCut : public StMyTrackCut{
 public:
 StMyTrackEtaMaxCut(): _etamax(2.5){}
  bool operator()(const StMuTrack *track){
    double eta = track->eta();
    return eta > _etamax;
  }
  //private:
  double _etamax;

  ClassDef(StMyTrackEtaMaxCut, 0);
};

#endif
