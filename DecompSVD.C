TFile *mFile;
const int Ntrg = 3;

const char *trg[] = {"JP0", "JP1", "JP2"};


TH2D *hdetVspar;
//TH1D *hdet;
//TH1D *hproj;
TH1D *hpar;

TH1D *hjet;
TH2D *htwojet;

int i = 1;
int Ntrun[] = {1, 4, 6};
int Mtrun[] = {11, 13, 15};
int Nb = Mtrun[i] - Ntrun[i] + 1;

TMatrixD Cc(Nb, Nb);

TMatrixD Ii(Nb, Nb);

void initIi(double epsilon = 0.00001){
 for(int id = 0; id < Nb; id++){
    Ii(id, id) = epsilon*1.;
  } 
}
void initCc()
{
  Cc.Zero();
  Cc(0,0) = -1.;
  Cc(0,1) = 1.;
  for(int ic = 1; ic < Nb-1; ic++){
    Cc(ic,ic-1) = 1.;
    Cc(ic, ic) = -2.;
    Cc(ic, ic+1) = 1.;
  }
  Cc(Nb-1, Nb-2) = 1.;
  Cc(Nb-1, Nb-1) = -1.; 

  for(int id = 0; id < Nb; id++){
    Cc(id, id) += 1e-5;
  } 
}

//double Tau = 95.78*95.78;
//double Tau = 0.963*0.963;
//double Tau = 472.45*472.45;
double Tau = 7.12*7.12;

int DecompSVD(const char *file = "ptbin.list.run12.i.v1.w.cmb.mc.root")
{
  TH1::SetDefaultSumw2(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetHistLineWidth(2);

  gSystem->Load("/star/u/zchang/software/local/lib/libRooUnfold.so");
  
  mFile = TFile::Open(file);

  hdetVspar = (TH2D*) mFile->Get(Form("%sJetPtVsParticleJetPtRes", trg[i]));
  if(!hdetVspar) { Info("Read", Form("histogram %sJetPtVsParticleJetPt not found", trg[i])); return 0;}
  hjet = (TH1D*) mFile->Get(Form("%sJetPtMc", trg[i]));
  if(!hjet) { Info("Read", Form("histogram %sJetPtMc not found", trg[i])); return 0;}
  htwojet = (TH2D*) mFile->Get(Form("%sTwoJetPtMc", trg[i]));
  if(!htwojet) { Info("Read", Form("histogram %sTwoJetPtMc not found", trg[i])); return 0;}
  //htwojet->Print("all");

  hpar = hdetVspar->ProjectionY(Form("%sParticle", hdetVspar->GetName()));
  //cout<<"hpar: "<<endl;
  //hpar->Print("all");
  TMatrixD x = histToMatrix(hpar, Ntrun[i], Mtrun[i]);  
  cout<<"x: "<<endl;
  x.Print("all");

  TMatrixD res = histToMatrix(hdetVspar);
  //res.Print("all");

  TMatrixD restrun = histToMatrixTrunc(hdetVspar, Ntrun[i], Mtrun[i]);
  restrun.Print("all");
  initCc();
  initIi();

  TMatrixD b = histToMatrix(hjet, Ntrun[i], Mtrun[i]);

  //NormalEquation(restrun, b);

  //SVDReg(res,b);

  //covariance
  TH2D *hvar = getVariance(hjet, htwojet);
  //hvar->Print("all");
  TMatrixD cov_o = histToMatrix(hvar);
  //cout<<"Original Covariance Matrix"<<endl;
  //cov_o->Print("all");
  TMatrixD cov = histToMatrixTrunc(hvar, Ntrun[i], Mtrun[i]);
  cout<<"Covariance Matrix"<<endl;
  cov.Print("all");
  TDecompSVD svdcov(cov);
  TVectorD scov = svdcov.GetSig();
  //scov.Print("all");
  TMatrixD ucov = svdcov.GetU();
  ucov.Transpose(ucov);
  //ucov.Print("all");
  TMatrixD vcov = svdcov.GetV();
  //vcov.Print("all");
  TMatrixD r(Nb, Nb);
  for(int ir = 0; ir < Nb; ir++){
     if(scov[ir] > 0.) r(ir,ir) = 1./TMath::Sqrt(scov(ir));
  }
  //Info("", "new response matrix");
  TMatrixD rucov = r*ucov;
  TMatrixD rescov = r*ucov*restrun;
  rescov.Print("all");

  TMatrixD bcov = rucov*b;
  //Info("", "new detector response");
  bcov.Print("all"); 
  //
  SVDReg(rescov, bcov, x);

  return 1;
}
void NormalEquation(TMatrixD &res, TMatrixD &b)
{
  TMatrixD resT(Nb, Nb);
  resT.Transpose(res);
  TMatrixD res2 = resT*res;
  res2.Print("all");

  //Info("NormalEquation", "C matrix"); 
  //Cc.Print("all");
  
  TMatrixD CcT(Nb, Nb);
  CcT.Transpose(CcT);
  TMatrixD cc2 = CcT*Cc;

  //TMatrixD reg(Nb, Nb);
  //reg.Plus(Tau*cc2, res2);
  TMatrixD reg = Tau*cc2+res2;

  Info("NormalEquation", "Reg matrix");
  reg.Print("all");
  //TMatrixD regI(reg);
  //regI.Invert();
  //SVD inverse
  TMatrixD regI = Inverse(reg);
  //regI.Print("all");
  TMatrixD reco = regI*resT*b;
  Info("NormalEquation", "reconstructed vector");
  reco.Print("all");
}
void plotUV(char *name, const TMatrixD &u, const TMatrixD &v, const TVectorD &sv)
{
  TCanvas *c = new TCanvas(Form("c%s", name), Form("c%s", name), 960, 1200);
  c->Divide(3, 5);

  for(int ic = 0; ic < u.GetNcols(); ic++){
    //matrix U
    TVectorD vu(TMatrixDColumn(u, ic));
    //vu.Print("all");
    //cout<<ic<<endl;
    TH1D *hu = vectorToHist(vu, Form("U%d", ic+1));
    c->cd(ic+1);
    hu->Draw();
    hu->GetYaxis()->SetRangeUser(-1.1, 1.1);
    hu->GetYaxis()->SetTitle(Form("column %d", ic+1));
    hu->SetLineColor(kBlue);
    //matrix V
    TVectorD vv(TMatrixDColumn(v, ic));
    //vv.Print("all");
    //cout<<ic<<endl;
    TH1D *hv = vectorToHist(vv, Form("V%d", ic+1));
    c->cd(ic+1);
    hv->Draw("same");
    hv->SetLineColor(kRed);
  }
  c->Print(Form("%sVectors.png", name));

  TCanvas *c1 = new TCanvas(Form("SV%s", name), "c1", 400, 300);
  TH1D *hsv = vectorToHist(sv, Form("hSV%s", name));
  hsv->Draw("same");
  gPad->SetLogy(1);
  c1->Print(Form("SV%s.png", name));
}
void SVDReg(TMatrixD &res, TMatrixD &b, TMatrixD &x)
{
  
  TMatrixD xx(Nb, Nb);
  for(int ix = 0; ix < Nb; ix++){
     xx(ix,ix) = x(ix, 0);
  }
  TMatrixD Ccp(Cc);
  cout<<"matrix C"<<endl;
  Ccp.Print("all");
  //TMatrixD CcpI(Ccp);
  //CcpI.Invert();
  //svd inverse
  TMatrixD CcpI = Inverse(Ccp);
  //cout<<"C inverse svd"<<endl;
  //CcpI.Print("all");
  /*
  TMatrixD IIcc = Ccp*CcpI;
  cout<<"C*Cinv"<<endl;
  IIcc.Print("all");
  cout<<"C matrix SVD"<<endl;
  Ccp.Print("all");
  */
  TMatrixD resp = res*CcpI;
  TDecompSVD svdp(resp);
  TVectorD sp = svdp.GetSig();
  TMatrixD up = svdp.GetU();
  //up.Print("all");
  TMatrixD vp = svdp.GetV();
  //vp.Print("all");
  plotUV("ACInvSVD", up, vp, sp);
  /*
  TMatrixD testsvd(Nb, Nb);
  for(int it = 0; it < Nb; it++){
     testsvd(it, it) = sp(it);
  }
  
  //TMatrixD vpp(vp);
  vpp.Transpose(vpp);
  TMatrixD testresp = (up*testsvd)*vpp;
  cout<<"before svd"<<endl;
  resp->Print("all");
  cout<<"after svd"<<endl;
  testresp->Print("all");
  */
  Info("SVD", "singular values");
  sp.Print("all");
  TMatrixD uTp(Nb, Nb);
  uTp.Transpose(up);

  TMatrixD dp = uTp*b;
  cout<<"d-vector: "<<endl;
  dp.Print("all");
  TVectorD dpv(TMatrixDColumn(dp, 0));
  TH1D *hdpv = vectorToHist(dpv, Form("dVector"), true);
  TMatrixD dTaup(dp);
  for(int id = 0; id < Nb; id++){
    dTaup(id,0) = dTaup(id,0)*(TMath::Power(sp(id), 2)/(TMath::Power(sp(id), 2)+Tau));
  }
  cout<<"dTau-vector: "<<endl;
  dTaup->Print("all");
  TVectorD dTaupv(TMatrixDColumn(dTaup, 0));
  TH1D *hdTaupv = vectorToHist(dTaupv, Form("dVector%.2lf", Tau), true);
  TCanvas *c0 = new TCanvas("c0", "c0", 400, 300);
  gPad->SetLogy(1);
  hdpv->Draw();
  hdpv->GetYaxis()->SetRangeUser(1e-12, 1e4);
  hdTaupv->Draw("same");
  hdTaupv->SetLineColor(kRed);
  TLegend *lg = new TLegend(0.7, 0.7, 0.85, 0.85);
  lg->AddEntry(hdpv, "d", "l");
  lg->AddEntry(hdTaupv, Form("d^{%.2f}", Tau), "l");
  lg->Draw("same");
  c0->Print("d-vector.png");
  TMatrixD zp(Nb, 1);
  for(int iz = 0; iz < Nb; iz++){
    if(sp(i) > 0) zp(iz,0) = dp(iz, 0)*sp(iz)/(sp(iz)*sp(iz)+Tau);
    //d_i > 0
    //if(zp(iz, 0) < 0) zp(iz,0) *= -1.;
  }
  Info("SVD", "z-vector");
  zp.Print("all");

  //cout<<"v matrix: "<<endl;
  //vp.Print("all");
  TMatrixD cvp = CcpI*vp;
  cout<<"C-1V:"<<endl;
  cvp.Print("all");
  TMatrixD wp = cvp*zp;
  Info("SVD", "w-vector");
  wp.Print("all");
  TMatrixD xrec = xx*wp;
  cout<<"SVD reco: "<<endl;
  xrec.Print("all");
  //error calculations
  TMatrixD zmp(Nb, Nb);
  for(int izm = 0; izm < Nb; izm++){
    zmp(izm, izm) = TMath::Power(sp(izm)/(sp(izm)*sp(izm)+Tau), 2);
  }
  cout<<"z covariance matrix"<<endl;
  zmp.Print("all");

  TMatrixD cvpT(Nb, Nb);
  cvpT.Transpose(cvp);
  //cout<<"C-1V transpose: "<<endl;
  //cvpT.Print("all");
  TMatrixD wmp = cvp*zmp*cvpT;
  cout<<"W covariance matrix"<<endl;
  wmp.Print("all");
  TMatrixD recoCov = xx*wmp*xx;
  cout<<"reco covariance matrix"<<endl;
  recoCov.Print("all");
  for(int ir = 0; ir < Nb; ir++){
    double err = TMath::Sqrt(recoCov(ir, ir));
    cout<<ir<<" "<<err<<endl;
  }

  TMatrixD resT = Transpose(res);
  TMatrixD xxI(Nb, Nb);
  for(int ix = 0; ix < Nb; ix++){
     xxI(ix,ix) = 1./x(ix, 0);
  }
  TMatrixD xInv = xxI*resT*res*xxI;
  cout<<" X-1: "<<endl;
  xInv.Print("all");
  TMatrixD xInvTau = xInv*recoCov*xInv;
  cout<<" X-1Tau: "<<endl;
  xInvTau.Print("all");
  TMatrixD xDiff = xInvTau - xInv;
  cout<<"diff || X-1*XTau*X-1 - X-1|| < tau"<<endl;
  xDiff.Print("all");
  double det = xDiff.Determinant();
  cout<<"determinant: "<<det<<endl;
}
TMatrixD histToMatrixTrunc(const TH2D *h2, int nn, int mm)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int ny = yaxis->GetNbins();

  //cout<<nn<<" "<<mm<<endl;
  TMatrixD h2m (mm-nn+1, mm-nn+1);
  
  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      double content = h2->GetBinContent(ix, iy);
      //if(ix <= nn) h2m(0, iy-) += content;
      //if(iy <= nn) h2m(ix-1, 0) += content;
      int jx, jy;
      if(ix < nn) jx = 0;
      else if(ix < mm) jx = ix-nn;
      else jx = mm-nn;
      
      if(iy < nn) jy = 0;
      else if(iy < mm) jy = iy-nn;
      else jy = mm-nn;
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      h2m(jx, jy) += content;
    }
  }
  return h2m;
}
TMatrixD histToMatrix(TH1D *h1d, int nn, int mm)
{
  TAxis *xaxis = h1d->GetXaxis();
  int nx = xaxis->GetNbins();
  int Nx = mm-nn+1;//h1d->GetNbinsX();
  //int Ny = h2d->GetNbinsY();
  int Ny = 1;
  TMatrixD matrix(Nx, Ny);

  for(int ix = 0; ix <= nx; ix++){
    //for(int iy = 4; iy <= Ny; iy++){
      int jx;
      if(ix < nn) jx = 0;
      else if(ix < mm) jx = ix-nn;
      else jx = mm-nn;
      matrix[jx][0] += h1d->GetBinContent(ix);
      //}
  }
  return matrix;
}
TMatrixD histToMatrix(TH2D *h2d, bool transpose = false)
{
  int Nx = h2d->GetNbinsX();
  int Ny = h2d->GetNbinsY();

  TMatrixD matrix(Nx, Ny);

  matrix.Zero();
  for(int ix = 1; ix <= Nx; ix++){
    for(int iy = 1; iy <= Ny; iy++){
      if(!transpose) matrix[ix-1][iy-1] = h2d->GetBinContent(ix, iy);
      else matrix[iy-1][ix-1] = h2d->GetBinContent(ix, iy);
    }
  }
  return matrix;
}
TH1D *vectorToHist(TVectorD vec, char *name = "hist", bool abs = false)
{
  TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), 0, vec.GetNrows());
  for(int ib = 1; ib <= vec.GetNrows(); ib++){
    if(abs) h->SetBinContent(ib, TMath::Abs(vec[ib-1])); 
    else
       h->SetBinContent(ib, vec[ib-1]);
  }
  return h;
}
TH2D *getVariance(TH1D *h, TH2D *h2d, bool norm = false)
{
  TAxis *xaxis = h2d->GetXaxis();
  TAxis *yaxis = h2d->GetYaxis();

  TH2D *hh = new TH2D(Form("%sVar%d", h2d->GetName(), norm), Form(";%s;%s", xaxis->GetTitle(), yaxis->GetTitle()), xaxis->GetNbins(), xaxis->GetXbins()->GetArray(), yaxis->GetNbins(), yaxis->GetXbins()->GetArray());
  TArrayD arr(xaxis->GetNbins());
  for(int ix = 1; ix <= xaxis->GetNbins(); ix++){
    double eh = h->GetBinError(ix);
    double eh2 = h2d->GetBinError(ix, ix);
    arr[ix-1] = eh*eh + eh2*eh2;
  }
  for(int ix = 1; ix <= xaxis->GetNbins(); ix++){
    for(int iy = 1; iy <= yaxis->GetNbins(); iy++){
      double eh2 = h2d->GetBinError(ix, iy);
      double content = TMath::Power(eh2, 2);
      double var = content;
      if(iy == ix) { double err = h->GetBinError(ix); var += err*err;}

      if(norm && arr[ix-1] > 0 && arr[iy-1] > 0) var = var/TMath::Sqrt(arr[ix-1]*arr[iy-1]);
      hh->SetBinContent(ix, iy, var);
      hh->SetBinError(ix, iy, 0);
    }
  }
  return hh;
}
TMatrixD Inverse(const TMatrixD &m)
{
  TDecompSVD svdm(m);
  TMatrixD um = svdm.GetU();
  TMatrixD vm = svdm.GetV();
  TVectorD sm = svdm.GetSig();

  TMatrixD sIm(m.GetNrows(), m.GetNcols());
  for(int ic = 0; ic < m.GetNrows(); ic++){
    sIm(ic, ic) = 1./sm(ic);
    //tmpCc(ic, ic) = sCc(ic);
    cout<<"singular value "<<ic<<": "<<sm(ic)<<endl;
  }
  TMatrixD uTm(um.GetNcols(), um.GetNrows());
  uTm.Transpose(um);
  //TMatrixD vTm(vm.GetNcols(), vm.GetNrows());
  //vTm.Transpose(vm);
  
  TMatrixD mI = (vm*sIm)*uTm;
  cout<<"Inverse SVD"<<endl;
  mI.Print("all");
  return mI;
}
TMatrixD Transpose(const TMatrixD &m)
{
  TMatrixD mT(m.GetNcols(), m.GetNrows());
  mT.Transpose(m);
  return mT;
}
