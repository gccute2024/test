/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:32:34
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

#include "KERNEL_WATCHDOG.H"

#define _REGADDR_WDT_CR			0x00000000
#define _REGADDR_WDT_TORR 		0x00000004
#define _REGADDR_WDT_CCVR 		0x00000008
#define _REGADDR_WDT_CRR		0x0000000C
#define _REGADDR_WDT_STAT 		0x00000010
#define _REGADDR_WDT_EOI		0x00000014
#define _REGADDR_WDT_CP5		0x000000E4
#define _REGADDR_WDT_CP4		0x000000E8
#define _REGADDR_WDT_CP3		0x000000EC
#define _REGADDR_WDT_CP2		0x000000F0
#define _REGADDR_WDT_CP1		0x000000F4
#define _REGADDR_WDT_CV			0x000000F8
#define _REGADDR_WDT_CT			0x000000FC

/**
 * @brief 清除看门狗超时中断但不进行喂狗
 *
 * @param    无
 *
 * @return   无
 */
void WDT_Clear_IRQ(void)
{
	while(1)
	{
		if(WDT_REG(_REGADDR_WDT_STAT) & 0x01)
		{
			/*Clear interruption*/
			WDT_REG(_REGADDR_WDT_EOI);
			return;
		}
	}
}

/**
 * @brief 清除看门狗超时中断并进行喂狗操作
 *
 * @param fd_counter   最多喂狗次数
 *
 * @return             无
 */
void WDT_FeedDog(DWORD fd_counter)
{
	while(fd_counter--)
	{
		//dprint("*****Feed Watchdog****\n");  FIXME xia
		WDT_REG(_REGADDR_WDT_CRR) = 0x76;
		
		if(WDT_REG(_REGADDR_WDT_STAT) & 0x01){
			/*Clear interruption*/
			WDT_REG(_REGADDR_WDT_EOI);
			dprint("*****Watchdog Feed Failed****\n");
			return;
		}
	}

	return;
}

/**
 * @brief 看门狗初始化
 *
 * @param    mode       0:超时复位      1：超时中断
 * @param    count      重装载计数值
 *
 * @return   无
 */
void WDT_Init(BYTE  mode,WORD count)
{
	#if !(WDT_CLOCK_EN)
	dprint("WDT时钟未打开!");
	return;
	#endif
	BYTE  temp=0;
	//WDT_REG(_REGADDR_WDT_CR) = 0x1f;
	if(mode==0)
	{
		temp=0x1d;
	}
	if(mode==1)
	{
		temp=0x1f;
	}
	WDT_REG(_REGADDR_WDT_CR) = temp;
	WDT_REG(_REGADDR_WDT_TORR) = count;
	WDT_FeedDog(100);
	dprint("Watchdog Init OK\n");
	return;
}

