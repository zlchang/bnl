const unsigned long nevents = 1;
void readMuDst(
	       const char* mudstfile = "/star/u/zchang/data05/MuDst/13078011/st_physics_13078011_raw_1020001.MuDst.root",
	       //	       const char* mudstfile = "/star/u/zchang/data05/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109015/pt11_15_st_zerobias_adc_13109015_raw_1590002_12.MuDst.root"
	       )
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  //gROOT->Macro("LoadJetMaker.C");
  ///*
  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StEEmcUtil");
  gSystem->Load("StEmcRawMaker");
  gSystem->Load("StEmcADCtoEMaker");
  gSystem->Load("StDetectorDbMaker");
  gSystem->Load("StTpcDb");
  gSystem->Load("StDbUtilities");
  gSystem->Load("StDaqLib");
  gSystem->Load("StDbBroker");
  gSystem->Load("St_db_Maker");
  gSystem->Load("StTrsMaker");	
  //*/
  // Create chain
  StChain* chain = new StChain;
  //
  //     // GEANT reader
  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",mudstfile,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");


  StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  adc->saveAllStEvent(1);


  chain->Init();
  //StTpcDb *tpcdb = StTpcDb::instance();
  //float ifc = tpcdb->Dimensions()->ifcRadius();
  //float ofc = tpcdb->Dimensions()->ofcRadius();
  //Printf("ifc=%lf, ofc=%lf\n", ifc, ofc);
  for (int iEvent = 1; iEvent <= nevents; ++iEvent) {
    chain->Clear();
    int status = chain->Make(iEvent);
    if (status % 10 == kStEOF || status % 10 == kStFatal) break;
    //StTpcDbGeometry *tpcGeom = StTpcDbGeometry::instance(tpcdb);
    //tpcGeom->print(std::cout);

    StMuPrimaryVertex *vertex = StMuDst::primaryVertex();
    int nutrks = vertex->nTracksUsed();
    int notrks = vertex->noTracks();
    int nprim = StMuDst::numberOfPrimaryTracks();
    Printf("nutrks=%d nprim=%d notrks=%d\n", nutrks, nprim, notrks);
    for(int iTrack = 0; iTrack < nprim; iTrack++){
      const StMuTrack* muTrack = StMuDst::primaryTracks(iTrack);
      int nhits = muTrack->nHits();
      int nhitsfit = muTrack->nHitsFit();
      int nhitsposs = muTrack->nHitsPoss();
      bool flag = muTrack->flag();
      Printf("nhits=%d nhitsfit=%d nhitsposs=%d flag=%d\n", nhits, nhitsfit, nhitsposs, flag);
      
    }
  }
}
