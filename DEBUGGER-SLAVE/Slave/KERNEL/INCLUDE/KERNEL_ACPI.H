/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:26:17
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

#ifndef KERNEL_ACPI_H
#define KERNEL_ACPI_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

extern VBYTE * Tmp_XPntr;

#define T_Burst_Loop        250

#define PMC1_Int_Enable(type)	(PMC1_CTL |= (0x1<<type))
#define PMC2_Int_Enable(type)	(PMC2_CTL |= (0x1<<type))
#define PMC3_Int_Enable(type)	(PMC3_CTL |= (0x1<<type))
#define PMC4_Int_Enable(type)	(PMC4_CTL |= (0x1<<type))
#define PMC5_Int_Enable(type)	(PMC5_CTL |= (0x1<<type))

#define PMC1_Int_Disable(type)	(PMC1_CTL &= ~(0x1<<type))
#define PMC2_Int_Disable(type)	(PMC2_CTL &= ~(0x1<<type))
#define PMC3_Int_Disable(type)	(PMC3_CTL &= ~(0x1<<type))
#define PMC4_Int_Disable(type)	(PMC4_CTL &= ~(0x1<<type))
#define PMC5_Int_Disable(type)	(PMC5_CTL &= ~(0x1<<type))

#define PMC1_Int_Enable_Read(type)	(PMC1_CTL & (0x1<<type) != 0)
#define PMC2_Int_Enable_Read(type)	(PMC2_CTL & (0x1<<type) != 0)
#define PMC3_Int_Enable_Read(type)	(PMC3_CTL & (0x1<<type) != 0)
#define PMC4_Int_Enable_Read(type)	(PMC4_CTL & (0x1<<type) != 0)
#define PMC5_Int_Enable_Read(type)	(PMC5_CTL & (0x1<<type) != 0)

/*-----------------------------------------------------------------------------
 * Code Parameter
 *---------------------------------------------------------------------------*/
#define _PM1_STEP_1         1
#define _PM1_STEP_2         2
#define _PM1_STEP_3         3
#define _PM1_STEP_4         4
#define _PM1_STEP_5         5
#define _PM1_STEP_6         6
#define _PM1_STEP_7         7

#define QeventSCI           0x01

#define SCIMode_Normal      0x00    // For CUSTOM SCI event
#define SCIMode_Pending     0x01
#define SCIMode_Force       0x02
#define SCIMode_ACPI_SMBus  0x03
/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
#define   EVENT_BUFFER_SIZE   9
#define   PENDING_BUFFER      8

extern VBYTE SCI_Event_Buffer[10];
extern VBYTE SMI_Event_Buffer[10];
extern VBYTE PD_SCI_Event_Buffer[10];

extern void Service_PCI2(void);
extern void Service_PCI2_Main(void);

extern void EC6266_Cmd_80(void);
extern void EC6266_Cmd_81(void);
extern void EC6266_Cmd_82(void);
extern void EC6266_Cmd_83(void);
extern void EC6266_Cmd_84(void);
extern void EC_Cmd_8X(void);

extern void Service_Event_Center(void);
extern void SCI_Response(void);
extern void SCI_Interrupt(void);
extern void SMI_Interrupt(void);
extern void Loop_Delay(BYTE delay);
extern BYTE Read_SCI_Query_Value(void);
extern BYTE Write_SCI_Query_Value(BYTE NewSCI);
extern BYTE Read_SMI_Query_Value(void);
extern BYTE Write_SMI_Query_Value(BYTE NewSMI);
extern void Clear_Event_Buffer(void);
extern void vDelayXus(BYTE bUS);  //zb 10.26 add
extern int CheckBurstMode(void);  //zb 10.26 add
extern void vSetTotalBurstTime(void);
extern void ACPI_SMB_Via_EC(void);

/*-----------------------------------------------------------------------------
 * Internal Calling Prototype
 *---------------------------------------------------------------------------*/
static BYTE ACPI_SMB_NULL(void);
static BYTE ACPI_SMB_WQuick_CMD(void);
static BYTE ACPI_SMB_RQuick_CMD(void);
static BYTE ACPI_SMB_Send_BYTE(void);
static BYTE ACPI_SMB_Recive_BYTE(void);
static BYTE ACPI_SMB_Write_BYTE(void);
static BYTE ACPI_SMB_Read_BYTE(void);
static BYTE ACPI_SMB_Write_WORD(void);
static BYTE ACPI_SMB_Read_WORD(void);
static BYTE ACPI_SMB_Write_BLOCK(void);
static BYTE ACPI_SMB_Read_BLOCK(void);
static BYTE ACPI_SMB_Process_Call(void);
static BYTE ACPI_SMB_BLOCK_Call(void);

extern void Gen_EC_QEvent(BYTE sci_number, BYTE sci_mode);
extern void EC_QEvent_Suspend(BYTE sci_number);
extern void Release_EC_QEvent_Suspend(void);

extern void Disable_Interrupt_Main_Switch(void);
extern void Enable_Interrupt_Main_Switch(void);

extern void Write_Ext_RAMSpace(void);
extern void Read_Ext_RAMSpace(void);
extern BYTE Read_Map_ECSPACE_BASE_ADDR(BYTE MapIndex);
extern void Write_Map_ECSPACE_BASE_ADDR( BYTE MapIndex,BYTE data1 );
//-----------------------------------------------------------------------------
#include "CUSTOM_PMC1.H"
#include "CUSTOM_PMC2.H"
#include "CUSTOM_PMC3.H"
#include "CUSTOM_PMC4.H"
#include "CUSTOM_PMC5.H"
#include "CUSTOM_GPIO.H"
#endif
