double tesla = 1e-13;
double coulomb = 1.6e-19;

const int mNr = 3;
double mRadius[] = {225, 238, 255};

double mX[mNr];// = {0, 222, 234};
double mY[mNr];// = {0, 38, 42};

int testHelix(double pt = 1)
{
  gSystem->Load("StarClassLibrary");
  
  StThreeVectorD vertex(0, 0, 0);
  StThreeVectorD momemtum(pt, 0, 1);
  //double pt = momemtum.perp();
  //Printf("pt=%f\n", pt);
  double charge = -1;
  double b = 0.5;
  b *= tesla;
  /*
  StPhysicalHelixD helix_t(momemtum, vertex, b, charge); 

  double curv_t = helix_t.curvature();
  double factor = 0.3/curv_t;
  double dipA_t = helix_t.dipAngle();
  Printf("curv=%lf, dip=%lf factor=%e\n", curv_t, dipA_t, factor);
  */
  StPhysicalHelixD helix(momemtum, vertex, b, charge); 
  double curv = helix.curvature();
  double dipA = helix.dipAngle();
  Printf("curv=%lf, dip=%lf\n", curv, dipA);
  for(int ir = 0; ir < mNr; ir++){
    double rr = mRadius[ir];
    double xx, yy, zz;
    getPos(helix, rr, xx, yy, zz);
    mX[ir] = xx; mY[ir] = yy;
  }
  findCircle();
  /*
  StThreeVectorD rp = helix.at(radius);
  Printf("radius: x=%f y=%f z=%f\n", rp.x(), rp.y(), rp.z());

  StThreeVectorD rp2 = helix.at(radius/2);
  Printf("radius/2: x=%f y=%f z=%f\n", rp2.x(), rp2.y(), rp2.z());

  StThreeVectorD rp3 = helix.at(radius/3);
  Printf("radius/3: x=%f y=%f z=%f\n", rp3.x(), rp3.y(), rp3.z());
  */
  return 1;
}

void getPos(const StPhysicalHelix &hx, double radius, double &x, double &y, double &z)
{
  pairD pathLength = hx.pathLength(radius);
  double s1 = pathLength.first;
  double s2 = pathLength.second;
  Printf("s1=%f s2=%f\n", s1, s2);

  double ss = s1 > 0 ? s1 : s2;
  StThreeVectorD pos = hx.at(ss);
  Printf("x=%f y=%f z=%f\n", pos.x(), pos.y(), pos.z());
  x = pos.x();
  y = pos.y();
  z = pos.z();
}
void findCircle()
{
  TMatrixD mm(4, 4);
  mm.Print();
  for(int ip = 0; ip < mNr; ip++){
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
