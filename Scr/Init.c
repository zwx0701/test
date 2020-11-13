/******************************************************************************
Subroutine:             			初始化程序
Description:                	对IO口、功能模块和寄存器初始化处理
******************************************************************************/
#include <SN8F5702.h>	
#include "Init.h"
#include "Data.h"	
#include "Sub.h"

//-------------------------------------------------------------
//	Init ram define
//-------------------------------------------------------------
//====NONE

//=============================================================
//	初始化程序
//=============================================================
void Init(void)
{	
//-----------------------------------
//	clock初始化
//-----------------------------------
//	CKCON = 0x31;								//From = 32M/4
//	CLKSEL = 0x07;							//Fcpu = 32M/1
//	CLKCMD = 0x69;							//Clock switch start
	CKCON = 0X70;	
	CLKSEL = 0X07;//Fcpu = 32M/1 = 32M
	CLKCMD = 0X69;	
	CKCON = 0X30;//From = 8M		
//-----------------------------------
//	IO口初始化
//-----------------------------------
	P0UR = 0X59;								//P00为震动开关检测口 P02为输出使能口 
	P0 = 0X00;									//P05为UART TX P06为UART RX
	P0M = 0X26;									//P07为充电检测口 P01充电电流控制口1
	
	P1UR = 0XE2;								//P10为打开5189T的开关 P13为AD口，检测温度
	P1 = 0X01;									//P14为放电检测口 P12充电电流控制口2
	P1M = 0X05;			
	P1W = 0x10;									//使能P14的唤醒功能	
	
	P2UR = 0XFE;
	P2 = 0X00;
	P2M =0X00;
//-----------------------------------
//	T2初始化 --------> 5ms
//-----------------------------------		
	T2CON = 0x91;								//Fclk = Fcpu/24
												//AutoReload	
	TH2 = 0xE5;
	TL2 = 0xF5;									//计数6667次

	CRCH = 0xE5;
	CRCL = 0xF5;								//指定重装载值

	ET2 = 1;                		//Timer2 interrupt enable
//-----------------------------------
//	AD初始化
//-----------------------------------
	ADR |= 0x40;								// Enable AIN channel (GCHS)

	VREFH |= 0x00;							// VHS[2] = 0 :  ADC internal VREFH function is depend on VHS[1:0]
															// VHS[1:0] = 00b : reference voltage is 2V.  
															
															// ADC internal reference high voltage select bits. 
															//==============================================
															//	VHS1 |	VHS0	|		Internal AVREFH Voltage
															//==============================================
															//		0				0						2.0V (default)
															//		0				1						3.0V
															//		1				0						4.0V
															//		1				1						VDD
															//==============================================
	P1CON = 0x08; 							// P13 to be pure analog input pin
	P2CON = 0x01; 							// P20 to be pure analog input pin
	
	ADM = 0x83;									// ADC enable (ADENB AIN3)
//-----------------------------------
//	读出Rolling Code(4BYTE  start address is 0x0FC0)
//-----------------------------------
	Isp_RdRom();								// 读出Rolling Code	
//-----------------------------------
//	开总中断
//-----------------------------------
  EAL = 1;               			//Golbal Interrupt enable		
}