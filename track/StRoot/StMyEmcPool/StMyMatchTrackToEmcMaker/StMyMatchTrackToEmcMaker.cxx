#include "StMyMatchTrackToEmcMaker.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyMatchTrackToEmcHist.h"
#include "StRoot/StMyEmcPool/StMyMatchTrackToEmcHist/StMyTrack.h"

#include "TFile.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TH1D.h"
#include "TH2D.h"

#include "StEventTypes.h"
#include "StEmcUtil/projection/StEmcPosition.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StEmcUtil/geometry/StEmcGeom.h"

int StMyMatchTrackToEmcMaker::Init()
{
  mFile = new TFile(mFileName, "recreate");
  mHistNoCut = new StMyMatchTrackToEmcHist("NoCut");
  mHist = new StMyMatchTrackToEmcHist("All");
  mHistPos = new StMyMatchTrackToEmcHist("Pos");
  mHistNeg = new StMyMatchTrackToEmcHist("Neg");

  mBemcGeom = StEmcGeom::instance("bemc");
  return StMaker::Init();
}
int StMyMatchTrackToEmcMaker::Make()
{
  StMuPrimaryVertex *vertex = StMuDst::primaryVertex();
  if(!vertex) return kStSkip;
  if(vertex->ranking() < 0.1) return kStSkip;
  double mag = StMuDst::event()->magneticField()/10.0;

  double energy[4800];
  for(int it = 0; it < 4800; it++){
      energy[it] = 0;
  }
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
	  energy[towerId-1] = he;
          //Printf("ee = %lf\n", energy[towerId-1]);
	}
      }
    }
  }

  for(unsigned int iTrack = 0; iTrack < StMuDst::numberOfPrimaryTracks(); iTrack++)
    {
      const StMuTrack* muTrack = StMuDst::primaryTracks(iTrack);
      if(!muTrack) continue;
      if(muTrack->flag() < 0) continue;
      if(muTrack->topologyMap().trackFtpcEast() || muTrack->topologyMap().trackFtpcWest()) continue;
      if(muTrack->nHits() <= 12) continue;
      if(muTrack->nHits() < muTrack->nHitsPoss()*0.51) continue;
      if(muTrack->dcaGlobal().mag() > 3) continue;
      double pt = muTrack->pt();
      double eta = muTrack->eta();
      //double mom = muTrack->p().mag();
      if(pt < 0.2) continue;
      if(eta < -2.5 || eta > 2.5) continue;
      int charge = muTrack->charge();
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
	  //hprofmatch->Fill(pt, false); 
	  continue;
	}
	//hprofmatch->Fill(pt, true);
	double ee = energy[exitTowerId-1];
	Printf("tower Id %d, energy %lf, track pt %.2lf eta %.2lf", exitTowerId, ee, pt, eta);
	float beta, bphi;
	mBemcGeom->getEtaPhi(exitTowerId, beta, bphi);
	//Printf("tower Id %d, center eta: %.2lf, phi: %.2lf", exitTowerId, beta, bphi);
	double dd = TMath::Power(beta-exitEta, 2)+TMath::Power(TVector2::Phi_mpi_pi(bphi-exitPhi), 2);
	dd = TMath::Sqrt(dd);
	//hprofevsd->Fill(dd, ee/pt);
	//h2devsd->Fill(dd, ee/pt);
	//
	StMyTrack myTrack;
	myTrack.mPt = pt;
	myTrack.mE = ee;
	myTrack.mDist = dd;
	//double res0 = ee-pt;
	//double res1 = ee-pt*TMath::CosH(eta);
	//hresidue->Fill(res0);
	//hresTrans->Fill(res1);
	//h2d->Fill(pt, ee);
	//h2de->Fill(pt*TMath::CosH(eta), ee);
	//if(ee > 0.2) hresTransCut->Fill(res1);

	//
	double nsigE = muTrack->nSigmaElectron();
	double nsigPi = muTrack->nSigmaPion();
	myTrack.mNSigmaElectron = nsigE;
	myTrack.mNSigmaPion = nsigPi;
		
	//looking for neighboring tower
	double nsum = 0;
	double emax = ee;
	for(int ieta = -1; ieta <= 1; ieta++){
	  for(int iphi = -1; iphi <=1; iphi++){
	    int ntowerId = EmcPosition.getNextTowerId(exitTowerId, ieta, iphi);
	    
	    if(ntowerId <=0 || ntowerId > 4800) {Printf("neighboring towerId invalid Id: %d\n", ntowerId); continue;}
	    double nee = energy[ntowerId-1];
	    if(nee > emax) emax = nee;
	    nsum += nee;
	    //Printf("ntowerId=%d, exitTowerId=%d, nee=%4.3lf, et=%4.3lf\n", ntowerId, exitTowerId, nee, ee);
	    
	  }
	}
	double fee = 0;
	double fmax = 0;
	if(nsum > 0. || nsum < 0.){
	  fee = ee/nsum;
	  fmax = emax/nsum;
	}
	//Printf("fee=%4.3lf, fmax=%4.3lf\n", fee, fmax);
	myTrack.mHitFrac = fee;
	myTrack.mHitMax = fmax;
	myTrack.mECluster = nsum;
	if(nsigE < -2. || nsigE > 2.){
	  if(charge > 0){
	    fillHists(myTrack, mHistPos);
	  }
	  if(charge < 0){
	    fillHists(myTrack, mHistNeg);
	  }
	  fillHists(myTrack, mHist);
	}
	fillHists(myTrack, mHistNoCut);	
      }
    }

  return StMaker::Make();
}
int StMyMatchTrackToEmcMaker::Finish()
{
  mFile->Write();
  mFile->Close();
  return StMaker::Finish();
}
void StMyMatchTrackToEmcMaker::fillHists(const StMyTrack &track, StMyMatchTrackToEmcHist *hist)
{
  double tpt = track.mPt;
  double tee = track.mE;
  double tnsigE = track.mNSigmaElectron;
  double tnsigPi = track.mNSigmaPion;
  double tdd = track.mDist;
  double tcluster = track.mECluster;
  double tfee = track.mHitFrac;
  double tfmax = track.mHitMax;
  hist->mHistNSigmaElectron->Fill(tnsigE);
  hist->mHistNSigmaPion->Fill(tnsigPi);
  hist->mHistTrack->Fill(tpt);
  if(tee > 0.2) hist->mHistTrackFrac->Fill(tpt, 1);
  else hist->mHistTrackFrac->Fill(tpt, 0);
  hist->mHistTower->Fill(tee);
  hist->mHistEptVsPt->Fill(tpt, tee/tpt);
  hist->mHistEptVsPtCluster->Fill(tpt, tcluster/tpt);
  hist->mHistHitTowerFracCluster->Fill(tpt, tfee);
  hist->mHistMaxTowerFracCluster->Fill(tpt, tfmax);
  hist->mHistEptVsDist->Fill(tdd, tee/tpt);
}
