#ifndef STMYMCTRACKHIST
#define STMYMCTRACKHIST
#include "TObject.h"
#include "StMyEmcPool/StMyMatchTrackToEmcHist/StMyHist.h"

class StMyMcTrackHist : public TObject{
 public:
  StMyMcTrackHist(){};
  StMyMcTrackHist(const char* name){
    int nbins = 30;
    double min = -1;
    double max = 5;
    mEptVsPt = new StMyHist2D(Form("%sEptVspt", name), ";p_{T} [GeV]; E/p_{T}", nbins, min, max, 150, -0.1, 1.4); 
    mNVsPt = new StMyHist2D(Form("%sNVspt", name), ";p_{T} [GeV]; N", nbins, min, max, 10, 0, 10); 
  }
  StMyHist2D *mEptVsPt;
  StMyHist2D *mNVsPt;

  ClassDef(StMyMcTrackHist, 1);
};
#endif
