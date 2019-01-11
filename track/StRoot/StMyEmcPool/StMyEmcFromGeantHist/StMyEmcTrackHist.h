#ifndef STMYEMCTRACKHIST
#define STMYEMCTRACKHIST
#include "TObject.h"
#include "StMyEmcPool/StMyObjs/StMyHist.h"

class StMyEmcTrackHist : public TObject{
 public:
  StMyEmcTrackHist(){};
  StMyEmcTrackHist(const char* name){
    int nbins = 30;
    double min = -1;
    double max = 5;
    mEpVsPt = new StMyHist2D(Form("%sEpVsPt", name), ";p_{T} [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4); 
    mNVsPt = new StMyHist2D(Form("%sNVsPt", name), ";p_{T} [GeV]; N", nbins, min, max, 10, 0, 10); 
  }
  StMyHist2D *mEpVsPt;
  StMyHist2D *mNVsPt;

  ClassDef(StMyEmcTrackHist, 1);
};
class StMyEmcTrackMatchHist : public TObject{
 public:
  StMyEmcTrackMatchHist(){};
  StMyEmcTrackMatchHist(const char* name){
    int nbins = 30;
    double min = -1;
    double max = 5;
    mEpVsPt = new StMyHist2D(Form("%sEpVsPt", name), ";p_{T} [GeV]; E/p", nbins, min, max, 150, -0.1, 1.4);
    mDVsPt = new StMyHist2D(Form("%sDVsPt", name), ";p_{T} [GeV]; D", nbins, min, max, 10, 0, 0.5);
  }
  StMyHist2D *mEpVsPt;
  StMyHist2D *mDVsPt;
  ClassDef(StMyEmcTrackMatchHist, 1);
};

#endif
