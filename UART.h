#ifndef __UART_H__
#define __UART_H__

void UART_Init();
void UART_SendByte(unsigned char Byte);
void UART_Send_16bit_Data(unsigned int value);
void UART_Send_String(unsigned char *string);

#endif
