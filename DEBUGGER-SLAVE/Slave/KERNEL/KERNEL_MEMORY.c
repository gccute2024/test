/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-05 10:50:55
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

//-----------------------------------------------------------------------------
// Common Header Include
//-----------------------------------------------------------------------------
#include "KERNEL_MEMORY.H"
#include "CUSTOM_POWER.H"

VBYTE SCI_Event_Buffer[10];//offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE SMI_Event_Buffer[10];//offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE PD_SCI_Event_Buffer[10];//offset:HEX 0x00-0x09 SIZE:DEC 10
VBYTE KBDataPending[3];
VBYTE MSDataPending[3];
VBYTE PS2_PortN_Data[3];
VBYTE *Tmp_code_pointer=NULL;
VBYTE * Tmp_XPntr;
VBYTE * Tmp_XPntr1;

void init_memory_var(void)
{
        
        #ifdef AE101
        CORE_Version=101;
        #elif defined(AE102)
        CORE_Version=102;
        #elif defined(AE103)
        CORE_Version=103;
        #endif
        dprint("CORE VERSION AE%d\n",CORE_Version);

        NumLockKey = 1;
        Host_Flag = 0x3;
        
        Host_Flag_INTR_KEY = 1;
        Host_Flag_INTR_AUX = 1;
        PowerSequence_Step = 1;

        System_PowerState=SYSTEM_S5;
}

