#ifndef READPYTHIACRS
#define READPYTHIACRS
#include<vector>

#include "TObject.h"
#include "StMyJetPool/StMyJetSpin/MyJetYield.h"
class StJetEvent;
class StPythiaEvent;

//class MyJetYieldPar;
class StMyPDF;
//class MyJet;

class TFile;

class ReaderPythiaCrs : public TObject
{
  public:
  ReaderPythiaCrs(StJetEvent *parton = 0x0, StJetEvent *particle = 0x0, StPythiaEvent *pyevnt = 0x0);
  int Init();
  int Make(int iEvent);
  int Finish();
  void SetOutput(const char *output) { mOutput = output; }
  void fillMcEvent(StJetEvent *jetParEvent, MyJetYieldPar *jetYield, bool flag = false);
  void fillResponse(bool flag);
  void SetUE(bool flag) { mFlag = flag; }
 private:
  StJetEvent *mJetParticleEvent;
  StJetEvent *mJetPartonEvent;

  StPythiaEvent *mPythiaEvent;
  
  MyJetYieldPar *mJetYieldParton;
  MyJetYieldPar *mJetYieldParticle;

  //StMyPDF *mPdf;
  //StMyPDF *mPdfnn[MyJetYieldPar::NMod];
  bool mFlag;
  const char *mOutput;
  TFile *mFile;
  ClassDef(ReaderPythiaCrs, 0);
};

#endif
