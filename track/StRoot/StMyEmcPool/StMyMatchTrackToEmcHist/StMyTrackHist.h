#ifndef STMYTRACKHIST
#define STMYTRACKHIST

#include "TObject.h"
#include "StMyEmcPool/StMyObjs/StMyHist.h"


class StMyTrackHist : public TObject{
 public:
  
  StMyTrackHist(){}
  
  StMyTrackHist(const char *name){
    int nbins = 55;
    double min = -1;
    double max = 10;
    TH1::SetDefaultSumw2(kTRUE); 
    mHistTrack = new TH1D(Form("%sTrack", name), "; track p_{T} [GeV]", nbins, min, max); 
    mHistTrackFrac = new StMyProfile(Form("%sTrackFrac", name), "; track p_{T} [GeV]", nbins, min, max);    
    mHistNSigmaElectron = new TH1D(Form("%sNSigmaElectron", name), ";n#sigma_{e}", 200, -10, 10);
    mHistNSigmaPion = new TH1D(Form("%sNSigmaPion", name), ";n#sigma_{e}", 200, -10, 10); 
  }
  ~StMyTrackHist(){
    delete mHistTrack;
    delete mHistTrackFrac;
    delete mHistNSigmaElectron;
    delete mHistNSigmaPion;
  }
  
  TH1D *mHistTrack;
  StMyProfile *mHistTrackFrac;
  TH1D *mHistNSigmaElectron;
  TH1D *mHistNSigmaPion;
  
  ClassDef(StMyTrackHist, 0);
};
#endif
