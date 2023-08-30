/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-13 20:08:36
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

#include "HARDWARE_PPORT.H"
#if (defined(AE102)||defined(AE103))

/****************PPort*******************/
void PPort_Init(void)
{
    #if !(PPORT_CLOCK_EN)
        dprint("PPORT CLOCK NOT ENABLE\n");
        return;
    #endif

    dprint("0x4001 data is 0x%x\n", *((volatile uint8_t *)(0x4001)));

	#if(defined(AE102))
		#if (SYSCTL_CLOCK_EN)
		SYSCTL_PIO5_CFG |= 0x8000; 
		SYSCTL_PIO5_CFG &= 0xbfff; //select PPort Function
		#endif
	#endif
	#if(defined(AE103))
		#if (SYSCTL_CLOCK_EN)
		SYSCTL_PIO5_CFG |= 0x80000; 
		SYSCTL_PIO5_CFG &= 0xbffff; //select PPort Function
		#endif
	#endif
 
    dprint("0x30468 data is 0x%x\n", *((volatile uint16_t *)(0x30468)));

    PP_CTL &= 0xdf;     //set output direction
    PP_CTL |= 0x10;     //enable IRQ

    dprint("0x4001 data is 0x%x\n", *((volatile uint8_t *)(0x4001)));
    dprint("0x4002 data is 0x%x\n", *((volatile uint8_t *)(0x4002)));

}

void PPort_Send(void)
{

    uint8_t status;

    #if !(PPORT_CLOCK_EN)
        dprint("PPORT CLOCK NOT ENABLE\n");
        return;
    #endif

    //polling and wait for pport free
    status = PP_STA;
    dprint("status is 0x%x\n", status);
    while((status & 0x80) == 0x80) 
     {
      status = PP_STA;
      dprint("status is 0x%x\n", status);
     }
    PP_DATA |= 0xaf; //write data to data register
 
    dprint("start a byte transfer by pport!\n");

}
#endif
/****************PPort*******************/
