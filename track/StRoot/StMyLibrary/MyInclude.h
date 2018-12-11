#ifndef MYLIBRARY
#define MYLIBRARY
#include<vector>
#include "TObject.h"
#include "TString.h"

using namespace std;

class MyCircle : public TObject
{
 public:
  MyCircle(){
    for(unsigned int ip = 0; ip < 3; ip++){
      mX[ip] = 0; mY[ip] = 0;
    }
  }
  void SetPoint(unsigned int ip, double x, double y){
    if(ip > 3) Printf("wrong index\n");
    mX[ip] = x; mY[ip] = y;
  }
  void findCircle(double &x0, double &y0, double &r);
 private:
  double mX[3];
  double mY[3];
  ClassDef(MyCircle, 0);
};
#endif
