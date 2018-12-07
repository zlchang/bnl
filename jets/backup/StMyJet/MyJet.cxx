#include "MyJet.h"

#include "StSpinPool/StJetEvent/StJetCandidate.h"
#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisCones.h"
//ClassImp(MyJet);
int MyJet::Compare(const TObject* obj) const{
  MyJet* p = (MyJet*) obj;
  if(pt() > p->pt()) return -1;
  else if(pt() < p->pt()) return 1;
  return 0;
}
MyJet::MyJet(StJetCandidate *j, StUeOffAxisConesJet *u)
{
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
}

float MyJet::pt() const{ return mJet->pt(); }
float MyJet::eta() const{ return mJet->eta(); }
float MyJet::phi() const{ return mJet->phi(); }
float MyJet::rt() const{ return mJet->rt(); }
int MyJet::trackmult() const{return mJet->numberOfTracks(); }
int MyJet::towermult() const { return mJet->numberOfTowers(); }
float MyJet::uedensity(const char *branch) const{ map<string, float> ue = mJet->ueDensity(); return ue[string(branch)];}
float MyJet::area() const { return mJet->area(); }
int MyJet::trackmultUe() const
{
  if(! mJetUe) return 0;
  int mult = 0;
  int nCones = mJetUe->numberOfCones();
  for(int icone = 0; icone < nCones; icone++){
    StUeOffAxisCones *jcone = mJetUe->cone(icone);
    mult += jcone->numberOfTracks();
  }
  return mult;

}
float MyJet::sumtrackUe() const
{
  if(! mJetUe) return 0;
  float sum = 0;
  int nCones = mJetUe->numberOfCones();
  for(int icone = 0; icone < nCones; icone++){
    StUeOffAxisCones *jcone = mJetUe->cone(icone);
    float areaue = 3.1416 * TMath::Power(jcone->radius(), 2);
    float den = 0.;
    for(int it = 0; it < jcone->numberOfTracks(); it++){
      den += jcone->track(it)->pt();
    }
    sum += den/areaue;
  }
  return sum/(nCones+0.);
}
int MyJet::towermultUe() const
{
  if(! mJetUe) return 0;
  int mult = 0;
  int nCones = mJetUe->numberOfCones();
  for(int icone = 0; icone < nCones; icone++){
    StUeOffAxisCones *jcone = mJetUe->cone(icone);
    mult += jcone->numberOfTowers();
  }
  return mult;

}
float MyJet::sumtowerUe() const
{
  if(! mJetUe) return 0;
  float sum = 0;
  int nCones = mJetUe->numberOfCones();
  for(int icone = 0; icone < nCones; icone++){
    StUeOffAxisCones *jcone = mJetUe->cone(icone);
    float areaue = 3.1416 * TMath::Power(jcone->radius(), 2);
    float den = 0.;
    for(int it = 0; it < jcone->numberOfTowers(); it++){
      den += jcone->tower(it)->pt();
    }
    sum += den/areaue;
  }
  return sum/(nCones+0.);
}
float MyJet::trackjtUe(int ic, int it) const
{
  StUeOffAxisCones *jcone = mJetUe->cone(ic);
  StJetElement *track = jcone->track(it);
  return localMomentumUe(jcone, track).Perp();
}
float MyJet::towerjtUe(int ic, int it) const
{
  StUeOffAxisCones *jcone = mJetUe->cone(ic);
  StJetElement *tower = jcone->tower(it);
  return localMomentumUe(jcone, tower).Perp();
}
TVector3 MyJet::localMomentumUe(StUeOffAxisCones *cone, StJetElement *element) const
{
  TVector3 vec;
  vec.SetPtEtaPhi(cone->pt(), cone->eta(), cone->phi());
  TVector3 longUnit = vec.Unit();
  TVector3 normUnit = TVector3(0,0,1).Cross(longUnit).Unit();
  TVector3 sideUnit = longUnit.Cross(normUnit);
  TVector3 mom = element->momentum();
  return TVector3(mom.Dot(sideUnit),mom.Dot(normUnit),mom.Dot(longUnit));
}
