#include "StMyJetBayesUnfold.h"

void StMyJetBayesUnfold::unfold()
{
  for(int it = 0; it < mNit; it++){
    mChi2.SetAt(bayesUnfold(), it);
    mX = mRes;
  }
}
Double_t StMyJetBayesUnfold::bayesUnfold()
{
  TVectorD xp(mX);
  double norm = xp.Sum();
  xp *= 1./norm;
  //x.Print();
  //xp.Print();
  //cout<<"prob: "<<endl;
  //prob.Print();
  TMatrixD pand(mA);
  pand.NormByRow(xp, "M");
  //cout<<"pand: "<<endl;
  //pand.Print();
  int nrow = pand.GetNrows();
  TVectorD pe(nrow);
  for(int ir = 0; ir < nrow; ir++){
    pe(ir) = TVectorD(TMatrixDRow(pand, ir)).Sum();
  }
  //cout<<"pe: "<<endl;
  //pe.Print();
  
  pand.NormByColumn(pe);
  //pand.Print();
  TMatrixD pandT(pand.GetNcols(), pand.GetNrows());
  pandT.Transpose(pand);
  mRes = pandT*mB;
  TVectorD xd = mRes-mX;
  return xd.Norm2Sqr();
  //rec.Print();
  //  return rec;
}

