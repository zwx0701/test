/***************************************************************************************************************
Subroutine:             														AD�������
Description:  																			                           								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "AD.h"
#include "Ntc.h"
#include "App.h"
#include "Sub.h"	

//-------------------------------------------------------------
//	AD ram define
//-------------------------------------------------------------
bit F_AD_Updata;																		//10ms�ɼ�AD
idata uint16_t R_AD_Data;														//�ɼ���AD����

bit F_Ain_Num;																			//��ǰ�ɼ�ͨ��

idata uint16_t R_Vbat_AD;

idata uint16_t R_VbatFf[8];													//����ÿ�βɼ���ADֵ�����ڻ���ƽ��
idata uint8_t R_VbatFf_Wt;

//=============================================================
//	AD�ɼ�����
//=============================================================
void Mn_AD(void)		
{
	if(F_AD_Updata)
	{
		F_AD_Updata = 0;
		
		if(F_Ain_Num == 0)
		{
			ADM = 0x89;																		//AIN9,�ɼ���ص�ѹ
															
			Get_AD();																			//�ɼ�ADֵ		
			
			WTAff(&R_VbatFf[R_VbatFf_Wt],&R_VbatFf_Wt,R_AD_Data);	
																										//����ADֵ
			R_Vbat_AD = AVERAGE(R_VbatFf);								//�󻬶�ƽ��	
//			R_Vbat_AD = (R_Vbat_AD * 3);
//			R_Vbat_AD = (R_Vbat_AD >> 1);
			
			F_Ain_Num = 1;
			
			VREFH = ((VREFH & 0xFC) | 0x03);							//��һ�βɼ��¶���VDD�ο�
		}
		else
		{
			ADM = 0x83;																		//AIN3,�ɼ��¶�		
														
			Get_AD();																			//�ɼ�ADֵ										
			WTAff(&R_NtcFf[R_NtcFf_Wt],&R_NtcFf_Wt,R_AD_Data);	
																										//����ADֵ
			R_Ntc_AD = AVERAGE(R_NtcFf);									//�󻬶�ƽ��							
			F_Ntc_Updata = 1;															//�ɼ����¶�	

			F_Ain_Num = 0;
			
			VREFH = (VREFH & 0xFC);												//��һ�βɼ���ѹ���ڲ�2V�ο�
		}
	}
}