/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:27:56
 * @Description: This file is used for setting logical device relevant configuration
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

#ifndef KERNEL_PNP_H
#define KERNEL_PNP_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

/*--------------------PNP Register Address definition-----------------*/
#define PNP_CHIPID_VER_OFFSET           HEX04
#define PNP_LD_EN_OFFSET                HEX08
#define PNP_CONFIG_OFFSET               HEX0C
#define PNP_CTRL0_OFFSET                HEX10
#define PNP_CTRL1_OFFSET                HEX14

#define PNP_REG_ADDR(offset)            REG_ADDR(SYSCTL_BASE_ADDR,offset)

#define PNP_CHIPID_VER_ADDR             PNP_REG_ADDR(PNP_CHIPID_VER_OFFSET)
#define PNP_LD_EN_ADDR                  PNP_REG_ADDR(PNP_LD_EN_OFFSET)
#define PNP_CONFIG_ADDR                 PNP_REG_ADDR(PNP_CONFIG_OFFSET)
#define PNP_CTRL0_ADDR                  PNP_REG_ADDR(PNP_CTRL0_OFFSET)
#define PNP_CTRL1_ADDR                  PNP_REG_ADDR(PNP_CTRL1_OFFSET)

#define PNP_CHIPID_VER                  REG32(PNP_CHIPID_VER_ADDR)
#define PNP_LD_EN                       REG32(PNP_LD_EN_ADDR)
#define PNP_CONFIG                      REG32(PNP_CONFIG_ADDR)
#define PNP_CTRL0                       REG32(PNP_CTRL0_ADDR)
#define PNP_CTRL1                       REG32(PNP_CTRL1_ADDR)
/*--------------------------------------------------------------------------*/

extern void Config_PNP_Access_Request(void);
extern void Config_PNP_Access_Release(void);
extern void Config_PNP_Write(BYTE  idx,BYTE  ldn,BYTE  data);
extern void Config_PNP_Read(BYTE  idx,BYTE  ldn);
extern void Port80_Bram_PNP_Config(void);
extern void Clear_Logical_Device_Default_IRQ_Num(void);
extern void Config_KBC_PNP_Only(void);
extern void Config_All_Logical_Device_PNP(void);
extern void LogicalDevice_PNP_Config(void);
extern BYTE LD_PNP_Config(void);

#endif


