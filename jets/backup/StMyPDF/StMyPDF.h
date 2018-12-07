#ifndef STMYPDF_H
#define STMYPDF_H

#include"TObject.h"

namespace LHAPDF
{
  class PDF;
}
class StMyPDF : public TObject
{
 public:
  StMyPDF();
  StMyPDF(const char* name, int mem);
  double xfx(int fl, double x, double q2);
 private:
  LHAPDF::PDF *mPdf;

  ClassDef(StMyPDF, 0);
};
#endif
