#include "StMyMatchTrackToEmcMaker.h"
#include "StMyTrackFlagCut.h"
#include "StMyTrackFtpcCut.h"
#include "StMyTrackHitsCut.h"
#include "StMyTrackDcaCut.h"
#include "StMyTrackDcaPtCut.h"
#include "StMyTrackPtCut.h"
#include "StMyTrackEtaCut.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyMatchTrackToEmcHist.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyTowerHist.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyClusterHist.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyTrack.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyTower.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyCluster.h"

#include "TFile.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TH1D.h"
#include "TH2D.h"

#include "StEventTypes.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include <vector>
#include <map>
int StMyMatchTrackToEmcMaker::Init()
{
  mFile = new TFile(mFileName, "recreate");
  mHistNoCut = new StMyMatchTrackToEmcHist("NoCut");
  mHist = new StMyMatchTrackToEmcHist("All");
  mHistPos = new StMyMatchTrackToEmcHist("Pos");
  mHistNeg = new StMyMatchTrackToEmcHist("Neg");

  mHistTower = new StMyTowerHist("Tower");
  mHistCluster = new StMyClusterHist("Cluster");

  mTrackCuts.push_back(new StMyTrackFlagCut());
  mTrackCuts.push_back(new StMyTrackFtpcCut());
  mTrackCuts.push_back(new StMyTrackHitMinCut());
  mTrackCuts.push_back(new StMyTrackHitFracCut());
  mTrackCuts.push_back(new StMyTrackDcaCut());
  mTrackCuts.push_back(new StMyTrackDcaPtCut());
  mTrackCuts.push_back(new StMyTrackPtMinCut());
  mTrackCuts.push_back(new StMyTrackEtaMinCut());
  mTrackCuts.push_back(new StMyTrackEtaMaxCut());
  
  mBemcGeom = StEmcGeom::instance("bemc");
  return StMaker::Init();
}
int StMyMatchTrackToEmcMaker::Make()
{
  StMuPrimaryVertex *vertex = StMuDst::primaryVertex();
  if(!vertex) return kStSkip;
  if(vertex->ranking() < 0.1) return kStSkip;
  if(TMath::Abs(vertex->position().z()) > 30.) return kStSkip;
  double mag = StMuDst::event()->magneticField()/10.0;

  vector<StMyTower> myTowerList(4800);

  StEmcCollection *emc = StMuDst::emcCollection();
  StEmcDetector* bemc = emc->detector(kBarrelEmcTowerId);
  if (bemc) {
    for(unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
      StSPtrVecEmcRawHit& rawHits = bemc->module(m)->hits();
      for(size_t k = 0; k < rawHits.size(); ++k) {
	StEmcRawHit *hit = rawHits[k];
	int towerId = 0;
	mBemcGeom->getId(hit->module(), hit->eta(), abs(hit->sub()), towerId);
	if(towerId >=1 && towerId <= 4800){
          double he = hit->energy();
 	  if(he < 0.2) he = 0;
	  (myTowerList[towerId-1]).mId = towerId;
	  (myTowerList[towerId-1]).mE = he;
          //Printf("ee = %lf\n", he);
	}
      }
    }
  }

  vector<const StMuTrack*> muTrackList;
  for(unsigned int iTrack = 0; iTrack < StMuDst::numberOfPrimaryTracks(); iTrack++)
    {
      const StMuTrack* muTrack = StMuDst::primaryTracks(iTrack);
      if(!muTrack) continue;
      bool flag = false;
      for(vector<StMyTrackCut*>::iterator icut = mTrackCuts.begin(); icut != mTrackCuts.end(); icut++){
	StMyTrackCut* trackcut = *icut;
	if((*trackcut)(muTrack)) {flag = true; break;}
	///trackcut->operator(muTrack);
      }
      if(!flag) muTrackList.push_back(muTrack);
    }
  Printf("MuDst track size: %d", muTrackList.size());
  //
  vector<StMyTrackMatch> myTrackList;
  for(vector<const StMuTrack*>::const_iterator it = muTrackList.begin(); it != muTrackList.end(); it++){
    const StMuTrack *muTrack = *it;
    double pt = muTrack->pt();
    double eta = muTrack->eta();

    StMyTrackMatch myTrack;
    myTrack.mPt = pt;
    //
    double nsigE = muTrack->nSigmaElectron();
    myTrack.mNSigmaElectron = nsigE;
    
    double nsigPi = muTrack->nSigmaPion();
    myTrack.mNSigmaPion = nsigPi;
    
    int charge = muTrack->charge();
    myTrack.mCharge = charge;
    
    //Printf("pt = %.2lf B = %.2lf", pt, mag);
    StThreeVectorD momentumAt, positionAt;
    //StMuEmcPosition EmcPosition;
    StEmcPosition EmcPosition;
    double bemcRadius = 238.6;
    //double bemcRadius = 245;
    //double bemcRadius = 225.405;
    if(EmcPosition.trackOnEmc(&positionAt, &momentumAt, muTrack, mag, bemcRadius)){
      double exitEta = positionAt.pseudoRapidity();
      double exitPhi = positionAt.phi();
      int exitTowerId = 0;
      //Printf("tower Id %d, exit eta: %.2lf, phi: %.2lf", exitTowerId, exitEta, exitPhi);
      //StEmcGeom::instance("bemc")->getId(exitPhi, exitEta, exitTowerId);
      mBemcGeom->getId(exitPhi, exitEta, exitTowerId);
      
      if(exitTowerId <=0 || exitTowerId > 4800){ 
	Printf("exitTowerId %d out of range\n", exitTowerId);
	//hprofmatch->Fill(pt, false); 
	continue;
      }
      (myTowerList[exitTowerId-1].mHits)++;
      //hprofmatch->Fill(pt, true);
      //double ee = myTowerList[exitTowerId-1].mE;//energy[exitTowerId-1];

      //Printf("tower Id %d, energy %lf, track pt %.2lf eta %.2lf", exitTowerId, ee, pt, eta);
      float beta, bphi;
      mBemcGeom->getEtaPhi(exitTowerId, beta, bphi);
      //Printf("tower Id %d, center eta: %.2lf, phi: %.2lf", exitTowerId, beta, bphi);
      double dd = TMath::Power(beta-exitEta, 2)+TMath::Power(TVector2::Phi_mpi_pi(bphi-exitPhi), 2);
      dd = TMath::Sqrt(dd);
      //hprofevsd->Fill(dd, ee/pt);
      //h2devsd->Fill(dd, ee/pt);
      //
      myTrack.mTower = myTowerList[exitTowerId-1];
      myTrack.mDist = dd;
      myTrack.mMatch = true;
    }else{
      myTrack.mMatch = false;
      Printf("no match for track pt %.2lf eta %.2lf", pt, eta);
    }
    myTrackList.push_back(myTrack);
  }
  map<int, StMyCluster> myClusterList;
  for(vector<StMyTrackMatch>::iterator it = myTrackList.begin(); it != myTrackList.end(); it++){
    StMyTrackMatch &myTrack = *it;
    if(!myTrack.mMatch) continue;
    int exitTowerId = myTrack.mTower.mId;
    //looking for neighboring tower
    StMyCluster &myCluster = myTrack.mCluster;
    myCluster.mId = exitTowerId;
    StEmcPosition EmcPosition;
    double nsum = 0, emax = 0;
    int nhits = 0;
    for(int ieta = -1; ieta <= 1; ieta++){
      for(int iphi = -1; iphi <=1; iphi++){
	int ntowerId = EmcPosition.getNextTowerId(exitTowerId, ieta, iphi);
	
	if(ntowerId <=0 || ntowerId > 4800) {Printf("neighboring towerId invalid Id: %d\n", ntowerId); continue;}
	nhits += myTowerList[ntowerId-1].mHits;
	double nee = (myTowerList[ntowerId-1]).mE;
	if(nee > emax) emax = nee;
	nsum += nee;
	//Printf("ntowerId=%d, exitTowerId=%d, nee=%4.3lf, et=%4.3lf\n", ntowerId, exitTowerId, nee, ee);	
      }
    }
    myCluster.mE = nsum;
    myCluster.mEMax = emax;
    myCluster.mHits = nhits;

    if(myClusterList.find(exitTowerId) == myClusterList.end()){
      myClusterList.insert(pair<int, StMyCluster>(exitTowerId, myCluster));
    }
  }
  
  //fill tower histograms
  for(vector<StMyTower>::const_iterator it = myTowerList.begin(); it != myTowerList.end(); it++){
    const StMyTower &myTower = *it;
    fillHistTower(myTower, mHistTower);
  }
  //fill histograms
  for(vector<StMyTrackMatch>::const_iterator it = myTrackList.begin(); it != myTrackList.end(); it++){
    const StMyTrackMatch &myTrack = *it;
    
    fillHist(myTrack, mHistNoCut);	

    int charge = myTrack.mCharge;
    double nsigE = myTrack.mNSigmaElectron;
    if(nsigE < -2. || nsigE > 2.){
      if(charge > 0){
	fillHist(myTrack, mHistPos);
      }
      if(charge < 0){
	fillHist(myTrack, mHistNeg);
      }
      fillHist(myTrack, mHist);
    }    
  }
  //fill cluster histogram
  for(map<int, StMyCluster>::const_iterator it = myClusterList.begin(); it != myClusterList.end(); it++){
    //int Id = it->first;
    const StMyCluster &myCluster = it->second;
    fillHistCluster(myCluster, mHistCluster);
  }
  return StMaker::Make();
}
int StMyMatchTrackToEmcMaker::Finish()
{
  mFile->Write();
  mFile->Close();
  return StMaker::Finish();
}
void StMyMatchTrackToEmcMaker::fillHist(const StMyTrackMatch &track, StMyMatchTrackToEmcHist *hist)
{
  double tpt = track.mPt;
  double tnsigE = track.mNSigmaElectron;
  double tnsigPi = track.mNSigmaPion;
  bool match = track.mMatch;
  //Printf("match = %d\n", match);
  hist->mHistTrack->Fill(tpt);
  hist->mHistNSigmaElectron->Fill(tnsigE);
  hist->mHistNSigmaPion->Fill(tnsigPi);
  hist->mHistTrackFrac->Fill(tpt, match);
  if(match){
    double tee = track.mTower.mE;
    double tdd = track.mDist;
    double tcluster = track.mCluster.mE;
    double tclustermax = track.mCluster.mEMax;
    double tfee = 0;
    if(tcluster > 0) tfee = tee/tcluster;
    double tfmax = 0;
    if(tcluster > 0) tfmax = tclustermax/tcluster;    
    hist->mHistTower->Fill(tee);
    hist->mHistEptVsPt->Fill(tpt, tee/tpt);
    int nhits = track.mCluster.mHits;
    //Printf("nhits = %d\n", nhits);
    //isolation cut
    if(nhits == 1){
      hist->mHistEptVsPtCluster->Fill(tpt, tcluster/tpt);
      hist->mHistHitTowerFracCluster->Fill(tpt, tfee);
      hist->mHistMaxTowerFracCluster->Fill(tpt, tfmax);
    }
    hist->mHistEptVsDist->Fill(tdd, tee/tpt);
  }
}
void StMyMatchTrackToEmcMaker::fillHistTower(const StMyTower &tower, StMyTowerHist *hist)
{
    hist->hE->Fill(tower.mE);
    hist->hNHits->Fill(tower.mHits);
    hist->hHits->Fill(tower.mId);
}
void StMyMatchTrackToEmcMaker::fillHistCluster(const StMyCluster &cluster, StMyClusterHist *hist)
{
    double ec = cluster.mE;
    hist->hE->Fill(ec);
    hist->hNHits->Fill(cluster.mHits);
    double fmax = 0;
    double emaxc = cluster.mEMax;
    if(ec > 0) fmax = emaxc/ec;
    hist->hMaxFrac->Fill(fmax);
}
