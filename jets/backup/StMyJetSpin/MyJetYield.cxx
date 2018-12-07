#include "MyJetYield.h"

ClassImp(MyJetYield);

void MyJetYield::Add(MyJetYield *jetyield, float w)
{
  ///*
  hjetptvtxprof->Add(jetyield->GetJetVertexProf(), w);
  hjetptrtprof->Add(jetyield->GetJetRtProf(), w);
  hjetpartonratioprof->Add(jetyield->GetJetPartonRatioProfile(), w);
  hjetparticleratioprof->Add(jetyield->GetJetParticleRatioProfile(), w);
  hjettwojet->Add(jetyield->GetJetTwoJet(), w);
  hjetdetector->Add(jetyield->GetJetDetectorJet(), w);
  //response
  hjetptpartonres->Add(jetyield->GetJetPartonResMC(), w);
  hjetptparticleres->Add(jetyield->GetJetParticleResMC(), w);
  //profile
  hjetptpartonprof->Add(jetyield->GetJetPartonProfMC(), w);
  hjetptparticleprof->Add(jetyield->GetJetParticleProfMC(), w);
  //hjetptparticlecrrres->Add(jetyield->GetJetParticleCrrResMC(), w);
  hjetparton->Add(jetyield->GetJetPartonJet(), w);
  hjetparticle->Add(jetyield->GetJetParticleJet(), w);
  hjetpartonT->Add(jetyield->GetJetPartonTwoJet(), w);
  hjetparticleT->Add(jetyield->GetJetParticleTwoJet(), w);
  //crr
  //hjetparticlecrr->Add(jetyield->GetJetParticleJet(), w);
  //hjetparticlecrrT->Add(jetyield->GetJetParticleTwoJetCrr(), w);
  for(int ih = 0; ih < NMod; ih++){
     //hpdfw[ih]->Add(jetyield->GetW(ih), w);
     hjetptetaw[ih]->Add(jetyield->GetJetDetectorW(ih), w);
     hjetptpartonresw[ih]->Add(jetyield->GetJetPartonResMCW(ih), w);
     hjetptparticleresw[ih]->Add(jetyield->GetJetParticleResMCW(ih), w); 
  }
  for(int ip = 0; ip < yNproc; ip++){
    hjetdet[ip]->Add(jetyield->GetJetDetectorProc(ip), w);
    hjetdetmparticle[ip]->Add(jetyield->GetJetDetMatchParticleProc(ip), w);
    hjetdetmparton[ip]->Add(jetyield->GetJetDetMatchPartonProc(ip), w);
    //
    hjetparticleproc[ip]->Add(jetyield->GetJetParticleProc(ip), w);
    hjetpartonproc[ip]->Add(jetyield->GetJetPartonProc(ip), w);
  }
  //*/
  hjetpartonptr->Add(jetyield->GetJetPartonScale(), w);
  hjetparticleptr->Add(jetyield->GetJetParticleScale(), w);
  hjetpartonnm->Add(jetyield->GetJetPartonNMatch(), w);
  hjetparticlenm->Add(jetyield->GetJetParticleNMatch(), w);
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
/*
int MyJetYield::indexDet(float pt, float eta)
{
  TAxis xdet(yNPtBins, yPtBins);
  int x1 = xdet.FindBin(pt);
  TAxis ydet(yNEtaBins, yEtaBins);
  int y1 = ydet.FindBin(eta);
  return x1+y1*(xdet.GetNbins()+2);
}
int MyJetYield::indexPar(float pt, float eta)
{
  TAxis xdet(yNParPtBins, yParPtBins);
  int x1 = xdet.FindBin(pt);
  TAxis ydet(yNEtaBins, yEtaBins);
  int y1 = ydet.FindBin(eta);
  return x1+y1*(xdet.GetNbins()+2);
}
*/
void MyJetYield::FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight){
  //
  /*
  TAxis *xdet = hjetdetector->GetXaxis();
  int x1 = xdet->FindBin(pt1);
  //if(x1 == 0) return;
  //if(x1 == xdet->GetNbins()+1) x1 = xdet->GetNbins();
  if(x1 == 0 || x1 == xdet->GetNbins()+1) return;
  TAxis *ydet = hjetdetector->GetYaxis();
  int y1 = ydet->FindBin(eta1);
  //if(y1 == 0) return;
  //if(y1 == ydet->GetNbins()+1) y1 = ydet->GetNbins();
  if(y1 == 0 || y1 == ydet->GetNbins()+1) return;
  
  int aa = (x1-1)+(y1-1)*xdet->GetNbins();
  //
  TAxis *xpar = hjetparton->GetXaxis();
  int x2 = xpar->FindBin(pt2);
  if(x2 == 0) return;
  if(x2 == xpar->GetNbins()+1) x2 = xpar->GetNbins();
  //if(x2 == 0 || x2 == xpar->GetNbins()+1) return;
  TAxis *ypar = hjetparton->GetYaxis();
  int y2 = ypar->FindBin(eta2);
  if(y2 == 0) return;
  if(y2 == ypar->GetNbins()+1) y2 = ypar->GetNbins();
  //if(y2 == 0 || y2 == ypar->GetNbins()+1) return;
      
  int bb = (x2-1)+(y2-1)*xpar->GetNbins();
  */  
  int aa = indexDet(pt1, eta1);
  int bb = indexPar(pt2, eta2);
  hjetptpartonres->Fill(aa, bb, weight);
  hjetparton->Fill(pt2, eta2, weight);
} 
void MyJetYield::FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf){
  int aa = indexDet(pt1, eta1);
  int bb = indexPar(pt2, eta2);
  hjetptpartonres->Fill(aa, bb, weight);
  hjetparton->Fill(pt2, eta2, weight);
  for(int iset = 0; iset < NMod; iset++){
    hjetptpartonresw[iset]->Fill(aa, bb, weight*wf[iset]);
  }
} 
void MyJetYield::FillPartonResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf, int proc){
  int aa = indexDet(pt1, eta1);
  int bb = indexPar(pt2, eta2);
  hjetptpartonres->Fill(aa, bb, weight);
  hjetparton->Fill(pt2, eta2, weight);
  for(int iset = 0; iset < NMod; iset++){
    hjetptpartonresw[iset]->Fill(aa, bb, weight*wf[iset]);
  }
  hjetdetmparton[proc]->Fill(aa, bb, weight);
} 
void MyJetYield::FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight){
  /*
  //
  TAxis *xdet = hjetdetector->GetXaxis();
  int x1 = xdet->FindBin(pt1);
  //if(x1 == 0) return;
  //if(x1 == xdet->GetNbins()+1) x1 = xdet->GetNbins();
  if(x1 == 0 || x1 == xdet->GetNbins()+1) return;
  TAxis *ydet = hjetdetector->GetYaxis();
  int y1 = ydet->FindBin(eta1);
  //if(y1 == 0) return;
  //if(y1 == ydet->GetNbins()+1) y1 = ydet->GetNbins();
  if(y1 == 0 || y1 == ydet->GetNbins()+1) return;
  
  int aa = (x1-1)+(y1-1)*xdet->GetNbins();
  //
  TAxis *xpar = hjetparticle->GetXaxis();
  int x2 = xpar->FindBin(pt2);
  if(x2 == 0) return;
  if(x2 == xpar->GetNbins()+1) x2 = xpar->GetNbins();
  //if(x2 == 0 || x2 == xpar->GetNbins()+1) return;
  TAxis *ypar = hjetparticle->GetYaxis();
  int y2 = ypar->FindBin(eta2);
  if(y2 == 0) return;
  if(y2 == ypar->GetNbins()+1) y2 = ypar->GetNbins();
  //if(y2 == 0 || y2 == ypar->GetNbins()+1) return;
      
  int bb = (x2-1)+(y2-1)*xpar->GetNbins();
  */
  int aa = indexDet(pt1, eta1);
  int bb = indexPar(pt2, eta2);  
  hjetptparticleres->Fill(aa, bb, weight);
  hjetparticle->Fill(pt2, eta2, weight);
}
void MyJetYield::FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf){  
  int aa = indexDet(pt1, eta1);
  int bb = indexPar(pt2, eta2);  

  hjetptparticleres->Fill(aa, bb, weight);
  hjetparticle->Fill(pt2, eta2, weight);
  for(int iset = 0; iset < NMod; iset++){
    hjetptparticleresw[iset]->Fill(aa, bb, weight*wf[iset]);
  }
}
void MyJetYield::FillParticleResponse(float pt1, float eta1, float pt2, float eta2, float weight, const float *wf, int proc){  
  int aa = indexDet(pt1, eta1);
  int bb = indexPar(pt2, eta2);  

  hjetptparticleres->Fill(aa, bb, weight);
  hjetparticle->Fill(pt2, eta2, weight);
  for(int iset = 0; iset < NMod; iset++){
    hjetptparticleresw[iset]->Fill(aa, bb, weight*wf[iset]);
  }
  hjetdetmparticle[proc]->Fill(aa, bb, weight);
}    
ClassImp(MyJetYieldPar);

void MyJetYieldPar::Add(MyJetYieldPar *par, float w)
{
  hpteta->Add(par->GetPtEta(), w);
  //hptetacrr->Add(par->GetPtEtaCrr(), w);
  for(int ih = 0; ih < NMod; ih++){
     //hpdfw[ih]->Add(par->GetW(ih), w);
     hptetaw[ih]->Add(par->GetPtEtaW(ih), w);
  }
  for(int ip = 0; ip < yNproc; ip++){
    hptetaproc[ip]->Add(par->GetPtEtaProc(ip), w);
  }
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

