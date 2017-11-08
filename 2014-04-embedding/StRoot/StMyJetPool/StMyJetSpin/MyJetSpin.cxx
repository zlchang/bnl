#include"MyJetSpin.h"

ClassImp(MyJetSpin);

void MyJetSpin::AddJetSpin(MyJetSpin *jetspin, float w)
{
  for(int i = 0; i < 4; i++){
    hjetpt[i] = (jetspin->GetJetSpin())[i];
    //    hjetonept[i] = (jetspin->GetJetOneSpin())[i];
    hjettwopt[i] = (jetspin->GetJetTwoSpin())[i];

    hjetsigpt[i] = (jetspin->GetJetSigSpin())[i];
    hjetmeanpt[i] = (jetspin->GetJetMeanPtSpin())[i];
    hjetdpt[i] = (jetspin->GetJetDPtSpin())[i];
  }
}
void MyJetSpin::fill_pt(int spinbit, float pt)
{
  //  FillJetOnePt(spinbit, pt);
  FillJetPt(spinbit, pt);
}
void MyJetSpin::fill_twojet_pt(int spinbit, float pta, float ptb)
{
  FillJetTwoPt(spinbit,pta,ptb);
  FillJetTwoPt(spinbit,ptb,pta);
  //  FillJetPt(spinbit,pta);
  //  FillJetPt(spinbit,ptb);
}
void MyJetSpin::fill_error()
{
  FillJetErrorPt(5);
  FillJetErrorPt(6);
  FillJetErrorPt(9);
  FillJetErrorPt(10);
}
void MyJetSpin::fill_dpt(int spinbit, float pt, float dpt)
{
  FillJetMeanPt(spinbit, pt, pt);
  FillJetDPt(spinbit, pt, dpt);
}
