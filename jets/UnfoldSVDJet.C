TFile *mFile;
TFile *mFileTest;
TFile *mFilePy;
TFile *mFileOut;
const int Ntrg = 4;

const char *trg[] = {"JP0", "JP1", "JP2", "Cmb"};

const char *par = "Particle";
TH2D *hdetVspar;
TH2D *hdetVsparTest;
//TH1D *hdet;
//TH1D *hproj;
TH1D *hpar;
//TH1D *hparTest;
//
TH2D *hparPy;

TH1D *hjet;
//TH2D *htwojet;

int i = 3;
int Ntrun[] = {2, 4, 8, 2};
int Mtrun[] = {11, 13, 15, 15};
int Nb = Mtrun[i] - Ntrun[i] + 1;
TArrayD *xbins;
TArrayD *ybins;

int dnshift = 0;
int upshift = 0;

int step = 1;

int Nrep = 100;
bool mFlag = true;

//particle 21; parton 18
int mReg = 15;
//cmb
//double fReg = 0.01;
double fReg = 1;

TCanvas *cRes;
TCanvas *cBias;

int UnfoldSVDJet(//const char *file = "ptbin.list.run12.l.v2.w.cmb.crs.root",
	      //const char* testfile = "run12.final.v2.list.v0.1.crs.root",
	      //const char* pyfile = "ptbin.list.run12.l.v1.w.cmb.pmc.root",
              const char *file = "ptbin.list.run12.l.ue.v2.w.cmb.crs.root",
	      const char* testfile = "run12.final.v2.list.ue.v1.0.crs.root",
	      const char* pyfile = "ptbin.list.run12.l.ue.v2.w.cmb.pmc.root",
	      const char* ver = "JetUeV1"
	      )
{
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  TH1::SetDefaultSumw2(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetHistLineWidth(2);

  gSystem->Load("StMyJetUnfold");
  gSystem->Load("StMyJetUtilities");
  //gSystem->Load("/star/u/zchang/software/local/lib/libRooUnfold.so");
  
  mFile = TFile::Open(file);
  mFileTest = TFile::Open(testfile);
  mFilePy = TFile::Open(pyfile);
  
  mFileOut = new TFile(Form("%s%s%sUnfoldSVD.root", trg[i], par, ver), "recreate");

  hdetVspar = (TH2D*) mFile->Get(Form("%sJetPtVs%sJetPtRes", trg[i], par));
  if(!hdetVspar) { Info("Read", Form("histogram %sJetPtVs%setPt not found", trg[i], par)); return 0;}
  //error on the response matrix
  //TMatrixD resErr = histToMatrixTruncError2dn(hdetVspar, Ntrun[i], Mtrun[i], Ntrun[i]+dnshift, Mtrun[i]-upshift, 2, mFlag);  
  //resErr.Sqrt();
  //resErr.Print();
  //efficiency
  hparPy = (TH2D*) mFilePy->Get(Form("%sJetEta", par));
  if(!hparPy) { Info("Read", Form("histogram %sJetEta not found", par)); return 0;}

  TH2D *hjetmc = (TH2D*) mFileTest->Get(Form("%sDetectorJet", trg[i]));
  if(!hjetmc) { Info("Read", Form("histogram %sDetectorJet not found", trg[i])); return 0;}

  //TH2D* hparticle = (TH2D*) mFilePy->Get(Form("%sJetEta", par));
  //if(!hparticle) { Info("Read", Form("histogram %sJetEta not found", par)); return 0;}
  
  TH2D *hjetdet = (TH2D*) mFile->Get(Form("%sDetectorJet", trg[i]));
  if(!hjetdet) { Info("Read", Form("histogram %sDetectorJet not found", trg[i])); return 0;}

  htwojet = (TH2D*) mFileTest->Get(Form("%sTwoJet", trg[i]));
  if(!htwojet) { Info("Read", Form("histogram %sTwoJet not found", trg[i])); return 0;}

  TH2D *hvar = getVariance(hjetmc, htwojet);		
  StMyJetSVDUnfold *unfold = new StMyJetSVDUnfold(hdetVspar, hparPy, hjetdet, hjetmc, hvar);
  StMyJetSVDUnfoldResponseError *resp = new StMyJetSVDUnfoldResponseError(unfold);
  unfold->setResponseError(resp);
  StMyJetSVDUnfoldBias *bias = new StMyJetSVDUnfoldBias();
  unfold->setBias(bias);
  unfold->SetReg(mReg);
  unfold->Unfold();
  unfold->getReco()->Print("all");
  unfold->getRecoNoReg()->Print("all");
  resp->getProfile()->Print("all"); 
  bias->getProfile()->Print("all"); 
  mFileOut->Write();
  return -1;
  //hparPy->Print("all");
  TCanvas *cr = new TCanvas("cr", "cr");
  TH1D *hratio = vectorToHist(vr, "matchRatio");
  cr->cd();hratio->Draw();
  hratio->GetXaxis()->SetTitle("detector jet p_{T} bin index");
  cr->Print(Form("%s%s%sMatchRatio.png", trg[i],par,ver));
  //return -2;
  //test sample
  //get particle spectrum axis
  //TH2D* hparticle = (TH2D*) mFileTest->Get("ParticleJetEta");
  //test
  TArrayD *parbins = getBins(hparticle->GetXaxis()->GetXbins(), Ntrun[i]+dnshift, Mtrun[i]-upshift, mFlag);
  //cout<<"size: "<<parbins->GetSize()<<endl;
  //for(int ib = 0; ib < parbins->GetSize(); ib++){
  //cout<<ib<<" "<<parbins->At(ib)<<endl;
  //}
  TArrayD *detbins = getBins(hjetmc->GetXaxis()->GetXbins(), Ntrun[i], Mtrun[i], true);
  ///*
  //pt shift
  /*TProfile2D *hprofpt = (TProfile2D*) mFile->Get(Form("%sJetPtVs%sProfile", trg[i], par));
  TCanvas *cpt = new TCanvas("cpt", "cpt");
  TProfile *h1 = hprofpt->ProfileX("eta1", 1, 1, "s");
  //updateRmsAsError(h1);
  h1->Draw();
  h1->GetXaxis()->SetTitle("detector jet p_{T} [GeV]");
  h1->GetYaxis()->SetTitle("particle jet <p_{T}> [GeV]");
  h1->SetLineColor(kRed);
  //h1->Print("all");
  TProfile *h2 = hprofpt->ProfileX("eta2", 2, 2, "s");
  //updateRmsAsError(h2);
  h2->Draw("same");
  h2->SetLineColor(kBlue);
  //h2->Print("all");
  for(int ib = 0; ib < parbins->GetSize(); ib++){
    TLine *ln = new TLine(detbins->At(0), parbins->At(ib), detbins->At(detbins->GetSize()-1), parbins->At(ib));
    ln->Draw("same");
  }
  TLegend *lg = new TLegend(0.2, 0.7, 0.4, 0.8);
  lg->AddEntry(h1, "|#eta| < 0.5", "l");
  lg->AddEntry(h2, "0.5 < |#eta| < 0.9", "l");
  lg->Draw("same");

  cpt->Print(Form("%s%sPt.png", trg[i],ver));
  */
  //*/
  //plotRes(resp, resErrp, detbins, parbins, Form("%sShift%dResponse", trg[i], dnshift));
  //return -2;
  //}
  //return -2;

  TVectorD b = hist2dToVector(hjetmc, Ntrun[i], Mtrun[i], mFlag);
  b.Print();
  //matching ratio to b
  ElementMult(b, vr);
  
  TH2D *hvar = getVariance(hjetmc, htwojet);
  //hvar->Print("all");
  TMatrixD cov = histToMatrixTrunc2d(hvar, Ntrun[i], Mtrun[i], 2, mFlag);  
  cov.Print();
  //matching ratio correction to covariance matrix
  TMatrixD mr = DiagMatrix(vr);
  cov = mr*cov*mr;
  //TCanvas *ca = new TCanvas("ca", "ca");
  //ca->cd();
  //hdetVspar->Draw("colz");

  //TCanvas *cb = new TCanvas("cb", "cb");
  //cb->cd();
  //hdetVsparTest->Draw("colz");
  //
  //hdetVsparTest = (TH2D*) mFileTest->Get(Form("%sJetPtVsParticleJetPtRes", trg[i]));
  //if(!hdetVsparTest) { Info("Read", Form("histogram %sJetPtVsParticleJetPt not found", trg[i])); return 0;}

  //TMatrixD resTest = histToMatrixTrunc2d(hdetVsparTest, Ntrun[i], Mtrun[i], Ntrun[i]+dnshift, Mtrun[i]-upshift, 2);  
  //hparTest = hdetVsparTest->ProjectionY(Form("%sParticleTest", hdetVspar->GetName()));
  //hparTest->Print("all");
  //TVectorD xTest = histToVector2d(hparTest, Ntrun[i]+dnshift, Mtrun[i]-upshift, 2);  
  //cout<<"xTest: "<<endl;
  //xTest.Print("all");
  //response probability raw train
  //TMatrixD respTest(resTest);
  //respTest.NormByRow(xTest);
  //TMatrixD respDiff = resp-respTest;
  //respDiff.Print();
  //return -4;
  
  //TCanvas *ca = new TCanvas("ca", "ca");
  //ca->cd();
  //hdetVspar->Draw("colz");

  //TCanvas *cb = new TCanvas("cb", "cb");
  //cb->cd();
  //hdetVsparTest->Draw("colz");
  //TCanvas *ca = new TCanvas("ca", "ca");
  //ca->cd();
  //hpar->Draw();
  //hparTest->Draw("same");
  //hparTest->SetLineColor(2);
  //gPad->SetLogy(1);
  //ca->Print(Form("%sParticle.png", trg[i]));
  //
  //
  TMatrixD covf = covFactor(cov);
  //don't consider covariance matrix
  //covf.UnitMatrix();
  TMatrixD rescov = covf*res;
  TVectorD bcov = covf*b;
  
  //TGraph *grchi2 = new TGraph(Nrep+1);
  //response probability
  TMatrixD rescovp(rescov);
  //rescovp.NormByRow(x);
  //unfold

  //unfold without regularization
  TMatrixD ex0;
  TVectorD x0 = SVD(rescovp, bcov, ex0);
  ElementMult(x0, x);
  TMatrixD xm = DiagMatrix(x);
  ex0 = xm*ex0*xm;
  TH1D *hx0 = vectorToHist(x0, ex0, "recoNo");  
  TH2D *hex0 = matrixToHist(ex0, "recoNoError");
  //regularization
  int nxx = rescovp.GetNcols();
  TMatrixD cc = regMatrix(nxx/2, 2);
  TDecompSVD rsvd(rescovp);
  TVectorD sr = rsvd.GetSig();

  double reg = sr(mReg)*fReg;

  TCanvas *csig = new TCanvas("csig", "csig");
  TH1D *hsig = vectorToHist(sr, "hsig");
  csig->cd();
  hsig->Draw();
  gPad->SetLogy(1);
  TAxis *xaxis = hsig->GetXaxis();
  xaxis->SetTitle("bin index");
  double xmin = xaxis->GetXmin(), xmax = xaxis->GetXmax();
  TLine *lreg = new TLine(xmin, reg, xmax, reg);
  lreg->Draw("same");
  hsig->GetYaxis()->SetRangeUser(0.01*sr.Min(), 2*sr.Max());
  hsig->GetYaxis()->SetTitle("Singular Values");
  csig->Print(Form("%s%s%sSigAndReg.png", trg[i],par,ver));
  //
  TMatrixD ex;
  TVectorD xs = svdReg(rescovp, x, bcov, cc, reg, ex);
  //TMatrixD cn = reg*cc;
  //TMatrixD resn = addRows(rescovp, cn);

  //TVectorD bzero(cc.GetNrows());
  //bzero.Zero();
  //TVectorD bn = addRows(bcov, bzero);
  //bn.Print();
  //TVectorD xs = SVD(resn, bn, ex);
  //
  //double mod = TMath::Sqrt(x.Norm2Sqr());
  //TMatrixD ex0;
  //TVectorD x0 = SVD(rescovp, bcov, ex0);
  //x0.Print();
  //grchi2->SetPoint(0, 0, chi2Diff(x0, xTest));
  //TCanvas *ct = new TCanvas("ct", "ct");
  //TH1D *hx0 = vectorToHist(x0, ex0, "test0");
  //ct->cd();hx0->Draw();
  //hx0->SetLineColor(kRed);
  //TLine *ln = new TLine(x0.GetNrows()/2, hx0->GetYaxis()->GetXmin(), x0.GetNrows()/2, hx0->GetYaxis()->GetXmax());
  //ln->Draw("same");
  //canvas
  cResp = new TCanvas("cResp", "cResp");
  TH1D *hxs = vectorToHist(xs, ex, "reco");
  TH2D *hex = matrixToHist(ex, "recoError");
  cResp->cd(); hxs->Draw();
  gPad->SetLogy(1);
  hxs->SetLineColor(kRed);
  hxs->GetXaxis()->SetTitle("particle jet p_{T} bin index");
  
  //response
  TProfile *hprof = getRespSyst(res, resErr, x, covf, bcov, cc, reg);
  cResp->cd(); hxs->Draw("same");
  TCanvas *ct = new TCanvas("ct", "ct");
  ct->cd(); hxs->Draw();
  ct->cd(); hprof->Draw("pE2same");
  hprof->Print("all");
  hprof->SetLineColor(kBlue);
  hprof->SetFillColor(kBlue);
  TVectorD eresp(hprof->GetNbinsX());
  for(int ib = 1; ib <= hprof->GetNbinsX(); ib++){
     eresp(ib-1) = hprof->GetBinError(ib);
  }
  ct->cd(); gPad->SetLogy(1);
  double factor = xs.Sum()/x.Sum();
  TVector xscale = factor*x;
  TH1D *hxp = vectorToHist(xscale, "scale");
  ct->cd(); hxp->Draw("same");
  cout<<"reg: "<<reg<<endl;
  //bias
  TProfile *hprofbias = getBiasSyst(xscale, rescovp, cc, reg, factor);
  hprofbias->Print("all");
  TVectorD ebias(hprofbias->GetNbinsX());
  for(int ib = 1; ib <= hprofbias->GetNbinsX(); ib++){
     double cont = hprofbias->GetBinContent(ib);
     double error = hprofbias->GetBinError(ib);
     double bs = TMath::Sqrt(cont*cont+error*error); 
     //double bs = error;
     //double bs = TMath::Sqrt(cont);
     ebias(ib-1) = bs;
  }
  TH1D *hxbias = vectorToHist(xs, ebias, "recoBias");
  ct->cd(); hxbias->Draw("same");
  hxbias->SetLineColor(kGreen);
  hxbias->Print("all");
  TLegend *lct = new TLegend(0.7, 0.7, 0.85, 0.85);
  lct->AddEntry(hxs, "reco", "l");
  lct->AddEntry(hxp, "scaled emb", "l");
  lct->AddEntry(hprof, "resp", "l");
  lct->AddEntry(hxbias, "bias", "l");
  lct->Draw("same");
  hxs->Draw("same");
  ct->Print(Form("%s%s%sResTest.png", trg[i],par,ver));
  //
  cResp->Print(Form("%s%s%sRespSyst.png", trg[i],par,ver));
  cBias->cd(2); hxs->Draw("same");
  cBias->Print(Form("%s%s%sBiasSyst.png", trg[i],par,ver));

  TCanvas *creg = new TCanvas("creg", "creg");
  TLegend *lgreg = new TLegend(0.7, 0.7, 0.85, 0.85);
  creg->cd();hxs->Draw();
  hxs->GetYaxis()->SetRangeUser(0.01*xs.Min(), 2*xs.Max());
  creg->cd(); hx0->Draw("same");
  hx0->SetLineColor(kBlue);
  gPad->SetLogy(1);
  lgreg->AddEntry(hxs, "w reg.", "l");
  lgreg->AddEntry(hx0, "w/o reg.", "l");
  lgreg->Draw("same");
  creg->Print(Form("%s%s%sWithNoReg.png", trg[i],par,ver));

  //errors
  TCanvas *crr = new TCanvas("crr", "crr");
  TVectorD xe = getDiag(ex); xe.Sqrt();
  TVectorD rstat = ElemDiv(xe, xs);
  rstat.Print();
  TH1D *hrstat = vectorToHist(rstat, "hStat");
  hrstat->Draw(); hrstat->GetYaxis()->SetRangeUser(-0.1, 0.8);
  TVectorD rresp = ElemDiv(eresp, xs);
  TH1D *hrresp = vectorToHist(rresp, "hResp");
  hrresp->Draw("same"); hrresp->SetLineColor(kRed);
  TVectorD rbias = ElemDiv(ebias, xs);
  TH1D *hrbias = vectorToHist(rbias, "hBias");
  hrbias->Draw("same"); hrbias->SetLineColor(kBlue);
  crr->BuildLegend(0.6,0.7,0.8,0.85);
  crr->Print(Form("%s%s%sErrorRatio.png", trg[i],par,ver));
  
  //mFileOut->Write();
  //mFileOut->Close();
  //mFilePy->Close();
  //mFileTest->Close();
  //mFile->Close();
  return 1;
}
