#ifndef _InChk_
#define _InChk_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_InChk(void);

void Chk_Chg(void);
void Chk_DisChg(void);
//void Chk_Lock(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------
#define I_Chk_Chg			P07							//充电信号检测口
#define I_Chk_DisChg		P14							//放电信号检测口

#define LTm_Chk_Chg			16							//80ms
#define LTm_Chk_DisChg	16							//80ms
#define LTm_Chk_NDisChg	40							//200ms

//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------
extern bit F_Chg_Exist;									//有充电标志
extern bit F_Chk_Chg;										//充电检测口状态
extern idata uint8_t RTm_Chk_Chg;				//消抖

extern bit F_DisChg_Exist;							//有放电标志
extern bit F_Chk_DisChg;								//放电检测口状态
extern idata uint8_t RTm_Chk_DisChg;		//消抖
extern idata uint8_t RTm_Chk_NDisChg;//消抖
#endif