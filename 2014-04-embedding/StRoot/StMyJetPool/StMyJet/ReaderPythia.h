#ifndef READPYTHIA
#define READPYTHIA
#include<vector>

#include "TObject.h"

class StJetEvent;
class StPythiaEvent;

class MyJetPar;
class MyJetYieldPar;
//class MyJet;

class TFile;

class ReaderPythia : public TObject
{
  public:
  ReaderPythia(StJetEvent *parton = 0x0, StJetEvent *particle = 0x0, StPythiaEvent *pyevnt = 0x0);
  int Init();
  int Make(int iEvent);
  int Finish();
  void SetOutput(const char *output) { mOutput = output; }
  void fillMcEvent(StJetEvent *jetParEvent, MyJetPar *jetPar, MyJetYieldPar *jetYield, bool flag = false);
  
 private:
  StJetEvent *mJetParticleEvent;
  StJetEvent *mJetPartonEvent;

  StPythiaEvent *mPythiaEvent;
  
  MyJetPar *mJetParton;
  MyJetPar *mJetParticle;
  MyJetYieldPar *mJetYieldParton;
  MyJetYieldPar *mJetYieldParticle;

  const char *mOutput;
  TFile *mFile;
  ClassDef(ReaderPythia, 0);
};

#endif
