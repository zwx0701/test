#ifndef _UART_h_
#define _UART_h_

#include "SN8F5702.h"
//波特率设置
//====================================================================================//
#define     	L_BRUD_RATE						L_BRUD_RATE_9600
#define				L_BRUD_RATE_4800			0x01	
#define				L_BRUD_RATE_9600			0x02	
#define				L_BRUD_RATE_19200			0x03	
#define				L_BRUD_RATE_38400			0x04	
#define				L_BRUD_RATE_56000			0x05	
#define				L_BRUD_RATE_57600			0x06	
#define				L_BRUD_RATE_115200		0x07	
#define				L_BRUD_RATE_128000		0x08	
#define				L_BRUD_RATE_250000		0x09	

#define				L_S0RELH_4800					0x03
#define				L_S0RELL_4800					0x98
#define				L_S0RELH_9600					0x03
#define				L_S0RELL_9600					0xcc
#define				L_S0RELH_19200				0x03
#define				L_S0RELL_19200					0xe6
#define				L_S0RELH_38400				0x03
#define				L_S0RELL_38400					0xf3
#define				L_S0RELH_56000				0x03
#define				L_S0RELL_56000					0xee
#define				L_S0RELH_57600				0x03
#define				L_S0RELL_57600					0xef
#define				L_S0RELH_115200				0x03
#define				L_S0RELL_115200				0xf7
#define				L_S0RELH_128000				0x03
#define				L_S0RELL_128000				0xf8
#define				L_S0RELH_250000				0x03
#define				L_S0RELL_250000				0xfc
																	
#define				L_EVEN_CHECK					0//偶校验	
#define				L_UNEVEN_CHECK				1//奇校验
#define				L_CHECK_MODEL				L_UNEVEN_CHECK	


extern idata	uint8_t	cRxTick;
extern idata	uint8_t 	cTxPointer;
extern idata	uint8_t	cTxClearCnt;

void UART3init(void);
void UARTwriteonebyte(uint8_t r_txdata);
void UART_SendNByte(void);	
void ClearUTXIO(void);
#endif

