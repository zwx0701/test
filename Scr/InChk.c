/***************************************************************************************************************
Subroutine:             								输入信号检测
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
bit F_Chg_Exist;												//有充电标志
bit F_Chk_Chg;													//充电检测口状态
idata uint8_t RTm_Chk_Chg;							//消抖

bit F_DisChg_Exist = 1;									//有放电标志
bit F_Chk_DisChg = 1;										//放电检测口状态
idata uint8_t RTm_Chk_DisChg = LTm_Chk_DisChg;//消抖
idata uint8_t RTm_Chk_NDisChg = LTm_Chk_NDisChg;//消抖
//=============================================================
//	电平检测程序
//=============================================================
void Mn_InChk(void)
{																			
	Chk_Chg();														//检测充电信号	
	
  if(F_Chg_Exist == 0)
  {
   	Chk_DisChg();												//检测放电状态	 
  }	
}
/****************************************************************************
* Function		: Chk_Chg
* Description	: 充电信号检测
* Input				: None			  				  
* Output			: None
****************************************************************************/
void Chk_Chg(void)
{		
	if(I_Chk_Chg == 1)										//检测充电信号
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
* Description	: 放电信号检测
* Input				: None			  				  
* Output			: None
* Note				: 开LED时，LED引脚会先输出200ms低电平，MCU正好利用这个电平检测放电状态
****************************************************************************/
void Chk_DisChg(void)
{
	if(I_Chk_DisChg == 0)//检测放电信号
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
				RTm_Sys_Slp = 0; //不进入睡眠	
				RTm_Delay_Slp = 0;	
				
				F_DisChg_Exist = 0;	//放电完成
				if(F_Chg_Exist == 0)
				{
//					O_Chg_DisChg_En = 1;//关断放电	
					
					if((F_UartRx_En == 0) && (F_UartTx_En == 0))	
					{
//						RTm_Delay_Slp = LTm_Delay_Slp; 	//MCU延时10s进入睡眠
						RTm_Sys_Slp = LTm_Sys_Slp; 									//重新开始睡眠定时								
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
			F_DisChg_Exist = 1;	//放电		
			O_Chg_DisChg_En = 0;//开放电	
			
			F_Key_Push = 0;											
			R_Sys_Mode = 0;						
			RTm_Sys_Slp = 0;//不进入睡眠	
			RTm_Delay_Slp = 0;					
		}		
	}

}
