#include "StMyMatchTrackToEmcFile.h"
#include "StMyMatchTrackToEmcHist.h"

void StMyMatchTrackToEmcFile::Add(StMyMatchTrackToEmcHist *pp, char *name, double ww)
{
  AddHist(pp->mHist, Form("%sTrack", name), ww);
  AddHist(pp->mHistEta, Form("%sTrackEta", name), ww);
  AddHist(pp->mHistPhi, Form("%sTrackPhi", name), ww);
  AddHist(pp->mHistFrac, Form("%sTrackFrac", name), ww);
  AddHist(pp->mHistNHits, Form("%sTrackNHits", name), ww);
  AddHist(pp->mHistNHitsFitRatio, Form("%sTrackNHitsFitRatio", name), ww);
  AddHist(pp->mHistDcaPt, Form("%sTrackDcaPt", name), ww);
  //AddHist(pp->mHistTower, Form("%sTower", nn), ww);
  AddHist(pp->mHistNSigmaElectron, Form("%sNSigmaElectron", name), ww);
  AddHist(pp->mHistNSigmaPion, Form("%sNSigmaPion", name), ww);
  AddHist(pp->mHistNSigmaKaon, Form("%sNSigmaKaon", name), ww);
  AddHist(pp->mHistNSigmaProton, Form("%sNSigmaProton", name), ww);
  AddHist(pp->mHistEpVsPt, Form("%sEpVsPt", name), ww);
  AddHist(pp->mHistEpVsPtCluster, Form("%sEpVsPtCluster", name), ww);
  //hit frac
  AddHist(pp->mHistHitTowerFracCluster, Form("%sHitTowerFracCluster", name), ww);
  //max frac
  AddHist(pp->mHistMaxTowerFracCluster, Form("%sMaxTowerFracCluster", name), ww);
  //
  AddHist(pp->mHistEpVsDist, Form("%sEpVsDist", name), ww);
  AddHist(pp->mHRadius, Form("%sRadius", name), ww);
  AddHist(pp->mHistMult, Form("%sTrackMult", name), ww);
}
