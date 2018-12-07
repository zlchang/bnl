#ifndef ROOT_STMYRESPONSE
#define ROOT_STMYRESPONSE
#include "TObject.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TAxis.h"
//#include "TProfile2D.h"
#include"StMyJetPool/StMyDef/MyDef.h"
#include "StMyHistUtil/StMyHist.h"
class StMyRes : public TObject{
 public:
  virtual TH2D* GetJetRes() const = 0;
  //virtual TH1D *GetParJet() const = 0;
  //virtual TH1D *GetParTwoJet() const = 0;
  
  virtual StMyProfile *GetJetMatchRatio() const = 0;
  //virtual StMyProfile *GetJetPtScale() const = 0;
  virtual StMyProfile *GetJetPtMean() const = 0;
 private:
  ClassDef(StMyRes, 0);
};
class StMyRes2D : public StMyRes{

 public:
  StMyRes2D(){};
  
  StMyRes2D(const char* name, int nxpt, const double *xptbins, int nxeta, const double *xetabins, int nypt, const double *yptbins, int nyeta, const double *yetabins){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);

    xpt = new TAxis(nxpt, xptbins);
    xeta = new TAxis(nxeta, xetabins);
    ypt = new TAxis(nypt, yptbins);
    yeta = new TAxis(nyeta, yetabins);
    //response matrix
    int nx = (nxpt+2)*(nxeta+2);
    int ny = (nypt+2)*(nyeta+2);
    hjetres = new TH2D(Form("%sResponse", name), "; index x; index y", nx, 0, nx, ny, 0, ny);
    //matching ratio
    hjetmatchratioprof = new StMyProfile(Form("%sMatchRatioProf", name), "; index x; matchRatio", nx, 0, nx);
    //pt mean
    hjetptmeanprof = new StMyProfile(Form("%sJetPtMeanProf", name), ";index x;jet p_{T} mean [GeV]", nx, 0, nx);
    //hjetptratioprof = new StMyProfile(Form("%sJetPtRatioProf", name), ";index x;p_{T} ratio", nx, 0, nx);
    //particle spectrum
    //hjetpar = new TH1D(Form("%sParJet", name), ";par jet p_{T} [GeV]", ny, 0, ny);
    //hjetparT = new TH1D(Form("%sParMatchedTwoJets", name), ";parton jet _{T} [GeV]", ny, 0, ny);
  }
  ~StMyRes2D(){
    delete hjetmatchratioprof;
    //response
    delete hjetres;
    delete hjetptmeanprof;
    //delete hjetptratioprof;
    //delete hjetpar;
    //delete hjetparT;

    delete xpt;
    delete xeta;
    delete ypt;
    delete yeta;
  }
  void Add(StMyRes2D *jetyield, float w);
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
  //StMyProfile *GetJetPtScale() const {return hjetptratioprof;} 
  StMyProfile *GetJetPtMean() const {return hjetptmeanprof;} 

  void FillJetMatchRatio(double pt, double eta, bool match, double w) {
    int ii = XIndex(pt, eta);
    hjetmatchratioprof->Fill(ii, match, w);
  }   
  //void *FillParJet(double pt, double eta, double w){
  //hjetpar->Fill(YIndex(pt, eta), w);
  //}
  //void *FillParTwoJet(double pt, double eta, double w){
  //hjetparT->Fill(YIndex(pt, eta), w);
  //}
  void FillResponse(float pt1, float eta1, float pt2, float eta2, float weight);

 protected:
  int XIndex(double pt, double eta);
  int YIndex(double pt, double eta);
 private:
  StMyProfile *hjetmatchratioprof;
  //response
  TH2D *hjetres;
  //profile
  StMyProfile *hjetptmeanprof;
  //StMyProfile *hjetptratioprof;
  //TH1D *hjetpar;
  //TH1D *hjetparT;

  TAxis *xpt;
  TAxis *xeta;
  TAxis *ypt;
  TAxis *yeta;
  
  ClassDef(StMyRes2D, 1);
};
class StMyRes1D : public StMyRes{

 public:
  StMyRes1D(){};
  
  StMyRes1D(const char* name, int nxpt, const double *xptbins, int nypt, const double *yptbins){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);

    //response matrix
    hjetres = new TH2D(Form("%sResponse", name), "; index x; index y", nxpt, xptbins, nypt, yptbins);
    //matching ratio
    hjetmatchratioprof = new StMyProfile(Form("%sMatchRatioProf", name), "; index x; matchRatio", nxpt, xptbins);
    //pt mean
    hjetptmeanprof = new StMyProfile(Form("%sJetPtMeanProf", name), ";index x;jet p_{T} mean [GeV]", nxpt, xptbins);
    //hjetptratioprof = new StMyProfile(Form("%sJetPtRatioProf", name), ";index x;p_{T} ratio", nxpt, xptbins);
    //particle spectrum
    //hjetpar = new TH1D(Form("%sParJet", name), ";par jet p_{T} [GeV]", nypt, yptbins);
    //hjetparT = new TH1D(Form("%sParMatchedTwoJets", name), ";parton jet _{T} [GeV]", nypt, yptbins);
  }
  ~StMyRes1D(){
    delete hjetmatchratioprof;
    //response
    delete hjetres;
    delete hjetptmeanprof;
    //delete hjetptratioprof;
    //delete hjetpar;
    //delete hjetparT;
  }
  void Add(StMyRes1D *jetyield, float w);
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
  //StMyProfile *GetJetPtScale() const {return hjetptratioprof;} 
  StMyProfile *GetJetPtMean() const {return hjetptmeanprof;} 
  
  void FillResponse(float pt1, float pt2, float weight);

 protected:
 private:
  //response
  TH2D *hjetres;
  //profile
  StMyProfile *hjetmatchratioprof;
  StMyProfile *hjetptmeanprof;
  //StMyProfile *hjetptratioprof;
  //TH1D *hjetpar;
  //TH1D *hjetparT;
  
  ClassDef(StMyRes1D, 2);
};
#endif
