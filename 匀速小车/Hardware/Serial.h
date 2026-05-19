#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "stdio.h"



void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *a,uint16_t Length);
void Serial_SendNumber(uint16_t number,uint8_t Length);
void Serial_Printf(char* format,...);
void  USART1_IRQHandler(void);


#endif
