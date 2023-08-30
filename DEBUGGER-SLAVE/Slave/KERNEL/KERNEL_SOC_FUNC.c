/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-10 17:27:50
 * @Description: This is about the  national crypto algorithm implementation
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

#include "KERNEL_SOC_FUNC.H"
#include "AE_FUNC.H"
#include "AE_CONFIG.H"

void udelay(DWORD delay)
{
	do{
		delay--;
	}while(delay);
}

int pmc1_init(void)
{ 
	sysctl_iomux_pmc();

	PMC1_IE =  0x3f;
#if (defined(AE101)||defined(AE102)||defined(AE103))
	KBC_CTL =  KBC_CTL | PM1_HOST_INT_ENABLE ;
#else
	SWUCTL1 =  SWUCTL1 | PM1_HOST_INT_ENABLE ;
#endif
	return 0;
}


int pmc2_init(void)
{ 
	PMC2_IE =  0x3f;
	PMC2_CTL = (PMC2_CTL | IBF_INT_ENABLE);  //Enable int
	return 0;
}

int pmc3_init(void)
{ 
	PMC3_IE =  0x3f;
	PMC3_CTL = (PMC3_CTL | IBF_INT_ENABLE);  //Enable int
	return 0;
}


int pmc4_init(void)
{ 
	PMC4_IE =  0x3f;
	PMC4_CTL = (PMC4_CTL | IBF_INT_ENABLE);  //Enable int
	return 0;
}


int pmc5_init(void)
{ 
	PMC5_IE =  0x3f;
	PMC5_CTL = (PMC5_CTL | IBF_INT_ENABLE);  //Enable int
	return 0;
}


int kbc_init(void)
{
	SET_BIT(KBC_CTL,3); //set KBC_IBF enable
	return 0;
}

void Init_ClearRam(void)
{
  register int i = 0;
  for( i = 0 ; i < 0x800 ;i ++)
	(*(BYTEP )(DRAM_BASE_ADDR + i)) = 0;
}

void __weak soc_init(void)
{
	// This function need to be added by user to initialize their SoC
	/*Logical device SERIRQ interrupts polarity configuration*/
	//(*((VDWORDP)0x304c8)) |= 0x1dffe;
	(*((VDWORDP)0x304c8)) |= 0x1cffc;

	(*((volatile unsigned int *)0x30408)) = 0x3fffffff;//enable all logical device
	#if defined(AE103)
		#if (OSC_CLOCK_SWITCH==1)
		(*(VDWORDP)0x304D4)&=~BIT0;//切换外部
		(*(VDWORDP)0x304D4)&=~BIT1;//关闭内部osc192M（低功耗考虑）
		SYSCTL_PLLCFG = 0x40800000 | ((CHIP_CLOCK_SWITCH-1)<<16);//PLL分频
		#else
		(*(VDWORDP)0x304D4)|= 1;//切换内部时钟
		REG32(0x304CC)= (CHIP_CLOCK_SWITCH-1);	
		#endif
  	#else
	SYSCTL_PLLCFG = 0x40800000 | ((CHIP_CLOCK_SWITCH-1)<<16);
	#endif
#if (SYSCTL_CLOCK_EN)
	SYSCTL_HDEVEN = 0x3fffffff;
#endif
  	get_cpu_freq();
	//dprint
#if UART_MODULE_EN
	#ifdef UART0_BAUD
	#if (UART0_CLOCK_EN)
		uart0_clock_en();
		sysctl_iomux_uart0();
		serial_init(0,UART0_BAUD);
	#endif
	#endif
#if (defined(AE102)||defined(AE103))
	#ifdef UART1_BAUD
	#if (UART1_CLOCK_EN)
		uart1_clock_en();
		sysctl_iomux_uart1();
		serial_init(1,UART1_BAUD);
	#endif
	#endif
	#ifdef UART2_BAUD
	#if (UART2_CLOCK_EN)
		uart2_clock_en();
		sysctl_iomux_uart2();
		serial_init(2,UART2_BAUD);
	#endif
	#endif
	#ifdef UART3_BAUD
	#if (UART3_CLOCK_EN)
		uart3_clock_en();
		sysctl_iomux_uart3();
		serial_init(3,UART3_BAUD);
	#endif
	#endif
#endif
	#ifdef UARTA_BAUD
	#if (UARTA_CLOCK_EN)
		uarta_clock_en();
		sysctl_iomux_uarta();
		serial_init(4,UARTA_BAUD);
	#endif
	#endif
	#ifdef UARTB_BAUD
	#if (UARTB_CLOCK_EN)
		uartb_clock_en();
		sysctl_iomux_uartb();
		serial_init(5,UARTB_BAUD);
	#endif
	#endif
#endif

#if IRQC_MODULE_EN
	ictl_clock_en();
	soc_irq_init();
	dprint("Soc_irq_init done.\n");
#endif

#if I2C_MODULE_EN
	smbus0_clock_en();
	smbus1_clock_en();
	smbus2_clock_en();
	smbus3_clock_en();
	sysctl_iomux_i2c();
	i2c_pull_up();
	I2c_Channel_Init();
	dprint("I2c_channel_init done.\n");
#endif
	
#if ADC_MODULE_EN
	adc_clock_en();
	sysctl_iomux_adc();
#endif
/*//////////////
///attention the ps2 must be disable before using the cec
*///////////////
#if (CEC_MODULE_EN&CEC_CLOCK_EN&(!PS2_MODULE_EN))
	cec_clock_en();
	sysctl_iomux_cec();
	cec_pull_up();
	#if CEC_mode_select
		CEC_initiator_init();
	#else
		CEC_follower_init();
	#endif
#endif

#if (PS2_MODULE_EN&PS2M_CLOCK_EN&PS2K_CLOCK_EN)
	ps2m_clock_en();
	ps2k_clock_en();
	sysctl_iomux_ps2();
	ps2_pull_up();
	//PS2_Init();
	dprint("Ps2_init done.\n");
#endif
	
#if (KBS_MODULE_EN&KBS_CLOCK_EN)
	kbs_clock_en();
	KBS_Init();
	dprint("Kbd init done.\n");
#endif

#if SPI_MODULE_EN
	spim_clock_en();
	sysctl_iomux_spim();
	SPI_Init();
	dprint("Spi init done.\n");
#endif

#if WDT_MODULE_EN
	/*wdt*/
	wdt_clock_en();
	WDT_Init(0x1,0xa);  //FIXME
	dprint("watchdong init done.\n");
#endif


#if (PWM_MODULE_EN&PWM_CLOCK_EN)
	pwm_clock_en();
	sysctl_iomux_pwm();
	PWM_Init(0, 0x5);
	dprint("Pwm_init done\n");
#endif

#if (TACH_MODULE_EN&PWM_CLOCK_EN)
	sysctl_iomux_tach();
	tach_pull_up();
	TACH_Init();
	dprint("After TACH_Init \n");
#endif


#if TIMER_MODULE_EN
#if (SYSCTL_CLOCK_EN)
	timer0_clock_en();
	timer1_clock_en();
	timer2_clock_en();
	timer3_clock_en();
  	SYSCTL_CLKDIV_TMR0 = TIMER0_freq;  //32768/101  	(*(BYTEP )(0x30440)) = 0;  //32768/10
	SYSCTL_CLKDIV_TMR1 =TIMER1_freq;  //timer1 时钟分频设置 32.768k/2 
  	SYSCTL_CLKDIV_TMR2 =TIMER2_freq;  //timer2 时钟分频设置 24.576Mhz/2
	SYSCTL_CLKDIV_TMR3 =TIMER3_freq;
#endif

    //TIMER_Init(TIMER0,TIMER0_1s,0x1,0x0);	// 32768/2
    //TIMER_Init(TIMER1,TIMER1_20ms,0x1,0x0); //1s

	#if (defined(TEST101))
    TIMER_Init(TIMER2, CPU_FREQ / 1000, 0x1, 0x0);
	#elif (defined(AE101)||defined(AE102)||defined(AE103))
	TIMER_Init(TIMER2, CPU_FREQ / 1000 / 2, 0x1, 0x0);
	#endif


    TIMER_Init(TIMER3,0x5678,0x1,0x0);//2.4MHz
	dprint("Timer init done\n");
#endif

#if (KBC_MODULE_EN&PMCKBC_CLOCK_EN)
	pmckbc_clock_en();
	kbc_init();
	dprint("kbc init done.\n");
#endif

#if (PMC_MODULE_EN&PMCKBC_CLOCK_EN)
	pmckbc_clock_en();
	pmc1_init();
	pmc2_init();
	pmc3_init();
	pmc4_init();
	pmc5_init();
	dprint("pmc init done.\n");
#endif

#if (ADC_MODULE_EN &ADC_CLOCK_EN)
	adc_clock_en();
	ADC_Init(0,0,0);

	dprint("ADC init done \n");
#endif

#if	!(PB_CLOCK_EN)
	pb_clock_en();
	dprint("APB CLOCK NOT ENABLE\n");
#endif
	dprint("End init \n");
}


void chaoyue_pull_up(void)
{
	#if defined(AE101)||defined(AE102)
	//A5
	GPIO_Pullup_Config(GPIOA,5);
	//C1 C2 
	GPIO_Pullup_Config(GPIOA,17);
	GPIO_Pullup_Config(GPIOA,18);
	//H1(G5) H2(G6)
	GPIO_Pullup_Config(GPIOB,21);
	GPIO_Pullup_Config(GPIOB,22);
	// select smclk 
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_PIO5_UDCFG |= ((1 << 7)) ;
	#endif
	#endif
}

void i2c_pull_up(void)
{
	#if (defined(TEST101)||defined(AE101)||defined(AE102))
		#if SMBUS0_CLOCK_EN
		GPIO_Pullup_Config(GPIOA,11);
		GPIO_Pullup_Config(GPIOA,12);
		#endif
		#if SMBUS1_CLOCK_EN 
		GPIO_Pullup_Config(GPIOA,17);
		GPIO_Pullup_Config(GPIOA,18);
		#endif
		#if SMBUS2_CLOCK_EN
		GPIO_Pullup_Config(GPIOB,14);
		GPIO_Pullup_Config(GPIOB,15);
		#endif
		#if SMBUS3_CLOCK_EN
		GPIO_Pullup_Config(GPIOB,21);
		GPIO_Pullup_Config(GPIOB,22);
		#endif
	// select smclk 
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_PIO5_UDCFG |= ((1 << 7)) ;
	#endif
	#endif
	#if (defined(AE103))
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_PIO0_UDCFG |= ((1 << 18)|(1 << 17)|(1 << 11)|(1 << 12));
	SYSCTL_PIO1_UDCFG |= ((1 << 14)|(1 << 15)|(1 << 25)|(1 << 26));
	#endif
	#endif
}

void ps2_pull_up(void)
{
	//SYSCTL_PIO2_UDCFG  |= (1 << 8)|(1 << 9) ;
	SYSCTL_PIO2_UDCFG  |= (1 << 8)|(1 << 9) | (1 << 12) | (1 << 13) ;
}

void cec_pull_up(void)
{
	SYSCTL_PIO2_UDCFG  |= (1 << 8);//pull up
}

void tach_pull_up(void)
{
	#if (defined(AE103))
	GPIO_Pullup_Config(GPIOA,30);//gpd6,d7 pull up
	GPIO_Pullup_Config(GPIOA,31);
	GPIO_Pullup_Config(GPIOC,10);
	GPIO_Pullup_Config(GPIOC,11);
	#endif
}



