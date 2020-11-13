#ifndef _Data_
#define _Data_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Mn_Data(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------
#define O_Uart_Tx				P05
#define I_Uart_Rx				P06

#define Uart_TxM_OUT		P0M = (P0M | 0x80)
#define Uart_TxM_IN			P0M = (P0M & 0x7F)

#define LTm_Pluse				204
#define LTm_Pluse_52us	210
#define LTm_Start				193

#define L_Uart_RxBuf		2												//UART������1Byte����
#define L_Uart_TxBuf		11											//UART��෢��12Byte����
#define L1_Uart_TxBuf		11											//UART��෢��11Byte����

#define LTm_Uart_Reset	60											//UART����30ms��λ

//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------			
extern bit F_UartRx_En;										//UART���տ�ʼ

extern idata uint8_t R_UartRx_Byte;							//ͳ�ƽ��յ�BYTE��
extern idata uint8_t R_Uart_RxBuf[L_Uart_RxBuf];//UART����BUF

extern bit F_UartTx_En;													//UART���Ϳ�ʼ
extern idata uint8_t R_UartTx_Byte;							//ͳ�Ʒ��͵�BYTE��
extern idata uint8_t R_Uart_TxBuf[L_Uart_TxBuf];//UART����BUF

extern idata uint8_t  Temperture;//�¶ȴ���
extern idata uint16_t  BatValue;
#endif