#ifndef ROOT_MYJETYIELD
#define ROOT_MYJETYIELD
#include "TObject.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile2D.h"
#include"StMyJetPool/StMyDef/MyDef.h"
#include "StMyHistUtil/StMyHist.h"

class MyJetYield : public TObject{

 public:
  MyJetYield(){};

  MyJetYield(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hjetptrtprof = new StMyProfile2D(Form("%sJetPtRtMcProf",name), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);

    //response matrix
    //hjetptpartonres = new TH2F(Form("%sJetPtVsPartonJetPtRes", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yParPtBins);
    int N2d = (yNPtBins+2)*(yNEtaBins+2);
    hjettwojet = new TH2F(Form("%sTwoJet", name), ";jet p_{T} [GeV];jet p_{T} [GeV]", N2d, 0, N2d, N2d, 0, N2d);
    //detector spectrum
    hjetdetector = new TH2F(Form("%sDetectorJet", name), ";detector jet _{T} [GeV]", yNPtBins, yPtBins, yNEtaBins, yEtaBins);
   } 
    ~MyJetYield(){
      delete hjetptrtprof;
      //delete hjetptrtprofw2;
      delete hjettwojet;
      delete hjetdetector;
  }
  void Add(MyJetYield *jetyield, float w);
  
  TH2F *GetJet() const{
    return hjetdetector;
  }
  StMyProfile2D *GetJetRtProf() const{
    return hjetptrtprof;
  }
  TH2F *GetJetTwoJet() const{
    return hjettwojet;
  }

  void FillJetTwoJet(float pt1, float eta1, float pt2, float eta2, float weight);
  void FillJetPtEtaRt(float pt, float eta, float rt, float weight);
  
 private:
  StMyProfile2D *hjetptrtprof;
  TH2F *hjettwojet;
  //th2f
  TH2F *hjetdetector;
  ClassDef(MyJetYield, 1);
};

class MyJetYieldPar : public TObject{
 public:
  static const int NMod = 101;
  MyJetYieldPar(){}
  MyJetYieldPar(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hpteta = new TH2F(Form("%sJetPtEta", name), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    //hptetacrr = new TH2F(Form("%sJetEtaCrr", name), ";p_{T} [GeV]; #eta", yNPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    //for(int ih = 0; ih < NMod; ih++){
       //hpdfw[ih] = new TH1F(Form("hw_%s_iset_%d", name, ih), ";weight", 20, 0, 2);
       //hptetaw[ih] = new TH2F(Form("%sJetEta_iset_%d", name, ih), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);//yMinEta, yMaxEta);
    //}
    //for(int ip = 0; ip < yNproc; ip++){
    //hptetaproc[ip] = new TH2F(Form("%sJetEta_proc_%d", name, ip), ";p_{T} [GeV]; #eta", yNParPtBins, yParPtBins, yNEtaBins, yEtaBins);
    //}
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
    //for(int ih = 0; ih < NMod; ih++){
      //delete hpdfw[ih];
      //delete hptetaw[ih];
    //}
    //for(int ip = 0; ip < yNproc; ip++){
    //delete hptetaproc[ip];
    //}
  }
  TH2F *GetPtEta() const{ return hpteta;} 
  //TH1F *GetW(int iset = 0) const{ return hpdfw[iset];} 
  //TH2F *GetPtEtaW(int iset = 0) const{ return hptetaw[iset];} 
  //TH2F *GetPtEtaProc(int ip = 0) const{ return hptetaproc[ip];} 
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
  //TH2F *hptetaw[NMod];
  //TH2F *hptetaproc[yNproc];
  //TH2F *hptetacrr;
  TH2F *hjetptres; 
  ClassDef(MyJetYieldPar, 1);
};
#endif
