#ifndef ROOT_MYJETVTX
#define ROOT_MYJETVTX

#include "TObject.h"
#include"StMyJetPool/StMyDef/MyDef.h"
#include "StMyHistUtil/StMyHist.h"

class MyJetVtx : public TObject{
 public:
  MyJetVtx(){};
  MyJetVtx(const char* name){
    hjetptvtxprof = new StMyProfile2D(Form("%sJetPtVtxMcProf",name), ";p_{T} [GeV]; #eta", yNPtBins, yPtBins, yNEtaBins, yEtaBins);      
  }
  ~MyJetVtx(){
    delete hjetptvtxprof;
  }

  StMyProfile2D *GetJetVertexProf() const{
    return hjetptvtxprof;
  }
  void Add(MyJetVtx *jetvtx, float w){
    hjetptvtxprof->Add(jetvtx->GetJetVertexProf(), w);
  }
  
 private:
  StMyProfile2D *hjetptvtxprof;
  ClassDef(MyJetVtx, 1);
};
#endif
