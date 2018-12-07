#ifndef ROOT_MYJETTEST
#define ROOT_MYJETTEST
#include "TObject.h"
#include "TH2F.h"
#include "TProfile2D.h"
//#include"StMyJetPool/StMyDef/MyDef.h"

class MyJetTest : public TObject{

 public:
  MyJetTest(){};
  MyJetTest(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hjetptvtxprof = new TProfile(Form("%sJetPtVtxMcProf",name), ";p_{T} [GeV]", 100, 0, 100);
    hjetpartonratioprof = new TProfile(Form("%sJetPartonMatchProf", name), ";p_{T} [GeV]", 100, 0, 100);
    hjetparticleratioprof = new TProfile(Form("%sJetParticleMatchProf", name), ";p_{T} [GeV]", 100, 0, 100);
    //response matrix
    //hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yParPtBins);
    hjettwojet = new TH2F(Form("%sTwoJet", name), ";jet p_{T} [GeV];jet p_{T} [GeV]", 100, 0, 100, 100, 0, 100);
    hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", 100, 0, 100, 100, 0, 100);
    hjetptparticleres = new TH2F(Form("%sJetPtVsParticleJetPtRes", name), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", 100, 0, 100, 100, 0, 100);
    //profile
    hjetptpartonprof = new TProfile(Form("%sJetPtVsPartonJetPtProf", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", 100, 0, 100);
    hjetptparticleprof = new TProfile(Form("%sJetPtVsParticleJetPtProf", name), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", 100, 0, 100);

    //detector spectrum
    hjetdetector = new TH1F(Form("%sDetectorJet", name), ";detector jet _{T} [GeV]", 100, 0, 100);
    //particle spectrum
    hjetparton = new TH1F(Form("%sPartonJet", name), ";parton jet _{T} [GeV]", 100, 0, 100);
    hjetpartonT = new TH1F(Form("%sPartonTwoJet", name), ";parton jet _{T} [GeV]", 100, 0, 100);
    hjetparticle = new TH1F(Form("%sParticleJet", name), ";particle jet p_{T} [GeV]", 100, 0, 100);
    hjetparticleT = new TH1F(Form("%sParticleTwoJet", name), ";particle jet p_{T} [GeV]", 100, 0, 100);
    
  }
  ~MyJetTest(){
    delete hjetptvtxprof;
    delete hjetpartonratioprof;
    delete hjetparticleratioprof;
    delete hjettwojet;
    //response
    delete hjetptpartonres;
    delete hjetptparticleres;
    //profile
    delete hjetptpartonprof;
    delete hjetptparticleprof;
    //delete hjetptparticlecrrres;
    delete hjetdetector;
    delete hjetparton;
    delete hjetpartonT;
    delete hjetparticle;
    delete hjetparticleT;
    //delete hjetparticlecrr;
    //delete hjetparticlecrrT;
  }
  void Add(MyJetTest *jetyield, float w);
  //response matrix
  TH2F* GetJetPartonResMC() const{
    return hjetptpartonres;
  }	
  TH2F* GetJetParticleResMC() const{
    return hjetptparticleres;
  }
  //profile
  TProfile* GetJetPartonProfMC() const{
    return hjetptpartonprof;
  }	
  TProfile* GetJetParticleProfMC() const{
    return hjetptparticleprof;
  }
  //TH2F* GetJetParticleCrrResMC() const{
    //return hjetptparticlecrrres;
  //}
  TH1F *GetJetDetectorJet() const{
    return hjetdetector;
  }
  TH1F *GetJetPartonJet() const{
    return hjetparton;
  }
  TH1F *GetJetPartonTwoJet() const{
    return hjetpartonT;
  }
  TH1F *GetJetParticleJet() const{
    return hjetparticle;
  }
  TH1F *GetJetParticleTwoJet() const{
    return hjetparticleT;
  }
  //TH2F *GetJetParticleJetCrr() const{
    //return hjetparticlecrr;
  //}
  //TH2F *GetJetParticleTwoJetCrr() const{
    //return hjetparticlecrrT;
  //}
  TProfile *GetJetVertexProf() const{
    return hjetptvtxprof;
  }
  TProfile *GetJetPartonRatioProfile() const{
    return hjetpartonratioprof;
  }
  TProfile *GetJetParticleRatioProfile() const{
    return hjetparticleratioprof;
  }
  TH2F *GetJetTwoJet() const{
    return hjettwojet;
  }
  //void FillJetTwoJet(float pt1, float eta1, float pt2, float eta2, float weight);
  //void FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight);
  //void FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight);
  //void FillParticleCrrResponse(float pt1, float eta1, float pt2, float eta2, float weight);
 private:
  TProfile *hjetptvtxprof;
  TProfile *hjetpartonratioprof;
  TProfile *hjetparticleratioprof;
  TH2F *hjettwojet;
  //response
  TH2F *hjetptpartonres;
  TH2F *hjetptparticleres;
  //profile
  TProfile *hjetptpartonprof;
  TProfile *hjetptparticleprof;
  //TH2F *hjetptparticlecrrres;
  //th2f
  TH1F *hjetdetector;
  //th2f par
  TH1F *hjetparton;
  TH1F *hjetpartonT;
  TH1F *hjetparticle;
  TH1F *hjetparticleT;
  //TH2F *hjetparticlecrr;
  //TH2F *hjetparticlecrrT;
  ClassDef(MyJetTest, 1);
};

class MyJetTestPar : public TObject{
 public:
  MyJetTestPar(){}
  MyJetTestPar(const char* name){
    hpt = new TH1F(Form("%sJetEta", name), ";p_{T} [GeV]; #eta", 100, 0, 100);//yMinEta, yMaxEta);
    //hptetacrr = new TH2F(Form("%sJetEtaCrr", name), ";p_{T} [GeV]; #eta", yNPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
  }
  ~MyJetTestPar(){
    delete hpt;
    //delete hptetacrr;
  }
  TH1F *GetPt() const{ return hpt;} 
  //TH2F *GetPtEtaCrr() const{ return hptetacrr;} 
  void Add(MyJetTestPar *par, float w = 1.);
 private:
  TH1F *hpt;
  //TH2F *hptetacrr;
  ClassDef(MyJetTestPar, 1);
};
#endif
