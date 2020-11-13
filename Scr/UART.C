#include "UART.h"    									
#include "IntGnd.h"
#include "Data.h"	
#include "App.h"
#include "InChk.h"	
idata	uint8_t 	cTxPointer;
idata	uint8_t	cRxTick;
idata	uint8_t	cTxClearCnt;
//*****************************************************************************
// Function			: UART3init
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void UART3init(void)
{
//	P05 = 1;
	P0M |= 0x20;//UTX初始化输出高		
	P0M &= 0xbf;//URX初始化数模模式
	
	SM0 = 1;//模式3																		
	SM1 = 1;
	REN0 = 1;																			
//  case L_BRUD_RATE_9600:
	PCON &= 0x7f;       													
	BD = 1;				
	S0RELH = L_S0RELH_9600;
	S0RELL = L_S0RELL_9600;      						
	ES0 = 1;//使能UART中断
}
//*****************************************************************************
// Function			: UARTwriteonebyte
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void UARTwriteonebyte(uint8_t r_txdata)
{
	uint8_t i = 0;
	uint8_t temp_check = 0;
	uint8_t temp_number = 0;	
	temp_check = r_txdata;	
	for(i=0;i<8;i++)//判读发送数据‘1’的个数
	{
		if(temp_check & 0x01)
			temp_number++;
		temp_check = _crol_(temp_check,1);//右位判断下一位数据是否为‘1’	
	}	
	if(temp_number & 0x01)//奇校验  //第9位,奇偶校验的值
		TB80 = 0;
	else
		TB80 = 1;	
	S0BUF = r_txdata;
}
//*****************************************************************************
// Function			: UARTISR
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void UARTISR(void) interrupt ISRUart 
{
	uint8_t i = 0;
	uint8_t temp_check = 0;
	uint8_t temp_number = 0;
	if(RI0 == 1)                                      			
	{
		RI0 = 0;														
		temp_check = S0BUF;		
		for(i=0;i<8;i++)															
		{
			if((temp_check & 0x01) == 1)
				temp_number++;
			temp_check = _crol_(temp_check,1);
		}		
		if(RB80 == 1)																
			temp_number++;		
		if(temp_number & 0x01)//奇校验
		{
			R_Uart_RxBuf[R_UartRx_Byte++] = temp_check;	
			cRxTick = 0;	
			F_UartRx_En = 1;	
			
			F_Key_Push = 0;											
			R_Sys_Mode = 0;						
			RTm_Sys_Slp = 0; //不进入睡眠	
			RTm_Delay_Slp = 0;				
		}			
	}
	if(TI0)
	{
		TI0 = 0;		
		UART_SendNByte();	
	}
}
//*****************************************************************************
// Function			: UART0_SendNByte
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void UART_SendNByte(void)
{	
	if(cTxPointer < L_Uart_TxBuf)
	{
		F_UartTx_En = 1;
		UARTwriteonebyte(R_Uart_TxBuf[cTxPointer]);//发送数据
		cTxPointer++;	
	}
	else
	{
		//P05 = 0;
		cTxClearCnt = 2;
		cTxPointer = 0;
		F_UartTx_En = 0;
		if((F_Chg_Exist == 0) && (F_DisChg_Exist == 0))
		{
			RTm_Sys_Slp = LTm_Sys_Slp; 							//开始睡眠定时	
		}			
	}
}
//*****************************************************************************
// Function			: ClearUTXIO
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void ClearUTXIO(void)
{
	if(cTxClearCnt !=0)
	{
		cTxClearCnt--;
		if(cTxClearCnt == 0)
		{
			P05 = 0;
		}	
	}
}