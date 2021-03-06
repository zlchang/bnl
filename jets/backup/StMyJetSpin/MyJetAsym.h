#ifndef ROOT_MyJetAsym
#define ROOT_MyJetAsym
//#include<iostream>
//using namespace std;
#include"StMyJetPool/StMyDef/MyDef.h"
#include"TH1F.h"
#include"TH2F.h"
#include"TProfile.h"

#include"TObject.h"
#include"TMath.h"

#include"MyJetCrs.h"

class MyJetCrs;

class MyJetAsym : public TObject
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
  ClassDef(MyJetAsym,1);
 public:
  MyJetAsym(){};
  MyJetAsym(const char* myjetname)
    {
      char name[64];

      TH1::StatOverflows(kTRUE);
//      TH1::SetDefaultSumw2(kTRUE);

      sprintf(name, "%sJetAsym", myjetname);
      hjet = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetBlueAsym", myjetname);
      hjetblue = new TH1F(name,";p_{T} [Gev]",yNPtBins,yPtBins);
      sprintf(name, "%sJetYellAsym", myjetname);
      hjetyell = new TH1F(name,";p_{T} [Gev]",yNPtBins,yPtBins);
      sprintf(name, "%sJetLikeSignAsym", myjetname);
      hjetlsign = new TH1F(name,";p_{T} [Gev]",yNPtBins,yPtBins);
      sprintf(name, "%sJetUnlikeSignAsym", myjetname);
      hjetusign = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      //sig
      sprintf(name, "%sJetAsymSigma", myjetname);
      hjetsig = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetBlueAsymSigma", myjetname);
      hjetsigblue = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetYellAsymSigma", myjetname);
      hjetsigyell = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetLikeSignAsymSigma", myjetname);
      hjetsiglsign = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
      sprintf(name, "%sJetUnlikeSignAsymSigma", myjetname);
      hjetsigusign = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);

      //err
      sprintf(name, "%sJetAsymError", myjetname);
      hjeterr = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetBlueAsymError", myjetname);
      hjeterrblue = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetYellAsymError", myjetname);
      hjeterryell = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetLikeSignAsymError", myjetname);
      hjeterrlsign = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);
      sprintf(name, "%sJetUnlikeSignAsymError", myjetname);
      hjeterrusign = new TH2F(name,";p_{T} [GeV];p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);      
    }

  ~MyJetAsym()
    {
      if(hjet != NULL) { delete hjet; hjet = 0;}
      if(hjetblue != NULL) { delete hjetblue; hjetblue = 0;}
      if(hjetyell != NULL) { delete hjetyell; hjetyell = 0;}
      if(hjetlsign != NULL) { delete hjetlsign; hjetlsign = 0;}
      if(hjetusign != NULL) { delete hjetusign; hjetusign = 0;}

      if(hjeterr != NULL) { delete hjeterr; hjeterr = 0;}
      if(hjeterrblue != NULL) { delete hjeterrblue; hjeterrblue = 0;}
      if(hjeterryell != NULL) { delete hjeterryell; hjeterryell = 0;}
      if(hjeterrlsign != NULL) { delete hjeterrlsign; hjeterrlsign = 0;}
      if(hjeterrusign != NULL) { delete hjeterrusign; hjeterrusign = 0;}

      if(hjetsig != NULL) { delete hjetsig; hjetsig = 0;}
      if(hjetsigblue != NULL) { delete hjetsigblue; hjetsigblue = 0;}
      if(hjetsigyell != NULL) { delete hjetsigyell; hjetsigyell = 0;}
      if(hjetsiglsign != NULL) { delete hjetsiglsign; hjetsiglsign = 0;}
      if(hjetsigusign != NULL) { delete hjetsigusign; hjetsigusign = 0;}
    }
	
  TH1F *GetJetAsym() const{
    return hjet;
  }

  TH1F *GetJetBlueAsym() const{
    return hjetblue;
  }

  TH1F *GetJetYellAsym() const{
    return hjetyell;
  }

  TH1F *GetJetLikeSignAsym() const{
    return hjetlsign;
  }

  TH1F *GetJetUnlikeSignAsym() const{
    return hjetusign;
  }
  //
  TH1F *GetJetAsymSigma() const{
    return hjetsig;
  }

  TH1F *GetJetBlueAsymSigma() const{
    return hjetsigblue;
  }

  TH1F *GetJetYellAsymSigma() const{
    return hjetsigyell;
  }

  TH1F *GetJetLikeSignAsymSigma() const{
    return hjetsiglsign;
  }

  TH1F *GetJetUnlikeSignAsymSigma() const{
    return hjetsigusign;
  }

  TH2F *GetJetAsymError() const{
    return hjeterr;
  }

  TH2F *GetJetBlueAsymError() const{
    return hjeterrblue;
  }

  TH2F *GetJetYellAysmError() const{
    return hjeterryell;
  }

  TH2F *GetJetLikeSignAsymError() const{
    return hjeterrlsign;
  }

  TH2F *GetJetUnlikeSignAsymError() const{
    return hjeterrusign;
  }
  //Fill
  void FillJetAsym(MyJetCrs *pol, MyJetCrs *unp)
  {
    hjet->Add(pol->GetJetXsection());
    hjet->Divide(unp->GetJetXsection());
  }

  void FillJetBlueAsym(MyJetCrs *pol, MyJetCrs *unp)
  {
    hjetblue->Add(pol->GetJetBlueXsection());
    hjetblue->Divide(unp->GetJetBlueXsection());
  }

  void FillJetYellAsym(MyJetCrs *pol, MyJetCrs *unp)
  {
    hjetyell->Add(pol->GetJetYellXsection());
    hjetyell->Divide(unp->GetJetYellXsection());
  }

  void FillJetLikeSignAsym(MyJetCrs *pol, MyJetCrs *unp)
  {
    hjetlsign->Add(pol->GetJetLikeSignXsection());
    hjetlsign->Divide(unp->GetJetLikeSignXsection());
  }

  void FillJetUnlikeSignAsym(MyJetCrs *pol, MyJetCrs *unp)
  {
    hjetusign->Add(pol->GetJetUnlikeSignXsection());
    hjetusign->Divide(unp->GetJetUnlikeSignXsection());
  }

  void FillJetAsymError(MyJetCrs *pol, MyJetCrs *unp)
  {
    float tmp;
    hjetsig->Add(pol->GetJetXsectionSigma());
    hjeterr->Add(pol->GetJetXsectionError());
    int NBin = hjet->GetNbinsX();
    for(int xBin = 1; xBin <= NBin; xBin++)
      {
	tmp=hjetsig->GetBinContent(xBin);
	tmp=TMath::Sqrt(tmp);
	hjetsig->SetBinContent(xBin,tmp);
	for(int yBin = 1; yBin <= NBin; yBin++)
	  {
	    tmp=hjeterr->GetBinContent(xBin,yBin);
	    tmp=TMath::Sqrt(tmp);
	    tmp=tmp/unp->GetJetXsection()->GetBinContent(xBin);
	    hjeterr->SetBinContent(xBin,yBin,tmp);
	  }
      }
    hjetsig->Divide(unp->GetJetXsection());

  }

  void FillJetBlueAsymError(MyJetCrs *pol, MyJetCrs *unp)
  {
    float tmp;
    hjetsigblue->Add(pol->GetJetBlueXsectionSigma());
    hjeterrblue->Add(pol->GetJetBlueXsectionError());
    int NBin = hjetsigblue->GetNbinsX();
    for(int xBin = 1; xBin <= NBin; xBin++)
      {
	tmp=hjetsigblue->GetBinContent(xBin);
	tmp=TMath::Sqrt(tmp);
	hjetsigblue->SetBinContent(xBin,tmp);
	for(int yBin = 1; yBin <= NBin; yBin++)
	  {
	    tmp=hjeterrblue->GetBinContent(xBin,yBin);
	    tmp=TMath::Sqrt(tmp);
	    tmp=tmp/unp->GetJetBlueXsection()->GetBinContent(xBin);
	    hjeterrblue->SetBinContent(xBin,yBin,tmp);
	  }

      }
    hjetsigblue->Divide(unp->GetJetBlueXsection());
  }

  void FillJetYellAsymError(MyJetCrs *pol, MyJetCrs *unp)
  {
    float tmp;
    hjetsigyell->Add(pol->GetJetYellXsectionSigma());
    hjeterryell->Add(pol->GetJetYellXsectionError());
    int NBin = hjetsigyell->GetNbinsX();
    for(int xBin = 1; xBin <= NBin; xBin++)
      {
	tmp=hjetsigyell->GetBinContent(xBin);
	tmp=TMath::Sqrt(tmp);
	hjetsigyell->SetBinContent(xBin,tmp);
	for(int yBin = 1; yBin <= NBin; yBin++)
	  {
	    tmp=hjeterryell->GetBinContent(xBin,yBin);
	    tmp=TMath::Sqrt(tmp);
	    tmp=tmp/unp->GetJetYellXsection()->GetBinContent(xBin);
	    hjeterryell->SetBinContent(xBin,yBin,tmp);
	  }
      }
    hjetsigyell->Divide(unp->GetJetYellXsection());
  }

  void FillJetLikeSignAsymError(MyJetCrs *pol, MyJetCrs *unp)
  {
    float tmp;
    hjetsiglsign->Add(pol->GetJetLikeSignXsectionSigma());
    hjeterrlsign->Add(pol->GetJetLikeSignXsectionError());
    int NBin = hjetsiglsign->GetNbinsX();
    for(int xBin = 1; xBin <= NBin; xBin++)
      {
	tmp=hjetsiglsign->GetBinContent(xBin);
	tmp=TMath::Sqrt(tmp);
	hjetsiglsign->SetBinContent(xBin,tmp);
	for(int yBin = 1; yBin <= NBin; yBin++)
	  {
	    tmp=hjeterrlsign->GetBinContent(xBin,yBin);
	    tmp=TMath::Sqrt(tmp);
	    tmp=tmp/unp->GetJetLikeSignXsection()->GetBinContent(xBin);
	    hjeterrlsign->SetBinContent(xBin,yBin,tmp);
	  }

      }

    hjetsiglsign->Divide(unp->GetJetLikeSignXsection());

  }

  void FillJetUnlikeSignAsymError(MyJetCrs *pol, MyJetCrs *unp)
  {
    float tmp;
    hjetsigusign->Add(pol->GetJetUnlikeSignXsectionSigma());
    hjeterrusign->Add(pol->GetJetUnlikeSignXsectionError());
    int NBin = hjetsigusign->GetNbinsX();
    for(int xBin = 1; xBin <= NBin; xBin++)
      {
	tmp=hjetsigusign->GetBinContent(xBin);
	tmp=TMath::Sqrt(tmp);
	hjetsigusign->SetBinContent(xBin,tmp);
	for(int yBin = 1; yBin <= NBin; yBin++)
	  {
	    tmp=hjeterrusign->GetBinContent(xBin,yBin);
	    tmp=TMath::Sqrt(tmp);
	    tmp=tmp/unp->GetJetUnlikeSignXsection()->GetBinContent(xBin);
	    hjeterrusign->SetBinContent(xBin,yBin,tmp);
	  }
      }
    hjetsigusign->Divide(unp->GetJetUnlikeSignXsection());
  }

  void FillAsymmetry(MyJetCrs *jetpol, MyJetCrs *jetunp);
};
#endif
