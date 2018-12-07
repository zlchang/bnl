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

    //detector spectrum
    int nbins = 150;
    float ptmax = 150;
    hjetpt = new TH1F(Form("%sJetPt", name), ";jet p_{T} [GeV]", nbins, 0, ptmax);    
    hjeteta = new TH1F(Form("%sJetEta", name), ";jet #eta", 60, -1.5, 1.5);    
    hjetphi = new TH1F(Form("%sJetPhi", name), ";jet #phi", 126, -3.15, 3.15);
    hjettwo = new TH2F(Form("%sTwoJet", name), "; jet p_{1,T} [GeV]; jet p_{2,T} [GeV]", nbins, 0, ptmax, nbins, 0, ptmax);
  }
  ~MyJetTest(){
    delete hjetpt;
    delete hjeteta;
    delete hjetphi;
    delete hjettwo;
  }
  void Add(MyJetTest *jetyield, float w);
  TH1F *GetJetPt() const{ return hjetpt; }
  TH1F *GetJetEta() const{ return hjeteta; }
  TH1F *GetJetPhi() const{ return hjetphi; }
  TH2F *GetTwoJet() const{ return hjettwo; }
  
 private:
  TH1F *hjetpt;
  TH1F *hjeteta;
  TH1F *hjetphi;
  TH2F *hjettwo;
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
