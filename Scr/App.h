#ifndef _App_
#define _App_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_App(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------
#define LTm_Sys_Slp					250						//1250ms���¼���������˯��
#define LTm_Delay_Slp				2000					//MCU��ʱ10S����˯��

#define LTm_KEY						41					//200MS	�̰���	

#define O_Output_En					P02						//���ʹ�ܿ�

#define O_Chg_Half_En				P01						//���ٳ�����
#define O_Chg_DisChg_En			P12						//��ŵ�ضϿ�
#define I_SW1								P00
#define O_KEY							P10
//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------
extern bit F_Key_Push;
extern idata uint8_t R_Sys_Mode;					//ϵͳ����ģʽ

extern idata uint8_t RTm_Sys_Slp;					//ϵͳ����˯�߶�ʱ
extern idata uint16_t RTm_Delay_Slp;			//MCU��ʱ����˯��

extern bit F_Charge_Ok;										//���OK��־
extern idata uint8_t RTm_Key;
extern bit F_Min;
//extern bit F_Output_En;										//������ƿڵ�״̬
uint8_t ReadIO(void);
//void ShortKey(void);
void MINKey(void);
void MINKey_Intgnd(void);
void CloseKey(void);
#endif