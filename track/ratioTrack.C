TFile *mFa, *mFb;
const char* trg[] = {"JP0", "JP1", "JP2"};
const int Ntrg = 3;
const char* type[] = {"All", "Pos", "Neg"};
const int Ntype = 3;
const char *ver = "v3a";

class drawObject;
void ratioTrack(
		const char * filea = "ptbin.list.run12.ef.track.v0.w.track.root",
		//const char *fileb = "ptbin.test.list.run12.e.track.v0.w.track.root")
		const char *fileb = "run12.final.v2.list.run12.ef.track.v0.w.track.root")
{
  gStyle->SetOptStat(0);
  gSystem->Load("StMyHistUtil");
  mFa = TFile::Open(filea);
  mFb = TFile::Open(fileb);
  //
  //plotRatio("EpVsPtClusterProf");
  //plotRatio("NSigmaPionProf");
  ///*
  //plotRatio("TrackFracProf");
  //plotRatio("NSigmaElectronProf");
  //plotRatio("NSigmaPionProf");
  //plotRatio("NSigmaKaonProf");
  //plotRatio("NSigmaProtonProf");
  //plotRatio("TrackNHitsProf");
  //*/
  //plotRatioNorm("NSigmaPion");
   
  for(int it = 0; it < Ntrg; it++){
    /*
    drawObject dtrack;
    dtrack.setLogy(true);
    dtrack.setYrange(1.0e-8, 1.1);
    plotRatio(Form("%s", trg[it]), "Track", getRatio1D, dtrack);
    //
    drawObject dmult;
    dmult.setYrange(0, 0.12);
    plotRatio(Form("%s", trg[it]), "TrackMult", getRatio1D, dmult);
    //
    drawObject dnn;
    dnn.setYrange(0, 0.08);
    plotRatio(Form("%s", trg[it]), "TrackNHits", getRatioNorm, dnn);
    */
    //
    drawObject dep;
    dep.setYrange(0, 0.08);
    plotRatio(Form("%s", trg[it]), "EpVsPt", getRatioNorm, dep);
    /*
    //
    drawObject dnhits;
    dnhits.setYrange(0, 52);    
    plotRatio(Form("%s", trg[it]), "TrackNHitsProf", getRatio, dnhits);
    //
    drawObject ddca;
    ddca.setYrange(0, 1.2);
    plotRatio(Form("%s", trg[it]), "TrackDcaPtProf", getRatio, ddca);
    //
    */
    drawObject depp;
    depp.setYrange(0, 0.6);
    plotRatio(Form("%s", trg[it]), "EpVsPtProf", getRatio, depp);
    /*
    drawObject depc;
    depc.setYrange(0, 1.1);
    plotRatio(Form("%s", trg[it]), "EpVsPtClusterProf", getRatio, depc);
    */
  }
  //plotPM();
  //plotFrac();
  //plotNSigma();
}
void plotRatio(const char* prefix, const char*name, TH1D* (*ratio)(const char*, TH1D **, TH1D **), const drawObject &obj)
{
  int ncx = 1, ncy = 2;
  TCanvas *cr = new TCanvas(Form("cr%s%s", prefix, name), Form("cr%s%s", prefix, name), ncx*480, ncy*360);
  cr->Divide(ncx, ncy);
  TLegend *lg = new TLegend(obj.lx1, obj.ly1, obj.lx2, obj.ly2);
  int color = 2;
  for(int ii = 0; ii < Ntype; ii++){
    TH1D *ha, *hb;
    TH1D *hr = ratio(Form("Cmb%s%s%s", prefix, type[ii], name), &ha, &hb);
    //hr->Print("all");
    cr->cd(1);
    if(ha && hb){
      ha->Print();
      hb->Print();
      cr->cd(1);
      if(ii == 0){
	ha->Draw("histE");
	if(obj.ymax > obj.ymin) ha->GetYaxis()->SetRangeUser(obj.ymin, obj.ymax);
      }else{
	ha->Draw("histEsame");
      }
      ha->SetLineColor(ii+color);
      hb->Draw("pEsame");
      hb->SetLineColor(ii+color);
    }else
      Printf("%x %x", ha, hb);
    gPad->SetLogy(obj.logy);
    //TH1D *hrnsigpi = getRatioNorm(Form("Cmb%sNSigmaPion", type[ii])); 
    cr->cd(2);
    if(ii == 0){
      hr->Draw();
      hr->GetYaxis()->SetTitle("#frac{data-emb}{emb}");
      hr->GetYaxis()->SetRangeUser(-1.7, 1.7);
    }else hr->Draw("same");
    //hr->Fit("pol0", "RS", "same", 0, 3);
    //TF1 *f1 = hr->GetFunction("pol0");
    //lg->AddEntry(hr, Form("%.3lf#pm %.3lf\n", f1->GetParameter(0), f1->GetParError(0)), "l");
    hr->SetLineColor(ii+color);
    //f1->SetLineColor(ii+color);
    //f1->SetLineStyle(1);
    lg->AddEntry(hr, Form("%s", type[ii]), "l");
  }
  cr->cd(2);
  lg->Draw("same");
  cr->Print(Form("%s%sRatio%s.png", prefix, name, ver));
}
TH1D* getRatio(const char* name, TH1D **ha, TH1D **hb)
{
  mFa->cd();
  (*ha) = convertTProfile(mFa, name);
  mFb->cd();
  (*hb) = convertTProfile(mFb, name);
  
  //TH1D *hr = ratioHist("CmbAllEpVsPtProf", mFa, mFb);
  TH1D *hr = ratioHist(Form("%sRatio", name), (*ha), (*hb));
  //TCanvas *c = new TCanvas(name, name);
  return hr;
}
TH1D* getRatioNorm(const char* name, TH1D ** ha, TH1D ** hb)
{
  mFa->cd();
  Printf("%s", name);
  TH2D *ha2d = (TH2D *)mFa->Get(Form("%sScatter", name));
  (*ha) = ha2d->ProjectionY(Form("%s_apy", name));
  //ha->Rebin(20);
  double na = (*ha)->Integral();
  (*ha)->Scale(1/na);
  //(*ha)->Print();
  mFb->cd();
  TH2D *hb2d = (TH2D *)mFb->Get(Form("%sScatter", name));
  (*hb) = hb2d->ProjectionY(Form("%s_bpy", name));
  //hb->Rebin(20);
  double nb = (*hb)->Integral();
  (*hb)->Scale(1/nb);
  (*hb)->Print();
  //TH1D *hr = ratioHist("CmbAllEpVsPtProf", mFa, mFb);
  TH1D *hr = ratioHist(Form("%sRatio", name), (*ha), (*hb), true);
  //hr->Fit("pol0", "R", "", 0, 2);
  //TCanvas *c = new TCanvas(name, name);
  //hr->Draw();
  //hr->Fit("pol0", "R", "", -1, 1);
  //hr->GetXaxis()->SetRangeUser(-3, 3);
  //hr->GetYaxis()->SetRangeUser(0.8, 1.2);
  //c->Print(Form("%sRatio.png", name));
  return hr;
}
TH1D *getRatio1D(const char* name, TH1D **ha, TH1D **hb)
{
  //Printf("%s", name);
  mFa->cd();
  (*ha) = (TH1D*) mFa->Get(Form("%s", name));
  double na = (*ha)->Integral();//(11, 40);
  (*ha)->Scale(1/na);
  (*ha)->Print();
  
  mFb->cd();
  (*hb) = (TH1D *) mFb->Get(Form("%s", name));
  double nb = (*hb)->Integral();//11, 40);
  (*hb)->Scale(1/nb);
  //hb->Print("all");
  
  TH1D *hr = ratioHist(Form("%sRatio", name), (*ha), (*hb));
  return hr;
}
void plotPM()
{
  TH1D *hra = getRatioPtoM(mFa, "embed");
  TH1D *hrb = getRatioPtoM(mFb, "data");

  TCanvas *c1 = new TCanvas;
  hra->Draw();
  hra->GetYaxis()->SetRangeUser(0.8, 1.2);
  hra->GetYaxis()->SetTitle("h^{+}/h^{-}");
  hra->SetLineColor(kRed);
  hrb->Draw("same");
  hrb->SetLineColor(kBlue);
  c1->Print(Form("PMRatio%s.png", ver));
}
void plotFrac()
{

  double ymin = -1, ymax = 1;
  int ncx = 1, ncy = 1;
  TCanvas *cr = new TCanvas("cfrac", "cfrac", ncx*480, ncy*360);
  cr->Divide(ncx, ncy);
  TLegend *lg = new TLegend(0.79, 0.83, 0.99, 0.98);
  int color = 2;
  for(int it = 0; it < Ntype; it++){
    //cr->cd(it+1);
    mFa->cd();
    TH2D *hha = (TH2D*) mFa->Get(Form("Cmb%sNSigmaPionScatter", type[it]));
    TH1D *hra = getFrac(hha, ymin, ymax);
    //
    mFb->cd();
    TH2D *hhb = (TH2D*) mFb->Get(Form("Cmb%sNSigmaPionScatter", type[it]));
    TH1D *hrb = getFrac(hhb, ymin, ymax);

    if(it == 0){
      hra->Draw("hist");
      hra->GetYaxis()->SetRangeUser(0.2, 0.8);
      hra->GetYaxis()->SetTitle("frac(|n#sigma_{#pi}| < 1)");
    }else hra->Draw("histsame");
    hra->SetLineColor(it+color);

    hrb->Draw("psame");
    hrb->SetLineColor(it+color);
    hrb->SetMarkerStyle(7);
    hrb->SetMarkerColor(it+color);
    if(it == 0){
      lg->AddEntry(hra, Form("%s embed", type[it]), "pl");
      lg->AddEntry(hrb, Form("%s data", type[it]), "p");
    }else
      lg->AddEntry(hra, Form("%s", type[it]), "l");
  }
  lg->Draw("same");
  cr->Print(Form("frac%s.png", ver));

}
void plotNSigma()
{
  const int ns = 4;
  const char *sname[] = {"Pion", "Kaon", "Proton", "Electron"};
  int ncx = 2, ncy = 2;
  TCanvas *cn = new TCanvas("cnsig", "cnsig", ncx*480, ncy*360);
  cn->Divide(ncx, ncy);
  int color = 1;
  TLegend *lg = new TLegend(0.3, 0.3, 0.7, 0.7);
  for(int it = 0; it < Ntype; it++){
    cn->cd(it+1);
    for(int is = 0; is < ns; is++){ 
      mFa->cd();
      TH2D *hha = (TH2D*) mFa->Get(Form("Cmb%sNSigma%sScatter", type[it], sname[is]));
      TH1D *hpa = hha->ProjectionY(Form("%s%proj", type[it], sname[is]));
      double sa = hpa->Integral(); hpa->Scale(1/sa);
      //hpa->SetLineStyle(1);
      hpa->SetLineColor(is+color);
      //
      mFb->cd();
      TH2D *hhb = (TH2D*) mFb->Get(Form("Cmb%sNSigma%sScatter", type[it], sname[is]));
      TH1D *hpb = hhb->ProjectionY(Form("%s%proj", type[it], sname[is]));
      double sb = hpb->Integral(); hpb->Scale(1/sb);
      //hpb->SetLineStyle(2);
      hpb->SetLineColor(is+color);
      hpb->SetMarkerStyle(7);
      hpb->SetMarkerColor(is+color);
      
      if(is == 0){
	hpa->Draw("hist");
	hpa->GetYaxis()->SetRangeUser(-0.01, 0.09);
	hpa->GetYaxis()->SetTitle("Normalized counts");
	if(it == 0){
	  lg->AddEntry(hpa, "embed", "pl");
	  lg->AddEntry(hpb, "data", "p");
	}
      }else
	hpa->Draw("histsame");
      hpb->Draw("psame");
      if(it == 0){
	lg->AddEntry(hpa, sname[is], "l");
      }
    }
  }
  cn->cd(4);
  lg->Draw();
  cn->Print(Form("NSigma%s.png", ver));
}
TH1D *getFrac(TH2D *hh, double ymin, double ymax)
{
  TAxis *xaxis = hh->GetXaxis();
  TH1D *hr = new TH1D(Form("%sFrac", hh->GetName()), Form(";%s; frac", xaxis->GetTitle()), xaxis->GetNbins(), xaxis->GetXmin(), xaxis->GetXmax());
  int nx = hh->GetNbinsX();
  for(int ix = 1; ix <= nx; ix++){
    TH1D *hp = (TH1D *) hh->ProjectionY(Form("%sproj%d", hh->GetName(), ix), ix, ix);
    double sum = hp->Integral();
    int jya = hh->GetYaxis()->FindBin(ymin);
    int jyb = hh->GetYaxis()->FindBin(ymax);

    //Printf("jya=%d, jyb=%d\n", jya, jyb);
    double cut = hp->Integral(jya, jyb);
    double frac = sum > 0 ? cut/sum : 0;
    //Printf("ix=%d frac=%f\n", ix, frac);
    hr->SetBinContent(ix, frac);
  }
  //hr->Print("all");
  return hr;
}
TH1D *getRatioPtoM(TFile *file, char *name)
{
  TH1D *hp = (TH1D*) file->Get("CmbPosTrack");
  TH1D *hm = (TH1D*) file->Get("CmbNegTrack");

  TH1D *hr = hp->Clone(Form("%sRatio", name));
  hr->Divide(hm);
  return hr;
}
