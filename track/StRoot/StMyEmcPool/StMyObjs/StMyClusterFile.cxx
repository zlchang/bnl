#include "StMyClusterFile.h"
#include "StMyClusterHist.h"

void StMyClusterFile::Add(StMyClusterHist *pp, char *name, double ww)
{
  AddHist(pp->hE, Form("%sClusterE", name), ww);
  AddHist(pp->hNHits, Form("%sClusterNHits", name), ww);
  AddHist(pp->hMaxFrac, Form("%sClusterMaxFrac", name), ww);
}
