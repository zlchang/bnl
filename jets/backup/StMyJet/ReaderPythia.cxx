#include <iostream>
#include <utility>
#include <fstream>
using namespace std;
//#include "TObjArray.h"
#include "TMath.h"
#include "TFile.h"

#include "ReaderPythia.h"
//#include "trgUtilities.h"
#include"StSpinPool/StJetEvent/StJetEvent.h"
#include"StSpinPool/StJetEvent/StJetVertex.h"
#include"StSpinPool/StJetEvent/StJetCandidate.h"
#include"StSpinPool/StJetEvent/StJetParticle.h"

#include"StSpinPool/StJetSkimEvent/StPythiaEvent.h"

//#include "StSpinPool/StUeEvent/StUeOffAxisConesEvent.h"
//#include "StSpinPool/StUeEvent/StUeEvent.h"
//#include "StSpinPool/StUeEvent/StUeVertex.h"
//#include "StSpinPool/StUeEvent/StUeJet.h"
//#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"
//#include "StSpinPool/StUeEvent/StUeOffAxisCones.h"
//#include "StMyJetPool/StMyDef/MyDef.h"
#include "StMyJetPool/StMyDef/funcUtilities.h"
#include "StMyJetPool/StMyJetSpin/MyJetMC.h"

//#include "MyJet.h"

ReaderPythia::ReaderPythia(StJetEvent *parton, StJetEvent *particle, StPythiaEvent *pyevnt)
{

  mJetPartonEvent = parton;
  mJetParticleEvent = particle;
  mPythiaEvent = pyevnt;
  mFlag = false;

}
int ReaderPythia::Init()
{
  mFile = new TFile(mOutput, "recreate");
  mFile->cd();
  mJetParton = new MyJetPar("Parton");
  mJetParticle = new MyJetPar("Particle");
  return 1;
}
int ReaderPythia::Make(int iEvent)
{
  fillMcEvent(mJetPartonEvent, mJetParton, false);
  fillMcEvent(mJetParticleEvent, mJetParticle, mFlag);
  return 1;
}
int ReaderPythia::Finish()
{
  mFile->cd();
  mJetParton->Write("PartonMc");
  mJetParticle->Write("ParticleMc");
  mFile->Write();
  mFile->Close();
  /**/
  return 1;
}
void ReaderPythia::fillMcEvent(StJetEvent *jetParEvent, MyJetPar *jetPar, bool flag)
{
  
  const StPythiaEvent *pythia = mPythiaEvent;
  float partonicPt = pythia->pt();
  float weight = partonicWeight(partonicPt);
  float x1 = pythia->x1();
  float x2 = pythia->x2();
  int flavor1 = pythia->particle(4)->GetPdgCode();
  flavor1 = (flavor1 == 21) ? 0 : flavor1;
  int flavor2 = pythia->particle(5)->GetPdgCode();
  flavor2 = (flavor2 == 21) ? 0 : flavor2;
  
  //float vz = jetParEvent->vertex()->position().z();
  //particle aLL                                                                                
  //cout<<"vz: "<<vz<<endl;
  //if(vz < 90. && vz > -90.){
  int NJets = jetParEvent->vertex()->numberOfJets();
  for(int iJet = 0; iJet < NJets; iJet++){
    StJetCandidate *jetcnd = jetParEvent->vertex()->jet(iJet);
    float pt = jetcnd->pt();
    float eta = jetcnd->eta();
    map<string, float> rpho = jetcnd->ueDensity();
    float dpt = rpho["OffAxisConesR050"]*jetcnd->area();
    cout<<iJet<<" jet pt: "<<pt<<" dpt: "<<dpt<<endl;
    if(flag) pt -= dpt;
    //
    if(eta > -0.9 && eta < 0.9){
      jetPar->GetX()->Fill(x1, pt, weight);
      jetPar->GetX()->Fill(x2, pt, weight);
      float waLL = pythia->partonALL();
      if(flavor1 == 0) jetPar->GetXg()->Fill(x1, pt, waLL*weight);
      if(flavor2 == 0) jetPar->GetXg()->Fill(x2, pt, waLL*weight);
      
      jetPar->GetPtPar()->Fill(pt, pt, weight);
      jetPar->GetPtParW2()->Fill(pt, pt, weight*weight);
      for(int imod = 0; imod < MyJetPar::NMod; imod++){
	float aLL = pythia->aLLNNPDF(imod);
        if(aLL > 1.){ aLL = 1.0; cout<<"setting aLL to "<<aLL<<" rep: "<<imod<<endl;}
        if(aLL < -1.){ aLL = -1.0; cout<<"setting aLL to "<<aLL<<" rep: "<<imod<<endl;}
	//cout<<aLL<<endl;
	//if(imod == 0 && pt > 23 && pt < 24) cout<<pt<<" "<<aLL<<endl;
	jetPar->GetALLPar(imod)->Fill(pt, aLL, weight);
	jetPar->GetALLPar2D(imod)->Fill(pt, aLL, weight);
	//w^2                                                                                
	jetPar->GetALLParW2(imod)->Fill(pt, aLL, weight*weight);
      }
    }
  }
  //  }
}
