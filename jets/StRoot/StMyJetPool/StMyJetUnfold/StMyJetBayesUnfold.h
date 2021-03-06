#ifndef STMYJETBAYESUNFOLD
#define STMYJETBAYESUNFOLD

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TArrayD.h"

class StMyJetBayesUnfold : public TObject{
 public:
  StMyJetBayesUnfold(const TMatrixD &A, const TVectorD &b, const TVectorD &x, int nit){
    mB.ResizeTo(b.GetNrows()); mB = b; 
    mA.ResizeTo(A.GetNrows(), A.GetNcols()); mA = A; 
    mX.ResizeTo(x.GetNrows());mX = x; 
    mNit = nit;
    mRes.ResizeTo(x.GetNrows());
    mChi2.Set(nit);
  }
  void unfold();
  Double_t bayesUnfold();
  TVectorD getX() const {return mX; }
  TVectorD getB() const { return mB; }
  TMatrixD getA() const { return mA; }
  TVectorD getRes() const { return mRes; }
  TArrayD getChi2() const { return mChi2; }
 private:
  int mNit;
  TVectorD mX;
  TVectorD mB;
  TMatrixD mA;
  TVectorD mRes;
  TArrayD mChi2;
  ClassDef(StMyJetBayesUnfold, 0);
};

#endif
