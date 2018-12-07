#include"MyJetAsym.h"

ClassImp(MyJetAsym);
void MyJetAsym::FillAsymmetry(MyJetCrs *jetpol, MyJetCrs *jetunp)
{
  FillJetAsym(jetpol,jetunp);
  FillJetBlueAsym(jetpol,jetunp);
  FillJetYellAsym(jetpol,jetunp);
  FillJetLikeSignAsym(jetpol,jetunp);
  FillJetUnlikeSignAsym(jetpol,jetunp);

  FillJetAsymError(jetpol,jetunp);
  FillJetBlueAsymError(jetpol,jetunp);
  FillJetYellAsymError(jetpol,jetunp);
  FillJetLikeSignAsymError(jetpol,jetunp);
  FillJetUnlikeSignAsymError(jetpol,jetunp);

}
