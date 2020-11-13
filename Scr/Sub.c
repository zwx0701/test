/***************************************************************************************************************
Subroutine:             											各种子程序
Description:  																	                          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "Sub.h"	
#include "App.h"
#include "InChk.h"
#include "Data.h"	
#include "AD.h"
#include "Ntc.h"
//-------------------------------------------------------------
//	Sub ram define
//-------------------------------------------------------------
idata uint8_t R_Wk00;													//临时存储等级
idata uint8_t R_Rolling_Code[4];							//存储Rolling Code
idata uint8_t R_Rolling_CodeBuf[4];						//存储Rolling Code

/****************************************************************************
* Function		: Get_AD
* Description	: 采集AD值
* Input				: None			  				  
* Output			: R_AD_Data
* Return			: None
* Note				: None
****************************************************************************/
void Get_AD(void)
{ 	
	idata uint8_t i;
	idata uint16_t R_AD_Max = 0;
	idata uint16_t R_AD_Min = 0xFFFF;
	idata uint16_t R_AD_fifo;
	R_AD_Data = 0;
	for(i = 0; i < 18; i++)				
	{
		EnADC_Coverting;													//开始AD转换(32us)
		while(!_EOC);															//等待AD转换完成
		ADM &= 0xDF;															//清EOC
		R_AD_fifo = ADB;
		R_AD_fifo = (R_AD_fifo << 4);
		R_AD_fifo = (R_AD_fifo + (ADR & 0x0F));		//将转换完成的12bit数字量存入AD_fifo中
		R_AD_Data = (R_AD_Data + R_AD_fifo);
		if(R_AD_fifo < R_AD_Min)
		{
			R_AD_Min = R_AD_fifo; 
		}
		if(R_AD_fifo > R_AD_Max)
		{
			R_AD_Max = R_AD_fifo;
		}
	}
	R_AD_Data = (R_AD_Data - R_AD_Min);					//去极值求平均
	R_AD_Data = (R_AD_Data - R_AD_Max);
	R_AD_Data = (R_AD_Data >> 4);
}
/****************************************************************************
* Function		: WTAff
* Description	: 保存数据到buf
* Input				: None			  				  
* Output			: None
* Return			: None
* Note				: None
****************************************************************************/
void WTAff(uint16_t *R_Ff,uint8_t *R_Ff_Wt,uint16_t R_Data)
{ 	
	*R_Ff = R_Data;	

	(*R_Ff_Wt)++;
	if(*R_Ff_Wt >= L_Array_Length)
	{
		*R_Ff_Wt = 0;
	}
}
/****************************************************************************
* Function		: AVERAGE
* Description	: 求滑动平均
* Input				: None			  				  
* Output			: None
* Return			: None
* Note				: None
****************************************************************************/
uint16_t AVERAGE(uint16_t *R_Ff)
{ 	
	uint8_t i;
	uint16_t R_Data = 0;	
	
	for(i = 0; i < L_Array_Length; i++)				
	{
		R_Data += *R_Ff;	
		R_Ff++;				
	}
	R_Data = (R_Data >> 3);
	
	return(R_Data);
}
/****************************************************************************
* Function		: BinChk_Rank
* Description	: 折半法查表
* Input				: None			  				  
* Output			: None
* Return			: R_Wk00
* Note				: None
****************************************************************************/
uint8_t BinChk_Rank(uint16_t code *Array,uint8_t Length)
{ 	
	bit F_Chk_Rank = 1;
	uint8_t Low = 0,Mid,High = Length;
	
	while(F_Chk_Rank)
	{
		Mid = ((Low + High) / 2);
		if(R_AD_Data < *(Array + Mid))
		{			
			if((R_AD_Data >= *(Array + Mid - 1)) || (Mid == 0))
			{
				F_Chk_Rank = 0;											
			}
			else
			{
				High = Mid;	
			}
		}
		else
		{
			if(Mid >= (Length - 1))
			{
				Mid++;
				F_Chk_Rank = 0;				
			}
			else
			{
				Low = Mid;				
			}
		}
	}
	return(Mid);
}
//****************************************************************************
// Function		: Isp_RdRom
// Description: 将ISP的数据读出       
// Input			: None			  				  
// Output			: None
// Return			: None
// Note				: None
//****************************************************************************
void Isp_RdRom(void)
{
	uint8_t R_RomData;
	uint16_t RomAddr;
								
//CBYTE[RomAddr] 等效于 ((uint8_t volatile code*)(0))[romaddr]
//=================================
//#define CBYTE ((unsigned char volatile code  *) 0)  需要在Sub.h里面加上这条语句
//=================================	

	if((R_Rolling_Code[0] == R_Rolling_CodeBuf[0]) && (R_Rolling_Code[1] == R_Rolling_CodeBuf[1])
		&& (R_Rolling_Code[2] == R_Rolling_CodeBuf[2]) && (R_Rolling_Code[3] == R_Rolling_CodeBuf[3]))
	{
		RomAddr = L_IspAddr1;
		R_RomData = CBYTE[RomAddr]; 	
		if(R_RomData == 0x55)
		{
			O_Output_En = 1;
//			F_Output_En = 1;
		}
		else if(R_RomData == 0xAA)
		{
			O_Output_En = 0;
//			F_Output_En = 1;
		}
	}	
	
//	RomAddr++;	
	RomAddr = L_IspAddr;
	R_RomData = CBYTE[RomAddr]; 	
	R_Rolling_Code[0] = R_RomData;								
	
	RomAddr++;		
	R_RomData = CBYTE[RomAddr]; 	
	R_Rolling_Code[1] = R_RomData;	
	
	RomAddr++;			
	R_RomData = CBYTE[RomAddr]; 	
	R_Rolling_Code[2] = R_RomData;	
	
	RomAddr++;		
	R_RomData = CBYTE[RomAddr]; 	
	R_Rolling_Code[3] = R_RomData;
	
	R_Rolling_CodeBuf[0] = R_Rolling_Code[0];
	R_Rolling_CodeBuf[1] = R_Rolling_Code[1];
	R_Rolling_CodeBuf[2] = R_Rolling_Code[2];
	R_Rolling_CodeBuf[3] = R_Rolling_Code[3];
}
//****************************************************************************
// Function		: Isp_WrRom
// Description: 将RAM中的数据ISP到ROM中  
// Input			: F_Sys_On、R_Work_Mode、R_Wind_Mode				  
// Output			: None
// Return			: None
// Note				: None
//****************************************************************************
void Isp_WrRom(void)
{
	uint8_t DataBuf[5];
	
  // step 1 : Get data
//	DataBuf[0] = R_Rolling_Code[0];			
//	DataBuf[1] = R_Rolling_Code[1];									//风速的档位
//	
//	DataBuf[2] = R_Rolling_Code[2];
//	DataBuf[3] = R_Rolling_Code[3];									//保存设置的开机时间	
	
	if(O_Output_En == 1)
	{
		DataBuf[0] = 0x55;
	}
	else
	{
		DataBuf[0] = 0xAA;
	}
	
	// step 2 : Set RAM addr of data
	PERAM = DataBuf;

	// step 3 : Set ROM start addr (Range is 0x0000~0x03FF)
	PEROML &= 0x0F;
	PEROML |= (L_IspAddr1 & 0x00F0);
	PEROMH = (L_IspAddr1 >> 8);	
	
	// step 4 : Progarm one page (32 bytes)
	PEROML |= 0x0A;	
	PECMD = 0x5A;
}
//****************************************************************************
// Function		: Uart_Tx_Start
// Description: 启动UART发送
// Input			: None				  
// Output			: None
//****************************************************************************
//void Uart_Tx_Start(uint8_t Tx_Lenth)
//{
//	F_UartTx_En = 1;
//	F_UartTx_Start = 1;
//	F_UartTx_Stop = 0;
//	F_UartTx_Over = 0;
//	R_UartTx_Bit = 8;
//	R_UartTx_Byte = 0;
//	R_UartTx_Data = R_Uart_TxBuf[0];

//	R_Uart_TxLen = Tx_Lenth;													//发送N byte数据
//	
//	RTm_Uart_Reset = 0;
//	
//	IEN0 &= 0xFE;       															//INT0 isr Disable (EX0)

//	TL0 = LTm_Pluse_52us;															//定时52us	
//	TR0 = 1;                													//Enable T0 timer function 
//	
//	O_Uart_Tx = 0;
////	Uart_TxM_OUT;		

//	RTm_Sys_Slp = 0; 																	//不进入睡眠		
//	RTm_Delay_Slp = 0;	
//}
//****************************************************************************
// Function		: Uart_Load_Data
// Description: 装载UART发送的数据
// Input			: None				  
// Output			: None
//****************************************************************************
void Uart_Load_Data(void)
{
idata uint16_t	TempBAT;
//====准备数据 1BYTE“导码” + 1BYTE“P” + 1BYTE“04” + 4BYTE“Rolling Code” + 2BYTE“电池电压的AD值” + 1BYTE“充电宝状态” + 1BYTE“Check Sum”
	R_Uart_TxBuf[0] = ~R_Uart_RxBuf[0];//命令的反码
	R_Uart_TxBuf[1] = 0x00;//Bit7---bit4=客户代码 Bit3---bit0=充电宝代数														//"P"的ASCII码
	R_Uart_TxBuf[2] = 0X00;//Bit7---bit4=工厂序号 Bit3---bit0=方案代号															//"05"	
	if((R_Rolling_Code[0] != R_Rolling_CodeBuf[0]) || (R_Rolling_Code[1] != R_Rolling_CodeBuf[1])
		|| (R_Rolling_Code[2] != R_Rolling_CodeBuf[2]) || (R_Rolling_Code[3] != R_Rolling_CodeBuf[3]))
	{
		Isp_RdRom();																		// 读出Rolling Code	
	}	
	R_Uart_TxBuf[3] = 0;
	R_Uart_TxBuf[4] = 0;
	R_Uart_TxBuf[5] = 0;
	R_Uart_TxBuf[6] = 0;
	R_Uart_TxBuf[3] |= ((R_Rolling_Code[2]>>4)|(R_Rolling_Code[3]<<4));//高位
	R_Uart_TxBuf[4] |= ((R_Rolling_Code[1]>>4)|(R_Rolling_Code[2]<<4));
	R_Uart_TxBuf[5] |= ((R_Rolling_Code[0]>>4)|(R_Rolling_Code[1]<<4));
	R_Uart_TxBuf[6] |= (R_Rolling_Code[0]<<4); //Bit3---bit0=预留，默认发0000 // Bit7---bit4=Bat_id0Rolling code 低位						
	R_Uart_TxBuf[7] = 0;//Bit3---bit0=预留，默认发0000 
	if(R_Rt_Temp<=75)//+80
		Temperture = 0;
	else if((R_Rt_Temp>75)&&(R_Rt_Temp<=85))
		Temperture = 1;
	else if((R_Rt_Temp>85)&&(R_Rt_Temp<=95))
		Temperture = 2;
	else if((R_Rt_Temp>95)&&(R_Rt_Temp<=125))
		Temperture = 3;	
	else if((R_Rt_Temp>125)&&(R_Rt_Temp<140))
		Temperture = 4;	
	else if(R_Rt_Temp>=140)
		Temperture = 5;	
	if((R_Ntc_AD>4000)||(R_Ntc_AD<100))
		Temperture = 3;	

	R_Uart_TxBuf[7] |= (Temperture<<4);//Bit7---bit4=温度上报   
	
	R_Uart_TxBuf[8] = 0;
		
	if(F_Chg_Exist==0)R_Uart_TxBuf[8] |= 0X80;//Bit7=0 充电OK   	Bit7=1 不充电				
						//Bit6=0 无拆机   	Bit6=1 有拆机	
	//if(F_DisChg_Exist==0)R_Uart_TxBuf[8] |= 0X20;//Bit5=0 放电OK   	Bit5=1 不放电OK										
	TempBAT = (uint16_t)(R_Vbat_AD*0.732421875);//     R_Vbat_AD/4096*3*1000 实际电压值(毫伏)除以二
	
	if((BatValue >TempBAT)&&(BatValue - TempBAT)>50)//有0.1V压降
	{
		R_Uart_TxBuf[8] &= 0XDF;//Bit5=0 放电OK
	}
	else
	{
		R_Uart_TxBuf[8] |= 0X20;//Bit5=1 不放电OK
	}
		
	R_Uart_TxBuf[8] |= ((TempBAT & 0x0F00) >> 8);//Bit3---bit0=上报电芯电压高位								
	R_Uart_TxBuf[9] = (TempBAT & 0x00FF);//上报电芯电压低位	

}
//-----------------------------------------------------------------
// Function		: Delay_5us
// Description: 延时5us
//-----------------------------------------------------------------
void Delay_5us(void)
{
	uint8_t k;
	for(k = 0; k < 10; k++) 													//等待5us
	{
		
	}	
}