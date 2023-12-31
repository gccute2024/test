/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:20:43
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

#ifndef CUSTOM_MAIN_H
#define CUSTOM_MAIN_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_HOST.H"

extern VBYTE * Tmp_XPntr;
/*-----------------------------------------------------------------------------
 * CUSTOM SCI/SMI Event Code Define
 *---------------------------------------------------------------------------*/
#define _SCIEVT_NULL                0x00    //Can't use
#define _SCIEVT_01H                 0x01    //Reserved
#define _SCIEVT_02H                 0x02    //Reserved
#define _SCIEVT_03H                 0x03    //Reserved
#define _SCIEVT_04H                 0x04    //Reserved
#define _SCIEVT_05H                 0x05    //Reserved
#define _SCIEVT_BATTERY             0x06    //Battery status changed
#define _SCIEVT_ADAPTER             0x07    //Adapter status changed
#define _SCIEVT_08H                 0x08    //Reserved
#define _SCIEVT_09H                 0x09    //Reserved
#define _SCIEVT_FnF1                0x0A    //Hotkey Fn+ F1 Event
#define _SCIEVT_FnF2                0x0B    //Hotkey Fn+ F2 Event
#define _SCIEVT_FnF3                0x0C    //Hotkey Fn+ F3 Event
#define _SCIEVT_FnF4                0x0D    //Hotkey Fn+ F4 Event
#define _SCIEVT_FnF5                0x0E    //Hotkey Fn+ F5 Event
#define _SCIEVT_FnF6                0x0F    //Hotkey Fn+ F6 Event
#define _SCIEVT_FnF7                0x10    //Hotkey Fn+ F7 Event
#define _SCIEVT_FnF8                0x11    //Hotkey Fn+ F8 Event
#define _SCIEVT_FnF9                0x12    //Hotkey Fn+ F9 Event
#define _SCIEVT_FnF10               0x13    //Hotkey Fn+F10 Event
#define _SCIEVT_FnF11               0x14    //Hotkey Fn+F11 Event
#define _SCIEVT_FnF12               0x15    //Hotkey Fn+F12 Event

#define _SCIEVT_SLEEP               0x31
#define _SCIEVT_LCD                 0x32

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Service_OEM_LoTimer(void);
extern void Service_LPCRST(void);
extern void Service_WaitPS2_Handle(void);
extern BYTE  OemSetWakeUpSource(void);
extern void InitWakeFromDeepSleep(void);
extern void InitEnterDeepSleep(void);
extern BYTE  vCheckChipCanDeepSleep(void);
extern void OEM_ColdResetInit(void);
extern void OEM_Init_SMBus(void);
extern void OEM_INIT_ACPI(void);
/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
void Hook_1msEvent(BYTE EventId);
void Hook_5msEvent(void);
void Hook_10msEvent(void);
void Hook_10msEventA(void);
void Hook_10msEventB(void);
void Hook_50msEventA(void);
void Hook_50msEventB(void);
void Hook_50msEventC(void);
void Hook_100msEventA(void);
void Hook_100msEventB(void);
void Hook_100msEventC(void);
void Hook_500msEventA(void);
void Hook_500msEventB(void);
void Hook_500msEventC(void);
void Hook_1secEventA(void);
void Hook_1secEventB(void);
void Hook_1secEventC(void);
void Hook_1minEvent(void);

//-----------------------------------------------------------------------------
#endif
