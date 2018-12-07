#include "MyEventQA.h"

ClassImp(MyEventQA);

void MyEventQA::Add(MyEventQA *event, float w)
{
  hweight->Add(event->GetEventWeight(), w);
  hmatchedVx->Add(event->GetMatchedVx(), w);
  hvertexz->Add(event->GetVertexZ(), w);
  hnposvtx->Add(event->GetNPosVtx(), w);
}

