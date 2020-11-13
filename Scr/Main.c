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
//	主程序
//=============================================================
void main(void)
{
	Init();//系统初始化		
	UART3init();//UART1初始化
	while(1)
	{
		WDTR = 0x5a;						//清狗				
		Mn_IntGnd();						//系统时间处理		
		Mn_InChk();							//电平检测			
		Mn_App();								//应用程序	
		Mn_Data();							//通讯数据处理
		Mn_AD();								//AD采集程序
		Mn_Ntc();								//温度处理
	}
}     