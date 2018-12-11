
const unsigned long nevents = 10000;
/*
class myTower{//: public TObject{
public:
  float dE;
  float eta;
  float e;
  float id;
};
class myTrack{// : public TObject{
public:
  float pt;
  long id;
  long geantId;
  vector<myTower> towers;
};
*/
void readMuDstGeant(//const char* mudstfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root",
		    //const char* geantfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.geant.root", 
                    const char* mudstfile = "/star/u/zchang/data05/pp500_production_2012/pt5_7_100_20160201/P13ib.SL13b/2012/078/13078035/pt5_7_st_zerobias_adc_13078035_raw_3580001_7.MuDst.root",
		    const char* geantfile = "/star/u/zchang/data05/pp500_production_2012/pt5_7_100_20160201/P13ib.SL13b/2012/078/13078035/pt5_7_st_zerobias_adc_13078035_raw_3580001_7.geant.root",
	            const char* ver = "v1"
//		     const char* geantfile = "test.list",
//		     const char* ver = "v1"
  )
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  //gROOT->Macro("LoadJetMaker.C");

  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StEEmcUtil");
  
  gSystem->Load("StEmcRawMaker");
  gSystem->Load("StEmcADCtoEMaker");
  
  gSystem->Load("StDbBroker");
  gSystem->Load("St_db_Maker");	
  
  //emc simulator
  gSystem->Load("StEmcSimulatorMaker");
  // Create chain
  StChain* chain = new StChain;
  //
  /*
  StFile *fileset = new StFile;
  const char file[256];
  ifstream in(geantfile);
  while(in >> file){
    TString str(file);
    str.ReplaceAll(".MuDst.root", ".geant.root");
    fileset->AddFile(str.Data());
    Printf("%s\n", file);
  }
  */
  // GEANT reader
  StIOMaker* ioMaker = new StIOMaker;
  ioMaker->SetFile(geantfile);
  //ioMaker->SetFileSet(fileset);
  ioMaker->SetIOMode("r");
  ioMaker->SetBranch("*",0,"0");             // Deactivate all branches
  ioMaker->SetBranch("geantBranch",0,"r");   // Activate geant branch
 
  StEmcGeom *emcGeom = StEmcGeom::instance("bemc"); 
  // Monte Carlo event maker
  StMcEventMaker* mcEventMaker = new StMcEventMaker;
  mcEventMaker->doPrintEventInfo  = false;
  mcEventMaker->doPrintMemoryInfo = false;
  
  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",mudstfile,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");
    
  TFile *fout = new TFile(Form("simu.emc.%s.root", ver), "recreate");
  TProfile *htrkeff = new TProfile("trackEff", "; p_{T} [GeV]", 50, 0, 50);  
  TH1D *htrkphi = new TH1D("trackPhi", "; #phi", 126, -3.15, 3.15);  
  chain->Init();
  
  for (int iEvent = 0; iEvent < nevents; ++iEvent) {
    chain->Clear();
    
    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    //print flag
    bool flag_print = (iEvent%100 == 0);
    //bool flag = chain->GetMaker("Eread") || chain->GetMaker("emcRaw");
    //Printf("embedding flag = %d\n", flag);

    //emc simulator

    //bool flag_bfc = chain->InheritsFrom("StBFChain");
    //Printf("bfc flag = %d\n", flag_bfc);
    //StEvent *event = (StEvent*) chain->GetDataSet("StEvent");
    //if(!event) {Printf("event %d not found\n", iEvent); continue;}
    //StMuDst *dst = (StMuDst*)chain->GetDataSet("StMuDst");
    //if(!dst) Printf("mudst not found\n");
    int nTracks = StMuDst::numberOfPrimaryTracks();
    map<long, int> myIds;
    for(int itrk = 0; itrk < nTracks; itrk++){
      StMuTrack *track = StMuDst::primaryTracks(itrk);
      if(track->flag() < 0) continue;
      if(track->topologyMap().trackFtpcEast() || track->topologyMap().trackFtpcWest()) continue;
      float mpt = track->pt();
      float meta = track->eta();
      int id = track->idTruth();
      int qa = track->qaTruth();
      int nhits = track->nHits();
      if(id == 0) continue;
      if(nhits < 12) continue;
      if(myIds.find(id) == myIds.end()) myIds.insert(pair<long, int>(id, 1));
      else myIds[id]++;
      Printf("pt=%f, eta=%f, id=%d qa=%d nhits=%d\n", mpt, meta, id, qa, nhits);
    }
    int sum = 0;
    for(map<long, int>::const_iterator im = myIds.begin(); im != myIds.end(); im++){
	//long mid = im->first;
        int mcnt = im->second;
	sum += mcnt;
    }
    for(map<long, int>::const_iterator im = myIds.begin(); im != myIds.end(); im++){
	long mid = im->first;
        int mcnt = im->second;
        double mr = (mcnt+0.)/(sum+0.);
        Printf("id=%d cnt=%d r=%f\n", mid, mcnt, mr);
    }
    //MuDst
    // Get StMcEvent
    StMcEvent* mcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mcEvent) continue;

    StMcVertex* mcVx = mcEvent->primaryVertex();
    float vZ = mcEvent->primaryVertex()->position().z();
    Printf("vZ=%f\n", vZ);
    StPtrVecMcTrack& daughters = mcVx->daughters();
    int counter = 0;
    for(int idt = 0; idt < daughters.size(); idt++){
       StMcTrack *dtrk = daughters[idt];
       long dgid = dtrk->geantId();
       float dpt = dtrk->pt();
       float deta = dtrk->momentum().pseudoRapidity();
       float dphi = dtrk->momentum().phi();
       long dkey = dtrk->key();
       unsigned int dntpc = dtrk->tpcHits().size();
       //if(dgid != 8 && dgid != 9) continue;
       if(TMath::Abs(deta) > 2.5) continue;
       //Printf("did=%d dpt=%f deta=%f dgid=%d dntpc=%d\n", idt, dpt, deta, dgid, dntpc);
       if(dntpc == 0) continue;
       bool match = false;
       if(myIds.find(dkey) != myIds.end()){
          match = true;
       }
       htrkeff->Fill(dpt, match);
       htrkphi->Fill(dphi);
       counter++;

       //stop vertex
       StMcVertex *stopv = dtrk->stopVertex();
       unsigned int dnd = 0;
       if(stopv) dnd = stopv->numberOfDaughters();
       Printf("dkey=%d dpt=%f deta=%f dgid=%d dntpc=%d match=%d daughters=%d\n", dkey, dpt, deta, dgid, dntpc, match, dnd);
    }
    Printf("%d mudst %d mc\n", myIds.size(), counter);
    /*
    StSPtrVecMcTrack gTracks = mcEvent->tracks();
    for(int igt = 0; igt < gTracks.size(); igt++){
      StMcTrack *trk = gTracks[igt];
      float pt = trk->pt();
      //if(pt < 5.0) continue;
      float r = 0;
      float eta = trk->pseudoRapidity();
      //StMcTrack *prnt = trk->parent();
      //if(prnt->geantId() != 0) continue;
      //StMcVertex *startv = trk->startVertex();
      //float zt = startv->position().z();
      int ishw = trk->isShower();
      long pdg = trk->pdgId();
      long gid = trk->geantId();
      if(gid == 0) continue;
      long key = trk->key();
      if(myIds.find(key) == myIds.end()) continue;

      StPtrVecMcTpcHit ghits = trk->tpcHits();
      unsigned int nghits = ghits.size();
      if(nghits == 0) continue;
      Printf("pt=%f eta=%f ishw=%d pdg=%ld geantId=%ld key=%ld tpcHits=%d\n", pt, eta, ishw, pdg, gid, key, nghits);
      
      for(int ih = 0; ih < nghits; ih++){
	StMcTpcHit *tpc = ghits[ih];
	unsigned long sec = tpc->sector();
	unsigned long padrow = tpc->padrow();
	
	float adc = tpc->adc();
	if(adc < 0) Print("WARNING\n");
	float lgamma = tpc->lgamma();
	float pad = tpc->pad();

	//Printf("sec=%d, pad=%d, adc=%f, lgamma=%f, pad=%f\n", sec, padrow, adc, lgamma, pad);
      }
    }
   */
  }
  htrkeff->Print("all");
  htrkphi->Print("all");
  fout->Write();
  fout->Close();
}
