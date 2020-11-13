/***************************************************************************************************************
Subroutine:             													Ӧ�ó���
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
idata uint8_t R_Sys_Mode;													//ϵͳ����ģʽ
//idata uint8_t RTm_Sys_Slp = LTm_Sys_Slp;					//ϵͳ����˯�߶�ʱ
idata uint8_t RTm_Sys_Slp;												//ϵͳ����˯�߶�ʱ
idata uint16_t RTm_Delay_Slp;											//MCU��ʱ����˯��
bit F_Charge_Ok;																	//���OK��־
idata uint8_t RTm_Key;														//�̰�������
bit F_Min;//һ�ְ��־	
idata uint8_t RTm_1S;
idata uint8_t RTm_30S;
idata uint8_t RTm_30S_Cnt;
//=============================================================
//	Ӧ�ó���
/*
˯�ߴ����¼���
1������Ӧ�����     1.25S��˯��
2���ŵ����         10S+1.25S��˯��
3���ε����         1.25S��˯��
4���������ݴ���			1.25S��˯��
5��˯������			 	1.25S��˯��
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
		F_Key_Push = 0;																//�а�������
		if(R_Sys_Mode == 1)															
		{																							//����˯��
			//if(((P0 & 0x07) == 0x05) && (F_UartRx_En == 0) && (F_UartTx_En == 0))					
			if((I_Chk_DisChg ==0) &&(I_SW1 ==1) &&(I_Chk_Chg ==0) && (F_UartRx_En == 0) && (F_UartTx_En == 0))	
			{																						//�ж�δ����������߰ε���UART���պͷ������
				O_Chg_DisChg_En = 0;											//�����			
				O_Chg_Half_En = 0;												//�ذ��ٳ��					
				ADM = 0;																	//��AD	
				P05 = 0;
				REN0=0;				
				Goto_Sleep:		
//					Uart_TxM_IN;					
				STOP();	
				REN0=1;				
				//P05 = 1;
				Delay_5us();															//��ʱ5us�ȴ���ƽ�ȶ�	
				//Goto_Sleep10:							
				if(I_Uart_Rx)													
				{																					//��ͨѶ�ڻ���
					j = ReadIO();
					if(j != 0X00)
					{
						loop=10;
						while((j == ReadIO())&&((--loop) != 0));	
						if(0 != loop)
						{
							goto Goto_Sleep;								//���Ż��ѣ���������˯��
						}
						else//���ںͷŵ�ڻ���
						{
							if(O_Output_En)//���������
							{
								MINKey();//����IP5189	
							}								
						}
					}
					else//�������ػ���
					{
						if(O_Output_En)//���������
						{
							MINKey();//����IP5189		
						}							
					}						
				}	
				else//UART����
				{
//					if(O_Output_En)//���������
//					{
//						MINKey();//����IP5189	
//					}						
				}	
				ADM = 0x83;																//ADC enable (ADENB AIN3)
			}
			R_Sys_Mode = 0;	
			RTm_Sys_Slp = LTm_Sys_Slp; 									//���¿�ʼ˯�߶�ʱ		
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
// Description		: ���ΰ���
// Input					: None
// Output				: None
// Return				: None
// Note					: None
//*****************************************************************************
void MINKey(void)
{
	if(F_Min !=0)//�ȹ�
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