/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-14 00:17:35
 * @Description: 中断号越大优先级越高
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

#ifndef KERNEL_IRQ_H_
#define KERNEL_IRQ_H_

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_INCLUDE.H"

#if IRQC_DEBUG
#define irqprint(fmt, args...) dprint(fmt, ##args)
#else
#define irqprint(fmt, args...)
#endif
extern void _Dummy_(void);
extern DWORD kernel_trap(uintptr_t mcause, uintptr_t sp);
extern FUNCT_PTR_V_V Fla_If_Ptr,Smf_Ptr;
#include "AE_DEBUGGER.H"   
#endif
