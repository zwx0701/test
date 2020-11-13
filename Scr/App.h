#ifndef _App_
#define _App_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_App(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------
#define LTm_Sys_Slp					250						//1250ms无事件发生进入睡眠
#define LTm_Delay_Slp				2000					//MCU延时10S进入睡眠

#define LTm_KEY						41					//200MS	短按键	

#define O_Output_En					P02						//输出使能口

#define O_Chg_Half_En				P01						//半速充电控制
#define O_Chg_DisChg_En			P12						//充放电关断口
#define I_SW1								P00
#define O_KEY							P10
//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------
extern bit F_Key_Push;
extern idata uint8_t R_Sys_Mode;					//系统工作模式

extern idata uint8_t RTm_Sys_Slp;					//系统进入睡眠定时
extern idata uint16_t RTm_Delay_Slp;			//MCU延时进入睡眠

extern bit F_Charge_Ok;										//充电OK标志
extern idata uint8_t RTm_Key;
extern bit F_Min;
//extern bit F_Output_En;										//输出控制口的状态
uint8_t ReadIO(void);
//void ShortKey(void);
void MINKey(void);
void MINKey_Intgnd(void);
void CloseKey(void);
#endif