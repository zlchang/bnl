#include "MyJetTest.h"

ClassImp(MyJetTest);

void MyJetTest::Add(MyJetTest *jettest, float w)
{
  hjetpt->Add(jettest->GetJetPt(), w);
  hjeteta->Add(jettest->GetJetEta(), w);
  hjetphi->Add(jettest->GetJetPhi(), w);
  hjettwo->Add(jettest->GetTwoJet(), w);
}
ClassImp(MyJetTestPar);

void MyJetTestPar::Add(MyJetTestPar *par, float w)
{
  hpt->Add(par->GetPt(), w);
  //hptetacrr->Add(par->GetPtEtaCrr(), w);
}
