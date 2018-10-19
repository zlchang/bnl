void matchTrackToBemc(const char* MuDst = "st_physics_13109014_raw_*.MuDst.root")//,st_physics_13109014_raw_0040001.MuDst.root")
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gROOT->Macro("LoadJetMaker.C");

  StChain *chain = new StChain;

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"/star/u/zchang/2013-08-trgsimu/MuDst/",MuDst,"",1000);
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
  TProfile *hprofept = new TProfile("ept", ";track p_{T}", 110, -1, 10);
  TProfile *hprofep = new TProfile("ep", ";track p", 110, -1, 10);
  TH2D *hepvsp = new TH2D("epvsp", ";track p; E/p", 110, -1, 10, 100, 0, 1); 
  TH2D *hepvspt = new TH2D("epvspt", ";track p_{T}; E/p_{T}", 110, -1, 10, 100, 0, 1); 
  //cluster
  TProfile *hprofeptcls = new TProfile("eptcls", ";track p_{T}; E_{cls}/p_{T}", 110, -1, 10);
  TProfile *hprofepcls = new TProfile("epcls", ";track p;E_{cls}/p", 110, -1, 10);
  TH2D *hepvspcls = new TH2D("epvspcls", ";track p; E_{cls}/p", 110, -1, 10, 100, 0, 1); 
  TH2D *hepvsptcls = new TH2D("epvsptcls", ";track p_{T}; E_{cls}/p_{T}", 110, -1, 10, 100, 0, 1); 
  TProfile *hproffracptcls = new TProfile("fracptcls", ";track p_{T}; frac", 110, -1, 10);
  TProfile *hproffracpcls = new TProfile("fracpcls", ";track p; frac", 110, -1, 10);
  TH2D *hfracptcls = new TH2D("fracptcls2d", ";track p_{T}; frac", 110, -1, 10, 100, 0, 1);
  TH2D *hfracpcls = new TH2D("fracpcls2d", ";track p; frac", 110, -1, 10, 100, 0, 1);

  //others
  TProfile *hprofmatch = new TProfile("matchprof", ";track p_{T} [GeV];matching ratio", 110, -1, 10);
  TProfile *hprofevsd = new TProfile("evsdprof", "; d = #sqrt{#Delta #eta^{2}+#Delta #phi^{2}}; E", 11, -0.01, 0.1);
  TH2D *h2devsd = new TH2D("evsd2d", "; d = #sqrt{#Delta #eta^{2}+#Delta #phi^{2}}; E", 110, -1, 10, 11, -0.01, 0.1);

  StEmcGeom *bemcGeom = StEmcGeom::instance("bemc");
  
  //  StMuEvent *muEvt= muDst->event();
  //  cout<<muEvt->eventId()<<" "<<muEvt->runId()<<"\n";
  //for(int iEvent = 0; iEvent < 1000000; iEvent++)
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
		if(towerId >=1 && towerId <= 4800){
		  energy[towerId-1] = hit->energy();
		}
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
          int charge = muTrack->charge();
          Printf("charge = %d\n", charge);
          //Printf("pt = %.2lf B = %.2lf", pt, mag);
	  StThreeVectorD momentumAt, positionAt;
          StMuEmcPosition EmcPosition;
          //StEmcPosition EmcPosition;
          double bemcRadius = 238.6;
	  //double bemcRadius = 245;
	  //double bemcRadius = 225.405;
          if(EmcPosition.trackOnEmc(&positionAt, &momentumAt, muTrack, mag, bemcRadius)){
	     double exitEta = positionAt.pseudoRapidity();
             double exitPhi = positionAt.phi();
             int exitTowerId = 0;
	     Printf("tower Id %d, exit eta: %.2lf, phi: %.2lf", exitTowerId, exitEta, exitPhi);
	     //StEmcGeom::instance("bemc")->getId(exitPhi, exitEta, exitTowerId);
	     bemcGeom->getId(exitPhi, exitEta, exitTowerId);
	     
             if(exitTowerId <=0 || exitTowerId > 4800){ hprofmatch->Fill(pt, false); continue;}
	     hprofmatch->Fill(pt, true);
             double ee = energy[exitTowerId-1];
             Printf("tower Id %d, energy %lf, track pt %.2lf eta %.2lf", exitTowerId, ee, pt, eta);
	     float beta, bphi;
	     bemcGeom->getEtaPhi(exitTowerId, beta, bphi);
	     Printf("tower Id %d, center eta: %.2lf, phi: %.2lf", exitTowerId, beta, bphi);
	     float dd = TMath::Power(beta-exitEta, 2)+TMath::Power(TVector2::Phi_mpi_pi(bphi-exitPhi), 2);
	     dd = TMath::Sqrt(dd);
	     hprofevsd->Fill(dd, ee/pt);
	     h2devsd->Fill(dd, ee/pt);
	     //
	     htrack->Fill(pt);
	     htower->Fill(ee);
             double res0 = ee-pt;
             double res1 = ee-pt*TMath::CosH(eta);
             hresidue->Fill(res0);
             hresTrans->Fill(res1);
	     h2d->Fill(pt, ee);
	     h2de->Fill(pt*TMath::CosH(eta), ee);
             if(ee > 0.2) hresTransCut->Fill(res1);

	     //
	     double nsigE = muTrack->nSigmaElectron();
	     hnsige->Fill(nsigE);
	     double nsigPi = muTrack->nSigmaPion();
	     hnsigpi->Fill(nsigPi);
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
                        Printf("ntowerId=%d, exitTowerId=%d, nee=%4.3lf, et=%4.3lf\n", ntowerId, exitTowerId, nee, ee);
           
		}
             }
             double fee = 0;
	     if(nsum > 0. || nsum < 0.) fee = emax/nsum;
 	     Printf("fee=%4.3lf\n", fee);
	     if(nsigE < -2. || nsigE > 2.){
	       hprofept->Fill(pt, ee/pt);
	       hepvspt->Fill(pt, ee/pt);
	       //cluster
	       hprofeptcls->Fill(pt, nsum/pt);
	       hepvsptcls->Fill(pt, nsum/pt);
	       hproffracptcls->Fill(pt, fee);
	       hfracptcls->Fill(pt, fee);
	       //momentum
	       hprofep->Fill(mom, ee/mom);
	       hepvsp->Fill(mom, ee/mom);
	       hprofepcls->Fill(mom, nsum/mom);
	       hepvspcls->Fill(mom, nsum/mom);
	       hproffracpcls->Fill(mom, fee);
	       hfracpcls->Fill(mom, fee);
	       //cluster

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
