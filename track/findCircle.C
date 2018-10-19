//reference
//https://math.stackexchange.com/questions/213658/get-the-equation-of-a-circle-when-given-3-points

const int mN = 3;
double mX[] = {0, 222, 234};
double mY[] = {0, 38, 42};
void findCircle()
{
  TMatrixD mm(4, 4);
  mm.Print();
  for(int ip = 0; ip < mN; ip++){
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
  for(int id = 0; id < 4; id++){
    TVectorD v1(4);
    v1.Zero();
    v1(id) = 1;
    TMatrixDRow(mm, 0) = v1;
    det[id] = mm.Determinant();
  }
  det.Print();

  double x0 = 0, y0 = 0, r = 0;
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

