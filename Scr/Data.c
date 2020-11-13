/***************************************************************************************************************
Subroutine:             												ͨѶ���ݴ���
Description:  									                          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "Data.h"		
#include "App.h"
#include "InChk.h"
#include "AD.h"
#include "Ntc.h"
#include "Sub.h"	
#include "UART.h"    
//-------------------------------------------------------------
//	Data ram define
//-------------------------------------------------------------
bit F_UartRx_En;																//UART���տ�ʼ
idata uint8_t R_UartRx_Byte;										//ͳ�ƽ��յ�BYTE��
idata uint8_t R_Uart_RxBuf[L_Uart_RxBuf];				//UART����BUF

bit F_UartTx_En;																//UART���Ϳ�ʼ
idata uint8_t R_UartTx_Byte;										//ͳ�Ʒ��͵�BYTE��
idata uint8_t R_Uart_TxBuf[L_Uart_TxBuf];				//UART����BUF

idata uint8_t  Temperture;//�¶ȴ���
idata uint16_t  BatValue;
//=============================================================
//	��ƽ������
//=============================================================
void Mn_Data(void)
{	
	if(cRxTick ==20)//��������OK
	{
		F_UartRx_En = 0;	
		if(R_UartRx_Byte >0)	
		{					
			if((R_Uart_RxBuf[0]  == 0X5E) && (R_Uart_RxBuf[1] == 0XA1))//��ѯ��籦״̬
			{
				P05 = 1;
				Uart_Load_Data();													//����UART_TXBUF����
				
				if(O_Output_En == 1)//Bit4=0 ������ر� Bit4=1 �������
				{
					R_Uart_TxBuf[8] |= 0x10;//�������						
				}
	//			else
	//			{
	//				R_Uart_TxBuf[8] &= 0XEF;//������ر�					
	//			}
				R_Uart_TxBuf[10] = (R_Uart_TxBuf[0] + R_Uart_TxBuf[1] + R_Uart_TxBuf[2] + R_Uart_TxBuf[3] + R_Uart_TxBuf[4] +
													 R_Uart_TxBuf[5] + R_Uart_TxBuf[6] + R_Uart_TxBuf[7] + R_Uart_TxBuf[8] + R_Uart_TxBuf[9]);			
				RTm_Sys_Slp = 0;//������˯��		
				RTm_Delay_Slp = 0;	
				UART_SendNByte();//Uart_Tx_Start(L1_Uart_TxBuf);							//����UART����						
			}
			else if((R_Uart_RxBuf[0] == 0x7C) && (R_Uart_RxBuf[1] == 0X83))//�ر������
			{	
				P05 = 1;
				Uart_Load_Data();												//����UART_TXBUF����		
				//R_Uart_TxBuf[8] &= 0XEF;//������ر�  //Bit4=0 ������ر� Bit4=1 �������				
				R_Uart_TxBuf[10] = (R_Uart_TxBuf[0] + R_Uart_TxBuf[1] + R_Uart_TxBuf[2] + R_Uart_TxBuf[3] + R_Uart_TxBuf[4] +
													 R_Uart_TxBuf[5] + R_Uart_TxBuf[6] + R_Uart_TxBuf[7] + R_Uart_TxBuf[8] + R_Uart_TxBuf[9]);
				if(O_Output_En == 1)
				{
					O_Output_En = 0;											//�����
					
					Isp_WrRom();													//дRom
				}
				RTm_Sys_Slp = 0;//������˯��		
				RTm_Delay_Slp = 0;					
				UART_SendNByte();//Uart_Tx_Start(L_Uart_TxBuf);						//����UART����	
				CloseKey();
			}
			else if((R_Uart_RxBuf[0] == 0X5C) && (R_Uart_RxBuf[1] == 0XA3))//�������
			{	
				P05 = 1;
				BatValue = (uint16_t)(R_Vbat_AD*0.732421875);//     R_Vbat_AD/4096*3*1000
				Uart_Load_Data();												//����UART_TXBUF����			
				R_Uart_TxBuf[8] |= 0x10;//������� //Bit4=0 ������ر� Bit4=1 �������							
				R_Uart_TxBuf[10] = (R_Uart_TxBuf[0] + R_Uart_TxBuf[1] + R_Uart_TxBuf[2] + R_Uart_TxBuf[3] + R_Uart_TxBuf[4] +
													 R_Uart_TxBuf[5] + R_Uart_TxBuf[6] + R_Uart_TxBuf[7] + R_Uart_TxBuf[8] + R_Uart_TxBuf[9]);			
				if(O_Output_En == 0)
				{
					O_Output_En = 1;											//�����
					
					Isp_WrRom();													//дRom
				}
				RTm_Sys_Slp = 0;//������˯��		
				RTm_Delay_Slp = 0;					
				UART_SendNByte();//Uart_Tx_Start(L_Uart_TxBuf);						//����UART����
				MINKey();//����IP5189				
			}		
			else
			{
				//Uart_TxM_IN;				
				if((F_Chg_Exist == 0) && (F_DisChg_Exist == 0))
				{
					RTm_Sys_Slp = LTm_Sys_Slp; 							//��ʼ˯�߶�ʱ	
				}			
			}
		}
		R_UartRx_Byte = 0;			
	}
}