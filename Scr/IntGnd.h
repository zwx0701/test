#ifndef _IntGnd_
#define _IntGnd_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_IntGnd(void);

void Tm_Sys_Slp(void);
void Tm_Delay_Slp(void);
void Tm_Chk_Chg(void);
void Tm_Chk_DisChg(void);
void Tm_Rt_Chat(void);
	
//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------


//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------
extern bit F_5Ms;											//5ms时间溢出标志																

#endif