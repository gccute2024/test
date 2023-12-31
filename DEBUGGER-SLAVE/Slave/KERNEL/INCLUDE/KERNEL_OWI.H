/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:27:41
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

#ifndef KERNEL_OWI_H
#define  KERNEL_OWI_H


extern void OWI_TM1803_Init(u8 One_Len, u8 Zero_Len);
extern void OWI_GENERAL_Init(u8 len, u8 cycle_num_clk);
extern void Owi_Init(void);
extern void Owi_SendData(void);

#endif 
