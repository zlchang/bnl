#include "StMyJetSVDUnfoldBias.h"
//#include"StMyJetSVDUnfold.h"
#include "TRandom.h"

#include "StMyJetPool/StMyJetUtilities/StMatrixUtilities.h"
#include "StMyJetPool/StMyJetUtilities/StHistUtilities.h"
#include "StMyJetPool/StMyJetUtilities/StJetUtilities.h"

void StMyJetSVDUnfoldBias::getError(const TVectorD &x, const TMatrixD &rescovp, const TMatrixD &cc, double reg)
{
  int nx = x.GetNrows();
  hprof = new TProfile("profBiasSyst", "; particle jet p_{T} bin index;diff #pm rms", nx, 0, nx, "s");
  const int Nr = Nrep;
  TH1D *hxtest[Nr];
  hx = hxtest;
  for(int irep = 0; irep < Nrep; irep++){
    TVectorD vsmr(nx);
    gRandom->RndmArray(nx, vsmr.GetMatrixArray());
    vsmr *= 0.2; // random number from 0.9 to 1.1
    vsmr += 0.7; // random number from 0.9 to 1.1
    //vsmr = 2*vsmr; // random number from 0 to 2
    vsmr.Print();
    
    TVectorD xTest(x);
    ElementMult(xTest, vsmr);
    hxtest[irep] = vectorToHist(xTest, Form("xTest%d", irep));
    //cBias->cd(1); hxtest->Draw("same");
    //gPad->SetLogy(1);
    //hxtest->SetLineColor(kBlue-10);
    ElementDiv(xTest, x);
    xTest.Print();

    TVectorD bcovTest = rescovp*xTest;
    TMatrixD ex;
    TVectorD xs = svdReg(rescovp, x, bcovTest, cc, reg, ex);
    xs.Print();
    //TVectorD xrec(xs);
    for(int ir = 0; ir < xs.GetNrows(); ir++){
      double bias = (xs(ir)-xTest(ir)*x(ir))/x(ir);
      //double bias = xparw(ir)-xparw0(ir);
      Printf("%d %e", ir, bias);
      //double bias = (xs(ir)-xTest(ir)*x(ir))/x(ir);
      //double bias = TMath::Power(xs(ir)-xTest(ir), 2);
      hprof->Fill(ir, bias);
    }
  }
}
