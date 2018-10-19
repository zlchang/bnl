#include "StMyEmcGeantMaker.h"
#include "StMyEmcPool/StMyUtils/StMyMcVertexCut.h"
#include "TFile.h"

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/database/StBemcTables.h"

#include "StMcEvent/StMcEventTypes.hh"

#include "StMyEmcPool/StMyEmcFromGeantHist/StMyMcTrackHist.h"
#include "StMyEmcPool/StMyObjs/StMyGeantId.h"
#include "StMyEmcPool/StMyObjs/StMyMcTrack.h"
#include "StMyEmcPool/StMyObjs/StMyMcTower.h"

#include "StMyEmcPool/StMyUtils/func.h"
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
    mMapHists.insert(pair<long, StMyMcTrackHist*>(id, new StMyMcTrackHist(geantId.getName(id))));
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
    float e = track.sumE();
    unsigned int n = track.size();

    if(mMapHists.find(id) == mMapHists.end()) continue;
    if(!(p > 0)) continue;
    mMapHists[id]->mEpVsP->Fill(p, e/p);
    mMapHists[id]->mNVsP->Fill(p, n);
  }
  
  return StMaker::Make();
}

int StMyEmcFromGeantMaker::Finish()
{
  mFile->Write();
  mFile->Close();
  return StMaker::Finish();
}
