#ifndef STMYTRACK
#define STMYTRACK
#include "TObject.h"

class StMyTrack : public TObject{
 public:
  StMyTrack(){};
  double mPt;
  double mEta;
  double mPhi;
  int mCharge;
  double mNSigmaPion;
  double mNSigmaElectron;
  ClassDef(StMyTrack, 0);
};

#endif
