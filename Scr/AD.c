/***************************************************************************************************************
Subroutine:             														AD处理程序
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
bit F_AD_Updata;																		//10ms采集AD
idata uint16_t R_AD_Data;														//采集的AD数据

bit F_Ain_Num;																			//当前采集通道

idata uint16_t R_Vbat_AD;

idata uint16_t R_VbatFf[8];													//保存每次采集的AD值，用于滑动平均
idata uint8_t R_VbatFf_Wt;

//=============================================================
//	AD采集程序
//=============================================================
void Mn_AD(void)		
{
	if(F_AD_Updata)
	{
		F_AD_Updata = 0;
		
		if(F_Ain_Num == 0)
		{
			ADM = 0x89;																		//AIN9,采集电池电压
															
			Get_AD();																			//采集AD值		
			
			WTAff(&R_VbatFf[R_VbatFf_Wt],&R_VbatFf_Wt,R_AD_Data);	
																										//保存AD值
			R_Vbat_AD = AVERAGE(R_VbatFf);								//求滑动平均	
//			R_Vbat_AD = (R_Vbat_AD * 3);
//			R_Vbat_AD = (R_Vbat_AD >> 1);
			
			F_Ain_Num = 1;
			
			VREFH = ((VREFH & 0xFC) | 0x03);							//下一次采集温度用VDD参考
		}
		else
		{
			ADM = 0x83;																		//AIN3,采集温度		
														
			Get_AD();																			//采集AD值										
			WTAff(&R_NtcFf[R_NtcFf_Wt],&R_NtcFf_Wt,R_AD_Data);	
																										//保存AD值
			R_Ntc_AD = AVERAGE(R_NtcFf);									//求滑动平均							
			F_Ntc_Updata = 1;															//采集完温度	

			F_Ain_Num = 0;
			
			VREFH = (VREFH & 0xFC);												//下一次采集电压用内部2V参考
		}
	}
}