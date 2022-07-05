#include <REGX52.H>
#include "STDIO.H"
#include "STRING.H"
#include "UART.h"

/**
 * @brief  串口初始化，4800bps@12.000MHz
 * @param  无
 * @retval 无
 */
void UART_Init()
{
	// TL1 = 0xF3;	  //设定定时初值
	// TH1 = 0xF3;	  //设定定时器重装值
	SCON = 0x50; //八位数据 可变波特率
	TMOD &= 0x0F; //清除定时器1模式位 0000 1111
	TMOD |= 0x20; //设定定时器1为8位自动重装方式 0010 0000
	TH1 = 0xFA;	  //设定定时器重装值
	TL1 = 0xFA;	  //设定定时初值
	ET1 = 0;	  //禁止定时器1中断
	TR1 = 1;	  //启动定时器1
	EA = 1;		  //总中断允许位置
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
 * @brief  串口发送字符串函数
 * @param  string 要发送的字符串
 * @retval 无
 */
void UART_Send_String(unsigned char *string)
{
	while (*string)
	{
		UART_SendByte(*string);
		string++;
	}
}

/**
 * @brief  串口发送16bit的十进制数函数
 * @param  value 要发送的十进制数字
 * @retval 无
 */
void UART_Send_16bit_Data(unsigned int value)
{
	char strings[8];
	sprintf(strings, "%d", value);
	UART_Send_String(strings);
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
