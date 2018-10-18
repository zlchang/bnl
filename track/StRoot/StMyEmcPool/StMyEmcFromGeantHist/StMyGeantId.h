#ifndef STMYGEANTID
#define STMYGEANTID
#include "TObject.h"
#include<map>

class StMyGeantId : public TObject{
 public:
  StMyGeantId(){
    mMap.insert(pair<long, const char*>(1, "Photon"));
    //
    mMap.insert(pair<long, const char*>(2, "Electron"));
    mMap.insert(pair<long, const char*>(3, "Positron"));
    //
    mMap.insert(pair<long, const char*>(7, "Pi0"));
    mMap.insert(pair<long, const char*>(8, "Pi+"));
    mMap.insert(pair<long, const char*>(9, "Pi-"));
    //
    mMap.insert(pair<long, const char*>(10, "K0Long"));
    mMap.insert(pair<long, const char*>(11, "K+"));
    mMap.insert(pair<long, const char*>(12, "K-"));
    //
    mMap.insert(pair<long, const char*>(13, "Neutron"));
    mMap.insert(pair<long, const char*>(14, "Proton"));
    mMap.insert(pair<long, const char*>(15, "AntiProton"));
    //
    mMap.insert(pair<long, const char*>(16, "K0Short"));
  };
  const char* getName(long id){ const char* name = 0x0; if(mMap.find(id) != mMap.end()) name=mMap[id]; return name;}
  void insert(long id, const char* name){
    mMap.insert(pair<long, const char*>(id, name));
  }
  //private:
  map<long, const char*> mMap;
  ClassDef(StMyGeantId, 0);
};

#endif
