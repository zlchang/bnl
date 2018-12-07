#include"MyJetMC.h"

ClassImp(MyJetMC);
const int MyJetMC::NMod;
const int MyJetPar::NMod;
void MyJetMC::Add(MyJetMC *jetmc, float w)
{
  //x, xg
  hxparton->Add(jetmc->GetXParton(), w);
  hxgparton->Add(jetmc->GetXgParton(), w);
  hxparticle->Add(jetmc->GetXParticle(), w);
  hxgparticle->Add(jetmc->GetXgParticle(), w);
  //
  hjetpt->Add(jetmc->GetJetMC(), w);
  hjetptnovtx->Add(jetmc->GetJetPtNoVtx(), w);
  hjetptnovtxprof->Add(jetmc->GetJetPtNoVtxProf(), w);
  //th2f
  htwojetpt->Add(jetmc->GetTwoJetMC(), w);
  hjetptparton->Add(jetmc->GetJetPartonMC(), w);
  hjetptparticle->Add(jetmc->GetJetParticleMC(), w);
  hjetptparticlecrr->Add(jetmc->GetJetParticleCrrMC(), w);
  //
  hjetpartonN->Add(jetmc->GetJetPartonNoMatch(), w);
  hjetparticleN->Add(jetmc->GetJetParticleNoMatch(), w);
  hjetpartonNprof->Add(jetmc->GetJetPartonNoMatchProf(), w);
  hjetparticleNprof->Add(jetmc->GetJetParticleNoMatchProf(), w);
  //pt shift
  hdptparton->Add(jetmc->GetJetDiffPartonProf(), w);
  hdptpartonw2->Add(jetmc->GetJetDiffPartonW2Prof(), w);
  hdptparticle->Add(jetmc->GetJetDiffParticleProf(), w);
  hdptparticlew2->Add(jetmc->GetJetDiffParticleW2Prof(), w);
  hdptparticlecrr->Add(jetmc->GetJetDiffParticleCrrProf(), w);
  hdptparticlecrrw2->Add(jetmc->GetJetDiffParticleCrrW2Prof(), w);
  //aLL
  for(int i = 0; i < NMod; i++){
    haLLdetector[i]->Add(jetmc->GetALLDetector(i), w);
    haLLdetectorw2[i]->Add(jetmc->GetALLDetectorW2(i), w);
  }
  //mean pt
  hptdetector->Add(jetmc->GetPtDetector(), w);
  hptdetectorw2->Add(jetmc->GetPtDetectorW2(), w);
  //dpt
  hjetdpt->Add(jetmc->GetDPtJet(), w);
  hjetdptw2->Add(jetmc->GetDPtJetW2(), w);
  
}
void MyJetPar::Add(MyJetPar *jetpar, float w)
{
  //x, xg
  hx->Add(jetpar->GetX(), w);
  hxg->Add(jetpar->GetXg(), w);

  for(int i = 0; i < NMod; i++){
    haLLpar[i]->Add(jetpar->GetALLPar(i), w);
    haLLparw2[i]->Add(jetpar->GetALLParW2(i), w);
  }
  //mean pt
  hptpar->Add(jetpar->GetPtPar(), w);
  hptparw2->Add(jetpar->GetPtParW2(), w);

}  
