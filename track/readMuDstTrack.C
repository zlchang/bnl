void readMuDstTrack(//const char* MuDst = "/star/u/zchang/2013-08-trgsimu/MuDst/st_physics_13109014_raw_*.MuDst.root")//,st_physics_13109014_raw_0040001.MuDst.root"
const char* MuDst = "/global/homes/z/zchang/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root"
)
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gROOT->Macro("LoadJetMaker.C");

  StChain *chain = new StChain;

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",MuDst,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");

  StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  adc->saveAllStEvent(1);
  //  StMuDst* muDst = muDstMaker->muDst();
  chain->Init();
  TFile *fout = new TFile("test.root", "recreate");
  TH1D *htrack = new TH1D("htrack", "", 110, -1, 10);
  TH1D *htower = new TH1D("htower", "", 110, -1, 10);
  TH2D *h2d = new TH2D("h2d", ";track p_{T} ;tower e", 110, -1, 10, 100, -1, 10);
  TH2D *h2de = new TH2D("h2de", ";track e;tower e", 110, -1, 10, 100, -1, 10);
  TH1D *hresidue = new TH1D("hresidue", "", 200, -10, 10);
  TH1D *hresTrans = new TH1D("hresTrans", "", 200, -10, 10);
  TH1D *hresTransCut = new TH1D("hresTransCut", "", 200, -10, 10);

  TH1D *hnsige = new TH1D("hnsige",";n#sigma_{e}", 200, -10, 10);
  TH1D *hnsigpi = new TH1D("hnsigpi",";n#sigma_{#pi}", 200, -10, 10);
  TProfile *hprofept = new TProfile("ept", ";track p_{T}", 100, -1, 10);
  TProfile *hprofep = new TProfile("ep", ";track p", 100, -1, 10);
  TH2D *hepvsp = new TH2D("epvsp", ";track p; E/p", 100, -1, 10, 100, 0, 1); 
  TH2D *hepvspt = new TH2D("epvspt", ";track p_{T}; E/p_{T}", 100, -1, 10, 100, 0, 1); 
  //  StMuEvent *muEvt= muDst->event();
  //  cout<<muEvt->eventId()<<" "<<muEvt->runId()<<"\n";
  for(int iEvent = 0; iEvent < 10; iEvent++)
    {
      chain->Clear();
      int status = chain->Make(iEvent);
       if(status == kStSkip) continue;
       if(status % 10 == kStEOF || status % 10 == kStFatal) break;

      //      chain->EventLoop(10);
      StMuPrimaryVertex *vertex = StMuDst::primaryVertex();
      if(!vertex) continue;
      if(vertex->ranking() < 0.1) continue;
      double mag = StMuDst::event()->magneticField()/10.0;
      Printf("mag field = %lf\n", mag);
      return ;
      //emc towers
      double energy[4800];
      StEmcCollection *emc = StMuDst::emcCollection();
      StEmcDetector* bemc = emc->detector(kBarrelEmcTowerId);
      if (bemc) {
	for(unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
              StSPtrVecEmcRawHit& rawHits = bemc->module(m)->hits();
              for(size_t k = 0; k < rawHits.size(); ++k) {
                StEmcRawHit *hit = rawHits[k];
		int towerId = 0;
                StEmcGeom::instance("bemc")->getId(hit->module(), hit->eta(), abs(hit->sub()), towerId);
		if(towerId >=1 && towerId <= 4800) energy[towerId-1] = hit->energy();
              }
         }     
       }
      for(int iTrack = 0; iTrack < StMuDst::numberOfPrimaryTracks(); iTrack++)
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
	  double mom = muTrack->p()->mag();
          if(pt < 0.2) continue;
	  if(eta < -2.5 || eta > 2.5) continue;
          //Printf("pt = %.2lf B = %.2lf", pt, mag);
	  StThreeVectorD momentumAt, positionAt;
          StMuEmcPosition EmcPosition;
          double bemcRadius = 238.6;
          if(EmcPosition.trackOnEmc(&positionAt, &momentumAt, muTrack, mag, bemcRadius)){
	     double exitEta = positionAt.pseudoRapidity();
             double exitPhi = positionAt.phi();
             int exitTowerId = 0;
	     StEmcGeom::instance("bemc")->getId(exitPhi, exitEta, exitTowerId);

	     Printf("eta=%4.3lf, phi=%4.3lf\n", exitPhi, exitEta);
             if(exitTowerId <=0 || exitTowerId > 4800) continue;
             double et = energy[exitTowerId-1];
             Printf("tower Id %d, energy %lf, track pt %.2lf eta %.2lf", exitTowerId, et, pt, eta);
	     htrack->Fill(pt);
	     htower->Fill(et);
             double res0 = et-pt;
             double res1 = eta-pt*TMath::CosH(eta);
             hresidue->Fill(res0);
             hresTrans->Fill(res1);
	     h2d->Fill(pt, et);
	     h2de->Fill(pt*TMath::CosH(eta), et);
             if(et > 0.2) hresTransCut->Fill(res1);

	     //
	     double nsigE = muTrack->nSigmaElectron();
	     hnsige->Fill(nsigE);
	     double nsigPi = muTrack->nSigmaPion();
	     hnsigpi->Fill(nsigPi);
	     if(nsigE < -2. || nsigE > 2.){
	       hprofept->Fill(pt, et/pt);
	       hepvspt->Fill(pt, et/pt);
	       //
	       hprofep->Fill(mom, et/mom);
	       hepvsp->Fill(mom, et/mom);
	     }
           }
	}
      /*
      StMuEmcCollection *muemc = StMuDst::muEmcCollection();
      for(int iTower = 1; iTower < 4801; iTower++)
	{
	  if(muemc->getTowerADC(iTower)>100)
	    cout<<iTower<<" "<<muemc->getTowerADC(iTower)<<endl;
	}

      vector<unsigned int> triggerIds = StMuDst::event()->triggerIdCollection().nominal().triggerIds();
      for(int iTrig = 0; iTrig < triggerIds.size(); iTrig++)
	{ 
	  cout<<triggerIds.at(iTrig)<<" ";
	}
      cout<<"\n";
      */
      /*
      StEmcCollection *emc = StMuDst::emcCollection();//->detector(kBarrelEmcTowerId)->numberOfHits();
      cout<<emc->detector(kBarrelEmcTowerId)->numberOfModules()<<endl;
      cout<<emc->detector(kBarrelEmcTowerId)->module(50)->hits().at(4)->energy()<<endl;
      */
      /*
      cout<<emc->getNPrsHits()<<endl; //Preshower Hits
      for(int iEmcp = 0; iEmcp < emc->getNPoints(); iEmcp++)
	{
	  StMuEmcPoint *emcp = emc->getPoint(iEmcp);
	  cout<<"eta "<<emcp->getEta()<<" phi "<<emcp->getPhi()<<" energy "<<emcp->getEnergy()<<endl;
	}
      */
    }
    fout->Write();
    fout->Close();
}
