#ifndef ROOT_MyJetSpin
#define ROOT_MyJetSpin

#include"StMyJetPool/StMyDef/MyDef.h"

#include"TH1F.h"
#include"TH2F.h"
#include"TProfile.h"

#include"TObject.h"

class MyJetSpin : public TObject
{
 private:
  TH1F *hjetpt[4];
  //TH1F *hjetonept[4];
  TH2F *hjettwopt[4];

  TH1F *hjetsigpt[4];
  //  TH2F *hjeterrpt[4];

  TProfile *hjetmeanpt[4];
  TProfile *hjetdpt[4];
  TProfile *hjetmult[4];
  TProfile *hjetmultue[4];

 public:
  MyJetSpin(){}
  MyJetSpin(const char* myjetname)
    {
      int spinbit;
      char name[50];
      
      TH1::StatOverflows(kTRUE);
      TH1::SetDefaultSumw2(kTRUE);
      
      for(int iSpin = 0; iSpin < 4; iSpin++)
	{
	  if(iSpin == 0)
	    {
	      spinbit = 5;
	    }else if(iSpin == 1)
	    {
	      spinbit = 6;
	    }else if(iSpin == 2)
	    {
	      spinbit = 9;
	    }else if(iSpin == 3)
	    {
	      spinbit = 10;
	    }

	  sprintf(name,"%s_pT_spin%d", myjetname, spinbit);
	  hjetpt[iSpin] = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);

	  //sprintf(name,"%s_pT_onejet_spin%d",myjetname, spinbit);
	  //hjetonept[iSpin] = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
	  sprintf(name,"%s_pT_twojet_spin%d", myjetname, spinbit);
	  hjettwopt[iSpin] = new TH2F(name,";p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);

	  sprintf(name,"%s_pT_sig_spin%d", myjetname, spinbit);
	  hjetsigpt[iSpin] = new TH1F(name,";p_{T} [GeV]",yNPtBins,yPtBins);
	  //	  sprintf(name,"%s_pT_err_spin%d",myjetname, spinbit);
	  //	  hjeterrpt[iSpin] = new TH2F(name,";p_{T} [GeV]",yNPtBins,yPtBins,yNPtBins,yPtBins);

	  sprintf(name,"%s_pT_mean_pt%d", myjetname, spinbit);
	  hjetmeanpt[iSpin] = new TProfile(name,";p_{T} [GeV]",yNPtBins, yPtBins);
	  sprintf(name,"%s_pT_dpt%d", myjetname, spinbit);
	  hjetdpt[iSpin] = new TProfile(name,";p_{T} [GeV]",yNPtBins, yPtBins);
	  sprintf(name,"%s_pT_mult%d", myjetname, spinbit);
	  hjetmult[iSpin] = new TProfile(name,";p_{T} [GeV];mult",yNPtBins, yPtBins);
	  sprintf(name,"%s_pT_mult_ue%d", myjetname, spinbit);
	  hjetmultue[iSpin] = new TProfile(name,";p_{T} [GeV];ue mult",yNPtBins, yPtBins);
	}
    }
  
  ~MyJetSpin()
    {
      for(int iSpin = 0; iSpin < 4; iSpin++)
	{
	  if(hjetpt[iSpin] != NULL)
	    {
	      delete hjetpt[iSpin];
	      hjetpt[iSpin] = 0;
	    }
	  //if(hjetonept[iSpin] != NULL)
	  //  {
	  //    delete hjetonept[iSpin];
	  //    hjetonept[iSpin] = 0;
	  //  }
	  if(hjettwopt[iSpin] != NULL)
	    {
	      delete hjettwopt[iSpin];
	      hjettwopt[iSpin] = 0;
	    }

	  //	  if(hjeterrpt[iSpin] != NULL)
	  //	    {
	  //	      delete hjeterrpt[iSpin];
	  //	      hjeterrpt[iSpin] = 0;
	  //	    }

	  if(hjetsigpt[iSpin] != NULL)
	    {
	      delete hjetsigpt[iSpin];
	      hjetsigpt[iSpin] = 0;
	    }
	  if(hjetmeanpt[iSpin] != NULL)
	    {
	      delete hjetmeanpt[iSpin];
	      hjetmeanpt[iSpin] = 0;
	    }
	  if(hjetdpt[iSpin] != NULL)
	    {
	      delete hjetdpt[iSpin];
	      hjetdpt[iSpin] = 0;
	    }
	  if(hjetmult[iSpin] != NULL)
	    {
	      delete hjetmult[iSpin];
	      hjetmult[iSpin] = 0;
	    }
	  if(hjetmultue[iSpin] != NULL)
	    {
	      delete hjetmultue[iSpin];
	      hjetmultue[iSpin] = 0;
	    }

	}
    }
  static int SpinIndex(int spin){
    switch(spin)
      {
      case 5 :
	return 0;
	//	break;
      case 6 :
	return 1;
      case 9 :
	return 2;
      case 10 :
	return 3;
      default :
	return -1;
      }
  }
  void AddJetSpin(MyJetSpin *jetspin, float w = 1);

  TH1F * const *GetJetSpin() const{
    return hjetpt;
  }
  TH1F *GetJetSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjetpt[index];
    else
      return NULL;
  }

  TH1F * const *GetJetSigSpin() const{
    return hjetsigpt;
  }
  TH1F *GetJetSigSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjetsigpt[index];
    else
      return NULL;
  }

//  TH1F * const *GetJetOneSpin() const{
//    return hjetonept;
//  }
//  TH1F *GetJetOneSpin(int spin) const{
//    int index = MyJetSpin::SpinIndex(spin);
//    if(index >= 0 && index <=3) 
//      return hjetonept[index];
//    else
//      return NULL;
//  }

  TH2F * const *GetJetTwoSpin() const{
    return hjettwopt;
  }
  TH2F *GetJetTwoSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjettwopt[index];
    else
      return NULL;
  }

  TH2F * const *GetJetErrSpin() const{
    //    return hjeterrpt;
    return hjettwopt;
  }
  TH2F *GetJetErrSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      //    return hjeterrpt[index];
      return hjettwopt[index];
    else
      return NULL;
  }

  TProfile * const *GetJetMeanPtSpin() const{
    return hjetmeanpt;
  }
  TProfile *GetJetMeanPtSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjetmeanpt[index];
    else
      return NULL;
  }

  TProfile * const *GetJetDPtSpin() const{
    return hjetdpt;
  }
  TProfile *GetJetDPtSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjetdpt[index];
    else
      return NULL;
  }

  TProfile * const *GetJetMultSpin() const{
    return hjetmult;
  }
  TProfile *GetJetMultSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjetmult[index];
    else
      return NULL;
  }

  TProfile * const *GetJetMultUeSpin() const{
    return hjetmultue;
  }
  TProfile *GetJetMultUeSpin(int spin) const{
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <=3) 
      return hjetmultue[index];
    else
      return NULL;
  }

  void FillJetPt(int spin, float pt, float w  = 1)
  {
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <= 3)
      hjetpt[index]->Fill(pt,w);
  }

//  void FillJetOnePt(int spin, float pt, float w  = 1)
//  {
//    int index = MyJetSpin::SpinIndex(spin);
//    if(index >= 0 && index <= 3)
//      hjetonept[index]->Fill(pt,w);
//  }

  void FillJetTwoPt(int spin, float pt1, float pt2, float w  = 1)
  {
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <= 3)
      hjettwopt[index]->Fill(pt1,pt2,w);
  }

  void FillJetErrorPt(int spin)
  {
    int index = MyJetSpin::SpinIndex(spin);

    float tmp;  
    //float sum;

    if(index >= 0 && index <= 3)
      {
	int NBin = hjetpt[index]->GetNbinsX();
	for(int xBin = 1; xBin <= NBin; xBin++)
	  {
	    tmp = hjettwopt[index]->GetBinContent(xBin,xBin);
	    tmp = hjetpt[index]->GetBinContent(xBin) + tmp;
	    hjetsigpt[index]->SetBinContent(xBin,tmp);
	  }
      }
  }
	    
  void FillJetMeanPt(int spin, float pt1, float pt2, float w  = 1)
  {
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <= 3)
      hjetmeanpt[index]->Fill(pt1,pt2,w);
  }
  void FillJetDPt(int spin, float pt1, float pt2, float w  = 1)
  {
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <= 3)
      hjetdpt[index]->Fill(pt1,pt2,w);
  }
  void FillJetMult(int spin, float pt1, float mult, float w  = 1)
  {
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <= 3)
      hjetmult[index]->Fill(pt1,mult,w);
  }
  void FillJetMultUe(int spin, float pt1, float mult, float w  = 1)
  {
    int index = MyJetSpin::SpinIndex(spin);
    if(index >= 0 && index <= 3)
      hjetmultue[index]->Fill(pt1,mult,w);
  }
  //fill
  void fill_pt(int spinbit, float pt);
  void fill_twojet_pt(int spinbit, float pta, float ptb);
  void fill_error();
  void fill_dpt(int spinbit, float pt, float dpt);
  ClassDef(MyJetSpin,3);
};

#endif
