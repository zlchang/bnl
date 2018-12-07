#ifndef FUNC_UTILITIES
#define FUNC_UTILITIES

#include "TRandom.h"

//TRandom yRndm;
float partonicWeight(float pt, float p0 = 1.987, float p1 = -0.677, float p2 = 0.265, float p3 = 0.759);
float trackSyst(float trkCalib = 0.01, float trkEff = 0.65, float hadScale = 1.16, float trkProj = 0.5, float npResp = 0.3, float npRespSyst = 0.09);
float towerSyst(float gainSyst = 0.038, float twrEff = 0.01);
float nonPhotonicSyst(float trkEff, float hadScale, float trkProj, float npResp, float npRespSyst);
#endif
