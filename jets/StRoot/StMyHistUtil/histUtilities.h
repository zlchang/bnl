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

TH1* normHistogram(TH1 *h, double ss = 1.0);
void drawHistRatio(TPad *p1, TPad *p2, TH1 *hdata, TH1 *hembed, TH1 *hratio, double xmin, double xmax, double ymin, double ymax, double zoom = 1.0, bool fnorm = true);
void drawHistRatio(TPad *p1, TPad *p2, TFile *fdata, TFile *fembed, TFile *fratio, char *name, double xmin, double xmax, double ymin, double ymax);
TH1* compareHistogram(TH1 *ha, TH1 *hb);
TH1D* compareProfile(TProfile *ha, TProfile *hb, bool flag);
TH1D* compareHistogram(TFile *fa, TFile *fb, char* name);
void addHistogram(TH1 *hist, TFile *f, char *name, double w = 1.);
void plotHist(TCanvas *c, TH1** h, char *option, int Ntrg);
void drawHist(TFile *f, char*name, const char*option = "");
void combineHist(TH1D *h, TProfile **he, TH1D **hd, int Ntrg = 3);
void printHist(TH1D *h, ofstream &of);

//NLO
TH1D* ratioHist(const char* name, TH1 *ha, TH1 *hb, bool flag = true);
//TH1D* ratioHistNorm(const char* name, TH1 *ha, TH1 *hb, bool flag = true);
TH1D* ratioHist(const char* name, TFile *ha, TFile *hb, bool flag = true);
TH1D* rawRatioHist(const char* name, TH1 *ha, TH1 *hb, bool flag = true);
TH1D *loadFiles(const char* file, double scale = 1.0);
void printToFile(TH1* ha, ofstream &of, int xbin = 1, int ybin = 1);
#endif
