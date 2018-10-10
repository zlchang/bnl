#ifndef STMYMCVERTEXZCUT
#define STMYMCVERTEXZCUT
#include "StMyMcVertexCut.h"
#include "StMcEvent/StMcVertex.hh"
#include "TMath.h"

class StMyMcVertexZCut : public StMyMcVertexCut{
 public:
  
 StMyMcVertexZCut() : _vz(30.){}
 StMyMcVertexZCut(double vz) : _vz(vz){}
  bool operator ()(const StMcVertex *vertex){
    double vz = vertex->position().z();
    return TMath::Abs(vz) > _vz;
  }
  double _vz;
 private:
  ClassDef(StMyMcVertexZCut, 0);
};
#endif
