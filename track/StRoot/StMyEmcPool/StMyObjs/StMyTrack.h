#ifndef STMYTRACK
#define STMYTRACK
#include "TObject.h"

class StMyTrack : public TObject{
 public:
  StMyTrack(){mPt = 0; mEta = 0; mPhi = 0; 
              mCharge = 0; mNSigmaPion = 0.;
              mNSigmaElectron = 0.;
  };
  double mPt;
  double mEta;
  double mPhi;
  int mCharge;
  int mNHits;
  int mNHitsFit;
  double mNHitsFitRatio;
  double mDca;
  double mNSigmaPion;
  double mNSigmaElectron;
  double mNSigmaKaon;
  double mNSigmaProton;
  ClassDef(StMyTrack, 0);
};

#endif
