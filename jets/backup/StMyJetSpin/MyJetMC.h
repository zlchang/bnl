#ifndef ROOT_MyJetMC
#define ROOT_MyJetMC


#include"TH1.h"
#include"TH1F.h"
#include"TH2.h"
#include"TH2F.h"
#include"TProfile.h"
#include"TObject.h"
#include"TMath.h"
#include"StMyJetPool/StMyDef/MyDef.h"

class MyJetMC : public TObject
{
 public:

  static const int NMod = 101;

  MyJetMC(){}
  MyJetMC(const char* name)
    {
      if(name)
	{
	  TH1::StatOverflows(kTRUE);
          TH1::SetDefaultSumw2(kTRUE);
	  
	  //x, xg
	  const int NX = 150;
	  double xx[NX+1];
	  for(int ibin = 0; ibin <= NX; ibin++){
	    xx[ibin] = TMath::Power(10, -3+3*(ibin+0.)/(NX+0.));
	  }
	  hxgparton = new TH2F(Form("%sPartonXg", name), ";x_{g}", NX, xx, yNPtBins, yPtBins);
	  hxparton = new TH2F(Form("%sPartonX", name), ";x", NX, xx, yNPtBins, yPtBins);
	  //
	  hxgparticle = new TH2F(Form("%sParticleXg", name), ";x_{g}", NX, xx, yNPtBins, yPtBins);
	  hxparticle = new TH2F(Form("%sParticleX", name), ";x", NX, xx, yNPtBins, yPtBins);
	  //process
	  hproc = new TH2F(Form("%sProcessFlavor", name), ";fl_{1}; fl_{2}", 13, -6, 7, 13, -6, 7);
          //two jet pt
	  hjetpt = new TH1F(Form("%sJetPtMc", name), ";p_{T} [GeV]",yNPtBins,yPtBins);
          hjetptnovtx = new TH1F(Form("%sJetPtNoVtxMc",name), ";p_{T} [GeV]", yNPtBins, yPtBins);
          hjetptnovtxprof = new TProfile(Form("%sJetPtNoVtxMcProf",name), ";p_{T} [GeV]", yNPtBins, yPtBins);
	  htwojetpt = new TH2F(Form("%sTwoJetPtMc", name), ";p_{T} [GeV]; p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
	  //2d
	  hjetptparton = new TH2F(Form("%sJetPtVsPartonJetPt", name), ";jet p_{T} [GeV];parton jet p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
	  hjetptparticle = new TH2F(Form("%sJetPtVsParticleJetPt", name), ";jet p_{T} [GeV];particle jet p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
	  hjetptparticlecrr = new TH2F(Form("%sJetPtVsParticleJetPtCrr", name), ";jet p_{T} [GeV];particle jet p_{T,crr} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
	  //pt shift dpt
	  hdptparton = new TProfile(Form("%sJetVsPartonDiffPtProf", name), ";jet p_{T} [GeV];parton jet p_{T} - jet p_{T} [GeV]", yNPtBins, yPtBins);
	  hdptparticle = new TProfile(Form("%sJetVsParticleDiffPtProf", name), ";jet p_{T} [GeV];particle jet p_{T} - jet p_{T} [GeV]", yNPtBins, yPtBins);
	  hdptparticlecrr = new TProfile(Form("%sJetVsParticleDiffPtCrrProf", name), ";jet p_{T} [GeV];particle jet p_{T,crr} - jet p_{T} [GeV]", yNPtBins, yPtBins);
	  //w2
	  hdptpartonw2 = new TProfile(Form("%sJetVsPartonDiffPtW2Prof", name), ";jet p_{T} [GeV];parton jet p_{T} - jet p_{T} [GeV]", yNPtBins, yPtBins);
	  hdptparticlew2 = new TProfile(Form("%sJetVsParticleDiffPtW2Prof", name), ";jet p_{T} [GeV];particle jet p_{T} - jet p_{T} [GeV]", yNPtBins, yPtBins);
	  hdptparticlecrrw2 = new TProfile(Form("%sJetVsParticleDiffPtCrrW2Prof", name), ";jet p_{T} [GeV];particle jet p_{T,crr} - jet p_{T} [GeV]", yNPtBins, yPtBins);
	  //not matching
	  hjetpartonN = new TH1F(Form("%sJetPartonNoMatch", name), ";p_{T} [GeV]", yNPtBins, yPtBins);
	  hjetparticleN = new TH1F(Form("%sJetParticleNoMatch", name), ";p_{T} [GeV]", yNPtBins, yPtBins);
	  hjetpartonNprof = new TProfile(Form("%sJetPartonNoMatchProf", name), ";p_{T} [GeV]", yNPtBins, yPtBins);
	  hjetparticleNprof = new TProfile(Form("%sJetParticleNoMatchProf", name), ";p_{T} [GeV]", yNPtBins, yPtBins);
	  //mean pt
	  hptdetector = new TProfile(Form("%s_pT_detector", name), "; p_{T} [GeV]; p_{T} [GeV]", yNPtBins, yPtBins);
	  //w2
	  hptdetectorw2 = new TProfile(Form("%s_pT_detector_w2", name), "; p_{T} [GeV]; p_{T} [GeV]", yNPtBins, yPtBins);	  
	  // dpt
	  hjetdpt = new TProfile(Form("%sJetDPt", name), ";p_{T} [GeV]; p_{T} [GeV]", yNPtBins, yPtBins);
	  hjetdpt2d = new TH2F(Form("%sJetPtVsPtCrr", name), ";p_{T} [GeV]; p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
	  hjetdptw2 = new TProfile(Form("%sJetDPtW2", name), ";p_{T} [GeV]; p_{T} [GeV]", yNPtBins, yPtBins);
	  for(int imod = 0; imod < NMod; imod++){
	    char title[128];
	    char hname[128];
	    char htitle[128];
	    sprintf(title, "%s_NNPDF_iset_%d", name, imod);
	    sprintf(hname, "aLL_detector_%s", title); 
	    sprintf(htitle, "aLL_detector_%s;p_{T} [GeV]", title); 
	    haLLdetector[imod] = new TProfile(hname, htitle, yNPtBins, yPtBins);
	    haLLdetector2d[imod] = new TH2F(Form("%s_2d", hname), htitle, yNPtBins, yPtBins, 200, -1, 1);
	    //w^2
	    sprintf(hname, "aLL_w2_detector_%s", title); 
	    sprintf(htitle, "aLL_w2_detector_%s;p_{T} [GeV]", title); 
	    haLLdetectorw2[imod] = new TProfile(hname, htitle, yNPtBins, yPtBins);
	  }
	}
    }
  
  ~MyJetMC()
    {
      if(hjetpt != NULL) delete hjetpt;
      if(hjetptnovtx != NULL) delete hjetptnovtx;
      if(hjetptnovtxprof != NULL) delete hjetptnovtxprof;
      if(htwojetpt != NULL) delete htwojetpt;
      //x, xg
      if(hxparton != NULL) delete hxparton;
      if(hxgparticle != NULL) delete hxgparticle;
      if(hxparticle != NULL) delete hxparticle;
      if(hxgparticle != NULL) delete hxgparticle;
      //pt shift
      if(hdptparton != NULL) delete hdptparton;
      if(hdptparticle != NULL) delete hdptparticle;
      if(hdptparticlecrr != NULL) delete hdptparticlecrr;
      //2d
      if(!hjetptparton) delete hjetptparton;
      if(!hjetptparticle) delete hjetptparticle;
      if(!hjetptparticlecrr) delete hjetptparticlecrr;
      //w^2
      if(hdptpartonw2 != NULL) delete hdptpartonw2;
      if(hdptparticlew2 != NULL) delete hdptparticlew2;
      if(hdptparticlecrrw2 != NULL) delete hdptparticlecrrw2;
      //mean pt
      if(hptdetector != NULL) delete hptdetector;
      //w^2
      if(hptdetectorw2 != NULL) delete hptdetectorw2;
      //dpt
      if(hjetdpt != NULL) delete hjetdpt;
      //w^2
      if(hjetdptw2 != NULL) delete hjetdptw2;
      delete hproc;
      delete hjetdpt2d;
      for(int imod = 0; imod < NMod; imod++){
	if(haLLdetector[imod] != NULL) delete haLLdetector[imod];
         delete haLLdetector2d[imod];
	//w^2
	if(haLLdetectorw2[imod] != NULL) delete haLLdetectorw2[imod];
      }
    }
  
  TH1F* GetJetMC() const{
    return hjetpt;
  }	
  TH2F *GetTwoJetMC() const{
    return htwojetpt;
  }
  TH1F *GetJetPtNoVtx() const{
    return hjetptnovtx;
  }
  TProfile *GetJetPtNoVtxProf() const{
    return hjetptnovtxprof;
  }

  //x, xg
  TH2F *GetXParton() const{
    return hxparton;
  }
  TH2F *GetXgParton() const{
    return hxgparton;
  }
  TH2F *GetXParticle() const{
    return hxparticle;
  }
  TH2F *GetXgParticle() const{
    return hxgparticle;
  }
  TH2F* GetJetPartonMC() const{
    return hjetptparton;
  }	
  TH2F* GetJetParticleMC() const{
    return hjetptparticle;
  }
  TH2F* GetJetParticleCrrMC() const{
    return hjetptparticlecrr;
  }
  TH1F *GetJetPartonNoMatch() const{
    return hjetpartonN;
  }
  TH1F *GetJetParticleNoMatch() const{
    return hjetparticleN;
  }
  TProfile *GetJetPartonNoMatchProf() const{
    return hjetpartonNprof;
  }
  TProfile *GetJetParticleNoMatchProf() const{
    return hjetparticleNprof;
  }
  TH2F *GetProcessFlavor() const{
    return hproc;
  }
  //pt shift
  TProfile *GetJetDiffPartonProf() const{
    return hdptparton;
  }

  TProfile *GetJetDiffParticleProf() const{
    return hdptparticle;
  }
  TProfile *GetJetDiffParticleCrrProf() const{
    return hdptparticlecrr;
  }
  //w^2
  TProfile *GetJetDiffPartonW2Prof() const{
    return hdptpartonw2;
  }
  TProfile *GetJetDiffParticleW2Prof() const{
    return hdptparticlew2;
  }
  TProfile *GetJetDiffParticleCrrW2Prof() const{
    return hdptparticlecrrw2;
  }

  TProfile *GetALLDetector(int imod) const {
    if(imod < NMod)
      return haLLdetector[imod];
    else
      return NULL;
  }
  TH2F *GetALLDetector2D(int imod) const {
    if(imod < NMod)
      return haLLdetector2d[imod];
    else
      return NULL;
  }
  //w^2
  TProfile *GetALLDetectorW2(int imod) const {
    if(imod < NMod)
      return haLLdetectorw2[imod];
    else
      return NULL;
  }
  //mean pt
  TProfile *GetPtDetector() const {
    return hptdetector;
  }
  //w^2
  TProfile *GetPtDetectorW2() const {
    return hptdetectorw2;
  }
  //dpt
  TProfile *GetDPtJet() const {
    return hjetdpt;
  }
  TH2F *GetDPtJet2D() const {
    return hjetdpt2d;
  }
  //w^2
  TProfile *GetDPtJetW2() const {
    return hjetdptw2;
  }
  //
  void FillJetPt(float pt, float w  = 1)
  {
    hjetpt->Fill(pt,w);
  }
  //x, xg
  void FillXParton(float x, float pt, float w = 1)
  {
    hxparton->Fill(x, pt, w);
  }
  void FillXgParton(float xg, float pt, float w = 1)
  {
    hxgparton->Fill(xg, pt, w);
  }
  void FillXParticle(float x, float pt, float w = 1)
  {
    hxparticle->Fill(x, pt, w);
  }
  void FillXgParticle(float xg, float pt, float w = 1)
  {
    hxgparticle->Fill(xg, pt, w);
  }
  void FillJetPtNoVtx(float pt, float w = 1)
  {
    hjetptnovtx->Fill(pt, w);
  }

  //
  void FillTwoJetPt(float pt1, float pt2, float w = 1){
    htwojetpt->Fill(pt1, pt2, w);
    htwojetpt->Fill(pt2, pt1, w);
  }
  void FillJetPartonPt(float pt, float ptpar, float w  = 1)
  {
    hjetptparton->Fill(pt, ptpar, w);
    //hjetptpartonres->Fill(pt, ptpar, w);
  }
  void FillJetParticlePt(float pt, float ptpar, float w  = 1)
  {
    hjetptparticle->Fill(pt, ptpar, w);
    //hjetptparticleres->Fill(pt, ptpar, w);
  }
  void FillJetParticlePtCrr(float pt, float ptpar, float w  = 1)
  {
    hjetptparticlecrr->Fill(pt, ptpar, w);
    //hjetptparticlecrrres->Fill(pt, ptpar, w);
  }
  //pt shift
  void FillJetDiffPartonProf(float pt, float ptpar, float w = 1)
  {
    hdptparton->Fill(pt, ptpar, w);
    //w^2
    hdptpartonw2->Fill(pt, ptpar, w*w);
  }
  void FillJetDiffParticleProf(float pt, float ptpar, float w = 1)
  {
    hdptparticle->Fill(pt, ptpar, w);
    //w^2
    hdptparticlew2->Fill(pt, ptpar, w*w);
  }
  void FillJetDiffParticleCrrProf(float pt, float ptpar, float w = 1)
  {
    hdptparticlecrr->Fill(pt, ptpar, w);
    //w^2
    hdptparticlecrrw2->Fill(pt, ptpar, w*w);
  }
  //not matching
  void FillJetPartonNoMatch(float pt, float w = 1)
  {
    hjetpartonN->Fill(pt, w);
  }
  void FillJetParticleNoMatch(float pt, float w = 1)
  {
    hjetparticleN->Fill(pt, w);
  }
  void FillJetDPt(float pt, float dpt, float w = 1)
  {
    hjetdpt->Fill(pt, dpt, w);
    hjetdptw2->Fill(pt, dpt, w*w);
    hjetdpt2d->Fill(pt, pt-dpt, w);
  }
  void FillJetDetectorPt(float pt, float w = 1)
  {
    hptdetector->Fill(pt, pt, w);
    hptdetectorw2->Fill(pt, pt, w*w);
  }
 void Add(MyJetMC *jetmc, float w = 1.);

 private:

  TH1F *hjetpt;
  TH1F *hjetptnovtx;
  TProfile *hjetptnovtxprof;

  //x, xg
  TH2F *hxparton;
  TH2F *hxgparton;
  TH2F *hxparticle;
  TH2F *hxgparticle;
  TH2F *hproc;
  //QA
  TH2F *htwojetpt;

  TH2F *hjetptparton;
  TH2F *hjetptparticle;
  TH2F *hjetptparticlecrr;

  TProfile *hdptparton;
  TProfile *hdptparticle;
  TProfile *hdptparticlecrr;

  TProfile *hdptpartonw2;
  TProfile *hdptparticlew2;
  TProfile *hdptparticlecrrw2;

  TH1F *hjetpartonN;
  TH1F *hjetparticleN;
  TProfile *hjetpartonNprof;
  TProfile *hjetparticleNprof;
  //dpt
  TProfile *hjetdpt;
  TH2F *hjetdpt2d;
  TProfile *hjetdptw2;

  //mean pt
  TProfile *hptdetector;
  TProfile *hptdetectorw2;

  TProfile *haLLdetector[NMod];
  TH2F *haLLdetector2d[NMod];
  TProfile *haLLdetectorw2[NMod];

	    
  ClassDef(MyJetMC,1);
};
//const int MyJetMC::NMod = 101;
class MyJetPar : public TObject
{  
 public:
  static const int NMod = 101;
  MyJetPar(){}
  MyJetPar(const char *name){
    if(name){
      TH1::StatOverflows(kTRUE);
      TH1::SetDefaultSumw2(kTRUE);
      //x, xg
      const int NX = 150;
      double xx[NX+1];
      for(int ibin = 0; ibin <= NX; ibin++){
	xx[ibin] = TMath::Power(10, -3+3*(ibin+0.)/(NX+0.));
      }
      hxg = new TH2F(Form("%sXg", name), ";x_{g}", NX, xx, yNPtBins, yPtBins);
      hx = new TH2F(Form("%sX", name), ";x", NX, xx, yNPtBins, yPtBins);
      //
      hptpar = new TProfile(Form("%s_pT", name), "; p_{T} [GeV]; p_{T} [GeV]",  100, 0, 100);
      //w^2
      hptparw2 = new TProfile(Form("%s_pT_w2", name), "; p_{T} [GeV]; p_{T} [GeV]", 100, 0, 100);
      for(int imod = 0; imod < NMod; imod++){
	char title[128];
	char hname[128];
	char htitle[128];
	sprintf(title, "%s_NNPDF_iset_%d", name, imod);
    
	sprintf(hname, "aLL_%s", title); 
	sprintf(htitle, "aLL_%s;p_{T} [GeV]", title); 
	haLLpar[imod] = new TProfile(hname, htitle, 100, 0, 100);
	haLLpar2d[imod] = new TH2F(Form("%s_2d", hname), Form("%s;aLL", htitle), 100, 0, 100, 200, -1, 1);
	//w^2
	sprintf(hname, "aLL_w2_%s", title); 
	sprintf(htitle, "aLL_w2_%s;p_{T} [GeV]", title); 
	haLLparw2[imod] = new TProfile(hname, htitle, 100, 0, 100);
      }
    }
  }
  ~MyJetPar(){
    if(hx != NULL) delete hx;
    if(hxg != NULL) delete hxg;
    if(hptpar != NULL) delete hptpar;
    if(hptparw2 != NULL) delete hptparw2;
    for(int imod = 0; imod < NMod; imod++){
      if(haLLpar[imod] != NULL) delete haLLpar[imod];
      delete haLLpar2d[imod];
      if(haLLparw2[imod] != NULL) delete haLLparw2[imod];
    }
  }
  TProfile *GetALLPar(int imod) const {
    if(imod < NMod)
      return haLLpar[imod];
    else
      return NULL;
  }
  TH2F *GetALLPar2D(int imod) const {
    if(imod < NMod)
      return haLLpar2d[imod];
    else
      return NULL;
  }
  TProfile *GetALLParW2(int imod) const {
    if(imod < NMod)
      return haLLparw2[imod];
    else
      return NULL;
  }
  TProfile *GetPtPar() const {
    return hptpar;
  }
  TProfile *GetPtParW2() const {
    return hptparw2;
  }
  //x, xg
  TH2F* GetX() const{
    return hx;
  }
  TH2F* GetXg() const{
    return hxg;
  }

  void Add(MyJetPar *jetmc, float w = 1.);
 private:
  TH2F *hx;
  TH2F *hxg;
  TProfile *hptpar;
  TProfile *hptparw2;

  TProfile *haLLpar[NMod];
  TH2F *haLLpar2d[NMod];

  TProfile *haLLparw2[NMod];

  ClassDef(MyJetPar, 1);
};
//const int MyJetPar::NMod = 101;
#endif
