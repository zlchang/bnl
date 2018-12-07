#include <fstream>

#include "TFile.h"

#include "ReaderRun.h"
#include "StMyJetPool/StMyDef/MyDef.h"
#include "StMyJetPool/StMyJetSpin/MyJetSpin.h"
#include "StMyJetPool/StMyJetSpin/MyJetPtAsym.h"
#include "StMyJetPool/StMyJetSpin/MyJetSum.h"
#include "StMyJetPool/StMyJetSpin/MyTrigQA.h"

int ReaderRun::Init()
{
  mFile = new TFile(mOutput, "recreate");
  for(vector<string>::const_iterator it = mTriggers.begin(); it < mTriggers.end(); it++){
      mMapJetSum.insert(std::make_pair(*it, new MyJetSum((*it).c_str())));
      //mMapJetSumCrr.insert(std::make_pair(*it, new MyJetSum(((*it)+string("Crr")).c_str())));
  }
  mJetSum = new MyJetSum("Cmb");
  //mJetSumCrr = new MyJetSum("CmbCrr");
  mTrigQA = new MyTrigQA("P13ib");
  return ReaderVirtual::Init();
}
int ReaderRun::Make(int run)
{
  //load relative luminosity
  float rel[6] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
  char *relfile = Form("%s/%d.rel.dat", mDirRel, run);
  ifstream inrel(relfile);
  if(!inrel) cout<<"loading relative luminosity failure!!! "<<relfile<<endl;
  std::cout<<"relative luminosity:";
  for(int irel = 0; irel < 6; irel++)
    {
      inrel >> rel[irel];
      std::cout<<" "<<rel[irel];
    }
  std::cout<<endl;
  inrel.close();

  //Load polarization
  float pb = 0.55,py = 0.55;
  char* polfile = Form("%s/%d.pol.dat", mDirPol, run);
  ifstream inpol(polfile);
  if(!inpol) cout<<"loading polarization failure!!! "<<polfile<<endl;
  cout<<"polarization";
  inpol>>pb>>py;
  cout<<" blue: "<<pb<<" yellow: "<<py<<endl;
  inpol.close();
  //pt
  map<string, MyJetPtSpin*> mapJetPtSpin;
  map<string, MyJetPtAsym*> mapJetPtAsym;
  map<string, MyJetPtSpin*> mapJetMultSpin;
  map<string, MyJetPtAsym*> mapJetMultAsym;
  //dpt
  map<string, MyJetPtSpin*> mapJetDPtSpin;
  map<string, MyJetPtAsym*> mapJetDPtAsym;
  map<string, MyJetPtSpin*> mapJetMultUeSpin;
  map<string, MyJetPtAsym*> mapJetMultUeAsym;
  //correction
  //pt
  //map<string, MyJetPtSpin*> mapJetPtSpinCrr;
  //dpt
  //map<string, MyJetPtSpin*> mapJetDPtSpinCrr;

  for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
    //pt
    mapJetPtSpin.insert(std::make_pair(*p, new MyJetPtSpin((*p+"Pt_t").c_str())));
    mapJetPtAsym.insert(std::make_pair(*p, new MyJetPtAsym((*p+"PtAsym_t").c_str())));
    mapJetMultSpin.insert(std::make_pair(*p, new MyJetPtSpin((*p+"Mult_t").c_str())));
    mapJetMultAsym.insert(std::make_pair(*p, new MyJetPtAsym((*p+"MultAsym_t").c_str())));
    //dpt
    mapJetDPtSpin.insert(std::make_pair(*p, new MyJetPtSpin((*p+"DPt_t").c_str())));
    mapJetDPtAsym.insert(std::make_pair(*p, new MyJetPtAsym((*p+"DPtAsym_t").c_str())));
    mapJetMultUeSpin.insert(std::make_pair(*p, new MyJetPtSpin((*p+"MultUe_t").c_str())));
    mapJetMultUeAsym.insert(std::make_pair(*p, new MyJetPtAsym((*p+"MultUeAsym_t").c_str())));
    //correction
    //pt
    //mapJetPtSpinCrr.insert(std::make_pair(*p, new MyJetPtSpin((*p+"PtCrr").c_str())));
    //dpt
    //mapJetDPtSpinCrr.insert(std::make_pair(*p, new MyJetPtSpin((*p+"DPtCrr").c_str())));
  }
  //pt
  MyJetPtSpin *cmbJetPtSpin = new MyJetPtSpin("CmbPt_t");
  MyJetPtAsym *cmbJetPtAsym = new MyJetPtAsym("CmbPtAsym_t");
  MyJetPtSpin *cmbJetMultSpin = new MyJetPtSpin("CmbMult_t");
  MyJetPtAsym *cmbJetMultAsym = new MyJetPtAsym("CmbMultAsym_t");
  //dpt
  MyJetPtSpin *cmbJetDPtSpin = new MyJetPtSpin("CmbDPt_t");
  MyJetPtAsym *cmbJetDPtAsym = new MyJetPtAsym("CmbDPtAsym_t");
  MyJetPtSpin *cmbJetMultUeSpin = new MyJetPtSpin("CmbMultUe_t");
  MyJetPtAsym *cmbJetMultUeAsym = new MyJetPtAsym("CmbMultUeAsym_t");
  //correction
  //pt
  //MyJetPtSpin *cmbJetPtSpinCrr = new MyJetPtSpin("CmbPtCrr");
  //dpt
  //MyJetPtSpin *cmbJetDPtSpinCrr = new MyJetPtSpin("CmbDPtCrr");

  for(int ii = 0; ii < mFileIndex; ii++){
    char* file = Form("%s/%d/run%d.%d.spin.root", mDir, run, run, ii);
    cout<<file<<endl;
    TFile *f = TFile::Open(file);
    f->cd();
    MyTrigQA *trigQA = (MyTrigQA *)f->Get("TrigQA");
    mTrigQA->Add(trigQA);
    for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
	cout<<*p<<" = "<<trigQA->GetTrigNevnt(*p)<<" / ";
	cout<<mTrigQA->GetTrigNevnt(*p)<<" ";
    }
    cout<<endl;
    for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
      MyJetSpin *jetspin = (MyJetSpin *)f->Get((*p+"JetSpin").c_str());
      mMapJetSum[*p]->FillJetSpin(jetspin, rel, pb, py);
      mapJetPtSpin[*p]->AddMeanPt(jetspin);
      mapJetMultSpin[*p]->AddMult(jetspin);
      mapJetDPtSpin[*p]->AddDPt(jetspin);
      mapJetMultUeSpin[*p]->AddMultUe(jetspin);
      //correction
      //MyJetSpin *jetspincrr = (MyJetSpin *)f->Get((*p+"jetSpinCrr").c_str());
      //mMapJetSumCrr[*p]->FillJetSpin(jetspincrr, rel, pb, py);
      //mapJetPtSpinCrr[*p]->AddMeanPt(jetspincrr);
      //mapJetDPtSpinCrr[*p]->AddDPt(jetspincrr);
    }
    //combine
    MyJetSpin *jetspincmb = (MyJetSpin *)f->Get("CmbJetSpin");
    mJetSum->FillJetSpin(jetspincmb, rel, pb, py);
    cmbJetPtSpin->AddMeanPt(jetspincmb);
    cmbJetMultSpin->AddMult(jetspincmb);
    cmbJetDPtSpin->AddDPt(jetspincmb);
    cmbJetMultUeSpin->AddMultUe(jetspincmb);
    //correction
    //MyJetSpin *jetspincmbcrr = (MyJetSpin *)f->Get("CmbjetSpinCrr");
    //mJetSumCrr->FillJetSpin(jetspincmbcrr, rel, pb, py);
    //cmbJetPtSpinCrr->AddMeanPt(jetspincmbcrr);
    //cmbJetDPtSpinCrr->AddDPt(jetspincmbcrr);
    f->Close();
  }
  for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
    mapJetPtAsym[*p]->FillJetPtAsym(mapJetPtSpin[*p], pb, py);
    mapJetMultAsym[*p]->FillJetPtAsym(mapJetMultSpin[*p], pb, py);
    mapJetDPtAsym[*p]->FillJetPtAsym(mapJetDPtSpin[*p], pb, py);
    mapJetMultUeAsym[*p]->FillJetPtAsym(mapJetMultUeSpin[*p], pb, py);
  }
  cmbJetPtAsym->FillJetPtAsym(cmbJetPtSpin, pb, py);
  cmbJetMultAsym->FillJetPtAsym(cmbJetMultSpin, pb, py);
  cmbJetDPtAsym->FillJetPtAsym(cmbJetDPtSpin, pb, py);
  cmbJetMultUeAsym->FillJetPtAsym(cmbJetMultUeSpin, pb, py);

  for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
    mMapJetSum[*p]->GetPtAsym()->AddAsymmetry(mapJetPtAsym[*p]);
    mMapJetSum[*p]->GetMultAsym()->AddAsymmetry(mapJetMultAsym[*p]);
    mMapJetSum[*p]->GetDptAsym()->AddAsymmetry(mapJetDPtAsym[*p]);
    mMapJetSum[*p]->GetMultUeAsym()->AddAsymmetry(mapJetMultUeAsym[*p]);
  }
  mJetSum->GetPtAsym()->AddAsymmetry(cmbJetPtAsym);
  mJetSum->GetMultAsym()->AddAsymmetry(cmbJetMultAsym);
  mJetSum->GetDptAsym()->AddAsymmetry(cmbJetDPtAsym);
  //debug
  //cmbJetDPtAsym->GetPtUnlikeSignAsymmetry()->Print("all");
  //mJetSum->GetDptAsym()->GetPtUnlikeSignAsymmetry()->Print("all");
  mJetSum->GetMultUeAsym()->AddAsymmetry(cmbJetMultUeAsym);
  
  for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
    delete mapJetPtSpin[*p];
    delete mapJetPtAsym[*p];
    delete mapJetMultSpin[*p];
    delete mapJetMultAsym[*p];
    delete mapJetDPtAsym[*p];
    delete mapJetDPtSpin[*p];
    delete mapJetMultUeSpin[*p];
    delete mapJetMultUeAsym[*p];
  }
  delete cmbJetPtSpin;
  delete cmbJetPtAsym;
  delete cmbJetMultSpin;
  delete cmbJetMultAsym;
  delete cmbJetDPtSpin;
  delete cmbJetDPtAsym;
  delete cmbJetMultUeSpin;
  delete cmbJetMultUeAsym;
/*
  for(vector<string>::iterator p = mTriggers.begin(); p != mTriggers.end(); p++){
    //pt
    mMapJetSum[*p]->FillJetPtSpin(mapJetPtSpin[*p], pb, py);
    mapJetPtSpin[*p]->Delete();
    //dpt
    mMapJetSum[*p]->FillJetDPtSpin(mapJetDPtSpin[*p], pb, py);
    mapJetDPtSpin[*p]->Delete();
    //correction
    //pt
    mMapJetSumCrr[*p]->FillJetPtSpin(mapJetPtSpinCrr[*p], pb, py);
    mapJetPtSpinCrr[*p]->Delete();
    //dpt
    mMapJetSumCrr[*p]->FillJetDPtSpin(mapJetDPtSpinCrr[*p], pb, py);
    mapJetDPtSpinCrr[*p]->Delete();
  }
  //pt
  mJetSum->FillJetPtSpin(cmbJetPtSpin, pb, py);
  cmbJetPtSpin->Delete();
  //dpt
  mJetSum->FillJetDPtSpin(cmbJetDPtSpin, pb, py);
  cmbJetDPtSpin->Delete();
  //correction
  mJetSumCrr->FillJetPtSpin(cmbJetPtSpinCrr, pb, py);
  cmbJetPtSpinCrr->Delete();
  //dpt
  mJetSumCrr->FillJetDPtSpin(cmbJetDPtSpinCrr, pb, py);
  cmbJetDPtSpinCrr->Delete();
*/
  cout<<run<<endl;
  return ReaderVirtual::Make(run);
}
int ReaderRun::Finish()
{
  mFile->cd();
  for(vector<string>::iterator p =mTriggers.begin(); p != mTriggers.end(); p++)
    {
      mMapJetSum[*p]->FillAsym();
      mMapJetSum[*p]->Write((*p + "JetSum").c_str());
      // correction
      //mMapJetSumCrr[*p]->FillAsym();
      //mMapJetSumCrr[*p]->Write((*p + "JetSumCrr").c_str());
    }
  // combine
  mJetSum->FillAsym();
  mJetSum->Write("CmbJetSum");
  // correction
  //mJetSumCrr->FillAsym();
  //mJetSumCrr->Write("CmbJetSumCrr");
  mTrigQA->Write("SumTrigQA");
  mFile->Write();
  mFile->Close();
  return ReaderVirtual::Finish();
}
