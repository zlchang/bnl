#include "StMyRes.h"

ClassImp(StMyRes);
ClassImp(StMyRes2D);

void StMyRes2D::Add(StMyRes2D *jetres, float w)
{
  hjetmatchratioprof->Add(jetres->GetJetMatchRatio(), w);
  hjetres->Add(jetres->GetJetRes(), w);
  hjetptmeanprof->Add(jetres->GetJetPtMean(), w);
}
int StMyRes2D::XIndex(double pt, double eta){
  int ipt = xpt->FindBin(pt);
  int jeta = xeta->FindBin(eta);
  return ipt+jeta*(xpt->GetNbins()+2);
}
int StMyRes2D::YIndex(double pt, double eta){
  int ipt = ypt->FindBin(pt);
  int jeta = yeta->FindBin(eta);
  return ipt+jeta*(ypt->GetNbins()+2);
}
void StMyRes2D::FillResponse(float pt1, float eta1, float pt2, float eta2, float weight)
{
  int i1 = XIndex(pt1, eta1);
  int i2 = YIndex(pt2, eta2);

  hjetres->Fill(i1, i2, weight);
  hjetptmeanprof->Fill(i1, pt2-pt1, weight);
}
//StMyRes1D
void StMyRes1D::Add(StMyRes1D *jetres, float w)
{
  hjetmatchratioprof->Add(jetres->GetJetMatchRatio(), w);
  hjetres->Add(jetres->GetJetRes(), w);
  hjetptmeanprof->Add(jetres->GetJetPtMean(), w);

}
void StMyRes1D::FillResponse(float pt1, float pt2, float weight)
{
  hjetres->Fill(pt1, pt2, weight);
  hjetptmeanprof->Fill(pt1, pt2-pt1, weight);
}
