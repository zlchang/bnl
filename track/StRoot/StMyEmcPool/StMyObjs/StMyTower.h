#ifndef STMYTOWER
#define STMYTOWER
class StMyTower : public TObject{
 public:
  StMyTower(){mId = 0; mE = 0; mHits = 0;}
  int mId;
  double mE;
  int mHits;
  ClassDef(StMyTower, 0);
};
#endif
