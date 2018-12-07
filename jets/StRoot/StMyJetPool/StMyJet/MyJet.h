#ifndef MYJET
#define MYJET
#include "TObject.h"
#include "TVector3.h"
//#include "StSpinPool/StJetEvent/StJetCandidate.h"
//#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"

class StJetCandidate;
class StUeOffAxisConesJet;
class StJetElement;
class StUeOffAxisCones;
class MyJet : public TObject{
public:
//  MyJet(){mJet = 0x0; mJetUe = 0x0;}
  MyJet(StJetCandidate *j = NULL, StUeOffAxisConesJet *u = NULL);
/*{
    mJet = j; mJetUe = u;
    if(u != NULL){
      int nCones = mJetUe->numberOfCones();
      for(int icone = 0; icone < nCones; icone++){
        StUeOffAxisCones *jcone = mJetUe->cone(icone);
        for(int ui = 0; ui < jcone->numberOfTracks(); ui++){
          jcone->track(ui)->setJet(j);
        }
        for(int ui = 0; ui < jcone->numberOfTowers(); ui++){
          jcone->tower(ui)->setJet(j);
        }
      }
 
    }
  }*/
  int Compare(const TObject* obj) const;
/*
  {
    MyJet* p = (MyJet*) obj;
    if(pt() > p->pt()) return -1;
    else if(pt() < p->pt()) return 1;
    return 0;
  }
*/
  StJetCandidate *jet() const { return mJet;}
  StUeOffAxisConesJet *jetUe() const { return mJetUe; }
  float pt() const;
  float eta() const;
  float phi() const;
  float rt() const;
  int trackmult() const;//{mJet->numberOfTracks(); }
  int towermult() const;//{ mJet->numberOfTowers(); }
  float uedensity(const char *branch = "OffAxisConesR050") const;//{ map<string, float> uedensity = mJet->ueDensity(); return density[string(branch)];}
  float area() const ;//{ mJetCnd->area(); }
  float dpt() const { return area()*uedensity(); }
  int trackmultUe() const;
  float sumtrackUe() const;
  int towermultUe() const;
  float sumtowerUe() const;
  float rtUe() const { if(sumtrackUe()+sumtowerUe() > 0) return sumtowerUe()/(sumtrackUe()+sumtowerUe()); else return 0;}
  float trackjtUe(int ic, int it) const;
  float towerjtUe(int ic, int it) const;
  bool IsSortable() const { return kTRUE; }
  /*bool operator > (const MyJet &a) const {
    if(pt > a.pt) return true;
    else return false;
  }*/
protected:
  TVector3 localMomentumUe(StUeOffAxisCones *cone, StJetElement *element) const;
private:
  StJetCandidate *mJet;
  StUeOffAxisConesJet *mJetUe; 
  ClassDef(MyJet, 0);
};
#endif
