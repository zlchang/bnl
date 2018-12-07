#ifndef TRG_UTILITIES
#define TRG_UTILITIES
#include<vector>
#include<map>
#include<iostream>
#include<string>

using namespace std;

class StJetCandidate;
class StJetSkimEvent;
class StJetEvent;
class MyJet;

float smearPtFrac(float rt);
bool check_badbx(int bx, vector<int> badbunch);
bool check_spinbit(int spinbit);

StJetCandidate *FindParJet(StJetCandidate* jet, StJetEvent *evntpar, int &index, float range, int &counter);
float DeltaR(float etaA, float phiA, float etaB, float phiB);
//merge sort
//string JetCategory(StJetCandidate *jetCnd, StJetSkimEvent *skimevnt, int th_plusi = 0);
string JetCategory(MyJet *myjet, StJetSkimEvent *skimevnt, map<string, int>mapTrigShouldFire, map<string, int>mapTrigDidFire, float flag = 0, float smear = 0, int th_plus = 0);
vector<MyJet*> mergesort(vector<MyJet*>, bool flag = false);
vector<MyJet*> merge(vector<MyJet*> a, vector<MyJet*> b, bool flag = false);
//JP triggers
bool MatchJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
bool MatchBarrelJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
bool MatchEndcapJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
bool MatchOverlapJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
//AJP trigger
bool MatchAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
bool MatchBarrelAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
bool MatchEndcapAdjacentJetPatch(StJetCandidate *jetcnd, StJetSkimEvent *skim, int th, int shift = 0);
void GetBarrelAdjacentJetPatchEtaPhi(StJetCandidate *jetcnd, int i, float &eta, float &phi);
void GetEndcapAdjacentJetPatchEtaPhi(StJetCandidate *jetcnd, int i, float &eta, float &phi);

bool HitJetPatch(float deteta, float phi, float jpeta, float jpphi);

int checkpt(float pt);
//
#endif
