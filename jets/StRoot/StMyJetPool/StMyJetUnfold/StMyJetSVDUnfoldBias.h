#ifndef MYJETSVDUNFOLDBIAS
#define MYJETSVDUNFOLDBIAS
#include "TObject.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TProfile.h"

class StMyJetSVDUnfold;
class StMyJetSVDUnfoldBias : public TObject
{
 public:
 StMyJetSVDUnfoldBias()
   //:mUnfold(0x0), Nrep(100){}
   :Nrep(100){}
  //StMyJetSVDUnfoldBias(StMyJetSVDUnfold *unfold): mUnfold(unfold), Nrep(100){}
  void getError(const TVectorD &x, const TMatrixD &rescovp, const TMatrixD &cc, double reg);
  //TMatrixD replica(const TMatrixD &res, const TMatrixD &resErr, int seed);
  TProfile *getProfile() const{ return hprof;}
  void setNrep(int nrep) { Nrep = nrep;}
 protected:
  TH1D **hx;
  TProfile *hprof;
 private:
  //StMyJetSVDUnfold *mUnfold;
  int Nrep;
  ClassDef(StMyJetSVDUnfoldBias, 0);
};
#endif
