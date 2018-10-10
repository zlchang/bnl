#include "StMyTrackToGeantMaker.h"
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

#include "StMyEmcPool/StMyObjs/StMyTrackGeant.h"

#include "StMyEmcPool/StMyTrackToGeantHist/StMyTrackToGeantHist.h"
#include "StMyEmcPool/StMyObjs/StMyMcTrack.h"

#include "TFile.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TH1D.h"
#include "TH2D.h"

#include "StEventTypes.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMcEvent/StMcEventTypes.hh"

#include <vector>
#include <map>
int StMyTrackToGeantMaker::Init()
{
  mFile = new TFile(mFileName, "recreate");
  mHist = new StMyTrackToGeantHist("All");

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

  return StMaker::Init();
}
int StMyTrackToGeantMaker::Make()
{
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

  vector<StMyTrackGeant> myTrackList;
  map<long, StMyMcTrack> myMcTrackList;
  for(vector<const StMuTrack*>::const_iterator it = muTrackList.begin(); it != muTrackList.end(); it++){
    const StMuTrack *muTrack = *it;
    double pt = muTrack->pt();
    double eta = muTrack->eta();
    double phi = muTrack->phi();
    double nsigE = muTrack->nSigmaElectron();
    double nsigPi = muTrack->nSigmaPion();
    int charge = muTrack->charge();
    long id = muTrack->idTruth();
    int qa = muTrack->qaTruth();
    int nhits = muTrack->nHits();
    if(id != 0 && myMcTrackList.find(id) == myMcTrackList.end()){
      myMcTrackList.insert(pair<long, StMyMcTrack>(id, StMyMcTrack()));
    }
    
    //Printf("pt=%f, eta=%f, id=%d qa=%d nhits=%d\n", pt, eta, id, qa, nhits);

    StMyTrackGeant myTrack;
    myTrack.mPt = pt;
    myTrack.mEta = eta;
    myTrack.mPhi = phi;   
    myTrack.mNSigmaElectron = nsigE;
    myTrack.mNSigmaPion = nsigPi;
    myTrack.mCharge = charge;    

    myTrack.mKey = id;
    myTrack.mQa = qa;
    myTrack.mNhits = nhits;

    myTrackList.push_back(myTrack);
  }

  int muEventId = StMuDst::event()->eventNumber();
  float muVz = vertex->position().z();
  //McEvent
  StMcEvent* mcEvent = (StMcEvent*) GetDataSet("StMcEvent");
  if (!mcEvent) return kStSkip;

  int mcEventId = mcEvent->eventNumber();
  if(muEventId != mcEventId){
    Printf("Event Id %d (mu) %d (mc) not matching\n", muEventId, mcEventId); 
    return kStSkip;
  }
  float vZ = mcEvent->primaryVertex()->position().z();
  Printf("vZ=%f\n", vZ);

  float dZ = muVz-vZ;
  mHist->mDvZ->Fill(dZ);
  if(TMath::Abs(dZ) > 2) return kStSkip;

  StSPtrVecMcTrack gTracks = mcEvent->tracks();
  for(unsigned int igt = 0; igt < gTracks.size(); igt++){
    StMcTrack *trk = gTracks[igt];
    float gpt = trk->pt();
    StLorentzVectorF gmom = trk->fourMomentum();
    //if(pt < 5.0) continue;
    //float gr = 0;
    float geta = trk->pseudoRapidity();
    //StMcTrack *prnt = trk->parent();
    //if(prnt->geantId() != 0) continue;
    //StMcVertex *startv = trk->startVertex();
    //float zt = startv->position().z();
    int gishw = trk->isShower();
    long gpdg = trk->pdgId();
    long gid = trk->geantId();
    if(gid == 0) continue;
    long gkey = trk->key();

    if(myMcTrackList.find(gkey) == myMcTrackList.end()) continue;
    
    StPtrVecMcTpcHit ghits = trk->tpcHits();
    unsigned int nghits = ghits.size();
    //Printf("pt=%f eta=%f ishw=%d pdg=%ld geantId=%ld key=%ld tpcHits=%d\n", gpt, geta, gishw, gpdg, gid, gkey, nghits);

    StMyMcTrack &mcTrack = myMcTrackList[gkey];
    mcTrack.mom = TLorentzVector(gmom.x(), gmom.y(), gmom.z(), gmom.t());
    mcTrack.id = gkey;
    mcTrack.geantid = gid;
    //example
    /*
    for(unsigned int ih = 0; ih < nghits; ih++){
      StMcTpcHit *tpc = ghits[ih];
      unsigned long sec = tpc->sector();
      unsigned long padrow = tpc->padrow();

      float adc = tpc->adc();
      if(adc < 0) Print("WARNING\n");
      float lgamma = tpc->lgamma();
      float pad = tpc->pad();

      Printf("sec=%d, pad=%d, adc=%f, lgamma=%f, pad=%f\n", sec, padrow, adc, lgamma, pad);
    }
    */
  }
  //fill histogram
  for(vector<StMyTrackGeant>::const_iterator it = myTrackList.begin(); it != myTrackList.end(); it++){
    const StMyTrackGeant &myTrack = *it;
    long tkey = myTrack.mKey;
    StMyMcTrack myMcTrack;
    if(myMcTrackList.find(tkey) != myMcTrackList.end()){
      myMcTrack = myMcTrackList[tkey];
    }
    fillHist(myTrack, myMcTrack, mHist);	
  }
  return StMaker::Make();
}
int StMyTrackToGeantMaker::Finish()
{
  mFile->Write();
  mFile->Close();
  return StMaker::Finish();
}
void StMyTrackToGeantMaker::fillHist(const StMyTrackGeant &track, const StMyMcTrack &mcTrack, StMyTrackToGeantHist *hist)
{
  double tpt = track.mPt;
  double teta = track.mEta;
  double tqa = track.mQa;
  long tkey = track.mKey;
  bool match = tkey != 0;

  hist->mMatchVsPt->Fill(tpt, match);
  hist->mMatchVsEta->Fill(teta, match);

  if(tkey != 0){
    hist->mQaVsPt->Fill(tpt, tqa);
    hist->mQaVsEta->Fill(teta, tqa);
  }
  
  if(match){
    double mkey = mcTrack.id;
    if(mkey == tkey){
    double mpt = mcTrack.mom.Pt();
    double meta = mcTrack.mom.Eta();
    hist->mPtMcVsMu->Fill(tpt, mpt);
    hist->mEtaMcVsMu->Fill(teta, meta);
    }else{
     Printf("M/C track %d not found\n", tkey);
   }
 }
}
