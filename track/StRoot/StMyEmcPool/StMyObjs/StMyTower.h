#ifndef STMYTOWER
#define STMYTOWER
class StMyTower : public TObject{
 public:
  StMyTower(){mId = 0; mE = 0; mHits = 0;}
  StMyTower(const StMyTower &copy){mId = copy.mId; mE = copy.mE; mHits = copy.mHits;}
  int mId;
  double mE;
  int mHits;
  bool mStatus;
  ClassDef(StMyTower, 0);
};
#endif
