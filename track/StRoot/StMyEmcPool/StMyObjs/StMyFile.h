#ifndef STMYFILE
#define STMYFILE
#include "TObject.h"
#include "TFile.h"

class StMyHist2D;
class StMyProfile;
class TH1D;
class TH2D;
class TProfile;

class StMyFile : public TObject{
 public:
  StMyFile(){ mFile = 0x0; }
  StMyFile(TFile *fl) { mFile = fl; }
  void AddHist(StMyHist2D *hist, const char* name, double w = 1.0);
  void AddHist(StMyProfile *hist, const char* name, double w = 1.0);
  void AddHist(TH1D *hist, const char* name, double w = 1.0);
  void AddHist(TH2D *hist, const char* name, double w = 1.0);
  void AddHist(TProfile *hist, const char* name, double w = 1.0);

 private:
  TFile *mFile;
  ClassDef(StMyFile, 0);
};
#endif
