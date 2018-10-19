const unsigned long nevents = 10000;
const double MipPhotoElectrons = 63;
const double MipEnergyDeposit = 0.0198;
double mSF[3] = {14.356, -0.512, 0.668};
double samplingFraction(double eta) {
  double x = TMath::Abs(eta);
  return mSF[0]+mSF[1]*x+mSF[2]*x*x;
}
double samplingFraction(double *x, double *par){
  double xx = TMath::Abs(x[0]);
  return 1./(par[0]+par[1]*xx+par[2]*xx*xx);
}
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
void readTestEmcSimu(//const char* mudstfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root",
	     //const char* geantfile = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.geant.root", const char* ver = "v1")
		     const char* geantfile = "test.list",
		     const char* ver = "v1"
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
  StFile *fileset = new StFile;
  const char file[256];
  ifstream in(geantfile);
  while(in >> file){
    TString str(file);
    str.ReplaceAll(".MuDst.root", ".geant.root");
    fileset->AddFile(str.Data());
    Printf("%s\n", file);
  }

  // GEANT reader
  StIOMaker* ioMaker = new StIOMaker;
  ioMaker->SetFile(geantfile);
  ioMaker->SetFileSet(fileset);
  ioMaker->SetIOMode("r");
  ioMaker->SetBranch("*",0,"0");             // Deactivate all branches
  ioMaker->SetBranch("geantBranch",0,"r");   // Activate geant branch
 
  StEmcGeom *emcGeom = StEmcGeom::instance("bemc"); 
  // Monte Carlo event maker
  StMcEventMaker* mcEventMaker = new StMcEventMaker;
  mcEventMaker->doPrintEventInfo  = false;
  mcEventMaker->doPrintMemoryInfo = false;
  
  //StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",mudstfile,"",1000);
  //StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");
  

  //StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  //adc->saveAllStEvent(true);

  //StEmcSimulatorMaker *emcSimu = new StEmcSimulatorMaker;


  StBemcTables *emcTables = new StBemcTables;

  TFile *fout = new TFile(Form("simu.emc.%s.root", ver), "recreate");
  TF1 *f1 = new TF1("samplingFraction", samplingFraction, -2, 2, 3);
  f1->SetParameters(mSF);
  int nbins = 50;
  double xmin = 5, xmax= 10;
  TH2D *h = new TH2D("hsimu", ";geant;simu", 50, 0, 0.05, 25, -0.5, 2);
  TProfile *hph = new TProfile("hph", ";p_{T} [GeV]; E/p_{T}", nbins, xmin, xmax); 
  TH2D *hph2d = new TH2D("hph2d", ";p_{T} [GeV]; E/p_{T}", nbins, xmin, xmax, 150, -0.1, 1.4); 
  TProfile *he = new TProfile("he", ";p_{T} [GeV]; E/p_{T}", nbins, xmin, xmax); 
  TProfile *hpi = new TProfile("hpi", ";p_{T} [GeV]; E/p_{T}", nbins, xmin, xmax); 
  TProfile *hk = new TProfile("hk", ";p_{T} [GeV]; E/p_{T}", nbins, xmin, xmax);
  TH1D *heta = new TH1D("heta", ";#eta", 100, -5, 5); 
  TProfile *hpheta = new TProfile("hpheta", ";#eta; E/p_{T}", 100, -5, 5); 
  
  chain->Init();
  
  for (int iEvent = 0; iEvent < nevents; ++iEvent) {
    chain->Clear();
    
    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;

    emcTables->loadTables(chain);
    
    //print flag
    bool flag_print = (iEvent%100 == 0);
    //bool flag = chain->GetMaker("Eread") || chain->GetMaker("emcRaw");
    //Printf("embedding flag = %d\n", flag);

    //emc simulator
    /*
    //bool flag_bfc = chain->InheritsFrom("StBFChain");
    //Printf("bfc flag = %d\n", flag_bfc);
    StEvent *event = (StEvent*) chain->GetDataSet("StEvent");
    if(!event) {Printf("event %d not found\n", iEvent); continue;}
    //StMuDst *dst = (StMuDst*)chain->GetDataSet("StMuDst");
    //if(!dst) Printf("mudst not found\n");
    StEmcCollection *emc = event->emcCollection();
    StEmcDetector *bemc = emc->detector(kBarrelEmcTowerId);
    if (bemc) {
      for(unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
	StSPtrVecEmcRawHit& rawHits = bemc->module(m)->hits();
	//Printf("MuDst module %d hits %d\n", m, rawHits.size());
	for(size_t k = 0; k < rawHits.size(); ++k) {
	  StEmcRawHit *hit = rawHits[k];
	  float ee = hit->energy();
	  unsigned int adc = hit->adc();
	  int towerId = 0;
	  emcGeom->getId(hit->module(), hit->eta(), hit->sub(), towerId);
	  if(flag_print) Printf("id=%d ee=%f adc=%u\n", towerId, ee, adc);
	}
      }
    }else{
      Printf("bemc not found\n");
    }
    */
    //MuDst
    /*StEmcCollection *emc = StMuDst::emcCollection();
    StEmcDetector* bemc = emc->detector(kBarrelEmcTowerId);

    vector<float> energy(4800);
    if (bemc) {
      for(unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
	StSPtrVecEmcRawHit& rawHits = bemc->module(m)->hits();
	//Printf("MuDst module %d hits %d\n", m, rawHits.size());
	for(size_t k = 0; k < rawHits.size(); ++k) {
	  StEmcRawHit *hit = rawHits[k];
	  float ee = hit->energy();
	  int towerId = 0;
	  emcGeom->getId(hit->module(), hit->eta(), abs(hit->sub()), towerId);
	  energy[towerId-1] = ee;
	  //if(towerId >=1 && towerId <= 4800){
	  //energy[towerId-1] = hit->energy();
	  //}
	}
      }
      Printf("mudst hit size: %d\n", energy.size());
    }else{
      Printf("bemc not found\n");
    }
    */
    // Get StMcEvent
    StMcEvent* mcEvent = (StMcEvent*)chain->GetDataSet("StMcEvent");
    if (!mcEvent) continue;
    unsigned long eid = mcEvent->eventNumber();
    if(eid != 37865) continue;
    float vZ = mcEvent->primaryVertex()->position().z(); 
    Printf("vZ=%f\n", vZ);

    StSPtrVecMcTrack mTracks = mcEvent->tracks();
    for(int igt = 0; igt < mTracks.size(); igt++){
      StMcTrack *trk_m = mTracks[igt];
      long id_m = trk_m->geantId();
      float pt_m = trk_m->pt();
      if(id_m != 0){
	Printf("id_m=%d pt_m=%f\n", id_m, pt_m);
      }
    }

    StMcEmcHitCollection *emcHits = mcEvent->bemcHitCollection();
    unsigned long nhits = emcHits->numberOfHits();
    unsigned int nmodules = emcHits->numberOfModules();

    //vector<float> mcDe(4800);
    
    unsigned long nsum = 0;

    map<long, float> tracks;
    map<long, float> tracks_pt;
    map<long, float> tracks_eta;
    map<long, long> tracks_id;
    
    for(unsigned int im = 1; im <= nmodules; im++){
      StMcEmcModuleHitCollection *module = emcHits->module(im);
      if(!module) { Printf("module %d not found\n", im); continue; }
      unsigned long nh = module->numberOfHits();
      nsum += nh;
      unsigned long ndh = module->numberOfDetectorHits();
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
	emcGeom->getId(im, hh->eta(), sub, towerId);
        //sampling fraction
	//float eta; emcGeom->getEta(im, hh->eta(), eta); 
	//double f_sample = f1->Eval(eta);//samplingFraction(eta);
	//dE *= 1./f_sample;
	//if(flag_print) Printf("id=%d e=%f\n", towerId, dE);
	Printf("id=%d e=%f\n", towerId, dE);
	
	//mcDe[towerId-1] += dE;
	////float ee = energy[towerId-1];
	////if(dE > 0) Printf("towerId = %d, dE = %f, ee = %f\n", towerId, dE, ee);
	//if(dE > 0) Printf("towerId = %d, dE = %f\n", towerId, dE);
	StMcTrack *trk = hh->parentTrack();
	if(!trk){ 
	  Printf("track not found for dE = %f\n", dE); 
	}
	else{
	  float pt = trk->pt();
          //if(pt < 5.0) continue;
	  float r = 0;
	  float eta = trk->pseudoRapidity();
          heta->Fill(eta);
          //StMcTrack *prnt = trk->parent();
          //if(prnt->geantId() != 0) continue;
          StMcVertex *startv = trk->startVertex();
          float zt = startv->position().z();
          if(TMath::Abs(zt) > 30) continue;
	  double f_sample = f1->Eval(eta);//samplingFraction(eta);
	  dE *= 1./f_sample;
	  if(pt > 0) r = dE/pt;
          int ishw = trk->isShower();
          long pdg = trk->pdgId();
          long id = trk->geantId();
          long key = trk->key();
	  if(id == 7){
             Printf("towerid = %d r=%f, pt=%f ishw = %d, pdg = %ld geantId = %ld key = %ld\n, ", towerId, r, pt, ishw, pdg, id, key);
             Printf("start x=%f, y=%f, z=%f\n", startv->position().x(), startv->position().y(), zt);
             StMcVertex *stopv = trk->stopVertex();
             if(stopv){
               float stopvx = stopv->position().x();
               float stopvy = stopv->position().y();
               float stopvz = stopv->position().z();
               Printf("stop x=%f, y=%f, z=%f\n", stopvx, stopvy, stopvz);
             }
             StSPtrVecMcTrack gTracks = mcEvent->tracks();
             for(int igt = 0; igt < gTracks.size(); igt++){
               StMcTrack *trk_t = gTracks[igt];
      	       StMcTrack *prnt = trk_t->parent();
               if(prnt){
      	         if(prnt->key() == key){
                 int id_t = trk_t->geantId();
                 //if(id_t == 0) continue;
                 float pt_t = trk_t->pt();
                 Printf("id_t=%d, pt_t=%f\n", id_t, pt_t);
                 //return; 
                 }
               }
             }
          }
          else continue;
	  if(flag_print) Printf("id=%d e=%f pt=%f r=%f ishw=%d, pdg=%ld geantId=%ld key=%ld\n", towerId, dE, pt, r, ishw, pdg, id, key);
	  if(tracks.find(key) == tracks.end()){
	    tracks.insert(pair<long, float>(key, dE));
	  }else{
	    tracks[key] += dE;
	  }
	  if(tracks_id.find(key) == tracks_id.end()){
	    tracks_id.insert(pair<long, long>(key, id));
	  }
	  if(tracks_pt.find(key) == tracks_pt.end()){
	    tracks_pt.insert(pair<long, float>(key, pt));
	  }
	  if(tracks_eta.find(key) == tracks_eta.end()){
	    tracks_eta.insert(pair<long, float>(key, eta));
	  }
	}
      }
     // */
     /*
     //detector hits
     StSPtrVecMcCalorimeterHit &dets = module->detectorHits();
     Printf("detector hit size: %d\n", dets.size());
     for(unsigned long ih = 0; ih < dets.size(); ih++){
     StMcCalorimeterHit *hh = dets[ih];
     if(!hh) continue;
     float dE = hh->dE();
     int towerId;
     int sub = hh->sub();
     //if(sub < 0) Print("sub = %d\n", sub);
     emcGeom->getId(im, hh->eta(), sub, towerId);
     //if(flag_print) Printf("detector hits: id=%d e=%f\n", towerId, dE);
     //mcDe[towerId-1] += dE;
	  ////float ee = energy[towerId-1];
	  ////if(dE > 0) Printf("towerId = %d, dE = %f, ee = %f\n", towerId, dE, ee);
	  //if(dE > 0) Printf("towerId = %d, dE = %f\n", towerId, dE);

	  //test
	  float ped = emcTables->pedestal(1, towerId);
	  float rms = emcTables->pedestalRMS(1, towerId);
	  float calib = emcTables->calib(1, towerId);
	  //if(flag_print) 
	  Printf("id=%d ped=%f, rms=%f, calib=%f\n", towerId, ped, rms, calib);
	  //double f_photo = MipPhotoElectrons/MipEnergyDeposit;
	  //double n_photo = dE * f_photo;
	  float eta; emcGeom->getEta(im, hh->eta(), eta);
	  Printf("sub eta=%d, eta=%f\n", hh->eta(), eta);
	  double f_sample = samplingFraction(eta);
	  Printf("f_sample = %f\n", f_sample);
	  //StEmcPmtSimulator simu(kBarrelEmcTowerId, StEmcVirtualSimulator::kPrimarySecondaryFullMode);
	  StEmcPmtSimulator simu(kBarrelEmcTowerId, StEmcVirtualSimulator::kPrimaryOnlyMode);
	  simu.setEmbeddingMode(false);
	  simu.setTables(emcTables);
	  simu.setCalibScale(1.0);
	  simu.setCalibSpread(0);
	  simu.setMaximumAdc(4095);
	  simu.setMaximumAdcSpread(0);
	  
	  StEmcRawHit *raw = simu.makeRawHit(hh);
	  float ee = raw->energy();
	  h->Fill(dE, ee);
	  Printf("id=%d dE=%f ee=%f\n", towerId, dE, ee);
	  //StPmtSignal pmt;
	}
     */
    }
    
    //Printf("nhits=%d, sum=%d, nmodules=%d \n", nhits, nsum, nmodules);
    /*
      for(int it = 0; it < 4800; it++){
      float diff = energy[it] - mcDe[it];
      if(mcDe[it] > 0) Printf("towerId = %d, mu = %f, mc = %f, diff = %f\n", it+1, energy[it], mcDe[it], diff);
      }
    */
    Printf("track size: %u %u %u\n", tracks.size(), tracks_id.size(), tracks_pt.size());

    for(map<long, long>::const_iterator it = tracks_id.begin(); it != tracks_id.end(); it++){
      long key = it->first;
      long id = it->second;
      float e = tracks[key];
      float pt = tracks_pt[key];
      float r = 0;
      if(pt > 0) r = e/pt;
      if(id == 1){
	hph->Fill(pt, r);
	hpheta->Fill(eta, r);
	hph2d->Fill(pt, r);
      }
      else if(id == 2 || id == 3)
	he->Fill(pt, r);
      else if(id == 7 || id == 8 || id == 9)
	hpi->Fill(pt, r);
      else if(id = 10 || id == 11 || id == 12)
	hk->Fill(pt, r);
      heta->Fill(tracks_eta[key]);
    }
  }
  fout->cd();
  f1->Write(f1->GetName());
  fout->Write();
  fout->Close();
}
