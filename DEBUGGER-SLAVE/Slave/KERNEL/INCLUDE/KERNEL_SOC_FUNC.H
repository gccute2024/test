/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-12 19:05:24
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

#ifndef KERNEL_SOC_FUNC_H
#define KERNEL_SOC_FUNC_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_INCLUDE.H"
__BEGIN_DECLS

#define PM1_HOST_INT_ENABLE	0x10		//P143
#define IBF_INT_ENABLE	0x1	//P157
#define OBE_INT_ENABLE  0x2
#define OBF_KEYBOARD_INT_ENABLE	0x1		//P143
#define OBF_MOUSE_INT_ENABLE 	0x2		//P143
#define IRQ1_CONTROL_BIT	0x1		//P144
#define IRQ12_CONTROL_BIT	0x2		//P144
#define IRQ11_CONTROL_BIT	0x4		//P144

#define UARTn_IER	0x1
#define UARTn_LCR	0x3
#define	DLAB	BIT7

#define printdbg  dprint
void udelay(DWORD delay);
void soc_init();
int  PS2_Init(void);
int pmc1_init(void);
int pmc2_init(void);
int pmc3_init(void);
int pmc4_init(void);
int pmc5_init(void);
int kbc_init(void);
void i2c_pull_up(void);
void ps2_pull_up(void);
void cec_pull_up(void);
void tach_pull_up(void);


__END_DECLS

#endif

