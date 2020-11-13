/***************************************************************************************************************
Subroutine:             								温度控制
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
bit F_Ntc_Updata;												//采集完成NTC的AD值
idata uint16_t R_Ntc_AD;								//保存每次采集到的AD值

idata uint16_t R_NtcFf[8];							//保存每次采集的AD值，用于滑动平均
idata uint8_t R_NtcFf_Wt;

idata uint8_t R_Ntc_Debouce;						//检测等级消抖

idata uint8_t R_Rt_Temp;								//测试的温度数值
//idata uint8_t R_Rt_TempBuf;							//临时存储显示测试的温度数值
//idata uint8_t RTm_Rt_Chat;							//温度在1度跳动时4S变化一次

idata uint8_t RTm_RtLevel_Chat;					//检测温度等级消抖
idata uint8_t RTm_RtLevel_Chat1;				//检测温度等级消抖

bit F_Ntc_Error;												//温度异常标志

//*********************************************************************************
//	温度程序
//*********************************************************************************
void Mn_Ntc(void)
{
	uint8_t R_Class_CAG = 0;
	
	if(F_Ntc_Updata)
	{
		F_Ntc_Updata = 0;
		
		R_AD_Data = R_Ntc_AD;	
		
//==检测实际温度值	
		R_Wk00 = BinChk_Rank(Temp_AD,135);	//实时检测当前温度	
		
		R_Rt_Temp = Temp_Num[R_Wk00];

//==充电状态	
		if(F_Chg_Exist)									
		{																		//充电状态
			if(O_Chg_DisChg_En == 1)//1关闭充电状态
			{																	//充电关断
				if((R_Rt_Temp >= 88) && (R_Rt_Temp <= 137))
				{																//5°~60°   +80    85°~140°
					RTm_RtLevel_Chat++;
					if(RTm_RtLevel_Chat >= 100)
					{
						RTm_RtLevel_Chat = 0;
						O_Chg_DisChg_En = 0;	//重新启动充电	
						O_Chg_Half_En = 1;//半速充电						
					}																			
				}
				else
				{
					RTm_RtLevel_Chat = 0;
				}
			}
			else//2半充和全充
			{
				if((R_Rt_Temp <= 85) || (R_Rt_Temp >= 140))//3温度异常小于5度和大于60度
				{
					RTm_RtLevel_Chat++;
					if(RTm_RtLevel_Chat >= 100)
					{
						O_Chg_Half_En = 1;					//关断半速充电			
						O_Chg_DisChg_En = 1;				//关断充电	
					}										
				}
				else//4温度正常5~60度
				{
					RTm_RtLevel_Chat = 0;

					if(O_Chg_Half_En == 1)//半速充状态
					{															//半速充电开启
						if((R_Rt_Temp <= 122)&&(R_Rt_Temp >= 98))
						{														//15°   45°   +80    95   125
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
					else//全速充状态
					{
						//if(R_Rt_Temp >= 125)
						if(((R_Rt_Temp >= 88)&&(R_Rt_Temp <= 95))||((R_Rt_Temp >= 125)&&(R_Rt_Temp <= 137)))
						{	
							RTm_RtLevel_Chat1++;
							if(RTm_RtLevel_Chat1 >= 100)
							{
								RTm_RtLevel_Chat1 = 0;
								O_Chg_Half_En = 1;		//开启半速充电
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
//====放电状态		
			if(F_DisChg_Exist)									
			{																		//放电状态
				if(O_Chg_DisChg_En == 1)//关闭状态
				{																	//放电关断
					if((R_Rt_Temp >= 78) && (R_Rt_Temp <= 137))			
					{																//-5°~60°   +80    75°~140°
						RTm_RtLevel_Chat++;
						if(RTm_RtLevel_Chat >= 100)
						{
							RTm_RtLevel_Chat = 0;
							O_Chg_DisChg_En = 0;				//重新启动放电	
							O_Chg_Half_En	 = 0;						
						}							
					}
					else
					{
						RTm_RtLevel_Chat = 0;
					}
				}
				else//全速和半速 
				{
					if((R_Rt_Temp <= 75) || (R_Rt_Temp >= 140))
					{	
						RTm_RtLevel_Chat++;
						if(RTm_RtLevel_Chat >= 100)
						{
							RTm_RtLevel_Chat = 0;
							O_Chg_DisChg_En = 1;				//关断放电
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
//			R_Rt_Temp = R_Rt_TempBuf;					//温度在1度跳动时5S变化一次
//			RTm_Rt_Chat = LTm_Rt_Chat;
//		}	
	}	
}