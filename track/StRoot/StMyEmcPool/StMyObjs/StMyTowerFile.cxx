#include "StMyTowerFile.h"
#include "StMyTowerHist.h"

void StMyTowerFile::Add(StMyTowerHist *pp, char *name, double ww)
{
  AddHist(pp->hE, Form("%sTowerE", name), ww);
  AddHist(pp->hHits, Form("%sTowerHits", name), ww);
  AddHist(pp->hNHits, Form("%sTowerNHits", name), ww);
}
