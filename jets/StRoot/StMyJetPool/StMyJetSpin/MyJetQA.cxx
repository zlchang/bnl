#include"MyJetQA.h"

ClassImp(MyJetQA);
void MyJetQA::Add(MyJetQA *jetqa, float w)
{
  //jet pt no vertex
  hjetptnovtx->Add(jetqa->GetJetPtNoVtx(), w);
  hjetnposvtx->Add(jetqa->GetJetNPosVtx(), w);
  hjetvertexz->Add(jetqa->GetJetVertexZ(), w);
  //
  hjetpt->Add(jetqa->GetJetPt(), w);
  //htwojetpt->Add(jetqa->GetTwoJetPt(), w);
  hjeteta->Add(jetqa->GetJetEta(), w);
  hjetphi->Add(jetqa->GetJetPhi(), w);
  hjetrt->Add(jetqa->GetJetRt(), w);
  hjetparjt->Add(jetqa->GetJetParJt(), w);
  hjetparz->Add(jetqa->GetJetParZ(), w);
  hjetmult->Add(jetqa->GetJetMult(), w);
  hjettrkmult->Add(jetqa->GetJetTrkMult(), w);
  hjettrkmultprof->Add(jetqa->GetJetTrkMultProf(), w);
  hjettrkpt->Add(jetqa->GetJetTrkPt(), w);
  hjettwrmult->Add(jetqa->GetJetTwrMult(), w);
  hjettwrmultprof->Add(jetqa->GetJetTwrMultProf(), w);
  hjettwrpt->Add(jetqa->GetJetTwrPt(), w);
  hjetrtprof->Add(jetqa->GetJetRtProf(), w);
  hjetptprof->Add(jetqa->GetJetPtProf(), w);
  //ue
  hjetdptue->Add(jetqa->GetJetUeDpt(), w);
  hjetuert->Add(jetqa->GetJetUeRt(), w);
  hjettrkmultue->Add(jetqa->GetJetUeTrkMult(), w);
  hjettwrmultue->Add(jetqa->GetJetUeTwrMult(), w);  
  hjetueparjt->Add(jetqa->GetJetUeParJt(), w);
  hjetueparz->Add(jetqa->GetJetUeParZ(), w);
  //
  hjetptarea->Add(jetqa->GetJetAreaVsPt(), w);
  hdptue->Add(jetqa->GetUeDpt(), w);
  hmultue->Add(jetqa->GetUeMult(), w);
  htrkmultue->Add(jetqa->GetUeTrkMult(), w);
  htwrmultue->Add(jetqa->GetUeTwrMult(), w);
  htrkptue->Add(jetqa->GetUeTrkPt(), w);
  htwrptue->Add(jetqa->GetUeTwrPt(), w);
  hdpt3due->Add(jetqa->GetUeDpt3D(), w);
  hmult3due->Add(jetqa->GetUeMult3D(), w);
  //
  for(int i = 0; i < 3; i++){
    hjetprof[i]->Add(jetqa->GetJetProf(i), w);
  }
  hjetetart->Add(jetqa->GetJetRtVsEta(), w);
}
