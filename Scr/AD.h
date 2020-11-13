#ifndef _AD_
#define _AD_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_AD(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------


//-------------------------------------------------------------
//	AD ram define
//-------------------------------------------------------------
extern bit F_AD_Updata;										//2ms采集AD
extern idata uint16_t R_AD_Data;					//采集的AD数据

extern bit F_Ain_Num;											//当前采集通道

extern idata uint16_t R_Vbat_AD;

extern idata uint16_t R_VbatFf[8];				//保存每次采集的AD值，用于滑动平均
extern idata uint8_t R_VbatFf_Wt;

#endif