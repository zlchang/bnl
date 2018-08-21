#ifndef STMYCLUSTERHIST
#define STMYCLUSTERHIST
#include "TObject.h"
#include "TH1D.h"
class StMyClusterHist : public TObject
{

 public:
  StMyClusterHist(){};
  StMyClusterHist(const char* name){
    hE = new TH1D(Form("%sE", name), "; cluster E [GeV]", 110, -0.1, 10);
    //hHits = new TH1D(Form("%sHits", name), "; cluster Id; nHits" , 4800, 1, 4801);
    hNHits = new TH1D(Form("%sNHits", name), ";  nHits" , 10, 0, 10);
    hMaxFrac = new TH1D(Form("%sMaxFrac", name), "; max frac", 101, 0, 1.01);
  }
 ~StMyClusterHist(){
    delete hE;
    delete hNHits;
    delete hMaxFrac;
 }	
  TH1D *hE;
  //TH1D *hHits;
  TH1D *hNHits;
  TH1D *hMaxFrac;
  ClassDef(StMyClusterHist, 1);
};
#endif
