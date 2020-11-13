/*********************************************************************************************************
Subroutine:             										定时程序
Description:                                   
Input:                                         	
Output:                                        
**********************************************************************************************************/
#include <SN8F5702.h>	
#include "IntGnd.h"
#include "InChk.h"
#include "App.h"
#include "Data.h"
#include "AD.h"	
#include "Ntc.h"
#include "UART.h"    	
//-------------------------------------------------------------
//	time ram define
//-------------------------------------------------------------																	
bit F_5Ms;																	//5ms时间溢出标志														

//=============================================================
//	定时程序
//=============================================================
void Mn_IntGnd(void)
{
	if(F_5Ms)																	//5ms时间到
	{
		F_5Ms = 0;
		ClearUTXIO();
		cRxTick++;		
		if(cRxTick >21)
			cRxTick = 21;
		if(RTm_Key !=0)
		{
			RTm_Key--;
			if(RTm_Key == 0)
			{
				O_KEY = 1;
			}
		}
		F_AD_Updata = 1;												//5ms采集一次AD值
		
		Tm_Sys_Slp();														//系统睡眠定时
		Tm_Delay_Slp();													//MCU延时进入睡眠
		
		Tm_Chk_Chg();														//检测充电信号消抖
		Tm_Chk_DisChg();												//检测放电信号消抖		
		MINKey_Intgnd();		
	}
}
/****************************************************************************
* Function		  : Tm_Sys_Slp
* Description		: 系统睡眠定时	
* Input					: RTm_Sys_Slp 				  
* Output				: RTm_Sys_Slp
****************************************************************************/
void Tm_Sys_Slp(void)
{
	if(RTm_Sys_Slp > 0)
	{
		RTm_Sys_Slp--;
		if(RTm_Sys_Slp == 0)	
		{
			F_Key_Push = 1;												//进入睡眠
			R_Sys_Mode = 1;												
		}
	}
}
/****************************************************************************
* Function		  : Tm_Sys_Slp
* Description		: 系统延时进入睡眠定时	
* Input					: RTm_Sys_Slp 				  
* Output				: RTm_Sys_Slp
****************************************************************************/
void Tm_Delay_Slp(void)
{
	if(RTm_Delay_Slp > 0)
	{
		RTm_Delay_Slp--;
		if(RTm_Delay_Slp == 0)	
		{
			if((F_UartRx_En == 0) && (F_UartTx_En == 0))	
			{
				if((F_Chg_Exist == 0) && (F_DisChg_Exist == 0))
				{
					RTm_Sys_Slp = LTm_Sys_Slp; 				//重新开始睡眠定时
				}					
			}																
		}
	}
}
/****************************************************************************
* Function		  : Tm_Chk_Chg
* Description		: 充电信号检测消抖
* Input					: RTm_Chk_Chg	  				  
* Output				: RTm_Chk_Chg
****************************************************************************/
void Tm_Chk_Chg(void)
{
	if(RTm_Chk_Chg > 0)
	{
		RTm_Chk_Chg--;
		if(RTm_Chk_Chg == 0)
		{				
			F_Key_Push = 0;											
			R_Sys_Mode = 0;						
			RTm_Sys_Slp = 0; 											//不进入睡眠	
			RTm_Delay_Slp = 0;
			
			if(F_Chk_Chg)
			{
				F_Chg_Exist = 1;										//充电				
												
				F_Charge_Ok = 1;

				O_Chg_DisChg_En = 0;								//开充电
				
				if(R_Rt_Temp <= 125)//全速充电
				{
					O_Chg_Half_En = 0;								//关半速充电	
					O_Chg_DisChg_En = 0;
				}
				else//半速充电
				{
					O_Chg_Half_En = 1;								//开半速充电	
					O_Chg_DisChg_En = 0;					
				}					
			}
			else
			{
				F_Chg_Exist = 0;										//充电拔掉
				
				O_Chg_Half_En = 0;									//关断半速充电					

				if(F_DisChg_Exist == 0)								
				{
//					O_Chg_DisChg_En = 1;							//关充电	
					
					if((F_UartRx_En == 0) && (F_UartTx_En == 0))	
					{
						RTm_Sys_Slp = LTm_Sys_Slp; 			//重新开始睡眠定时
					}					
				}					
			}
		}
	}
}
/****************************************************************************
* Function		  : Tm_Chk_DisChg
* Description		: 放电信号检测消抖
* Input					: RTm_Chk_DisChg  				  
* Output				: RTm_Chk_DisChg
****************************************************************************/
void Tm_Chk_DisChg(void)
{
	if(RTm_Chk_DisChg !=0)
		RTm_Chk_DisChg--;	
	if(RTm_Chk_NDisChg <LTm_Chk_NDisChg)
		RTm_Chk_NDisChg++;
}
/****************************************************************************
* Function		  : Tm_Uart_Reset
* Description		: 20MS未收到数据，复位UART
* Input					: RTm_Uart_Reset				  
* Output				: RTm_Uart_Reset
****************************************************************************/
//void Tm_Uart_Reset(void)
//{
//	if(RTm_Uart_Reset > 0)
//	{
//		RTm_Uart_Reset--;
//		if(RTm_Uart_Reset == 0)	
//		{
//			F_Uart_Rx = 0;

//			F_UartRx_En = 0;											//关UART接收
//			F_UartRx_Stop = 0;
//			R_UartRx_Bit = 8;											//8bit不带校验位	
//			R_UartRx_Byte = 0;
//		
//			TR0 = 0;                							//Disable T0 timer function
//			IEN0 |= 0x01;       									//INT0 isr enable (EX0) 

//			Uart_TxM_IN;
//			
//			if((F_Chg_Exist == 0) && (F_DisChg_Exist == 0))
//			{
//				RTm_Sys_Slp = LTm_Sys_Slp; 					//开始睡眠定时	
//			}				
//		}
//	}
//}
/****************************************************************************
* Function		  : Tm_Rt_Chat
* Description		: 温度在1度跳动时5S变化一次
* Input					: RTm_Rt_Chat  				  
* Output				: RTm_Rt_Chat
****************************************************************************/
//void Tm_Rt_Chat(void)
//{
//	if(RTm_Rt_Chat > 0)
//	{
//		RTm_Rt_Chat--;							
//	}
//}
//-----------------------------------------------------------------------------
//  Timer2 ISR
//-----------------------------------------------------------------------------
void Timer2_ISR(void) interrupt ISRTimer2 	//Vector @  0x2B
{
	if(TF2 == 1)															//必须软件判断和清零T2中断标志位
	{
		TF2 = 0;                								//Clear Timer2 overflow flag 
										
		F_5Ms = 1;															//定时5ms		
	}      
}
