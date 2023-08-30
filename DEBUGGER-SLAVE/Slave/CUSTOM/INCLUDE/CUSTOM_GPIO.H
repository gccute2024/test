/*
 * @Author: Iversu
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-21 16:16:32
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

#ifndef CUSTOM_GPIO_H
#define CUSTOM_GPIO_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// GPIO A[7:0] Registers define
//------------------------------------------------------------------------------
#define PinA0_7_Init      0x00            // GPIO A[7:0] data register OUTPUT init
#define PinA0_7_InOut     0x2B            // GPIO A[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
//#define PinA0_7_InOut     0x00          // GPIO A[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA0_7_IoMux     0x0010          // GPIO A[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinA0_7_PullUp    0x00            // GPIO A[7:0] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
// GPIO A[15:8] Registers define
//------------------------------------------------------------------------------
#define PinA8_15_Init      0x00            // GPIO A[15:8] data register OUTPUT init
#define PinA8_15_InOut     0xC4            // GPIO A[15:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA8_15_IoMux     0x014A          // GPIO A[15:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinA8_15_PullUp    0x18            // GPIO A[15:8] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO A[23:16] Registers define
//------------------------------------------------------------------------------
#define PinA16_23_Init      0x00            // GPIO A[23:16] data register OUTPUT init
#define PinA16_23_InOut     0x10            // GPIO A[23:16] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA16_23_IoMux     0x4014          // GPIO A[23:16] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinA16_23_PullUp    0x40            // GPIO A[23:16] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO A[31:24] Registers define
//------------------------------------------------------------------------------
#define PinA24_31_Init      0x00            // GPIO A[31:24] data register OUTPUT init
#define PinA24_31_InOut     0x3B            // GPIO A[31:24] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinA24_31_IoMux     0x1010          // GPIO A[31:24] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinA24_31_PullUp	   0x20         // GPIO A[31:24] data direction register init.0 is INPUT,1 is OUTPUT.
//------------------------------------------------------------------------------
// GPIO B[7:0] Registers define
//------------------------------------------------------------------------------
#define PinB0_7_Init      0x00            // GPIO B[7:0] data register OUTPUT init
#define PinB0_7_InOut     0xCE            // GPIO B[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB0_7_IoMux     0x0000          // GPIO B[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinB0_7_PullUp    0x00            // GPIO B[7:0] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO B[15:8] Registers define
//------------------------------------------------------------------------------
#define PinB8_15_Init      0x10            // GPIO B[15:8] data register OUTPUT init
#define PinB8_15_InOut     0x10            // GPIO B[15:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB8_15_IoMux     0x5005          // GPIO B[15:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinB8_15_PullUp    0x0             // GPIO B[15:8] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO B[23:16] Registers define
//------------------------------------------------------------------------------
#define PinB16_23_Init    0x00          // GPIO B[23:16] data register OUTPUT init
#define PinB16_23_InOut   0x04          // GPIO B[23:16] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB16_23_IoMux   0x0100        // GPIO B[23:16] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinB16_23_PullUp        0x00    // GPIO B[23:16] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO B[26:24] Registers define
//------------------------------------------------------------------------------
#define PinB24_26_Init      0x03       // GPIO B[26:24] data register OUTPUT init
#define PinB24_26_InOut     0x03        // GPIO B[26:24] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinB24_26_IoMux     0x0         // GPIO B[26:24] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinB24_26_PullUp    0x00        // GPIO B[26:24] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO C[7:0] Registers define
//------------------------------------------------------------------------------
#define PinC0_7_Init      0x00            // GPIO C[7:0] data register OUTPUT init
#define PinC0_7_InOut     0x23            // GPIO C[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinC0_7_IoMux     0x0000          // GPIO C[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinC0_7_PullUp    0x00            // GPIO C[7:0] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO C[13:8] Registers define
//------------------------------------------------------------------------------
#define PinC8_13_Init      0x11        // GPIO C[13:8] data register OUTPUT init
#define PinC8_13_InOut     0x17        // GPIO C[13:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinC8_13_IoMux     0x0000      // GPIO C[13:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinC8_13_PullUp    0x00        // GPIO C[13:8] IO Pull Up control register init.0 is not PULLUP,1 is PULLUP.
//------------------------------------------------------------------------------
// GPIO D[7:0] Registers define
//------------------------------------------------------------------------------
#define PinD0_6_Init      0x00        // GPIO D[7:0] data register OUTPUT init
#define PinD0_6_InOut     0x00        // GPIO D[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinD0_6_IoMux     0x1333      // GPIO D[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinD0_6_PullUp    0x0          // GPIO D[7:0] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."

//------------------------------------------------------------------------------
// GPIO E[7:0] Registers define
//------------------------------------------------------------------------------
#define PinE0_7_Init      0x00         // GPIO E[7:0] data register OUTPUT init
#define PinE0_7_InOut     0x00         // GPIO E[7:0] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinE0_7_IoMux     0x1          // GPIO E[7:0] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinE0_7_PullUp    0x0          // GPIO E[7:0] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."

//------------------------------------------------------------------------------
// GPIO E[15:8] Registers define
//------------------------------------------------------------------------------
#define PinE8_15_Init      0x00        // GPIO E[15:8] data register OUTPUT init
#define PinE8_15_InOut     0x00        // GPIO E[15:8] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinE8_15_IoMux     0x1         // GPIO E[15:8] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinE8_15_PullUp    0x0         // GPIO E[15:8] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."

//------------------------------------------------------------------------------
// GPIO E[23:16] Registers define
//------------------------------------------------------------------------------
#define PinE16_23_Init      0x00        // GPIO E[23:16] data register OUTPUT init
#define PinE16_23_InOut     0x00        // GPIO E[23:16] data direction register init.0 is INPUT,1 is OUTPUT.
#define PinE16_23_IoMux     0x1         // GPIO E[23:16] IO multiplex control register init. refer to 'pin_iocfg.xlsx'
#define PinE16_23_PullUp    0x0         // GPIO E[23:16] IO Pull Up control register init. And bit7, 0 is "Gph0 or Gph1 selects uartb function.", 1 is "Gph0 or Gph1 selects SMCLK3 function."

/*-----------------------------------------------------------------------------
 * Special define
 *---------------------------------------------------------------------------*/
#define PinA0_15_IoMux  ((PinA8_15_IoMux<<16)+PinA0_7_IoMux)
#define PinA16_31_IoMux ((PinA24_31_IoMux<<16)+PinA16_23_IoMux)
#define PinB0_15_IoMux  ((PinB8_15_IoMux<<16)+PinB0_7_IoMux)
#define PinB16_26_IoMux ((PinB24_26_IoMux<<16)+PinB16_23_IoMux)
#define PinC0_13_IoMux  ((PinC8_13_IoMux<<16)+PinC0_7_IoMux)
#define PinDE_IoMux     ((PinE0_7_IoMux<<14)+PinD0_6_IoMux)

#define PinA0_15_PullUp     ((PinA8_15_PullUp<<8)+PinA0_7_PullUp)
#define PinA16_31_PullUp    ((PinA24_31_PullUp<<8)+PinA16_23_PullUp)
#define PinB0_15_PullUp     ((PinB8_15_PullUp<<8)+PinB0_7_PullUp)
#define PinB16_26_PullUp    ((PinB24_26_PullUp<<4)+PinB16_23_PullUp)
#define PinC0_13_PullUp     ((PinC8_13_PullUp<<8)+PinC0_7_PullUp)
#define PinDE_PullUp        ((PinE0_7_PullUp<<16)+(PinE8_15_PullUp<<24)+(PinE16_23_PullUp<<8)+PinD0_6_PullUp)


#define GPIO_INPUT_VAL  (0x00)
#define GPIO_INPUT_EN   (0x04)
#define GPIO_OUTPUT_EN  (0x08)
#define GPIO_OUTPUT_VAL (0x0C)
#define GPIO_PULLUP_EN  (0x10)
#define GPIO_DRIVE      (0x14)
#define GPIO_RISE_IE    (0x18)
#define GPIO_RISE_IP    (0x1C)
#define GPIO_FALL_IE    (0x20)
#define GPIO_FALL_IP    (0x24)
#define GPIO_HIGH_IE    (0x28)
#define GPIO_HIGH_IP    (0x2C)
#define GPIO_LOW_IE     (0x30)
#define GPIO_LOW_IP     (0x34)
#define GPIO_IOF_EN     (0x38)
#define GPIO_IOF_SEL    (0x3C)
#define GPIO_DEBOUNCE_EN   (0x48)
#define GPIO_OUTPUT_XOR    (0x40)


/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/
#define PALT            0x00
#define INPUT           0x80
#define OUTPUT          0x40

#define NO_PULL         0x00
#define PULL_UP         0x01

#define Driving4mA      0x00    //4mA
#define Driving8mA      0x10    //8mA
#define Driving12mA     0x18    //12mA
#define MaxDriving      0x38    //16mA

//-----------------------------------------------------------------
// GPIO Active
//-----------------------------------------------------------------
#define GPin0           bit0
#define GPin1           bit1
#define GPin2           bit2
#define GPin3           bit3
#define GPin4           bit4
#define GPin5           bit5
#define GPin6           bit6
#define GPin7           bit7
#define HighActive      1
#define LowActive       0


#define Read_ALW_PWRGD()		IS_MASK_SET(GPIO0_DR0,BIT(2))

#define POWER_STATUS_H()		SET_MASK(GPIO0_DR0,BIT(3))
#define POWER_STATUS_L()		CLEAR_MASK(GPIO0_DR0,BIT(3))

#define CHARGE_LED_ON()         SET_MASK(GPIO0_DR0, BIT(3))
#define CHARGE_LED_OFF()        CLEAR_MASK(GPIO0_DR0, BIT(3))

#define S3OK_H()		SET_MASK(GPIO0_DR0,BIT(4))  //GPIO0_A1
#define S3OK_L()		CLEAR_MASK(GPIO0_DR0,BIT(4))

#define CHARGE_FULL_LED_ON()    SET_MASK(GPIO0_DR0, BIT(5))
#define CHARGE_FULL_LED_OFF()   CLEAR_MASK(GPIO0_DR0, BIT(5))

#define Read_BC_ACOK()		IS_MASK_SET(GPIO0_DR0,BIT(6))//AC present detect 

#define ALW_ON()		SET_MASK(GPIO0_DR0,BIT(7))
#define ALW_OFF()		CLEAR_MASK(GPIO0_DR0,BIT(7))

#define HOT_LED_H()		SET_MASK(GPIO0_DR1,BIT(0))
#define HOT_LED_L()		CLEAR_MASK(GPIO0_DR1,BIT(0))

#define Read_FT_GPIO1_A5_H()		IS_MASK_SET(GPIO0_DR1,BIT(1))
#define Read_FT_GPIO1_A5_L()		IS_MASK_CLEAR(GPIO0_DR1,BIT(1))

#define P1V8_EN_H()				SET_MASK(GPIO0_DR1,BIT(2))
#define P1V8_EN_L()				CLEAR_MASK(GPIO0_DR1,BIT(2))

#define H_A20GATE_ON()	SET_MASK(GPIO0_DR1,BIT(5))
#define H_A20GATE_OFF()	CLEAR_MASK(GPIO0_DR1,BIT(5))

#define H_RCIN_ON()		CLEAR_MASK(GPIO0_DR1,BIT(6))
#define H_RCIN_OFF()		SET_MASK(GPIO0_DR1,BIT(6))

#define Read_FT_GPIO1_B0_H()		IS_MASK_SET(GPIO0_DR1,BIT(7))
#define Read_FT_GPIO1_B0_L()		IS_MASK_CLEAR(GPIO0_DR1,BIT(7))

#define EN_P5V_3P3V_H()	SET_MASK(GPIO0_DR2,BIT(0))		
#define EN_P5V_3P3V_L()	CLEAR_MASK(GPIO0_DR2,BIT(0))

#define Read_MXM_PWR_GD()		IS_MASK_SET(GPIO0_DR2,BIT(3)) //

#define Read_CPU_RST_OK()		IS_MASK_SET(GPIO0_DR2,BIT(4)) 

#define EN_MXM_PWR_EC_H()	SET_MASK(GPIO0_DR2,BIT(5))	
#define EN_MXM_PWR_EC_L()	CLEAR_MASK(GPIO0_DR2,BIT(5))

#define CPU_RST_H()	SET_MASK(GPIO0_DR2,BIT(6))		
#define CPU_RST_L()	CLEAR_MASK(GPIO0_DR2,BIT(6))

#define MM_RESET_H()		SET_MASK(GPIO0_DR2,BIT(7))
#define MM_RESET_L()		CLEAR_MASK(GPIO0_DR2,BIT(7))

#define Read_LID()		IS_MASK_CLEAR(GPIO0_DR3,BIT(0))

#define CHARG_LED_ON()	SET_MASK(GPIO0_DR3,BIT(1))       
#define CHARG_LED_OFF()	CLEAR_MASK(GPIO0_DR3,BIT(1))

#define SCIREQ_ON()		CLEAR_MASK(GPIO0_DR3,BIT(3))  
#define SCIREQ_OFF()		SET_MASK(GPIO0_DR3,BIT(3))

#define SOC_WAKE_SCI_ON()           CLEAR_MASK(GPIO0_DR3,       BIT(3)) //OUTPUT
#define SOC_WAKE_SCI_OFF()          SET_MASK(GPIO0_DR3,         BIT(3))

#define EXTSMI_ON()		CLEAR_MASK(GPIO0_DR3,BIT(4))
#define EXTSMI_OFF()		SET_MASK(GPIO0_DR3,BIT(4))

#define NUMLOCK_LED_ON()	SET_MASK(GPIO0_DR3,BIT(5))       
#define NUMLOCK_LED_OFF()	CLEAR_MASK(GPIO0_DR3,BIT(5))//

#define LVDS_EN_H()		SET_MASK(GPIO0_DR3,BIT(6))  //
#define LVDS_EN_L()		CLEAR_MASK(GPIO0_DR3,BIT(6))

#define CAPSLOCK_LED_ON()	SET_MASK(GPIO0_DR3,BIT(7))        
#define CAPSLOCK_LED_OFF()	CLEAR_MASK(GPIO0_DR3,BIT(7))//

#define HotControl1_ON()	CLEAR_MASK(GPIO1_DR0,BIT(0))      
#define HotControl1_OFF()	SET_MASK(GPIO1_DR0,BIT(0)) 

#define Read_PWR_SW()		IS_MASK_CLEAR(GPIO1_EXT0,BIT(4))

#define EC_L_BKLT_EN_ON()		SET_MASK(GPIO1_DR0,BIT(5))

#define EC_L_BKLT_EN_OFF()		CLEAR_MASK(GPIO1_DR0,BIT(5))

#define Read_VDDCORE_PG_H()		IS_MASK_SET(GPIO1_DR0,BIT(6)) 

#define VDD_CORE_EN_H()		SET_MASK(GPIO1_DR0,BIT(7))
#define VDD_CORE_EN_L()		CLEAR_MASK(GPIO1_DR0,BIT(7))

#define EN_1P8V_STBY_H()	SET_MASK(GPIO1_DR1,BIT(2))
#define EN_1P8V_STBY_L()	CLEAR_MASK(GPIO1_DR1,BIT(2))

#define EN_1P05V_720201_H()	SET_MASK(GPIO1_DR1,BIT(3))
#define EN_1P05V_720201_L()	CLEAR_MASK(GPIO1_DR1,BIT(3))

#define EN_MXM_P19V_H()	SET_MASK(GPIO1_DR1,BIT(4))
#define EN_MXM_P19V_L()	CLEAR_MASK(GPIO1_DR1,BIT(4))

#define TMDS_EN_H()	SET_MASK(GPIO1_DR1,BIT(6))       
#define TMDS_EN_L()	CLEAR_MASK(GPIO1_DR1,BIT(6)) 

#define EC_1V1_WX_EN_H()		SET_MASK(GPIO1_DR2,BIT(0))
#define EC_1V1_WX_EN_L()		CLEAR_MASK(GPIO1_DR2,BIT(0))
#define LED_POWER_H()				SET_MASK(GPIO1_DR2,BIT(1))
#define LED_POWER_L()				CLEAR_MASK(GPIO1_DR2,BIT(1))
#define Read_EC_1V1_WX_PWRGD()		IS_MASK_SET(GPIO1_DR2,BIT(2))
//#define Read_BAT_DET_H()		IS_MASK_SET(GPIO1_DR2,BIT(6)) 
#define MEM_RESET_S3_H()			SET_MASK(GPIO1_DR2,BIT(6)) 
#define MEM_RESET_S3_L()			CLEAR_MASK(GPIO1_DR2,BIT(6)) // EN_DDR_SDRAM  -----C7

#define DDR4_VPP_EN_H()					SET_MASK(GPIO1_DR2,BIT(7))			//FIXME

#define DDR4_VPP_EN_L()					CLEAR_MASK(GPIO1_DR3,BIT(3))

#define WX1860_RSTN_H()					SET_MASK(GPIO1_DR3,BIT(0))			//FIXME

#define WX1860_RSTN_L()					CLEAR_MASK(GPIO1_DR3,BIT(4))
#define Read_BATT_PRS_1()				IS_MASK_CLEAR(GPIO1_DR3,BIT(5))  //BAT PRESENT DETECT

#define VDDQ_MCUS3_EN_H()					SET_MASK(GPIO1_DR3,BIT(2))		//FIXME

#define VDDQ_MCUS3_EN_L()					CLEAR_MASK(GPIO1_DR3,BIT(6))

#define VDDQ_MCUS5_EN_H()					SET_MASK(GPIO2_DR0,BIT(0))
#define VDDQ_MCUS5_EN_L()					CLEAR_MASK(GPIO2_DR0,BIT(0))
#define M2_PCIE_RST_H()		SET_MASK(GPIO2_DR0,BIT(1))
#define M2_PCIE_RST_L()		CLEAR_MASK(GPIO2_DR0,BIT(1))
#define SM768_PCIE_RST_H()	SET_MASK(GPIO2_DR0,BIT(2))
#define SM768_PCIE_RST_L()	CLEAR_MASK(GPIO2_DR0,BIT(2))

#define P12V_LVDS_EN_H()	SET_MASK(GPIO2_DR0,BIT(3))
#define P12V_LVDS_EN_L()	CLEAR_MASK(GPIO2_DR0,BIT(3)) //@

#define BACKLIGHT_H()	SET_MASK(GPIO2_DR0,BIT(4))
#define BACKLIGHT_L()	CLEAR_MASK(GPIO2_DR0,BIT(4))

#define HotControl3_ON()	SET_MASK(GPIO2_DR0,BIT(5))     
#define HotControl3_OFF()	CLEAR_MASK(GPIO2_DR0,BIT(5))

#define USB_PONRSTB_H()		SET_MASK(GPIO2_DR0,BIT(6))
#define USB_PONRSTB_L()		CLEAR_MASK(GPIO2_DR0,BIT(6))
#define USB_RESET_H()		SET_MASK(GPIO2_DR0,BIT(7))
#define USB_RESET_L()		CLEAR_MASK(GPIO2_DR0,BIT(7))  


#define DISCHARGE_SIG_H()	SET_MASK(GPIO2_DR1,BIT(0))
#define DISCHARGE_SIG_L()	CLEAR_MASK(GPIO2_DR1,BIT(0))
#define READ_VDD_MCU01_PG()		IS_MASK_SET(GPIO1_DR1,BIT(5))

#define FULL_CHARG_LED_ON()	 SET_MASK(GPIO2_DR1,BIT(3))       
#define FULL_CHARG_LED_OFF()	CLEAR_MASK(GPIO2_DR1,BIT(3)) 

#define READ_P1V8_PG()		IS_MASK_SET(GPIO2_DR1,BIT(4))
#define READ_P5V_3P3V_PG()		IS_MASK_SET(GPIO2_DR1,BIT(5))

#define Read_SLPS3()	IS_MASK_SET(GPIO1_DR0,BIT(5))
#define Read_SLPS4()	IS_MASK_SET(GPIO0_DR2,BIT(4))
#define Read_SLPS5()	IS_MASK_SET(GPIO0_DR1,BIT(1))
#define Read_HWPG()		IS_MASK_SET(GPIO0_DR2,BIT(6))

#define DNBSWON_ON()	CLEAR_MASK(GPIO2_DR1,BIT(3))
#define DNBSWON_OFF()	SET_MASK(GPIO2_DR1,BIT(3))


extern void Init_GPIO(void);

#endif
