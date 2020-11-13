/***************************************************************************************************************
Subroutine:             											�����ӳ���
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
idata uint8_t R_Wk00;													//��ʱ�洢�ȼ�
idata uint8_t R_Rolling_Code[4];							//�洢Rolling Code
idata uint8_t R_Rolling_CodeBuf[4];						//�洢Rolling Code

/****************************************************************************
* Function		: Get_AD
* Description	: �ɼ�ADֵ
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
		EnADC_Coverting;													//��ʼADת��(32us)
		while(!_EOC);															//�ȴ�ADת�����
		ADM &= 0xDF;															//��EOC
		R_AD_fifo = ADB;
		R_AD_fifo = (R_AD_fifo << 4);
		R_AD_fifo = (R_AD_fifo + (ADR & 0x0F));		//��ת����ɵ�12bit����������AD_fifo��
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
	R_AD_Data = (R_AD_Data - R_AD_Min);					//ȥ��ֵ��ƽ��
	R_AD_Data = (R_AD_Data - R_AD_Max);
	R_AD_Data = (R_AD_Data >> 4);
}
/****************************************************************************
* Function		: WTAff
* Description	: �������ݵ�buf
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
* Description	: �󻬶�ƽ��
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
* Description	: �۰뷨���
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
// Description: ��ISP�����ݶ���       
// Input			: None			  				  
// Output			: None
// Return			: None
// Note				: None
//****************************************************************************
void Isp_RdRom(void)
{
	uint8_t R_RomData;
	uint16_t RomAddr;
								
//CBYTE[RomAddr] ��Ч�� ((uint8_t volatile code*)(0))[romaddr]
//=================================
//#define CBYTE ((unsigned char volatile code  *) 0)  ��Ҫ��Sub.h��������������
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
// Description: ��RAM�е�����ISP��ROM��  
// Input			: F_Sys_On��R_Work_Mode��R_Wind_Mode				  
// Output			: None
// Return			: None
// Note				: None
//****************************************************************************
void Isp_WrRom(void)
{
	uint8_t DataBuf[5];
	
  // step 1 : Get data
//	DataBuf[0] = R_Rolling_Code[0];			
//	DataBuf[1] = R_Rolling_Code[1];									//���ٵĵ�λ
//	
//	DataBuf[2] = R_Rolling_Code[2];
//	DataBuf[3] = R_Rolling_Code[3];									//�������õĿ���ʱ��	
	
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
// Description: ����UART����
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

//	R_Uart_TxLen = Tx_Lenth;													//����N byte����
//	
//	RTm_Uart_Reset = 0;
//	
//	IEN0 &= 0xFE;       															//INT0 isr Disable (EX0)

//	TL0 = LTm_Pluse_52us;															//��ʱ52us	
//	TR0 = 1;                													//Enable T0 timer function 
//	
//	O_Uart_Tx = 0;
////	Uart_TxM_OUT;		

//	RTm_Sys_Slp = 0; 																	//������˯��		
//	RTm_Delay_Slp = 0;	
//}
//****************************************************************************
// Function		: Uart_Load_Data
// Description: װ��UART���͵�����
// Input			: None				  
// Output			: None
//****************************************************************************
void Uart_Load_Data(void)
{
idata uint16_t	TempBAT;
//====׼������ 1BYTE�����롱 + 1BYTE��P�� + 1BYTE��04�� + 4BYTE��Rolling Code�� + 2BYTE����ص�ѹ��ADֵ�� + 1BYTE����籦״̬�� + 1BYTE��Check Sum��
	R_Uart_TxBuf[0] = ~R_Uart_RxBuf[0];//����ķ���
	R_Uart_TxBuf[1] = 0x00;//Bit7---bit4=�ͻ����� Bit3---bit0=��籦����														//"P"��ASCII��
	R_Uart_TxBuf[2] = 0X00;//Bit7---bit4=������� Bit3---bit0=��������															//"05"	
	if((R_Rolling_Code[0] != R_Rolling_CodeBuf[0]) || (R_Rolling_Code[1] != R_Rolling_CodeBuf[1])
		|| (R_Rolling_Code[2] != R_Rolling_CodeBuf[2]) || (R_Rolling_Code[3] != R_Rolling_CodeBuf[3]))
	{
		Isp_RdRom();																		// ����Rolling Code	
	}	
	R_Uart_TxBuf[3] = 0;
	R_Uart_TxBuf[4] = 0;
	R_Uart_TxBuf[5] = 0;
	R_Uart_TxBuf[6] = 0;
	R_Uart_TxBuf[3] |= ((R_Rolling_Code[2]>>4)|(R_Rolling_Code[3]<<4));//��λ
	R_Uart_TxBuf[4] |= ((R_Rolling_Code[1]>>4)|(R_Rolling_Code[2]<<4));
	R_Uart_TxBuf[5] |= ((R_Rolling_Code[0]>>4)|(R_Rolling_Code[1]<<4));
	R_Uart_TxBuf[6] |= (R_Rolling_Code[0]<<4); //Bit3---bit0=Ԥ����Ĭ�Ϸ�0000 // Bit7---bit4=Bat_id0Rolling code ��λ						
	R_Uart_TxBuf[7] = 0;//Bit3---bit0=Ԥ����Ĭ�Ϸ�0000 
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

	R_Uart_TxBuf[7] |= (Temperture<<4);//Bit7---bit4=�¶��ϱ�   
	
	R_Uart_TxBuf[8] = 0;
		
	if(F_Chg_Exist==0)R_Uart_TxBuf[8] |= 0X80;//Bit7=0 ���OK   	Bit7=1 �����				
						//Bit6=0 �޲��   	Bit6=1 �в��	
	//if(F_DisChg_Exist==0)R_Uart_TxBuf[8] |= 0X20;//Bit5=0 �ŵ�OK   	Bit5=1 ���ŵ�OK										
	TempBAT = (uint16_t)(R_Vbat_AD*0.732421875);//     R_Vbat_AD/4096*3*1000 ʵ�ʵ�ѹֵ(����)���Զ�
	
	if((BatValue >TempBAT)&&(BatValue - TempBAT)>50)//��0.1Vѹ��
	{
		R_Uart_TxBuf[8] &= 0XDF;//Bit5=0 �ŵ�OK
	}
	else
	{
		R_Uart_TxBuf[8] |= 0X20;//Bit5=1 ���ŵ�OK
	}
		
	R_Uart_TxBuf[8] |= ((TempBAT & 0x0F00) >> 8);//Bit3---bit0=�ϱ���о��ѹ��λ								
	R_Uart_TxBuf[9] = (TempBAT & 0x00FF);//�ϱ���о��ѹ��λ	

}
//-----------------------------------------------------------------
// Function		: Delay_5us
// Description: ��ʱ5us
//-----------------------------------------------------------------
void Delay_5us(void)
{
	uint8_t k;
	for(k = 0; k < 10; k++) 													//�ȴ�5us
	{
		
	}	
}