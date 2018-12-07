#ifndef MYJETSUM
#define MYJETSUM

#include "TObject.h"
class TH1F;
class TH2F;
class MyJetAsym;
class MyJetCrs;
class MyJetPtAsym;
class MyJetSpin;
class MyJetPtSpin;

class MyJetSum : public TObject{
public:

  MyJetSum():mAsym(0x0), mPol(0x0), mUnp(0x0), mPtAsym(0x0), mDptAsym(0x0), mMultAsym(0x0), mMultUeAsym(0x0){
    htwojetpt = 0x0;
    hjetpt = 0x0;
  }
  MyJetSum(const char* name);

  MyJetAsym *GetAsym() const{ return mAsym;}
  MyJetCrs *GetPol() const { return mPol; }
  MyJetCrs *GetUnp() const { return mUnp; }
  MyJetPtAsym *GetPtAsym() const { return mPtAsym; }
  MyJetPtAsym *GetDptAsym() const { return mDptAsym; }
  MyJetPtAsym *GetMultAsym() const { return mMultAsym; }
  MyJetPtAsym *GetMultUeAsym() const { return mMultUeAsym; }

  void FillJetSpin(MyJetSpin *jetspin, float rel[], float pb, float py, float scale = 1.0);
  void FillJetPtSpin(MyJetPtSpin *jetptspin, float pb = 0.55, float py = 0.55);
  void FillJetDPtSpin(MyJetPtSpin *jetptspin, float pb = 0.55, float py = 0.55);
  void FillAsym();
private:
  MyJetAsym *mAsym;
  MyJetCrs *mPol;
  MyJetCrs *mUnp;
  MyJetPtAsym *mPtAsym;
  MyJetPtAsym *mDptAsym;
  MyJetPtAsym *mMultAsym;
  MyJetPtAsym *mMultUeAsym;
  TH2F *htwojetpt;
  TH1F *hjetpt;
  ClassDef(MyJetSum, 1);
};
#endif
