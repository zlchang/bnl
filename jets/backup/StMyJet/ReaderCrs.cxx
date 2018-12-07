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

#include "StSpinPool/StUeEvent/StUeOffAxisConesEvent.h"
#include "StSpinPool/StUeEvent/StUeVertex.h"
#include "StSpinPool/StUeEvent/StUeJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisCones.h"
//#include "StSpinPool/StUeEvent/StUeEvent.h"

#include "StMyJetPool/StMyJetSpin/MyJetYield.h"
#include "StMyJetPool/StMyJetSpin/MyJetTest.h"
#include "StMyJetPool/StMyJetSpin/MyJetQA.h"
#include "StMyJetPool/StMyJetSpin/MyEventQA.h"
#include "StMyJetPool/StMyJetSpin/MyTrigQA.h"
#include "StMyJetPool/StMyDef/MyDef.h"

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
    //pdf
    mPdf = new StMyPDF;
    for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
      mPdfnn[iset] = new StMyPDF("NNPDF30_lo_as_0130", iset);
    }
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
    fillMyJetQA(numJet, jetmax, trgmax, 1.0);
    fillMyJetCrs(numJet, jetmax, trgmax, 1.0);
    //
  }else{
    //fill particle and parton jets before vertex matching
    fillMcEvent(mJetPartonEvent, mJetYieldParton, false, weight);
    fillMcEvent(mJetParticleEvent, mJetYieldParticle, mFlag > 0, weight);
    //
    if(fillMyJetVtx(numJet, jetmax, trgmax, weight)){
      fillMyJetQA(numJet, jetmax, trgmax, weight);
      fillMyJetCrs(numJet, jetmax, trgmax, weight);
      fillMyJetMc(numJet, jetmax, trgmax, weight);
    }
  }
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
    float pt = jetmax[ijet]->pt();
    float eta = jetmax[ijet]->eta();
    float dpt = jetmax[ijet]->dpt();
    cout<<"pt: "<<pt<<" dpt: "<<dpt<<endl;
    if(mFlag > 0) pt -= dpt*mFlag; 
    mMapJetYield[trgmax[ijet]]->GetJetVertexProf()->Fill(pt, TMath::Abs(eta), match, weight);
    mCmbJetYield->GetJetVertexProf()->Fill(pt, TMath::Abs(eta), match, weight);
    //test
    mMapJetTest[trgmax[ijet]]->GetJetVertexProf()->Fill(pt, match, weight);
    mCmbJetTest->GetJetVertexProf()->Fill(pt, match, weight);
  }
  return match;
}
bool ReaderCrs::fillMyJetCrs(int numJet, MyJet *jetmax[], string trgmax[], float weight)
{
  for(int ijet = 0; ijet < numJet; ijet++){
    string trgname = trgmax[ijet];
    float pt = jetmax[ijet]->pt();
    float eta = jetmax[ijet]->eta();
    float rt = jetmax[ijet]->rt();
    float dpt = jetmax[ijet]->dpt();
    float rtue = jetmax[ijet]->rtUe();
    //
    //cout<<"pt: "<<pt<<" dpt: "<<dpt<<endl;
    Printf("jet pt: %lf, rt: %lf, dpt: %lf, rtue: %lf", pt, rt, dpt, rtue);
    //if(mFlag > 0) pt -= dpt*smearPtFrac(rtue)*mFlagSmear*mFlag; 
    //if(mFlagSmear){
    //float spt = jetmax[ijet]->pt()*smearPtFrac(rt)*mFlagSmear;
    //pt += spt;
    pt = pt+pt*smearPtFrac(rt)*mFlagSmear;
    dpt = dpt+dpt*smearPtFrac(rtue)*mFlagSmear;
    pt = pt - dpt*mFlag;
    //Printf("jet pt: %lf, with smear: %lf", pt);
    //} 
    mMapJetYield[trgname]->GetJetDetectorJet()->Fill(pt, TMath::Abs(eta), weight);  
    mCmbJetYield->GetJetDetectorJet()->Fill(pt, TMath::Abs(eta), weight);  
    //rt
    mMapJetYield[trgname]->GetJetRtProf()->Fill(pt, TMath::Abs(eta), rt, weight);  
    mCmbJetYield->GetJetRtProf()->Fill(pt, TMath::Abs(eta), rt, weight);  
    
    mMapJetTest[trgname]->GetJetDetectorJet()->Fill(pt, weight);  
    mCmbJetTest->GetJetDetectorJet()->Fill(pt, weight);  
    
  }
  if(numJet == 2){
    MyJet *pj1 = jetmax[0];
    MyJet *pj2 = jetmax[1];
    //string trgname1 = trgmax[0];
    float pt1 = pj1->pt();
    float eta1 = pj1->eta();
    float rt1 = pj1->rt();
    float dpt1 = pj1->dpt();
    float rt1ue = pj1->rtUe();

    pt1 = pt1+pt1*smearPtFrac(rt1)*mFlagSmear;
    dpt1 = dpt1+dpt1*smearPtFrac(rt1ue)*mFlagSmear;
    pt1 = pt1 - dpt1*mFlag;
    //
    float pt2 = pj2->pt();
    float eta2 = pj2->eta();
    float rt2 = pj2->rt();
    float dpt2 = pj2->dpt();
    float rt2ue = pj2->rtUe();
    pt2 = pt2+pt1*smearPtFrac(rt2)*mFlagSmear;
    dpt2 = dpt2+dpt2*smearPtFrac(rt2ue)*mFlagSmear;
    pt2 = pt2 - dpt2*mFlag;
    /*if(mFlag > 0){
      pt1 -= dpt1*mFlag;
      pt2 -= dpt2*mFlag;
    }
    //if(mFlagSmear){
    float spt1 = pj1->pt()*smearPtFrac(rt1)*mFlagSmear;
    pt1 += spt1;
    float spt2 = pj2->pt()*smearPtFrac(rt2)*mFlagSmear;
    pt2 += spt2;
    //}
    */
 
    mCmbJetYield->FillJetTwoJet(pt1, TMath::Abs(eta1), pt2, TMath::Abs(eta2), weight);    
    //test
    mCmbJetTest->GetJetTwoJet()->Fill(pt1, pt2, weight);
    if(trgmax[0] == trgmax[1]){
      mMapJetYield[trgmax[0]]->FillJetTwoJet(pt1, TMath::Abs(eta1), pt2, TMath::Abs(eta2), weight);
      mMapJetTest[trgmax[0]]->GetJetTwoJet()->Fill(pt1, pt2, weight);
    }
  }
  return true;
}
int ReaderCrs::Finish()
{
  mFile->cd();
  mTrig->Write("TrigQA");
  if(!mMcFlag){
    //data
  }else{
    //jet yield
    cout<<"CmbJetYield has been written"<<endl;
    mJetYieldParton->Write("YieldPartonMc");
    mJetYieldParticle->Write("YieldParticleMc");
    //test
    mJetTestParton->Write("TestPartonMc");
    mJetTestParticle->Write("TestParticleMc");
  }
  for(map<string,MyJetYield*>::iterator p = mMapJetYield.begin(); p != mMapJetYield.end(); p++){
    string objname = p->first + string("JetYield");
    //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  //test
  for(map<string,MyJetTest*>::iterator p = mMapJetTest.begin(); p != mMapJetTest.end(); p++){
    string objname = p->first + string("JetTest");
      //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  mCmbJetYield->Write("CmbJetYield");
  //test
  mCmbJetTest->Write("CmbJetTest");
  
  for(map<string,MyJetQA*>::iterator p = mMapJetQa.begin(); p != mMapJetQa.end(); p++){
    string objname = p->first + string("JetQA");
    //p->second->fill_error();
    p->second->Write(objname.c_str());
    cout<<objname<<" has been written"<<endl;
  }
  
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
	  arr.push_back(new MyJet(jetcnd, uejet));
	  //		cout<<jetcnd->pt()<<" "<<uejet->pt()<<endl;
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
    string trgname(JetCategory(pj, mJetSkimEvent, mapTrigShouldFired, mapTrigDidFired, flag, smear));
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
    float jarea = jetmax[ijet]->area();
    //cout<<"ue pt:"<<jdptue<<" trkmult:"<<jtrkmultue<<" twrmult:"<<jtwrmultue<<endl;
    //ue correction
    if(mFlag > 0){
      jpt -= jdptue*mFlag;
      cout<<"jet pt: "<<jpt<<" after ue correction: "<<jdptue<<endl;
    }
    float factor = jetmax[ijet]->pt()/jpt;
    cout<<"ue correction factor: "<<factor<<endl;
    mMapJetQa[trgmax[ijet]]->FillJetPtEtaPhi(jpt, jeta, jphi, weight);
    mMapJetQa[trgmax[ijet]]->FillJetPtRt(jpt, jrt, weight);
    mMapJetQa[trgmax[ijet]]->FillJetMult(jtwrmult, jtrkmult, weight);
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
          cout<<"ue track pt: "<<trkpt<<endl;
          float jt = jetmax[ijet]->trackjtUe(ic, im);//->jetUe()->cone(ic)->track(im)->jt();
          cout<<"ue jt: "<<jt<<endl;
          mMapJetQa[trgmax[ijet]]->GetJetUeParJt()->Fill(jpt, jt, jarea/areaue*weight);
          float frag = jetmax[ijet]->jetUe()->cone(ic)->track(im)->frag();
          frag *= factor;
          cout<<"ue frag: "<<frag<<endl;
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
  //float weight = 1.0;
  //pythia event

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
  Printf("proc = %d, with flavor1 = %d and flavor2 = %d", proc, flavor1, flavor2);

  vector<int> partonIndex(numJet); 
  vector<int> particleIndex(numJet); 
  for(int ijet = 0; ijet < numJet; ijet++){
    MyJet *pj = jetmax[ijet];
    string trgname = trgmax[ijet];
    float pt = pj->pt();
    float dpt = pj->dpt();
    float eta = pj->eta();
    float rt = pj->rt();
    float rtue = pj->rtUe();
    
    pt = pt+pt*smearPtFrac(rt)*mFlagSmear;
    dpt = dpt+dpt*smearPtFrac(rtue)*mFlagSmear;
    pt = pt - dpt*mFlag;

    /*if(mFlag > 0){
      pt -= dpt*mFlag;
      cout<<"jet pt: "<<pt<<" with ue correction: "<<dpt<<endl;
    }
    
    //if(mFlagSmear){
    float spt = pj->pt()*smearPtFrac(rt)*mFlagSmear;
    pt += spt;
    Printf("jet pt: %lf, with smear: %lf", pt, spt);
    //}
    */
    for(int iset = 0; iset < MyJetYield::NMod; iset++){
      mMapJetYield[trgname]->GetJetDetectorW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
      mCmbJetYield->GetJetDetectorW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
    }
    //subprocess
    mMapJetYield[trgname]->GetJetDetectorProc(proc)->Fill(pt, TMath::Abs(eta), weight);
    mCmbJetYield->GetJetDetectorProc(proc)->Fill(pt, TMath::Abs(eta), weight);
    int partonCounter = 0;
    StJetCandidate *parton = FindParJet(pj->jet(), mJetPartonEvent, partonIndex[ijet], 0.9, partonCounter);
    mMapJetYield[trgname]->GetJetPartonNMatch()->Fill(pt, TMath::Abs(eta), partonCounter, weight);
    mCmbJetYield->GetJetPartonNMatch()->Fill(pt, TMath::Abs(eta), partonCounter, weight);
    //
    if(parton){
      float ptpar = parton->pt();
      cout<<"Parton jet found fill "<<pt<<" "<<parton->pt()<<"\n";
      //response
      //mapJetYield[trgname]->GetJetPartonResMC()->Fill(pt, ptpar, weight);
      float etapar = parton->eta(); 
      //mapJetYield[trgname]->GetJetPartonJet()->Fill(ptpar, etapar, weight);

      //
      mMapJetYield[trgname]->FillPartonResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight, wf, proc);
      mMapJetYield[trgname]->GetJetPartonProfMC()->Fill(pt, TMath::Abs(eta), ptpar, weight); 
      mMapJetYield[trgname]->GetJetPartonScale()->Fill(pt, TMath::Abs(eta), ptpar/pt, weight); 
      mMapJetYield[trgname]->GetJetPartonRatioProfile()->Fill(pt, TMath::Abs(eta), true, weight); 
      //
      mCmbJetYield->FillPartonResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight, wf, proc);
      mCmbJetYield->GetJetPartonProfMC()->Fill(pt, TMath::Abs(eta), ptpar, weight); 
      mCmbJetYield->GetJetPartonScale()->Fill(pt, TMath::Abs(eta), ptpar/pt, weight); 
      mCmbJetYield->GetJetPartonRatioProfile()->Fill(pt, TMath::Abs(eta), true, weight); 
      //test
      mMapJetTest[trgname]->GetJetPartonResMC()->Fill(pt, ptpar, weight);
      mMapJetTest[trgname]->GetJetPartonProfMC()->Fill(pt, ptpar, weight); 
      mMapJetTest[trgname]->GetJetPartonRatioProfile()->Fill(pt, true, weight); 
      //
      mCmbJetTest->GetJetPartonResMC()->Fill(pt, ptpar, weight);
      mCmbJetTest->GetJetPartonProfMC()->Fill(pt, ptpar, weight); 
      mCmbJetTest->GetJetPartonRatioProfile()->Fill(pt, true, weight); 
      //subprocess
      //mMapJetYield[trgname]->GetJetDetMatchPartonProc(proc)->Fill(pt, TMath::Abs(eta), weight);
      //mCmbJetYield->GetJetDetMatchPartonProc(proc)->Fill(pt, TMath::Abs(eta), weight);
      //parton
      mMapJetYield[trgname]->GetJetPartonProc(proc)->Fill(ptpar, TMath::Abs(etapar), weight);
      mCmbJetYield->GetJetPartonProc(proc)->Fill(ptpar, TMath::Abs(etapar), weight);
    }else{
      //match ratio
      mMapJetYield[trgname]->GetJetPartonRatioProfile()->Fill(pt, TMath::Abs(eta), false, weight);
      //
      mCmbJetYield->GetJetPartonRatioProfile()->Fill(pt, TMath::Abs(eta), false, weight);
      //test
      mMapJetTest[trgname]->GetJetPartonRatioProfile()->Fill(pt, false, weight);
      //
      mCmbJetTest->GetJetPartonRatioProfile()->Fill(pt, false, weight);
    }
    //particle
    int particleCounter = 0;
    StJetCandidate *particle = FindParJet(pj->jet(), mJetParticleEvent, particleIndex[ijet], 0.9, particleCounter);
    mMapJetYield[trgname]->GetJetParticleNMatch()->Fill(pt, TMath::Abs(eta), particleCounter, weight);
    mCmbJetYield->GetJetParticleNMatch()->Fill(pt, TMath::Abs(eta), particleCounter, weight);
    if(particle){
      float ptpar = particle->pt();
      map<string, float> rpho = particle->ueDensity();
      float dptpar = rpho["OffAxisConesR050"]*particle->area();
      //if(mFlag > 0) ptpar -= dptpar*mFlag;
      ptpar = ptpar - mFlag*dptpar;
      cout<<"Particle jet found fill "<<pt<<" "<<particle->pt()<<"\n";
      //                                                                               
      //response
      float etapar = particle->eta(); 
      mMapJetYield[trgname]->FillParticleResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight, wf, proc);
      mMapJetYield[trgname]->GetJetParticleProfMC()->Fill(pt, TMath::Abs(eta), ptpar, weight); 
      mMapJetYield[trgname]->GetJetParticleScale()->Fill(pt, TMath::Abs(eta), ptpar/pt, weight); 
      mMapJetYield[trgname]->GetJetParticleRatioProfile()->Fill(pt, TMath::Abs(eta), true, weight);
      // 
      mCmbJetYield->FillParticleResponse(pt, TMath::Abs(eta), ptpar, TMath::Abs(etapar), weight, wf, proc);
      mCmbJetYield->GetJetParticleProfMC()->Fill(pt, TMath::Abs(eta), ptpar, weight); 
      mCmbJetYield->GetJetParticleScale()->Fill(pt, TMath::Abs(eta), ptpar/pt, weight); 
      mCmbJetYield->GetJetParticleRatioProfile()->Fill(pt, TMath::Abs(eta), true, weight); 
      //test
      mMapJetTest[trgname]->GetJetParticleResMC()->Fill(pt, ptpar, weight);
      mMapJetTest[trgname]->GetJetParticleProfMC()->Fill(pt, ptpar, weight); 
      mMapJetTest[trgname]->GetJetParticleRatioProfile()->Fill(pt, true, weight); 
      //
      mCmbJetTest->GetJetParticleResMC()->Fill(pt, ptpar, weight);
      mCmbJetTest->GetJetParticleProfMC()->Fill(pt, ptpar, weight); 
      mCmbJetTest->GetJetParticleRatioProfile()->Fill(pt, true, weight); 
      //subprocess
      //mMapJetYield[trgname]->GetJetDetMatchParticleProc(proc)->Fill(pt, TMath::Abs(eta), weight);
      //mCmbJetYield->GetJetDetMatchParticleProc(proc)->Fill(pt, TMath::Abs(eta), weight);
      //particle
      mMapJetYield[trgname]->GetJetParticleProc(proc)->Fill(ptpar, TMath::Abs(etapar), weight);
      mCmbJetYield->GetJetParticleProc(proc)->Fill(ptpar, TMath::Abs(etapar), weight);
    }else{
      mMapJetYield[trgname]->GetJetParticleRatioProfile()->Fill(pt, TMath::Abs(eta), false, weight); 
      mCmbJetYield->GetJetParticleRatioProfile()->Fill(pt, TMath::Abs(eta), false, weight); 
      //test
      mMapJetTest[trgname]->GetJetParticleRatioProfile()->Fill(pt, false, weight);
      //
      mCmbJetTest->GetJetParticleRatioProfile()->Fill(pt, false, weight);
    }
  }
  //consider cases where on detector jet matched to the same particle/parton jet
  if(numJet == 2){
    if(partonIndex[0] == partonIndex[1] && partonIndex[0] != -1){
      //fill parton two jet
      if(trgmax[0] == trgmax[1]) fillParTwoJet(mMapJetYield[trgmax[0]]->GetJetPartonTwoJet(), mJetPartonEvent, partonIndex[0], weight, false);
      fillParTwoJet(mCmbJetYield->GetJetPartonTwoJet(), mJetPartonEvent, partonIndex[0], weight, false);
      //test
      if(trgmax[0] == trgmax[1]) fillParTwoJet(mMapJetTest[trgmax[0]]->GetJetPartonTwoJet(), mJetPartonEvent, partonIndex[0], weight, false);
      fillParTwoJet(mCmbJetTest->GetJetPartonTwoJet(), mJetPartonEvent, partonIndex[0], weight, false);
    }
    if(particleIndex[0] == particleIndex[1] && particleIndex[0] != -1){
      //fill particle two jet
      if(trgmax[0] == trgmax[1]) fillParTwoJet(mMapJetYield[trgmax[0]]->GetJetParticleTwoJet(), mJetParticleEvent, particleIndex[0], weight, mFlag > 0);
      fillParTwoJet(mCmbJetYield->GetJetParticleTwoJet(), mJetParticleEvent, particleIndex[0], weight, mFlag > 0);
      //test
      if(trgmax[0] == trgmax[1]) fillParTwoJet(mMapJetTest[trgmax[0]]->GetJetParticleTwoJet(), mJetParticleEvent, particleIndex[0], weight, mFlag > 0);
      fillParTwoJet(mCmbJetTest->GetJetParticleTwoJet(), mJetParticleEvent, particleIndex[0], weight, mFlag > 0);
    }
  }
  return true;
}
void ReaderCrs::fillMcEvent(StJetEvent *jetParEvent, MyJetYieldPar *jetYieldPar, bool flag, float weight)
{
  //int weight = 1.;
  
  const StPythiaEvent *pythia = mJetSkimEvent->mcEvent();
  float partonicPt = pythia->pt();
  //float weight = partonicWeight(partonicPt);
  float x1 = pythia->x1();
  float x2 = pythia->x2();
  int flavor1 = pythia->particle(4)->GetPdgCode();
  //flavor1 = (flavor1 == 21) ? 0 : flavor1;
  int flavor2 = pythia->particle(5)->GetPdgCode();
  //flavor2 = (flavor2 == 21) ? 0 : flavor2;
  
  double q2 = TMath::Power(partonicPt, 2);

  double xf1 = mPdf->xfx(flavor1, x1, q2);
  double xf2 = mPdf->xfx(flavor2, x2, q2);
  double xf = xf1*xf2;
  
  //Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
  double wf[MyJetYieldPar::NMod];
  for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
    double xf1nn = mPdfnn[iset]->xfx(flavor1, x1, q2);
    double xf2nn = mPdfnn[iset]->xfx(flavor2, x2, q2);
    double xfnn = xf1nn*xf2nn;
    
    wf[iset] = xfnn/xf;
    //Printf("xf1: %e xf2: %e wf: %e", xf1, xf2, wf);
    //jetYieldPar->GetW(iset)->Fill(wf[iset], weight);
  }
 
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
      for(int iset = 0; iset < MyJetYieldPar::NMod; iset++){
        jetYieldPar->GetPtEtaW(iset)->Fill(pt, TMath::Abs(eta), weight*wf[iset]);
      }
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
void ReaderCrs::fillParTwoJet(TH2F *h, StJetEvent *evntpar, int index, float weight, bool flag)
{
  StJetCandidate *jetpar = evntpar->vertex()->jet(index);
  float ptpar = jetpar->pt();
  float etapar = jetpar->eta();
  if(flag > 0){
    map<string, float> rpho = jetpar->ueDensity();
    float dptpar = rpho["OffAxisConesR050"]*jetpar->area();
    ptpar -= dptpar*flag;
  }
  h->Fill(ptpar, TMath::Abs(etapar), weight);
  cout<<"find two jets matched to the same particle jet with par pt:"<<ptpar<<endl;
}
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
