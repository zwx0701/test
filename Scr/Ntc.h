#ifndef _Ntc_
#define _Ntc_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_Ntc(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------


//-------------------------------------------------------------
//	AD ram declare
//-------------------------------------------------------------
extern bit F_Ntc_Updata;									//采集完成NTC的AD值
extern idata uint16_t R_Ntc_AD;						//保存每次采集到的AD值

extern idata uint16_t R_NtcFf[8];					//保存每次采集的AD值，用于滑动平均
extern idata uint8_t R_NtcFf_Wt;

extern idata uint8_t R_Ntc_Debouce;				//检测等级消抖

extern idata uint8_t R_Rt_Temp;						//测试的温度数值

extern bit F_Ntc_Error;										//温度异常标志

#endif