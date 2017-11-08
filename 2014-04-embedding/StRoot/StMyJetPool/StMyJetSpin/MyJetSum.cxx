#include "TH2F.h"
#include "MyJetSum.h"
#include "MyJetSpin.h"
#include "MyJetPtAsym.h"
#include "MyJetAsym.h"
#include "MyJetCrs.h"

ClassImp(MyJetSum);
MyJetSum::MyJetSum(const char *name){
  mAsym = new MyJetAsym(Form("%sAsym", name));
  mPol = new MyJetCrs(Form("%sPol", name));
  mUnp = new MyJetCrs(Form("%sUnp", name));
  mPtAsym = new MyJetPtAsym(Form("%sPtAsym", name));
  mDptAsym = new MyJetPtAsym(Form("%sPtUeAsym", name));
  mMultAsym = new MyJetPtAsym(Form("%sMultAsym", name));
  mMultUeAsym = new MyJetPtAsym(Form("%sMultUeAsym", name));
  htwojetpt = new TH2F(Form("%sTwoJetPt", name), ";p_{T} [GeV];p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
  hjetpt = new TH1F(Form("%sJetPt", name), ";p_{T} [GeV]", yNPtBins, yPtBins);

}

void MyJetSum::FillJetSpin(MyJetSpin *jetspin, float rel[], float pb, float py, float scale)
{
  mPol->FillPolXection(jetspin, rel, pb, py, scale);
  mUnp->FillUnpXection(jetspin, rel, pb, py, scale);
  hjetpt->Add(jetspin->GetJetSpin(5));
  hjetpt->Add(jetspin->GetJetSpin(6));
  hjetpt->Add(jetspin->GetJetSpin(9));
  hjetpt->Add(jetspin->GetJetSpin(10));
  //
  htwojetpt->Add(jetspin->GetJetTwoSpin(5));
  htwojetpt->Add(jetspin->GetJetTwoSpin(6));
  htwojetpt->Add(jetspin->GetJetTwoSpin(9));
  htwojetpt->Add(jetspin->GetJetTwoSpin(10));
}
void MyJetSum::FillJetPtSpin(MyJetPtSpin *jetptspin, float pb, float py)
{
  mPtAsym->FillJetPtAsym(jetptspin, pb, py);
}
void MyJetSum::FillJetDPtSpin(MyJetPtSpin *jetptspin, float pb, float py)
{
  mDptAsym->FillJetPtAsym(jetptspin, pb, py);
}
void MyJetSum::FillAsym()
{
  mAsym->FillAsymmetry(mPol,mUnp);
}
