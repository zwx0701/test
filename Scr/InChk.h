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
#define I_Chk_Chg			P07							//����źż���
#define I_Chk_DisChg		P14							//�ŵ��źż���

#define LTm_Chk_Chg			16							//80ms
#define LTm_Chk_DisChg	16							//80ms
#define LTm_Chk_NDisChg	40							//200ms

//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------
extern bit F_Chg_Exist;									//�г���־
extern bit F_Chk_Chg;										//������״̬
extern idata uint8_t RTm_Chk_Chg;				//����

extern bit F_DisChg_Exist;							//�зŵ��־
extern bit F_Chk_DisChg;								//�ŵ����״̬
extern idata uint8_t RTm_Chk_DisChg;		//����
extern idata uint8_t RTm_Chk_NDisChg;//����
#endif