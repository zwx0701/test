/***************************************************************************************************************
Subroutine:             								�¶ȿ���
Description:  									                          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "Ntc.h"
#include "AD.h"
#include "App.h"
#include "InChk.h"	
#include "Sub.h"
#include "Tab.h"

//-------------------------------------------------------------
//	NTC ram define
//-------------------------------------------------------------
bit F_Ntc_Updata;												//�ɼ����NTC��ADֵ
idata uint16_t R_Ntc_AD;								//����ÿ�βɼ�����ADֵ

idata uint16_t R_NtcFf[8];							//����ÿ�βɼ���ADֵ�����ڻ���ƽ��
idata uint8_t R_NtcFf_Wt;

idata uint8_t R_Ntc_Debouce;						//���ȼ�����

idata uint8_t R_Rt_Temp;								//���Ե��¶���ֵ
//idata uint8_t R_Rt_TempBuf;							//��ʱ�洢��ʾ���Ե��¶���ֵ
//idata uint8_t RTm_Rt_Chat;							//�¶���1������ʱ4S�仯һ��

idata uint8_t RTm_RtLevel_Chat;					//����¶ȵȼ�����
idata uint8_t RTm_RtLevel_Chat1;				//����¶ȵȼ�����

bit F_Ntc_Error;												//�¶��쳣��־

//*********************************************************************************
//	�¶ȳ���
//*********************************************************************************
void Mn_Ntc(void)
{
	uint8_t R_Class_CAG = 0;
	
	if(F_Ntc_Updata)
	{
		F_Ntc_Updata = 0;
		
		R_AD_Data = R_Ntc_AD;	
		
//==���ʵ���¶�ֵ	
		R_Wk00 = BinChk_Rank(Temp_AD,135);	//ʵʱ��⵱ǰ�¶�	
		
		R_Rt_Temp = Temp_Num[R_Wk00];

//==���״̬	
		if(F_Chg_Exist)									
		{																		//���״̬
			if(O_Chg_DisChg_En == 1)//1�رճ��״̬
			{																	//���ض�
				if((R_Rt_Temp >= 88) && (R_Rt_Temp <= 137))
				{																//5��~60��   +80    85��~140��
					RTm_RtLevel_Chat++;
					if(RTm_RtLevel_Chat >= 100)
					{
						RTm_RtLevel_Chat = 0;
						O_Chg_DisChg_En = 0;	//�����������	
						O_Chg_Half_En = 1;//���ٳ��						
					}																			
				}
				else
				{
					RTm_RtLevel_Chat = 0;
				}
			}
			else//2����ȫ��
			{
				if((R_Rt_Temp <= 85) || (R_Rt_Temp >= 140))//3�¶��쳣С��5�Ⱥʹ���60��
				{
					RTm_RtLevel_Chat++;
					if(RTm_RtLevel_Chat >= 100)
					{
						O_Chg_Half_En = 1;					//�ضϰ��ٳ��			
						O_Chg_DisChg_En = 1;				//�ضϳ��	
					}										
				}
				else//4�¶�����5~60��
				{
					RTm_RtLevel_Chat = 0;

					if(O_Chg_Half_En == 1)//���ٳ�״̬
					{															//���ٳ�翪��
						if((R_Rt_Temp <= 122)&&(R_Rt_Temp >= 98))
						{														//15��   45��   +80    95   125
							RTm_RtLevel_Chat1++;
							if(RTm_RtLevel_Chat1 >= 100)
							{
								RTm_RtLevel_Chat1 = 0;
								O_Chg_Half_En = 0;		
								O_Chg_DisChg_En = 0;								
							}															
						}
						else
						{
							RTm_RtLevel_Chat1 = 0;
						}
					}
					else//ȫ�ٳ�״̬
					{
						//if(R_Rt_Temp >= 125)
						if(((R_Rt_Temp >= 88)&&(R_Rt_Temp <= 95))||((R_Rt_Temp >= 125)&&(R_Rt_Temp <= 137)))
						{	
							RTm_RtLevel_Chat1++;
							if(RTm_RtLevel_Chat1 >= 100)
							{
								RTm_RtLevel_Chat1 = 0;
								O_Chg_Half_En = 1;		//�������ٳ��
								O_Chg_DisChg_En = 0;										
							}																		
						}
						else
						{
							RTm_RtLevel_Chat1 = 0;
						}
					}						
				}
			}
		}
		else
		{
//====�ŵ�״̬		
			if(F_DisChg_Exist)									
			{																		//�ŵ�״̬
				if(O_Chg_DisChg_En == 1)//�ر�״̬
				{																	//�ŵ�ض�
					if((R_Rt_Temp >= 78) && (R_Rt_Temp <= 137))			
					{																//-5��~60��   +80    75��~140��
						RTm_RtLevel_Chat++;
						if(RTm_RtLevel_Chat >= 100)
						{
							RTm_RtLevel_Chat = 0;
							O_Chg_DisChg_En = 0;				//���������ŵ�	
							O_Chg_Half_En	 = 0;						
						}							
					}
					else
					{
						RTm_RtLevel_Chat = 0;
					}
				}
				else//ȫ�ٺͰ��� 
				{
					if((R_Rt_Temp <= 75) || (R_Rt_Temp >= 140))
					{	
						RTm_RtLevel_Chat++;
						if(RTm_RtLevel_Chat >= 100)
						{
							RTm_RtLevel_Chat = 0;
							O_Chg_DisChg_En = 1;				//�ضϷŵ�
							O_Chg_Half_En	 = 1;		
						}						
					}
					else
					{
						RTm_RtLevel_Chat = 0;
					}
				}
			}			
		}
//		R_Rt_TempBuf = Temp_Num[R_Wk00];

//		if((R_Rt_TempBuf > (R_Rt_Temp + 1)) || (R_Rt_Temp > (R_Rt_TempBuf + 1)) || (RTm_Rt_Chat == 0))
//		{	
//			R_Rt_Temp = R_Rt_TempBuf;					//�¶���1������ʱ5S�仯һ��
//			RTm_Rt_Chat = LTm_Rt_Chat;
//		}	
	}	
}