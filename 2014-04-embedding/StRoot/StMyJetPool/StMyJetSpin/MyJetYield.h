#ifndef ROOT_MYJETYIELD
#define ROOT_MYJETYIELD
#include "TObject.h"
#include "TH2F.h"
#include "TProfile2D.h"
#include"StMyJetPool/StMyDef/MyDef.h"

class MyJetYield : public TObject{

 public:
  MyJetYield(){};
  MyJetYield(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hjetptvtxprof = new TProfile2D(Form("%sJetPtVtxMcProf",name), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetpartonratioprof = new TProfile2D(Form("%sJetPartonMatchProf", name), ";p_{T} [GeV];#eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetparticleratioprof = new TProfile2D(Form("%sJetParticleMatchProf", name), ";p_{T} [GeV];#eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    //response matrix
    //hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yParPtBins);
    int N2d = yNPtBins*yNEtaBins;
    int N2dPar = yNParPtBins*yNEtaBins;
    hjettwojet = new TH2F(Form("%sTwoJet", name), ";jet p_{T} [GeV];jet p_{T} [GeV]", N2d, 0, N2d, N2d, 0, N2d);
    hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);
    hjetptparticleres = new TH2F(Form("%sJetPtVsParticleJetPtRes", name), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);//yNPtBins, yPtBins, yNPtBins, yParPtBins);
    //hjetptparticlecrrres = new TH2F(Form("%sJetPtVsParticleJetPtCrrRes", name), ";jet p_{T} [GeV];particle jet p_{T,crr} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);//yNPtBins, yPtBins, yNPtBins, yParPtBins);
    //detector spectrum
    hjetdetector = new TH2F(Form("%sDetectorJet", name), ";detector jet _{T} [GeV]", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    //particle spectrum
    hjetparton = new TH2F(Form("%sPartonJet", name), ";parton jet _{T} [GeV]", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    hjetpartonT = new TH2F(Form("%sPartonTwoJet", name), ";parton jet _{T} [GeV]", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    hjetparticle = new TH2F(Form("%sParticleJet", name), ";particle jet p_{T} [GeV]", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    hjetparticleT = new TH2F(Form("%sParticleTwoJet", name), ";particle jet p_{T} [GeV]", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins); 
    //hjetparticlecrr = new TH2F(Form("%sParticleJetCrr", name), ";particle jet p_{T} [GeV]", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    //hjetparticlecrrT = new TH2F(Form("%sParticleTwoJetCrr", name), ";particle jet p_{T} [GeV]", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins); 
    
  }
  ~MyJetYield(){
    delete hjetptvtxprof;
    delete hjetpartonratioprof;
    delete hjetparticleratioprof;
    delete hjettwojet;
    //response
    delete hjetptpartonres;
    delete hjetptparticleres;
    //delete hjetptparticlecrrres;
    delete hjetdetector;
    delete hjetparton;
    delete hjetpartonT;
    delete hjetparticle;
    delete hjetparticleT;
    //delete hjetparticlecrr;
    //delete hjetparticlecrrT;
  }
  void Add(MyJetYield *jetyield, float w);
  //response matrix
  TH2F* GetJetPartonResMC() const{
    return hjetptpartonres;
  }	
  TH2F* GetJetParticleResMC() const{
    return hjetptparticleres;
  }
  //TH2F* GetJetParticleCrrResMC() const{
    //return hjetptparticlecrrres;
  //}
  TH2F *GetJetDetectorJet() const{
    return hjetdetector;
  }
  TH2F *GetJetPartonJet() const{
    return hjetparton;
  }
  TH2F *GetJetPartonTwoJet() const{
    return hjetpartonT;
  }
  TH2F *GetJetParticleJet() const{
    return hjetparticle;
  }
  TH2F *GetJetParticleTwoJet() const{
    return hjetparticleT;
  }
  //TH2F *GetJetParticleJetCrr() const{
    //return hjetparticlecrr;
  //}
  //TH2F *GetJetParticleTwoJetCrr() const{
    //return hjetparticlecrrT;
  //}
  TProfile2D *GetJetVertexProf() const{
    return hjetptvtxprof;
  }
  TProfile2D *GetJetPartonRatioProfile() const{
    return hjetpartonratioprof;
  }
  TProfile2D *GetJetParticleRatioProfile() const{
    return hjetparticleratioprof;
  }
  TH2F *GetJetTwoJet() const{
    return hjettwojet;
  }
  void FillJetTwoJet(float pt1, float eta1, float pt2, float eta2, float weight);
  void FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight);
  void FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight);
  //void FillParticleCrrResponse(float pt1, float eta1, float pt2, float eta2, float weight);
 private:
  TProfile2D *hjetptvtxprof;
  TProfile2D *hjetpartonratioprof;
  TProfile2D *hjetparticleratioprof;
  TH2F *hjettwojet;
  //response
  TH2F *hjetptpartonres;
  TH2F *hjetptparticleres;
  //TH2F *hjetptparticlecrrres;
  //th2f
  TH2F *hjetdetector;
  //th2f par
  TH2F *hjetparton;
  TH2F *hjetpartonT;
  TH2F *hjetparticle;
  TH2F *hjetparticleT;
  //TH2F *hjetparticlecrr;
  //TH2F *hjetparticlecrrT;
  ClassDef(MyJetYield, 1);
};

class MyJetYieldPar : public TObject{
 public:
  MyJetYieldPar(){}
  MyJetYieldPar(const char* name){
    hpteta = new TH2F(Form("%sJetEta", name), ";p_{T} [GeV]; #eta", yNPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    //hptetacrr = new TH2F(Form("%sJetEtaCrr", name), ";p_{T} [GeV]; #eta", yNPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
  }
  ~MyJetYieldPar(){
    delete hpteta;
    //delete hptetacrr;
  }
  TH2F *GetPtEta() const{ return hpteta;} 
  //TH2F *GetPtEtaCrr() const{ return hptetacrr;} 
  void Add(MyJetYieldPar *par, float w = 1.);
 private:
  TH2F *hpteta;
  //TH2F *hptetacrr;
  ClassDef(MyJetYieldPar, 1);
};
#endif
