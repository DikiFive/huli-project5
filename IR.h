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

// u8 IR_command[7][3] = {
// 	{45, 46, 47}, // power,mode,47
// 	{44, 40, 43}, // stop,<<,>>
// 	{7, 15, 9},	  // EQ,vol-.vol+
// 	{16, 19, 13}, // 0,RPT,U/SD
// 	{12, 18, 64}, // 1,2,3
// 	{8, 22, 60},  // 4,5,6
// 	{42, 52, 50}, // 7,8,9
// };