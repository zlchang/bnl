#ifndef DRAWOBJECT
#define DRAWOBJECT
#include "TObject.h"

class drawObject : public TObject
{
 public:
  drawObject(){
    ymin = 0; ymax = -1;
    lx1 = 0.46; ly1 = 0.74; lx2 = 0.76; ly2 = 0.89;
    logy = false;
    //lx1 = 0.2; ly1 = 0.70; lx2 = 0.5; ly2 = 0.85;
  }
  void setLegend(double x1, double y1, double x2, double y2){ lx1 = x1; ly1 = y1; lx2 = x2; ly2 = y2;}
  void setYrange(double min, double max){ymin = min; ymax = max;}
  void setZrange(double min, double max){zmin = min; zmax = max;}
  void setLogy(bool flag){logy = flag;}

  double ymin, ymax;
  double zmin, zmax;
  double lx1,ly1,lx2,ly2;
  bool logy;
  
 private:
  ClassDef(drawObject, 0);
};
#endif
