#include <REGX52.H>
#include "IR.h"

/**
 * @brief  串口初始化，4800bps@12.000MHz
 * @param  无
 * @retval 无
 */
void UART_Init()
{
	SCON = 0x50;
	PCON |= 0x80;
	TMOD &= 0x0F; //设置定时器模式
	TMOD |= 0x20; //设置定时器模式
	TL1 = 0xF3;	  //设定定时初值
	TH1 = 0xF3;	  //设定定时器重装值
	ET1 = 0;	  //禁止定时器1中断
	TR1 = 1;	  //启动定时器1
	EA = 1;
	ES = 1;
}

/**
 * @brief  串口发送一个字节数据
 * @param  Byte 要发送的一个字节数据
 * @retval 无
 */
void UART_SendByte(unsigned char Byte)
{
	SBUF = Byte;
	while (TI == 0)
		;
	TI = 0;
}

/**
 * @brief  识别红外命令解析为10进制数
 * @param  IR_sum 命令码
 */
u8 Identify_UNum(u8 sum)
{
	u8 Num_command;
	switch (sum)
	{
	case 128:
		Num_command = 0;
		break;
	case 129:
		Num_command = 1;
		break;
	case 130;
		Num_command = 2;
		break;
	case 131:
		Num_command = 3;
		break;
	case 132:
		Num_command = 4;
		break;
	case 133:
		Num_command = 5;
		break;
	case 134:
		Num_command = 6;
		break;
	case 135:
		Num_command = 7;
		break;
	case 136:
		Num_command = 8;
		break;
	case 137:
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

/*串口中断函数模板
void UART_Routine() interrupt 4
{
	if(RI==1)
	{

		RI=0;
	}
}
*/
