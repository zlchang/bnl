#ifndef STMYVERTEXRANKINGCUT
#define STMYVERTEXRANKINGCUT
#include "StMyVertexCut.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "TMath.h"

class StMyVertexRankingCut : public StMyVertexCut{
 public:
  
 StMyVertexRankingCut() : _rk(0){}
 StMyVertexRankingCut(double rk) : _rk(rk){}
  bool operator ()(const StMuPrimaryVertex *vertex){
    double rk = vertex->ranking();
    return rk < _rk;
  }
  double _rk;
 private:
  ClassDef(StMyVertexRankingCut, 0);
};
#endif
