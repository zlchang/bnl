#ifndef STMYTRACKTOGEANTHIST
#define STMYTRACKTOGEANTHIST
#include "TObject.h"
#include "StMyEmcPool/StMyObjs/StMyHist.h"

class StMyTrackToGeantHist : public TObject{
 public:
  StMyTrackToGeantHist(){};
  StMyTrackToGeantHist(const char* name){
    int nbins = 50;
    double min = 0;
    double max = 5;

    mMatchVsPt = new StMyProfile(Form("%sMatchVsPt", name), ";p_{T} [GeV]; ratio", nbins, min, max);
    mQaVsPt = new StMyHist2D(Form("%sQaVsPt", name), ";p_{T} [GeV]; QA", nbins, min, max, 100, 0, 100);
    mPtMcVsMu = new StMyHist2D(Form("%sPtMcVsMu", name), ";p_{T} [GeV]; M/C p_{T} [GeV]", nbins, min, max, nbins, min, max);
    int nbinsEta = 50;
    double minEta = -2.5;
    double maxEta = 2.5;
    mMatchVsEta = new StMyProfile(Form("%sMatchVsEta", name), ";#eta; ratio", nbinsEta, minEta, maxEta);
    mQaVsEta = new StMyHist2D(Form("%sQaVsEta", name), ";#eta; QA", nbinsEta, minEta, maxEta, 100, 0, 100);
    mEtaMcVsMu = new StMyHist2D(Form("%sEtaMcVsMu", name), ";#eta; M/C #eta", nbinsEta, minEta, maxEta, nbinsEta, minEta, maxEta);
    mDvZ = new TH1D(Form("%sDvZ", name), ";#Delta V_{z} [cm]", 40, -10, 10);
  }
  //StMyHist2D *mEpVsP;
  //StMyHist2D *mNVsP;
  StMyProfile *mMatchVsPt;
  StMyProfile *mMatchVsEta;
  StMyHist2D *mQaVsPt;
  StMyHist2D *mQaVsEta;

  StMyHist2D *mPtMcVsMu;
  StMyHist2D *mEtaMcVsMu;

  TH1D *mDvZ;
  ClassDef(StMyTrackToGeantHist, 1);
};
#endif
