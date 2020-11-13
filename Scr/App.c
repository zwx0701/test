/***************************************************************************************************************
Subroutine:             													应用程序
Description:  																	                          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "App.h"
#include "InChk.h"
#include "Data.h"
#include "Sub.h"

//-------------------------------------------------------------
//	app ram define
//-------------------------------------------------------------
bit F_Key_Push;
idata uint8_t R_Sys_Mode;													//系统工作模式
//idata uint8_t RTm_Sys_Slp = LTm_Sys_Slp;					//系统进入睡眠定时
idata uint8_t RTm_Sys_Slp;												//系统进入睡眠定时
idata uint16_t RTm_Delay_Slp;											//MCU延时进入睡眠
bit F_Charge_Ok;																	//充电OK标志
idata uint8_t RTm_Key;														//短按键计数
bit F_Min;//一分半标志	
idata uint8_t RTm_1S;
idata uint8_t RTm_30S;
idata uint8_t RTm_30S_Cnt;
//=============================================================
//	应用程序
/*
睡眠触发事件：
1、数据应答完成     1.25S后睡眠
2、放电结束         10S+1.25S后睡眠
3、拔掉充电         1.25S后睡眠
4、接收数据错误			1.25S后睡眠
5、睡眠误唤醒			 	1.25S后睡眠
*/
//=============================================================
void Mn_App(void)		
{	
	uint8_t loop,j;
	if (j==0)
	{
		j++;
	}
	else
	{
		j--;
	}
	
	if(F_Key_Push)														
	{
		F_Key_Push = 0;																//有按键按下
		if(R_Sys_Mode == 1)															
		{																							//进入睡眠
			//if(((P0 & 0x07) == 0x05) && (F_UartRx_En == 0) && (F_UartTx_En == 0))					
			if((I_Chk_DisChg ==0) &&(I_SW1 ==1) &&(I_Chk_Chg ==0) && (F_UartRx_En == 0) && (F_UartTx_En == 0))	
			{																						//判断未上锁，充电线拔掉且UART接收和发射完成
				O_Chg_DisChg_En = 0;											//开充电			
				O_Chg_Half_En = 0;												//关半速充电					
				ADM = 0;																	//关AD	
				P05 = 0;
				REN0=0;				
				Goto_Sleep:		
//					Uart_TxM_IN;					
				STOP();	
				REN0=1;				
				//P05 = 1;
				Delay_5us();															//延时5us等待电平稳定	
				//Goto_Sleep10:							
				if(I_Uart_Rx)													
				{																					//非通讯口唤醒
					j = ReadIO();
					if(j != 0X00)
					{
						loop=10;
						while((j == ReadIO())&&((--loop) != 0));	
						if(0 != loop)
						{
							goto Goto_Sleep;								//干扰唤醒，继续进入睡眠
						}
						else//充电口和放电口唤醒
						{
							if(O_Output_En)//软件锁打开了
							{
								MINKey();//唤醒IP5189	
							}								
						}
					}
					else//抖动开关唤醒
					{
						if(O_Output_En)//软件锁打开了
						{
							MINKey();//唤醒IP5189		
						}							
					}						
				}	
				else//UART唤醒
				{
//					if(O_Output_En)//软件锁打开了
//					{
//						MINKey();//唤醒IP5189	
//					}						
				}	
				ADM = 0x83;																//ADC enable (ADENB AIN3)
			}
			R_Sys_Mode = 0;	
			RTm_Sys_Slp = LTm_Sys_Slp; 									//重新开始睡眠定时		
		}	
	}
}
//*****************************************************************************
// Function			: ReadIO
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
uint8_t ReadIO(void)
{
	idata uint8_t Wake=0;	
	Wake |= P0&0X80;//  P07
	Wake |= P1&0X10;//P14
	return Wake;
}
//*****************************************************************************
// Function			: ShortKey
// Description		: 
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
//void ShortKey(void)
//{
//	O_KEY = 0;
//	RTm_Key = LTm_KEY;//200MS
//}
//*****************************************************************************
// Function			: MINKey
// Description		: 三次按键
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void MINKey(void)
{
	if(F_Min !=0)//先关
	{
		F_Min = 0;
		RTm_30S_Cnt = 0;
		O_KEY = 1;	
		RTm_Key = 0;
		RTm_30S = 0;
		RTm_1S = 0;			
	}	
	O_KEY = 0;
	RTm_Key = LTm_KEY;//200MS
	F_Min = 1;
	RTm_30S = 0;
	RTm_30S_Cnt = 0;  
	RTm_1S = 0;	
}
void CloseKey(void)
{
	if(F_Min !=0)
	{
		F_Min = 0;
		RTm_30S_Cnt = 0;
		O_KEY = 1;	
		RTm_Key = 0;
		RTm_30S = 0;
		RTm_1S = 0;		
	}
}
void MINKey_Intgnd(void)
{
	if(F_Min == 1)
	{
		RTm_1S++;
		if(RTm_1S >=200)
		{
			RTm_1S = 0;
			RTm_30S++;
			if(RTm_30S>=23)
			{
				RTm_30S = 0;
				RTm_30S_Cnt++;
				if(RTm_30S_Cnt >=1)
				{
					RTm_30S_Cnt = 0;
					F_Min = 0;
				}
				else
				{
					O_KEY = 0;
					RTm_Key = LTm_KEY;//200MS   				
				}
			}			
		}
	}
}