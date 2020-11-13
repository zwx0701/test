/*********************************************************************************************************
Subroutine:             										��ʱ����
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
bit F_5Ms;																	//5msʱ�������־														

//=============================================================
//	��ʱ����
//=============================================================
void Mn_IntGnd(void)
{
	if(F_5Ms)																	//5msʱ�䵽
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
		F_AD_Updata = 1;												//5ms�ɼ�һ��ADֵ
		
		Tm_Sys_Slp();														//ϵͳ˯�߶�ʱ
		Tm_Delay_Slp();													//MCU��ʱ����˯��
		
		Tm_Chk_Chg();														//������ź�����
		Tm_Chk_DisChg();												//���ŵ��ź�����		
		MINKey_Intgnd();		
	}
}
/****************************************************************************
* Function		  : Tm_Sys_Slp
* Description		: ϵͳ˯�߶�ʱ	
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
			F_Key_Push = 1;												//����˯��
			R_Sys_Mode = 1;												
		}
	}
}
/****************************************************************************
* Function		  : Tm_Sys_Slp
* Description		: ϵͳ��ʱ����˯�߶�ʱ	
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
					RTm_Sys_Slp = LTm_Sys_Slp; 				//���¿�ʼ˯�߶�ʱ
				}					
			}																
		}
	}
}
/****************************************************************************
* Function		  : Tm_Chk_Chg
* Description		: ����źż������
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
			RTm_Sys_Slp = 0; 											//������˯��	
			RTm_Delay_Slp = 0;
			
			if(F_Chk_Chg)
			{
				F_Chg_Exist = 1;										//���				
												
				F_Charge_Ok = 1;

				O_Chg_DisChg_En = 0;								//�����
				
				if(R_Rt_Temp <= 125)//ȫ�ٳ��
				{
					O_Chg_Half_En = 0;								//�ذ��ٳ��	
					O_Chg_DisChg_En = 0;
				}
				else//���ٳ��
				{
					O_Chg_Half_En = 1;								//�����ٳ��	
					O_Chg_DisChg_En = 0;					
				}					
			}
			else
			{
				F_Chg_Exist = 0;										//���ε�
				
				O_Chg_Half_En = 0;									//�ضϰ��ٳ��					

				if(F_DisChg_Exist == 0)								
				{
//					O_Chg_DisChg_En = 1;							//�س��	
					
					if((F_UartRx_En == 0) && (F_UartTx_En == 0))	
					{
						RTm_Sys_Slp = LTm_Sys_Slp; 			//���¿�ʼ˯�߶�ʱ
					}					
				}					
			}
		}
	}
}
/****************************************************************************
* Function		  : Tm_Chk_DisChg
* Description		: �ŵ��źż������
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
* Description		: 20MSδ�յ����ݣ���λUART
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

//			F_UartRx_En = 0;											//��UART����
//			F_UartRx_Stop = 0;
//			R_UartRx_Bit = 8;											//8bit����У��λ	
//			R_UartRx_Byte = 0;
//		
//			TR0 = 0;                							//Disable T0 timer function
//			IEN0 |= 0x01;       									//INT0 isr enable (EX0) 

//			Uart_TxM_IN;
//			
//			if((F_Chg_Exist == 0) && (F_DisChg_Exist == 0))
//			{
//				RTm_Sys_Slp = LTm_Sys_Slp; 					//��ʼ˯�߶�ʱ	
//			}				
//		}
//	}
//}
/****************************************************************************
* Function		  : Tm_Rt_Chat
* Description		: �¶���1������ʱ5S�仯һ��
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
	if(TF2 == 1)															//��������жϺ�����T2�жϱ�־λ
	{
		TF2 = 0;                								//Clear Timer2 overflow flag 
										
		F_5Ms = 1;															//��ʱ5ms		
	}      
}
