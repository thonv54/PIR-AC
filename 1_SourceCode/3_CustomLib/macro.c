



#include "macro.h"
#include "app/framework/include/af.h"


unsigned char xorStr(unsigned char *Str, unsigned char Length){
    unsigned char i;
    unsigned char xor = 0;
    for(i=0;i<Length;i++){
        xor^=Str[i];
    }
    return xor;
}

unsigned short SwapEndiannessInt16u(unsigned short val)
{
    unsigned short ret;

    ((unsigned char*)&ret)[0] = ((unsigned char*)&val)[1];
    ((unsigned char*)&ret)[1] = ((unsigned char*)&val)[0];

    return ret;
}
void SwapEndiannessEUI64(unsigned char *val)
{
    unsigned char ret[8];

    ret[0] = val[7];
    ret[1] = val[6];
    ret[2] = val[5];
    ret[3] = val[4];
    ret[4] = val[3];
    ret[5] = val[2];
    ret[6] = val[1];
    ret[7] = val[0];

    memcpy(val,ret, sizeof(ret));
}











