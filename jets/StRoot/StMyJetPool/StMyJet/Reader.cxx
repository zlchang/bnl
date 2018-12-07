#include <utility>
#include <map>
#include <fstream>
using namespace std;
#include "TObjArray.h"
#include "TMath.h"
#include "TFile.h"

#include "Reader.h"
#include "trgUtilities.h"
#include"StSpinPool/StJetEvent/StJetEvent.h"
#include"StSpinPool/StJetEvent/StJetVertex.h"
#include"StSpinPool/StJetEvent/StJetCandidate.h"
#include"StSpinPool/StJetEvent/StJetTrack.h"
#include"StSpinPool/StJetEvent/StJetTower.h"

#include"StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include"StSpinPool/StJetSkimEvent/StPythiaEvent.h"

#include "StSpinPool/StUeEvent/StUeOffAxisConesEvent.h"
//#include "StSpinPool/StUeEvent/StUeEvent.h"
#include "StSpinPool/StUeEvent/StUeVertex.h"
#include "StSpinPool/StUeEvent/StUeJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisCones.h"

#include "StMyJetPool/StMyJetSpin/MyJetMC.h"
//#include "StMyJetPool/StMyJetSpin/MyJetYield.h"
#include "StMyJetPool/StMyJetSpin/MyJetSpin.h"
#include "StMyJetPool/StMyJetSpin/MyJetQA.h"
#include "StMyJetPool/StMyJetSpin/MyEventQA.h"
#include "StMyJetPool/StMyJetSpin/MyTrigQA.h"
#include "StMyJetPool/StMyDef/MyDef.h"

#include "MyJet.h"
#include "StMyJetPool/StMyDef/funcUtilities.h"

//Reader::Reader(StJetEvent *jetevnt, StJetEvent *parton, StJetEvent *particle, StJetSkimEvent *skimevnt, StUeEvent *ueocevnt, StUeEvent *particleueoc)
Reader::Reader(StJetEvent *jetevnt, StJetEvent *parton, StJetEvent *particle, StJetSkimEvent *skimevnt, StUeOffAxisConesEvent *ueocevnt, StUeOffAxisConesEvent *particleueoc)
{
    mJetEvent = jetevnt;
    mJetPartonEvent = parton;
    mJetParticleEvent = particle;
    mJetSkimEvent = skimevnt;
    mJetUeEvent = ueocevnt;
    mJetParticleUeEvent = particleueoc;
    mMcFlag = false;
    mFlag = false;
    mBadBxFile = "/star/u/zchang/2013-05-jets/dev/bad_bunches/13095016.dat"; 
    mOption = TString("JP0,JP1,JP2");
}
int Reader::Init()
{
  mFile = new TFile(mOutput, "recreate");
  mFile->cd();
  //  mTrigger.insert(std::make_pair(380401, string("JP0"));
  //  mTrigger.insert(std::make_pair(380402, string("JP1"));
  //  mTrigger.insert(std::make_pair(380403, string("JP2"));
  mTrig = new MyTrigQA("P13ib");
  mTriggers.push_back("JP0");
  mTriggers.push_back("JP1");
  mTriggers.push_back("JP2");
  
  for(vector<string>::const_iterator it = mTriggers.begin(); it < mTriggers.end(); it++){
    if(mOption.Contains(*it)){
      mMapJetQa.insert(std::make_pair(*it, new MyJetQA((*it).c_str())));
      //mMapJetQaCrr.insert(std::make_pair(*it, new MyJetQA(((*it)+string("Crr")).c_str())));
      mMapEventQa.insert(std::make_pair(*it, new MyEventQA((*it).c_str())));
    }
  }
  if(!mMcFlag){
    //data
    for(vector<string>::const_iterator it = mTriggers.begin(); it < mTriggers.end(); it++){
      if(mOption.Contains(*it)){
	mMapJetSpin.insert(std::make_pair(*it, new MyJetSpin((*it).c_str())));
	//mMapJetSpinCrr.insert(std::make_pair(*it, new MyJetSpin(((*it)+string("Crr")).c_str())));
      }
    }
    mJetSpin = new MyJetSpin("Cmb");
    //  mJetSpinCrr = new MyJetSpin("CmbCrr");
    //  ifstream inBadBxFile(Form("%s/%d.dat",mBadBxFile, mRun));
    ifstream inBadBxFile(mBadBxFile);
    int badbx;
    while(inBadBxFile >> badbx){
      mBadBx.push_back(badbx);
      cout<<"badbx: "<<badbx<<endl;
    }
    inBadBxFile.close();
  }else{
    for(vector<string>::const_iterator it = mTriggers.begin(); it < mTriggers.end(); it++){
      if(mOption.Contains(*it)){
	mMapJetMc.insert(std::make_pair(*it, new MyJetMC((*it).c_str())));
        //jet yield
        //mMapJetYield.insert(std::make_pair(*it, new MyJetYield((*it).c_str())));
      }
    }
    mJetParton = new MyJetPar("Parton");
    mJetParticle = new MyJetPar("Particle");
    //yield
    //mJetYieldParton = new MyJetYieldPar("Parton");
    //mJetYieldParticle = new MyJetYieldPar("Particle");
  }
  return 1;
}
int Reader::Make(int iEvent)
//(StJetEvent *jetevnt, StJetEvent *parevnt, StJetSkimEvent *skimevnt, StUeOffAxisConesEvent *ueocevnt)
{
  float weight = 1.0;
  if(mMcFlag){
    const StPythiaEvent *pythia = mJetSkimEvent->mcEvent();
    float partonicPt = pythia->pt();
    weight = partonicWeight(partonicPt);
  }
  
  if(!mMcFlag){
    int spinbit= mJetSkimEvent->spin4usingBx48();
    int bx = mJetSkimEvent->bx7();
    if(bx >= 0 && bx <= 119){
      bx = (bx+1)%120;
    }
    if(!check_badbx(bx, mBadBx)) return 0;
    if(!check_spinbit(spinbit)) return 0;
  }else{
    //fillMcEvent(mJetPartonEvent, mJetParton, mJetYieldParton, weight);
    fillMcEvent(mJetPartonEvent, mJetParton, weight);
    //fillMcEvent(mJetParticleEvent, mJetParticle, mJetYieldParticle, weight);
    fillMcEvent(mJetParticleEvent, mJetParticle, weight);
  }
  
  map<string, int>mapTrigShouldFired;
  map<string, int>mapTrigDidFired;
  readTrigMap(mapTrigShouldFired, mapTrigDidFired);

  fillVtx(mapTrigShouldFired, mapTrigDidFired);

  vector<MyJet*> arr = findMyJet();
  //cout<<iEvent<<" find jets: "<<arr.size()<<endl;
  
  int numJet = 0;
  MyJet *jetmax[2] = {0x0, 0x0};
  string trgmax[2] = {"", ""};
  cout<<iEvent<<" finding jets ..."<<endl;
  countMyJet(arr, mapTrigShouldFired, mapTrigDidFired, numJet, jetmax, trgmax, mFlag);
  cout<<numJet<<" jets found from "<<arr.size()<<endl;
  if(!mMcFlag){
    fillMyJetSpin(numJet, jetmax, trgmax, mMapJetSpin, mJetSpin, mFlag);
    fillMyJetQA(numJet, jetmax, trgmax, mMapJetQa, mFlag);
  }else{
    float z = mJetEvent->vertex()->position().z();
    float zpar = mJetPartonEvent->vertex()->position().z();

    cout<<"z(detector): "<<z<<" z(par): "<<zpar<<endl;
    if(TMath::Abs(z-zpar) < 2.0){
      fillMyJetQA(numJet, jetmax, trgmax, mMapJetQa, mFlag, weight);
      //fillMyJetMc(numJet, jetmax, trgmax, mMapJetMc, mMapJetYield, mFlag, weight);
      fillMyJetMc(numJet, jetmax, trgmax, mMapJetMc, mFlag, weight);
      for(int ijet = 0; ijet < numJet; ijet++){
        float pt = jetmax[ijet]->pt();
        float dpt = jetmax[ijet]->dpt();
        cout<<"pt: "<<pt<<" dpt: "<<dpt<<endl;
        if(mFlag) pt -= dpt; 
        mMapJetMc[trgmax[ijet]]->GetJetPtNoVtxProf()->Fill(pt, true, weight);
      }
    }else{
      for(int ijet = 0; ijet < numJet; ijet++){
        float pt = jetmax[ijet]->pt();
        float dpt = jetmax[ijet]->dpt();
        if(mFlag) pt -= dpt; 
        mMapJetMc[trgmax[ijet]]->FillJetPtNoVtx(pt, weight);
        mMapJetMc[trgmax[ijet]]->GetJetPtNoVtxProf()->Fill(pt, false, weight);
      }
    }
  }
  return 1;
}
int Reader::Finish()
{
  mFile->cd();
  mTrig->Write("TrigQA");
  if(!mMcFlag){
    mJetSpin->fill_error();
    mJetSpin->Write("CmbJetSpin");
//    mJetSpinCrr->fill_error();
//    mJetSpinCrr->Write("CmbJetSpinCrr");
  
    for(map<string,MyJetSpin*>::iterator p = mMapJetSpin.begin(); p != mMapJetSpin.end(); p++){
      string objname = p->first + string("JetSpin");
      p->second->fill_error();
      p->second->Write(objname.c_str());
      cout<<objname<<" has been written"<<endl;
    }

//    for(map<string,MyJetSpin*>::iterator p = mMapJetSpinCrr.begin(); p != mMapJetSpinCrr.end(); p++){
//      string objname = p->first + string("JetSpinCrr");
//      p->second->fill_error();
//      p->second->Write(objname.c_str());
//      cout<<objname<<" has been written"<<endl;
//    }
  }else{
    for(map<string,MyJetMC*>::iterator p = mMapJetMc.begin(); p != mMapJetMc.end(); p++){
      string objname = p->first + string("JetMc");
      //p->second->fill_error();
      p->second->Write(objname.c_str());
      cout<<objname<<" has been written"<<endl;
    }
    //jet yield
    //for(map<string,MyJetYield*>::iterator p = mMapJetYield.begin(); p != mMapJetYield.end(); p++){
      //string objname = p->first + string("JetYield");
      //p->second->fill_error();
      //p->second->Write(objname.c_str());
      //cout<<objname<<" has been written"<<endl;
    //}
    mJetParton->Write("PartonMc");
    mJetParticle->Write("ParticleMc");
    //mJetYieldParton->Write("YieldPartonMc");
    //mJetYieldParticle->Write("YieldParticleMc");
  }
  for(map<string,MyJetQA*>::iterator p = mMapJetQa.begin(); p != mMapJetQa.end(); p++){
    string objname = p->first + string("JetQA");
    //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  
//  for(map<string,MyJetQA*>::iterator p = mMapJetQaCrr.begin(); p != mMapJetQaCrr.end(); p++){
//    string objname = p->first + string("JetQACrr");
    //p->second->fill_error();
//    p->second->Write(objname.c_str());
//    cout<<objname<<" has been written"<<endl;
//  }
  
  for(map<string,MyEventQA*>::iterator p = mMapEventQa.begin(); p != mMapEventQa.end(); p++){
    string objname = p->first + string("EventQa");
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  mFile->Write();
  mFile->Close();
  /**/
  return 1;
}
void Reader::readTrigMap(map<string, int> &mapTrigShouldFired, map<string, int> &mapTrigDidFired)
{
  if(!mMcFlag){
    for(Int_t iSkimTrig = 0; iSkimTrig < mJetSkimEvent->triggers()->GetEntriesFast(); iSkimTrig++){
      StJetSkimTrig *skimTr = (StJetSkimTrig *) mJetSkimEvent->triggers()->At(iSkimTrig);
      //                cout<<skimTr->trigId()<<"\n";
      if(mTrig->CheckId(skimTr->trigId()))
	{
	  string trigname = mTrig->GetTrigName(skimTr->trigId());
	  //            cout<<skimTr->trigId()<<" "<<trigname<<"\n";
	  if(mOption.Contains(trigname))
	    {
	      mapTrigShouldFired[trigname] = skimTr->shouldFire();
	      mapTrigDidFired[trigname] = skimTr->didFire();
	      if(mapTrigDidFired[trigname]) mTrig->Count(trigname);
	    }
	}
    }  
  }else{
    if(mOption.Contains("JP0")){
      mapTrigShouldFired["JP0"] = mJetSkimEvent->JP0();
      mapTrigDidFired["JP0"] = 1;
    }
    if(mOption.Contains("JP1")){
      mapTrigShouldFired["JP1"] = mJetSkimEvent->JP1();
      mapTrigDidFired["JP1"] = 1;
    }
    if(mOption.Contains("JP2")){
      mapTrigShouldFired["JP2"] = mJetSkimEvent->JP2();
      mapTrigDidFired["JP2"] = 1;
    }
  }
}
vector<MyJet*> Reader::findMyJet()
{
  vector<MyJet*> arr;
  if(mJetEvent->numberOfVertices()){
    StJetVertex *vtx = mJetEvent->vertex();
    StUeVertex *ueVtx = 0x0;
    if(mJetUeEvent)  ueVtx = mJetUeEvent->vertex();
    if(vtx->ranking() > 0.){
      //flagVtx = 1;
      float z = vtx->position().z();
      if(TMath::Abs(z) < 90){
	//	TObjArray *arr = new TObjArray;    
	for(int iJet = 0; iJet < vtx->numberOfJets(); iJet++){
	  StJetCandidate *jetcnd = vtx->jet(iJet);
	  //StUeOffAxisConesJet *uejet = dynamic_cast<StUeOffAxisConesJet*> ueVtx->ueJet(iJet);
	  StUeOffAxisConesJet *uejet = 0x0;
          if(ueVtx) uejet = (StUeOffAxisConesJet*) ueVtx->ueJet(iJet);
	  if(jetcnd->rt() > 0.94) continue;
	  if(jetcnd->pt() < 5.) continue;
	  if(jetcnd->sumTrackPt() < 0.5) continue;
	  if(jetcnd->detEta() > 0.9 || jetcnd->detEta() < -0.7) continue;
          if(jetcnd->eta() < -0.9 || jetcnd->eta() > 0.9) continue;
          if(mMcFlag){
            float pt_par = mJetSkimEvent->mcEvent()->pt();
            float pt_d = jetcnd->pt();
            if(pt_par < 4 && pt_d > 15.3) continue;
          }
	  bool flagtrk = false;
          for(int i = 0; i < jetcnd->numberOfTracks(); i++){
            float trkpt = jetcnd->track(i)->pt();
	    if(trkpt > 30.){
              flagtrk = true;
              cout<<"skip jet with track pT: "<<trkpt<<endl;
	      break;
            }
          }
          if(flagtrk) continue;
	  //arr->Add(new MyJet(jetcnd, uejet));
	  arr.push_back(new MyJet(jetcnd, uejet));
	  //		cout<<jetcnd->pt()<<" "<<uejet->pt()<<endl;
	}
      }
    }
  }
  return arr;
}
void Reader::countMyJet(vector<MyJet*> arr, map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, int &numJet, MyJet *jetmax[], string trgmax[], bool flag)
{
  vector<MyJet *> vec = mergesort(arr, flag);
  //cout<<"sorting: "<<vec.size()<<endl;
  for(size_t ivec = 0; ivec < vec.size() && numJet < 2; ivec++){
    //	MyJet *pj = dynamic_cast<MyJet*>arr->At(iarr);
    MyJet *pj = vec.at(ivec);
    //jet
    //StJetCandidate *jc = pj->jet();
    //ue jet
    //StUeOffAxisConesJet *jcue = pj->jetUe();
    //cout<<"pt: "<<pj->pt()<<" jet: "<<jc->pt()<<" ue: "<<jcue->pt()<<endl;
    string trgname(JetCategory(pj, mJetSkimEvent, mapTrigShouldFired, mapTrigDidFired, flag));
    if(trgname != "Out"){
      jetmax[numJet] = pj;
      trgmax[numJet] = trgname;
      numJet++;
    }
  }
}
void Reader::fillMyJetQA(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetQA*> mapJetQa, bool flag, float weight)
{
  int nposvtx = countNPosVtx();
  float vz = getVz();
  //QA
  for(int ijet = 0; ijet < numJet; ijet++){
    float jpt = jetmax[ijet]->pt();
    float jeta = jetmax[ijet]->eta();
    float jphi = jetmax[ijet]->phi();
    float jrt = jetmax[ijet]->rt();
    int jtrkmult = jetmax[ijet]->trackmult();
    int jtwrmult = jetmax[ijet]->towermult();
    float jdptue = jetmax[ijet]->dpt();
    float jtrkmultue = jetmax[ijet]->trackmultUe();
    float jtwrmultue = jetmax[ijet]->towermultUe();
    float jarea = jetmax[ijet]->area();
    //cout<<"ue pt:"<<jdptue<<" trkmult:"<<jtrkmultue<<" twrmult:"<<jtwrmultue<<endl;
    //ue correction
    if(flag){
      jpt -= jdptue;
      cout<<"jet pt: "<<jpt<<" after ue correction: "<<jdptue<<endl;
    }
    float factor = jetmax[ijet]->pt()/jpt;
    cout<<"ue correction factor: "<<factor<<endl;
    mapJetQa[trgmax[ijet]]->FillJetPtEtaPhi(jpt, jeta, jphi, weight);
    mapJetQa[trgmax[ijet]]->FillJetPtRt(jpt, jrt, weight);
    mapJetQa[trgmax[ijet]]->FillJetMult(jpt, jtwrmult, jtrkmult, weight);
    mapJetQa[trgmax[ijet]]->FillJetNPosVtx(jpt, nposvtx, weight);
    mapJetQa[trgmax[ijet]]->FillJetVertexZ(vz, weight);
    mapJetQa[trgmax[ijet]]->FillJetUe(jpt, jdptue, jtwrmultue, jtrkmultue, weight);
    mapJetQa[trgmax[ijet]]->GetJetAreaVsPt()->Fill(jpt, jarea, weight);
    mapJetQa[trgmax[ijet]]->GetJetRtVsEta()->Fill(jeta,jrt, weight);
    for(int i = 0; i < jtrkmult; i++){
      float trkpt = jetmax[ijet]->jet()->track(i)->pt();
      mapJetQa[trgmax[ijet]]->GetJetTrkPt()->Fill(trkpt, weight);
      float jt = jetmax[ijet]->jet()->track(i)->jt();
      mapJetQa[trgmax[ijet]]->GetJetParJt()->Fill(jpt, jt, weight);
      float frag = jetmax[ijet]->jet()->track(i)->frag();
      frag *= factor;
      mapJetQa[trgmax[ijet]]->GetJetParZ()->Fill(jpt, frag, weight);
    }
    for(int i = 0; i < jtwrmult; i++){
      float twrpt = jetmax[ijet]->jet()->tower(i)->pt();
      mapJetQa[trgmax[ijet]]->GetJetTwrPt()->Fill(twrpt, weight);
      //float jt = jetmax[ijet]->jet()->tower(i)->jt();
      //mapJetQa[trgmax[ijet]]->GetJetParJt()->Fill(jt);
    }

    for(int i = 0; i < yNSEL; i++){
      int isel = ySEL[i];
      if(jpt < yPtBins[isel] || jpt < yPtBins[isel+1]) continue;
      for(int j = 0; j < 25; j++){
        float r = 0.02*i + 0.02;
        float prof = jetmax[ijet]->jet()->profile(r);
        mapJetQa[trgmax[ijet]]->GetJetProf(i)->Fill(r, prof, weight);
      }
    }
    if(jetmax[ijet]->jetUe()){
      float pt0 = jetmax[ijet]->jetUe()->cone(0)->pt();
      float pt1 = jetmax[ijet]->jetUe()->cone(1)->pt();
      mapJetQa[trgmax[ijet]]->GetUeDpt3D()->Fill(jpt, pt0, pt1, weight);
      float mult0 = jetmax[ijet]->jetUe()->cone(0)->mult();
      float mult1 = jetmax[ijet]->jetUe()->cone(1)->mult();
      mapJetQa[trgmax[ijet]]->GetUeMult3D()->Fill(jpt, mult0, mult1, weight);
      mapJetQa[trgmax[ijet]]->GetJetUeRt()->Fill(jpt, jrt*factor - jetmax[ijet]->sumtowerUe()/jpt, weight);
      for(int ic = 0; ic < 2; ic++){
        float areaue = 3.1416 * TMath::Power(jetmax[ijet]->jetUe()->cone(ic)->radius(), 2);
        for(int im = 0; im < jetmax[ijet]->jetUe()->cone(ic)->numberOfTracks(); im++){
          float trkpt = jetmax[ijet]->jetUe()->cone(ic)->track(im)->pt();
          mapJetQa[trgmax[ijet]]->GetUeTrkPt()->Fill(jpt, trkpt, weight);
          cout<<"ue track pt: "<<trkpt<<endl;
          float jt = jetmax[ijet]->trackjtUe(ic, im);//->jetUe()->cone(ic)->track(im)->jt();
          cout<<"ue jt: "<<jt<<endl;
          mapJetQa[trgmax[ijet]]->GetJetUeParJt()->Fill(jpt, jt, jarea/areaue*weight);
          float frag = jetmax[ijet]->jetUe()->cone(ic)->track(im)->frag();
          frag *= factor;
          cout<<"ue frag: "<<frag<<endl;
          mapJetQa[trgmax[ijet]]->GetJetUeParZ()->Fill(jpt, frag, jarea/areaue*weight);

        }
        for(int im = 0; im < jetmax[ijet]->jetUe()->cone(ic)->numberOfTowers(); im++){
          mapJetQa[trgmax[ijet]]->GetUeTwrPt()->Fill(jpt, jetmax[ijet]->jetUe()->cone(ic)->tower(im)->pt(), weight);
        }
      }
    }
  }
}
void Reader::fillMyJetSpin(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetSpin*> mapJetSpin, MyJetSpin* JetSpin, bool flag)
{
  int spinbit= mJetSkimEvent->spin4usingBx48();   
  for(int ijet = 0; ijet < numJet; ijet++){
    //jets
    float jpt = jetmax[ijet]->pt();
    float jdpt = jetmax[ijet]->dpt();

    float jtrkmult = jetmax[ijet]->trackmult();
    float jtwrmult = jetmax[ijet]->towermult();
    float jmult = jtrkmult + jtwrmult;

    float jtrkmultue = jetmax[ijet]->trackmultUe();
    float jtwrmultue = jetmax[ijet]->towermultUe();
    float jmultue = jtrkmultue + jtwrmultue;
    //ue correction
    if(flag){ 
	jpt -= jdpt;
        cout<<"jet pt: "<<jpt<<" with ue correction: "<<jdpt<<endl;
    }
    mapJetSpin[trgmax[ijet]]->fill_pt(spinbit ,jpt);
    mapJetSpin[trgmax[ijet]]->fill_dpt(spinbit, jpt, jdpt);
    mapJetSpin[trgmax[ijet]]->FillJetMult(spinbit, jpt, jmult);
    mapJetSpin[trgmax[ijet]]->FillJetMultUe(spinbit, jpt, jmultue);
    JetSpin->fill_pt(spinbit ,jpt);
    JetSpin->fill_dpt(spinbit, jpt, jdpt);
    JetSpin->FillJetMult(spinbit, jpt, jmult);
    JetSpin->FillJetMultUe(spinbit, jpt, jmultue);
  }
  if(numJet == 2){
    float npt[2];
    float ndpt[2];
    for(int i = 0; i < 2; i++){
      npt[i] = jetmax[i]->pt();
      ndpt[i] = jetmax[i]->dpt();
      if(flag){
        npt[i] -= ndpt[i];
        //cout<<"jet pt: "<<jpt<<" with ue correction: "<<jdpt<<endl;
      }
    }
    JetSpin->fill_twojet_pt(spinbit, npt[0], npt[1]);
    if(trgmax[0] == trgmax[1]){
      mapJetSpin[trgmax[0]]->fill_twojet_pt(spinbit, npt[0], npt[1]);
    }
  }
}
void Reader::fillVtx(map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, float weight)
{
  int nposvtx = countNPosVtx();
  float vz = getVz();
  //fill vertex info
  /*
  if(mJetSkimEvent->JP2()){
    mMapJetMc["JP2"]->GetNPosVtx()->Fill(nposvtx);
  }else if(mJetSkimEvent->JP1()){
    mMapJetMc["JP1"]->GetNPosVtx()->Fill(nposvtx);
  }else if(mJetSkimEvent->JP0()){
    mMapJetMc["JP0"]->GetNPosVtx()->Fill(nposvtx);
  }
  */
  if(mapTrigDidFired["JP2"] && mapTrigShouldFired["JP2"]){
    mMapEventQa["JP2"]->GetNPosVtx()->Fill(nposvtx, weight);
    mMapEventQa["JP2"]->GetVertexZ()->Fill(vz, weight);
  }else if(mapTrigDidFired["JP1"] && mapTrigShouldFired["JP1"]){
    mMapEventQa["JP1"]->GetNPosVtx()->Fill(nposvtx, weight);
    mMapEventQa["JP1"]->GetVertexZ()->Fill(vz, weight);
  }
  if(mapTrigDidFired["JP0"] && mapTrigShouldFired["JP0"]){
    mMapEventQa["JP0"]->GetNPosVtx()->Fill(nposvtx, weight);
    mMapEventQa["JP0"]->GetVertexZ()->Fill(vz, weight);
  }
}
int Reader::countNPosVtx()
{
  int nposvtx = 0;
  for(int ivtx = 0; ivtx < mJetEvent->numberOfVertices(); ivtx++){
    StJetVertex *jetVert = mJetEvent->vertex(ivtx);
    if(jetVert->ranking() > 0) nposvtx++;
  }
  return nposvtx;
}
float Reader::getVz()
{
  return mJetEvent->vertex()->position().z();
}
//void Reader::fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetMC*> mapJetMc, map<string, MyJetYield*>mapJetYield, bool flag, float weight)
void Reader::fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], map<string, MyJetMC*> mapJetMc, bool flag, float weight)
{
  //float weight = 1.0;
  //pythia event

  const StPythiaEvent *pythia = mJetSkimEvent->mcEvent();
  //float partonicPt = pythia->pt();
  //weight = partonicWeight(partonicPt);

  float x1 = pythia->x1();
  float x2 = pythia->x2();
  int flavor1 = pythia->particle(4)->GetPdgCode();
  flavor1 = (flavor1 == 21) ? 0 : flavor1;
  int flavor2 = pythia->particle(5)->GetPdgCode();
  flavor2 = (flavor2 == 21) ? 0 : flavor2;
   
  vector<int> partonIndex(numJet); 
  vector<int> particleIndex(numJet); 
  for(int ijet = 0; ijet < numJet; ijet++){
    MyJet *pj = jetmax[ijet];
    string trgname = trgmax[ijet];
    float pt = pj->pt();
    float dpt = pj->dpt();
    //float eta = pj->eta();
    if(flag){
      pt -= dpt;
      cout<<"jet pt: "<<pt<<" with ue correction: "<<dpt<<endl;
    }
    mapJetMc[trgname]->FillJetPt(pt, weight);
    mapJetMc[trgname]->FillJetDetectorPt(pt, weight);
    mapJetMc[trgname]->FillJetDPt(pt, dpt, weight);
    for(int imod = 0; imod < MyJetMC::NMod; imod++){
      float aLL = pythia->aLLNNPDF(imod);
      if(imod == 0) cout<<"x1: "<<x1<<" x2: "<<x2<<" fl1: "<<flavor1<<" fl2: "<<flavor2<<" aLL: "<<aLL<<endl;
      if(aLL > 1.){ aLL = 1.0; cout<<"setting aLL to "<<aLL<<" rep: "<<imod<<endl;}
      if(aLL < -1.){ aLL = -1.0; cout<<"setting aLL to "<<aLL<<" rep: "<<imod<<endl;}
      mapJetMc[trgname]->GetALLDetector(imod)->Fill(pt, aLL, weight);
      mapJetMc[trgname]->GetALLDetector2D(imod)->Fill(pt, aLL, weight);
      //w^2                                                                            
      mapJetMc[trgname]->GetALLDetectorW2(imod)->Fill(pt, aLL, weight*weight);
    }
    mapJetMc[trgname]->GetProcessFlavor()->Fill(flavor1, flavor2, weight);
    //yield
    //mapJetYield[trgname]->GetJetDetectorJet()->Fill(pt, TMath::Abs(eta));  
    StJetCandidate *parton = FindParJet(pj->jet(), mJetPartonEvent, partonIndex[ijet]);
    if(parton){
      float ptpar = parton->pt();
      cout<<"Parton jet found fill "<<pt<<" "<<parton->pt()<<"\n";
      //x, xg                                                                            
      float waLL = pythia->aLLNNPDF(0);
      mapJetMc[trgname]->FillXParton(x1, pt, weight);
      mapJetMc[trgname]->FillXParton(x2, pt, weight);
      if(flavor1 == 0) mapJetMc[trgname]->FillXgParton(x1, pt, waLL*weight);
      if(flavor2 == 0) mapJetMc[trgname]->FillXgParton(x2, pt, waLL*weight);
      mapJetMc[trgname]->FillJetPartonPt(pt, ptpar, weight);
      //                                                                               
      mapJetMc[trgname]->FillJetDiffPartonProf(pt, ptpar-pt, weight);
      mapJetMc[trgname]->GetJetPartonNoMatchProf()->Fill(pt, true, weight);
     //response
     //mapJetYield[trgname]->GetJetPartonResMC()->Fill(pt, ptpar, weight);
     //float etapar = parton->eta(); 
     //mapJetYield[trgname]->GetJetPartonJet()->Fill(ptpar, etapar, weight);
     //mapJetYield[trgname]->FillPartonResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight);
     
    }else{
      mapJetMc[trgname]->FillJetPartonNoMatch(pt, weight);
      mapJetMc[trgname]->GetJetPartonNoMatchProf()->Fill(pt, false, weight);
    }
    //particle
    StJetCandidate *particle = FindParJet(pj->jet(), mJetParticleEvent, particleIndex[ijet]);
    if(particle){
      float ptpar = particle->pt();
      map<string, float> rpho = particle->ueDensity();
      float dptpar = rpho["OffAxisConesR050"]*particle->area();
      cout<<"Particle jet found fill "<<pt<<" "<<particle->pt()<<"\n";
      mapJetMc[trgname]->FillJetParticlePt(pt, ptpar, weight);
      mapJetMc[trgname]->FillJetParticlePtCrr(pt, ptpar-dptpar, weight);
      //                                                                               
      mapJetMc[trgname]->FillJetDiffParticleProf(pt, ptpar-pt, weight);
      mapJetMc[trgname]->FillJetDiffParticleCrrProf(pt, (ptpar-dptpar)-pt, weight);
      mapJetMc[trgname]->GetJetParticleNoMatchProf()->Fill(pt, true, weight);
      //response
      //mapJetYield[trgname]->GetJetParticleResMC()->Fill(pt, ptpar, weight);
      //mapJetYield[trgname]->GetJetParticleCrrResMC()->Fill(pt, ptpar-dptpar, weight);
      //float etapar = particle->eta(); 
      //mapJetYield[trgname]->GetJetParticleJet()->Fill(ptpar, etapar, weight);
      //mapJetYield[trgname]->GetJetParticleJetCrr()->Fill(ptpar-dptpar, etapar, weight);
      //mapJetYield[trgname]->FillParticleResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight);
      //mapJetYield[trgname]->FillParticleCrrResponse(pt, TMath::Abs(eta), ptpar-dptpar, TMath::Abs(etapar), weight);
    }else{
      mapJetMc[trgname]->FillJetParticleNoMatch(pt, weight);
      mapJetMc[trgname]->GetJetParticleNoMatchProf()->Fill(pt, false, weight);
    }
  }
  if(numJet == 2 && trgmax[0] == trgmax[1]){
      MyJet *pj1 = jetmax[0];
      MyJet *pj2 = jetmax[1];
      //string trgname1 = trgmax[0];
      float pt1 = pj1->pt();
      float pt2 = pj2->pt();
      if(flag){
	float dpt1 = pj1->dpt();
	pt1 -= dpt1;
	float dpt2 = pj2->dpt();
	pt2 -= dpt2;
      }
       
      //mapJetMc[trgmax[0]]->FillTwoJetPt(pt1, pt2, weight);
      /*
      if(partonIndex[0] == partonIndex[1] && partonIndex[0] != -1){
	//fill parton two jet
	fillParTwoJet(mapJetYield[trgmax[0]]->GetJetPartonTwoJet(), mJetPartonEvent, partonIndex[0], weight);
      }
      if(particleIndex[0] == particleIndex[1] && particleIndex[0] != -1){
	//fill particle two jet
	fillParTwoJet(mapJetYield[trgmax[0]]->GetJetParticleTwoJet(), mJetParticleEvent, particleIndex[0], weight);
	fillParTwoJet(mapJetYield[trgmax[0]]->GetJetParticleTwoJetCrr(), mJetParticleEvent, particleIndex[0], weight, true);
      }
      //fillParJet(mapJetYield[trgmax[0]]->GetJetPartonJet(), mJetPartonEvent, weight);
      //fillParJet(mapJetYield[trgmax[0]]->GetJetParticleJet(), mJetParticleEvent, weight);
      //fillParJet(mapJetYield[trgmax[0]]->GetJetParticleJetCrr(), mJetParticleEvent, weight, true);
  }
  //else{
  //  for(int ijet = 0; ijet < numJet; ijet++){
  //    if(partonIndex[ijet] != -1){
  //	fillParTwoJet(mapJetYield[trgmax[ijet]]->GetJetPartonTwoJet(), mJetPartonEvent, partonIndex[ijet], weight);
  //    }
  //    if(particleIndex[ijet] != -1){
  //	fillParTwoJet(mapJetYield[trgmax[ijet]]->GetJetParticleTwoJet(), mJetParticleEvent, particleIndex[ijet], weight);
  //	fillParTwoJet(mapJetYield[trgmax[ijet]]->GetJetParticleTwoJetCrr(), mJetParticleEvent, particleIndex[ijet], weight, true);
  //    }
      //fillParJet(mapJetYield[trgmax[ijet]]->GetJetPartonJet(), mJetPartonEvent, weight);
      //fillParJet(mapJetYield[trgmax[ijet]]->GetJetParticleJet(), mJetParticleEvent, weight);
      //fillParJet(mapJetYield[trgmax[ijet]]->GetJetParticleJetCrr(), mJetParticleEvent, weight, true);
  //  }
  //}
  */
  } 
}
//void Reader::fillMcEvent(StJetEvent *jetParEvent, MyJetPar *jetPar, MyJetYieldPar *jetYieldPar, float weight)
void Reader::fillMcEvent(StJetEvent *jetParEvent, MyJetPar *jetPar, float weight)
{
  //int weight = 1.;
  
  const StPythiaEvent *pythia = mJetSkimEvent->mcEvent();
  float partonicPt = pythia->pt();
  weight = partonicWeight(partonicPt);

  float x1 = pythia->x1();
  float x2 = pythia->x2();
  int flavor1 = pythia->particle(4)->GetPdgCode();
  flavor1 = (flavor1 == 21) ? 0 : flavor1;
  int flavor2 = pythia->particle(5)->GetPdgCode();
  flavor2 = (flavor2 == 21) ? 0 : flavor2;
  
  float vz = jetParEvent->vertex()->position().z();
  //particle aLL                                                                                
  //cout<<"vz: "<<vz<<endl;
  if(vz < 90. && vz > -90.){
    int NJets = jetParEvent->vertex()->numberOfJets();
    for(int iJet = 0; iJet < NJets; iJet++){
      StJetCandidate *jetcnd = jetParEvent->vertex()->jet(iJet);
      float pt = jetcnd->pt();
      float eta = jetcnd->eta();
      map<string, float> rpho = jetcnd->ueDensity();
      //float dpt = rpho["OffAxisConesR050"]*jetcnd->area();
      //if(eta > -1.4 && eta < 1.4){
         //jetYieldPar->GetPtEta()->Fill(pt, eta, weight);
         //jetYieldPar->GetPtEtaCrr()->Fill(pt-dpt, eta, weight);
      //}
      if(eta > -0.9 && eta < 0.9){
        jetPar->GetX()->Fill(x1, pt, weight);
        jetPar->GetX()->Fill(x2, pt, weight);
        float waLL = pythia->aLLNNPDF(0);
        if(flavor1 == 0) jetPar->GetXg()->Fill(x1, pt, waLL*weight);
        if(flavor2 == 0) jetPar->GetXg()->Fill(x2, pt, waLL*weight);

        jetPar->GetPtPar()->Fill(pt, pt, weight);
        jetPar->GetPtParW2()->Fill(pt, pt, weight*weight);
        for(int imod = 0; imod < MyJetPar::NMod; imod++){
          float aLL = pythia->aLLNNPDF(imod);
          if(aLL > 1.){ aLL = 1.0; cout<<"setting aLL to "<<aLL<<" rep: "<<imod<<endl;}
          if(aLL < -1.){ aLL = -1.0; cout<<"setting aLL to "<<aLL<<" rep: "<<imod<<endl;}
          //cout<<aLL<<endl;
          //if(imod == 0 && pt > 23 && pt < 24) cout<<pt<<" "<<aLL<<endl;
          jetPar->GetALLPar(imod)->Fill(pt, aLL, weight);
          jetPar->GetALLPar2D(imod)->Fill(pt, aLL, weight);
          //w^2                                                                                
          jetPar->GetALLParW2(imod)->Fill(pt, aLL, weight*weight);
        }
      }
    }
  }
}
/*
void Reader::fillParJet(TH2F *h, StJetEvent *evntpar, float weight, bool flag)
{
  int NJets = evntpar->vertex()->numberOfJets();
  for(int ijet = 0; ijet < NJets; ijet++){
    StJetCandidate *jetpar = evntpar->vertex()->jet(ijet);
    float ptpar = jetpar->pt();
    float etapar = jetpar->eta();
    if(flag){
      map<string, float> rpho = jetpar->ueDensity();
      float dptpar = rpho["OffAxisConesR050"]*jetpar->area();
      ptpar -= dptpar;
    }
    if(etapar < 1.4 && etapar > -1.4) h->Fill(ptpar, etapar, weight);
  }
}
void Reader::fillParTwoJet(TH2F *h, StJetEvent *evntpar, int index, float weight, bool flag)
{
  StJetCandidate *jetpar = evntpar->vertex()->jet(index);
  float ptpar = jetpar->pt();
  float etapar = jetpar->eta();
  if(flag){
    map<string, float> rpho = jetpar->ueDensity();
    float dptpar = rpho["OffAxisConesR050"]*jetpar->area();
    ptpar -= dptpar;
  }
  h->Fill(ptpar, etapar, weight);
  cout<<"find two jets matched to the same particle jet with par pt:"<<ptpar<<endl;
}
*/
