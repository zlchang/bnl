#include"MyFun.h"
#include "MyDef.h"
int checkpt(double pt)
{
  for(int i = 0; i < yNPtBins+1; i++){
    if(pt  < yPtBins[i]){
      return i;
    }
  }
  return yNPtBins+1;
}
//static
int SpinBit(int index)
{
    int m = index / 2 + 1;
    int n = index % 2 + 1;
    return 4*m + n;
}
//static 
int SpinBitIndex(int spinbit)
{
    int quo = spinbit/4;
    int mod = spinbit%2;
    return 2*(quo-1)+(1-mod);
}	
