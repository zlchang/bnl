#include<iostream>

#include "StMyJetSVDUnfold.h"
#include "StMyJetSVDUnfoldResponseError.h"
#include "StMyJetSVDUnfoldBias.h"
#include "TMath.h"
#include "TDecompSVD.h"

//#include "TMatrixD.h"
//#include "TVectorD.h"
#include "StMyJetPool/StMyJetUtilities/StMatrixUtilities.h"
#include "StMyJetPool/StMyJetUtilities/StHistUtilities.h"
#include "StMyJetPool/StMyJetUtilities/StJetUtilities.h"

using namespace std;
StMyJetSVDUnfold::StMyJetSVDUnfold(TH2D *hresponse, TH2D *hpar, TH2D *hdet, TH2D *hmc, TH2D *hvar, int nx, int ny, int dim, bool flag) :
  hdetVspar(hresponse),hparPy(hpar),hjetdet(hdet), hjetmc(hmc), hjetvar(hvar), 
  mMin(nx), mMax(ny), mDim(2), mFlag(flag), mReg(0), mfReg(0)
  ,mResp(0x0),mBias(0x0)
  //,res(histToMatrixTrunc2dn(hresponse, nx, ny, nx, ny, dim, flag))
  /*,x(hist2dToVector(hpar, nx, ny, flag)),
  b(hist2dToVector(hmc, nx, ny, flag)),
  //veff(projCols(res)),
  //vr(projRows(res)),
  vjetdet(hist2dToVector(hdet, nx, ny, flag)),
  cov(histToMatrixTrunc2d(hvar, nx, ny, dim, flag))
  //mr(),
  //covf()
  */
{
  //res.Print();
  //Printf("nc = %d", res.GetNrows());
  //Unfold();
  /*  hdetVspar = hresponse;
  hparPy = hpar;
  hjetdet = hdet;
  hjetmc = hmc;
  htwojet = hvar;
  mMin = nx; mMax = ny;
  mFlag = true;
  mReg = 1;
  mfReg = 0;
  */
}

void StMyJetSVDUnfold::Unfold()
{
  TMatrixD res = histToMatrixTrunc2dn(hdetVspar, mMin, mMax, mMin, mMax, mDim, mFlag);
  TVectorD x = hist2dToVector(hparPy, mMin, mMax, mFlag);
  //x.Print();
  TVectorD b = hist2dToVector(hjetmc, mMin, mMax, mFlag);
  //b.Print();
  TVectorD vjetdet = hist2dToVector(hjetdet, mMin, mMax, mFlag);
  TMatrixD cov = histToMatrixTrunc2d(hjetvar, mMin, mMax, mDim, mFlag);
  /*
  TVectorD xproj = projCols(res);
  TVectorD veff(xproj);
  ElementDiv(veff, x);
  heff = vectorToHist(veff, "Efficiency");
  TVectorD vdet = projRows(res);
  
  TVectorD vr(vdet);
  ElementDiv(vr, vjetdet);
  hratio = vectorToHist(vr, "matchRatio");
  //matching ratio to b
  ElementMult(b, vr);
  //TH2D *hvariance = getVariance(hjetmc, htwojet);
  //hvariance->Print("all");
  
  cov.Print();
  TMatrixD mr = DiagMatrix(vr);
  cov = mr*cov*mr;
  TMatrixD covf = covFactor(cov);
  */
  //Printf("nc = %d", res.GetNrows());
  //exit(0);
  TVectorD xproj = projCols(res);
  xproj.Print();
  TVectorD veff(xproj);
  ElementDiv(veff, x);
  veff.Print();
  heff = vectorToHist(veff, "Efficiency");
  TVectorD vdet = projRows(res);
  //TVectorD vjetdet = hist2dToVector(hjetdet, mMin, mMax, mFlag);
  TVectorD vr(vdet);
  ElementDiv(vr, vjetdet);
  hratio = vectorToHist(vr, "matchRatio");
  //apply matching ratio to b
  TVectorD bn(b);
  ElementMult(bn, vr);
  TMatrixD mr = DiagMatrix(vr);
  //cov = mr*cov*mr;
  TMatrixD covf = covFactor(mr*cov*mr);
  
  TMatrixD rescov = covf*res;
  TVectorD bcov = covf*bn;
  
  TMatrixD rescovp(rescov);
  
  TMatrixD ex0;
  TVectorD x0 = SVD(rescovp, bcov, ex0);
  ElementMult(x0, x);
  TMatrixD xm = DiagMatrix(x);
  ex0 = xm*ex0*xm;
  hx0 = vectorToHist(x0, ex0, "recoNo");
  hex0 = matrixToHist(ex0, "recoNoError");

  int nxx = rescovp.GetNcols();
  TMatrixD cc = regMatrix(nxx/2, 2);
  TDecompSVD rsvd(rescovp);
  TVectorD sr = rsvd.GetSig();
  
  reg = sr(mReg)*mfReg;
  hsig = vectorToHist(sr, "sig");
  
  TMatrixD ex;
  TVectorD xs = svdReg(rescovp, x, bcov, cc, reg, ex);
  
  hxs = vectorToHist(xs, ex, "reco");
  hex = matrixToHist(ex, "recoError");

  if(mResp) mResp->getError(res, x, covf, bcov, cc);
  if(mBias) mBias->getError(x, rescovp, cc, reg);

}
