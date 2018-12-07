#ifndef READEVENT
#define READEVENT
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
class MyJetSpin;
class MyJetQA;
class MyJetMC;
//class MyJetYield;
//class MyJetYieldPar;
class MyJetPar;
class MyJet;
class MyTrigQA;
class MyEventQA;
class TFile;
class TH2F;
//using namespace std;
class Reader : public TObject
{
  public:
  //Reader(StJetEvent *jetevnt, StJetEvent *parton, StJetEvent *particle, StJetSkimEvent *skimevnt, StUeEvent *ueocevnt, StUeEvent *particleueoc);
  Reader(StJetEvent *jetevnt = 0x0, StJetEvent *parton = 0x0, StJetEvent *particle = 0x0, StJetSkimEvent *skimevnt = 0x0, StUeOffAxisConesEvent *ueocevnt = 0x0, StUeOffAxisConesEvent *particleueoc = 0x0);
  int Init();
  int Make(int iEvent);
  //MyJetMC * GetJetMC(char *name) { return mMapJetMc[string(name)]; }
  int Finish();
  void SetBadBxFile(const char* file) { mBadBxFile  = file; }
  void SetOutput(const char *output) { mOutput = output; }
  void SetMC(bool flag) { mMcFlag = flag;}
  void SetUE(bool flag) { mFlag = flag;}
  //void fillMcEvent(StJetEvent *jetParEvent, MyJetPar *jetPar, MyJetYieldPar *jetYieldPar, float weight = 1.0);
  void fillMcEvent(StJetEvent *jetParEvent, MyJetPar *jetPar, float weight = 1.0);
  void readTrigMap(map<string, int> &mapTrigShouldFired, map<string, int> &mapTrigDidFired);
  void countMyJet(vector<MyJet*> vec, map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, int &numJet, MyJet *jetmax[], string trgmax[], bool flag = false);
  vector<MyJet*> findMyJet();

  void fillMyJetQA(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetQA*> mapJetQa, bool flag = false, float weight = 1.0);
  void fillMyJetSpin(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetSpin*>, MyJetSpin*, bool flag = false);
  //void fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetMC*> mapJetMc, map<string, MyJetYield*> mapJetYield, bool flag = false, float weight = 1.0);
  void fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetMC*> mapJetMc, bool flag = false, float weight = 1.0);
  void fillVtx(map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, float weight = 1.0);
  int countNPosVtx();
  float getVz();

 protected:
  //void fillParJet(TH2F *h, StJetEvent *evntpar, float weight = 1.0, bool flag = false);
  //void fillParTwoJet(TH2F *h, StJetEvent *evntpar, int index, float weight = 1.0, bool flag = false);
 private:
  StJetEvent *mJetEvent;
  StJetEvent *mJetParticleEvent;
  StJetEvent *mJetPartonEvent;
  StJetSkimEvent *mJetSkimEvent;
  //StUeEvent *mJetUeEvent;
  //StUeEvent *mJetParticleUeEvent;
  StUeOffAxisConesEvent *mJetUeEvent;
  StUeOffAxisConesEvent *mJetParticleUeEvent;

  map<string, MyJetMC*> mMapJetMc;
  //map<string, MyJetMC*> mMapJetMcCrr;

  MyJetPar *mJetParton;
  MyJetPar *mJetParticle;
  //
  //MyJetYieldPar *mJetYieldParton;
  //MyJetYieldPar *mJetYieldParticle;

  map<string, MyEventQA*> mMapEventQa;
  map<string, MyJetQA*> mMapJetQa;
  //map<string, MyJetQA*> mMapJetQaCrr;
  MyTrigQA *mTrig;
  //data
  map<string, MyJetSpin*> mMapJetSpin;
  MyJetSpin* mJetSpin;
  //jet yield
  //map<string, MyJetYield*> mMapJetYield;
  //correction
  //map<string, MyJetSpin*> mMapJetSpinCrr;
  //MyJetSpin* mJetSpinCrr;
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
  ClassDef(Reader, 0);
};

#endif
