#include "profileUtilities.h"

#include "TProfile.h"
#include "TH1D.h"
#include "TFile.h"
#include "TMath.h"

TH1D *convertTProfile(TFile *fa, const char *name, const char* ver)
{
  TProfile *hw = (TProfile*)fa->Get(Form("%s", name));
  //hw->Print();
  TProfile *hw2 = (TProfile*)fa->Get(Form("%sW2", name));
  //hw2->Print();
  return convertTProfile(hw, hw2, ver);

}
TH1D *convertTProfile(TProfile *hw, TProfile *hw2, const char* ver)
{
  int N = hw->GetNbinsX();

  //TArrayD *bin = hw->GetXaxis()->GetXbins();
  //TH1F *hres = new TH1F(Form("%sHist", hw->GetName()), Form(";%s;%s", hw->GetXaxis()->GetTitle(), hw->GetYaxis()->GetTitle()), N, bin->GetArray());
  TAxis *xaxis = hw->GetXaxis();
  TAxis *yaxis = hw->GetYaxis();
  TH1D *hres = new TH1D(Form("%s%sHist", hw->GetName(), ver), Form(";%s;%s", xaxis->GetTitle(), yaxis->GetTitle()), N, xaxis->GetXmin(), xaxis->GetXmax());

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

    //Printf("content = %lf entriesEff = %lf content2 = %lf error2 = %lf entries2 = %lf newError = %lf\n", content, entriesEff, content2, error2, entries2, newError);
  }
  return hres;
}
