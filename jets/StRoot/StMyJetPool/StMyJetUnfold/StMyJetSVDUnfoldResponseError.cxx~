#include"StMyJetSVDUnfoldResponseError.h"
#include"StMyJetSVDUnfold.h"

#include "TRandom.h"

#include "StMyJetPool/StMyJetUtilities/StMatrixUtilities.h"
#include "StMyJetPool/StMyJetUtilities/StHistUtilities.h"
#include "StMyJetPool/StMyJetUtilities/StJetUtilities.h"

void StMyJetSVDUnfoldResponseError::getError(const TMatrixD &res, const TVectorD &xx, const TMatrixD &covf, const TVectorD &bcov, const TMatrixD &cc)
{
  TH2D *hdetVspar = mUnfold->getResponse();
  int nx = mUnfold->getNx();
  int ny = mUnfold->getNy();
  int nd = mUnfold->getNdim();
  bool flag = mUnfold->getFlag();
  double reg = mUnfold->getReg();
  //TH2D *hjetdet = mUn
  TMatrixD resErr = histToMatrixTruncError2dn(hdetVspar, nx, ny, nx, ny, nd, flag);
  resErr.Sqrt();
  resErr.Print();
  //
  hprof = new TProfile("profRespSyst", ";particle jet p_{T} bin; diff #pm rms", res.GetNcols(), 0, res.GetNcols(), "s");
  for(int irep = 0; irep < Nrep; irep++){
    TMatrixD mrep = replica(res, resErr, irep);
    if(irep%10 == 0) mrep.Print();
    TMatrixD mcovrep = covf*mrep;
    TMatrixD mprep(mcovrep);
    //normalize
    //TVectorD x0rep = projCols(mrep);
    //mprep.NormByRow(x0rep);
    //mprep.NormByRow(xx);
    TMatrixD exrep;
    //TVectorD xrep = SVD(mprep, bcov, exrep);
    TVectorD xrep = svdReg(mprep, xx, bcov, cc, reg, exrep);
    xrep.Print();
    TH1D *hxrep = vectorToHist(xrep, Form("testresrep%d", irep));
    //cResp->cd();hxrep->Draw("same");
    hxrep->SetLineColor(kBlue-10);
    for(int ir = 0; ir < xrep.GetNrows(); ir++){
      hprof->Fill(ir, xrep(ir));
    }
  }
  //cResp->cd(); hprof->Draw("same");
}
TMatrixD StMyJetSVDUnfoldResponseError::replica(const TMatrixD &res, const TMatrixD &resErr, int seed)
{
  int nr = res.GetNrows();
  int nc = res.GetNcols();
  const int Nrndm = nr*nc;
  double rndm[Nrndm];
  gRandom->SetSeed(seed);
  gRandom->RndmArray(Nrndm, rndm);
  TMatrixD mrndm(nr, nc, rndm);
  TMatrixD resRep(nr, nc);
  resRep.Zero();
  for(int ir = 0; ir < nr; ir++){
    for(int ic = 0; ic < nc; ic++){
      if(mrndm(ir, ic) < 0.5) resRep(ir, ic) = res(ir,ic) + resErr(ir, ic);
      else resRep(ir, ic) = res(ir, ic) - resErr(ir, ic);
    }
  }
  //resRep.Print("all");
  return resRep;
}
