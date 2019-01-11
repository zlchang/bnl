#ifndef STMYMCTRACKHIST
#define STMYMCTRACKHIST

#include "TObject.h"
//#include "StMyHist.h"
#include "StMyDef.h"

class StMyMcTrackHist : public TObject{
 public:
  
  StMyMcTrackHist(){}
  
  StMyMcTrackHist(const char *name){
    TH1::SetDefaultSumw2(kTRUE); 
    mHist = new TH1D(Form("%sTrack", name), "; track p_{T} [GeV]", yNbins, yMin, yMax); 

    mHistEta = new TH1D(Form("%sTrackEta", name), "; track #eta", yNbinsEta, yMinEta, yMaxEta); 
    mHistPhi = new TH1D(Form("%sTrackPhi", name), "; track #phi", yNbinsPhi, yMinPhi, yMaxPhi); 
    mHistNHits = new TH1D(Form("%sTrackNHits", name), "; track N_{hits}", yNhits, yMinHit, yMaxHit);
  }
  ~StMyMcTrackHist(){
    delete mHist;
    delete mHistEta;
    delete mHistPhi;
    delete mHistNHits;
  }
  
  TH1D *mHist;
  TH1D *mHistEta;
  TH1D *mHistPhi;
  TH1D *mHistNHits;

  
  ClassDef(StMyMcTrackHist, 0);
};
#endif
