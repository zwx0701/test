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

#define L_Uart_RxBuf		2												//UART最多接收1Byte数据
#define L_Uart_TxBuf		11											//UART最多发送12Byte数据
#define L1_Uart_TxBuf		11											//UART最多发送11Byte数据

#define LTm_Uart_Reset	60											//UART接收30ms复位

//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------			
extern bit F_UartRx_En;										//UART接收开始

extern idata uint8_t R_UartRx_Byte;							//统计接收的BYTE数
extern idata uint8_t R_Uart_RxBuf[L_Uart_RxBuf];//UART接收BUF

extern bit F_UartTx_En;													//UART发送开始
extern idata uint8_t R_UartTx_Byte;							//统计发送的BYTE数
extern idata uint8_t R_Uart_TxBuf[L_Uart_TxBuf];//UART发送BUF

extern idata uint8_t  Temperture;//温度代号
extern idata uint16_t  BatValue;
#endif