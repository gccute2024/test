/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:21:24
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

#ifndef CUSTOM_PMC2_H
#define CUSTOM_PMC2_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_SPIF.H"

/*--------------------PMC2 Register Address definition-----------------*/

#define OBF2            BIT(0x00)
#define IBF2            BIT(0x01)
#define DBG_En          BIT(0x02)
#define C_D2            BIT(0x03)

/*--------------------------------------------------------------------------*/

#define SUPPORTED_PMC2_DX  1

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Service_PCI3(void);
extern BYTE OEM_GetPortPM2Data(void);
/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static BYTE Set_PortPM2_Data_Handle(void);
//-----------------------------------------------------------------------------

#include "AE_DEBUGGER.H"
#endif
