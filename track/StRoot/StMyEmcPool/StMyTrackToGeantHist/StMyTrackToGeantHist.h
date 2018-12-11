#ifndef STMYTRACKTOGEANTHIST
#define STMYTRACKTOGEANTHIST
#include "TObject.h"
#include "StMyEmcPool/StMyObjs/StMyHist.h"
#include "StMyEmcPool/StMyObjs/StMyTrackHist.h"
#include "StMyEmcPool/StMyObjs/StMyMcTrackHist.h"

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
    mNHitsMcVsMu = new StMyHist2D(Form("%sNHitsMcVsMu", name), ";N_{Hits}; M/C N_{Hits}", yNhits, yMinHit, yMaxHit, yNhits, yMinHit, yMaxHit);

    int nbinsPhi = 126;
    double minPhi = -3.15;
    double maxPhi = 3.15;
    mMatchVsPhi = new StMyProfile(Form("%sMatchVsPhi", name), ";#phi; ratio", nbinsPhi, minPhi, maxPhi);
    mQaVsPhi = new StMyHist2D(Form("%sQaVsPhi", name), ";#phi; QA", nbinsPhi, minPhi, maxPhi, 100, 0, 100);
    mPhiMcVsMu = new StMyHist2D(Form("%sPhiMcVsMu", name), ";#phi; M/C #phi", nbinsPhi, minPhi, maxPhi, nbinsPhi, minPhi, maxPhi);
    mDvZ = new TH1D(Form("%sDvZ", name), ";#Delta V_{z} [cm]", 40, -10, 10);

    mTrack = new StMyTrackHist(Form("%s",name));
    mMcTrack = new StMyMcTrackHist(Form("Mc%s",name));
  }
  //StMyHist2D *mEpVsP;
  //StMyHist2D *mNVsP;
  StMyProfile *mMatchVsPt;
  StMyProfile *mMatchVsEta;
  StMyProfile *mMatchVsPhi;
  StMyHist2D *mQaVsPt;
  StMyHist2D *mQaVsEta;
  StMyHist2D *mQaVsPhi;

  StMyHist2D *mPtMcVsMu;
  StMyHist2D *mEtaMcVsMu;
  StMyHist2D *mPhiMcVsMu;
  StMyHist2D *mNHitsMcVsMu;

  TH1D *mDvZ;
  
  StMyTrackHist *mTrack;
  StMyMcTrackHist *mMcTrack;
  ClassDef(StMyTrackToGeantHist, 1);
};
#endif
