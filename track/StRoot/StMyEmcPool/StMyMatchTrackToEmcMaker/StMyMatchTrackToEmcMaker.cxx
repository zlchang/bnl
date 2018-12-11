#include "StMyMatchTrackToEmcMaker.h"
#include "StMyEmcPool/StMyUtils/StMyTrackFlagCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackFtpcCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackHitsCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackDcaCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackDcaPtCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackPtCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackEtaCut.h"
#include "StMyEmcPool/StMyUtils/StMyTrackLastpointCut.h"
#include "StMyEmcPool/StMyUtils/StMyVertexCut.h"
#include "StMyEmcPool/StMyUtils/StMyVertexZCut.h"
#include "StMyEmcPool/StMyUtils/StMyVertexRankingCut.h"
#include "StMyEmcPool/StMyUtils/func.h"
#include "StMyEmcPool/StMyUtils/StMyTrackProjEmc.h"

#include "StMyEmcPool/StMyMatchTrackToEmcHist/StMyMatchTrackToEmcHist.h"
#include "StMyEmcPool/StMyObjs/StMyTrackMatch.h"
#include "StMyEmcPool/StMyObjs/StMyTower.h"
#include "StMyEmcPool/StMyObjs/StMyCluster.h"
#include "StMyEmcPool/StMyObjs/SFfunc.h"
#include "StMyEmcPool/StMyObjs/StMyTowerHist.h"
#include "StMyEmcPool/StMyObjs/StMyClusterHist.h"

#include "TFile.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TH1D.h"
#include "TH2D.h"

#include "StEventTypes.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
#include "StTriggerUtilities/StTriggerSimuMaker.h"
#include "StTriggerUtilities/Emc/StEmcTriggerSimu.h"
#include "StTriggerUtilities/Bemc/StBemcTriggerSimu.h"
#include "StTriggerUtilities/Eemc/StEemcTriggerSimu.h"

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
  mTrackCuts.push_back(new StMyTrackLastpointCut());
  mTrackCuts.push_back(new StMyTrackDcaPtCut());
  mTrackCuts.push_back(new StMyTrackPtMinCut());
  mTrackCuts.push_back(new StMyTrackEtaMinCut());
  mTrackCuts.push_back(new StMyTrackEtaMaxCut());
  
  mVertexCuts.push_back(new StMyVertexRankingCut());

  mBemcGeom = StEmcGeom::instance("bemc");

  StEmcADCtoEMaker* emc2e = static_cast<StEmcADCtoEMaker*> ( GetMakerInheritsFrom("StEmcADCtoEMaker") );
  mBemcTable = emc2e->getBemcData()->getTables();
  return StMaker::Init();
}
int StMyMatchTrackToEmcMaker::Make()
{

  StTriggerSimuMaker *trgsim = (StTriggerSimuMaker*) GetMakerInheritsFrom("StTriggerSimuMaker");  if(!trgsim->emc->JP0()) return kStSkip;	
  StMuPrimaryVertex *vertex = StMuDst::primaryVertex();
  if(!vertex) return kStSkip;
  //if(vertex->ranking() < 0.1) return kStSkip;
  //if(TMath::Abs(vertex->position().z()) > 30.) return kStSkip;
  bool vflag = false;
  for(vector<StMyVertexCut*>::const_iterator iv = mVertexCuts.begin(); iv != mVertexCuts.end(); iv++){
     StMyVertexCut *vcut = *iv;
     if((*vcut)(vertex)){ vflag = true; break;}
  }
  if(vflag) return kStSkip;
  //Printf("z-vertex: %lf ranking: %lf\n", vertex->position().z(), vertex->ranking());
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
	  int status = 0;
          mBemcTable->getStatus(1, towerId, status);
          //unsigned int adc = hit->adc();
	  //float pedestal, rms;
          //int CAP = 0;
	  //mBemcTable->getPedestal(1, towerId, CAP, pedestal, rms);
          //if(adc < pedestal+4){ 
            //Printf("adc = %d pedestal = %f\n", adc, pedestal); 
            //he = 0;
          //}
          //if(adc < pedestal+3.0*rms){ 
            //Printf("adc = %d pedestal = %f rms = %f\n", adc, pedestal, rms); 
            //he = 0;
          //}
          //if(mBemcTable->status(1, towerId, "calib") != 1) status = 0;
          //if(status != 1) {
            //Printf("id: %d status: %d\n", towerId, status); 
            //he = 0;
          //}
          myTowerList[towerId-1].mStatus = (status == 1); 
 	  //if(he < 0.2) he = 0;
 	  if(he < 0) { //Printf("Setting tower %d energy %f to 0\n", towerId, he); 
                     he = 0;}
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
    double phi = muTrack->phi();

    StMyTrackMatch myTrack;
    myTrack.mPt = pt;
    myTrack.mEta = eta;
    myTrack.mPhi = phi;
    //
    double nsigE = muTrack->nSigmaElectron();
    myTrack.mNSigmaElectron = nsigE;
    
    double nsigPi = muTrack->nSigmaPion();
    myTrack.mNSigmaPion = nsigPi;

    double nsigK = muTrack->nSigmaKaon();
    myTrack.mNSigmaKaon = nsigK;
    
    double nsigP = muTrack->nSigmaProton();
    myTrack.mNSigmaProton = nsigP;
    
    int charge = muTrack->charge();
    myTrack.mCharge = charge;
    
    //Printf("pt = %.2lf B = %.2lf", pt, mag);
    //StThreeVectorD momentumAt, positionAt;
    //StEmcPosition EmcPosition;
    //double bemcRadius = getRadiusLinear(pt);
    //double bemcRadius = 225.405, depth = 30;
    //Printf("Using bemc radius %lf to project towers\n", bemcRadius);
    //if(EmcPosition.trackOnEmc(&positionAt, &momentumAt, muTrack, mag, bemcRadius)){
    //double exitEta = positionAt.pseudoRapidity();
      //double exitPhi = positionAt.phi();
      //int exitTowerId = 0;
      //Printf("tower Id %d, exit eta: %.2lf, phi: %.2lf", exitTowerId, exitEta, exitPhi);
      //StEmcGeom::instance("bemc")->getId(exitPhi, exitEta, exitTowerId);
      //mBemcGeom->getId(exitPhi, exitEta, exitTowerId);
    //}
    //int towerId = projTowerId(muTrack, &momentumAt, &positionAt, mag, bemcRadius, mBemcGeom);
    int towerId = mTrackProj->findTower(muTrack, mag);
    double bemcRadius = mTrackProj->getRadius();
    //if(t_towerId != towerId) Printf("t_towerId=%d, towerId=%d\n", t_towerId, towerId);
    myTrack.mRadius = bemcRadius;
    //Printf("mRadius=%f\n", myTrack.mRadius);
    //
    if(towerId <=0 || towerId > 4800){ 
      //Printf("towerId %d out of range\n", towerId);
      myTrack.mMatch = false;
      myTrackList.push_back(myTrack);
      Printf("no match for track pt %.2lf eta %.2lf", pt, eta);
      continue;
    }    
    if(!myTowerList[towerId-1].mStatus){
      myTrack.mMatch = false;
      myTrackList.push_back(myTrack);
      Printf("track with pt %lf and eta %lf matching to tower with bad status\n", pt, eta);
      continue;
    }
    (myTowerList[towerId-1].mHits)++;
    myTrack.mMatch = true;
    myTrack.mTower = myTowerList[towerId-1];
    //Printf("pt=%f, id=%d, tower hits %d, ee=%f\n", pt, myTrack.mTower.mId, myTrack.mTower.mHits, myTrack.mTower.mE);
    //double ee = myTowerList[towerId-1].mE;//energy[towerId-1];    
    //Printf("tower Id %d, energy %lf, track pt %.2lf eta %.2lf", towerId, ee, pt, eta);
    StThreeVectorD &positionAt = mTrackProj->mPositionAt;
    double exitEta = positionAt.pseudoRapidity();
    double exitPhi = positionAt.phi();
    //Printf("exitEta=%lf, exitPhi=%lf, id=%d\n", exitEta, exitPhi,towerId);
    float beta, bphi;
    mBemcGeom->getEtaPhi(towerId, beta, bphi);
    //Printf("tower Id %d, center eta: %.2lf, phi: %.2lf", towerId, beta, bphi);
    double dd = TMath::Power(beta-exitEta, 2)+TMath::Power(TVector2::Phi_mpi_pi(bphi-exitPhi), 2);
    dd = TMath::Sqrt(dd);
    myTrack.mDist = dd;
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
	
	if(ntowerId <=0 || ntowerId > 4800) {Printf("tower %d neighboring towerId invalid Id: %d\n", exitTowerId, ntowerId); continue;}
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
    //Printf("track list: pt=%f, id=%d, tower hits %d, ee=%f\n", myTrack.mPt, myTrack.mTower.mId, myTrack.mTower.mHits, myTrack.mTower.mE);
    
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
  double teta = track.mEta;
  double tphi = track.mPhi;
  TVector3 tmom; tmom.SetPtEtaPhi(tpt, teta, tphi);
  double tp = tmom.Mag();
  double tnsigE = track.mNSigmaElectron;
  double tnsigPi = track.mNSigmaPion;
  double tnsigK = track.mNSigmaKaon;
  double tnsigP = track.mNSigmaProton;
  double tr = track.mRadius;
  bool match = track.mMatch;
  double tdd = track.mDist;
  //Printf("fill hist: match=%d tpt=%f tnhits=%d id=%d tee=%f\n", match, tpt, track.mTower.mHits, track.mTower.mId, track.mTower.mE);
  hist->mHist->Fill(tpt);
  hist->mHistNSigmaElectron->Fill(tpt, tnsigE);
  hist->mHistNSigmaPion->Fill(tpt, tnsigPi);
  hist->mHistNSigmaKaon->Fill(tpt, tnsigK);
  hist->mHistNSigmaProton->Fill(tpt, tnsigP);
  hist->mHistFrac->Fill(tpt, match);
  hist->mHRadius->Fill(tr);
  if(match){
    double tee = track.mTower.mE;
    int tnhits = track.mTower.mHits;
    //Printf("filling: tpt=%f tnhits=%d id=%d tee=%f\n", tpt, tnhits, track.mTower.mId, tee);
    if(tnhits == 1){
      hist->mHistTower->Fill(tpt, tee);
      hist->mHistEpVsPt->Fill(tpt, tee/tp);
      hist->mHistEpVsDist->Fill(tdd, tee/tp);
    }
    double tcluster = track.mCluster.mE;
    double tclustermax = track.mCluster.mEMax;
    double tfee = 0;
    if(tcluster > 0) tfee = tee/tcluster;
    double tfmax = 0;
    if(tcluster > 0) tfmax = tclustermax/tcluster;    
    int cnhits = track.mCluster.mHits;//test
    //Printf("nhits = %d\n", nhits);
    //isolation cut
    if(cnhits == 1){
      hist->mHistHitTowerFracCluster->Fill(tpt, tfee);
      hist->mHistMaxTowerFracCluster->Fill(tpt, tfmax);
      //maximum cut
      if(!(tfee < tfmax)) hist->mHistEpVsPtCluster->Fill(tpt, tcluster/tp);
    }
  }
}
void StMyMatchTrackToEmcMaker::fillHistTower(const StMyTower &tower, StMyTowerHist *hist)
{
    hist->hE->Fill(tower.mE);
    hist->hNHits->Fill(tower.mHits);
    hist->hHits->Fill(tower.mId);
    //Printf("e=%f, id=%d, nhits=%d", tower.mE, tower.mId, tower.mHits);
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
