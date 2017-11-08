#include"MyFun.h"

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
