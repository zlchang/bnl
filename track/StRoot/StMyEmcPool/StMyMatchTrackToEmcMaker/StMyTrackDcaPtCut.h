#ifndef STMYTRACKDCAPTCUT
#define STMYTRACKDCAPTCUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackDcaPtCut : public TObject{
 public:
 StMyTrackDcaPtCut(): _pt1(0.5), _dca1(2.0), _pt2(1.5), _dca2(1.0){}
  bool operator()(const StMuTrack *track){
    double pt = track->pt();
    double dca = track->dcaGlobal().mag();
    if(pt < _pt1) return TMath::Abs(dca) > _dca1;
    else if(pt > _pt2)
      return TMath::Abs(dca) > _dca2;
    else
      return TMath::Abs(dca) > TMath::Abs(_dca1 + (_dca1-_dca2)/(_pt1-_pt2)*(pt-_pt1)); 
  }
 private:
  double _pt1;
  double _dca1;
  double _pt2;
  double _dca2;
  
  ClassDef(StMyTrackDcaPtCut, 0);
};
#endif
