#include <REGX52.H>
#include "IR.h"

/**
 * @brief  红外专属延迟
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
 * @brief  识别红外数字
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
        Num_command = 69;
        break;
    case 47:
        Num_command = 96;
        break;
    case 46:
        Num_command = 69;
        break;
    case 46:
        Num_command = 69;
        break;
    }
    return Num_command;
}