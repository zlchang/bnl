#ifndef ROOT_MYJETYIELD
#define ROOT_MYJETYIELD
#include "TObject.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile2D.h"
#include"StMyJetPool/StMyDef/MyDef.h"

class MyJetYield : public TObject{

 public:
  MyJetYield(){};
  static const int NMod = 101;
  MyJetYield(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hjetptvtxprof = new TProfile2D(Form("%sJetPtVtxMcProf",name), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetptrtprof = new TProfile2D(Form("%sJetPtRtMcProf",name), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetpartonratioprof = new TProfile2D(Form("%sJetPartonMatchProf", name), ";p_{T} [GeV];#eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetparticleratioprof = new TProfile2D(Form("%sJetParticleMatchProf", name), ";p_{T} [GeV];#eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    //response matrix
    //hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yParPtBins);
    int N2d = (yNPtBins+2)*(yNEtaBins+2);
    int N2dPar = (yNParPtBins+2)*(yNEtaBins+2);
    hjettwojet = new TH2F(Form("%sTwoJet", name), ";jet p_{T} [GeV];jet p_{T} [GeV]", N2d, 0, N2d, N2d, 0, N2d);
    hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);
    hjetptparticleres = new TH2F(Form("%sJetPtVsParticleJetPtRes", name), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);//yNPtBins, yPtBins, yNPtBins, yParPtBins);
    //pt shift
    hjetptpartonprof = new TProfile2D(Form("%sJetPtPartonProfile", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetptparticleprof = new TProfile2D(Form("%sJetPtVsParticleProfile", name), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
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
    for(int ih = 0; ih < NMod; ih++){
      //hpdfw[ih] = new TH1F(Form("hw_%s_iset_%d", name, ih), ";weight", 20, 0, 2);
      hjetptetaw[ih] = new TH2F(Form("%sDetectorJet_iset_%d", name, ih), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
      hjetptpartonresw[ih] = new TH2F(Form("%sJetPtVsPartonJetPtRes_iset_%d", name, ih), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);
      hjetptparticleresw[ih] = new TH2F(Form("%sJetPtVsParticleJetPtRes_iset_%d", name, ih), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", N2d, 0, N2d, N2dPar, 0, N2dPar);//yNPtBins, yPtBins, yNPtBins, yParPtBins);
    }
    for(int ip = 0; ip < yNproc; ip++){
      hjetdet[ip] = new TH2F(Form("%sDetectorJet_proc_%d", name, ip), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
      hjetdetmparticle[ip] = new TH2F(Form("%sDetectorJetMatchParticle_proc_%d", name, ip), ";p_{T} [GeV]; #eta", N2d, 0, N2d, N2dPar, 0, N2dPar);//yNPtBins, yPtBins, yNEtaBins, yEtaBins);
      hjetdetmparton[ip] = new TH2F(Form("%sDetectorJetMatchParton_proc_%d", name, ip), ";p_{T} [GeV]; #eta", N2d, 0, N2d, N2dPar, 0, N2dPar);//yNPtBins, yPtBins, yNEtaBins, yEtaBins);
      //
      hjetparticleproc[ip] = new TH2F(Form("%sParticleJet_proc_%d", name, ip), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
      hjetpartonproc[ip] = new TH2F(Form("%sPartonJet_proc_%d", name, ip), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    }
    hjetpartonptr = new TProfile2D(Form("%sJetPtVsPartonRatio", name), ";jet p_{T} [GeV];p_{T} ratio", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    hjetparticleptr = new TProfile2D(Form("%sJetPtVsParticleRatio", name), ";jet p_{T};p_{T} ratio", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
    double num[11];
    for(int in = 0; in < 11; in++){
      num[in] = in;
    }
    hjetpartonnm = new TH3F(Form("%sJetPartonNumMatch", name), ";jet p_{T}; #eta;N", yNPtBins, yPtBins, yNEtaBins, yEtaBins, 10, num);
    hjetparticlenm = new TH3F(Form("%sJetParticleNumMatch", name), ";jet p_{T}; #eta;N", yNPtBins, yPtBins, yNEtaBins, yEtaBins, 10, num);
  }
  ~MyJetYield(){
    delete hjetptvtxprof;
    delete hjetptrtprof;
    delete hjetpartonratioprof;
    delete hjetparticleratioprof;
    delete hjettwojet;
    //response
    delete hjetptpartonres;
    delete hjetptparticleres;
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
    for(int ih = 0; ih < NMod; ih++){
      //delete hpdfw[ih];
      delete hjetptetaw[ih];
      delete hjetptpartonresw[ih];
      delete hjetptparticleresw[ih];
    }
    for(int ip = 0; ip < yNproc; ip++){
      delete hjetdet[ip];
      delete hjetdetmparticle[ip];
      delete hjetdetmparton[ip];

      delete hjetparticleproc[ip];
      delete hjetpartonproc[ip];
    }
    delete hjetpartonptr;
    delete hjetparticleptr;
    delete hjetpartonnm;
    delete hjetparticlenm;
  }
  void Add(MyJetYield *jetyield, float w);
  //response matrix
  TH2F* GetJetPartonResMC() const{
    return hjetptpartonres;
  }	
  TH2F* GetJetParticleResMC() const{
    return hjetptparticleres;
  }
  //profile
  TProfile2D* GetJetPartonProfMC() const{
    return hjetptpartonprof;
  }	
  TProfile2D* GetJetParticleProfMC() const{
    return hjetptparticleprof;
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
  TProfile2D *GetJetRtProf() const{
    return hjetptrtprof;
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
  //TH1F *GetW(int iset = 0) const{ return hpdfw[iset];} 
  TH2F *GetJetDetectorW(int iset = 0) const{ return hjetptetaw[iset];} 
  TH2F* GetJetPartonResMCW(int iset = 0) const{
    return hjetptpartonresw[iset];
  }	
  TH2F* GetJetParticleResMCW(int iset = 0) const{
    return hjetptparticleresw[iset];
  }
  TH2F* GetJetDetectorProc(int ip) const { return hjetdet[ip];}
  TH2F* GetJetDetMatchParticleProc(int ip) const { return hjetdetmparticle[ip];}
  TH2F* GetJetDetMatchPartonProc(int ip) const { return hjetdetmparton[ip];}

  TH2F* GetJetParticleProc(int ip) const { return hjetparticleproc[ip];}
  TH2F* GetJetPartonProc(int ip) const { return hjetpartonproc[ip];}

  TProfile2D *GetJetPartonScale() const {return hjetpartonptr;} 
  TProfile2D *GetJetParticleScale() const {return hjetparticleptr;} 
  TH3F *GetJetPartonNMatch() const {return hjetpartonnm;}
  TH3F *GetJetParticleNMatch() const {return hjetparticlenm;}

  void FillJetTwoJet(float pt1, float eta1, float pt2, float eta2, float weight);
  void FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight);
  void FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight);

  void FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf);
  void FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf);
  void FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf, int proc);
  void FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf, int proc);
  //void FillParticleCrrResponse(float pt1, float eta1, float pt2, float eta2, float weight);
 private:
  TProfile2D *hjetptvtxprof;
  TProfile2D *hjetptrtprof;
  TProfile2D *hjetpartonratioprof;
  TProfile2D *hjetparticleratioprof;
  TH2F *hjettwojet;
  //response
  TH2F *hjetptpartonres;
  TH2F *hjetptparticleres;
  //profile
  TProfile2D *hjetptpartonprof;
  TProfile2D *hjetptparticleprof;
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
  //TH1F *hpdfw[NMod];
  TH2F *hjetptetaw[NMod];
  TH2F *hjetptpartonresw[NMod];
  TH2F *hjetptparticleresw[NMod];
  //subprocess
  TH2F *hjetdet[yNproc];
  TH2F *hjetdetmparticle[yNproc];
  TH2F *hjetdetmparton[yNproc];
  
  TH2F *hjetparticleproc[yNproc];
  TH2F *hjetpartonproc[yNproc];
  //
  TProfile2D *hjetpartonptr;
  TProfile2D *hjetparticleptr;
  TH3F *hjetpartonnm;
  TH3F *hjetparticlenm;

  ClassDef(MyJetYield, 2);
};

class MyJetYieldPar : public TObject{
 public:
  static const int NMod = 101;
  MyJetYieldPar(){}
  MyJetYieldPar(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hpteta = new TH2F(Form("%sJetEta", name), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    //hptetacrr = new TH2F(Form("%sJetEtaCrr", name), ";p_{T} [GeV]; #eta", yNPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    for(int ih = 0; ih < NMod; ih++){
       //hpdfw[ih] = new TH1F(Form("hw_%s_iset_%d", name, ih), ";weight", 20, 0, 2);
       hptetaw[ih] = new TH2F(Form("%sJetEta_iset_%d", name, ih), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    }
    for(int ip = 0; ip < yNproc; ip++){
      hptetaproc[ip] = new TH2F(Form("%sJetEta_proc_%d", name, ip), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    }
    //hpthat = new TH1F(Form("hpthat_%s", name), ";partonic p_{T} [GeV];", 1000, 0, 100);
    //for(int ip = 0; ip < NMod; ip++){
      //hpthatw[ip] = new TH1F(Form("hpthat_%s_%d", name, ip), ";partonic p_{T} [GeV];", 1000, 0, 100);
    //} 
    int N2dPar = (yNParPtBins+2)*(yNEtaBins+2);
    hjetptres = new TH2F(Form("%sJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", N2dPar, 0, N2dPar, N2dPar, 0, N2dPar);
  }
  ~MyJetYieldPar(){
    delete hpteta;
    //delete hptetacrr;
    for(int ih = 0; ih < NMod; ih++){
      //delete hpdfw[ih];
      delete hptetaw[ih];
    }
    for(int ip = 0; ip < yNproc; ip++){
      delete hptetaproc[ip];
    }
  }
  TH2F *GetPtEta() const{ return hpteta;} 
  //TH1F *GetW(int iset = 0) const{ return hpdfw[iset];} 
  TH2F *GetPtEtaW(int iset = 0) const{ return hptetaw[iset];} 
  TH2F *GetPtEtaProc(int ip = 0) const{ return hptetaproc[ip];} 
  //TH2F *GetPtEtaCrr() const{ return hptetacrr;} 
  //TH1F *GetPtHat() const {return hpthat; }
  //TH1F *GetPtHatW(int iset = 0) const {return hpthatw[iset]; }
  TH2F *GetPtRes() const{ return hjetptres;} 
  void FillResponse(float pt1, float eta1, float pt2, float eta2, float weight);
  void Add(MyJetYieldPar *par, float w = 1.);
 private:
  TH2F *hpteta;
  //TH1F *hpthat;
  //TH1F *hpdfw[NMod];
  //TH1F *hpthatw[NMod];
  TH2F *hptetaw[NMod];
  TH2F *hptetaproc[yNproc];
  //TH2F *hptetacrr;
  TH2F *hjetptres; 
  ClassDef(MyJetYieldPar, 2);
};
#endif
