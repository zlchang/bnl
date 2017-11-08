#include "funcUtilities.h"
#include "TMath.h"

float partonicWeight(float pt, float p0, float p1, float p2, float p3)
{
  float wght = 0.;
  wght = 1./(1+(p0+p1*(pt-2)+p2*TMath::Power(pt-2, 2))*TMath::Exp(-p3*(pt-2)));
  return wght;
}
