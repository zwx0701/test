#include <SN8F5702.h>
#include "Init.h"
#include "IntGnd.h"
#include "InChk.h"
#include "App.h"
#include "Data.h"	
#include "AD.h"
#include "Ntc.h"
#include "UART.h"    	
//=============================================================
//	������
//=============================================================
void main(void)
{
	Init();//ϵͳ��ʼ��		
	UART3init();//UART1��ʼ��
	while(1)
	{
		WDTR = 0x5a;						//�幷				
		Mn_IntGnd();						//ϵͳʱ�䴦��		
		Mn_InChk();							//��ƽ���			
		Mn_App();								//Ӧ�ó���	
		Mn_Data();							//ͨѶ���ݴ���
		Mn_AD();								//AD�ɼ�����
		Mn_Ntc();								//�¶ȴ���
	}
}     