#ifndef READERVIRTUAL
#define READERVIRTUAL
#include<vector>
#include<string>
using namespace std;
#include "TObject.h"
#include "TString.h"
#include "StMyJetPool/StMyDef/MyDef.h"

class ReaderVirtual : public TObject{
 public:
  virtual int Init(){ return yOk;}
  virtual int Make(int i){ return yOk; }
  virtual int Finish(){ return yOk; }
 private:
  ClassDef(ReaderVirtual, 0);
};
#endif
