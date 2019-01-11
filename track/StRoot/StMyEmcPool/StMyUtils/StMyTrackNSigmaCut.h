#ifndef STMYTRACKNSIGMACUT
#define STMYTRACKNSIGMACUT
#include "StMyTrackCut.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TMath.h"

class StMyTrackNSigmaPionCut : public StMyTrackCut{
 public:
 StMyTrackNSigmaPionCut(): _min(-990), _max(990){}
  bool operator()(const StMuTrack *track){
    double nsig = track->nSigmaPion();
    return nsig < _min || nsig > _max;
  }
  void setMinMax(double min, double max){_min = min; _max = max;}
  //private:
  double _min;
  double _max;
  
  ClassDef(StMyTrackNSigmaPionCut, 0);
};

class StMyTrackNSigmaElectronCut : public StMyTrackCut{
 public:
 StMyTrackNSigmaElectronCut(): _min(-990), _max(990){}
  bool operator()(const StMuTrack *track){
    double nsig = track->nSigmaElectron();
    return nsig < _min || nsig > _max;
  }
  void setMinMax(double min, double max){_min = min; _max = max;}
  //private:
  double _min;
  double _max;
  
  ClassDef(StMyTrackNSigmaElectronCut, 0);
};

class StMyTrackNSigmaKaonCut : public StMyTrackCut{
 public:
 StMyTrackNSigmaKaonCut(): _min(-990), _max(990){}
  bool operator()(const StMuTrack *track){
    double nsig = track->nSigmaKaon();
    return nsig < _min || nsig > _max;
  }
  void setMinMax(double min, double max){_min = min; _max = max;}
  //private:
  double _min;
  double _max;
  
  ClassDef(StMyTrackNSigmaKaonCut, 0);
};

class StMyTrackNSigmaProtonCut : public StMyTrackCut{
 public:
 StMyTrackNSigmaProtonCut(): _min(-990), _max(990){}
  bool operator()(const StMuTrack *track){
    double nsig = track->nSigmaProton();
    return nsig < _min || nsig > _max;
  }
  void setMinMax(double min, double max){_min = min; _max = max;}
  //private:
  double _min;
  double _max;
  
  ClassDef(StMyTrackNSigmaProtonCut, 0);
};

#endif
