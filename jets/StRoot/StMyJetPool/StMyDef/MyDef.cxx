#include"MyDef.h"

#include "TAxis.h"

int indexDet(double pt, double eta)
{
  TAxis xdet(yNPtBins, yPtBins);
  int x1 = xdet.FindBin(pt);
  TAxis ydet(yNEtaBins, yEtaBins);
  int y1 = ydet.FindBin(eta);
  return x1+y1*(xdet.GetNbins()+2);
}
int indexPar(double pt, double eta)
{
  TAxis xdet(yNParPtBins, yParPtBins);
  int x1 = xdet.FindBin(pt);
  TAxis ydet(yNEtaBins, yEtaBins);
  int y1 = ydet.FindBin(eta);
  return x1+y1*(xdet.GetNbins()+2);
}
