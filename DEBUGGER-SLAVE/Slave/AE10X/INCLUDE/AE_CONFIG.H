/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-16 15:34:24
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

#ifndef AE_CONFIG_H
#define AE_CONFIG_H
/****************************************************************************
 * Include Headfile
 *****************************************************************************/
#include "AE_GLOBAL.H"
#include "AE_REG.H"
/****************************************************************************
 *  Functional support
 *****************************************************************************/
#define SUPPORT_LOWPOWER            0
#define SUPPORT_CPUTIMER_WAKEUP     0
#define SUPPORT_PWRSW_WAKEUP        0
#define SUPPORT_KBS_WAKEUP          0


#define SUPPORT_MATRIX_UTILITY      0   //Enabled PMC Matrix Utility Support
//ACPI Function Setting
#define SUPPORT_ACPI_SMB_EC         0       //Enable ACPI SMBus via EC
#define SUPPORT_ACPI_SMI            0       //Use SMI in ACPI Mode(No recommend)
#define SUPPORT_QEvent_Pending      0

#define SUPPORT_I2C_TEMPERATURE		  0//I2C temp get
  
// Enabled KB Interrupt when system Sx
#define ENABLE_S5_KEYBOARD_INTR     0   //Enabled KB Interrupt when system S5
#define ENABLE_S3_KEYBOARD_INTR     1   //Enabled KB Interrupt when system S3
//debuger Function Setting
//EC FIRMWARE UPDATA
#define SUPPORT_FIRMWARE_UPDATE     1
#define SUPPORT_SHAREMEM_PNP        1
//UART

#define SUPPORT_SLAVE_PRINTF        1//0:等待输出；1：轮询输出（缓解输出时长问题）

#define ENABLE_COMMAND_SUPPORT      1 //CMD Enable
#define ENABLE_DEBUGGER_SUPPORT     1 	//Debugger Enable

#define SUPPORT_BREATHING_LED		    0//LED
//port80 &&pnp
#define SUPPORT_LD_PNP              1//1:open logicdevice pnp  0:close pnp
#define SUPPORT_LD_P80              0//1:open logicdevice port80  0:close port80

#define SUPPORT_8042DEBUG_OUTPUT    1    
#define SUIPPORT_MEM_MALLOC         0//0 run mem_malloc 1 not run mem_malloc (if user it,can't user heap)                 
/****************************************************************************
 * Service Enable
 *****************************************************************************/
#define Service_PCI_START 	        1
#define Service_Reserved1_START 	  1
#define Service_Send_START 	        1
#define Service_Send_PS2_START 	    1
#define Service_PCI2_START	        1
#define Service_MS_1_START	        1
#define Service_PCI3_START	        1
#define Service_PCI4_START	        1
#define Service_PCI5_START	        1
#define Service_PCI6_START	        1
#define Service_Reserved2_START     1
#define Service_Reserved3_START     1

#define Service_EVENT_CENTER_START  1
/****************************************************************************
 * Module Enable
 *****************************************************************************/
#define IRQC_MODULE_EN              1
#define I2C_MODULE_EN               0
#define PS2_MODULE_EN               0
#define KBS_MODULE_EN               0
#define SPI_MODULE_EN               0
#define WDT_MODULE_EN               0
#define PWM_MODULE_EN               0
#define TACH_MODULE_EN              0
#define TIMER_MODULE_EN             1
#define PMC_MODULE_EN               0
#define KBC_MODULE_EN               0
#define ADC_MODULE_EN               0
#define UART_MODULE_EN              1
#define CEC_MODULE_EN               0
/****************************************************************************
 * Switch 
 *****************************************************************************/
#define KBD_8_n_SWITCH	            18//n:8*n
#if (KBD_8_n_SWITCH==16)
#define KBD_8_16  		
#elif (KBD_8_n_SWITCH==17)
#define KBD_8_17 	
#elif (KBD_8_n_SWITCH==18)
#define KBD_SN1211 	
#endif
#define LPC_WAY_OPTION_SWITCH           1           //1:polling way   0:interrupt way
#define P80_TO_BRAM_SWITCH              0           //1:使能p80->bram,0:p80->Digital tube
#define KBC_PNP_ONLY_SWITCH	            0           //0:all logicdevice 1:only open keyboard&mouse 

#if defined(AE103)
#define OSC_CLOCK_SWITCH  0   // 1（外部） 0（内部）
#define CHIP_CLOCK_INT  192000000/CHIP_CLOCK_SWITCH   // 1(192) 2(96) 3(64) 4（48）5（38.4）分频数（时钟）
#define CHIP_CLOCK_EXT  96000000/CHIP_CLOCK_SWITCH   // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）
#if (OSC_CLOCK_SWITCH==1)
#define CHIP_CLOCK_SWITCH  4   // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）//2457600
#define LOW_CHIP_CLOCK 32768
#define HIGHT_CHIP_CLOCK CHIP_CLOCK_EXT
#elif (OSC_CLOCK_SWITCH==0)
#define CHIP_CLOCK_SWITCH  8   // 1(192) 2(96) 3(64) 4（48）8（24）分频数（时钟）//2457600
#define LOW_CHIP_CLOCK 32000   // 32768
#define HIGHT_CHIP_CLOCK CHIP_CLOCK_INT
#endif
#else
#define CHIP_CLOCK_SWITCH  4   // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）//2457600
#define LOW_CHIP_CLOCK 32000   // 32768
#define HIGHT_CHIP_CLOCK 96000000/CHIP_CLOCK_SWITCH   // 1(96) 2(48) 3(32) 4（24）5（19.2）分频数（时钟）
#endif
//uart 功能选择 suggestions COMMAND_UART_SWITCH == PRINTF_UART_SWITCH != DEBUG_UART_SWITCH
#if (defined(AE102)||defined(AE103))
#define DEBUG_UART_SWITCH           1
#define PRINTF_UART_SWITCH          0
#define COMMAND_UART_SWITCH         PRINTF_UART_SWITCH//It's aobut receive uart, comand send is printf.
#else
#define DEBUG_UART_SWITCH           4
#define PRINTF_UART_SWITCH          0
#define COMMAND_UART_SWITCH         PRINTF_UART_SWITCH//It's aobut receive uart, comand send is printf.
#endif
#if (COMMAND_UART_SWITCH==0||DEBUG_UART_SWITCH==0||PRINTF_UART_SWITCH==0)
#define UART0_BAUD	                115200
#endif
#if (defined(AE102)||defined(AE103))
#if (COMMAND_UART_SWITCH==1||DEBUG_UART_SWITCH==1||PRINTF_UART_SWITCH==1)
#define UART1_BAUD	                256000
#endif
#if (COMMAND_UART_SWITCH==2||DEBUG_UART_SWITCH==2||PRINTF_UART_SWITCH==2)
#define UART2_BAUD	                57600
#endif
#if (COMMAND_UART_SWITCH==3||DEBUG_UART_SWITCH==3||PRINTF_UART_SWITCH==3)
#define UART3_BAUD	                57600
#endif
#endif
#if (COMMAND_UART_SWITCH==4||DEBUG_UART_SWITCH==4||PRINTF_UART_SWITCH==4)
#define UARTA_BAUD 	                115200
#endif
#if (COMMAND_UART_SWITCH==5||DEBUG_UART_SWITCH==5||PRINTF_UART_SWITCH==5)
#define UARTB_BAUD	                256000
#endif


/****************************************************************************
 * Debug enable
 *****************************************************************************/
#define DEBUG                   1 //0:close printf   1:open printf

#define KB_MS_DEBUG             0&DEBUG//0:close printf   1:open printf
#define I2C_DEBUG	            0&DEBUG//0:close printf   1:open printf
#define IRQC_DEBUG	            1&DEBUG//0:close printf   1:open printf
/****************************************************************************
 * Test enable
 *****************************************************************************/

//TEST为1是TEST_MAIN.c测试代码 0为KERNEL_MAIN.c下的主体代码
#define TEST                    0
#define LogicDevice_Test        0&TEST              //1:open logicdevice pnp  0:close pnp (wait del)
#define SWUC_TEST               0&TEST
#define SPI_TEST                0&TEST
#define SMx_TEST                0//&TEST

/* ACPI EC SCI Interrupt Response Timing ( 8-16uSec).*/

#endif /* AE_CONFIG_H */

