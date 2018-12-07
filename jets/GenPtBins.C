const int nx = 21;

void GenPtBins()
{
  gen();
  gen(0, 6, 1.18);
}

void gen(double pt0 = 5, double pt1 = 6, double factor = 1.17)
{
  double ptbins[nx];
  ptbins[0] = pt0; ptbins[1] = pt1;
  for(int ip = 2; ip < nx; ip++){
    ptbins[ip] = ptbins[ip-1]*factor;
  }

  for(int ip = 0; ip < nx; ip++){
    Printf("%.2lf\n", ptbins[ip]);
  }  
}
