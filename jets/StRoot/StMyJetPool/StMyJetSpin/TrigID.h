#ifndef __TrigID__
#define __TrigID__

#include<map>
#include<vector>
#include<string>
#include<cstring>

using std::vector;
using std::map;
using std::string;
using std::make_pair;

class TrigID{

 private:
  map<int, string> *mTrigId;
 public:
  TrigID(){}
  TrigID(const char *opt){
    if(strcmp(opt,"P11idRun9") ==0 )
      {
	mTrigId = new map<int, string>;
	mTrigId->insert(std::make_pair(240410,string("JP1")));
	mTrigId->insert(std::make_pair(240411,string("JP1")));
	mTrigId->insert(std::make_pair(240650,string("L2JetHigh")));	
	mTrigId->insert(std::make_pair(240651,string("L2JetHigh")));
	mTrigId->insert(std::make_pair(240652,string("L2JetHigh")));
      }else if(strcmp(opt,"P11id") == 0)
      {
	mTrigId = new map<int, string>;
	mTrigId->insert(std::make_pair(320600,string("JP0")));
	mTrigId->insert(std::make_pair(320601,string("JP1")));
	mTrigId->insert(std::make_pair(320602,string("JP2*L2JetHigh")));
	mTrigId->insert(std::make_pair(320603,string("AJP")));
	mTrigId->insert(std::make_pair(320800,string("BHT2*JP1*L2Bgamma")));
	mTrigId->insert(std::make_pair(320102,string("ZDCMB-Cat2")));
	mTrigId->insert(std::make_pair(320000,string("VPDMB")));
	mTrigId->insert(std::make_pair(320001,string("VPDMB")));
	mTrigId->insert(std::make_pair(320011,string("VPDMB")));
	mTrigId->insert(std::make_pair(320021,string("VPDMB")));
	mTrigId->insert(std::make_pair(320103,string("BBCMB")));
	mTrigId->insert(std::make_pair(320113,string("BBCMB")));
	mTrigId->insert(std::make_pair(320123,string("BBCMB")));
      }else if(strcmp(opt, "P13ib") == 0)
      {
	mTrigId = new map<int, string>;
	mTrigId->insert(std::make_pair(380401,string("JP0")));
	mTrigId->insert(std::make_pair(380402,string("JP1")));
	mTrigId->insert(std::make_pair(380403,string("JP2")));
	mTrigId->insert(std::make_pair(380404,string("AJP")));
	mTrigId->insert(std::make_pair(380405,string("JP2*L2JetHigh")));
	mTrigId->insert(std::make_pair(380205,string("BHT2*BJP1")));
	mTrigId->insert(std::make_pair(380207,string("BHT2*BBCMB")));
	mTrigId->insert(std::make_pair(380208,string("BHT2*BJP1*L2Bgamma")));
	mTrigId->insert(std::make_pair(380001,string("VPDMB")));
	mTrigId->insert(std::make_pair(390203,string("BHT1*VPDMB")));
	mTrigId->insert(std::make_pair(380003,string("BBCMB")));
      }
    
    
  }
  //  void SetTrigIdMap();
  map<int, string>* GetTrigIdMap(){ return mTrigId; }
    
};
#endif
