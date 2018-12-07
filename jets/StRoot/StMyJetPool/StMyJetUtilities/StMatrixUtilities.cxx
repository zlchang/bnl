#include<iostream>

#include "StMatrixUtilities.h"

//#include "TMatrixD.h"
//#include "TArrayD.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TDecompSVD.h"
#include "TMath.h"
#include "TMatrixDUtilsfwd.h"

using namespace std;

TMatrixD covFactor(const TMatrixD &cov)
{
  const int Nb = cov.GetNrows();
  TDecompSVD svdcov(cov);
  TVectorD scov = svdcov.GetSig();
  //scov.Print("all");
  TMatrixD ucov = svdcov.GetU();
  ucov.Transpose(ucov);
  //ucov.Print("all");
  //TMatrixD vcov = svdcov.GetV();
  //vcov.Print("all");
  TMatrixD r(Nb, Nb);
  for(int ir = 0; ir < Nb; ir++){
    if(scov[ir] > 0.) r(ir,ir) = 1./TMath::Sqrt(scov(ir));
  }
  //Info("", "new response matrix");
  TMatrixD rucov = r*ucov;
  return rucov;
}
TMatrixD regMatrix(int nx, int ny)
{
  TMatrixD cc(nx*ny+nx, nx*ny);
  
  for(int ix = 0; ix < nx*ny+nx; ix++){
    if(ix < nx*ny){
      if(ix%nx == 0){
	cc(ix, ix) = -1;
	cc(ix, ix+1) = 1;
      }else if(ix%nx == nx-1){
	cc(ix, ix) = -1;
	cc(ix, ix-1) = 1;
      }
      else{
	cc(ix, ix-1) = 1;
	cc(ix, ix) = -2;
	cc(ix, ix+1) = 1;
      }
    }else{
      cc(ix, ix-nx*ny) = -1;
      cc(ix, ix-nx*ny+nx) = 1;
    }
  }
  return cc;
}
TVectorD svdReg(const TMatrixD &resp, const TVectorD &xi, const TVectorD &b, const TMatrixD &cc, double reg, TMatrixD &ex)
{
  TMatrixD cn = reg*cc;
  //cc.Print();
  TMatrixD respn = addRows(resp, cn);

  TVectorD bzero(cc.GetNrows());
  bzero.Zero();
  TVectorD bn = addRows(b, bzero);
  //bn.Print();

  //TMatrixD ex;
  TVectorD xs = SVD(respn, bn, ex);
  //cout<<"SVD: "<<endl;
  //xs.Print();
  //ex.Print();
  //TVectorD xrec(xs);
  ElementMult(xs, xi);
  TMatrixD xm = DiagMatrix(xi);
  ex = xm*ex*xm;

  //return xrec;
  //return xs.GetSub(0, resp.GetNcols()-1);
  return xs;
}
TVectorD SVD(const TMatrixD &A, const TVectorD &b, TMatrixD &e)
{
  TDecompSVD svd(A);
  TVectorD s = svd.GetSig();
  TMatrixD u = svd.GetU();
  //u.Print("all");
  TMatrixD v = svd.GetV();

  TMatrixD uT(u);
  uT.Transpose(u);

  TVectorD d = uT*b;

  //cout<<"d: "<<endl;
  //d.Print();
  TVectorD z(A.GetNcols());
  TMatrixD zm(A.GetNcols(), A.GetNcols());

  for(int iz = 0; iz < A.GetNcols(); iz++){
    z(iz) = d(iz)/s(iz);
    zm(iz, iz) = 1/TMath::Power(s(iz), 2);
  }

  //TMatrixD wm = v*zm*Transpose(v);
  //wm.Print();
  e.ResizeTo(A.GetNcols(), A.GetNcols());
  e = v*zm*Transpose(v);
  return v*z;
}
TMatrixD NormalEquation(const TMatrixD &res, const TMatrixD &b, const TMatrixD &c, double tau)
{
  TMatrixD resT = Transpose(res);
  TMatrixD res2 = resT*res;
  //Info("NormalEquation", "C matrix"); 
  //Cc.Print("all");

  TMatrixD cT= Transpose(c);
  TMatrixD c2 = cT*c;

  TMatrixD reg = tau*c2+res2;

  //Info("NormalEquation", "Reg matrix");
  //reg.Print("all");
  //TMatrixD regI(reg);
  //regI.Invert();
  //SVD inverse
  TMatrixD regI = Inverse(reg);
  //regI.Print("all");
  TMatrixD reco = regI*resT*b;

  //Info("NormalEquation", "reconstructed vector");
  //reco.Print("all");

  return reco;
}
TVectorD projRows(const TMatrixD &mm)
{
  int nr = mm.GetNrows();
  TVectorD proj(nr);
  for(int ir = 0; ir < nr; ir++){
    TVectorD row(TMatrixDRow_const(mm, ir));
    proj(ir) = row.Sum();
  }
  return proj;
}
TVectorD projCols(const TMatrixD &mm)
{
  int nc = mm.GetNcols();
  Printf("projCols: nc = %d", nc);
  TVectorD proj(nc);
  for(int ic = 0; ic < nc; ic++){
    TVectorD col(TMatrixDColumn_const(mm, ic));
    proj(ic) = col.Sum();
  }
  return proj;
}
TMatrixD DiagMatrix(const TVectorD &vec)
{
  int nr = vec.GetNrows();
  TMatrixD diag(nr, nr);
  for(int ir = 0; ir < nr; ir++){
    diag(ir,ir) = vec(ir);
  }
  return diag;
}
TMatrixD addRows(const TMatrixD &a, const TMatrixD &b)
{
  int nrow_a = a.GetNrows();
  int ncol_a = a.GetNcols();

  int nrow_b = b.GetNrows();
  int ncol_b = b.GetNcols();

  if(ncol_a != ncol_b) return TMatrixD(nrow_a+nrow_b, 1);

  TMatrixD m(nrow_a+nrow_b, ncol_a);

  for(int ic = 0; ic < ncol_a; ic++){
    for(int ir = 0; ir < nrow_a; ir++){
      m(ir,ic) = a(ir,ic);
    }
    for(int ir = nrow_a; ir < nrow_a + nrow_b; ir++){
      m(ir, ic) = b(ir-nrow_a, ic);
    }
  }
  return m;
}
TVectorD addRows(const TVectorD &a, const TVectorD &b)
{
  int nrow_a = a.GetNrows();
  int nrow_b = b.GetNrows();

  TVectorD v(nrow_a+nrow_b);

  for(int ir = 0; ir < nrow_a; ir++){
    v(ir) = a(ir);
  }
  for(int ir = nrow_a; ir < nrow_a + nrow_b; ir++){
    v(ir) = b(ir-nrow_a);
  }
  return v;
}
TVectorD ElemDiv(const TVectorD &va, const TVectorD &vb)
{
  int N = va.GetNrows();
  if(N != vb.GetNrows()) return va;
  TVectorD vd(N);
  for(int ix = 0; ix < N; ix++){
      if(vb(ix) > 0. || vb(ix) < 0.) vd(ix) = va(ix)/vb(ix);
  }
  return vd;
}
TMatrixD ElemDiv(const TMatrixD &ma, const TMatrixD &mb)
{
  int N = ma.GetNrows();
  int M = ma.GetNcols();
  TMatrixD md(N, M);
  for(int ix = 0; ix < N; ix++){
    for(int iy = 0; iy < M; iy++){
      if((mb(ix, 0) > 0 || mb(ix, 0) < 0) && (mb(iy,0) > 0 || mb(iy,0) < 0)) md(ix,iy) = ma(ix, iy)/mb(ix, 0)/mb(iy, 0);
      //cout<<md(ix,iy)<<" "<<ma(ix, iy)<<endl;
    }
  }
  return md;
}
int convertIndex(int xx, int nn, int mm, bool flag)
{
  //return -1: underflow/overflow if flag true
  //return index: within range
  int jx = -1;
  if(xx < nn){ if(!flag) jx = 0;}    
  else if(xx <= mm){ jx = xx-nn;}
  else{ if(!flag) jx = mm-nn;}
  
  return jx;
}
int convertIndex2d(int xx, int nn, int mm, int tx, bool flag)
{
  //return -1: underflow/overflow if flag true
  //return index: within range
  int jx = -1;
  if((xx-1)%tx < nn-1){ if(!flag) jx = 0;}
  else if((xx-1)%tx <= mm-1){ jx = (xx-1)%tx-nn+1;}
  else {if(!flag) jx = mm-nn;}
  if(jx != -1) jx += (xx-1)/tx*(mm-nn+1);
  return jx;
}
///*
int convertIndex2dn(int xx, int nn, int mm, int tx, bool flag)
{
  //return -1: underflow/overflow if flag true
  //return index: within range
  int jx = -1;
  if((xx-1)%tx < nn){ if(!flag) jx = 0;}
  else if((xx-1)%tx <= mm){ jx = (xx-1)%tx-nn;}
  else {if(!flag) jx = mm-nn;}
  if(jx != -1) jx += (xx-1)/tx*(mm-nn+1);
  return jx;
}
//*/
bool checkflow(int xx, int nn, int mm)
{
  if((xx-1)/nn == 0 || (xx-1)/nn == mm+1) return false;
  return true;
}
int convert2dIndex(int xx, int nn, int mm, int yy, bool flag)
{
  //return -1: underflow/overflow if flag true
  //return index: within range
  int jx = -1;
  if(xx < nn){ if(!flag) jx = 0;}
  else if(xx <= mm){ jx = xx-nn;}
  else {if(!flag) jx = mm-nn;}
  if(jx != -1) jx += (yy-1)*(mm-nn+1);
  return jx;
}
TMatrixD histToMatrixTrunc2d(const TH2D *h2, int nn, int mm, int tt, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int tx = nx/tt;
  int ny = yaxis->GetNbins();
  int ty = ny/tt;

  //cout<<nn<<" "<<mm<<endl;
  TMatrixD h2m ((mm-nn+1)*tt, (mm-nn+1)*tt);

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      double content = h2->GetBinContent(ix, iy);
      //if(ix <= nn) h2m(0, iy-) += content;
      //if(iy <= nn) h2m(ix-1, 0) += content;
      
      int jx = convertIndex2d(ix, nn, mm, tx, flag);
      int jy = convertIndex2d(iy, nn, mm, ty, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1)
	h2m(jx, jy) += content;
    }
  }
  return h2m;
}
//asymmetric cuts on x- and y- axises
TMatrixD histToMatrixTrunc2d(const TH2D *h2, int nnx, int mmx, int nny, int mmy, int tt, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int tx = nx/tt;
  int ny = yaxis->GetNbins();
  int ty = ny/tt;

  //cout<<nn<<" "<<mm<<endl;
  TMatrixD h2m ((mmx-nnx+1)*tt, (mmy-nny+1)*tt);

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      double content = h2->GetBinContent(ix, iy);
      int jx = convertIndex2d(ix, nnx, mmx, tx, flag);
      int jy = convertIndex2d(iy, nny, mmy, ty, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1)
	h2m(jx, jy) += content;
    }
  }
  return h2m;
}
TMatrixD histToMatrixTrunc2dn(const TH2D *h2, int nnx, int mmx, int nny, int mmy, int tt, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int tx = nx/(tt+2);
  int ny = yaxis->GetNbins();
  int ty = ny/(tt+2);
  
  //cout<<tx<<" "<<ty<<endl;
  TMatrixD h2m ((mmx-nnx+1)*tt, (mmy-nny+1)*tt);

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      if(!(checkflow(ix, tx, tt) && checkflow(iy, ty, tt))) continue;
      double content = h2->GetBinContent(ix, iy);
      int jx = convertIndex2dn(ix-tx, nnx, mmx, tx, flag);
      int jy = convertIndex2dn(iy-ty, nny, mmy, ty, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1)
	h2m(jx, jy) += content;
    }
  }
  return h2m;
}
//asymmetric cuts on x- and y- axises
TMatrixD histToMatrixTruncError2d(const TH2D *h2, int nnx, int mmx, int nny, int mmy, int tt, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int tx = nx/tt;
  int ny = yaxis->GetNbins();
  int ty = ny/tt;

  //cout<<nn<<" "<<mm<<endl;
  TMatrixD h2m ((mmx-nnx+1)*tt, (mmy-nny+1)*tt);

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      double error = h2->GetBinError(ix, iy);
      error *= error;
      //if(ix <= nn) h2m(0, iy-) += content;
      //if(iy <= nn) h2m(ix-1, 0) += content;
      int jx = convertIndex2d(ix, nnx, mmx, tx, flag);
      int jy = convertIndex2d(iy, nny, mmy, ty, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1)
	h2m(jx, jy) += error;
    }
  }
  return h2m;
}
TMatrixD histToMatrixTruncError2dn(const TH2D *h2, int nnx, int mmx, int nny, int mmy, int tt, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int tx = nx/(tt+2);
  int ny = yaxis->GetNbins();
  int ty = ny/(tt+2);

  //cout<<nn<<" "<<mm<<endl;
  TMatrixD h2m ((mmx-nnx+1)*tt, (mmy-nny+1)*tt);

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      if(!(checkflow(ix, tx, tt) && checkflow(iy,ty,tt))) continue;
      double error = h2->GetBinError(ix, iy);
      error *= error;
      //if(ix <= nn) h2m(0, iy-) += content;
      //if(iy <= nn) h2m(ix-1, 0) += content;
      int jx = convertIndex2dn(ix-tx, nnx, mmx, tx, flag);
      int jy = convertIndex2dn(iy-ty, nny, mmy, ty, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1)
	h2m(jx, jy) += error;
    }
  }
  return h2m;
}
TVectorD histToVector2d(const TH1D *h1, int nn, int mm, int tt, bool flag)
{
  TAxis *xaxis = h1->GetXaxis();

  int nx = xaxis->GetNbins();
  int tx = nx/tt;


  //cout<<nn<<" "<<mm<<endl;
  TVectorD h1v ((mm-nn+1)*tt);

  for(int ix = 1; ix <= nx; ix++){
    double content = h1->GetBinContent(ix);
    int jx = convertIndex2d(ix, nn, mm, tx, flag);
    //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
    if(jx != -1)
      h1v(jx) += content;
  }
  return h1v;
}
TVectorD hist2dToVector(const TH2D *h2, int nn, int mm, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int ny = yaxis->GetNbins();
  
  //int tx = nx/tt;

  //cout<<nn<<" "<<mm<<endl;
  TVectorD h1v ((mm-nn+1)*ny);

  for(int iy = 1; iy <= ny; iy++){
    for(int ix = 1; ix <= nx; ix++){
      double content = h2->GetBinContent(ix, iy);
      int jx = convert2dIndex(ix, nn, mm, iy, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1)
	h1v(jx) += content;
    }
  }
  return h1v;
}
 TVectorD hist2dToVectorError(const TH2D *h2, int nn, int mm, bool flag)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int ny = yaxis->GetNbins();
  
  TVectorD h1e ((mm-nn+1)*ny);
  
  for(int iy = 1; iy <= ny; iy++){
    for(int ix = 1; ix <= nx; ix++){
      double error = h2->GetBinError(ix, iy);
      int jx = convert2dIndex(ix, nn, mm, iy, flag);
      if(jx != -1)
	h1e(jx) += error*error;
    }
  }
  return h1e;
}
 TMatrixD histToMatrixTrunc(const TH2D *h2, int nn, int mm, bool flag)
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
      int jx = convertIndex(ix, nn, mm, flag);
      int jy = convertIndex(iy, nn, mm, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1) h2m(jx, jy) += content;
    }
  }
  return h2m;
}
//asymmetrix cut on both x and y axises
 TMatrixD histToMatrixTrunc(const TH2D *h2, int nnx, int mmx, int nny, int mmy, bool flag)
 {
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int ny = yaxis->GetNbins();

  //cout<<nn<<" "<<mm<<endl;
  TMatrixD h2m (mmx-nnx+1, mmy-nny+1);

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      double content = h2->GetBinContent(ix, iy);
      //if(ix <= nn) h2m(0, iy-) += content;
      //if(iy <= nn) h2m(ix-1, 0) += content;
      int jx = convertIndex(ix, nnx, mmx, flag);
      int jy = convertIndex(iy, nny, mmy, flag);
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      if(jx != -1 && jy != -1)
	h2m(jx, jy) += content;
    }
  }
  return h2m;
}
TVectorD histToVector(const TH1D *h1d, int nn, int mm, bool flag)
{
  TAxis *xaxis = h1d->GetXaxis();
  int nx = xaxis->GetNbins();
  int Nx = mm-nn+1;//h1d->GetNbinsX();
  //int Ny = h2d->GetNbinsY();
  TVectorD vec(Nx);

  for(int ix = 1; ix <= nx; ix++){
    //for(int iy = 4; iy <= Ny; iy++){
    int jx = convertIndex(ix, nn, mm, flag);
    if(jx != -1)
      vec[jx] += h1d->GetBinContent(ix);
    //}
  }
  return vec;
}
TVectorD histToVector(const TH1D *h1d)
{
  TAxis *xaxis = h1d->GetXaxis();
  int nx = xaxis->GetNbins();

  TVectorD vec(nx);
  
  for(int ix = 1; ix <= nx; ix++){
    vec[ix-1] += h1d->GetBinContent(ix);
  }
  return vec;
}
TVectorD histToVectorError(const TH1D *h1d)
{
  TAxis *xaxis = h1d->GetXaxis();
  int nx = xaxis->GetNbins();

  TVectorD vec(nx);
  
  for(int ix = 1; ix <= nx; ix++){
    vec[ix-1] += h1d->GetBinError(ix);
  }
  return vec;
}

TMatrixD histToMatrix(const TH2D *h2d, bool transpose)
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
TH1D *vectorToHist(const TVectorD &vec, const char *name, bool abs)
{
  TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), 0, vec.GetNrows());
  for(int ib = 1; ib <= vec.GetNrows(); ib++){
    if(abs) h->SetBinContent(ib, TMath::Abs(vec[ib-1]));
    else
      h->SetBinContent(ib, vec[ib-1]);
  }
  return h;
}
TH1D *vectorToHist(const TVectorD &vec, const TMatrixD &mm, const char *name, bool abs)
{
  TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), 0, vec.GetNrows());
  for(int ib = 1; ib <= vec.GetNrows(); ib++){
    if(abs) h->SetBinContent(ib, TMath::Abs(vec[ib-1]));
    else
      h->SetBinContent(ib, vec[ib-1]);
    h->SetBinError(ib, TMath::Sqrt(mm(ib-1, ib-1)));
  }
  return h;
}
TH1D *vectorToHist(const TVectorD &vec, const TVectorD &verr, const char *name, bool abs)
{
  TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), 0, vec.GetNrows());
  for(int ib = 1; ib <= vec.GetNrows(); ib++){
    if(abs) h->SetBinContent(ib, TMath::Abs(vec[ib-1]));
    else
      h->SetBinContent(ib, vec[ib-1]);
    double err = verr[ib-1];
    h->SetBinError(ib, err);
  }
  return h;
}
TH1D *vectorToHist(const TVectorD &vec, const TVectorD &verr, const TArrayD *bins, const char *name, bool abs)
{
  //TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), 0, vec.GetNrows());
  TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), bins->GetArray());
  for(int ib = 1; ib <= vec.GetNrows(); ib++){
    if(abs) h->SetBinContent(ib, TMath::Abs(vec[ib-1]));
    else
      h->SetBinContent(ib, vec[ib-1]);
    double err = verr(ib-1);
    h->SetBinError(ib, err);
  }
  return h;
}
TH1D *vectorToHist(const TVectorD &vec, const TArrayD *bins,  const char *name, bool abs)
{
  //TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), 0, vec.GetNrows());
  TH1D *h = new TH1D(Form("%s", name), "", vec.GetNrows(), bins->GetArray());
  for(int ib = 1; ib <= vec.GetNrows(); ib++){
    if(abs) h->SetBinContent(ib, TMath::Abs(vec[ib-1]));
    else
      h->SetBinContent(ib, vec[ib-1]);
  }
  return h;
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
    //cout<<"singular value "<<ic<<": "<<sm(ic)<<endl;
  }
  TMatrixD uTm(um.GetNcols(), um.GetNrows());
  uTm.Transpose(um);
  //TMatrixD vTm(vm.GetNcols(), vm.GetNrows());
  //vTm.Transpose(vm);

  TMatrixD mI = (vm*sIm)*uTm;
  //cout<<"Inverse SVD"<<endl;
  //mI.Print("all");
  return mI;
}
TMatrixD Transpose(const TMatrixD &m)
{
  TMatrixD mT(m.GetNcols(), m.GetNrows());
  mT.Transpose(m);
  return mT;
}
TH2D *matrixToHist(const TMatrixD &m, const TArrayD *xbns, const TArrayD *ybns, const char *name)
{
  int nx = m.GetNrows();
  int ny = m.GetNcols();

  TH2D *h2 = new TH2D(Form("%s2D", name), "", nx, xbns->GetArray(), ny, ybns->GetArray());

  for(int ix = 0; ix < nx; ix++){
     for(int iy = 0; iy < ny; iy++){
        double content = m(ix, iy);
        h2->SetBinContent(ix+1, iy+1, content);
     }
  }
  return h2;
}

TH2D *matrixToHist(const TMatrixD &m, const char *name)
{
  int nx = m.GetNrows();
  int ny = m.GetNcols();

  TH2D *h2 = new TH2D(Form("%s2D", name), "", nx, 0, nx, ny, 0, ny);

  for(int ix = 0; ix < nx; ix++){
     for(int iy = 0; iy < ny; iy++){
        double content = m(ix, iy);
        h2->SetBinContent(ix+1, iy+1, content);
     }
  }
  return h2;
}
