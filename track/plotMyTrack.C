TFile *mFile;
const char* type[] = {"CmbAll", "CmbPos", "CmbNeg"};
//const char* type[] = {"All", "Pos", "Neg"};
const int Nn = 3;

class drawObject{
public:
  drawObject(){
    ymin = 0; ymax = -1;
    lx1 = 0.46; ly1 = 0.74; lx2 = 0.76; ly2 = 0.89;
    logy = false;
    //lx1 = 0.2; ly1 = 0.70; lx2 = 0.5; ly2 = 0.85;
  }
  double ymin, ymax;
  double zmin, zmax;
  double lx1,ly1,lx2,ly2;
  bool logy;
  void setLegend(double x1, double y1, double x2, double y2){ lx1 = x1; ly1 = y1; lx2 = x2; ly2 = y2;}
  void setYrange(double min, double max){ymin = min; ymax = max;}
  void setZrange(double min, double max){zmin = min; zmax = max;}
  void setLogy(bool flag){logy = flag;}
};
const char* ver = "v3";
//const char* ver = "Datav3";
int plotMyTrack(//const char* file = "test.track.root"
const char* file = "ptbin.list.run12.e.track.v0.w.track.root"
//const char* file = "run.emc.list.run12.e.track.v0.w.track.root"
)
{
  
  mFile = TFile::Open(file);
 
  gStyle->SetOptStat(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPalette(1, 0);
  ///*
  drawObject d0;
  d0.setLogy(1);
  //d1.setYrange(-0.1, 1.7);
  //d1.setLegend(0.2, 0.7, 0.5, 0.85);
  drawHist("Track", d0);
/*
  drawObject d0a;
  d0a.setLogy(1);
  drawHist("Tower", d0a);
*/
/*
  drawObject d0c;
  d0c.setLegend(0.5, 0.3, 0.8, 0.45);
  drawHistProfile("TrackFracProf", d0c);
*/
/*
  drawObject d0d;
  drawHist("NSigmaElectron", d0d);

  drawObject d0e;
  d0e.setLegend(0.2, 0.7, 0.5, 0.85);
  drawHist("NSigmaPion", d0e);
*/
  drawObject d1;
  d1.setYrange(-0.1, 1.2);
  d1.setLegend(0.2, 0.7, 0.5, 0.85);
  drawHistProfile("EpVsPtProf", d1);
  
  drawObject d2;
  d2.setYrange(-0.1, 1.2);
  d2.setLegend(0.2, 0.7, 0.5, 0.85);
  drawHistProfile("EpVsPtClusterProf", d2);

  drawObject d3;
  d3.setYrange(-0.1, 1.2);
  //d3.setLegend(0.46, 0.74, 0.76, 0.89);
  drawHistProfile("HitTowerFracClusterProf", d3);

  drawObject d4;
  d4.setYrange(-0.1, 1.2);
  //d4.setLegend(0.46, 0.74, 0.76, 0.89);
  drawHistProfile("MaxTowerFracClusterProf", d4);

  drawObject d5;
  //d5.setYrange(-0.1, 0.29);
  //d5.setLegend(0.46, 0.74, 0.76, 0.89);
  drawHistProfile("EpVsDistProf", d5);
/*
  drawObject d6;
  drawHist("NSigmaElectron", d6);
  
  drawObject d7;
  d7.setLegend(0.13, 0.74, 0.43, 0.89);
  drawHist("NSigmaPion", d7);
*/
/*
  drawObject d8;
  drawHist2D("EpVsPtScatter", d8);

  drawObject d9;
  drawHist2D("EpVsPtClusterScatter", d9);

  drawObject d10;
  drawHist2D("HitTowerFracClusterScatter", d10);

  drawObject d11;
  drawHist2D("MaxTowerFracClusterScatter", d11);
*/
  //*/  
  drawObject d12;
  //d12.setYrange(-0.001, 0.005);
  drawHistProjY("EpVsPtScatter", 11, 15, d12);

  drawObject d13;
  //d13.setYrange(-0.0005, 0.002);
  drawHistProjY("EpVsPtScatter", 16, 55, d13);

  drawObject d14;
  //d14.setYrange(-0.001, 0.005);
  drawHistProjY("EpVsPtScatter", 6, 10, d14);
  return 1;
}
void drawHist(const char *name, const drawObject &obj)
{
  TH1 *hp[Nn];
  for(int ih = 0; ih < Nn; ih++){
    hp[ih] =(TH1*)mFile->Get(Form("%s%s", type[ih], name));
    if(!hp[ih]) Printf(Form("%s%s not found", type[ih], name)); 
    hp[ih]->Print();
  }

  TCanvas *c = new TCanvas(Form("c%s", name), Form("c%s", name), 480, 360);
  for(int ih = 0; ih < Nn; ih++){
    if(ih == 0){
      hp[ih]->Draw();
      if(obj.ymax > obj.ymin) hp[ih]->GetYaxis()->SetRangeUser(obj.ymin, obj.ymax);
    }else{
      hp[ih]->Draw("same");
    }
    hp[ih]->SetLineColor(ih+2);
  }
  TLegend *lg = new TLegend(obj.lx1, obj.ly1, obj.lx2, obj.ly2);
  for(int ih = 0; ih < Nn; ih++){
    lg->AddEntry(hp[ih], type[ih], "l");
  }
  lg->Draw("same");
  gPad->SetLogy(obj.logy);
  c->Print(Form("%s%s.png", name, ver));
}
void drawHistProfile(const char *name, const drawObject &obj)
{
  TProfile *hp[Nn], *hpw2[Nn];
  TH1D *hh[Nn];
  double hmean[Nn], herror[Nn];
  for(int ih = 0; ih < Nn; ih++){
    hp[ih] =(TProfile*)mFile->Get(Form("%s%s", type[ih], name));
    hp[ih]->Print();
    hpw2[ih] =(TProfile*)mFile->Get(Form("%s%sW2", type[ih], name));
    hpw2[ih]->Print();
    hh[ih] = convertTProfile(hp[ih], hpw2[ih]);
    getStatTProfile(hp[ih], hpw2[ih], hmean[ih], herror[ih]);
    Printf("%d, hmean=%lf, herror=%lf\n", ih, hmean[ih], herror[ih]);
  }

  TCanvas *c = new TCanvas(Form("c%s", name), Form("c%s", name), 480, 360);
  for(int ih = 0; ih < Nn; ih++){
    if(ih == 0){
      hh[ih]->Draw();
      if(obj.ymax > obj.ymin) hh[ih]->GetYaxis()->SetRangeUser(obj.ymin, obj.ymax);
    }else{
      hh[ih]->Draw("same");
    }
    hh[ih]->SetLineColor(ih+2);
  }
  TLegend *lg = new TLegend(obj.lx1, obj.ly1, obj.lx2, obj.ly2);
  for(int ih = 0; ih < Nn; ih++){
    lg->AddEntry(hh[ih], Form("%s %3.2lf#pm%3.2lf", type[ih], hmean[ih], herror[ih]), "l");
  }
  lg->Draw("same");
  c->Print(Form("%s%s.png", name, ver));
}
TH1D *convertTProfile(TProfile *hw, TProfile *hw2)
{
  int N = hw->GetNbinsX();

  //TArrayD *bin = hw->GetXaxis()->GetXbins();
  //TH1F *hres = new TH1F(Form("%sHist", hw->GetName()), Form(";%s;%s", hw->GetXaxis()->GetTitle(), hw->GetYaxis()->GetTitle()), N, bin->GetArray());
  TAxis *xaxis = hw->GetXaxis();
  TAxis *yaxis = hw->GetYaxis();
  TH1D *hres = new TH1D(Form("%sHist", hw->GetName()), Form(";%s;%s", xaxis->GetTitle(), yaxis->GetTitle()), N, xaxis->GetXmin(), xaxis->GetXmax());
  
  for(int i = 1; i <= N; i++){
    double content = hw->GetBinContent(i);
    hres->SetBinContent(i, content);
    double entries = hw->GetBinEntries(i);
    double content2 = hw2->GetBinContent(i);
    double error2 = hw2->GetBinError(i);
    double entries2 = hw2->GetBinEntries(i);
    double entriesEff = TMath::Power(entries, 2);
    double newError = 0;
    
    if(entries2 > 0) entriesEff /= entries2;
    //1/N*((<aLL>-<aLL>^w2)^2+var(aLL)^w2); N = (sumW)^2/sumW2;
    double var2 = TMath::Power(error2, 2)*entries2;
    if(entriesEff > 0.) newError = (1./entriesEff)*(TMath::Power(content-content2, 2)+var2);
    newError = TMath::Sqrt(newError);
    hres->SetBinError(i, newError);
    
    cout<<Form("content = %lf entriesEff = %lf content2 = %lf error2 = %lf entries2 = %lf newError = %lf", content, entriesEff, content2, error2, entries2, newError)<<endl;

  }
  return hres;
}
void drawHist2D(const char *name, const drawObject &obj)
{
  TH2D *hp[Nn];
  for(int ih = 0; ih < Nn; ih++){
    hp[ih] =(TH2D*)mFile->Get(Form("%s%s", type[ih], name));
    hp[ih]->Print();
  }

  int ncx = 2, ncy = 2;
  TCanvas *c = new TCanvas(Form("c%s", name), Form("c%s", name), ncx*480, ncy*360);
  c->Divide(ncx,ncy);
  for(int ih = 0; ih < Nn; ih++){
    c->cd(ih+1);
    hp[ih]->Draw("colz");
    if(obj.ymax > obj.ymin) hp[ih]->GetYaxis()->SetRangeUser(obj.ymin, obj.ymax);
    gPad->SetLogz(1);
    //hp[ih]->GetZaxis()->SetRangeUser(9e-13, 2.0);
    if(obj.zmax > obj.zmin) hp[ih]->GetZaxis()->SetRangeUser(obj.zmin, obj.zmax);
  }
  
  c->Print(Form("%s%s.png", name, ver));
}
void drawHistProjY(const char *name, int xlow, int xhigh, const drawObject &obj)
{
  TH2D *hh[Nn];
  TH1D *hp[Nn];
  for(int ih = 0; ih < Nn; ih++){
    hh[ih] =(TH2D*)mFile->Get(Form("%s%s", type[ih], name));
    if(!hh[ih]) Printf("Histogram not found\n");    
    hp[ih] = (TH1D*) hh[ih]->ProjectionY(Form("%s%sProj%dto%d", type[ih], name, xlow, xhigh), xlow, xhigh);
    hp[ih]->Print();
  }

  TCanvas *c = new TCanvas(Form("c%sproj%dto%d", name, xlow, xhigh), Form("c%sproj%dto%d", name, xlow, xhigh), 480, 360);
  for(int ih = 0; ih < Nn; ih++){
    if(ih == 0){
      hp[ih]->Draw();
      if(obj.ymax > obj.ymin) hp[ih]->GetYaxis()->SetRangeUser(obj.ymin, obj.ymax);
    }else
      hp[ih]->Draw("same");
    hp[ih]->SetLineColor(ih+2);
  }

  double xmin = hh[0]->GetXaxis()->GetBinLowEdge(xlow);
  double xmax = hh[0]->GetXaxis()->GetBinLowEdge(xhigh+1);
  TLegend *lg = new TLegend(obj.lx1, obj.ly1, obj.lx2, obj.ly2);
  lg->SetHeader(Form("%.1lf < p_{T} < %.1lf GeV", xmin, xmax));
  for(int ih = 0; ih < Nn; ih++){
    //lg->AddEntry(hh[ih], Form("%s %3.2lf#pm%3.2lf", type[ih], hmean[ih], herror[ih]), "l");
    lg->AddEntry(hp[ih], Form("%s", type[ih]), "l");
  }
  lg->Draw("same");  

  c->Print(Form("%sproj%dto%d%s.png", name, xlow, xhigh, ver));
}
//
void getStatTProfile(TProfile *hw, TProfile *hw2, double &mean, double &error)
{
  int N = hw->GetNbinsX();
  
  double sum_content = 0, sum_content2 = 0;
  double sum_entries = 0, sum_entries2 = 0;
  double sum_var = 0, sum_var2 = 0;
  for(int i = 1; i <= N; i++){
    double entries = hw->GetBinEntries(i);
    sum_entries += entries;
    double content = hw->GetBinContent(i);
    sum_content += content*entries;

    double err = hw->GetBinError(i);
    double var = (TMath::Power(err, 2)*entries + TMath::Power(content, 2))*entries;
    sum_var += var;
    
    double entries2 = hw2->GetBinEntries(i);
    sum_entries2 += entries2;
    double content2 = hw2->GetBinContent(i);
    sum_content2 += content2*entries2;

    double err2 = hw2->GetBinError(i);
    double var2 = (TMath::Power(err2, 2)*entries2 + TMath::Power(content2, 2))*entries2;
    sum_var2 += var2;
    
    cout<<Form("content = %lf content2 = %lf err= %lf err2 = %lf entries = %lf entries2 = %lf var=%lf, var2 = %lf", content,content2, err, err2, entries, entries2, var, var2)<<endl;
    
  }
  Printf("sum_entries=%lf, sum_content=%lf, sum_var=%lf\n", sum_entries, sum_content, sum_var);
  if(sum_entries > 0 && sum_entries2 > 0){
    double sum_mean = sum_content/sum_entries;
    double sum_mean2 = sum_content2/sum_entries2;
    
    double sum_entriesEff = TMath::Power(sum_entries, 2);
    double sum_error = 0;
  
    //if(sum_entries2 > 0) 
    sum_entriesEff /= sum_entries2;
    //1/N*((<aLL>-<aLL>^w2)^2+var(aLL)^w2); N = (sumW)^2/sumW2;

    //if(sum_entriesEff > 0.) 
    //sum_error = (1./sum_entriesEff)*(TMath::Power(sum_mean-sum_mean2, 2)+sum_var2/sum_entries2 - TMath::Power(sum_mean2, 2));
    sum_error = sum_var/sum_entries - TMath::Power(sum_mean, 2);
    sum_error = TMath::Sqrt(sum_error);
    
    mean = sum_mean;
    error = sum_error;
  }else{
    mean = 0;
    error = 0;
  }
}
