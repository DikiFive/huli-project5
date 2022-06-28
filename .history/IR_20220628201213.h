#ifndef __IR_h__
#define __IR_h__

typedef unsigned int u16; //对数据类型进行声明定义
typedef unsigned char u8;

struct ir
{
    u8 IrValue[6];
    u8 Time;
    u8 IR_sum;
    u8 IR_num;
};

sbit IRIN = P3 ^ 2;

void IR_delay(u16 i);
void IrInit();
u8 Identify_Num(u8 IR_sum);

#endif