#include "StMyEmcGeantMaker.h"
#include "StMyEmcPool/StMyUtils/StMyMcVertexCut.h"
#include "TFile.h"

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/database/StBemcTables.h"

#include "StMcEvent/StMcEventTypes.hh"

#include "StMyEmcPool/StMyEmcFromGeantHist/StMyEmcTrackHist.h"
#include "StMyEmcPool/StMyObjs/StMyGeantId.h"
#include "StMyEmcPool/StMyObjs/StMyMcTrack.h"
#include "StMyEmcPool/StMyObjs/StMyMcTower.h"

#include "StMyEmcPool/StMyUtils/func.h"

#include "StarClassLibrary/StParticleTable.hh"
#include "StarClassLibrary/StParticleDefinition.hh"

#include "StMyEmcPool/StMyUtils/StMyTrackProjEmc.h"

int StMyEmcFromGeantMaker::Init()
{
  mBemcTable = new StBemcTables;
  mBemcGeom = StEmcGeom::instance("bemc");

  mIds.push_back(1);
  mIds.push_back(2);
  mIds.push_back(3);

  mIds.push_back(7);
  mIds.push_back(8);
  mIds.push_back(9);

  mIds.push_back(10);
  mIds.push_back(11);
  mIds.push_back(12);

  mIds.push_back(13);
  mIds.push_back(14);
  mIds.push_back(15);

  mIds.push_back(16);

  mFile = new TFile(mFileName, "recreate");
  StMyGeantId geantId;
  for(vector<long>::const_iterator ii = mIds.begin(); ii != mIds.end(); ii++){
    long id = *ii;
    mMapHists.insert(pair<long, StMyEmcTrackHist*>(id, new StMyEmcTrackHist(geantId.getName(id))));
    mMapTrackHists.insert(pair<long, StMyEmcTrackMatchHist*>(id, new StMyEmcTrackMatchHist(Form("Track%s", geantId.getName(id)))));
  }
  
  return StMaker::Init();
}
int StMyEmcFromGeantMaker::Make()
{
  mBemcTable->loadTables(this);
  StMcEvent* mcEvent = (StMcEvent*) GetDataSet("StMcEvent");
  if (!mcEvent) return kStSkip;

  StMcVertex *primary = mcEvent->primaryVertex();
  float vZ = primary->position().z();
  
  Printf("vZ=%f\n", vZ);
  bool vflag = false;
  for(vector<StMyMcVertexCut*>::const_iterator iv = mVertexCuts.begin(); iv != mVertexCuts.end(); iv++){
     StMyMcVertexCut *vcut = *iv;
     if((*vcut)(primary)){ vflag = true; break;}
   }	
   if(vflag) return kStSkip;
  
  StMcEmcHitCollection *emcHits = mcEvent->bemcHitCollection();
  //unsigned long nhits = emcHits->numberOfHits();
  unsigned int nmodules = emcHits->numberOfModules();
  
  //vector<float> mcDe(4800);
    
  unsigned long nsum = 0;
  
  map<long, StMyMcTrack> tracks;
  //map<long, float> tracks_pt;
  //map<long, long> tracks_id;
    
  for(unsigned int im = 1; im <= nmodules; im++){
    StMcEmcModuleHitCollection *module = emcHits->module(im);
    if(!module) { Printf("module %d not found\n", im); continue; }
    unsigned long nh = module->numberOfHits();
    nsum += nh;
    //unsigned long ndh = module->numberOfDetectorHits();
    //Printf("module = %d hn = %d, hdh = %d \n", im+1, nh, ndh);
    ///*
    StSPtrVecMcCalorimeterHit &hits = module->hits();
    //Printf("geant hit size: %d\n", hits.size());
    for(unsigned long ih = 0; ih < hits.size(); ih++){
      StMcCalorimeterHit *hh = hits[ih];
      if(!hh) continue;
      float dE = hh->dE();
      int towerId;
      int sub = hh->sub();
      //if(sub < 0) Print("sub = %d\n", sub);
      mBemcGeom->getId(im, hh->eta(), sub, towerId);
      float eta; mBemcGeom->getEta(im, hh->eta(), eta);
      //double f_sample = samplingFraction(eta);
      //double ee = dE/f_sample;
      //Printf("id=%d e=%f\n", towerId, ee);
      
      StMyMcTower tower;
      tower.dE = dE;
      tower.eta = eta;
      //tower.e = ee;
      tower.id = towerId;
      
      //mcDe[towerId-1] += dE;
      ////float ee = energy[towerId-1];
      ////if(dE > 0) Printf("towerId = %d, dE = %f, ee = %f\n", towerId, dE, ee);
      //if(dE > 0) Printf("towerId = %d, dE = %f\n", towerId, dE);
      StMcTrack *trk = hh->parentTrack();
      if(!trk){ 
	Printf("track not found for dE = %f\n", dE); 
      }
      else{
	//float pt = trk->pt();
	StLorentzVectorF vec = trk->fourMomentum();
	TLorentzVector mom(vec.px(), vec.py(), vec.pz(), vec.e());
	//float r = 0;
	//if(pt > 0) r = ee/pt;
	//int ishw = trk->isShower();
	//long pdg = trk->pdgId();
	long id = trk->geantId();
	long key = trk->key();
	//float eta_trk = trk->pseudoRapidity();
        StMcVertex *startv = trk->startVertex();
        float dca = dcaT(primary, startv);
        if(dca > 3.) continue;
        //float z_trk = startv->position().z();
        //if(TMath::Abs(z_trk) > 30) continue;
	if(tracks.find(key) == tracks.end()){
	  StMyMcTrack track;
	  track.mom = mom;
          //track.eta = eta_trk;
	  track.id = key;
	  track.geantid = id;
	  track.towers.push_back(tower);
	  tracks.insert(pair<long, StMyMcTrack>(key,track));
	}else{
	  tracks[key].towers.push_back(tower);
	}
	//if(id == 7 || id == 1) Printf("towerid = %d r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n, ", towerId, r, pt, ishw, pdg, id, key);
	//Printf("id=%d e=%f pt=%f r=%f ishw=%d, pdg=%ld geantId=%ld key=%ld\n", towerId, ee, pt, r, ishw, pdg, id, key);

	/*if(tracks.find(key) == tracks.end()){
	  tracks.insert(pair<long, float>(key, ee));
	  }else{
	  tracks[key] += ee;
	  }
	  if(tracks_id.find(key) == tracks_id.end()){
	  tracks_id.insert(pair<long, long>(key, id));
	  }
	  if(tracks_pt.find(key) == tracks_pt.end()){
	  tracks_pt.insert(pair<long, float>(key, pt));
	  }*/
	
      }
    }
  }
  Printf("track size: %u\n", tracks.size());
  //fill tracks
  for(map<long, StMyMcTrack>::const_iterator im = tracks.begin(); im != tracks.end(); im++){
    const StMyMcTrack &track = im->second;
    long id = track.geantid;
    float p = track.mom.P();
    float pt = track.mom.Pt();
    float e = track.sumE();
    unsigned int n = track.size();

    if(mMapHists.find(id) == mMapHists.end()) continue;
    if(!(p > 0)) continue;
    mMapHists[id]->mEpVsPt->Fill(pt, e/p);
    mMapHists[id]->mNVsPt->Fill(pt, n);
  }
  //match track to tower
  map<long, StMyMcTower> towers;
  
  for(unsigned int im = 1; im <= nmodules; im++){
    StMcEmcModuleHitCollection *module = emcHits->module(im);
    if(!module) { Printf("module %d not found\n", im); continue; }
    StSPtrVecMcCalorimeterHit &hits = module->hits();
    //Printf("geant hit size: %d\n", hits.size());
    for(unsigned long ih = 0; ih < hits.size(); ih++){
      StMcCalorimeterHit *hh = hits[ih];
      if(!hh) continue;
      float dE = hh->dE();
      int towerId;
      int sub = hh->sub();
      //if(sub < 0) Print("sub = %d\n", sub);
      mBemcGeom->getId(im, hh->eta(), sub, towerId);
      float eta; mBemcGeom->getEta(im, hh->eta(), eta);
      //double f_sample = samplingFraction(eta);
      //double ee = dE/f_sample;
      //Printf("id=%d e=%f\n", towerId, ee);
      
      StMyMcTower tower;
      tower.dE = dE;
      tower.eta = eta;
      //tower.e = ee;
      tower.id = towerId;
      towers.insert(pair<long, StMyMcTower>(towerId,tower));
    }
  }

  StMcVertex* mcVx = mcEvent->primaryVertex();
  StPtrVecMcTrack& daughters = mcVx->daughters();
  vector<StMyMcTrack> myMcPrimaryTracks;
  //  int counter = 0;
  for(unsigned int idt = 0; idt < daughters.size(); idt++){
    StMcTrack *dtrk = daughters[idt];
    long dgid = dtrk->geantId();
    //Printf("dgid=%d\n", dgid);
    StParticleTable *table = StParticleTable::instance();
    StParticleDefinition *def = table->findParticleByGeantId(dgid);
    double chrg = def->charge();
    if(round(chrg) == 0) continue;
    StLorentzVectorF dmom = dtrk->fourMomentum();
    long dkey = dtrk->key();
    unsigned int dntpc = dtrk->tpcHits().size();
    //if(dgid != 8 && dgid != 9) continue;
    //float deta = dtrk->momentum().pseudoRapidity();
    float dpt = dtrk->pt();
    if(dpt < 0.2) continue;
    int towerId = mTrackProj->findTower(dtrk, -0.5);
    if(towerId <= 0 || towerId > 4800) continue;
    //bool dmatch = (myMcTrackGeantMap.find(dkey) != myMcTrackGeantMap.end());
    //bool ddecay = (dtrk->stopVertex() != 0);
    //if(dntpc == 45) Printf("did=%d dpt=%f deta=%f dgid=%d dntpc=%d dmatch=%d ddecay=%d\n", idt, dpt, deta, dgid, dntpc, dmatch, ddecay);
    //if(dntpc == 0) continue;
    if(towers.find(towerId) != towers.end()){
      StMyMcTrack ptrack;
      ptrack.mom = TLorentzVector(dmom.x(), dmom.y(), dmom.z(), dmom.t());
      ptrack.id = dkey;
      ptrack.geantid = dgid;
      ptrack.ntpchits = dntpc;
      //
      ptrack.towers.push_back(towers[towerId]);
      myMcPrimaryTracks.push_back(ptrack);
    }
    //*/
    //htrkeff->Fill(dpt, match);
    //counter++;
    //Printf("did=%d dpt=%f deta=%f dgid=%d dntpc=%d\n", idt, dpt, deta, dgid, dntpc);
  }

  //fill tracks
  for(vector<StMyMcTrack>::const_iterator im = myMcPrimaryTracks.begin(); im != myMcPrimaryTracks.end(); im++){
    const StMyMcTrack &track = *im;
    long id = track.geantid;
    float p = track.mom.P();
    float pt = track.mom.Pt();
    float e = track.sumE();
  
    if(!(p > 0)) continue;
    mMapTrackHists[id]->mEpVsPt->Fill(pt, e/p);
  }


  return StMaker::Make();
}

int StMyEmcFromGeantMaker::Finish()
{
  mFile->Write();
  mFile->Close();
  return StMaker::Finish();
}
