/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:26:41
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

#ifndef KERNEL_HOST_H
#define KERNEL_HOST_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"


extern VBYTE KBDataPending[3];
/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/

extern void Service_Send(void);

extern BYTE Send_KB_Data_To_Host(BYTE nKBData);
extern void KBC_Data_Suspend(BYTE nPending);
extern BYTE Release_KBC_Data_Suspend(void);
extern void Transmit_Data_To_Host(BYTE data_byte);
extern void Mouse_Data_To_Host(BYTE data_byte);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static BYTE Send_To_Host(BYTE data_word, BYTE break_prefix_flag);
static BYTE Common_Send_To_HOST(BYTE data_word, BYTE break_prefix_flag);
static BYTE Switch_Scan_Code(BYTE data_word, BYTE break_prefix_flag);

//-----------------------------------------------------------------------------
#include "KERNEL_PORT6064.H"
#endif
