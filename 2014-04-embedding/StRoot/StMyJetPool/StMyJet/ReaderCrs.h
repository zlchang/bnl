#ifndef READCRSEVENT
#define READCRSEVENT
#include<vector>
#include<map>
#include "TObject.h"
//#include "StMyJetPool/StMyDef/MyDef.h"
#include "TString.h"
//#include "MyJet.h"
class StJetEvent;
class StJetSkimEvent;
//class StUeEvent;
class StUeOffAxisConesEvent;
class MyJetQA;
class MyJetYield;
class MyJetYieldPar;
class MyJet;
class MyTrigQA;
class MyEventQA;
class TFile;
class TH2F;

//using namespace std;
class ReaderCrs : public TObject
{
  public:
  //ReaderCrs(StJetEvent *jetevnt, StJetEvent *parton, StJetEvent *particle, StJetSkimEvent *skimevnt, StUeEvent *ueocevnt);
  ReaderCrs(StJetEvent *jetevnt = 0x0, StJetEvent *parton = 0x0, StJetEvent *particle = 0x0, StJetSkimEvent *skimevnt = 0x0, StUeOffAxisConesEvent *ueocevnt = 0x0, StUeOffAxisConesEvent *particleueoc = 0x0);
  int Init();
  int Make(int iEvent);
  //MyJetMC * GetJetMC(char *name) { return mMapJetMc[string(name)]; }
  int Finish();
  void SetBadBxFile(const char* file) { mBadBxFile  = file; }
  void SetOutput(const char *output) { mOutput = output; }
  void SetMC(bool flag) { mMcFlag = flag;}
  void SetUE(bool flag) { mFlag = flag;}
  void fillMcEvent(StJetEvent *jetParEvent, MyJetYieldPar *jetYieldPar, bool flag, float weight = 1.0);
  void readTrigMap(map<string, int> &mapTrigShouldFired, map<string, int> &mapTrigDidFired);
  void countMyJet(vector<MyJet*> vec, map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, int &numJet, MyJet *jetmax[], string trgmax[], bool flag = false);
  vector<MyJet*> findMyJet();

  void fillMyJetQA(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  
  void fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  void fillVtx(map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, float weight = 1.0);
  int countNPosVtx();
  float getVz();
  
 protected:
  void fillParJet(TH2F *h, StJetEvent *evntpar, float weight = 1.0, bool flag = false);
  void fillParTwoJet(TH2F *h, StJetEvent *evntpar, int index, float weight = 1.0, bool flag = false);
 private:
  StJetEvent *mJetEvent;
  StJetEvent *mJetParticleEvent;
  StJetEvent *mJetPartonEvent;
  StJetSkimEvent *mJetSkimEvent;
  //StUeEvent *mJetUeEvent;
  StUeOffAxisConesEvent *mJetUeEvent;
  StUeOffAxisConesEvent *mJetParticleUeEvent;
  //
  MyJetYieldPar *mJetYieldParton;
  MyJetYieldPar *mJetYieldParticle;
  
  map<string, MyEventQA*> mMapEventQa;
  map<string, MyJetQA*> mMapJetQa;

  MyTrigQA *mTrig;
    
  //jet yield
  map<string, MyJetYield*> mMapJetYield;
  MyJetYield* mCmbJetYield;
  
  //info
  //int mRun;
  bool mMcFlag;
  bool mFlag;
  const char *mBadBxFile;
  vector<int> mBadBx;
  vector<string> mTriggers;
  TString mOption;
  const char *mOutput;
  TFile *mFile;
  ClassDef(ReaderCrs, 0);
};

#endif
