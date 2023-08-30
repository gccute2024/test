/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-12 19:02:25
 * @Description: This file is used for soc module initiliazation and configuration
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

#include "TEST_SOC_FUNC.H"
#if TEST
void soc_init(void)
{
	(*((VDWORDP)0x304c8)) |= 0x1cffc;

	(*((volatile unsigned int *)0x30408)) = 0x3fffffff;//enable all logical device
  
	SYSCTL_PLLCFG = 0x40800000 | ((CHIP_CLOCK_SWITCH-1)<<16);
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

#if CAN_INIT
	sysctl_iomux_can();
	/*can*/
	//波特率为125kbs 验证id为0x123 掩码0xffff8000
	CAN0_Init(_125kbs,0x123,0xffff8000);
	CAN1_Init(_125kbs,0x123,0xffff8000);
	CAN2_Init(_125kbs,0x123,0xffff8000);
	CAN3_Init(_125kbs,0x123,0xffff8000);
	#if CAN0_Receive
	Can_Int_Enable(CAN0,rie);
	#endif
	#if CAN1_Receive
	Can_Int_Enable(CAN1,rie);
	#endif
	#if CAN2_Receive
	Can_Int_Enable(CAN2,rie);
	#endif
	#if CAN3_Receive
	Can_Int_Enable(CAN3,rie);
	#endif
	dprint("Can init done\n");
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
  	SYSCTL_CLKDIV_TMR0 = 0;  //32768/101  	(*(BYTEP )(0x30440)) = 0;  //32768/10
	SYSCTL_CLKDIV_TMR1 =0x0;  //timer1 时钟分频设置 32.768k/2 
  	SYSCTL_CLKDIV_TMR2 =0x0;  //timer2 时钟分频设置 24.576Mhz/2
	SYSCTL_CLKDIV_TMR3 =0X0;
#endif

    //TIMER_Init(0x0,TIMER0_1s,0x1,0x0);	// 32768/2
    //TIMER_Init(0x1,TIMER1_20ms,0x1,0x0); //1s

	#if (defined(TEST101))
    TIMER_Init(0x2, CPU_FREQ / 1000, 0x1, 0x0);
	#elif (defined(AE101)||defined(AE102)||defined(AE103))
	TIMER_Init(0x2, CPU_FREQ / 1000 / 2, 0x1, 0x0);
	#endif


    TIMER_Init(0x3,0x5678,0x1,0x0);//2.4MHz
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
#if LPC_INIT
	LPC_MON_CNT=0;
	LPC_SOF=0;
	LPC_EOF=0;
	Lpc_To_Ram_Config(); 
#endif
	#if	!(PB_CLOCK_EN)
	pb_clock_en();
	dprint("APB CLOCK NOT ENABLE\n");
#endif
	dprint("End init \n");
}
#endif