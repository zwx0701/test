/***************************************************************************************************************
Subroutine:             								�����źż��
Description:  									                          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "InChk.h"		
#include "App.h"
#include "Data.h"
//-------------------------------------------------------------
//	InChk ram define
//-------------------------------------------------------------
bit F_Chg_Exist;												//�г���־
bit F_Chk_Chg;													//������״̬
idata uint8_t RTm_Chk_Chg;							//����

bit F_DisChg_Exist = 1;									//�зŵ��־
bit F_Chk_DisChg = 1;										//�ŵ����״̬
idata uint8_t RTm_Chk_DisChg = LTm_Chk_DisChg;//����
idata uint8_t RTm_Chk_NDisChg = LTm_Chk_NDisChg;//����
//=============================================================
//	��ƽ������
//=============================================================
void Mn_InChk(void)
{																			
	Chk_Chg();														//������ź�	
	
  if(F_Chg_Exist == 0)
  {
   	Chk_DisChg();												//���ŵ�״̬	 
  }	
}
/****************************************************************************
* Function		: Chk_Chg
* Description	: ����źż��
* Input				: None			  				  
* Output			: None
****************************************************************************/
void Chk_Chg(void)
{		
	if(I_Chk_Chg == 1)										//������ź�
	{
		if(F_Chk_Chg == 0)
		{
			F_Chk_Chg = 1;
			RTm_Chk_Chg = LTm_Chk_Chg;				
		}
	}
	else
	{
		if(F_Chk_Chg == 1)
		{
			F_Chk_Chg = 0;
			RTm_Chk_Chg = LTm_Chk_Chg;
		}
	}
}
/****************************************************************************
* Function		: Chk_DisChg
* Description	: �ŵ��źż��
* Input				: None			  				  
* Output			: None
* Note				: ��LEDʱ��LED���Ż������200ms�͵�ƽ��MCU�������������ƽ���ŵ�״̬
****************************************************************************/
void Chk_DisChg(void)
{
	if(I_Chk_DisChg == 0)//���ŵ��ź�
	{		
		if(F_Chk_DisChg == 0)
		{
			RTm_Chk_DisChg = LTm_Chk_DisChg;
			F_Chk_DisChg = 1;
		}
		else if(0 == RTm_Chk_DisChg)
		{
			F_Chk_DisChg = 0; 
			RTm_Chk_NDisChg = 0;
			if(F_DisChg_Exist == 1)
			{				
				F_Key_Push = 0;											
				R_Sys_Mode = 0;						
				RTm_Sys_Slp = 0; //������˯��	
				RTm_Delay_Slp = 0;	
				
				F_DisChg_Exist = 0;	//�ŵ����
				if(F_Chg_Exist == 0)
				{
//					O_Chg_DisChg_En = 1;//�ضϷŵ�	
					
					if((F_UartRx_En == 0) && (F_UartTx_En == 0))	
					{
//						RTm_Delay_Slp = LTm_Delay_Slp; 	//MCU��ʱ10s����˯��
						RTm_Sys_Slp = LTm_Sys_Slp; 									//���¿�ʼ˯�߶�ʱ								
					}					
				}	
				
			}			
		}
	}
	else
	{
		RTm_Chk_DisChg = 0;
		F_Chk_DisChg = 0;		
	}	
	if(RTm_Chk_NDisChg==LTm_Chk_NDisChg)
	{
		if(F_DisChg_Exist == 0)
		{
			F_DisChg_Exist = 1;	//�ŵ�		
			O_Chg_DisChg_En = 0;//���ŵ�	
			
			F_Key_Push = 0;											
			R_Sys_Mode = 0;						
			RTm_Sys_Slp = 0;//������˯��	
			RTm_Delay_Slp = 0;					
		}		
	}

}
