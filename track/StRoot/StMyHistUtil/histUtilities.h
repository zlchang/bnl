#ifndef HIST_UTILITIES
#define HIST_UTILITIES
#include<iostream>
#include<fstream>
#include<iomanip>

using namespace std;

class TFile;
class TCanvas;
class TPad;
class TH1;
class TH1D;
class TProfile;

void drawHistRatio(TPad *p1, TPad *p2, TH1 *hdata, TH1 *hembed, TH1 *hratio, float xmin, float xmax, float ymin, float ymax, float zoom = 1.0, bool fnorm = true);
void drawHistRatio(TPad *p1, TPad *p2, TFile *fdata, TFile *fembed, TFile *fratio, char *name, float xmin, float xmax, float ymin, float ymax);
void compareHistogram(TH1 *hist, TH1 *ha, TH1 *hb);
void compareProfile(TH1D *hist, TProfile *ha, TProfile *hb);
void compareHistogram(TH1D *hist, TFile *fa, TFile *fb, char* name);
void addHistogram(TH1 *hist, TFile *f, char *name, float w = 1.);
void plotHist(TCanvas *c, TH1** h, char *option, int Ntrg);
void drawHist(TFile *f, char*name, const char*option = "");
void combineHist(TH1D *h, TProfile **he, TH1D **hd, int Ntrg = 3);
void printHist(TH1D *h, ofstream &of);

//NLO
TH1D* ratioHist(const char* name, TH1 *ha, TH1 *hb, bool flag = true);
//TH1D* ratioHistNorm(const char* name, TH1 *ha, TH1 *hb, bool flag = true);
TH1D* ratioHist(const char* name, TFile *ha, TFile *hb, bool flag = true);
TH1D* rawRatioHist(const char* name, const TH1 *ha, const TH1 *hb, bool flag = true);
TH1D *loadFiles(const char* file, float scale = 1.0);
void printToFile(TH1* ha, ofstream &of, int xbin = 1, int ybin = 1);
#endif
