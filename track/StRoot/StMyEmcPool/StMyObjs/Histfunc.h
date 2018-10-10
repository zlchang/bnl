#ifndef HISTFUNC
#define HISTFUNC
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

#include "StMyHist.h"

extern TFile *yFile;

void AddHist(StMyHist2D *hist, const char* name, double w = 1.0);
void AddHist(StMyProfile *hist, const char* name, double w = 1.0);
void AddHist(TH1D *hist, const char* name, double w = 1.0);
void AddHist(TH2D *hist, const char* name, double w = 1.0);
void AddHist(TProfile *hist, const char* name, double w = 1.0);

#endif
