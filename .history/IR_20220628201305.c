#include <REGX52.H>
#include "IR.h"

/**
 * @brief  红外专属延迟——i=1时延迟约为10us
 * @param  i 延迟时长
 */
void IR_delay(u16 i)
{
    while (i--)
        ;
}

/**
 * @brief  红外初始化
 */
void IrInit()
{
    EA = 1;  //打开总中断
    EX0 = 1; //打开中断0允许
    IT0 = 1; //下降沿触发

    IRIN = 1; //初始化端口
}

/**
 * @brief  识别红外命令解析为10进制数
 * @param  IR_sum 命令码
 */
u8 Identify_Num(u8 IR_sum)
{
    u8 Num_command;
    switch (IR_sum)
    {
    case 16:
        Num_command = 0;
        break;
    case 12:
        Num_command = 1;
        break;
    case 18:
        Num_command = 2;
        break;
    case 64:
        Num_command = 3;
        break;
    case 8:
        Num_command = 4;
        break;
    case 22:
        Num_command = 5;
        break;
    case 60:
        Num_command = 6;
        break;
    case 42:
        Num_command = 7;
        break;
    case 52:
        Num_command = 8;
        break;
    case 50:
        Num_command = 9;
        break;
    case 46:
        Num_command = 69; // Mode
        break;
    case 47:
        Num_command = 96; // stop voice
        break;
    case 40:
        Num_command = 23; //<<
        break;
    case 43:
        Num_command = 32; //>>
        break;
    }
    return Num_command;
}
// u8 IR_command[7][3] = {
// 	{45, 46, 47}, // power,mode,47
// 	{44, 40, 43}, // stop,<<,>>
// 	{7, 15, 9},	  // EQ,vol-.vol+
// 	{16, 19, 13}, // 0,RPT,U/SD
// 	{12, 18, 64}, // 1,2,3
// 	{8, 22, 60},  // 4,5,6
// 	{42, 52, 50}, // 7,8,9
// };