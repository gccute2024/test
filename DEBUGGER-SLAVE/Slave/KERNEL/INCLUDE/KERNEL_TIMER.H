/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-08 14:24:44
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

#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

#define	TIMERn_TLC				0x0		//load value
#define	TIMERn_TCV				0x4		//current count
#define	TIMERn_TCR				0x8		//value to set
#define	TIMERn_TEOI				0xc		//clear interrupt
#define	TIMERn_TIS				0x10	//interrupt status

#if 0
#define	TIMER_TIS				0xA0	//TIMER0~3 interrupt status
#define	TIMER_TEOI				0xA4	//TIMER0~3 interrupt clear
#define	TIMER_TRIS				0xA8	//TIMER0~3 interrupt raw status

#define TIMER0_TCR	(*(volatile u8 *) 0x1808)
#define TIMER0_TIS	(*(volatile u8 *) 0x1810)
#define TIMER1_TCR	(*(volatile u8 *) 0x181C)
#define TIMER1_TIS	(*(volatile u8 *) 0x1824)
#define TIMER2_TCR	(*(volatile u8 *) 0x1830)
#define TIMER2_TIS	(*(volatile u8 *) 0x1838)
#define TIMER3_TCR	(*(volatile u8 *) 0x1844)
#define TIMER3_TIS	(*(volatile u8 *) 0x18A0)
#endif


#define TIMER0	0x0
#define TIMER1	0x1
#define TIMER2	0x2
#define TIMER3	0x3
/************TIMER0常见定时定义************/
#define TIMER0_freq  0x0
#define TIMER0_1s       (LOW_CHIP_CLOCK/(2<<TIMER0_freq))
#define TIMER0_100ms    (TIMER0_1s/10)
#define TIMER0_50ms     (TIMER0_1s/20)
#define TIMER0_20ms     (TIMER0_1s/50)
#define TIMER0_10ms     (TIMER0_1s/100)
#define TIMER0_5ms      (TIMER0_1s/200)
#define TIMER0_2ms      (TIMER0_1s/500)
#define TIMER0_1ms      (TIMER0_1s/1000)


/************TIMER1常见定时定义************/
#define TIMER1_freq  0x0
#define TIMER1_1s       (LOW_CHIP_CLOCK/(2<<TIMER1_freq))
#define TIMER1_100ms    (TIMER1_1s/10)
#define TIMER1_50ms     (TIMER1_1s/20)
#define TIMER1_20ms     (TIMER1_1s/50)
#define TIMER1_10ms     (TIMER1_1s/100)
#define TIMER1_5ms      (TIMER1_1s/200)
#define TIMER1_2ms      (TIMER1_1s/500)
#define TIMER1_1ms      (TIMER1_1s/1000)


/************TIMER2常见定时定义************/
//由于计数器只有16位而timer2频率有24.576Mhz所以延时最长大概4ms左右
#define TIMER2_freq  0x0
#define TIMER2_1ms       (HIGHT_CHIP_CLOCK/(2<<TIMER2_freq)/1000)
#define TIMER2_100us    (TIMER2_1ms/10)
#define TIMER2_50us     (TIMER2_1ms/20)
#define TIMER2_20us     (TIMER2_1ms/50)
#define TIMER2_10us     (TIMER2_1ms/100)
#define TIMER2_5us      (TIMER2_1ms/200)
#define TIMER2_2us      (TIMER2_1ms/500)
#define TIMER2_1us      (TIMER2_1ms/1000)

/************TIMER3常见定时定义************/
//由于计数器只有16位而timer2频率有24.576Mhz所以延时最长大概4ms左右
#define TIMER3_freq  0x0
#define TIMER3_1ms       (HIGHT_CHIP_CLOCK/(2<<TIMER3_freq)/1000)
#define TIMER3_100us    (TIMER3_1ms/10)
#define TIMER3_50us     (TIMER3_1ms/20)
#define TIMER3_20us     (TIMER3_1ms/50)
#define TIMER3_10us     (TIMER3_1ms/100)
#define TIMER3_5us      (TIMER3_1ms/200)
#define TIMER3_2us      (TIMER3_1ms/500)
#define TIMER3_1us      (TIMER3_1ms/1000)






#define	my_write8(base, addr, val)		*(volatile u8 *)(base + addr) = val
#define	my_readl(base, addr)			*(volatile u8 *)(base + addr)

extern int TIMER_Disable(u8 index);
extern int TIMER_Init(u8 index, u32 load_count, u8 loop_enable, u8 int_mask);
extern void InternalWDTNow(void);

extern BYTE  Timer_Int_Enable(BYTE  channel);
extern BYTE  Timer_Int_Disable(BYTE  channel);
extern BYTE  Timer_Int_Enable_Read(BYTE  channel);
extern BYTE  Timer_Int_Status(BYTE  channel);
extern void Service_MainLoTimer(void);
extern void vDelayXms(WORD bMS);
extern void vDelayXus(BYTE bUS);
#endif

