#ifndef STMYMCVERTEXCUT
#define STMYMCVERTEXCUT
#include"TObject.h"

class StMcVertex;

class StMyMcVertexCut : public TObject{
 public:
  virtual bool operator ()(const StMcVertex *vertex) = 0;
 private:
  ClassDef(StMyMcVertexCut, 0);
};
#endif
