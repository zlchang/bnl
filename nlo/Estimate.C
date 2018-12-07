const char * file = "UNPOL-JET-500.RES";
const double factor = 0.03;
TGraph *gr;

double constant(double x, double par){
  return par;
}
double root(double x, double par){
  return 3/TMath::Sqrt(x)*par;
}

int Estimate()
{
  gStyle->SetOptStat(0);
  gStyle->SetPadGridX(false);
  gr = new TGraph(file, "%lg %lg");

  gr->Draw("al");
  gPad->SetLogy();

  smear("const", constant);
  smear("root", root);
  return 1;
}

void smear(const char * name, double (*f)(double, double))
{
  int N  = gr->GetN();
  TGraph *grup = new TGraph(N-1);
  grup->SetName(Form("gr%sup", name));
  TGraph *grdn = new TGraph(N-1);
  grdn->SetName(Form("gr%sdn", name));

  for(int ip = 0; ip < N; ip++){
    double pt, crs;
    gr->GetPoint(ip, pt, crs);
    Printf("pt=%lf crs=%lf", pt, crs);    

    //double frac = 1/TMath::Sqrt(pt)*factor;
    double frac = f(pt, factor);//1/TMath::Sqrt(pt)*factor;

    Printf("frac=%lf", frac);
    if(ip < N-1){
      double ptup = pt*(1+frac);
      double crsup = gr->Eval(ptup, 0, "S");
      double rup = (crsup - crs)/crs;
      grup->SetPoint(ip, pt, rup);
      Printf("ptup=%lf crs=%lf", ptup, crsup);    
    }
    if(ip > 0){
      double ptdn = pt*(1-frac);
      double crsdn = gr->Eval(ptdn, 0, "S");
      double rdn = (crsdn - crs)/crs;
      grdn->SetPoint(ip-1, pt, rdn);
      Printf("ptdn=%lf crs=%lf", ptdn, crsdn);    
    }

  }

  TCanvas *c = new TCanvas(Form("c%s", name), Form("c%s", name), 480, 360);
  grup->Draw("al");
  grup->SetMarkerStyle(1);
  grdn->SetLineColor(kBlue);
  grup->GetYaxis()->SetLimits(-1, 1);
  grup->GetYaxis()->SetRangeUser(-1, 1);
  grdn->Draw("lsame");
  grdn->SetMarkerStyle(1);
  grdn->SetLineColor(kRed);
  
}
