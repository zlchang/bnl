void MyMatchTrack(
		  const char* MuDst = "test.emb.list",//~/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109027/pt11_15_st_zerobias_adc_13109027_raw_1600001_7.MuDst.root",///global/homes/z/zchang/data_embed/13109027/pt11_15_st_zerobias_adc_13109027_raw_4600001_8.MuDst.root",///star/u/zchang/2014-04-embedding/embedding/test.MuDst.root",
		  //const char* MuDst = "/star/u/zchang/public/MuDst/st_physics_13109014_raw_*.MuDst.root",
		  const char* Outfile = "MyMatchTrackToEmc.root",
		  bool flag = true
		  )
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gROOT->Macro("LoadJetMaker.C");

  gSystem->Load("StMyObjs");
  gSystem->Load("StMyUtils");
  gSystem->Load("StMyMatchTrackToEmcHist");
  gSystem->Load("StMyMatchTrackToEmcHist");
  gSystem->Load("StMyMatchTrackToEmcMaker");

  StChain *chain = new StChain;

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",MuDst,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");

  StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  adc->saveAllStEvent(1);
  //  StMuDst* muDst = muDstMaker->muDst();

  StEEmcDbMaker* eemcDb = new StEEmcDbMaker;
 
  //StTriggerFilterMaker *trgfilt = new StTriggerFilterMaker;
  //trgfilt->addTrigger(380403);
 
  StTriggerSimuMaker* simuTrig = new StTriggerSimuMaker;
  simuTrig->useOfflineDB(); 
  if(!flag) simuTrig->setMC(0); // 0=data, 1=simulation, 2=embedding
  else simuTrig->setMC(2);
  simuTrig->useBemc();
  simuTrig->useEemc();
  simuTrig->bemc->setConfig(StBemcTriggerSimu::kOnline);

  StMyTrackProjEmcPtRadius *proj = new StMyTrackProjEmcPtRadius;
  //StMyTrackProjEmcSameTower *proj = new StMyTrackProjEmcSameTower;
  StMyMatchTrackToEmcMaker *matchMaker = new StMyMatchTrackToEmcMaker;
  matchMaker->setMc(flag);
  matchMaker->addVertexCut(new StMyVertexZCut(30.));
  matchMaker->setTrackProjEmc(proj);
  matchMaker->SetOutfile(Outfile);
  chain->Init();
  chain->EventLoop();
  //for(int iEvent = 0; iEvent < 2; iEvent++)
  //for(int iEvent = 0; iEvent < 100; iEvent++)
  /*
  int status;
  int iEvent = 0;
  while((status = chain->Make(iEvent))%10 != kStEOF){
      chain->Clear();
      int status = chain->Make(iEvent);
      if(status == kStSkip) continue;
      //if(status % 10 == kStEOF || status % 10 == kStFatal) break;
      if(status % 10 == kStFatal) break;
      iEvent++;
    }
  */
  //chain->Finish();
}
