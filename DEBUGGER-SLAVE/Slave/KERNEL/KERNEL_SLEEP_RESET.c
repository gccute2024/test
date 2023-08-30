/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-21 18:39:51
 * @Description: This file applys for chip soft reset and chip sleep interface
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

#include "KERNEL_SLEEP_RESET.H"
#include "CUSTOM_POWER.H"
#include "KERNEL_TIMER.H"

int32_t SYSCTL_MODEN0_CONTEXT = 0;
int32_t SYSCTL_MODEN1_CONTEXT = 0;
int32_t SYSCTL_PIO0_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO1_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO2_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO3_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO4_IECFG_CONTEXT = 0;
int32_t SYSCTL_PIO5_IECFG_CONTEXT = 0;
int32_t SYSCTL_PLLCFG_CONTEXT = 0;

void RTC_Deep_Sleep_Test(void) 
{
	printf("RTC SLEEP CONFIG\n");

	SYSCTL_PMUCSR|=BIT(20);

	//CMR
	REG8(0x3F04)=0x1;
	REG8(0x3F05)=0x0;
	REG8(0x3F06)=0x0;
	REG8(0x3F07)=0x0;
	//CCR
	REG8(0x3F0C)=0x1|7<<2;
	REG8(0x3F0D)=0x0;
	REG8(0x3F0E)=0x0;
	REG8(0x3F0F)=0x0;
	//CPSR
	REG8(0x3F20)=0xA0;
	REG8(0x3F21)=0x00;
	REG8(0x3F22)=0x0;
	REG8(0x3F23)=0x0;

	// vDelayXus(120);

	SYSCTL_SWITCH_PLL=BIT3|1;

	SYSCTL_MODEN0=0x100;
	SYSCTL_MODEN1= 0x0|BIT3|BIT(19);

	SYSCTL_DVDD_EN |= 1<<4|1<<5;
	SYSCTL_DVDD_EN &= 0xfc;
	SYSCTL_DVDD_EN |= 1<<4|1<<5;

	CPU_Sleep();
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Module_SoftReset
 *
 * you need set corresponding bit to enable Module reset
 *
 * -----------------------------Module Reset bit map---------------------------
 * Moudle_RESET_REG0(0x30428)  1:reset enable  0:reset disable
 *   bit31:PS2M bit30:ROMP bit29:KBC bit28:PMC5 bit27:PMC4 bit26:PMC3 bit25:PMC2 bit24:PMC1
 *   bit23:SHM bit22:SWUC bit21:BRAM bit20:GPIO bit19:SMB0 bit18:SMB1 bit17:SMB2 bit16:SMB3
 *   bit15:WDT bit14:PPRORT bit13:HOST bit12:ADC bit11:PWM bit10:UARTA_BAUD bit9:UARTB_BAUD bit8:UART0_BAUD
 *   bit7:UART1_BAUD bit6:UART2_BAUD bit5:UART3_BAUD bit4:CAN0 bit3:CAN1 bit2:CAN2 bit1:CAN3 bit0:SPIM
 * ----------------------------------------------------------------------------
 * Moudle_RESET_REG1(0x3042c)  1:reset enable  0:reset disable
 *   bit18:LPC RESET ENABLE 1:yes 0:no     bit 16:CHIP RESET
 *   bit15:APB bit14:SYSCTL bit13:SM2 bit12:SM3 bit11:EFUSE bit10:PNP bit9:LDN bit8:SPIF
 *   bit7:H2E bit6:ICTL bit5:TIMER0 bit4:TIMER1 bit3:TIMER2 bit2:TIMER3 bit1:KBS bit0:PS2K
 * ------------------------------------------------------------------------- */
void Module_SoftReset(int reg_idx, int bit_no)
{
    /*--------------this is a module reset example-------------*/
    // Moudle_RESET_REG0 |= 0x3ff066e1;
    // Moudle_RESET_REG0 = 0x0;
    // Moudle_RESET_REG1 = 0x0; 
	if (reg_idx == 0)
	{
		Moudle_RESET_REG0 |= (1 << bit_no);
		Moudle_RESET_REG0 = 0x0;
	}
	else
	{
		Moudle_RESET_REG1 |= (1 << bit_no);
		Moudle_RESET_REG1 = 0x0;
	}
	
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   CPU_Sleep
 *
 * This is CPU Sleep Interface,If you call this function,cpu will sleep and 
 * it will wake up when interrupt occurs.
 *
 * ------------------------------------------------------------------------- */
void CPU_Sleep(void)
{
    asm volatile("wfi");
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Save_Context
 *
 * Before entering low power mode, save the necessary CPU status
 *
 *
 * ------------------------------------------------------------------------- */
void Save_Context(void)
{
	SYSCTL_MODEN0_CONTEXT = SYSCTL_MODEN0;
	SYSCTL_MODEN1_CONTEXT = SYSCTL_MODEN1;
	SYSCTL_PIO0_IECFG_CONTEXT = SYSCTL_PIO0_IECFG;
	SYSCTL_PIO1_IECFG_CONTEXT = SYSCTL_PIO1_IECFG;
	SYSCTL_PIO2_IECFG_CONTEXT = SYSCTL_PIO2_IECFG;
	SYSCTL_PIO3_IECFG_CONTEXT = SYSCTL_PIO3_IECFG;
	SYSCTL_PIO4_IECFG_CONTEXT = SYSCTL_PIO4_IECFG;
	SYSCTL_PIO5_IECFG_CONTEXT = SYSCTL_PIO5_IECFG;

	SYSCTL_PLLCFG_CONTEXT = SYSCTL_PLLCFG;

	// printf("BE:MODE0:%x,MODE1:%x\n",SYSCTL_MODEN0,SYSCTL_MODEN1);
	// printf("BE:IE0:%x,IE1:%x,IE2:%x,IE3:%x,IE4:%x,IE5:%x\n",SYSCTL_PIO0_IECFG,SYSCTL_PIO1_IECFG,SYSCTL_PIO2_IECFG,SYSCTL_PIO3_IECFG,SYSCTL_PIO4_IECFG,SYSCTL_PIO5_IECFG);
	// printf("BE:PLL:%x\n",SYSCTL_PLLCFG);
	// GPIO_Config(GPIOA, 0, 1, 0, 1, 1);
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Restore_Context
 *
 * Before exiting low power mode, restore the necessary CPU status
 *
 *
 * ------------------------------------------------------------------------- */
void Restore_Context(void)
{
	SYSCTL_MODEN0 = SYSCTL_MODEN0_CONTEXT;
	SYSCTL_MODEN1 = SYSCTL_MODEN1_CONTEXT;
	SYSCTL_PIO0_IECFG = SYSCTL_PIO0_IECFG_CONTEXT;
	SYSCTL_PIO1_IECFG = SYSCTL_PIO1_IECFG_CONTEXT;
	SYSCTL_PIO2_IECFG = SYSCTL_PIO2_IECFG_CONTEXT;
	SYSCTL_PIO3_IECFG = SYSCTL_PIO3_IECFG_CONTEXT;
	SYSCTL_PIO4_IECFG = SYSCTL_PIO4_IECFG_CONTEXT;
	SYSCTL_PIO5_IECFG = SYSCTL_PIO5_IECFG_CONTEXT;

	SYSCTL_PLLCFG = SYSCTL_PLLCFG_CONTEXT;

	// GPIO_Config(GPIOA, 0, 1, 1, 1, 1);
}

void CPU_Timer_Set()
{
	// stop timer
	write_csr(0xBDB, 0x1);	
	// enable cpu timer interrupt
	int32_t val = read_csr(0xBD1);
	write_csr(0xBD1, val | 0x2);
	// clear timer
	write_csr(0xBDA, 0x0);
	// set timer
	write_csr(0xBD9, 32768 / 2);
	// start timer
	write_csr(0xBDB, 0x0);	
}

void Low_Power_Set(void)
{
	SYSCTL_PIO0_IECFG = 0x0;
	// 0011 smbus3
	// SYSCTL_PIO1_IECFG = 0x3;
	SYSCTL_PIO1_IECFG = 0x0;
	SYSCTL_PIO2_IECFG = 0x10;
	SYSCTL_PIO3_IECFG = 0x0;
	SYSCTL_PIO4_IECFG = 0x0;

	#if SUPPORT_KBS_WAKEUP
		if(SystemIsS3) SYSCTL_PIO5_IECFG = 0xFFF0;
		else SYSCTL_PIO5_IECFG = 0x0;
	#else
	SYSCTL_PIO5_IECFG = 0x0;
	#endif

	// 0110 0000 0100 0000 1000 0000 0000 0000
	// SWUC
	#if SUPPORT_KBS_WAKEUP
		if(SystemIsS3) SYSCTL_MODEN0 = 0x10100000;
		else SYSCTL_MODEN0 = 0x100000;
	#else 
	SYSCTL_MODEN0 = 0x100000;//0x0;
	#endif
	// 1110 0001 1000
	// PB DRAM SYSCTL SRAM ICTL//SPIF
	SYSCTL_MODEN1 = 0xe0a;
	// 11111
	SYSCTL_PLLCFG |= (0x1F << 16);

	SYSCTL_PMUCSR &= 0xffefffff;

	#if SUPPORT_CPUTIMER_WAKEUP
	CPU_Timer_Set();
	#endif
}

void Enter_LowPower_Mode(void)
{
	printf("enter lowpower mode\n");
	Low_Power_Flag = 1;
	Save_Context();
	#if 0
	RTC_Deep_Sleep_Test();
	#else
	Low_Power_Set();
	#endif
	//printf("PIO2_IECFG:%x,MODEN0:%x,MODEN1:%x,PLLCFG%x,PMUCSR:%x\n",SYSCTL_PIO2_IECFG,SYSCTL_MODEN0,SYSCTL_MODEN1,SYSCTL_PLLCFG,SYSCTL_PMUCSR);
	CPU_Sleep();
}

void Exit_LowPower_Mode(void)
{
	Low_Power_Flag = 0;
	Restore_Context();
	printf("exit lowpower mode\n");
	// printf("AF:MODE0:%x,MODE1:%x\n",SYSCTL_MODEN0,SYSCTL_MODEN1);
	// printf("AF:IE0:%x,IE1:%x,IE2:%x,IE3:%x,IE4:%x,IE5:%x\n",SYSCTL_PIO0_IECFG,SYSCTL_PIO1_IECFG,SYSCTL_PIO2_IECFG,SYSCTL_PIO3_IECFG,SYSCTL_PIO4_IECFG,SYSCTL_PIO5_IECFG);
	// printf("AF:PLL:%x\n",SYSCTL_PLLCFG);
}
