#ifndef STMYMCTRACKHIST
#define STMYMCTRACKHIST
#include "TObject.h"
#include "StMyEmcPool/StMyObjs/StMyHist.h"

class StMyMcTrackHist : public TObject{
 public:
  StMyMcTrackHist(){};
  StMyMcTrackHist(const char* name){
    int nbins = 30;
    double min = -1;
    double max = 5;
    mEpVsP = new StMyHist2D(Form("%sEpVsp", name), ";p [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4); 
    mNVsP = new StMyHist2D(Form("%sNVsp", name), ";p [GeV]; N", nbins, min, max, 10, 0, 10); 
  }
  StMyHist2D *mEpVsP;
  StMyHist2D *mNVsP;

  ClassDef(StMyMcTrackHist, 1);
};
#endif
