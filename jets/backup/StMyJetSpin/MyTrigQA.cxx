#include"MyTrigQA.h"
#include<iostream>

using std::cout;
ClassImp(MyTrigQA);


void MyTrigQA::Add(MyTrigQA *trigQA)
{
  for(map<string, int>::iterator p = trigNevnt.begin(); p != trigNevnt.end(); p++)
      {
	/*   
      map<string, int>::iterator position = trigNevnt.find(p->first);
      
      if(position != trigNevnt.end())
	{
	  (position->second) += (p->second);
	  cout<<position->second<<" "<<p->second<<"\n";
	}
	*/
      p->second += trigQA->GetTrigNevnt(p->first);
      //      cout<<trigNevnt[p->first]<<" "<<p->second<<"\n";
      
    }
}
