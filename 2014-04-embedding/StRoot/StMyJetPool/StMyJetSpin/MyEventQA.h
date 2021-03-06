#ifndef ROOT_MyEventQA
#define ROOT_MyEventQA

#include "TH1F.h"
#include "TObject.h"

class MyEventQA : public TObject{
 private:
  TH1F *hweight;
  TH1F *hmatchedVx;
  TH1F *hvertexz;
  TH1F *hnposvtx;
  ClassDef(MyEventQA, 1);

 public:
  MyEventQA(){}
  MyEventQA(const char *name){
    TH1::StatOverflows(kTRUE);
    TH1::SetDefaultSumw2(kTRUE);
    hweight = new TH1F(Form("%s_eventWeight",name), ";event weight", 1600, 0, 16);
    hvertexz = new TH1F(Form("%s_vertexZ", name), "; Z [cm]", 400, -200, 200);
    hmatchedVx = new TH1F(Form("%s_matchedVertex", name), "matchedVertex", 3, 0, 3);
    hnposvtx = new TH1F(Form("%sNPosVtx", name), ";number of positive ranked vertices", 6, 0, 6);
  }

  ~MyEventQA(){
    if(hweight) delete hweight;
    if(hmatchedVx) delete hmatchedVx;
    if(hvertexz) delete hvertexz;
    if(hnposvtx) delete hnposvtx;
  }

  TH1F *GetEventWeight() const{
    return hweight;
  }
  TH1F *GetMatchedVx() const{
    return hmatchedVx;
  }
  TH1F *GetVertexZ() const {
    return hvertexz;
  }
  TH1F *GetNPosVtx() const {
    return hnposvtx;
  }
  void FillVertexZ(float v, float w = 1.) const{
    hvertexz->Fill(v, w);
  }
  void FillEventWeight(float v, float w = 1.) const{
    hweight->Fill(v, w);
  }

  void FillMatchedVx(float v, float w = 1.) const{
    hmatchedVx->Fill(v, w);
  }
  void Add(MyEventQA *event, float w = 1.);
};
#endif
