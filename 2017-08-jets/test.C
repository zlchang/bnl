int test()
{
  int N = 15;
  double eps = 1e-3;
  TMatrixD C(N, N);
  C(0,0) = -1.;
  C(0,-1) = 1.;
  for(int i = 1; i < N-1; i++){
    C(i, i-1) = 1.;
    C(i,i) = -2.;
    C(i, i+1) = 1.;
  }
  C(N-1, N-2) = 1.;
  C(N-1, N-1) = -1.;
  for(int i = 0; i < N; i++){
     C(i,i) += eps;
  }
  TDecompSVD svd(C);

  TMatrixD u = svd.GetU();
  TMatrixD v = svd.GetV();

  TMatrixD vt(v);
  v.Print("all");
  vt.Transpose(vt);
  vt.Print("all");
  TMatrixD ss(N, N);
  TVectorD s = svd.GetSig();
  for(int i = 0; i < N; i++){
    ss(i,i) = s(i);
    cout<<i<<" "<<s(i)<<endl; 
  }
  TMatrixD tmp = (u*ss)*vt;
  tmp.Print("all");

  TMatrixD m(5,5);
  m(0,0) = 1; m(0,4) = 2;
  m(1,2) = 3; m(3,1) = 2;
  TDecompSVD svdm(m);
  TMatrixD um = svdm.GetU();
  TMatrixD vm = svdm.GetV();
  TMatrixD sm(5,5);
  TVectorD sigm = svdm.GetSig();
  for(int im = 0; im < 5; im++){
    sm(im, im) = sigm(im);
    cout<<im<<" "<<sigm(im)<<endl;
  }
  vm.Transpose(vm);
  TMatrixD tm = (um*sm)*vm;
  um.Print("all");
  vm.Print("all");
  tm.Print("all");

  return 1;
}
