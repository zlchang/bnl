#include <utility>
#include <map>
#include <fstream>
using namespace std;
#include "TObjArray.h"
#include "TMath.h"
#include "TFile.h"

#include "ReaderCrs.h"
#include "trgUtilities.h"
#include"StSpinPool/StJetEvent/StJetEvent.h"
#include"StSpinPool/StJetEvent/StJetVertex.h"
#include"StSpinPool/StJetEvent/StJetCandidate.h"
#include"StSpinPool/StJetEvent/StJetTrack.h"
#include"StSpinPool/StJetEvent/StJetTower.h"

#include"StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include"StSpinPool/StJetSkimEvent/StPythiaEvent.h"

//#include "StSpinPool/StUeEvent/StUeEvent.h"
#include "StSpinPool/StUeEvent/StUeOffAxisConesEvent.h"
#include "StSpinPool/StUeEvent/StUeVertex.h"
#include "StSpinPool/StUeEvent/StUeJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisCones.h"

#include "StMyJetPool/StMyJetSpin/MyJetYield.h"
#include "StMyJetPool/StMyJetSpin/MyJetTest.h"
#include "StMyJetPool/StMyJetSpin/MyJetQA.h"
#include "StMyJetPool/StMyJetSpin/MyJetRes.h"
#include "StMyJetPool/StMyJetSpin/MyEventQA.h"
#include "StMyJetPool/StMyJetSpin/MyTrigQA.h"
#include "StMyJetPool/StMyDef/MyDef.h"
#include "StMyJetPool/StMyDef/MyFun.h"

#include "MyJet.h"
#include "StMyJetPool/StMyDef/funcUtilities.h"
//pdf
#include "StMyJetPool/StMyPDF/StMyPDF.h"
//ReaderCrs::ReaderCrs(StJetEvent *jetevnt, StJetEvent *parton, StJetEvent *particle, StJetSkimEvent *skimevnt, StUeEvent *ueocevnt, StUeEvent *particleueoc)
ReaderCrs::ReaderCrs(StJetEvent *jetevnt, StJetEvent *parton, StJetEvent *particle, StJetSkimEvent *skimevnt, StUeOffAxisConesEvent *ueocevnt, StUeOffAxisConesEvent *particleueoc)
{
    mJetEvent = jetevnt;
    mJetPartonEvent = parton;
    mJetParticleEvent = particle;
    mJetSkimEvent = skimevnt;
    mJetUeEvent = ueocevnt;
    mJetParticleUeEvent = particleueoc;
    mMcFlag = false;
    mFlag = 0;
    mFlagSmear = 0;
    mBadBxFile = "/star/u/zchang/2013-05-jets/dev/bad_bunches/13095016.dat"; 
    mOption = TString("JP0,JP1,JP2");
}
int ReaderCrs::Init()
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
    ifstream inBadBxFile(mBadBxFile);
    int badbx;
    while(inBadBxFile >> badbx){
      mBadBx.push_back(badbx);
      cout<<"badbx: "<<badbx<<endl;
    }
    inBadBxFile.close();
  }else{
    //yield
    mJetYieldParton = new MyJetYieldPar("Parton");
    mJetYieldParticle = new MyJetYieldPar("Particle");
    //test
    mJetTestParton = new MyJetTestPar("TestParton");
    mJetTestParticle = new MyJetTestPar("TestParticle");
    for(vector<string>::const_iterator it = mTriggers.begin(); it < mTriggers.end(); it++){
      if(mOption.Contains(*it)){
	//response
	mMapJetPartonRes.insert(std::make_pair(*it, new MyJetRes2D(Form("%sParton", (*it).c_str()))));
	mMapJetParticleRes.insert(std::make_pair(*it, new MyJetRes2D(Form("%sParticle", (*it).c_str()))));
      }
    }
    mCmbJetPartonRes = new MyJetRes2D("CmbParton");
    mCmbJetParticleRes = new MyJetRes2D("CmbParticle");
    //pdf
    //mPdf = new StMyPDF;
    //for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
    //mPdfnn[iset] = new StMyPDF("NNPDF30_lo_as_0130", iset);
    //}
  }
  for(vector<string>::const_iterator it = mTriggers.begin(); it < mTriggers.end(); it++){
    if(mOption.Contains(*it)){
      //jet yield
      mMapJetYield.insert(std::make_pair(*it, new MyJetYield((*it).c_str())));
      //test
      mMapJetTest.insert(std::make_pair(*it, new MyJetTest(Form("Test%s", (*it).c_str()))));
    }
  }
  mCmbJetYield = new MyJetYield("Cmb");
  //test
  mCmbJetTest = new MyJetTest("TestCmb");
  
  return 1;
}
int ReaderCrs::Make(int iEvent)
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
  countMyJet(arr, mapTrigShouldFired, mapTrigDidFired, numJet, jetmax, trgmax, mFlag, mFlagSmear);
  cout<<numJet<<" jets found from "<<arr.size()<<endl;
  if(!mMcFlag){
    fillMyJetQA(numJet, jetmax, trgmax, weight);
    fillMyJetYield(numJet, jetmax, trgmax, weight);
    //
  }else{
    //fill particle and parton jets before vertex matching
    fillMcEvent(mJetPartonEvent, mJetYieldParton, false, weight);
    fillMcEvent(mJetParticleEvent, mJetYieldParticle, mFlag > 0, weight);
    //
    if(fillMyJetVtx(numJet, jetmax, trgmax, weight)){
      fillMyJetQA(numJet, jetmax, trgmax, weight);
      fillMyJetMc(numJet, jetmax, trgmax, weight);
      fillMyJetYield(numJet, jetmax, trgmax, weight);
    }
  }
  fillMyJetTest(numJet, jetmax, trgmax, weight);
  return 1;
}
bool ReaderCrs::fillMyJetVtx(int numJet, MyJet *jetmax[], string trgmax[], float weight)
{
  float z = mJetEvent->vertex()->position().z();
  float zpar = mJetPartonEvent->vertex()->position().z();
  cout<<"z(detector): "<<z<<" z(par): "<<zpar<<endl;
  bool match = TMath::Abs(z-zpar) < 2.0;
  //vertex matching ratios
  for(int ijet = 0; ijet < numJet; ijet++){
    //float pt = smearJetPt(jetmax[ijet], mFlag, mFlagSmear);
    ////float pt = jetmax[ijet]->pt();
    //float eta = jetmax[ijet]->eta();
    //mMapJetYield[trgmax[ijet]]->GetJetVertexProf()->Fill(pt, TMath::Abs(eta), match, weight);
    //mCmbJetYield->GetJetVertexProf()->Fill(pt, TMath::Abs(eta), match, weight);
  }
  return match;
}
bool ReaderCrs::fillMyJetYield(int numJet, MyJet *jetmax[], string trgmax[], float weight)
{
  for(int ijet = 0; ijet < numJet; ijet++){
    string trgname = trgmax[ijet];
    //float pt = jetmax[ijet]->pt();
    float pt = smearJetPt(jetmax[ijet], mFlag, mFlagSmear);
    float eta = jetmax[ijet]->eta();
    float rt = jetmax[ijet]->rt();
    //float dpt = jetmax[ijet]->dpt();
    //float rtue = jetmax[ijet]->rtUe();
    //
    //cout<<"pt: "<<pt<<" dpt: "<<dpt<<endl;
    //Printf("jet pt: %lf, rt: %lf, dpt: %lf, rtue: %lf", pt, rt, dpt, rtue);
    //if(mFlag > 0) pt -= dpt*smearPtFrac(rtue)*mFlagSmear*mFlag; 
    //if(mFlagSmear){
    //float spt = jetmax[ijet]->pt()*smearPtFrac(rt)*mFlagSmear;
    //pt += spt;
    //pt = pt+pt*smearPtFrac(rt)*mFlagSmear;
    //dpt = dpt+dpt*smearPtFrac(rtue)*mFlagSmear;
    //pt = pt - dpt*mFlag;
    //Printf("jet pt: %lf, with smear: %lf", pt);
    //} 
    mMapJetYield[trgname]->FillJetPtEtaRt(pt, TMath::Abs(eta), rt, weight);  
    mCmbJetYield->FillJetPtEtaRt(pt, TMath::Abs(eta), rt, weight);  
  }
  if(numJet == 2){
    MyJet *pj1 = jetmax[0];
    MyJet *pj2 = jetmax[1];
    //string trgname1 = trgmax[0];
    //float pt1 = pj1->pt();
    float pt1 = smearJetPt(pj1, mFlag, mFlagSmear);
    float eta1 = pj1->eta();
    //float pt2 = pj2->pt();
    float pt2 = smearJetPt(pj2, mFlag, mFlagSmear);
    float eta2 = pj2->eta();
    
    mCmbJetYield->FillJetTwoJet(pt1, TMath::Abs(eta1), pt2, TMath::Abs(eta2), weight);    
    if(trgmax[0] == trgmax[1]){
      mMapJetYield[trgmax[0]]->FillJetTwoJet(pt1, TMath::Abs(eta1), pt2, TMath::Abs(eta2), weight);
    }
  }
  return true;
}
bool ReaderCrs::fillMyJetTest(int numJet, MyJet *jetmax[], string trgmax[], float weight)
{
  for(int ijet = 0; ijet < numJet; ijet++){
    string trgname = trgmax[ijet];
    //float pt = jetmax[ijet]->pt();
    float pt = smearJetPt(jetmax[ijet], mFlag, mFlagSmear);
    float eta = jetmax[ijet]->eta();
    float phi = jetmax[ijet]->phi();
    mMapJetTest[trgname]->GetJetPt()->Fill(pt, weight);
    mMapJetTest[trgname]->GetJetEta()->Fill(eta, weight);
    mMapJetTest[trgname]->GetJetPhi()->Fill(phi, weight);
    //
    mCmbJetTest->GetJetPt()->Fill(pt, weight);
    mCmbJetTest->GetJetEta()->Fill(eta, weight);
    mCmbJetTest->GetJetPhi()->Fill(phi, weight);

  }
  if(numJet == 2){
    MyJet *pj1 = jetmax[0];
    MyJet *pj2 = jetmax[1];
    //float pt1 = pj1->pt();
    float pt1 = smearJetPt(pj1, mFlag, mFlagSmear);
    //float pt2 = pj2->pt();
    float pt2 = smearJetPt(pj2, mFlag, mFlagSmear);
    //test
    mCmbJetTest->GetTwoJet()->Fill(pt1, pt2, weight);
    if(trgmax[0] == trgmax[1]){
      mMapJetTest[trgmax[0]]->GetTwoJet()->Fill(pt1, pt2, weight);
    }
  }
  return true;
}
int ReaderCrs::Finish()
{
  mFile->cd();
  mTrig->Write("TrigQA");
  if(mMcFlag){
    //mc
    for(map<string,MyJetRes2D*>::iterator p = mMapJetPartonRes.begin(); p != mMapJetPartonRes.end(); p++){
      string objname = p->first + string("JetPartonRes");
      //p->second->fill_error();
      p->second->Write(objname.c_str());
      cout<<objname<<" has been written"<<endl;
    }
    mCmbJetPartonRes->Write("CmbJetPartonRes");
    mJetYieldParton->Write("YieldPartonMc");
    mJetTestParton->Write("TestPartonMc");
    //particle
    for(map<string,MyJetRes2D*>::iterator p = mMapJetParticleRes.begin(); p != mMapJetParticleRes.end(); p++){
      string objname = p->first + string("JetParticleRes");
      //p->second->fill_error();
      p->second->Write(objname.c_str());
      cout<<objname<<" has been written"<<endl;
    }
    mCmbJetParticleRes->Write("CmbJetParticleRes");
    mJetYieldParticle->Write("YieldParticleMc");
    mJetTestParticle->Write("TestParticleMc");

  }
  
  //test
  for(map<string,MyJetTest*>::iterator p = mMapJetTest.begin(); p != mMapJetTest.end(); p++){
    string objname = p->first + string("JetTest");
      //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  mCmbJetTest->Write("CmbJetTest");
  //yield
  for(map<string,MyJetYield*>::iterator p = mMapJetYield.begin(); p != mMapJetYield.end(); p++){
    string objname = p->first + string("JetYield");
    //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  mCmbJetYield->Write("CmbJetYield");
  //qa
  for(map<string,MyJetQA*>::iterator p = mMapJetQa.begin(); p != mMapJetQa.end(); p++){
    string objname = p->first + string("JetQA");
    //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  //event qa
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
void ReaderCrs::readTrigMap(map<string, int> &mapTrigShouldFired, map<string, int> &mapTrigDidFired)
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
vector<MyJet*> ReaderCrs::findMyJet()
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
	  MyJet *myjet = new MyJet(jetcnd, uejet);
	  if(mFlag){
	    float pt = myjet->pt();
	    float dpt = myjet->dpt();  
	    cout<<"candidate pt: "<<pt<<" dpt: "<<dpt<<endl;
	    if(checkpt(pt) - checkpt(pt-dpt) > 2){
	      delete myjet; 
	      continue;
	    }
	  }
	  arr.push_back(myjet);
	  //cout<<jetcnd->pt()<<" "<<uejet->pt()<<endl;
	}
      }
    }
  }
  return arr;
}
void ReaderCrs::countMyJet(vector<MyJet*> arr, map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, int &numJet, MyJet *jetmax[], string trgmax[], float flag, float smear)
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
    JetPars pars(yPtBins[0], yPtBins[12], yPtBins[2], yPtBins[15], yPtBins[6]);
    string trgname(JetCategory(pj, mJetSkimEvent, mapTrigShouldFired, mapTrigDidFired, pars, flag, smear));
    if(trgname != "Out"){
      jetmax[numJet] = pj;
      trgmax[numJet] = trgname;
      numJet++;
    }
  }
}
bool ReaderCrs::fillMyJetQA(int numJet, MyJet *jetmax[], string trgmax[], float weight)
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
    float jrtue = jetmax[ijet]->rtUe();
    float jarea = jetmax[ijet]->area();

    //cout<<"ue pt:"<<jdptue<<" trkmult:"<<jtrkmultue<<" twrmult:"<<jtwrmultue<<endl;
    //ue correction
    jpt = jpt+jpt*smearPtFrac(jrt)*mFlagSmear;
    jdptue = jdptue+jdptue*smearPtFrac(jrtue)*mFlagSmear;
    jpt = jpt - jdptue*mFlag;
    //if(mFlag > 0){
      //jpt -= jdptue*mFlag;
      //cout<<"jet pt: "<<jpt<<" after ue correction: "<<jdptue<<endl;
    //}
    float factor = jetmax[ijet]->pt()/jpt;
    //cout<<"ue correction factor: "<<factor<<endl;
    mMapJetQa[trgmax[ijet]]->FillJetPtEtaPhi(jpt, jeta, jphi, weight);
    mMapJetQa[trgmax[ijet]]->FillJetPtRt(jpt, jrt, weight);
    mMapJetQa[trgmax[ijet]]->FillJetMult(jpt, jtwrmult, jtrkmult, weight);
    mMapJetQa[trgmax[ijet]]->FillJetNPosVtx(jpt, nposvtx, weight);
    mMapJetQa[trgmax[ijet]]->FillJetVertexZ(vz, weight);
    mMapJetQa[trgmax[ijet]]->FillJetUe(jpt, jdptue, jtwrmultue, jtrkmultue, weight);
    mMapJetQa[trgmax[ijet]]->GetJetAreaVsPt()->Fill(jpt, jarea, weight);
    mMapJetQa[trgmax[ijet]]->GetJetRtVsEta()->Fill(jeta,jrt, weight);
    for(int i = 0; i < jtrkmult; i++){
      float trkpt = jetmax[ijet]->jet()->track(i)->pt();
      mMapJetQa[trgmax[ijet]]->GetJetTrkPt()->Fill(trkpt, weight);
      float jt = jetmax[ijet]->jet()->track(i)->jt();
      mMapJetQa[trgmax[ijet]]->GetJetParJt()->Fill(jpt, jt, weight);
      float frag = jetmax[ijet]->jet()->track(i)->frag();
      frag *= factor;
      mMapJetQa[trgmax[ijet]]->GetJetParZ()->Fill(jpt, frag, weight);
    }
    for(int i = 0; i < jtwrmult; i++){
      float twrpt = jetmax[ijet]->jet()->tower(i)->pt();
      mMapJetQa[trgmax[ijet]]->GetJetTwrPt()->Fill(twrpt, weight);
      //float jt = jetmax[ijet]->jet()->tower(i)->jt();
      //mMapJetQa[trgmax[ijet]]->GetJetParJt()->Fill(jt);
    }

    for(int i = 0; i < yNSEL; i++){
      int isel = ySEL[i];
      if(jpt < yPtBins[isel] || jpt < yPtBins[isel+1]) continue;
      for(int j = 0; j < 25; j++){
        float r = 0.02*i + 0.02;
        float prof = jetmax[ijet]->jet()->profile(r);
        mMapJetQa[trgmax[ijet]]->GetJetProf(i)->Fill(r, prof, weight);
      }
    }
    if(jetmax[ijet]->jetUe()){
      float pt0 = jetmax[ijet]->jetUe()->cone(0)->pt();
      float pt1 = jetmax[ijet]->jetUe()->cone(1)->pt();
      mMapJetQa[trgmax[ijet]]->GetUeDpt3D()->Fill(jpt, pt0, pt1, weight);
      float mult0 = jetmax[ijet]->jetUe()->cone(0)->mult();
      float mult1 = jetmax[ijet]->jetUe()->cone(1)->mult();
      mMapJetQa[trgmax[ijet]]->GetUeMult3D()->Fill(jpt, mult0, mult1, weight);
      mMapJetQa[trgmax[ijet]]->GetJetUeRt()->Fill(jpt, jrt*factor - jetmax[ijet]->sumtowerUe()/jpt, weight);
      for(int ic = 0; ic < 2; ic++){
        float areaue = 3.1416 * TMath::Power(jetmax[ijet]->jetUe()->cone(ic)->radius(), 2);
        for(int im = 0; im < jetmax[ijet]->jetUe()->cone(ic)->numberOfTracks(); im++){
          float trkpt = jetmax[ijet]->jetUe()->cone(ic)->track(im)->pt();
          mMapJetQa[trgmax[ijet]]->GetUeTrkPt()->Fill(jpt, trkpt, weight);
          //cout<<"ue track pt: "<<trkpt<<endl;
          float jt = jetmax[ijet]->trackjtUe(ic, im);//->jetUe()->cone(ic)->track(im)->jt();
          //cout<<"ue jt: "<<jt<<endl;
          mMapJetQa[trgmax[ijet]]->GetJetUeParJt()->Fill(jpt, jt, jarea/areaue*weight);
          float frag = jetmax[ijet]->jetUe()->cone(ic)->track(im)->frag();
          frag *= factor;
          //cout<<"ue frag: "<<frag<<endl;
          mMapJetQa[trgmax[ijet]]->GetJetUeParZ()->Fill(jpt, frag, jarea/areaue*weight);

        }
        for(int im = 0; im < jetmax[ijet]->jetUe()->cone(ic)->numberOfTowers(); im++){
          mMapJetQa[trgmax[ijet]]->GetUeTwrPt()->Fill(jpt, jetmax[ijet]->jetUe()->cone(ic)->tower(im)->pt(), weight);
        }
      }
    }
  }
  return true;
}
void ReaderCrs::fillVtx(map<string, int> mapTrigShouldFired, map<string, int> mapTrigDidFired, float weight)
{
  int nposvtx = countNPosVtx();
  float vz = getVz();

  //fill vertex info
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
int ReaderCrs::countNPosVtx()
{
  int nposvtx = 0;
  for(int ivtx = 0; ivtx < mJetEvent->numberOfVertices(); ivtx++){
    StJetVertex *jetVert = mJetEvent->vertex(ivtx);
    if(jetVert->ranking() > 0) nposvtx++;
  }
  return nposvtx;
}
float ReaderCrs::getVz()
{
  return mJetEvent->vertex()->position().z();
}
bool ReaderCrs::fillMyJetMc(int numJet, MyJet *jetmax[], string trgmax[], float weight)
{
  fillMyJetRes2D(numJet, jetmax, trgmax, weight, mJetPartonEvent, false, mMapJetPartonRes, mCmbJetPartonRes);
  fillMyJetRes2D(numJet, jetmax, trgmax, weight, mJetParticleEvent, mFlag, mMapJetParticleRes, mCmbJetParticleRes);
  //float weight = 1.0;
  //pythia event
  /*
  const StPythiaEvent *pythia = mJetSkimEvent->mcEvent();
  float partonicPt = pythia->pt();
  float q2 = TMath::Power(partonicPt, 2);

  float x1 = pythia->x1();
  float x2 = pythia->x2();
  int flavor1 = pythia->particle(4)->GetPdgCode();
  //flavor1 = (flavor1 == 21) ? 0 : flavor1;
  int flavor2 = pythia->particle(5)->GetPdgCode();
  //flavor2 = (flavor2 == 21) ? 0 : flavor2;

  float xf1 = mPdf->xfx(flavor1, x1, q2);
  float xf2 = mPdf->xfx(flavor2, x2, q2);
  float xf = xf1*xf2;

  float wf[MyJetYieldPar::NMod];
  for(int iset = 0; iset < MyJetYield::NMod; iset++){
    float xf1nn = mPdfnn[iset]->xfx(flavor1, x1, q2);
    float xf2nn = mPdfnn[iset]->xfx(flavor2, x2, q2);
    float xfnn = xf1nn*xf2nn;
    
    wf[iset] = xfnn/xf;
    //Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
    //jetYield->GetW(iset)->Fill(wf[iset], weight);
  }
  //subprocess
  int proc = -1;
  if(flavor1 == 21 && flavor2 == 21){
    proc = 0;
  }else if(flavor1 != 21 && flavor2 != 21){
    proc = 1;
  }else
    proc = 2;
  //Printf("proc = %d, with flavor1 = %d and flavor2 = %d", proc, flavor1, flavor2);
    for(int iset = 0; iset < MyJetYield::NMod; iset++){
    mMapJetYield[trgname]->GetJetDetectorW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
      mCmbJetYield->GetJetDetectorW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
    }
    //subprocess
    mMapJetYield[trgname]->GetJetDetectorProc(proc)->Fill(pt, TMath::Abs(eta), weight);
    mCmbJetYield->GetJetDetectorProc(proc)->Fill(pt, TMath::Abs(eta), weight);
  */
  //consider cases where on detector jet matched to the same particle/parton jet
  //*/
  return true;
}
bool ReaderCrs::fillMyJetRes2D(int numJet, MyJet *jetmax[], string trgmax[], float weight, StJetEvent *parevnt, bool flag, map<string, MyJetRes2D*> mapJetRes, MyJetRes2D *cmbJetRes){
  
  vector<int> parIndex(numJet);   
  for(int ijet = 0; ijet < numJet; ijet++){
    MyJet *pj = jetmax[ijet];
    string trgname = trgmax[ijet];
    float pt = smearJetPt(pj, mFlag, mFlagSmear);
    float eta = pj->eta();
    
    int parCounter = 0;
    StJetCandidate *parjet = FindParJet(pj->jet(), parevnt, parIndex[ijet], 0.9, parCounter);
    mapJetRes[trgname]->FillJetMatchRatio(pt, TMath::Abs(eta), parCounter != 0, weight);
    cmbJetRes->FillJetMatchRatio(pt, TMath::Abs(eta), parCounter != 0, weight);
    //
    if(parjet){
      
      float ptpar = parjet->pt();
      map<string, float> rpho = parjet->ueDensity();
      float dptpar = rpho["OffAxisConesR050"]*parjet->area();
      //if(mFlag > 0) ptpar -= dptpar*mFlag;
      ptpar = ptpar - flag*dptpar;
      float etapar = parjet->eta();
      //cout<<"Parton jet found fill "<<pt<<" "<<ptpar<<" "<<etapar<<"\n"; 
      //response
      //
      mapJetRes[trgname]->FillResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight);
      cmbJetRes->FillResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight);
    }
  }
  //don't consider this for now
  //if(numJet == 2){
  //if(parIndex[0] == parIndex[1] && parIndex[0] != -1){
      //fill parton two jet
      //if(trgmax[0] == trgmax[1]) fillParTwoJet(mapJetYield[trgmax[0]], par, parIndex[0], weight, false);
      //fillParTwoJet(cmbJetYield, par, partIndex[0], weight, flag);
  //}
  //}
  return true;
}

void ReaderCrs::fillMcEvent(StJetEvent *jetParEvent, MyJetYieldPar *jetYieldPar, bool flag, float weight)
{
  //int weight = 1.;
  
  //const StPythiaEvent *pythia = mJetSkimEvent->mcEvent();
  ////float partonicPt = pythia->pt();
  ////float weight = partonicWeight(partonicPt);
  //float x1 = pythia->x1();
  //float x2 = pythia->x2();
  //int flavor1 = pythia->particle(4)->GetPdgCode();
  ////flavor1 = (flavor1 == 21) ? 0 : flavor1;
  //int flavor2 = pythia->particle(5)->GetPdgCode();
  ////flavor2 = (flavor2 == 21) ? 0 : flavor2;
  
  //double q2 = TMath::Power(partonicPt, 2);

  //double xf1 = mPdf->xfx(flavor1, x1, q2);
  //double xf2 = mPdf->xfx(flavor2, x2, q2);
  //double xf = xf1*xf2;
  
  //Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
  //double wf[MyJetYieldPar::NMod];
  //for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
  //double xf1nn = mPdfnn[iset]->xfx(flavor1, x1, q2);
  // double xf2nn = mPdfnn[iset]->xfx(flavor2, x2, q2);
  //double xfnn = xf1nn*xf2nn;
    
  //wf[iset] = xfnn/xf;
  ////Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
  ////jetYieldPar->GetW(iset)->Fill(wf[iset], weight);
  //}
 
  int NJets = jetParEvent->vertex()->numberOfJets();
  for(int iJet = 0; iJet < NJets; iJet++){
    StJetCandidate *jetcnd = jetParEvent->vertex()->jet(iJet);
    float pt = jetcnd->pt();
    float eta = jetcnd->eta();
    map<string, float> rpho = jetcnd->ueDensity();
    float dpt = rpho["OffAxisConesR050"]*jetcnd->area();
    if(flag){ 
       //cout<<iJet<<" jet pt: "<<pt<<" dpt: "<<dpt<<endl;
       pt -= dpt;
    } 
    //yield
    if(eta > -0.9 && eta < 0.9)  {
      jetYieldPar->GetPtEta()->Fill(pt, TMath::Abs(eta), weight);
      //for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
      //jetYieldPar->GetPtEtaW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
      //}
    }
  }
}
void ReaderCrs::fillParJet(TH2F *h, StJetEvent *evntpar, float weight, bool flag)
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
    if(etapar < 1.4 && etapar > -1.4) h->Fill(ptpar, TMath::Abs(etapar), weight);
  }
}
/*
void ReaderCrs::fillParTwoJet(TH2F *h, StJetEvent *evntpar, int index, float weight, bool flag)
{
  StJetCandidate *jetpar = evntpar->vertex()->jet(index);
  float ptpar = jetpar->pt();
  float etapar = jetpar->eta();
  
  map<string, float> rpho = jetpar->ueDensity();
  float dptpar = rpho["OffAxisConesR050"]*jetpar->area();
  ptpar -= dptpar*flag;

  h->Fill(ptpar, TMath::Abs(etapar), weight);
  cout<<"find two jets matched to the same particle jet with par pt:"<<ptpar<<endl;
}
*/
/*
void ReaderCrs::fillParTwoJet(TH1F *h, StJetEvent *evntpar, int index, float weight, bool flag)
{
  StJetCandidate *jetpar = evntpar->vertex()->jet(index);
  float ptpar = jetpar->pt();
  //float etapar = jetpar->eta();
  if(flag > 0){
    map<string, float> rpho = jetpar->ueDensity();
    float dptpar = rpho["OffAxisConesR050"]*jetpar->area();
    ptpar -= dptpar*flag;
  }
  h->Fill(ptpar,  weight);
  cout<<"find two jets matched to the same particle jet with par pt:"<<ptpar<<endl;
}
*/
