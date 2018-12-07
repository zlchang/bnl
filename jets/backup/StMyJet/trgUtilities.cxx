#include "trgUtilities.h"
#include "StMyJetPool/StMyDef/funcUtilities.h"
#include"StSpinPool/StJetEvent/StJetEvent.h"
#include"StSpinPool/StJetEvent/StJetVertex.h"
#include"StSpinPool/StJetEvent/StJetCandidate.h"
#include"StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include "MyJet.h"
#include "TMath.h"
#include "TRandom.h"
#include "TH1F.h"

float smearPtFrac(float rt)
{
  float ftrk = trackSyst();
  float ftwr = towerSyst();
  //TRandom rndm;
  //float factor = gRandom->Uniform();
  //if(factor < 0.5) factor = 1.0;
  //else factor = -1.0;
  //Printf("random factor: %lf", factor);
  //return factor*TMath::Sqrt(TMath::Power(rt*ftwr, 2)+TMath::Power((1-rt)*ftrk, 2));
  return TMath::Sqrt(TMath::Power(rt*ftwr, 2)+TMath::Power((1-rt)*ftrk, 2));
}
bool check_badbx(int bx, vector<int> badbunch)
{
  for(vector<int>::iterator p = badbunch.begin(); p != badbunch.end(); p++)
    {
      if(bx == *p)
        return false;
    }
  return true;
}
bool check_spinbit(int spinbit)
{
  return spinbit == 5 || spinbit == 6 || spinbit == 9 || spinbit == 10;
}
StJetCandidate *FindParJet(StJetCandidate* jet, StJetEvent *evntpar, int &index, float range, int &counter)
{
  float eta = jet->eta();
  float phi = jet->phi();

  int NJets = evntpar->vertex()->numberOfJets();

  float dR = 99.0;
  index = -1;
  counter = 0;
  for(int ijet = 0; ijet < NJets; ijet++){
    StJetCandidate *jetpar = evntpar->vertex()->jet(ijet);
    float etapar = jetpar->eta();
    if(TMath::Abs(etapar) > range) continue;
    float phipar = jetpar->phi();

    float dRpar = DeltaR(eta, phi, etapar, phipar);

    if(dR > dRpar){
      dR = dRpar;
      index = ijet;
    }
    if(dRpar < 0.5) counter++;
    //    cout<<ijet<<" "<<dRpar<<" min: "<<dR<<" of "<<index<<"\n";                           
  }

  if(dR < 0.5) return evntpar->vertex()->jet(index);
  else { index = -1; return NULL;}
}
float DeltaR(float etaA, float phiA, float etaB, float phiB)
{
  const float PI = 3.1416;

  float deta = etaA - etaB;
  float dphi = phiA - phiB;

  if(dphi > PI) dphi -= 2.*PI;
  if(dphi < -1.*PI) dphi += 2.*PI;

  float dR = TMath::Sqrt(deta*deta+dphi*dphi);
  //  cout<<etaA<<" "<<phiA<<" "<<etaB<<" "<<phiB<<" "<<dR<<"\n";                          
  return dR;
}

int checkpt(float pt)
{
  const double ptbin[16] = {5.0, 6.0, 7.0, 8.2, 9.6, 11.2, 13.1, 15.3, 17.9, 20.9, 24.5, 28.7, 33.6, 39.3, 46.0, 53.8};
  for(int i = 0; i < 16; i++){
    if(pt  < ptbin[i]){
      return i;
    }
  }
  return 17;
}
string JetCategory(MyJet *myjet, StJetSkimEvent *skimevnt, map<string, int>mapTrigShouldFired, map<string, int>mapTrigDidFired, float flag, float smear, int th_plus)
{
  string trgname("Out");
  float pt = myjet->pt();
  if(flag > 0){
    float dpt = myjet->dpt()*flag;
    cout<<"candidate pt: "<<pt<<" dpt: "<<dpt<<" flag: "<<flag<<endl;
    if(checkpt(pt) - checkpt(pt-dpt) > 2) return trgname;
    else pt -= dpt;
  }
  float rt = myjet->rt();
  if(smear > 0){
    float spt = smearPtFrac(rt);
    cout<<"candidate pt: "<<pt<<" smear spt: "<<spt<<" smear: "<<smear<<endl;
    pt += smear*spt;
  }

  StJetCandidate *jetCnd = myjet->jet();
  float maxjp0 = 33.6;
  float maxjp1 = 39.3;
  if(pt > 15.3){
    if(mapTrigDidFired["JP2"] && (mapTrigShouldFired["JP2"] && MatchJetPatch(jetCnd, skimevnt, 2))){
      trgname = "JP2";
      cout<<"pt="<<pt<<" eventId="<<skimevnt->eventId()<<" Good JP2\n";
    }else if((mapTrigDidFired["JP1"] || mapTrigDidFired["JP0"]) && (mapTrigShouldFired["JP1"] && MatchJetPatch(jetCnd, skimevnt, 1)) && pt < maxjp1){
      trgname = "JP1";
      cout<<"pt="<<pt<<" eventId="<<skimevnt->eventId()<<" Good JP1\n";
    }else if(mapTrigDidFired["JP0"] && mapTrigShouldFired["JP0"] && MatchJetPatch(jetCnd, skimevnt, 0) && pt < maxjp0){
      trgname = "JP0";
      cout<<"pt="<<pt<<" eventId="<<skimevnt->eventId()<<" Good JP0\n";
    }
  }else if(pt > 8.2){
    if((mapTrigDidFired["JP1"] || mapTrigDidFired["JP0"]) && (mapTrigShouldFired["JP1"] && MatchJetPatch(jetCnd, skimevnt, 1)) && pt < maxjp1){
      trgname = "JP1";
      cout<<"pt="<<pt<<" eventId="<<skimevnt->eventId()<<" Good JP1\n";
    }else if(mapTrigDidFired["JP0"] && mapTrigShouldFired["JP0"] && MatchJetPatch(jetCnd, skimevnt, 0) && pt < maxjp0){
      trgname = "JP0";
      cout<<"pt="<<pt<<" eventId="<<skimevnt->eventId()<<" Good JP0\n";
    }
  }else if(pt > 6){
    if(mapTrigDidFired["JP0"] && mapTrigShouldFired["JP0"] && MatchJetPatch(jetCnd, skimevnt, 0)&& pt < maxjp0){
      trgname = "JP0";
      cout<<"pt="<<pt<<" eventId="<<skimevnt->eventId()<<" Good JP0\n";
    }
  }
  return trgname;
}
bool MatchJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  return MatchBarrelJetPatch(jetcnd, skim, th, shift) || MatchEndcapJetPatch(jetcnd, skim, th, shift) || MatchOverlapJetPatch(jetcnd, skim, th, shift);
}

bool MatchBarrelJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  //  bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi);

  int thres = skim->barrelJetPatchTh(th) + shift;
//  std::cout<<"JP"<<th<<" threshold "<<thres<<"\n";
  float deteta = jetcnd->detEta();
  float phi = jetcnd->phi();
//  std::cout<<"Jet det_eta = "<<deteta<<" phi = "<<phi<<"\n";
  
  for(int ipatch = 0; ipatch < 18; ipatch++)
    {
      float jpeta, jpphi;
      jetcnd->getBarrelJetPatchEtaPhi(ipatch, jpeta, jpphi);
//      std::cout<<"Barrel Jet Patch "<<ipatch<<": eta = "<<jpeta<<" phi = "<<jpphi<<" adc = "<<skim->barrelJetPatchAdc(ipatch)<<"\n";
      if(HitJetPatch(deteta, phi, jpeta, jpphi))
	{
	  int jp = ipatch;
	  if(skim->barrelJetPatchAdc(jp) > thres)
	    {
//	      std::cout<<"Barrel jet patch "<<jp<<" matched.\n";
	      return true;
	    }
	}
    }
  return false;
}
bool MatchEndcapJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  //  bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi);

  int thres = skim->endcapJetPatchTh(th) + shift;
//  std::cout<<"JP"<<th<<" threshold "<<thres<<"\n";
  float deteta = jetcnd->detEta();
  float phi = jetcnd->phi();
//  std::cout<<"Jet det_eta = "<<deteta<<" phi = "<<phi<<"\n";

  for(int ipatch = 0; ipatch < 6; ipatch++)
    {
      float jpeta, jpphi;
      jetcnd->getEndcapJetPatchEtaPhi(ipatch, jpeta, jpphi);
//      std::cout<<"Endcap Jet Patch "<<ipatch<<": eta = "<<jpeta<<" phi = "<<jpphi<<" adc = "<<skim->barrelJetPatchAdc(ipatch)<<"\n";
      if(HitJetPatch(deteta, phi, jpeta, jpphi))
	{
	  int jp = ipatch;
	  if(skim->endcapJetPatchAdc(jp) > thres)
	    {
//	      std::cout<<"Endcap jet patch "<<jp<<" matched.\n";
	      return true;
	    }
	}
    }
  return false;
}
bool MatchOverlapJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  //  bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi);

  int thres = skim->overlapJetPatchTh(th) + shift;
//  std::cout<<"JP"<<th<<" threshold "<<thres<<"\n";
  float deteta = jetcnd->detEta();
  float phi = jetcnd->phi();
//  std::cout<<"Jet det_eta = "<<deteta<<" phi = "<<phi<<"\n";
  
  for(int ipatch = 0; ipatch < 6; ipatch++)
    {
      float jpeta, jpphi;
      jetcnd->getOverlapJetPatchEtaPhi(ipatch, jpeta, jpphi);
//      std::cout<<"Overlap Jet Patch "<<ipatch<<": eta = "<<jpeta<<" phi = "<<jpphi<<" adc = "<<skim->barrelJetPatchAdc(ipatch)<<"\n";
      if(HitJetPatch(deteta, phi, jpeta, jpphi))
	{
	  int jp = ipatch;
	  if(skim->overlapJetPatchAdc(jp) > thres)
	    {
//	      std::cout<<"Overlap jet patch "<<jp<<" matched.\n";
	      return true;
	    }
	}
    }
  return false;
}
bool MatchAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  //  bool MatchBarrelAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th);
  //  bool MatchEndcapAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th);

  return MatchBarrelAdjacentJetPatch(jetcnd, skim, th, shift) || MatchEndcapAdjacentJetPatch(jetcnd, skim, th, shift);
}
bool MatchBarrelAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  //  bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi);
  //  void GetBarrelAdjacentJetPatchEtaPhi(StJetCandidate *jetcnd, int i, float &eta, float &phi);

  int thres = skim->barrelJetPatchTh(th) + shift;
//  std::cout<<"JP"<<th<<" threshold "<<thres<<"\n";
  float deteta = jetcnd->detEta();
  float phi = jetcnd->phi();
//  std::cout<<"Jet det_eta = "<<deteta<<" phi = "<<phi<<"\n";
  
  for(int ipatch = 0; ipatch < 18; ipatch++)
    {
      float jpeta, jpphi;
      GetBarrelAdjacentJetPatchEtaPhi(jetcnd, ipatch, jpeta, jpphi);
//      std::cout<<"Barrel Adjacent Jet Patch "<<ipatch<<": eta = "<<jpeta<<" phi = "<<jpphi<<"\n";
      if(HitJetPatch(deteta, phi, jpeta, jpphi))
	{
	  int jpx = ipatch;
	  int jpy = ipatch + 1;
	  if(jpy % 6 == 0) jpy =  jpy - 6;
//	  std::cout<<"jet patch "<<jpx<<": adc = "<<skim->barrelJetPatchAdc(jpx)<<" jet patch "<<jpy<<": adc = "<<skim->barrelJetPatchAdc(jpy)<<"\n";
	  if(skim->barrelJetPatchAdc(jpx) > thres && skim->barrelJetPatchAdc(jpy) > thres)
	    {
//	      std::cout<<"Barrel adjacent jet patch "<<jpx<<" and "<<jpy<<" matched.\n";
	      return true;
	    }
	}
    }
  return false;
}
bool MatchEndcapAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift)
{
  //  bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi);
  //  void GetEndcapAdjacentJetPatchEtaPhi(StJetCandidate *jetcnd, int i, float &eta, float &phi);

  int thres = skim->endcapJetPatchTh(th) + shift;
//  std::cout<<"JP"<<th<<" threshold "<<thres<<"\n";
  float deteta = jetcnd->detEta();
  float phi = jetcnd->phi();
//  std::cout<<"Jet det_eta = "<<deteta<<" phi = "<<phi<<"\n";

  for(int ipatch = 0; ipatch < 6; ipatch++)
    {
      float jpeta, jpphi;
      GetEndcapAdjacentJetPatchEtaPhi(jetcnd, ipatch, jpeta, jpphi);
//      std::cout<<"Endcap Adjacent Jet Patch "<<ipatch<<": eta = "<<jpeta<<" phi = "<<jpphi<<"\n";
      if(HitJetPatch(deteta, phi, jpeta, jpphi))
	{
	  int jpx = ipatch;
	  int jpy = ipatch + 1;
	  if(jpy % 6 == 0) jpy =  jpy - 6;
//	  std::cout<<"jet patch "<<jpx<<": adc = "<<skim->endcapJetPatchAdc(jpx)<<" jet patch "<<jpy<<": adc = "<<skim->endcapJetPatchAdc(jpy)<<"\n";
	  if(skim->endcapJetPatchAdc(jpx) > thres && skim->endcapJetPatchAdc(jpy) > thres)
	    {
//	      std::cout<<"Endcap adjacent jet patch "<<jpx<<" and "<<jpy<<" matched.\n";
	      return true;
	    }
	}
    }

  return false;
}
bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi)
{
  const float PI = 3.1416;

  float upeta = jpeta + 0.6;
  float dweta = jpeta - 0.6;

  float upphi = jpphi + 0.6;
  float dwphi = jpphi - 0.6;

  if((deteta < upeta && deteta > dweta) && ((phi < upphi && phi > dwphi) || (phi + 2*PI < upphi && phi + 2*PI > dwphi) || (phi - 2*PI < upphi && phi - 2*PI > dwphi)))
    {
//      std::cout<<"jet hits a patch\n";
      return true;
    }else
    {
      return false;
    }
  //  return (deteta < upeta && deteta > dweta) && ((phi < upphi && phi > dwphi) || (phi + 2*PI < upphi && phi + 2*PI > dwphi) || (phi - 2*PI < upphi && phi - 2*PI > dwphi));
}
void GetBarrelAdjacentJetPatchEtaPhi(StJetCandidate *jetcnd, int i, float &eta, float &phi)
{
  const float PI = 3.1416;
  
  int jpx =  i;
  int jpy = i + 1;
  if(jpy % 6 == 0) jpy = jpy - 6;

  float etax, phix;
  float etay, phiy;

  jetcnd->getBarrelJetPatchEtaPhi(jpx, etax, phix);
  jetcnd->getBarrelJetPatchEtaPhi(jpy, etay, phiy);
  
  eta = (etax + etay) / 2;
  phi = (phix + phiy) / 2;
  if(phi > phix) phi = phi - PI; 
}
void GetEndcapAdjacentJetPatchEtaPhi(StJetCandidate *jetcnd, int i, float &eta, float &phi)
{
  const float PI = 3.1416;
  
  int jpx =  i;
  int jpy = i + 1;
  if(jpy % 6 == 0) jpy = jpy - 6;

  float etax, phix;
  float etay, phiy;

  jetcnd->getEndcapJetPatchEtaPhi(jpx, etax, phix);
  jetcnd->getEndcapJetPatchEtaPhi(jpy, etay, phiy);
  
  eta = (etax + etay) / 2;
  phi = (phix + phiy) / 2;
  if(phi > phix) phi = phi - PI; 
}

vector<MyJet*> mergesort(vector<MyJet*> input, bool flag)
{
  int n = input.size();
  vector<MyJet*>::iterator pbgn = input.begin();

  if(n <= 1){ 
    return input;
//    if(n == 1) std::cout<<*pbgn<<"\n";

  }//else if(n > 1){
  vector<MyJet*> half1(pbgn, pbgn + n/2);
  //std::cout<<"1 half "<<*pbgn<<" "<<*(pbgn + n/2 - 1)<<" size "<<half1.size()<<"\n";
  vector<MyJet*> half2(pbgn + n/2, pbgn + n);
  //std::cout<<"2 half "<<*(pbgn + n/2)<<" "<<*(pbgn + n - 1)<<" size "<<half2.size()<<"\n";
  return merge(mergesort(half1, flag), mergesort(half2, flag), flag);
  //}
}

vector<MyJet*> merge(vector<MyJet*> a, vector<MyJet*> b, bool flag)
{
  if(a.size() == 0){
    return b;
  }else if(b.size() == 0){
    return a;
  }else{
    float pta = a[0]->pt();
    float ptb = b[0]->pt();
    if(flag == 1){
      float dpta = a[0]->dpt();   
      pta -= dpta;
      float dptb = b[0]->dpt();   
      ptb -= dptb;
    }
    //cout<<pta<<" "<<ptb<<" flag:"<<flag<<endl;

   vector<MyJet*> c;
   if(pta >= ptb){
      c.insert(c.begin(), a[0]);
      vector<MyJet*> tmpa(a.begin()+1, a.end());
      vector<MyJet*> tmpc(merge(tmpa, b, flag));
      c.insert(c.begin()+1, tmpc.begin(), tmpc.end());
    }else{
      c.insert(c.begin(), b[0]);
      vector<MyJet*> tmpb(b.begin()+1, b.end());
      vector<MyJet*> tmpc(merge(a, tmpb, flag));
      c.insert(c.begin()+1, tmpc.begin(), tmpc.end());
    }
    return c;
  }
}
