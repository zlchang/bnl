#include <iostream>
#include <utility>
#include <fstream>
using namespace std;
//#include "TObjArray.h"
#include "TMath.h"
#include "TFile.h"

#include "ReaderPythiaCrs.h"
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
#include "StMyJetPool/StMyJetSpin/MyJetYield.h"

#include "StMyJetPool/StMyPDF/StMyPDF.h"
#include "trgUtilities.h"
//#include "MyJet.h"

ReaderPythiaCrs::ReaderPythiaCrs(StJetEvent *parton, StJetEvent *particle, StPythiaEvent *pyevnt)
{

  mJetPartonEvent = parton;
  mJetParticleEvent = particle;
  mPythiaEvent = pyevnt;
  mFlag = false;

}
int ReaderPythiaCrs::Init()
{
  mFile = new TFile(mOutput, "recreate");
  mFile->cd();
  mJetYieldParton = new MyJetYieldPar("YieldParton");
  mJetYieldParticle = new MyJetYieldPar("YieldParticle");

  mPdf = new StMyPDF;
  for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
    mPdfnn[iset] = new StMyPDF("NNPDF30_lo_as_0130", iset);
  }
  return 1;
}
int ReaderPythiaCrs::Make(int iEvent)
{
  fillMcEvent(mJetPartonEvent, mJetYieldParton, false);
  fillMcEvent(mJetParticleEvent, mJetYieldParticle, mFlag);
  fillResponse(mFlag);
  return 1;
}
int ReaderPythiaCrs::Finish()
{
  mFile->cd();
  mJetYieldParton->Write("YieldPartonMc");
  mJetYieldParticle->Write("YieldParticleMc");
  mFile->Write();
  mFile->Close();
  /**/
  return 1;
}
void ReaderPythiaCrs::fillMcEvent(StJetEvent *jetParEvent, MyJetYieldPar *jetYield, bool flag)
{
  
  const StPythiaEvent *pythia = mPythiaEvent;
  float partonicPt = pythia->pt();
  float weight = partonicWeight(partonicPt);
  float x1 = pythia->x1();
  float x2 = pythia->x2();
  int flavor1 = pythia->particle(4)->GetPdgCode();
  //flavor1 = (flavor1 == 21) ? 0 : flavor1;
  int flavor2 = pythia->particle(5)->GetPdgCode();
  //flavor2 = (flavor2 == 21) ? 0 : flavor2;
  
  double q2 = TMath::Power(partonicPt, 2);

  double xf1 = mPdf->xfx(flavor1, x1, q2);
  double xf2 = mPdf->xfx(flavor2, x2, q2);
  double xf = xf1*xf2;
  
  //Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
  double wf[MyJetYieldPar::NMod];
  for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
    double xf1nn = mPdfnn[iset]->xfx(flavor1, x1, q2);
    double xf2nn = mPdfnn[iset]->xfx(flavor2, x2, q2);
    double xfnn = xf1nn*xf2nn;

    wf[iset] = xfnn/xf;
    //Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
    //jetYield->GetW(iset)->Fill(wf[iset], weight);
  }
  int proc = -1;
  if(flavor1 == 21 && flavor2 == 21){
    proc = 0;
  }else if(flavor1 != 21 && flavor2 != 21){
    proc = 1;
  }else
    proc = 2;
  Printf("proc = %d, with flavor1 = %d and flavor2 = %d", proc, flavor1, flavor2);

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
    if(flag){ 
       cout<<iJet<<" jet pt: "<<pt<<" dpt: "<<dpt<<endl;
       pt -= dpt;
    } 
    //yield
    if(eta > -0.9 && eta < 0.9)  {
      jetYield->GetPtEta()->Fill(pt, TMath::Abs(eta), weight);
      jetYield->GetPtEtaProc(proc)->Fill(pt, TMath::Abs(eta), weight);

      for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
        jetYield->GetPtEtaW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
      }
    }
  }
  //  }
}
void ReaderPythiaCrs::fillResponse(bool flag)
{
   const StPythiaEvent *pythia = mPythiaEvent;
   float partonicPt = pythia->pt();
   float weight = partonicWeight(partonicPt);

   int NJets = mJetParticleEvent->vertex()->numberOfJets();
   for(int iJet = 0; iJet < NJets; iJet++){
    StJetCandidate *jetcnd = mJetParticleEvent->vertex()->jet(iJet);
    float pt = jetcnd->pt();
    float eta = jetcnd->eta();
    map<string, float> rpho = jetcnd->ueDensity();
    float dpt = rpho["OffAxisConesR050"]*jetcnd->area();
    if(flag){ 
       //cout<<iJet<<" jet pt: "<<pt<<" dpt: "<<dpt<<endl;
       pt -= dpt;
    } 
    //yield
    if(eta > -0.9 && eta < 0.9)  {
      int index, partonCounter;
      StJetCandidate *parton = FindParJet(jetcnd, mJetPartonEvent, index, 0.9, partonCounter);
      if(parton){
     float ptpar = parton->pt();
      cout<<"Parton jet found fill "<<pt<<" "<<parton->pt()<<"\n";
      //response
      float etapar = parton->eta();
      mJetYieldParticle->FillResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight);
      }
    }
   }
} 
