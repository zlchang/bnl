TFile *mFile;
TFile *mFileData;
const int Ntrg = 3;

const char *trg[] = {"JP0", "JP1", "JP2"};


TH2D *hdetVspar;
TH1D *hdet;
TH1D *hproj;
TH1D *hpar;
//TSVDUnfold remove hvar at the end of the program
//TH2D *hvar;
TH2D *hvarOrig;
TH1D *hjet;
TH2D *htwojet;
//histograms for SVD singular values and d-vector
TH1D *hsv;
TH1D *hd;
TH1D *hz;

int Ntrun[] = {1, 4, 8};
int Mtrun[] = {11, 13, 15};

int i = 1;
double Factor = 1.0;
int UnfoldJet(const char *file = "ptbin.list.run12.i.v1.w.cmb.mc.root",
const char* datafile = "run12.final.v2.list.crr.v1.asym.root")
{
  TH1::SetDefaultSumw2(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetHistLineWidth(2);

  gSystem->Load("/star/u/zchang/software/local/lib/libRooUnfold.so");
  
  mFile = TFile::Open(file);
  mFileData = TFile::Open(datafile);

  TH2D *hdetVsparOrig = (TH2D*) mFile->Get(Form("%sJetPtVsParticleJetPtRes", trg[i]));
  if(!hdetVsparOrig) { Info("Read", Form("histogram %sJetPtVsParticleJetPt not found", trg[i])); return 0;}
  //TH1D *hjetOrig = (TH1D*) mFile->Get(Form("%sJetPtMc", trg[i]));
  //if(!hjetOrig) { Info("Read", Form("histogram %sJetPtMc not found", trg[i])); return 0;}
  //TH2D *htwojetOrig = (TH2D*) mFile->Get(Form("%sTwoJetPtMc", trg[i]));
  //if(!htwojetOrig) { Info("Read", Form("histogram %sTwoJetPtMc not found", trg[i])); return 0;}
  TH1D *hjetOrig = (TH1D*) mFileData->Get(Form("%sJetPt", trg[i]));
  if(!hjetOrig) { Info("Read", Form("histogram %sJetPt not found", trg[i])); return 0;}
  TH2D *htwojetOrig = (TH2D*) mFileData->Get(Form("%sTwoJetPt", trg[i]));
  if(!htwojetOrig) { Info("Read", Form("histogram %sTwoJetPt not found", trg[i])); return 0;}
  TH1D *hdetOrig = updateErrors(hjetOrig, htwojetOrig);

  //x: detector y: parton/particle
  
  hdetVspar = histTrunc(hdetVsparOrig, Ntrun[i], Mtrun[i], Ntrun[i], Mtrun[i]);
  hpar = (TH1D *)hdetVspar->ProjectionY(Form("%sPar", trg[i]));
  //hproj = (TH1D *)hdetVspar->ProjectionX(Form("%sDetector", trg[i]));

  hjet = histTrunc(hjetOrig, Ntrun[i], Mtrun[i]);
  Factor = hjet->Integral();  
  //normal
  htwojet = histTrunc(htwojetOrig, Ntrun[i], Mtrun[i], Ntrun[i], Mtrun[i]);
  hvarOrig = getVariance(hjetOrig, htwojetOrig);
  //hdet = hproj;
  hjet->Print("all");
  TH2D *hcorrOrig = getVariance(hjetOrig, htwojetOrig, true);
  TCanvas *c0 = new TCanvas("c0", "c0");
  gPad->SetLogz(1);
  c0->cd();
  hcorrOrig->Draw("colz");
  c0->Print(Form("%sCorrelation.png", trg[i]));

  TCanvas *c0a = new TCanvas("c0a", "c0a");
  //gPad->SetLogz(1);
  c0a->cd();
  cout<<"response matrix: "<<endl;
  //hdetVspar->Print("all");
  hdetVsparOrig->Draw("colz");
  c0a->Print(Form("%sResponse.png", trg[i]));
  //hcorr->Print("all");

  //matching ratio
  TProfile *hmatch = (TProfile*) mFile->Get(Form("%sJetParticleNoMatchProf", trg[i]));
  if(!hmatch) { Info("Read", Form("histogram %sJetParticleNoMatchProf not found", trg[i]));}
  TCanvas *c0b = new TCanvas("c0b", "c0b");
  gPad->SetLogz(1);
  c0b->cd();
  hmatch->Draw("hist");
  hmatch->GetYaxis()->SetRangeUser(0.4, 1.2);
  c0b->Print(Form("%sMatching.png", trg[i]));


  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->cd();
  //hdet->Draw("E");
  TGraphErrors *grdet = histToGraph(hjet, "jet");
  grdet->Draw("paE");
  grdet->GetXaxis()->SetLimits(-1, 60);
  grdet->GetYaxis()->SetRangeUser(5e2, 3e+7);
  grdet->GetXaxis()->SetRangeUser(0, 60);

  
  //TH1D* hreco = UnfoldBayes();
  //hreco->SetLineColor(kBlue);  
  //hreco->Draw("Esame");

  TH1D *hparScale = hpar->Clone("parCopy");
  hparScale->Scale(Factor/hpar->Integral());
  TGraphErrors *grpar = histToGraph(hparScale, "particle");
  grpar->Draw("pEsame");
  grpar->SetLineColor(kRed);
  //hpar->SetLineColor(kRed);
  //hpar->Draw("E");

  TH1D *hrecoSVD[15];
  TGraphErrors *grrecoSVD[15];
  TH1D *hrecoDReg[15];
  TH1D *hrecoZReg[15];
  int start = 2, end = 6;
  int step = 2;
  TCanvas *c1a = new TCanvas("c1a", "c1a");
  TCanvas *c1b = new TCanvas("c1b", "c1b");

  for(int ir = start; ir <= end; ir+=step){ 
    hrecoSVD[ir] = UnfoldSVD(ir);
    c1->cd(); 
    //hrecoSVD[ir]->SetLineColor(kGreen+(ir-start)/step);
    //hrecoSVD[ir]->Draw("Esame");
    grrecoSVD[ir] = histToGraph(hrecoSVD[ir], Form("Reg%d", ir));
    grrecoSVD[ir]->SetLineColor(kGreen+(ir-start)/step);
    grrecoSVD[ir]->Draw("pEsame");

    c1a->cd();
    if(ir == start){
       hd->Draw();
       gPad->SetLogy(1);
       hd->GetYaxis()->SetRangeUser(1e-12, 1e+4);
    }
    hrecoDReg[ir] = getHistDReg(ir);
    hrecoDReg[ir] ->SetLineColor(kGreen+(ir-start)/step);
    hrecoDReg[ir]->Draw("same");

    //z
    c1b->cd();
    if(ir == start){
       hz->Draw();
       gPad->SetLogy(1);
       //hz->GetYaxis()->SetRangeUser(1e-8, 1e+4);
       hz->GetYaxis()->SetRangeUser(1e+0, 1e+13);
    }
    hrecoZReg[ir] = getHistZReg(ir);
    hrecoZReg[ir] ->SetLineColor(kGreen+(ir-start)/step);
    hrecoZReg[ir]->Draw("same");

  }

  c1->cd();
  gPad->SetLogy(1);
/*
  TH1D *htunfold = UnfoldRoot();
  htunfold->Draw("Esame");
  htunfold->SetLineColor(kYellow);
*/
  TLegend *lg = new TLegend(0.65, 0.7, 0.85, 0.85);
  lg->AddEntry(grdet, "Detector", "l");
  lg->AddEntry(grpar, "Parton/Particle", "l");
  //lg->AddEntry(hreco, "Unfold Bayes", "l");
  for(int ir = start; ir <= end; ir+=step){
    lg->AddEntry(grrecoSVD[ir], Form("Unfold SVD reg = %d", ir), "l");
  }
  //lg->AddEntry(htunfold, "TUnfold", "l");
  lg->Draw("same");

  c1->Print(Form("%sUnfold.png", trg[i]));
  c1a->cd();
  TLegend *lga = new TLegend(0.65, 0.72, 0.85, 0.87);
  lga->AddEntry(hd, "d", "l");
  for(int ir = start; ir <= end; ir+=step){
    lga->AddEntry(hrecoDReg[ir], Form("d^{(%d)}", ir), "l");
  }
  lga->Draw("same");
  c1a->Print(Form("%sDVector.png", trg[i]));
  c1b->cd();
  TLegend *lgb = new TLegend(0.35, 0.72, 0.55, 0.87);
  lgb->AddEntry(hz, "z", "l");
  for(int ir = start; ir <= end; ir+=step){
    lgb->AddEntry(hrecoZReg[ir], Form("z^{(%d)}", ir), "l");
  }
  lgb->Draw("same");
  c1b->Print(Form("%sZVector.png", trg[i]));
  TCanvas *c1c = new TCanvas("c1c", "c1c");
  hsv->Draw();
  gPad->SetLogy(1);
  c1c->Print(Form("%sSV.png", trg[i]));
  TCanvas *c2 = new TCanvas("c2", "c2");
  c2->cd();
  gPad->SetLogy(0);
  //TH1D *hr = ratioHist("M/C", hpar, hpar);
  TH1D *hr = ratioHist("M/C", hparScale, hparScale);

  hr->Draw();
  hr->GetYaxis()->SetLimits(-1, 1);
  hr->GetYaxis()->SetRangeUser(-1, 1);
  hr->SetLineColor(kRed);
  //TH1D *hrbayes = ratioHist("Bayes", hpar, hreco);
  //hrbayes->Draw("same");
  //hrbayes->SetLineColor(kBlue);

  TH1D *hrSVD[15];
  for(int ir = start; ir <= end; ir+=step){
    //hrSVD[ir] = ratioHist(Form("SVDreg%d", ir), hpar, hrecoSVD[ir]);
    hrSVD[ir] = ratioHist(Form("SVDreg%d", ir), hparScale, hrecoSVD[ir]);
    hrSVD[ir]->Draw("same");
    hrSVD[ir]->SetLineColor(kGreen+(ir-start)/step);
  }
  c2->Print(Form("%sUnfoldRatio.png", trg[i]));


  return 1;
}
TH2D* histTrunc(const TH2D *h2, int nnx, int mmx, int nny, int mmy)
{
  TAxis *xaxis = h2->GetXaxis();
  TAxis *yaxis = h2->GetYaxis();

  int nx = xaxis->GetNbins();
  int ny = yaxis->GetNbins();

  //cout<<nn<<" "<<mm<<endl;

  int nhx = mmx-nnx+1;
  int nhy = mmy-nny+1;

  TArrayD binsx(nhx+1);
  TArrayD *h2x = xaxis->GetXbins();
  binsx[0] = 0;
  for(int ix = 1; ix <= nhx; ix++){
    binsx[ix] = h2x->At(ix+nnx-1);
    //cout<<binsx[ix]<<endl;
  }

  TArrayD binsy(nhy+1);
  TArrayD *h2y = yaxis->GetXbins();
  binsy[0] = 0;
  for(int iy = 1; iy <= nhy; iy++){
    binsy[iy] = h2y->At(iy+nny-1);
    //cout<<binsy[iy]<<endl;
  }

  TH2D *h2t = new TH2D(Form("%sTrunc", h2->GetName()), "", nhx, binsx.GetArray(), nhy, binsy.GetArray());

  for(int ix = 1; ix <= nx; ix++){
    for(int iy = 1; iy <= ny; iy++){
      double content = h2->GetBinContent(ix, iy);
      //if(ix <= nn) h2m(0, iy-) += content;
      //if(iy <= nn) h2m(ix-1, 0) += content;
      int jx, jy;
      if(ix < nnx) jx = 0;
      else if(ix < mmx) jx = ix-nnx;
      else jx = mmx-nnx;

      if(iy < nny) jy = 0;
      else if(iy < mmy) jy = iy-nny;
      else jy = mmy-nny;
      //cout<<ix<<" "<<iy<<" new: "<<jx<<" "<<jy<<endl;
      h2t->AddBinContent((jx+1)+(jy+1)*(nhx+2), content);
    }
  }
  return h2t;
}
TH1D* histTrunc(const TH1D *h1, int nn, int mm)
{
  TAxis *xaxis = h1->GetXaxis();
  int nhx = mm-nn+1;

  TArrayD binsx(nhx+1);
  TArrayD *h1x = xaxis->GetXbins();
  //binsx[0] = 0;
  for(int ix = 0; ix <= nhx; ix++){
    binsx[ix] = h1x->At(ix+nn-1);
    cout<<binsx[ix]<<endl;
  }  

  //cout<<nn<<" "<<mm<<endl;

  TH1D *h1t = new TH1D(Form("%sTrunc", h1->GetName()), "", nhx, binsx.GetArray());

  int nx = xaxis->GetNbins();
  for(int ix = 1; ix <= nx; ix++){
    double content = h1->GetBinContent(ix);
    int jx;
    if(ix < nn) jx = 0;
    else if(ix < mm) jx = ix-nn;
    else jx = mm-nn;
    h1t->AddBinContent(jx+1, content);     
  }
  return h1t;
}
TH1D *UnfoldRoot(int reg = 0)
{
  int N = hdet->GetNbinsX();
  if(reg == 0) reg = N;

  TH2D *hvar = getVariance(hjet, htwojet);
  //hvar->Print("all");

  //TSVDUnfold svdUnfold(hdet, hdet, hpar, hdetVspar);
  
  TUnfold unfoldRoot(hdetVspar, TUnfold::kHistMapOutputVert, TUnfold::kRegModeCurvature, TUnfold::kEConstraintNone);
  
  unfoldRoot.SetInput(hdet, 0., 0., hvar);
  unfoldRoot.DoUnfold(10);
  TH1D *hrec =  new TH1D(Form("%s", "TUnfold"), ";p_{T} [GeV]", N, hpar->GetXaxis()->GetXbins()->GetArray());
  unfoldRoot.GetOutput(hrec);
  
  //svdUnfold.GetSV()->Print("all");
  //svdUnfold.GetD()->Print("all");
  //svdUnfold.GetAdetCovMatrix(10)->Print("all");
  //svdUnfold.GetUnfoldCovMatrix(10)->Print("all");
  hrec->Print("all");

  return hrec;
}
TH1D *UnfoldBayes()
{
  RooUnfoldResponse response(hdet, hpar, hdetVspar, Form("%sResponse", trg[i]));

  RooUnfoldBayes unfold(&response, hdet, 4);

  return (TH1D*) unfold.Hreco();

}
TH1D *UnfoldSVD(int reg = 0, bool normal =true)
{
  int N = hjet->GetNbinsX();
  if(reg == 0) reg = N;
  cout<<N<<endl;
  /*TH1D *hcov = new TH1D(Form("%sMeasuredCov", trg[i]), ";p_{T} [GeV]", N, hdet->GetXaxis()->GetXbins()->GetArray());

  for(int i = 1; i <= N; i++){
    float content = hdet->GetBinContent(i);
    float error = hdet->GetBinError(i);
    hcov->SetBinContent(i, content);
    hcov->SetBinError(i, error);
  }
  hcov->Sumw2(0);
  */

  //TH1D *hdettrunc = histTrunc(hdet, Ntrun[i], Mtrun[i]);
  //TH2D *hvar = getVariance(hjet, htwojet);
  //TH2D *hvartrunc = histTrunc(hvar, Ntrun[i], Mtrun[i]);
  //TH2D *hdetVspartrunc = histTrunc(hdetVspar, Ntrun[i], Mtrun[i]);
  //TH1D *hpartrunc = hdetVspartrunc->ProjectionY("truncPar");
  //TH1D *hprojtrunc = hdetVspartrunc->ProjectionX("truncDetector");
  //hvar->Print("all");

  //TSVDUnfold svdUnfold(hdet, hdet, hpar, hdetVspar);
  TH2D *hvar = histTrunc(hvarOrig, Ntrun[i], Mtrun[i], Ntrun[i], Mtrun[i]);
  //hjet->Print("all"); 
  //cout<<hvar<<endl;hvar->Print("all");
  //if(hpar) hpar->Print("all"); 
  //else exit;
  //hdetVspar->Print("all"); 
  cout<<"Regulation: "<<reg<<endl;
  TSVDUnfold svdUnfold(hjet, hvar, hjet, hpar, hdetVspar);
  //TSVDUnfold svdUnfold(hdettrunc, hvartrunc, hprojtrunc, hpartrunc, hdetVspartrunc);
  
  TH1D *hsvd =  recoSVD(svdUnfold, reg);
  //svdUnfold.GetSV()->Print("all");
  //svdUnfold.GetD()->Print("all");
  //svdUnfold.GetAdetCovMatrix(10)->Print("all");
  //svdUnfold.GetUnfoldCovMatrix(10)->Print("all");
  hsvd->Print("all");
  //hsvd->Scale(Factor/hsvd->Integral());
  return hsvd;
}
TH1D *recoSVD(TSVDUnfold &unfold, int reg)
{
  TH1D *hunfold = (TH1D*) unfold.Unfold(reg);
  //make a copy of SV
  TH1D *hsvtmp = unfold.GetSV();
  if(!hsv) hsv = (TH1D *)hsvtmp->Clone("SVcopy");
  unfold.GetSV()->Print("all");
  //make a copy of D-vector
  TH1D *hdtmp = unfold.GetD();
  if(!hd) hd = (TH1D *)hdtmp->Clone("dcopy");
  if(!hz) { hz = (TH1D *)hdtmp->Clone("zcopy"); hz->Divide(hsv); hz->SetTitle("Z-vector");}
  unfold.GetD()->Print("all");
  ///*
  //int niter = 10;
  TH2D *hcov = (TH2D*) unfold.GetXtau();
  TH2D *hinv = (TH2D*) unfold.GetXinv();
  hinv->Print("all");
  cout<<"Regulation: "<<reg<<endl;
  TH1D *hsvd = hpar->Clone(Form("SVD%d", reg));
  cout<<"Regulation: "<<reg<<endl;
  for(int ib = 1; ib <= hunfold->GetNbinsX(); ib++){
     double content = hunfold->GetBinContent(ib);
     double err = hcov->GetBinContent(ib, ib);
     err = TMath::Sqrt(err);
     hsvd->SetBinContent(ib, content);
     hsvd->SetBinError(ib, err);
  }//*/
  return hsvd;
}
TH1D *getHistDReg(int reg){
  TH1D *hd_reg = (TH1D *)hd->Clone(Form("dReg%d", reg));
  for(int ib = 1; ib <= hd_reg->GetNbinsX(); ib++){
    double content = hd_reg->GetBinContent(ib);
    double sreg = hsv->GetBinContent(reg);
    double si = hsv->GetBinContent(ib);
    content *= TMath::Power(si, 2)/(TMath::Power(si, 2)+TMath::Power(sreg, 2));
    cout<<"content: "<<content<<" sreg: "<<sreg<<endl;
    hd_reg->SetBinContent(ib, content);
    //hd_reg->SetBinError(ib, TMath::Sqrtcontent);
  }
  return hd_reg;
}
TH1D *getHistZReg(int reg){
  TH1D *hz_reg = (TH1D *)hz->Clone(Form("zReg%d", reg));
  for(int ib = 1; ib <= hz_reg->GetNbinsX(); ib++){
    double content = hz_reg->GetBinContent(ib);
    double sreg = hsv->GetBinContent(reg);
    double si = hsv->GetBinContent(ib);
    content *= TMath::Power(si, 2)/(TMath::Power(si, 2)+TMath::Power(sreg, 2));
    cout<<"z content: "<<content<<" sreg: "<<sreg<<endl;
    hz_reg->SetBinContent(ib, content);
    //hd_reg->SetBinError(ib, TMath::Sqrtcontent);
  }
  return hz_reg;
}
TH1D *ratioHist(const char *name, TH1 *ha, TH1 *hb)
{
  int N = ha->GetNbinsX();
  TH1D *hr = 0x0;
  if(ha->GetXaxis()->GetXbins()->GetSize() > 0)
    hr = new TH1D(name, Form(";%s;ratio", ha->GetXaxis()->GetTitle()), N, ha->GetXaxis()->GetXbins()->GetArray());
  else
    hr = new TH1D(name, Form(";%s;ratio", ha->GetXaxis()->GetTitle()), N, ha->GetXaxis()->GetXmin(), ha->GetXaxis()->GetXmax());
  for(int ib = 1; ib <= N; ib++){
    double aa = ha->GetBinContent(ib);
    //double ea = ha->GetBinError(ib);
    double bb = hb->GetBinContent(ib);
    //double eb = hb->GetBinError(ib);
    double rr = 0;
    double er = 0;
    if(aa > 0 || aa < 0){
      rr = (bb - aa)/aa;
      //er = bb/aa*TMath::Sqrt(TMath::Power(ea/aa, 2) + TMath::Power(eb/bb, 2));
    }
    hr->SetBinContent(ib, rr);
    hr->SetBinError(ib, er);
  }
  return hr;
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
  /*
  for(int ix = 1; ix <= xaxis->GetNbins(); ix++){
    cout<<h->GetBinError(ix)<<endl;
    for(int iy = 1; iy <= yaxis->GetNbins(); iy++){
      if(ix == iy)
	hh->SetBinContent(ix, iy, h->GetBinError(ix));
      else hh->SetBinContent(ix, iy, 0);
    } 
  }
  */
  return hh;
}
TH1D* updateErrors(TH1D *h, TH2D *h2d)
{
  TAxis* xaxis = h->GetXaxis();
  //TAxis* yaxis = h->GetYaxis();
  int N = xaxis->GetNbins();
  cout<<N<<endl;
  TH1D *hn = new TH1D(Form("%sUpdate", h->GetName()), Form(";%s", xaxis->GetTitle()), N, xaxis->GetXbins()->GetArray());
  for(int ib = 1; ib <= N; ib++){
    double content = h->GetBinContent(ib);
    double err1 = h->GetBinError(ib);
    double err2 = h2d->GetBinError(ib, ib);
    double error = TMath::Sqrt(TMath::Power(err1,2)+TMath::Power(err2, 2));
    hn->SetBinContent(ib, content);
    hn->SetBinError(ib, error);
    cout<<"content: "<<content<<" error:"<<error<<" err1:"<<err1<<" err2:"<<err2<<endl;
  }
  return hn;
}

TGraphErrors *histToGraph(TH1D *h, char *name)
{
  int N = h->GetNbinsX();
  TGraphErrors *gr = new TGraphErrors(N);
  gr->SetName(Form("%s%sGraph", h->GetName(), name));

  for(int ib = 1; ib <= N; ib++){
	double x = h->GetBinCenter(ib);
        double y = h->GetBinContent(ib);
        double xe = h->GetBinWidth(ib)/2.;
        double ye = h->GetBinError(ib);
	gr->SetPoint(ib-1, x, y);
        gr->SetPointError(ib-1, xe, ye);
  }
  return gr;
}
