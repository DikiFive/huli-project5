#include <REGX52.H>
#include "I2C.h"

#define AT24C02_ADDRESS 0xA0

/**
 * @brief  AT24C02写入一个字节
 * @param  WordAddress 要写入字节的地址
 * @param  Data 要写入的数据
 * @retval 无
 */
void AT24C02_WriteByte(unsigned char WordAddress, Data)
{
	I2C_Start();				   // I2C开始
	I2C_SendByte(AT24C02_ADDRESS); // I2C发送一个字节
	I2C_ReceiveAck();			   // I2C接收一个字节
	I2C_SendByte(WordAddress);	   // I2C发送一个字节
	I2C_ReceiveAck();			   // I2C接收一个字节
	I2C_SendByte(Data);			   // I2C发送数据
	I2C_ReceiveAck();			   // I2C接收数据
	I2C_Stop();					   // I2C停止
}

/**
 * @brief  AT24C02读取一个字节
 * @param  WordAddress 要读出字节的地址
 * @retval 读出的数据
 */
unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	I2C_Start();						  // I2C开始
	I2C_SendByte(AT24C02_ADDRESS);		  // I2C发送一个字节
	I2C_ReceiveAck();					  // I2C接收一个字节
	I2C_SendByte(WordAddress);			  // I2C发送一个字节
	I2C_ReceiveAck();					  // I2C接收一个字节
	I2C_Start();						  // I2C开始
	I2C_SendByte(AT24C02_ADDRESS | 0x01); // I2C发送一个字节
	I2C_ReceiveAck();					  // I2C接收一个字节
	Data = I2C_ReceiveByte();			  //接收到的数据赋值给Data
	I2C_SendAck(1);						  //确认
	I2C_Stop();							  // I2C停止
	return Data;						  //返回数据Data
}
