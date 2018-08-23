#ifndef STMYTOWERHIST
#define STMYTOWERHIST
#include "TObject.h"
#include "TH1D.h"

class StMyTowerHist : public TObject
{
  public:
  StMyTowerHist(){};
  StMyTowerHist(const char* name){
    hE = new TH1D(Form("%sE", name), "; tower E [GeV]", 110, -0.1, 10);
    hHits = new TH1D(Form("%sHits", name), "; towerId; nHits" , 4800, 1, 4801);
    hNHits = new TH1D(Form("%sNHits", name), ";  nHits" , 10, 0, 10);
  }
 ~StMyTowerHist(){
    delete hE;
    delete hHits;
    delete hNHits;
 }

  TH1D *hE;
  TH1D *hHits;
  TH1D *hNHits;
  ClassDef(StMyTowerHist, 1);  
};
#endif
