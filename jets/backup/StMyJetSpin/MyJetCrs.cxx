#include"MyJetCrs.h"

ClassImp(MyJetCrs);
void MyJetCrs::FillPolXection(MyJetSpin *jetspin, float rel[], float pb, float py, float scale)
{
  FillJetXsection(jetspin, -1.0, rel[2], pb, py, scale);
  FillJetXsectionError(jetspin, 1.0, rel[2]*rel[2], pb*pb, py*py, scale*scale);

  FillJetBlueXsection(jetspin, -1.0, rel[1], pb, scale);
  FillJetBlueXsectionError(jetspin, 1.0, rel[1]*rel[1], pb*pb, scale*scale);

  FillJetYellXsection(jetspin, -1.0, rel[0], py, scale);
  FillJetYellXsectionError(jetspin, 1.0, rel[0]*rel[0], py*py, scale*scale);

  FillJetLikeSignXsection(jetspin, -1.0, rel[3], pb, py, scale);
  FillJetLikeSignXsectionError(jetspin, 1.0, rel[3]*rel[3], pb*pb, py*py, scale*scale);

  FillJetUnlikeSignXsection(jetspin, -1.0, rel[4], rel[5], pb, py, scale);
  FillJetUnlikeSignXsectionError(jetspin, 1.0, rel[4]*rel[4], rel[5]*rel[5], pb*pb, py*py, scale*scale);
}
void MyJetCrs::FillUnpXection(MyJetSpin *jetspin, float rel[], float pb, float py, float scale)
{
  FillJetXsection(jetspin, 1.0, rel[2], pb*pb, py*py, scale);
  FillJetBlueXsection(jetspin, 1.0, rel[1], pb*pb, scale);
  FillJetYellXsection(jetspin, 1.0, rel[0], py*py, scale);
  FillJetLikeSignXsection(jetspin, 1.0, rel[3], pb*pb, py*py, scale);
  FillJetUnlikeSignXsection(jetspin, 1.0, rel[4], rel[5], pb*pb, py*py, scale);
}

