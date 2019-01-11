#include "SFfunc.h"

double samplingFraction(double eta) {
  double mSF[3] = {14.356, -0.512, 0.668};
  double x = TMath::Abs(eta);
  return 1./(mSF[0]+mSF[1]*x+mSF[2]*x*x);
}

float getRadiusConstant()
{
  float radius = 238.6;

  return radius;
}
float getRadiusLinear(float pt)
{
  float radius = 0;
  
  const float pt0 = 0.2;
  const float r0 = 225.4;

  const float pt1 = 0.5;
  const float r1 = 234.0;
  
  const float pt2 = 1.2;
  const float r2 = 238.6;
  
  if(pt < 0.2) radius = r0;
  else if(pt < 0.5){
    radius = linear(pt, pt0, r0, pt1, r1);
  }else if(pt < 1.2){
    radius = linear(pt, pt1, r1, pt2, r2);
  }else
    radius = r2;
  
  return radius;
}
float linear(float x, const float &x1, const float &y1, const float &x2, const float &y2)
{
  float y = 0;
  if(x2 > x1 || x2 < x1){
    y = (y2-y1)/(x2-x1)*(x-x1) + y1;
  }
  return y;
}
