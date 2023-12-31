/*
 * @Author: Iversu
 * @LastEditors: Maple
 * @LastEditTime: 2023-05-05 17:01:20
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

#ifndef KERNEL_PORT6064_H
#define KERNEL_PORT6064_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "AE_FUNC.H"
#include "KERNEL_HOST.H"
#include "KERNEL_PS2.H"

#define KBC_CTL_OBFKIE                          HEX01
#define KBC_CTL_OBFMIE                          HEX02
#define KBC_CTL_OBEIE                           HEX04
#define KBC_CTL_IBFIE                           HEX08
#define KBC_CTL_PMC12IE                         HEX10
#define KBC_STA_OBF                             HEX01
#define KBC_STA_IBF                             HEX02
#define KBC_STA_GPF                             HEX04
#define KBC_STA_A2                              HEX08
#define KBC_STA_STS                             0xF0

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define KBC_Int_Enable(type)	    (KBC_CTL |= (0x1<<type))
#define KBC_Int_Disable(type)	    (KBC_CTL &= ~(0x1<<type))
#define KBC_Int_Enable_Read(type)	((KBC_CTL & (0x1<<type))!=0)

#define KBC_IBF_INTERRUPT_OFFSET 13
/*-----------------------------------------------------------------------------
 * Wait Host Delay Parameter
 *---------------------------------------------------------------------------*/
#define WaitKBDataDelay         0x200
#define WaitECDataDelay         0x200
#define WaitPM2DataDelay        0x200

/*-----------------------------------------------------------------------------
 * Command Parameter
 *---------------------------------------------------------------------------*/
#define _CMD_BYTE1              0x02
#define _CMD_BYTE2              0x03
#define _CMD_BYTE3              0x04
#define _CMD_BYTE4              0x05

/*-----------------------------------------------------------------------------
 * External Calling Prototype
 *---------------------------------------------------------------------------*/
extern void Service_PCI(void);
extern void KB_Cmd_Handle(BYTE data_byte);
extern void Write_Debug_Data_To_Sram(uint8_t Data);

extern void Write_KCCB(BYTE NewKCCB);
extern VBYTE  Set_Port60_Data_Handle(void);
extern VBYTE  Set_Port60_Multi_Data_Handle(void);
extern VBYTE  Loop_Wait_Get_Port60_Data(void);
extern void Service_PCI_Main(void);
extern void A20GATE_ON(void);

extern void Command_A7(void);
extern void Command_A8(void);
extern void Command_AD(void);
extern void Command_AE(void);

extern BYTE Read_Output_Port_2(void);

/*-----------------------------------------------------------------------------
 * Internal Process Prototype
 *---------------------------------------------------------------------------*/
static void Cmd_0X(void);
static void Cmd_1X(void);
static void Cmd_2X(void);
static void Cmd_3X(void);
static void Cmd_4X(void);
static void Cmd_5X(void);
static void Cmd_6X(void);
static void Cmd_7X(void);
static void Command_8X(void);
static void Command_9X(void);
static void Command_AX(void);
static void Command_BX(void);
static void Command_CX(void);
static void Command_DX(void);
static void Command_EX(void);
static void Command_FX(void);

static void Command_A1(void);
static void Command_A4(void);
static void Command_A5(void);
static void Command_A5Data(void);

static void Command_A6(void);
static void Command_A9(void);
static void Command_AA(void);
static void Command_AB(void);
static void Command_AC(void);
static void Command_D3(void);
static void Command_D4(void);




//-----------------------------------------------------------------------------
#include "CUSTOM_KBC6064.H"
#endif
