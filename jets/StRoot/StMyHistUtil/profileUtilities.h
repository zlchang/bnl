#ifndef PROFILE_UTILITIES
#define PROFILE_UTILITIES

class TH1D;
class TProfile;
class TFile;

TH1D *convertTProfile(TProfile *hw, TProfile *hw2, const char* ver = "A");
TH1D *convertTProfile(TFile *f, const char*name, const char*ver = "A");

#endif
