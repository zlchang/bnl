#include "StMyPDF.h"

#include<iostream>
#include "LHAPDF/LHAPDF.h"

using namespace std;
using namespace LHAPDF;

StMyPDF::StMyPDF()
{
  mPdf = mkPDF(string("cteq6l1"), 0);
}
StMyPDF::StMyPDF(const char*name, int mem)
{
  mPdf = mkPDF(string(name), mem);
}
double StMyPDF::xfx(int fl, double x, double q2)
{
  double f = mPdf->xfxQ2(fl, x, q2);
  //cout<<"fl: "<<fl<<" x: "<<x<<" q2: "<<q2<<" f: "<<f<<endl;
  return f;
}
