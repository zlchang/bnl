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
  double pt() const {return mom.Pt();}
  double eta() const {return mom.Eta();}
  double phi() const {return mom.Phi();}
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
  int maxTowerId(){
    float max = 0;
    int id = 0;
    //double eta = mom.PseudoRapidity();
    //double sf = samplingFraction(eta);
    //if(sf < 0.0001) return id;
    for(vector<StMyMcTower>::const_iterator it = towers.begin(); it != towers.end(); it++){
      float ee = (*it).dE;
      if(ee > max){
         max = ee;
         id = (*it).id;
      }
    }
    return id;
  }
  vector<StMyMcTower> towers;
  ClassDef(StMyMcTrack, 1);
};
#endif
