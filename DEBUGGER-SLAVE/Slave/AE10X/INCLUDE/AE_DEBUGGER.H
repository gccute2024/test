/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-14 00:16:36
 * @Description: 
 * 
 * 
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 * 
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */

#ifndef AE_DEBUGGER_H
#define AE_DEBUGGER_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_INCLUDE.H"
#if TEST
#include "HARDWARE_CAN.H"
#endif

//  The Debugger Function Uart Entrance
#define READ_REQUEST 					0x55    //		0x55--read request
#define WRITE_REQUEST 					0xFF    //		0xff--write request
#define GET_DEBUG_TEMP 					0x88	//		0x88--Cancel set to slave so can get Debug_Temp
#define SLAVE_READ_DATA 				0xDD	//		0xDD--Set to slave so can read data
#define KBD_READ_REQUEST 				0xAA	//		0xAA--KBD read request
#define CANCEL_KBD_READ_REQUEST 		0xBB	//		0xBB--cancel KBD read request
#define RESET_CHIP_USE_WATCHDOG 		0xE0	//		0xE0--Reset chip (use watchdog)
#define SWITCH_KEYBOARD_CODE 			0xB0	//		0xB0--Switching keyboard Codes
#define FIRMWARE_UPDATE 				0xA5	//		0xA5--Firmware update
#define INTERRUPT_FUNCTION 				0x11	//		0x11--Interrupt function block
#define RECOARD_KBC_PMC_DATA            0xCC	//      0xCC--Recoard KBC/PMC data 
#define STOP_RECOARD_KBC_PMC_DATA       0xEE    //      0xEE--Stop recording KBC/PMC data


/*****************************************************************************
  *Debugger Macro definition 
 ****************************************************************************/
#define DEBUGGER_SLAVE	              1	//1:use as debugger slave; 0: use as debugger master
#define I2CToUart_Uart                0	//0:output form uart and i2c (when use i2c to connect)   1:only uart (Note that the serial port of the debugger cannot conflict with the serial port of the printed output)
#define DEBUGGER_UART_CHANNEL	      DEBUG_UART_SWITCH//select UART channel as debugger (0/1/2/3/4/5/-1)
////!!!!!!!!!!
//attention! iic_debugger must use the smbus0 
// the "DEBUGGER_I2C_CHANNEL" is just switch the channel mapping instead of initialization
#define DEBUGGER_I2C_CHANNEL	      0//select I2C channel mapping of smbus0
////!!!!!!!!!!
#if ENABLE_DEBUGGER_SUPPORT
/* Debugger Slave or Master channel */
#if (DEBUGGER_UART_CHANNEL == 0)
	#define DEBUGGER_UART0_CHANNEL	1	//debugger uart channel
	#define DEBUGGER_UART	UART0_BASE_ADDR
	#define DEBUGGER_UART_REG	UART0_REG
#elif (DEBUGGER_UART_CHANNEL == 1)
	#define DEBUGGER_UART1_CHANNEL	1
	#define DEBUGGER_UART	UART1_BASE_ADDR
	#define DEBUGGER_UART_REG	UART1_REG
#elif (DEBUGGER_UART_CHANNEL == 2)
	#define DEBUGGER_UART2_CHANNEL	1
	#define DEBUGGER_UART	UART2_BASE_ADDR
	#define DEBUGGER_UART_REG	UART2_REG
#elif (DEBUGGER_UART_CHANNEL == 3)
	#define DEBUGGER_UART3_CHANNEL	1
	#define DEBUGGER_UART	UART3_BASE_ADDR
	#define DEBUGGER_UART_REG	UART3_REG
#elif (DEBUGGER_UART_CHANNEL == 4)
	#define DEBUGGER_UARTA_CHANNEL	1
	#define DEBUGGER_UART	UARTA_BASE_ADDR
	#define DEBUGGER_UART_REG	UARTB_REG
#elif (DEBUGGER_UART_CHANNEL == 5)
	#define DEBUGGER_UARTB_CHANNEL	1
	#define DEBUGGER_UART	UARTA_BASE_ADDR
	#define DEBUGGER_UART_REG	UARTB_REG
#else
	#define DEBUGGER_UART	NULL
	#define DEBUGGER_UART_REG(offset)	0
#endif
#if ENABLE_DEBUGGER_SUPPORT	//Macros that enable the entire debugger function
#if (DEBUG_UART_SWITCH==PRINTF_UART_SWITCH||DEBUG_UART_SWITCH==COMMAND_UART_SWITCH)
#if (DEBUG_UART_SWITCH<6&&DEBUG_UART_SWITCH>=0)
#warning "If you think that the debuger and the output are on the same serial port, it will probably make an error"
#endif
#endif
#endif

static BYTE Service_Debugger_Flag = 0;

#define KBD_BUFF_SIZE 256
struct KBD_Event
{
	BYTE data[KBD_BUFF_SIZE];
	int head;
	int tail;
};

//*****************************************************************************
// extern variable
//*****************************************************************************
extern char Uart_data[];
extern char iic_data[];
extern char buf[];
extern char KBD_Event[];
extern char KBC_PMC_Event[];
extern char Intr_num[];
extern void Debugger_Cmd_Uart(BYTE debuge_data);
extern void Debugger_Cmd_I2c(WORD i2c_channel);
extern void Debugger_I2c_Req(WORD baseaddr);
extern void Debugger_I2c_Send(WORD baseaddr);
extern void Service_Data(void);//Debugger Function read
extern void Service_Change(void);
extern void KBD_ListeningEvent(void);
extern void KBC_PMC_DataPending(char * KBC_PMC_PendingData);
extern BYTE Debugger_KBC_PMC_Record(BYTE direction, BYTE channel, BYTE value);
extern BYTE Debugger_KBD_Record(BYTE value);
extern void Debugger_Send_KBD(void);
extern void Debugger_Master_Retrans(BYTE debuger);
extern void Service_Debugger_Int(void);
extern void DEBUGER_putchar(char ch);

#endif
extern char Intr_num[];
#endif
