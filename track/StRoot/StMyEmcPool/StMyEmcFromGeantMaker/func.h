#ifndef MYFUNC
#define MYFUNC

#include "TMath.h"

double mSF[3] = {14.356, -0.512, 0.668};
double samplingFraction(double eta) {
  double x = TMath::Abs(eta);
  return 1./(mSF[0]+mSF[1]*x+mSF[2]*x*x);
}
#endif
