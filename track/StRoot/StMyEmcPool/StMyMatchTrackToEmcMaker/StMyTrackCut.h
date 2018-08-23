#ifndef STMYTRACKCUT
#define STMYTRACKCUT
class StMuTrack;

class StMyTrackCut : public TObject{
 public:
  virtual bool operator ()(const StMuTrack *track) = 0;
 private:
  ClassDef(StMyTrackCut, 0);
};
#endif
