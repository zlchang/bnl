#ifndef STMYVERTEXCUT
#define STMYVERTEXCUT
class StMuPrimaryVertex;

class StMyVertexCut : public TObject{
 public:
  virtual bool operator ()(const StMuPrimaryVertex *vertex) = 0;
 private:
  ClassDef(StMyVertexCut, 0);
};
#endif
