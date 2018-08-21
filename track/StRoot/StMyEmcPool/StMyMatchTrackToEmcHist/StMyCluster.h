#ifndef STMYCLUSTER
#define STMYCLUSTER
class StMyCluster : public TObject{
 public:
  StMyCluster(){mId = 0; mE = 0; mEMax = 0; mHits = 0;}
  double mId;
  double mE;
  double mEMax;
  int mHits;
  ClassDef(StMyCluster, 0);
};
#endif
