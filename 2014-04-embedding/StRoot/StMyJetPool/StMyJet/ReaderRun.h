#ifndef READERRUN
#define READERRuN
//#include "TObject.h"
#include "ReaderVirtual.h"
#include<vector>
#include<map>
using namespace std;
class MyJetSum;
class MyTrigQA;
class TFile;

class ReaderRun : public ReaderVirtual
{
 public:
  ReaderRun(){
    mDir = 0x0;
    mDirRel = 0x0;
    mDirPol = 0x0;
    mJetSum = 0x0;
    mFile = 0x0;
    mFileIndex = 1;
    mTriggers.push_back("JP0");
    mTriggers.push_back("JP1");
    mTriggers.push_back("JP2");
  }
  int Init();
  int Make(int run);
  int Finish();

  void SetDir(const char *dir) { mDir = dir; }
  void SetFileIndex(int index) {mFileIndex = index; }
  void SetDirPol(const char *dir) { mDirPol = dir; }
  void SetDirRel(const char *dir) { mDirRel = dir; }
  void SetOutput(const char *output) { mOutput = output; }
 private:
  const char *mDir;
  const char *mDirRel;
  const char *mDirPol;
  const char *mOutput;
  int mFileIndex;
  TFile *mFile;
  vector<string> mTriggers;
  MyTrigQA *mTrigQA;
  map<string, MyJetSum*> mMapJetSum;
  MyJetSum *mJetSum;
  //map<string, MyJetSum*> mMapJetSumCrr;
  //MyJetSum *mJetSumCrr;
  ClassDef(ReaderRun, 0);
};
#endif
