#include "funcUtilities.h"
#include "TMath.h"

double partonicWeight(double pt, double p0, double p1, double p2, double p3)
{
  double wght = 0.;
  wght = 1./(1+(p0+p1*(pt-2)+p2*TMath::Power(pt-2, 2))*TMath::Exp(-p3*(pt-2)));
  return wght;
}
double nonPhotonicSyst(double trkEff, double hadScale, double trkProj, double npResp, double npRespSyst)
{
  return (1/trkEff*hadScale - trkProj)*npResp*npRespSyst;
}
double trackSyst(double trkCalib, double trkEff, double hadScale, double trkProj, double npResp, double npRespSyst)
{
  double npSyst = nonPhotonicSyst(trkEff, hadScale, trkProj, npResp, npRespSyst);
  double trkSyst = TMath::Sqrt(trkCalib*trkCalib+npSyst*npSyst);
  return trkSyst;
}
double towerSyst(double gainSyst, double twrEff)
{
  return TMath::Sqrt(gainSyst*gainSyst+twrEff*twrEff);
}

