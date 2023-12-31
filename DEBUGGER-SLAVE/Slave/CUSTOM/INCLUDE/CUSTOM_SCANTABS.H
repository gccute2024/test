/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:21:43
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

#ifndef CUSTOM_SCANTEBS_H
#define CUSTOM_SCANTEBS_H
#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_XLATE.H"

extern const BYTE  Rc_ROM_Tables[][8] ;
extern void Check_KB_IN_S3(BYTE  KSIValus, BYTE  KSOValus);
extern void Check_KB_Wake_S3(BYTE  BKSI,BYTE  BKSO);
extern void Ex_Check_KB_Wake_S3(BYTE  BKSI,BYTE  BKSO);
extern void Check_Send_Key(BYTE  table_entry, BYTE  event);
extern BYTE  Skip_Send_Key(void);
extern  void Process_Sskey3(BYTE  sskey2, BYTE  event);
extern BYTE  Scratch_Get_Index_Comb_BIT6(void);
extern BYTE  Scratch_Get_Index_Comb_BIT7(void);
extern void Scratch_Fn_Key_Break(void);
extern void Scratch_Fn_Key_Make(void);


extern VBYTE *Tmp_code_pointer;
#endif