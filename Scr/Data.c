/***************************************************************************************************************
Subroutine:             												通讯数据处理
Description:  									                          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "Data.h"		
#include "App.h"
#include "InChk.h"
#include "AD.h"
#include "Ntc.h"
#include "Sub.h"	
#include "UART.h"    
//-------------------------------------------------------------
//	Data ram define
//-------------------------------------------------------------
bit F_UartRx_En;																//UART接收开始
idata uint8_t R_UartRx_Byte;										//统计接收的BYTE数
idata uint8_t R_Uart_RxBuf[L_Uart_RxBuf];				//UART接收BUF

bit F_UartTx_En;																//UART发送开始
idata uint8_t R_UartTx_Byte;										//统计发送的BYTE数
idata uint8_t R_Uart_TxBuf[L_Uart_TxBuf];				//UART发送BUF

idata uint8_t  Temperture;//温度代号
idata uint16_t  BatValue;
//=============================================================
//	电平检测程序
//=============================================================
void Mn_Data(void)
{	
	if(cRxTick ==20)//接收数据OK
	{
		F_UartRx_En = 0;	
		if(R_UartRx_Byte >0)	
		{					
			if((R_Uart_RxBuf[0]  == 0X5E) && (R_Uart_RxBuf[1] == 0XA1))//查询充电宝状态
			{
				P05 = 1;
				Uart_Load_Data();													//加载UART_TXBUF数据
				
				if(O_Output_En == 1)//Bit4=0 软件锁关闭 Bit4=1 软件锁打开
				{
					R_Uart_TxBuf[8] |= 0x10;//软件锁打开						
				}
	//			else
	//			{
	//				R_Uart_TxBuf[8] &= 0XEF;//软件锁关闭					
	//			}
				R_Uart_TxBuf[10] = (R_Uart_TxBuf[0] + R_Uart_TxBuf[1] + R_Uart_TxBuf[2] + R_Uart_TxBuf[3] + R_Uart_TxBuf[4] +
													 R_Uart_TxBuf[5] + R_Uart_TxBuf[6] + R_Uart_TxBuf[7] + R_Uart_TxBuf[8] + R_Uart_TxBuf[9]);			
				RTm_Sys_Slp = 0;//不进入睡眠		
				RTm_Delay_Slp = 0;	
				UART_SendNByte();//Uart_Tx_Start(L1_Uart_TxBuf);							//启动UART发送						
			}
			else if((R_Uart_RxBuf[0] == 0x7C) && (R_Uart_RxBuf[1] == 0X83))//关闭软件锁
			{	
				P05 = 1;
				Uart_Load_Data();												//加载UART_TXBUF数据		
				//R_Uart_TxBuf[8] &= 0XEF;//软件锁关闭  //Bit4=0 软件锁关闭 Bit4=1 软件锁打开				
				R_Uart_TxBuf[10] = (R_Uart_TxBuf[0] + R_Uart_TxBuf[1] + R_Uart_TxBuf[2] + R_Uart_TxBuf[3] + R_Uart_TxBuf[4] +
													 R_Uart_TxBuf[5] + R_Uart_TxBuf[6] + R_Uart_TxBuf[7] + R_Uart_TxBuf[8] + R_Uart_TxBuf[9]);
				if(O_Output_En == 1)
				{
					O_Output_En = 0;											//关输出
					
					Isp_WrRom();													//写Rom
				}
				RTm_Sys_Slp = 0;//不进入睡眠		
				RTm_Delay_Slp = 0;					
				UART_SendNByte();//Uart_Tx_Start(L_Uart_TxBuf);						//启动UART发送	
				CloseKey();
			}
			else if((R_Uart_RxBuf[0] == 0X5C) && (R_Uart_RxBuf[1] == 0XA3))//打开软件锁
			{	
				P05 = 1;
				BatValue = (uint16_t)(R_Vbat_AD*0.732421875);//     R_Vbat_AD/4096*3*1000
				Uart_Load_Data();												//加载UART_TXBUF数据			
				R_Uart_TxBuf[8] |= 0x10;//软件锁打开 //Bit4=0 软件锁关闭 Bit4=1 软件锁打开							
				R_Uart_TxBuf[10] = (R_Uart_TxBuf[0] + R_Uart_TxBuf[1] + R_Uart_TxBuf[2] + R_Uart_TxBuf[3] + R_Uart_TxBuf[4] +
													 R_Uart_TxBuf[5] + R_Uart_TxBuf[6] + R_Uart_TxBuf[7] + R_Uart_TxBuf[8] + R_Uart_TxBuf[9]);			
				if(O_Output_En == 0)
				{
					O_Output_En = 1;											//开输出
					
					Isp_WrRom();													//写Rom
				}
				RTm_Sys_Slp = 0;//不进入睡眠		
				RTm_Delay_Slp = 0;					
				UART_SendNByte();//Uart_Tx_Start(L_Uart_TxBuf);						//启动UART发送
				MINKey();//唤醒IP5189				
			}		
			else
			{
				//Uart_TxM_IN;				
				if((F_Chg_Exist == 0) && (F_DisChg_Exist == 0))
				{
					RTm_Sys_Slp = LTm_Sys_Slp; 							//开始睡眠定时	
				}			
			}
		}
		R_UartRx_Byte = 0;			
	}
}