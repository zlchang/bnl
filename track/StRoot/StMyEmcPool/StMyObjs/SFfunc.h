#ifndef MYSFFUNC
#define MYSFFUNC

#include "TMath.h"

double samplingFraction(double eta);
float getRadiusConstant();
float getRadiusLinear(float pt);
float linear(float x, const float &x1, const float &y1, const float &x2, const float &y2);

#endif
