/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-05 15:47:23
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

#ifndef KERNEL_WATCHDOG_H
#define KERNEL_WATCHDOG_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

//#define WDT_STAT    (*(volatile u8 *) 0x3C10)

#define Wdt_Int_Status  (WDT_STAT & (0x1 << 0) != 0)


#define Wdt_Int_Status  (WDT_STAT & (0x1 << 0) != 0)

#define WDT_CRR_CRR     0x76;


void WDT_Clear_IRQ(void);
void WDT_FeedDog(DWORD fd_counter);
void WDT_Init(BYTE  mode,WORD count);
#endif
