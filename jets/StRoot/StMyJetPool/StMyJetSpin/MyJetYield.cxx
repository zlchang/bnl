#include "MyJetYield.h"

ClassImp(MyJetYield);

void MyJetYield::Add(MyJetYield *jetyield, float w)
{
  ///*
  hjettwojet->Add(jetyield->GetJetTwoJet(), w);
  hjetdetector->Add(jetyield->GetJet(), w);
  hjetptrtprof->Add(jetyield->GetJetRtProf(), w);
  //hjetptrtprofw2->Add(jetyield->GetJetRtProfW2(), w*w);
}
void MyJetYield::FillJetTwoJet(float pt1, float eta1, float pt2, float eta2, float weight)
{
  /*
  TAxis *xdet = hjetdetector->GetXaxis();
  int x1 = xdet->FindBin(pt1);
  //if(x1 == 0) return;
  //if(x1 == xdet->GetNbins()+1) x1 = xdet->GetNbins();
  if(x1 == 0 || x1 == xdet->GetNbins()+1) return;
  TAxis *ydet = hjetdetector->GetYaxis();
  int y1 = ydet->FindBin(eta1);
  //if(y1 == ydet->GetNbins()+1) y1 = ydet->GetNbins();
  //if(y1 == 0) return;
  if(y1 == 0 || y1 == ydet->GetNbins()+1) return;
  int aa = (x1-1)+(y1-1)*xdet->GetNbins();
  
  int x2 = xdet->FindBin(pt2);
  //if(x2 == 0) return;
  //if(x2 == xdet->GetNbins()+1) x2 = xdet->GetNbins();
  if(x2 == 0 || x2 == xdet->GetNbins()+1) return;
  int y2 = ydet->FindBin(eta2);
  //if(y2 == ydet->GetNbins()+1) y2 = ydet->GetNbins();
  //if(y2 == 0) return;
  if(y2 == 0 || y2 == ydet->GetNbins()+1) return;
  
  int bb = (x2-1)+(y2-1)*xdet->GetNbins();
  */
  int aa = indexDet(pt1, eta1);
  int bb = indexDet(pt2, eta2);

  hjettwojet->Fill(aa, bb, weight);
  hjettwojet->Fill(bb, aa, weight);
}
void MyJetYield::FillJetPtEtaRt(float pt, float eta, float rt, float weight)
{
  if(eta < 0) eta *= -1;
  hjetdetector->Fill(pt, eta, weight);
  hjetptrtprof->Fill(pt, eta, rt, weight);
  //hjetptrtprofw2->Fill(pt, eta, rt, weight*weight);
}
ClassImp(MyJetYieldPar);

void MyJetYieldPar::Add(MyJetYieldPar *par, float w)
{
  hpteta->Add(par->GetPtEta(), w);
  //hptetacrr->Add(par->GetPtEtaCrr(), w);
  //for(int ih = 0; ih < NMod; ih++){
     //hpdfw[ih]->Add(par->GetW(ih), w);
     //hptetaw[ih]->Add(par->GetPtEtaW(ih), w);
  //}
  //for(int ip = 0; ip < yNproc; ip++){
  //hptetaproc[ip]->Add(par->GetPtEtaProc(ip), w);
  //}
  hjetptres->Add(par->GetPtRes(), w);
  //hpthat->Add(par->GetPtHat(), w);
  //for(int ih = 0; ih < NMod; ih++){
     //hpthatw[ih]->Add(par->GetPtHatW(), w);
  //}
}
void MyJetYieldPar::FillResponse(float pt1, float eta1, float pt2, float eta2, float weight){
  int aa = indexPar(pt1, eta1);
  int bb = indexPar(pt2, eta2);
  hjetptres->Fill(aa, bb, weight);
  //hjetparton->Fill(pt2, eta2, weight);
  //for(int iset = 0; iset < NMod; iset++){
    //hjetptpartonresw[iset]->Fill(aa, bb, weight*wf[iset]);
  //}
} 

