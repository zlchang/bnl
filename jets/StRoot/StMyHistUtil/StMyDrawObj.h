#ifndef STMYDRAWOBJ
#define STMYDRAWOBJ

#include "TObject.h"

class MyDrawObj : public TObject{
 public:
  MyDrawObj(){
    ymin = 0; ymax = -1;
    lx1 = 0.46; ly1 = 0.74; lx2 = 0.76; ly2 = 0.89;
    logy = false;
    //lx1 = 0.2; ly1 = 0.70; lx2 = 0.5; ly2 = 0.85;
  }
  MyDrawObj(double min, double max){
    ymin = min; ymax = max;
    lx1 = 0.46; ly1 = 0.74; lx2 = 0.76; ly2 = 0.89;
    logy = false;
    //lx1 = 0.2; ly1 = 0.70; lx2 = 0.5; ly2 = 0.85;
  }
  double ymin, ymax;
  double lx1,ly1,lx2,ly2;
  bool logy;
  void setLegend(double x1, double y1, double x2, double y2){ lx1 = x1; ly1 = y1; lx2 = x2; ly2 = y2;}
  void setYrange(double min, double max){ymin = min; ymax = max;}
  void setLogy(bool flag){logy = flag;}

  ClassDef(MyDrawObj, 0);
};

#endif
