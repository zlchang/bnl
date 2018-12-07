#ifndef READCRSEVENT
#define READCRSEVENT
#include<vector>
#include<map>
#include "TObject.h"
//#include "StMyJetPool/StMyDef/MyDef.h"
#include "TString.h"
//#include "MyJet.h"
#include "StMyJetPool/StMyJetSpin/MyJetYield.h"
class StJetEvent;
class StJetSkimEvent;
class StUeOffAxisConesEvent;
class MyJetQA;
class MyJetRes2D;
//class MyJetYield;
//class MyJetYieldPar;
class MyJetTest;
class MyJetTestPar;
class MyJet;
class MyTrigQA;
class MyEventQA;
class TFile;
class TH1F;
class TH2F;

class StMyPDF;
//using namespace std;
//
//typedef StUeOffAxisConesEvent StUeEvent;
class ReaderCrs : public TObject
{
  public:
  //ReaderCrs(StJetEvent *jetevnt = 0x0, StJetEvent *parton = 0x0, StJetEvent *particle = 0x0, StJetSkimEvent *skimevnt = 0x0, StUeEvent *ueocevnt = 0x0, StUeEvent *particleueoc = 0x0);
  ReaderCrs(StJetEvent *jetevnt = 0x0, StJetEvent *parton = 0x0, StJetEvent *particle = 0x0, StJetSkimEvent *skimevnt = 0x0, StUeOffAxisConesEvent *ueocevnt = 0x0, StUeOffAxisConesEvent *particleueoc = 0x0);
  int Init();
  int Make(int iEvent);
  //MyJetMC * GetJetMC(char *name) { return mMapJetMc[string(name)]; }
  int Finish();
  void SetBadBxFile(const char* file) { mBadBxFile  = file; }
  void SetOutput(const char *output) { mOutput = output; }
  void SetMC(bool flag) { mMcFlag = flag;}
  void SetUE(float flag) { mFlag = flag;}
  void SetSmear(float flag) { mFlagSmear = flag;}
  void fillMcEvent(StJetEvent *jetParEvent, MyJetYieldPar *jetYieldPar, bool flag, float weight = 1.0);
  void readTrigMap(map<string, int> &mapTrigShouldFired, map<string, int> &mapTrigDidFired);
  void countMyJet(vector<MyJet*> vec, map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, int &numJet, MyJet *jetmax[], string trgmax[], float flag = 0, float smear = 0);
  vector<MyJet*> findMyJet();

  bool fillMyJetVtx(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  bool fillMyJetQA(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  bool fillMyJetTest(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  bool fillMyJetYield(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  bool fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], float weight = 1.0);
  
  void fillVtx(map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, float weight = 1.0);
  int countNPosVtx();
  float getVz();
  
 protected:
  bool fillMyJetRes2D(int numJet, MyJet *jetmax[], string trgmax[], float weight, StJetEvent *par, bool flag, map<string, MyJetRes2D*> mapJetRes, MyJetRes2D *cmbJetRes);
  void fillParJet(TH2F *h, StJetEvent *evntpar, float weight = 1.0, bool flag = false);
  //void fillParTwoJet(TH2F *h, StJetEvent *evntpar, int index, float weight = 1.0, bool flag = false);
  //void fillParTwoJet(TH1F *h, StJetEvent *evntpar, int index, float weight = 1.0, bool flag = false);
 private:
  StJetEvent *mJetEvent;
  StJetEvent *mJetParticleEvent;
  StJetEvent *mJetPartonEvent;
  StJetSkimEvent *mJetSkimEvent;
  StUeOffAxisConesEvent *mJetUeEvent;
  StUeOffAxisConesEvent *mJetParticleUeEvent;
  //
  MyJetYieldPar *mJetYieldParton;
  MyJetYieldPar *mJetYieldParticle;
  //
  MyJetTestPar *mJetTestParton;
  MyJetTestPar *mJetTestParticle;
  
  map<string, MyEventQA*> mMapEventQa;
  map<string, MyJetQA*> mMapJetQa;

  MyTrigQA *mTrig;
    
  //jet yield
  map<string, MyJetYield*> mMapJetYield;
  MyJetYield* mCmbJetYield;
  //
  map<string, MyJetTest*> mMapJetTest;
  MyJetTest* mCmbJetTest;
  //response
  map<string, MyJetRes2D*> mMapJetPartonRes;
  MyJetRes2D* mCmbJetPartonRes;

  map<string, MyJetRes2D*> mMapJetParticleRes;
  MyJetRes2D* mCmbJetParticleRes;
  
  //info
  //int mRun;
  bool mMcFlag;
  float mFlag;
  float mFlagSmear;
  const char *mBadBxFile;
  vector<int> mBadBx;
  vector<string> mTriggers;
  TString mOption;
  const char *mOutput;
  TFile *mFile;
  //StMyPDF *mPdf;
  //StMyPDF *mPdfnn[MyJetYieldPar::NMod];
  ClassDef(ReaderCrs, 0);
};

#endif
