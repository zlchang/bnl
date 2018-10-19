#ifndef STMYTRACKGEANT
#define STMYTRACKGEANT
#include "StMyTrack.h"

class StMyTrackGeant : public StMyTrack{
 public:
  StMyTrackGeant(){StMyTrack(); mKey=0;}
  long mKey;
  int mQa;
  int mNhits;
  ClassDef(StMyTrackGeant, 0);
};

#endif
