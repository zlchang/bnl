#ifndef STMYJETSVDUNFOLD
#define STMYJETSVDUNFOLD

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TH2D.h"

//class TMatrixD;
//class TVectorD;

class StMyJetSVDUnfoldResponseError;
class StMyJetSVDUnfoldBias;

class StMyJetSVDUnfold : public TObject
{
 public:
 StMyJetSVDUnfold():hdetVspar(0x0),hparPy(0x0),hjetdet(0x0), hjetmc(0x0), hjetvar(0x0), 
    mMin(0), mMax(0), mDim(0), mFlag(true), mReg(0), mfReg(0),
    mResp(0x0),mBias(0x0)
    //,res(TMatrixD())
    /*,x(TVectorD()),
      b(TVectorD()),
      vjetdet(TVectorD()),
      cov(TMatrixD())*/
    {
    }
  StMyJetSVDUnfold(TH2D *hresponse, TH2D *hpar, TH2D *hdet, TH2D *hmc, TH2D *hvar,
		   int nx = 2, int ny = 15, int dim = 2, bool flag = true);

  void Unfold();
  void SetXY(int min, int max){mMin = min; mMax = max;}
  void SetOverflow(bool flag){mFlag = flag;}
  void SetReg(int index, double frac = 1.0){mReg = index; mfReg = frac;} 
  TH1D *getEff() const{return heff;}
  TH1D *getMatch() const{return hratio;}
  TH1D *getRecoNoReg() const{return hx0;}
  TH2D *getRecoNoRegErr() const{return hex0;}
  TH1D *getReco() const{return hxs;}
  TH2D *getRecoErr() const{return hex;}
  TH1D *getSV() const{return hsig;}
  double getReg() const{return reg;}
  TH2D *getResponse() const { return hdetVspar;}
  int getNx() const {return mMin;}
  int getNy() const {return mMax;}
  int getNdim() const {return mDim;}
  int getFlag() const {return mFlag;}
  void setResponseError(StMyJetSVDUnfoldResponseError *resp){ mResp = resp;}
  StMyJetSVDUnfoldResponseError *setResponseError() const { return mResp;}
  void setBias(StMyJetSVDUnfoldBias *bias){ mBias = bias;}
  StMyJetSVDUnfoldBias *setBias() const { return mBias;}
 protected:
  TH1D *heff;
  TH1D *hratio;
  TH1D *hx0;
  TH2D *hex0;
  TH1D *hxs;
  TH2D *hex;
  TH1D *hsig;
  double reg;
 private:
  TH2D *hdetVspar;
  TH2D *hparPy;
  TH2D *hjetdet;
  TH2D *hjetmc;
  TH2D *hjetvar;
  int mMin;
  int mMax;
  int mDim;
  bool mFlag;
  int mReg;
  double mfReg;
  StMyJetSVDUnfoldResponseError *mResp;
  StMyJetSVDUnfoldBias *mBias;
  //const TMatrixD &res;
  /*
  const TVectorD &x;
  const TVectorD &b;
  //const TVectorD &veff;
  //const TVectorD &vr;
  const TVectorD &vjetdet;
  const TMatrixD &cov;
  //TMatrixD &mr;
  //TMatrixD &covf;
  */
  ClassDef(StMyJetSVDUnfold, 0);    
};
#endif
