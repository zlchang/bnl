#ifndef MYDEF
#define MYDEF

//const int yNPtBins = 15;
//const double yPtBins[yNPtBins+1] = {5.0, 6.0, 7.0, 8.2, 9.6, 11.2, 13.1, 15.3, 17.9, 20.9, 24.5, 28.7, 33.6, 39.3, 46.0, 53.8};
//const int yNParPtBins = 15;
//const double yParPtBins[yNParPtBins+1] = {0, 4, 4.8, 5.8, 7.0, 8.5,
//                             10.2, 12.3, 14.8, 17.9, 21.6,
//                             26.0, 31.4, 37.8, 45.6, 55.0};
const int yNPtBins = 20;
const double yPtBins[yNPtBins+1] = {5.0, 6.0, 7.02, 8.21, 9.61, 11.24, 13.15, 15.39,
                                  18.01, 21.07, 24.65, 28.84, 33.74, 39.48, 46.19,
			          54.04, 63.23, 73.98, 86.56, 101.27, 118.49};
const int yNParPtBins = 20;
const double yParPtBins[yNParPtBins+1] = {0, 6, 7.08, 8.35, 9.86, 11.63, 13.73, 16.20,
					  19.11, 22.55, 26.61, 31.40, 37.06, 43.73, 51.60,
					  60.88, 71.84, 84.77, 100.03, 118.04, 139.29};
enum status_t {yOk, ySkip, yError};
const int yNSEL = 3;
const int ySEL[yNSEL] = {2, 7, 12};
const int yNEtaBins = 2;
const double yEtaBins[yNEtaBins+1] = {0, 0.5, 0.9};

//yPtBins[0] = 5; yPtBins[1] = 6;
//for(int ip = 2; ip < yNPtBins; ip++){
    //ptbins[ip] = ptbins[ip-1]*1.17;
//}
 
//const double yMinEta = -0.9;
//const double yMaxEta = 0.9;
//const int yNEtaBins = 30;
//
//const int yNTRG = 3;
//const char *yTRGNAME[yNTRG] = {"JP0", "JP1", "JP2"};
//const int yNPAR = 2;
//const char *yPARNAME[yNPAR] = {"Parton", "Particle"};
const int yNproc = 3;
int indexDet(double pt, double eta);
int indexPar(double pt, double eta);

#endif
