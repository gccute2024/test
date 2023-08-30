/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:26:23
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

#ifndef __KERNEL_ADC_H
#define __KERNEL_ADC_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

/**************************后面测试可删除**************************/

#define ADC_Int_Mask(type)			(ADC_INTMASK |= (0x1<<type))
#define ADC_Int_Unmask(type)		(ADC_INTMASK &= ~(0x1<<type))
#define ADC_Int_Mask_Read(type)		((ADC_INTMASK & (0x1<<type)) != 0)
#define ADC_Int_Status(type)	((ADC_INTSTAT & (0x1<<type)) != 0)
/**************************后面测试可删除**************************/



/*****中断屏蔽宏*******/
#define  uv_irq                   HEX04  //下溢中断
#define  ov_irq                   HEX02  //上溢中断
#define  af_irq                   HEX01  //FIFO将满中断

#define  ENABLE                   HEX01
#define  DISABLE                  HEX00





//*****************************************************************************
// Function prototype
//*****************************************************************************

/**************************后面测试可删除**************************/
//int ADC_RegWR(void);
//void ADC_Trigger_HW(void);
//extern void ADC_Init(void);
/**************************后面测试可删除**************************/

void ADC__TriggerMode_Config(uint8_t mode);
void ADC_SampleMode_Config(uint8_t mode);
void ADC_Channelx_Config(uint8_t channelx,uint8_t sw);
void ADC_IRQ_Config(uint8_t irq_type,uint8_t sw);
void ADC_Clear_IRQ(uint8_t irq_type);
uint8_t ADC_Ready(void);
void ADC_Timercount_Set(uint32_t timercount);
void ADC_Init(uint8_t channelx,uint8_t trigger,uint8_t mode);
void ADC_Trigger_operation(void);
short ADC_ReadData(void);
short ADC_SW_Sample(void);
#endif
