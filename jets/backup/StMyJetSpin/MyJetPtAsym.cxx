#include "MyJetPtAsym.h"
#include "MyJetSpin.h"
//#include "StMyJetPool/StMyDef/MyFun.h"

ClassImp(MyJetPtAsym);
ClassImp(MyJetPtSpin);

void MyJetPtAsym::FillJetPtAsym(MyJetPtSpin *jetptspin, float pb, float py)
{
  FillJetPtAsym(jetptspin->GetJetPtProfile(), pb, py);
}
void MyJetPtSpin::AddMeanPt(MyJetSpin *jetspin)
{
  TProfile *hprof_pt[4];
  for(int ispn = 0; ispn < 4; ispn++){
    int spin = SpinBit(ispn);
    hprof_pt[ispn] = jetspin->GetJetMeanPtSpin(spin);
    //    hprof_dpt[ispn] = jetspin->GetJetDPtSpin(spin);
  }
  AddProfiles(hprof_pt);
}
void MyJetPtSpin::AddDPt(MyJetSpin *jetspin)
{
  TProfile *hprof_dpt[4];
  for(int ispn = 0; ispn < 4; ispn++){
    int spin = SpinBit(ispn);
    hprof_dpt[ispn] = jetspin->GetJetDPtSpin(spin);
  }
  AddProfiles(hprof_dpt);
}
void MyJetPtSpin::AddMult(MyJetSpin *jetspin)
{
  TProfile *hprof_pt[4];
  for(int ispn = 0; ispn < 4; ispn++){
    int spin = SpinBit(ispn);
    hprof_pt[ispn] = jetspin->GetJetMultSpin(spin);
  }
  AddProfiles(hprof_pt);
}
void MyJetPtSpin::AddMultUe(MyJetSpin *jetspin)
{
  TProfile *hprof_pt[4];
  for(int ispn = 0; ispn < 4; ispn++){
    int spin = SpinBit(ispn);
    hprof_pt[ispn] = jetspin->GetJetMultUeSpin(spin);
  }
  AddProfiles(hprof_pt);
}
