#ifndef STMYMCTRACK
#define STMYMCTRACK

#include "TObject.h"
#include "TString.h"
#include <vector>
#include "StMyMcTower.h"
#include "TLorentzVector.h"

#include "SFfunc.h"

class StMyMcTrack : public TObject{
 public:
  StMyMcTrack(){}
  TLorentzVector mom;
  //float pt;
  //float eta;
  long id;
  long  geantid;
  long ntpchits;
  unsigned int size() const{ return towers.size(); }
  float sumE() const{ 
    float sum = 0; 
    for(vector<StMyMcTower>::const_iterator it = towers.begin(); it != towers.end(); it++){
      sum += ((*it).dE);
    }
    double eta = mom.PseudoRapidity();
    double p = mom.P();
    double sf = samplingFraction(eta);
    if(sf > 0.001)
      return sum/sf;
    else{
      Printf("sf=%lf, eta=%lf, id=%ld, p=%lf\n", sf, eta, geantid, p); 
      return 0;
    }
  }
  vector<StMyMcTower> towers;
  ClassDef(StMyMcTrack, 1);
};
#endif
