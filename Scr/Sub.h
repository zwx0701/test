#ifndef _Sub_
#define _Sub_

//-------------------------------------------------------------
//	function declare
//-------------------------------------------------------------
void Get_AD(void);
void WTAff(uint16_t *R_Ff,uint8_t *R_Ff_Wt,uint16_t R_Data);
uint16_t AVERAGE(uint16_t *R_Ff);
uint8_t BinChk_Rank(uint16_t code *Array,uint8_t Length);

void Isp_RdRom(void);
void Isp_WrRom(void);

//void Uart_Tx_Start(uint8_t Tx_Lenth);
void Uart_Load_Data(void);

void Delay_5us(void);

//-------------------------------------------------------------
//	constant declare
//-------------------------------------------------------------		
#define L_IspAddr1 0x0FA0									//isp addr 0x0FA0～0x0FBF
#define L_IspAddr 0x0FC0									//isp addr 0x0FC0～0x0FDF
#define CBYTE ((unsigned char volatile code  *) 0)
	
#define 	_EOC						(ADM & 0x20)		// ADC status (EOC)
#define 	EnADC_Coverting	(ADM |= 0x40) 	// ADC converting start (ADS)
#define 	L_Array_Length	8								//存储数据的数组长度		

//-------------------------------------------------------------
//	extern ram declare
//-------------------------------------------------------------
extern idata uint8_t R_Wk00;							//临时存储等级

extern idata uint8_t R_Rolling_Code[4];		//存储Rolling Code
extern idata uint8_t R_Rolling_CodeBuf[4];//存储Rolling Code

#endif