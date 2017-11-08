#ifndef MYDEF
#define MYDEF

const int yNPtBins = 15;
const double yPtBins[yNPtBins+1] = {5.0, 6.0, 7.0, 8.2, 9.6, 11.2, 13.1, 15.3, 17.9, 20.9, 24.5, 28.7, 33.6, 39.3, 46.0, 53.8};
const int yNParPtBins = 15;
const double yParPtBins[yNParPtBins+1] = {0, 4, 4.8, 5.8, 7.0, 8.5,
                             10.2, 12.3, 14.8, 17.9, 21.6,
                             26.0, 31.4, 37.8, 45.6, 55.0};
enum status_t {yOk, ySkip, yError};
const int yNSEL = 3;
const int ySEL[yNSEL] = {2, 7, 12};
const int yNEtaBins = 2;
const double yEtaBins[yNEtaBins+1] = {0, 0.5, 0.9};
//const double yMinEta = -0.9;
//const double yMaxEta = 0.9;
//const int yNEtaBins = 30;
#endif
