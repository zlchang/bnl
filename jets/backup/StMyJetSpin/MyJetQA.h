#ifndef ROOT_MyJetQA
#define ROOT_MyJetQA

#include"StMyJetPool/StMyDef/MyDef.h"
#include"TH1.h"
#include"TH1F.h"
#include"TH2.h"
#include"TH2F.h"
#include"TH3F.h"
#include"TProfile.h"
#include"TObject.h"
#include"TMath.h"

class MyJetQA : public TObject
{
 public:
  //static const int NMod = 101;
  MyJetQA(){}
  MyJetQA(const char* name)
    {
      if(name)
	{
	  
	  TH1::StatOverflows(kTRUE);
          TH1::SetDefaultSumw2(kTRUE);
	  
	  //jet pT no vertex requirement
	  hjetptnovtx = new TH1F(Form("%sJetPtNoVtx",name), ";p_{T} [GeV]", yNPtBins, yPtBins);
	  //
	  hjetnposvtx = new TH2F(Form("%sJetNPosVtx", name), ";number of positive ranked vertices", yNPtBins, yPtBins, 6, 0, 6);
	  hjetvertexz = new TH1F(Form("%sJetVertexZ", name), "; Z [cm]", 400, -200, 200);
	  
	  //QA
	  hjetpt = new TH1F(Form("%sJetPt", name), ";p_{T} [GeV]",yNPtBins,yPtBins);
          //htwojetpt = new TH2F(Form("%sTwoJetPt", name), ";p_{T} [GeV]", yNPtBins, yPtBins, yNPtBins, yPtBins);
	  hjeteta = new TH1F(Form("%sJetEta", name), ";#eta", 40, -1., 1.);
	  hjetphi = new TH1F(Form("%sJetPhi", name), ";#phi", 126, -3.15, 3.15);
	  hjetrt = new TH2F(Form("%sJetRt", name), ";R_{T}", yNPtBins, yPtBins, 100, 0., 1.);
	  hjetparjt = new TH2F(Form("%sJetParticleJt", name), ";j_{T}", yNPtBins, yPtBins, 100, 0., 10.);
	  hjetparz = new TH2F(Form("%sJetParticleZ", name), ";z", yNPtBins, yPtBins, 100, 0., 1.);
	  hjetmult = new TH1F(Form("%sJetMult", name), ";mult", 200, 0, 200);
	  hjettrkmult = new TH1F(Form("%sJetTrackMult", name), ";track mult", 100, 0, 100);
	  hjettrkpt = new TH1F(Form("%sJetTrackPt", name), ";track p_{T} [GeV]", 100, 0, 100);
	  hjettwrmult = new TH1F(Form("%sJetTowerMult", name), ";tower mult", 100, 0, 100);
	  hjettwrpt = new TH1F(Form("%sJetTowerPt", name), ";tower p_{T} [GeV]", 100, 0, 100);
          for(int i = 0; i < 3; i++){
            int isel = ySEL[i];
            hjetprof[i] = new TProfile(Form("%sJetProfile%.1lf-%.1lf", name, yPtBins[isel], yPtBins[isel+1]), ";p_{T} [GeV]; frac", 25, 0., 1.0);
          }
          
          //profiles
          hjetrtprof = new TProfile(Form("%sJetRtVsPt", name), ";p_{T} [GeV]; R_{T}", yNPtBins, yPtBins);	 
          hjetptprof = new TProfile(Form("%sJetPtVsPt", name), ";p_{T} [GeV]; p_{T}", yNPtBins, yPtBins);	 
	  hjetdptue = new TProfile(Form("%sJetUeDptVsPt", name), ";p_{T} [GeV]; UE d_p{T}", yNPtBins, yPtBins); 
	  hjettwrmultue = new TProfile(Form("%sJetUeTowerMultVsPt", name), ";p_{T} [GeV]; UE tower mult", yNPtBins, yPtBins);
	  hjettrkmultue = new TProfile(Form("%sJetUeTrackMultVsPt", name), ";p_{T} [GeV]; UE track mult", yNPtBins, yPtBins);
          //ue qa
          hjetptarea = new TH2F(Form("%sJetAreaVsPt", name), ";p_{T} [GeV]; area", yNPtBins, yPtBins, 40, 0, 1.6);
	  hdptue = new TH2F(Form("%sJetUeDpt", name), ";p_{T} [GeV];UE d_p{T}", yNPtBins, yPtBins,80, 0, 16); 
          //ue
          hjetuert = new TProfile(Form("%sJetUeRt", name), ";p_{T} [GeV]; R_{T}", yNPtBins, yPtBins);//, 100, 0, 1.);	 
	  hjetueparjt = new TH2F(Form("%sJetUeParticleJt", name), ";j_{T}", yNPtBins, yPtBins, 100, 0., 10.);
	  hjetueparz = new TH2F(Form("%sJetUeParticleZ", name), ";z", yNPtBins, yPtBins, 100, 0., 1.);
	  hmultue = new TH2F(Form("%sJetUeMult", name), ";p_{T} [GeV];UE mult", yNPtBins, yPtBins, 20, 0, 20); 
	  htrkmultue = new TH2F(Form("%sJetUeTrackMult", name), ";p_{T} [GeV];UE track mult", yNPtBins, yPtBins, 10, 0, 10); 
	  htwrmultue = new TH2F(Form("%sJetUeTowerMult", name), ";p_{T} [GeV];UE tower mult", yNPtBins, yPtBins, 10, 0, 10); 
	  htrkptue = new TH2F(Form("%sJetUeTrackPt", name), ";p_{T} [GeV];UE track p_{T}", yNPtBins, yPtBins, 80, 0, 16); 
	  htwrptue = new TH2F(Form("%sJetUeTowerPt", name), ";p_{T} [GeV];UE tower p_{T}", yNPtBins, yPtBins,80, 0, 16); 
          //hdpt2due = new TH2F(Form("%sJetUeDpt2D", name), ";UE dp_{T,1} [GeV]; UE dp_{T,2}", 80, 0, 16, 80, 0, 16);
         double binM[10+1];
         for(int ib = 0; ib < 10+1; ib++){
           binM[ib] = ib*1.0;
         }
          hmult3due = new TH3F(Form("%sJetUeMult2D", name), "jet p_{T} [GeV] ;UE mult_{1}; UE mult_{2}",yNPtBins, yPtBins, 10, binM, 10, binM);
         double binT[40+1];
         for(int ib = 0; ib < 40+1; ib++){
           binT[ib] = -.2 + ib*.2;
         }
         hdpt3due = new TH3F(Form("%sJetUeDpt3D", name), ";jet p_{T} [GeV]; dp_{T,1} [GeV]; dp_{T,2} [GeV]", yNPtBins, yPtBins, 40, binT, 40, binT);

	}
        hjetetart = new TProfile(Form("%sJetRtVsEta", name), "; jet #eta; R_{T}", 10, -1.0, 1.0);
    }
  
  ~MyJetQA(){
      //
      if(hjetptnovtx != NULL) delete hjetptnovtx;
      if(hjetnposvtx != NULL) delete hjetnposvtx;
      if(hjetvertexz != NULL) delete hjetvertexz;
      //QA
      if(hjetpt != NULL) delete hjetpt;
      //if(htwojetpt != NULL) delete htwojetpt;
      if(hjeteta != NULL) delete hjeteta;
      if(hjetphi != NULL) delete hjetphi;
      if(hjetrt != NULL) delete hjetrt;
      if(hjetmult != NULL) delete hjetmult;
      if(hjettrkmult != NULL) delete hjettrkmult;
      if(hjettrkpt != NULL) delete hjettrkpt;
      if(hjettwrmult != NULL) delete hjettwrmult;
      if(hjettwrpt != NULL) delete hjettwrpt;
      if(hjetrtprof != NULL) delete hjetrtprof;
      if(hjetptprof != NULL) delete hjetptprof;
      if(hjetdptue != NULL) delete hjetdptue;
      if(hjettrkmultue != NULL) delete hjettrkmultue;
      if(hjettwrmultue != NULL) delete hjettwrmultue;
      //
      delete hjetparjt;
      delete hjetparz;
      delete hjetptarea;
      delete hdptue;
      delete hmultue;
      delete htrkmultue;
      delete htwrmultue;
      delete htrkptue;
      delete htwrptue;
      delete hdpt3due;
      delete hmult3due;
      delete hjetuert;
      delete hjetueparjt;
      delete hjetueparz;
      for(int i = 0; i < 3; i++){
        delete hjetprof[i];
      }
      delete hjetetart;
  }
  
  TH1F *GetJetPtNoVtx() const{
    return hjetptnovtx;
  }
  TH2F *GetJetNPosVtx() const {
    return hjetnposvtx;
  }
  TH1F *GetJetVertexZ() const {
    return hjetvertexz;
  }
  //QA
  TH1F* GetJetPt() const{
    return hjetpt;
  }
  //TH2F* GetTwoJetPt() const{
    //return htwojetpt;
  //}
  TProfile *GetJetProf(int i = 0){
    return hjetprof[i];
  }
  TH1F *GetJetEta() const{
    return hjeteta;
  }
  TH1F *GetJetPhi() const{
    return hjetphi;
  }	
  TH2F *GetJetRt() const{
    return hjetrt;
  }	
  TH2F *GetJetParJt() const{
    return hjetparjt;
  }	
  TH2F *GetJetParZ() const{
    return hjetparz;
  }	
  TH1F *GetJetMult() const{
    return hjetmult;
  }	
  TH1F *GetJetTrkMult() const{
    return hjettrkmult;
  }	
  TH1F *GetJetTrkPt() const{
    return hjettrkpt;
  }	
  TH1F *GetJetTwrPt() const{
    return hjettwrpt;
  }
  TH1F *GetJetTwrMult() const{
    return hjettwrmult;
  }
  //tprofiles	
  TProfile *GetJetRtProf() const{
    return hjetrtprof;
  }	
  TProfile *GetJetPtProf() const{
    return hjetptprof;
  }	
  TProfile *GetJetUeDpt() const{
    return hjetdptue;
  }
  TProfile *GetJetUeTwrMult() const{
    return hjettwrmultue;
  }	
  TProfile *GetJetUeTrkMult() const{
    return hjettrkmultue;
  }
  //
  TH2F *GetUeDpt() const{
    return hdptue;
  }  
  TH2F *GetUeMult() const{
    return hmultue;
  }  
  TH2F *GetUeTrkMult() const{
    return htrkmultue;
  }  
  TH2F *GetUeTwrMult() const{
    return htwrmultue;
  }  
  TH2F *GetUeTrkPt() const{
    return htrkptue;
  }  
  TH2F *GetUeTwrPt() const{
    return htwrptue;
  }  
  TH3F *GetUeDpt3D() const{
    return hdpt3due;
  }  
  TH3F *GetUeMult3D() const{
    return hmult3due;
  }
  TH2F *GetJetAreaVsPt() const{
    return hjetptarea;
  }  
  TProfile *GetJetUeRt() const{
    return hjetuert;
  }	
  TH2F *GetJetUeParJt() const{
    return hjetueparjt;
  }	
  TH2F *GetJetUeParZ() const{
    return hjetueparz;
  }	
  TProfile* GetJetRtVsEta() const{
     return hjetetart;
  }
  //QA	
  void FillJetPtEtaPhi(float pt, float eta, float phi, float w = 1.)
  {
    hjetpt->Fill(pt,w);
    hjeteta->Fill(eta,w);
    hjetphi->Fill(phi,w);
  }
  void FillJetPtRt(float pt, float rt, float w  = 1.)
  {
    hjetrtprof->Fill(pt, rt, w);
    hjetptprof->Fill(pt, pt, w);
    hjetrt->Fill(pt, rt,w);
  }
  void FillJetMult(float twrmult, float trkmult, float w = 1.)
  {
    hjetmult->Fill(twrmult+trkmult, w);
    hjettwrmult->Fill(twrmult,w);
    hjettrkmult->Fill(trkmult,w);
  }
  void FillJetUe(float pt, float dpt,float twrmult, float trkmult, float w = 1)
  {
    hjetdptue->Fill(pt, dpt, w);
    hjettwrmultue->Fill(pt, twrmult, w);
    hjettrkmultue->Fill(pt, trkmult, w);
    hdptue->Fill(pt, dpt, w);
    hmultue->Fill(pt, twrmult+trkmult, w);
    htrkmultue->Fill(pt, trkmult, w);
    htwrmultue->Fill(pt, twrmult, w);
  }
  //
  void FillJetPtNoVtx(float pt, float w = 1)
  {
    hjetptnovtx->Fill(pt, w);
  }
  void FillJetNPosVtx(float pt, float npos, float w = 1)
  {
    hjetnposvtx->Fill(pt, npos, w);
  }
  void FillJetVertexZ(float vz, float w = 1)
  {
    hjetvertexz->Fill(vz, w);
  }
 void Add(MyJetQA *jetqa, float w = 1.);

 private:

  TH1F *hjetptnovtx;
  TH2F *hjetnposvtx;
  TH1F *hjetvertexz;
  //QA
  TH1F *hjetpt;
  //TH2F *htwojetpt;
  TH1F *hjeteta;
  TH1F *hjetphi;
  TH2F *hjetrt;
  TH1F *hjetmult;
  TH2F *hjetparjt;
  TH2F *hjetparz;
  TH1F *hjettrkmult;
  TH1F *hjettrkpt;
  TH1F *hjettwrmult;
  TH1F *hjettwrpt;
  TProfile *hjetrtprof;
  TProfile *hjetptprof;
  //UE
  TH2F *hjetptarea;
  TProfile *hjetdptue;
  TProfile *hjettrkmultue;
  TProfile *hjettwrmultue;
  TH2F *hdptue;
  TH2F *hmultue;
  TProfile *hjetuert;
  TH2F *hjetueparjt;
  TH2F *hjetueparz;
  TH2F *htrkmultue;
  TH2F *htwrmultue;
  TH2F *htrkptue;
  TH2F *htwrptue;
  TH3F *hdpt3due;
  TH3F *hmult3due;
  TProfile *hjetprof[3];
  TProfile *hjetetart;
 
  ClassDef(MyJetQA,1);
};
#endif
