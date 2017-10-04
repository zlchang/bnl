TFile *mFile;
const int Ntrg = 3;

const char *trg[] = {"JP0", "JP1", "JP2"};

TProfile *hetaRt[Ntrg];

int plotMC(const char*file = "ptbin.list.run12.i.v1.w.cmb.mc.root")
{
  gStyle->SetOptStat(0);
  gStyle->SetHistLineWidth(2);

  mFile = TFile::Open(file);

  for(int i = 0; i < Ntrg; i++){
    hetaRt[i] = (TProfile *)getHistogram(Form("%sJetRtVsEta", trg[i]));
  }
  
  TCanvas *c = new TCanvas("c", "c");
  hetaRt[0]->Draw();
  hetaRt[0]->GetYaxis()->SetRangeUser(0.2, 0.7);
  hetaRt[0]->SetLineColor(kRed);
  hetaRt[1]->Draw("same");
  hetaRt[1]->SetLineColor(kBlue);
  hetaRt[2]->Draw("same");
  hetaRt[2]->SetLineColor(kGreen);

  TLegend *lg = new TLegend(0.65, 0.3, 0.85, 0.45);
  for(int i = 0; i < Ntrg; i++){
    lg->AddEntry(hetaRt[i], trg[i], "l");
  }
  lg->Draw("same");
  
  return 1;
}
TH1* getHistogram(const char *name)
{
  TH1 *h = (TH1*) mFile->Get(name);
  if(!h) { Info("Read", Form("histogram %s not found", name));}
  return h;
}
