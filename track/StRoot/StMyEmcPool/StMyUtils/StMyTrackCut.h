#ifndef STMYTRACKCUT
#define STMYTRACKCUT
#include "TObject.h"

class StMuTrack;

class StMyTrackCut : public TObject{
 public:
  virtual bool operator ()(const StMuTrack *track) = 0;
 private:
  ClassDef(StMyTrackCut, 0);
};
#endif
