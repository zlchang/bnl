#include "histUtilities.h"

#include "TFile.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TMath.h"
void printHist(TH1D *h, ofstream &of)
{
  int N = h->GetNbinsX();
  cout<<setw(10)<<"Bin"<<setw(10)<<"Content"<<setw(10)<<"Error"<<endl;
  of<<setw(10)<<"Bin"<<setw(10)<<"Content"<<setw(10)<<"Error"<<endl;
  for(int i = 1; i <= N; i++){
    float low = h->GetBinLowEdge(i);
    float cc = h->GetBinContent(i);
    float ee = h->GetBinError(i);
    
    cout<<setprecision(2)<<setw(10)<<low<<setw(10)<<cc<<setw(10)<<ee<<endl;
    of<<setprecision(2)<<setw(10)<<low<<setw(10)<<cc<<setw(10)<<ee<<endl;
  }
}
void combineHist(TH1D *h, TProfile **he, TH1D **hd, int Ntrg)
{
  int N = h->GetNbinsX();
  for(int ibin = 1; ibin <= N; ibin++){
    float content = 0;
    float error = 0;
    float sum = 0;
    float sum2 = 0;
    for(int itrg = 0; itrg < Ntrg; itrg++){
      float ec = he[itrg]->GetBinContent(ibin);
      float dc = hd[itrg]->GetBinContent(ibin);
      content += ec*dc;
      sum += dc;

      float ee = he[itrg]->GetBinError(ibin);
      error += ee*ee*dc*dc;
      sum2 += dc*dc;
    }
    if(sum > 0.){
      content /= sum;
      error /= sum2;
      error = TMath::Sqrt(error);
    }
    h->SetBinContent(ibin, content);
    h->SetBinError(ibin, error);
  }
}

//void drawHistRatio(TPad *p1, TPad *p2, TH1D *hdata, TH1D *hembed, TH1D *hratio, float xmin, float xmax, float ymin, float ymax)
void drawHistRatio(TPad *p1, TPad *p2, TH1 *hdata, TH1 *hembed, TH1 *hratio, float xmin, float xmax, float ymin, float ymax, float zoom, bool fnorm)
{
  p1->cd();
  //  gPad->SetLogy(1);
  if(fnorm) hdata->Scale(1./hdata->Integral());
  hdata->Draw("pE");
  hdata->SetLineColor(4);
  hdata->SetMarkerStyle(20);
  hdata->SetMarkerColor(4);
  hdata->GetXaxis()->SetRangeUser(xmin, xmax);
  hdata->GetYaxis()->SetRangeUser(ymin, ymax);

  if(fnorm) hembed->Scale(1./hembed->Integral());
  hembed->Draw("histsame");
  hembed->SetLineColor(2);
  hembed->SetLineWidth(2);

  p2->cd();
  hratio->Draw("p");
  hratio->SetMarkerStyle(4);
  hratio->GetXaxis()->SetRangeUser(xmin, xmax);
  hratio->GetYaxis()->SetRangeUser(-1.0*zoom, 1.0*zoom);

}
void drawHistRatio(TPad *p1, TPad *p2, TFile *fdata, TFile *fembed, TFile *fratio, char *name, float xmin, float xmax, float ymin, float ymax)
{
  TH1D *hdata = (TH1D *)fdata->Get(name);
  TH1D *hembed = (TH1D *)fembed->Get(name);
  TH1D *hratio = (TH1D *)fratio->Get(name);
  drawHistRatio(p1, p2, hdata, hembed, hratio, xmin, xmax, ymin, ymax);
}
void compareHistogram(TH1 *hist, TH1 *ha, TH1 *hb)
{
  if(ha && hb){
    if(ha->Integral() > 0 && hb->Integral() > 0){
      //ha->Scale(1./ha->Integral());
      hist->Add(ha, 1.0);
      //hb
      //hb->Scale(1./hb->Integral());
      hist->Add(hb, -1.0);
      hist->Divide(ha);
      //hist->GetYaxis()->SetTitle("(data-simu) / data");
    }
  }else if(!ha)
    //    cout<<"histogram "<<name_a<<" not found"<<endl;
    cout<<"histogram a not found"<<endl;
  else if(!hb)
    cout<<"histogram b not found"<<endl;
    //cout<<"histogram "<<name_b<<" not found"<<endl;
}
void compareProfile(TH1D *hist, TProfile *ha, TProfile *hb)
{
  if(ha && hb){
    int N = hist->GetNbinsX();
    for(int i = 1; i <= N; i++){
      float va = ha->GetBinContent(i);
      float ea = ha->GetBinError(i);
      float vb = hb->GetBinContent(i);
      float eb = hb->GetBinError(i);
      float r = 0.;
      float err = 0.;
      if(va > 0 || va < 0){
	r = 1 - vb / va;
	hist->SetBinContent(i, r);
	err = TMath::Sqrt(eb*eb+r*r*ea*ea)/va;
	hist->SetBinError(i, err);
      }
    }
    //hist->GetYaxis()->SetTitle("(data-simu) / data");
  }else if(!ha)
    cout<<"histogram a not found"<<endl;
  else if(!hb)
    cout<<"histogram b not found"<<endl;
}
void compareHistogram(TH1D *hist, TFile *fa, TFile *fb, char* name)
{
  //  TH1D *ha = (TH1D *)fa->Get(name_a);
  //  TH1D *hb = (TH1D *)fb->Get(name_b);
  TH1D *ha = (TH1D *)fa->Get(name);
  TH1D *hb = (TH1D *)fb->Get(name);
  compareHistogram(hist, ha, hb);
}
void addHistogram(TH1 *hist, TFile *f, char *name, float w)
{
  TH1 *hh = (TH1 *)f->Get(name);
  if(!hh)
    cout<<"histogram "<<name<<" not found"<<endl;
  else
    hist->Add(hh, w);
}
void plotHist(TCanvas *c, TH1** h, char *option, int Ntrg)
{
  for(int itrg = 0; itrg < Ntrg; itrg++){
    c->cd(itrg+1);
    //    gPad->SetLogy(0);                                                                     
    //    gPad->SetLogz(1);                                                                     
    h[itrg]->Draw(option);
  }
}
void drawHist(TFile *f, char*name, const char*option)
{
  TH1* h = (TH1*)f->Get(name);
  h->Draw(option);
}

TH1D *ratioHist(const char *name, TH1 *ha, TH1 *hb, bool flag)
{
  int N = ha->GetNbinsX();
  TH1D *hr = 0x0;
  if(ha->GetXaxis()->GetXbins()->GetSize() > 0)
    hr = new TH1D(name, Form(";%s;ratio", ha->GetXaxis()->GetTitle()), N, ha->GetXaxis()->GetXbins()->GetArray());
  else
    hr = new TH1D(name, Form(";%s;ratio", ha->GetXaxis()->GetTitle()), N, ha->GetXaxis()->GetXmin(), ha->GetXaxis()->GetXmax());
  for(int ib = 1; ib <= N; ib++){
    double aa = ha->GetBinContent(ib);
    double ea = ha->GetBinError(ib);
    double bb = hb->GetBinContent(ib);
    double eb = hb->GetBinError(ib);
    double rr = 0;
    double er = 0;
    if((aa > 0 || aa < 0) && (bb > 0 || bb < 0)){
      rr = (bb - aa)/aa;
      if(flag) er = bb/aa*TMath::Sqrt(TMath::Power(ea/aa, 2) + TMath::Power(eb/bb, 2));
    }
    hr->SetBinContent(ib, rr);
    hr->SetBinError(ib, er);
  }
  return hr;
}

TH1D *ratioHist(const char *name, TFile *fa, TFile *fb, bool flag)
{
  TH1 *ha = (TH1*) fa->Get(name);
  TH1 *hb = (TH1*) fb->Get(name);
  return ratioHist(Form("%sRatio", name), ha, hb, flag);
}
TH1D *rawRatioHist(const char *name, const TH1 *ha, const TH1 *hb, bool flag)
{
  int N = ha->GetNbinsX();
  TH1D *hr = 0x0;
  if(ha->GetXaxis()->GetXbins()->GetSize() > 0)
    hr = new TH1D(name, Form(";%s;ratio", ha->GetXaxis()->GetTitle()), N, ha->GetXaxis()->GetXbins()->GetArray());
  else
    hr = new TH1D(name, Form(";%s;ratio", ha->GetXaxis()->GetTitle()), N, ha->GetXaxis()->GetXmin(), ha->GetXaxis()->GetXmax());
  for(int ib = 1; ib <= N; ib++){
    double aa = ha->GetBinContent(ib);
    double ea = ha->GetBinError(ib);
    double bb = hb->GetBinContent(ib);
    double eb = hb->GetBinError(ib);
    double rr = 0;
    double er = 0;
    if(aa > 0 || aa < 0){
      rr = bb/aa;
      if(flag) er = bb/aa*TMath::Sqrt(TMath::Power(ea/aa, 2) + TMath::Power(eb/bb, 2));
    }
    hr->SetBinContent(ib, rr);
    hr->SetBinError(ib, er);
  }
  return hr;
}
TH1D *loadFiles(const char *file, float scale)
{
  TH1D *h = new TH1D(file, ";x ; y", 100, -0.5/scale, 99.5/scale);
  ifstream inf(file);
  float pt, crs;
  while(inf >> pt >> crs){
    pt /= scale;
    int bin = h->FindBin(pt);
    h->SetBinContent(bin, crs/1.0e+9);
  }
  inf.close();
  return h;
}
void printToFile(TH1* ha, ofstream &of, int xbin, int ybin)
{
  //int n = ha->GetNbinsX();
  for(int i = xbin; i <= ybin; i++){
    double aa = ha->GetBinContent(i);
    double ee = ha->GetBinError(i);
    //double aa = 0.;
    double pt = ha->GetBinLowEdge(i)+ha->GetBinWidth(i)/2;
    cout<<setw(5)<<setprecision(3)<<pt
        <<setw(10)<<setprecision(4)<<aa
        <<setw(10)<<setprecision(4)<<ee
        <<endl;
    of<<setw(5)<<setprecision(3)<<pt
      <<setw(10)<<setprecision(4)<<aa
      <<setw(10)<<setprecision(4)<<ee
      <<endl;
  }
}
