#ifndef STMYTRACKHITCUT
#define STMYTRACKHITCUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackHitMinCut : public StMyTrackCut{
 public:
 StMyTrackHitMinCut(): _hitmin(12){}
  bool operator()(const StMuTrack *track){
    int hit = track->nHits();
    return hit < _hitmin;
  }
  //private:
  int _hitmin;
  
  ClassDef(StMyTrackHitMinCut, 0);
};

class StMyTrackHitFracCut : public StMyTrackCut{
 public:
 StMyTrackHitFracCut(): _hitfrac(0.51){}
  bool operator()(const StMuTrack *track){
    double hit = track->nHits();
    double hitposs = track->nHitsPoss();
    return hit < hitposs*_hitfrac;
  }
  //private:
  double _hitfrac;

  ClassDef(StMyTrackHitFracCut, 0);
};

#endif
