/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-20 15:53:27
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

#include "AE_INIT.H"


void  init()
{
  #ifndef NO_INIT
  /*Module Clock Enable*/
  Module_Clock_Config();
  #if SUPPORT_LOWPOWER
  s0_low_power_set();
  SYSCTL_PWRSWCSR = 0x61; // enable pwrsw
  #endif
  
  soc_init();// We must put this first before use any printf later
  dprint("Core freq at %d Hz\n",CPU_FREQ );
  //IRQC init 
  irqc_init(IRQC_NUM_INTERRUPTS);
  //
  /* Before enter into main, add the cycle/instret disable by default to save power,
       only use them when needed to measure the cycle/instret */
  disable_mcycle_minstret();//save power
  #endif
  return;
}


#ifdef USER_AE10X_LIBC_A
NORETURN void _exit(int __status)
{
  while(1);
}
void exit(int __status)
{
  dprint("exit status doc %d\n",__status);
  _exit(__status);
}
#endif