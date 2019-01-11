#include "MyInclude.h"
#include "TMath.h"
#include "TVectorD.h"
#include "TMatrixD.h"


void MyCircle::findCircle(double &x0, double &y0, double &r)
{
  TMatrixD mm(4, 4);
  mm.Print();
  for(unsigned int ip = 0; ip < 3; ip++){
    TVectorD v(4);
    double x = mX[ip];
    double y = mY[ip];
    v[0] = TMath::Power(x, 2) + TMath::Power(y, 2);
    v[1] = x;
    v[2] = y;
    v[3] = 1;
    v.Print();
    TMatrixDRow(mm, ip+1) = v;
  }
  mm.Print();

  TVectorD det(4);
  for(unsigned int id = 0; id < 4; id++){
    TVectorD v1(4);
    v1.Zero();
    v1(id) = 1;
    TMatrixDRow(mm, 0) = v1;
    det[id] = mm.Determinant();
  }
  det.Print();

  if(det(0)){
    x0 = -1.*det(1)/det(0)/2;
    y0 = -1.*det(2)/det(0)/2;
    r = TMath::Power(x0,2) + TMath::Power(y0, 2) - det(3)/det(0);
    r = TMath::Sqrt(r);
  }
  double curv = 0;
  if(r > 0) curv = 1./r;
  Printf("x0=%lf, y0=%lf, r=%lf curv=%lf\n", x0, y0, r, curv);
}
