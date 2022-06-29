#ifndef __dk_h__
#define __dk_h__

struct ks
{
    u8 KeyNum;
    u8 MatrixKey;
    u8 Num;
    u8 test;
    u8 sum;
    u8 nums;
    u8 Ua
};

void Nt(unsigned char location, num);
void Timer0_Init();
void Delay(unsigned int xms);
unsigned char Key();
unsigned char MatrixKey();

#endif