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
extern bit F_AD_Updata;										//2ms�ɼ�AD
extern idata uint16_t R_AD_Data;					//�ɼ���AD����

extern bit F_Ain_Num;											//��ǰ�ɼ�ͨ��

extern idata uint16_t R_Vbat_AD;

extern idata uint16_t R_VbatFf[8];				//����ÿ�βɼ���ADֵ�����ڻ���ƽ��
extern idata uint8_t R_VbatFf_Wt;

#endif