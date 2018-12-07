#ifndef ROOT_MyTrigQA
#define ROOT_MyTrigQA
#include<map>
#include<string>

using std::map;
using std::string;

#include<TObject.h>

#include"TrigID.h"

class TrigID;

class MyTrigQA : public TObject
{
 private:
  map<string, int> trigNevnt;
  map<string, float> trigPrescale;
  map<int, string> trigIdmap;
 public:
  MyTrigQA(){}
  MyTrigQA(const char *opt){
    TrigID *trg = new TrigID(opt);
//trigIdmap = new map<int, string>;
    trigIdmap = *(trg->GetTrigIdMap());
    for(map<int, string>::iterator p = trigIdmap.begin(); p != trigIdmap.end(); ++p)
      {
	trigNevnt.insert(std::make_pair(p->second, 0));
	trigPrescale.insert(std::make_pair(p->second, 0.));
      }
  }
  //  void SetTrigIdMap();
  map<string, int> GetTrigEvent() const { return trigNevnt; }
  map<string, float> GetTrigPrescale() const { return trigPrescale; }

  int GetTrigNevnt(string id) {
    //    map<string, int>::iterator p = trigNevnt.find(id);
    //    return (p->second);
        return trigNevnt[id];
  }
  float GetTrigNprescale(string id) {
    //    map<string, float>::iterator p = trigPrescale.find(id);
    //    return (p->second);
    return trigPrescale[id];
  }


  void SetTrigNevnt(string id, int n){
    map<string, int>::iterator p = trigNevnt.find(id);
    p->second = n;
  }
  void Count(string id)
  {
    map<string, int>::iterator p = trigNevnt.find(id);
    p->second++;
  }
  void SetTrigPrescale(string id, float n){

    if( (n - trigPrescale[id]) > 0.0001 || (n - trigPrescale[id]) < 0.0001)
	trigPrescale[id] = n;
  }
  bool CheckId(int n){
    map<int, string>::iterator p = trigIdmap.find(n);
    if(p == trigIdmap.end())
      {
	return false;
      }
    return true;
  }
  string GetTrigName(int n){
    map<int, string>::iterator p = trigIdmap.find(n);
    if(p == trigIdmap.end())
      {
	return NULL;
      }
    return (p->second);
    //    return trigIdmap[n];
  }
  void Add(MyTrigQA *trigQA);  

  ClassDef(MyTrigQA, 1);    
    
};

#endif
