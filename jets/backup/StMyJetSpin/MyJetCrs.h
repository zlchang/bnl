#ifndef ROOT_MyJetCrs
#define ROOT_MyJetCrs

//#include<iostream>
#include "StMyJetPool/StMyDef/MyDef.h"
#include"TH1F.h"
#include"TH2F.h"
#include"TProfile.h"

#include"TObject.h"

#include"MyJetSpin.h"

//using std::cout;

class MyJetCrs : public TObject
{
 private:

  TH1F *hjet;
  TH1F *hjetblue;
  TH1F *hjetyell;
  TH1F *hjetlsign;
  TH1F *hjetusign;

  TH1F *hjetsig;
  TH1F *hjetsigblue;
  TH1F *hjetsigyell;
  TH1F *hjetsiglsign;
  TH1F *hjetsigusign;


  TH2F *hjeterr;
  TH2F *hjeterrblue;
  TH2F *hjeterryell;
  TH2F *hjeterrlsign;
  TH2F *hjeterrusign;
  ClassDef(MyJetCrs,1);
 public:

  MyJetCrs(){};
  MyJetCrs(const char* myjetname)
    {
      char name[64];

      TH1::StatOverflows(kTRUE);
      TH1::SetDefaultSumw2(kTRUE);    
  
      sprintf(name, "%sJetXsection", myjetname);
      hjet = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetBlueXsection", myjetname);
      hjetblue = new TH1F(name,";p_{T} [Gev]",yNPtBins,yPtBins);
      sprintf(name, "%sJetYellXsection", myjetname);
      hjetyell = new TH1F(name,";p_{T} [Gev]",yNPtBins,yPtBins);
      sprintf(name, "%sJetLikeSignXsection", myjetname);
      hjetlsign = new TH1F(name,";p_{T} [Gev]",yNPtBins,yPtBins);
      sprintf(name, "%sJetUnlikeSignXsection", myjetname);
      hjetusign = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      //sig
      sprintf(name, "%sJetXsectionSigma", myjetname);
      hjetsig = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetBlueXsectionSigma", myjetname);
      hjetsigblue = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetYellXsectionSigma", myjetname);
      hjetsigyell = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetLikeSignXsectionSigma", myjetname);
      hjetsiglsign = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetUnlikeSignXsectionSigma", myjetname);
      hjetsigusign = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);

      //err
      sprintf(name, "%sJetXsectionError", myjetname);
      hjeterr = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetBlueXsectionError", myjetname);
      hjeterrblue = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetYellXsectionError", myjetname);
      hjeterryell = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetLikeSignXsectionError", myjetname);
      hjeterrlsign = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetUnlikeSignXsectionError", myjetname);
      hjeterrusign = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      
    }

  ~MyJetCrs()
    {
      if(hjet != NULL) { delete hjet; hjet = 0;}
      if(hjetblue != NULL) { delete hjetblue; hjetblue = 0;}
      if(hjetyell != NULL) { delete hjetyell; hjetyell = 0;}
      if(hjetlsign != NULL) { delete hjetlsign; hjetlsign = 0;}
      if(hjetusign != NULL) { delete hjetusign; hjetusign = 0;}

      if(hjetsig != NULL) { delete hjetsig; hjetsig = 0;}
      if(hjetsigblue != NULL) { delete hjetsigblue; hjetsigblue = 0;}
      if(hjetsigyell != NULL) { delete hjetsigyell; hjetsigyell = 0;}
      if(hjetsiglsign != NULL) { delete hjetsiglsign; hjetsiglsign = 0;}
      if(hjetsigusign != NULL) { delete hjetsigusign; hjetsigusign = 0;}
      
      if(hjeterr != NULL) { delete hjeterr; hjeterr = 0;}
      if(hjeterrblue != NULL) { delete hjeterrblue; hjeterrblue = 0;}
      if(hjeterryell != NULL) { delete hjeterryell; hjeterryell = 0;}
      if(hjeterrlsign != NULL) { delete hjeterrlsign; hjeterrlsign = 0;}
      if(hjeterrusign != NULL) { delete hjeterrusign; hjeterrusign = 0;}
    }
	
  TH1F *GetJetXsection() const{
    return hjet;
  }

  TH1F *GetJetBlueXsection() const{
    return hjetblue;
  }

  TH1F *GetJetYellXsection() const{
    return hjetyell;
  }

  TH1F *GetJetLikeSignXsection() const{
    return hjetlsign;
  }

  TH1F *GetJetUnlikeSignXsection() const{
    return hjetusign;
  }
  //
  TH1F *GetJetXsectionSigma() const{
    return hjetsig;
  }

  TH1F *GetJetBlueXsectionSigma() const{
    return hjetsigblue;
  }

  TH1F *GetJetYellXsectionSigma() const{
    return hjetsigyell;
  }

  TH1F *GetJetLikeSignXsectionSigma() const{
    return hjetsiglsign;
  }

  TH1F *GetJetUnlikeSignXsectionSigma() const{
    return hjetsigusign;
  }

  //
  
  TH2F *GetJetXsectionError() const{
    return hjeterr;
  }

  TH2F *GetJetBlueXsectionError() const{
    return hjeterrblue;
  }

  TH2F *GetJetYellXsectionError() const{
    return hjeterryell;
  }

  TH2F *GetJetLikeSignXsectionError() const{
    return hjeterrlsign;
  }

  TH2F *GetJetUnlikeSignXsectionError() const{
    return hjeterrusign;
  }
  
  void FillJetXsection(MyJetSpin *jetspin, float pol = -1.0, float r3 = 1.0, float pb = 0.55, float py = 0.55, float scale = 1.0)
  {
    hjet->Add(jetspin->GetJetSpin(5),pb*py*scale);
    hjet->Add(jetspin->GetJetSpin(10),pb*py*scale);
    hjet->Add(jetspin->GetJetSpin(6), pb*py*pol*r3*scale);
    hjet->Add(jetspin->GetJetSpin(9), pb*py*pol*r3*scale);    
  }

  void FillJetBlueXsection(MyJetSpin *jetspin, float pol = -1.0, float r2 = 1.0, float pb = 0.55, float scale = 1.0)
  {
    hjetblue->Add(jetspin->GetJetSpin(5),pb*scale);
    hjetblue->Add(jetspin->GetJetSpin(6),pb*scale);
    hjetblue->Add(jetspin->GetJetSpin(9), pb*pol*r2*scale);
    hjetblue->Add(jetspin->GetJetSpin(10), pb*pol*r2*scale);    
  }

  void FillJetYellXsection(MyJetSpin *jetspin, float pol = -1.0, float r1 = 1.0, float py = 0.55, float scale = 1.0)
  {
    hjetyell->Add(jetspin->GetJetSpin(5),py*scale);
    hjetyell->Add(jetspin->GetJetSpin(9),py*scale);
    hjetyell->Add(jetspin->GetJetSpin(6), py*pol*r1*scale);
    hjetyell->Add(jetspin->GetJetSpin(10), py*pol*r1*scale);    
  }

  void FillJetLikeSignXsection(MyJetSpin *jetspin, float pol = -1.0, float r4 = 1.0, float pb = 0.55, float py = 0.55, float scale = 1.0)
  {
    hjetlsign->Add(jetspin->GetJetSpin(5),pb*py*scale);
    hjetlsign->Add(jetspin->GetJetSpin(10), pb*py*pol*r4*scale);    
  }

  void FillJetUnlikeSignXsection(MyJetSpin *jetspin, float pol = -1.0, float r5 = 1.0, float r6 = 1.0, float pb = 0.55, float py = 0.55, float scale = 1.0)
  {
    hjetusign->Add(jetspin->GetJetSpin(6),pb*py*r5*scale);
    hjetusign->Add(jetspin->GetJetSpin(9),pb*py*pol*r6*scale);
  }

  void FillJetXsectionError(MyJetSpin *jetspin, float pol = 1.0, float r3 = 1.0, float pb = 0.55, float py = 0.55, float scale = 1.0)
  {
    hjetsig->Add(jetspin->GetJetSigSpin(5),pb*py*scale);
    hjetsig->Add(jetspin->GetJetSigSpin(10),pb*py*scale);
    hjetsig->Add(jetspin->GetJetSigSpin(6), pb*py*pol*r3*scale);
    hjetsig->Add(jetspin->GetJetSigSpin(9), pb*py*pol*r3*scale);    

    hjeterr->Add(jetspin->GetJetErrSpin(5),pb*py*scale);
    hjeterr->Add(jetspin->GetJetErrSpin(10),pb*py*scale);
    hjeterr->Add(jetspin->GetJetErrSpin(6), pb*py*pol*r3*scale);
    hjeterr->Add(jetspin->GetJetErrSpin(9), pb*py*pol*r3*scale);    
  }

  void FillJetBlueXsectionError(MyJetSpin *jetspin, float pol = 1.0, float r2 = 1.0, float pb = 0.55, float scale = 1.0)
  {
/*
    cout<<"Blue\n";
    cout<<hjetsigblue->GetBinContent(1)<<"\n";
    cout<<jetspin->GetJetSigSpin(5)->GetBinContent(1)<<"\n";
    cout<<jetspin->GetJetSigSpin(6)->GetBinContent(1)<<"\n";
    cout<<jetspin->GetJetSigSpin(9)->GetBinContent(1)<<"\n";
    cout<<jetspin->GetJetSigSpin(10)->GetBinContent(1)<<"\n";
    cout<<pb<<" "<<pol<<" "<<r2<<"\n";
*/
    hjetsigblue->Add(jetspin->GetJetSigSpin(5),pb*scale);
    hjetsigblue->Add(jetspin->GetJetSigSpin(6),pb*scale);
    hjetsigblue->Add(jetspin->GetJetSigSpin(9), pb*pol*r2*scale);
    hjetsigblue->Add(jetspin->GetJetSigSpin(10), pb*pol*r2*scale);    

//    cout<<hjetsigblue->GetBinContent(1)<<"\n";

    hjeterrblue->Add(jetspin->GetJetErrSpin(5),pb*scale);
    hjeterrblue->Add(jetspin->GetJetErrSpin(6),pb*scale);
    hjeterrblue->Add(jetspin->GetJetErrSpin(9), pb*pol*r2*scale);
    hjeterrblue->Add(jetspin->GetJetErrSpin(10), pb*pol*r2*scale);    

  }

  void FillJetYellXsectionError(MyJetSpin *jetspin, float pol = 1.0, float r1 = 1.0, float py = 0.55, float scale = 1.0)
  {
    hjetsigyell->Add(jetspin->GetJetSigSpin(5),py*scale);
    hjetsigyell->Add(jetspin->GetJetSigSpin(9),py*scale);
    hjetsigyell->Add(jetspin->GetJetSigSpin(6), py*pol*r1*scale);
    hjetsigyell->Add(jetspin->GetJetSigSpin(10), py*pol*r1*scale);    

    hjeterryell->Add(jetspin->GetJetErrSpin(5),py*scale);
    hjeterryell->Add(jetspin->GetJetErrSpin(9),py*scale);
    hjeterryell->Add(jetspin->GetJetErrSpin(6), py*pol*r1*scale);
    hjeterryell->Add(jetspin->GetJetErrSpin(10), py*pol*r1*scale);    

  }

  void FillJetLikeSignXsectionError(MyJetSpin *jetspin, float pol = 1.0, float r4 = 1.0, float pb = 0.55, float py = 0.55, float scale = 1.0)
  {
    hjetsiglsign->Add(jetspin->GetJetSigSpin(5),pb*py*scale);
    hjetsiglsign->Add(jetspin->GetJetSigSpin(10), pb*py*pol*r4*scale);    

    hjeterrlsign->Add(jetspin->GetJetErrSpin(5),pb*py*scale);
    hjeterrlsign->Add(jetspin->GetJetErrSpin(10), pb*py*pol*r4*scale);    
  }

  void FillJetUnlikeSignXsectionError(MyJetSpin *jetspin, float pol = 1.0, float r5 = 1.0, float r6 = 1.0, float pb = 0.55, float py = 0.55, float scale = 1.0)
  {
    hjetsigusign->Add(jetspin->GetJetSigSpin(6),pb*py*r5*scale);
    hjetsigusign->Add(jetspin->GetJetSigSpin(9),pb*py*pol*r6*scale);

    hjeterrusign->Add(jetspin->GetJetErrSpin(6),pb*py*r5*scale);
    hjeterrusign->Add(jetspin->GetJetErrSpin(9),pb*py*pol*r6*scale);
  }
  void FillPolXection(MyJetSpin *jetspin, float rel[], float pb, float py, float scale = 1.0);
  void FillUnpXection(MyJetSpin *jetspin, float rel[], float pb, float py, float scale = 1.0);
};

#endif
