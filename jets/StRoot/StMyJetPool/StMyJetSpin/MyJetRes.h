#ifndef ROOT_MYJETRESPONSE
#define ROOT_MYJETRESPONSE
#include "TObject.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TAxis.h"
//#include "TProfile2D.h"
#include"StMyJetPool/StMyDef/MyDef.h"
#include "StMyHistUtil/StMyHist.h"
class MyJetRes : public TObject{
 public:
  virtual TH2D* GetJetRes() const = 0;
  //virtual TH1D *GetParJet() const = 0;
  //virtual TH1D *GetParTwoJet() const = 0;

  virtual StMyProfile *GetJetMatchRatio() const = 0;
  virtual StMyProfile *GetJetPtScale() const = 0;
  virtual StMyProfile *GetJetPtShift() const = 0;
 private:
  ClassDef(MyJetRes, 0);
};
class MyJetRes2D : public MyJetRes{

 public:
  MyJetRes2D(){};
  
  MyJetRes2D(const char* name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    //response matrix
    int nx = (yNPtBins+2)*(yNEtaBins+2);
    int ny = (yNParPtBins+2)*(yNEtaBins+2);
    hjetres = new TH2D(Form("%sResponse", name), "; index x; index y", nx, 0, nx, ny, 0, ny);
    //matching ratio
    hjetmatchratioprof = new StMyProfile(Form("%sMatchRatioProf", name), "; index x; matchRatio", nx, 0, nx);
    //pt shift
    hjetptshiftprof = new StMyProfile(Form("%sJetPtShiftProf", name), ";index x;jet p_{T} shift [GeV]", nx, 0, nx);
    hjetptratioprof = new StMyProfile(Form("%sJetPtRatioProf", name), ";index x;p_{T} ratio", nx, 0, nx);
    //particle spectrum
    //hjetpar = new TH1D(Form("%sParJet", name), ";par jet p_{T} [GeV]", ny, 0, ny);
    //hjetparT = new TH1D(Form("%sParMatchedTwoJets", name), ";parton jet _{T} [GeV]", ny, 0, ny);
  }
  ~MyJetRes2D(){
    delete hjetmatchratioprof;
    //response
    delete hjetres;
    delete hjetptshiftprof;
    delete hjetptratioprof;
    //delete hjetpar;
    //delete hjetparT;
  }
  void Add(MyJetRes2D *jetyield, float w);
  //response matrix
  TH2D* GetJetRes() const{
    return hjetres;
  }	
  //TH1D *GetParJet() const{
  //return hjetpar;
  //}
  //TH1D *GetParTwoJet() const{
  //return hjetparT;
  //}
  StMyProfile *GetJetMatchRatio() const {return hjetmatchratioprof;} 
  StMyProfile *GetJetPtScale() const {return hjetptratioprof;} 
  StMyProfile *GetJetPtShift() const {return hjetptshiftprof;} 

  void FillJetMatchRatio(double pt, double eta, bool match, double w) {
    int ii = indexDet(pt, eta);
    hjetmatchratioprof->Fill(ii, match, w);
  }   
  //void *FillParJet(double pt, double eta, double w){
  //hjetpar->Fill(YIndex(pt, eta), w);
  //}
  //void *FillParTwoJet(double pt, double eta, double w){
  //hjetparT->Fill(YIndex(pt, eta), w);
  //}
  void FillResponse(float pt1, float eta1, float pt2, float eta2, float weight);

 private:
  StMyProfile *hjetmatchratioprof;
  //response
  TH2D *hjetres;
  //profile
  StMyProfile *hjetptshiftprof;
  StMyProfile *hjetptratioprof;
  //TH1D *hjetpar;
  //TH1D *hjetparT;
  ClassDef(MyJetRes2D, 1);
};
class MyJetRes1D : public MyJetRes{

 public:
  MyJetRes1D(){};
  
  MyJetRes1D(const char* name){

    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);

    //response matrix
    hjetres = new TH2D(Form("%sResponse", name), "; index x; index y", yNPtBins, yPtBins, yNParPtBins, yParPtBins);
    //matching ratio
    hjetmatchratioprof = new StMyProfile(Form("%sMatchRatioProf", name), "; index x; matchRatio", yNPtBins, yPtBins);
    //pt shift
    hjetptshiftprof = new StMyProfile(Form("%sJetPtShiftProf", name), ";index x;jet p_{T} shift [GeV]", yNPtBins, yPtBins);
    hjetptratioprof = new StMyProfile(Form("%sJetPtRatioProf", name), ";index x;p_{T} ratio", yNPtBins, yPtBins);
    //particle spectrum
    //hjetpar = new TH1D(Form("%sParJet", name), ";par jet p_{T} [GeV]", nypt, yptbins);
    //hjetparT = new TH1D(Form("%sParMatchedTwoJets", name), ";parton jet _{T} [GeV]", nypt, yptbins);
  }
  ~MyJetRes1D(){
    delete hjetmatchratioprof;
    //response
    delete hjetres;
    delete hjetptshiftprof;
    delete hjetptratioprof;
    //delete hjetpar;
    //delete hjetparT;
  }
  void Add(MyJetRes1D *jetyield, float w);
  //response matrix
  TH2D* GetJetRes() const{
    return hjetres;
  }	
  //TH1D *GetParJet() const{
  //return hjetpar;
  //}
  //TH1D *GetParTwoJet() const{
  //  return hjetparT;
  //}

  StMyProfile *GetJetMatchRatio() const {return hjetmatchratioprof;} 
  StMyProfile *GetJetPtScale() const {return hjetptratioprof;} 
  StMyProfile *GetJetPtShift() const {return hjetptshiftprof;} 
  
  void FillResponse(float pt1, float pt2, float weight);

 protected:
 private:
  //response
  TH2D *hjetres;
  //profile
  StMyProfile *hjetmatchratioprof;
  StMyProfile *hjetptshiftprof;
  StMyProfile *hjetptratioprof;
  //TH1D *hjetpar;
  //TH1D *hjetparT;
  
  ClassDef(MyJetRes1D, 2);
};
#endif
