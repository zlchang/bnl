void MyMatchTrack(
const char* MuDst = "test.list",//~/data_embed/pp500_production_2012/pt11_15_100_20160201/P13ib.SL13b/2012/109/13109027/pt11_15_st_zerobias_adc_13109027_raw_1600001_7.MuDst.root",///global/homes/z/zchang/data_embed/13109027/pt11_15_st_zerobias_adc_13109027_raw_4600001_8.MuDst.root",///star/u/zchang/2014-04-embedding/embedding/test.MuDst.root",
const char* Outfile = "test.track.root"
//const char* MuDst = "/star/u/zchang/2013-08-trgsimu/MuDst/st_physics_13109014_raw_*.MuDst.root",
//const char* Outfile = "MyMatchTrackToEmc.root"
)
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");
  gROOT->Macro("LoadJetMaker.C");

  gSystem->Load("StMyMatchTrackToEmcHist");
  gSystem->Load("StMyMatchTrackToEmcMaker");

  StChain *chain = new StChain;

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",MuDst,"",1000);
  StMuDbReader *muDstDb = StMuDbReader::instance();

  St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");

  StEmcADCtoEMaker *adc = new StEmcADCtoEMaker;
  adc->saveAllStEvent(1);
  //  StMuDst* muDst = muDstMaker->muDst();

  StMyMatchTrackToEmcMaker *matchMaker = new StMyMatchTrackToEmcMaker;
  matchMaker->SetOutfile(Outfile);
  chain->Init();
  //for(int iEvent = 0; iEvent < 2; iEvent++)
  for(int iEvent = 0; iEvent < 10000000; iEvent++)
    {
      chain->Clear();
      int status = chain->Make(iEvent);
      if(status == kStSkip) continue;
      if(status % 10 == kStEOF || status % 10 == kStFatal) break;
    }
  //chain->Finish();
}
