/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:29:10
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

#ifndef KERNEL_XLATE_H
#define KERNEL_XLATE_H
#include <AE_GLOBAL.H>
#include <AE_REG.H>
#include <AE_CONFIG.H>
#include <KERNEL_KBS.H>
#define NUM_LOCK 	BIT(0)
#define LShift    	BIT(1)
#define RShift   	BIT(2)
#define LAlt       	BIT(3)
#define RAlt       	BIT(3)
#define LCtrl   	BIT(4)
#define RCtrl   	BIT(4)
#define OVLAY 		BIT(5)
#define FN        	BIT(6)
typedef volatile union _key_state
{
    BYTE byte;
    struct
    {
       BYTE num_lock:  1;  /* Bit[0] */
       BYTE lShift:      1;  /* Bit[1] */
       BYTE rShift:     1;  /* Bit[2] */
       BYTE tALT:       1;  /* Bit[3] */
       BYTE tCtrl:   1;  /* Bit[4] */
       BYTE tOVERLAY:   1;  /* Bit[5] */
       BYTE Fn:        1;  /* Bit[6] */
       BYTE bit7: 		 1;  /* Bit[7] */
    } index;
}  ukey_state;


extern void KBS_Xlate_Code2(uKEY key, BYTE  event);
extern void Transmit_Key(BYTE  table_entry, BYTE  event);
extern void Single_Code(BYTE  scan_code, BYTE  event);
extern  void Numlock_Corsor(BYTE  scan_code, BYTE  event);
extern  void Convert_Cursor(BYTE  scan_code, BYTE  event);
extern void Add_E0_Code(BYTE  scan_code, BYTE  event);
extern  void Generate_Scan_Code_E0(BYTE  state, BYTE  event);
extern  void Generate_Scan_Code(BYTE  state, BYTE  event);
extern void Particular_Code(BYTE  code_byte, BYTE  event);
extern  void HotKey_Fn_Function(BYTE  code_byte, BYTE  event);
extern void SetServiceSendFlag(void);
extern void Clear_FnKeys(void);
extern  BYTE  Get_Comb_Index(BYTE  comb, BYTE  table_entry, BYTE  event);

#include <CUSTOM_SCANTABS.H>
//-----------------------------------------------------------------------------
#include "KERNEL_PORT6064.H"
#endif
