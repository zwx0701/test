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
extern bit F_Ntc_Updata;									//�ɼ����NTC��ADֵ
extern idata uint16_t R_Ntc_AD;						//����ÿ�βɼ�����ADֵ

extern idata uint16_t R_NtcFf[8];					//����ÿ�βɼ���ADֵ�����ڻ���ƽ��
extern idata uint8_t R_NtcFf_Wt;

extern idata uint8_t R_Ntc_Debouce;				//���ȼ�����

extern idata uint8_t R_Rt_Temp;						//���Ե��¶���ֵ

extern bit F_Ntc_Error;										//�¶��쳣��־

#endif