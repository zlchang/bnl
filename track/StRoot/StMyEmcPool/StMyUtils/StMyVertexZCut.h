#ifndef STMYVERTEXZCUT
#define STMYVERTEXZCUT
#include "StMyVertexCut.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "TMath.h"

class StMyVertexZCut : public StMyVertexCut{
 public:
  
 StMyVertexZCut() : _vz(90.){}
 StMyVertexZCut(double vz) : _vz(vz){}
  bool operator ()(const StMuPrimaryVertex *vertex){
    double vz = vertex->position().z();
    return TMath::Abs(vz) > _vz;
  }
  double _vz;
 private:
  ClassDef(StMyVertexZCut, 0);
};
#endif
