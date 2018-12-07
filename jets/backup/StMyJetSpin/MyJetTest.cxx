#include "MyJetTest.h"

ClassImp(MyJetTest);

void MyJetTest::Add(MyJetTest *jettest, float w)
{
  hjetptvtxprof->Add(jettest->GetJetVertexProf(), w);
  hjetpartonratioprof->Add(jettest->GetJetPartonRatioProfile(), w);
  hjetparticleratioprof->Add(jettest->GetJetParticleRatioProfile(), w);
  hjettwojet->Add(jettest->GetJetTwoJet(), w);
  hjetdetector->Add(jettest->GetJetDetectorJet(), w);
  //response
  hjetptpartonres->Add(jettest->GetJetPartonResMC(), w);
  hjetptparticleres->Add(jettest->GetJetParticleResMC(), w);
  //profile
  hjetptpartonprof->Add(jettest->GetJetPartonProfMC(), w);
  hjetptparticleprof->Add(jettest->GetJetParticleProfMC(), w);
  //hjetptparticlecrrres->Add(jettest->GetJetParticleCrrResMC(), w);
  hjetparton->Add(jettest->GetJetPartonJet(), w);
  hjetparticle->Add(jettest->GetJetParticleJet(), w);
  hjetpartonT->Add(jettest->GetJetPartonTwoJet(), w);
  hjetparticleT->Add(jettest->GetJetParticleTwoJet(), w);
  //crr
  //hjetparticlecrr->Add(jettest->GetJetParticleJet(), w);
  //hjetparticlecrrT->Add(jettest->GetJetParticleTwoJetCrr(), w);

}
ClassImp(MyJetTestPar);

void MyJetTestPar::Add(MyJetTestPar *par, float w)
{
  hpt->Add(par->GetPt(), w);
  //hptetacrr->Add(par->GetPtEtaCrr(), w);
}
