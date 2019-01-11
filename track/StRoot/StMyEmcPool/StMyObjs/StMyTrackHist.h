#ifndef STMYTRACKHIST
#define STMYTRACKHIST

#include "TObject.h"
#include "StMyHist.h"
#include "StMyDef.h"

class StMyTrackHist : public TObject{
 public:
  
  StMyTrackHist(){}
  
  StMyTrackHist(const char *name){
    TH1::SetDefaultSumw2(kTRUE); 
    mHist = new TH1D(Form("%sTrack", name), "; track p_{T} [GeV]", yNbins, yMin, yMax); 

    mHistEta = new TH1D(Form("%sTrackEta", name), "; track #eta", yNbinsEta, yMinEta, yMaxEta); 
    mHistPhi = new TH1D(Form("%sTrackPhi", name), "; track #phi", yNbinsPhi, yMinPhi, yMaxPhi); 
    mHistNHits = new StMyHist2D(Form("%sTrackNHits", name), "; track N_{hits}", yNbins, yMin, yMax, yNhits, yMinHit, yMaxHit);
    //mHistNHitsFit = new TH1D(Form("%sTrackNHitsFit", name), "; track N_{fit}", yNhits, yMinHit, yMaxHit);
    mHistNHitsFitRatio = new StMyHist2D(Form("%sTrackNHitsFitRatio", name), "; track N_{fit}/N_{poss}", yNbins, yMin, yMax, 100, 0, 1);
    mHistDcaPt = new StMyHist2D(Form("%sTrackDcaPt", name), "; track p_{T} [GeV]; dca", yNbins, yMin, yMax, 30, 0, 3);
    //
    mHistFrac = new StMyProfile(Form("%sTrackFrac", name), "; track p_{T} [GeV]", yNbins, yMin, yMax);   
    int nBinsSig = 100;
    double minSig = -10;
    double maxSig = 10;
 
    mHistNSigmaElectron = new StMyHist2D(Form("%sNSigmaElectron", name), "; track p_{T} [GeV]; n#sigma_{e}", yNbins, yMin, yMax, nBinsSig, minSig, maxSig);
    mHistNSigmaPion = new StMyHist2D(Form("%sNSigmaPion", name), "; track p_{T} [GeV];n#sigma_{#pi}", yNbins, yMin, yMax, nBinsSig, minSig, maxSig); 
    mHistNSigmaKaon = new StMyHist2D(Form("%sNSigmaKaon", name), "; track p_{T} [GeV]; n#sigma_{K}", yNbins, yMin, yMax, nBinsSig, minSig, maxSig);
    mHistNSigmaProton = new StMyHist2D(Form("%sNSigmaProton", name), "; track p_{T} [GeV] ;n#sigma_{p}", yNbins, yMin, yMax, nBinsSig, minSig, maxSig); 
  }
  ~StMyTrackHist(){
    delete mHist;
    delete mHistEta;
    delete mHistPhi;
    delete mHistNHits;
    //delete mHistNHitsFit;
    delete mHistNHitsFitRatio;

    delete mHistDcaPt;

    delete mHistFrac;
    delete mHistNSigmaElectron;
    delete mHistNSigmaPion;
    delete mHistNSigmaKaon;
    delete mHistNSigmaProton;
  }
  
  TH1D *mHist;
  TH1D *mHistEta;
  TH1D *mHistPhi;
  StMyHist2D *mHistNHits;
  //TH1D *mHistNHitsFit;
  StMyHist2D *mHistNHitsFitRatio;
  
  StMyHist2D *mHistDcaPt;
  
  StMyProfile *mHistFrac;
  StMyHist2D *mHistNSigmaElectron;
  StMyHist2D *mHistNSigmaPion;
  StMyHist2D *mHistNSigmaKaon;
  StMyHist2D *mHistNSigmaProton;
  
  ClassDef(StMyTrackHist, 0);
};
#endif
