double samplingFraction(double *x, double *par){
  double xx = TMath::Abs(x[0]);
  return par[0]+par[1]*xx+par[2]*xx*xx;
}
void testSampling()
{
	const double sf[] = {14.356, -0.512, 0.668};
	TF1 *f1 = new TF1("f1", samplingFraction,-2, 2, 3);
        f1->SetParameters(sf);
        f1->GetYaxis()->SetRangeUser(10, 18);
	f1->Draw();
}
