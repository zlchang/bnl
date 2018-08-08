#ifndef STMYTRACK
#define STMYTRACK
class StMyTrack : public TObject{
 public:
  StMyTrack(){};
  double mPt;
  double mE;
  double mDist;
  double mNSigmaPion;
  double mNSigmaElectron;
  double mECluster;
  double mHitFrac;
  double mHitMax;
  ClassDef(StMyTrack, 0);
};
#endif
