#ifndef FUNC_UTILITIES
#define FUNC_UTILITIES

#include "TRandom.h"

//TRandom yRndm;
double partonicWeight(double pt, double p0 = 1.987, double p1 = -0.677, double p2 = 0.265, double p3 = 0.759);
double trackSyst(double trkCalib = 0.01, double trkEff = 0.65, double hadScale = 1.16, double trkProj = 0.5, double npResp = 0.3, double npRespSyst = 0.09);
double towerSyst(double gainSyst = 0.038, double twrEff = 0.01);
double nonPhotonicSyst(double trkEff, double hadScale, double trkProj, double npResp, double npRespSyst);
#endif
