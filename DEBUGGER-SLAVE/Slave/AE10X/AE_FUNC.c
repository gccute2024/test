/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-27 12:22:58
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

#include <AE_FUNC.H>


    // Configure PMP to make all the address space accesable and executable
void pmp_open_all_space(){
    // Config entry0 addr to all 1s to make the range cover all space
    asm volatile ("li x6, 0xffffffff":::"x6");
    asm volatile ("csrw pmpaddr0, x6":::);
    // Config entry0 cfg to make it NAPOT address mode, and R/W/X okay
    asm volatile ("li x6, 0x7f":::"x6");
    asm volatile ("csrw pmpcfg0, x6":::);
}

void switch_m2u_mode(){
    clear_csr (mstatus,MSTATUS_MPP);
    asm volatile ("la x6, 1f    ":::"x6");
    asm volatile ("csrw mepc, x6":::);
    asm volatile ("mret":::);
    asm volatile ("1:":::);
} 

uint32_t mtime_lo(void)
{
  return read_csr(0xBDA);
}


uint32_t get_timer_value()
{
    return mtime_lo();
}

uint32_t get_timer_freq()
{
  return TIMER_FREQ;
}

uint32_t get_instret_value()
{
    return read_csr(minstret);;
}

uint32_t get_cycle_value()
{
    return read_csr(mcycle);
}

uint32_t __attribute__((noinline)) measure_cpu_freq(register size_t n)
{
  enable_mcycle_minstret();
  register uint32_t start_mtime, delta_mtime;
  register uint32_t mtime_freq = get_timer_freq();

  // Don't start measuruing until we see an mtime tick
  register uint32_t tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  uint32_t start_mcycle = read_csr(mcycle);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < n);

  register uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle;
disable_mcycle_minstret() ;
  return (delta_mcycle / delta_mtime) * mtime_freq
         + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq()
{

  // warm up
  measure_cpu_freq(1);
  // measure for real
  CPU_FREQ = measure_cpu_freq(100);

  return CPU_FREQ;
}

/**
 * @brief 对模块时钟的开关
 *
 * @param 无
 *
 * @return 无
 *
 */
void Module_Clock_Config(void)
{
  #if SPIM_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<0);
  #else
  SYSCTL_MODEN0&=~(0x1<<0);
  #endif
#if (defined(AE102)||defined(AE103))
  #if CAN3_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<1);
  #else
  SYSCTL_MODEN0&=~(0x1<<1);
  #endif

  #if CAN2_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<2);
  #else
  SYSCTL_MODEN0&=~(0x1<<2);
  #endif

  #if CAN1_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<3);
  #else
  SYSCTL_MODEN0&=~(0x1<<3);
  #endif

  #if CAN0_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<4);
  #else
  SYSCTL_MODEN0&=~(0x1<<4);
  #endif

  #if UART3_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<5);
  #else
  SYSCTL_MODEN0&=~(0x1<<5);
  #endif

  #if UART2_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<6);
  #else
  SYSCTL_MODEN0&=~(0x1<<6);
  #endif

  #if UART1_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<7);
  #else
  SYSCTL_MODEN0&=~(0x1<<7);
  #endif
#endif

  #if UART0_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<8);
  #else
  SYSCTL_MODEN0&=~(0x1<<8);
  #endif

  #if UARTB_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<9);
  #else
  SYSCTL_MODEN0&=~(0x1<<9);
  #endif

  #if UARTA_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<10);
  #else
  SYSCTL_MODEN0&=~(0x1<<10);
  #endif

  #if PWM_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<11);
  #else
  SYSCTL_MODEN0&=~(0x1<<11);
  #endif

  #if ADC_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<12);
  #else
  SYSCTL_MODEN0&=~(0x1<<12);
  #endif
#if (defined(AE102)||defined(AE103))
  #if PPORT_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<14);
  #else
  SYSCTL_MODEN0&=~(0x1<<14);
  #endif
#endif
  #if WDT_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<15);
  #else
  SYSCTL_MODEN0&=~(0x1<<15);
  #endif

  #if SMBUS3_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<16);
  #else
  SYSCTL_MODEN0&=~(0x1<<16);
  #endif

  #if SMBUS2_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<17);
  #else
  SYSCTL_MODEN0&=~(0x1<<17);
  #endif

  #if SMBUS1_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<18);
  #else
  SYSCTL_MODEN0&=~(0x1<<18);
  #endif

  #if SMBUS0_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<19);
  #else
  SYSCTL_MODEN0&=~(0x1<<19);
  #endif

  #if GPIO_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<20);
  #else
  SYSCTL_MODEN0&=~(0x1<<20);
  #endif

  #if BRAM_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<21);
  #else
  SYSCTL_MODEN0&=~(0x1<<21);
  #endif

  #if SWUC_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<22);
  #else
  SYSCTL_MODEN0&=~(0x1<<22);
  #endif

  #if SHM_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<23);
  #else
  SYSCTL_MODEN0&=~(0x1<<23);
  #endif

  #if PMCKBC_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<24);
  #else
  SYSCTL_MODEN0&=~(0x1<<24);
  #endif

  #if PS2M_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<26);
  #else
  SYSCTL_MODEN0&=~(0x1<<26);
  #endif

  #if PS2K_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<27);
  #else
  SYSCTL_MODEN0&=~(0x1<<27);
  #endif

  #if KBS_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<28);
  #else
  SYSCTL_MODEN0&=~(0x1<<28);
  #endif

  #if TIMER3_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<29);
  #else
  SYSCTL_MODEN0&=~(0x1<<29);
  #endif

  #if TIMER2_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<30);
  #else
  SYSCTL_MODEN0&=~(0x1<<30);
  #endif

  #if TIMER1_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN0|=(0x1<<31);
  #else
  SYSCTL_MODEN0&=~(0x1<<31);
  #endif

  #if TIMER0_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<0);
  #else
  SYSCTL_MODEN1&=~(0x1<<0);
  #endif

  #if ICTL_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<1);
  #else
  SYSCTL_MODEN1&=~(0x1<<1);
  #endif

  #if H2E_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<2);
  #else
  SYSCTL_MODEN1&=~(0x1<<2);
  #endif

  #if SPIF_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<3);
  #else
  SYSCTL_MODEN1&=~(0x1<<3);
  #endif

  #if SRAM_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<4);
  #else
  SYSCTL_MODEN1&=~(0x1<<4);
  #endif

  #if GPIODB_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<5);
  #else
  SYSCTL_MODEN1&=~(0x1<<5);
  #endif

  #if SYSCTL_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<9);
  #else
  SYSCTL_MODEN1&=~(0x1<<9);
  #endif

  #if DRAM_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<10);
  #else
  SYSCTL_MODEN1&=~(0x1<<10);
  #endif

  #if PB_CLOCK_EN & SYSCTL_CLOCK_EN
  SYSCTL_MODEN1|=(0x1<<11);
  #else
  SYSCTL_MODEN1&=~(0x1<<11);
  #endif
}

void close_unnecessary_clocks()
{
	// SPIM
	SYSCTL_MODEN0 &= ~(0x1 << 0);
#if (defined(AE102)||defined(AE103))
	// CAN3
	SYSCTL_MODEN0 &= ~(0x1 << 1);
	// CAN2
	SYSCTL_MODEN0 &= ~(0x1 << 2);
	// CAN1
	SYSCTL_MODEN0 &= ~(0x1 << 3);
	// CAN0
	SYSCTL_MODEN0 &= ~(0x1 << 4);
	// UART3
	SYSCTL_MODEN0 &= ~(0x1 << 5);
	// UART2
	SYSCTL_MODEN0 &= ~(0x1 << 6);
	// UART1
	SYSCTL_MODEN0 &= ~(0x1 << 7);
#endif
	// UART0
	SYSCTL_MODEN0 &= ~(0x1 << 8);
	// UARTB
	SYSCTL_MODEN0 &= ~(0x1 << 9);
	// UARTA
	SYSCTL_MODEN0 &= ~(0x1 << 10);
	// PWM
	SYSCTL_MODEN0 &= ~(0x1 << 11);
	// ADC
	SYSCTL_MODEN0 &= ~(0x1 << 12);
#if (defined(AE102)||defined(AE103))
	// PPORT
	SYSCTL_MODEN0 &= ~(0x1 << 14);
#endif
	// WDT
	SYSCTL_MODEN0 &= ~(0x1 << 15);
	// SMBUS3
	SYSCTL_MODEN0 &= ~(0x1 << 16);
	// SMBUS2
	SYSCTL_MODEN0 &= ~(0x1 << 17);
	// SMBUS1
	SYSCTL_MODEN0 &= ~(0x1 << 18);
	// SMBUS0
	SYSCTL_MODEN0 &= ~(0x1 << 19);
	// GPIO
	// SYSCTL_MODEN0 &= ~(0x1 << 20);
	// BRAM
	SYSCTL_MODEN0 &= ~(0x1 << 21);
	// SWUC
	SYSCTL_MODEN0 &= ~(0x1 << 22);
	// SHM
	SYSCTL_MODEN0 &= ~(0x1 << 23);
	// PMCKBC
	SYSCTL_MODEN0 &= ~(0x1 << 24);
	// PS2M
	SYSCTL_MODEN0 &= ~(0x1 << 26);
	// PS2K
	SYSCTL_MODEN0 &= ~(0x1 << 27);
	// KBS
	SYSCTL_MODEN0 &= ~(0x1 << 28);
	// TIMER3
	SYSCTL_MODEN0 &= ~(0x1 << 29);
	// TIMER2
	SYSCTL_MODEN0 &= ~(0x1 << 30);
	// TIMER1
	SYSCTL_MODEN0 &= ~(0x1 << 31);
	// TIMER0
	SYSCTL_MODEN1 &= ~(0x1 << 0);
	// ICTL is necessary
	// SYSCTL_MODEN1&=~(0x1<<1);
	// H2E is necessary
	// SYSCTL_MODEN1&=~(0x1<<2);
	// SPIF is necessary
	// SYSCTL_MODEN1&=~(0x1<<3);
	// SRAM
	// SYSCTL_MODEN1 &= ~(0x1 << 4);
	// GPIODB
	// SYSCTL_MODEN1 &= ~(0x1 << 5);
	// SYSCTL is necessary
	// SYSCTL_MODEN1&=~(0x1<<9);
	// DRAM is necessary
	// SYSCTL_MODEN1&=~(0x1<<10);
	// PB is necessary
	// SYSCTL_MODEN1&=~(0x1<<11);
}

void disable_gpio_input(int val, int GPIO, int idx, int lens)
{
	for (int i = 0; i <= lens; i++)
	{
		int bit = ((val & (1 << i)) >> i);
		if (bit)
		{
			GPIO_Input_EN(GPIO, idx + i, DISABLE);
		}
		else
		{
			GPIO_Input_EN(GPIO, idx + i, ENABLE);
		}
	}
}

void disable_unnecessary_gpio_input()
{
	// SYSCTL_PIO0_IECFG = 0;
	// SYSCTL_PIO1_IECFG = 0;
	// SYSCTL_PIO2_IECFG = 0x10;
	// SYSCTL_PIO3_IECFG = 0;
	// SYSCTL_PIO4_IECFG = 0;
	// // 1000 0000
	// SYSCTL_PIO5_IECFG = 0xffffff00;
    disable_gpio_input(PinA0_7_InOut, GPIOA, 0, 7);
    disable_gpio_input(PinA8_15_InOut, GPIOA, 8, 15);
    disable_gpio_input(PinA16_23_InOut, GPIOA, 16, 23);
    disable_gpio_input(PinA24_31_InOut, GPIOA, 24, 31);
    disable_gpio_input(PinB0_7_InOut, GPIOB, 0, 7);
    disable_gpio_input(PinB8_15_InOut, GPIOB, 8, 15);
    disable_gpio_input(PinB16_23_InOut, GPIOB, 16, 23);
    disable_gpio_input(PinB24_26_InOut, GPIOB, 24, 26);
    disable_gpio_input(PinC0_7_InOut, GPIOC, 0, 7);
    disable_gpio_input(PinC8_13_InOut, GPIOC, 8, 13);
    disable_gpio_input(PinD0_6_InOut, GPIOD, 0, 6);
    disable_gpio_input(PinE0_7_InOut, GPIOE, 0, 7);
    disable_gpio_input(PinE8_15_InOut, GPIOE, 8, 15);
    disable_gpio_input(PinE16_23_InOut, GPIOE, 16, 23);
}

void s0_low_power_set()
{
	close_unnecessary_clocks();
	disable_unnecessary_gpio_input();
}

void spim_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 0);
}
void can3_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 1);
}
void can2_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 2);
}
void can1_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 3);
}
void can0_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 4);
}
void uart3_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 5);
}
void uart2_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 6);
}
void uart1_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 7);
}
void uart0_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 8);
}
void uartb_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 9);
}
void uarta_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 10);
}
void pwm_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 11);
}
void adc_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 12);
}
void cec_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 13);
}
void pport_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 14);
}
void wdt_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 15);
}
void smbus3_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 16);
}
void smbus2_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 17);
}
void smbus1_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 18);
}
void smbus0_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 19);
}
void gpio_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 20);
}
void bram_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 21);
}
void swuc_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 22);
}
void shm_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 23);
}
void pmckbc_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 24);
}
void ps2m_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 26);
}
void ps2k_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 27);
}
void kbs_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 28);
}
void timer3_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 29);
}
void timer2_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 30);
}
void timer1_clock_en()
{
	SYSCTL_MODEN0 |= (0x1 << 31);
}
void timer0_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 0);
}
void ictl_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 1);
}
void h2e_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 2);
}
void spif_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 3);
}
void sram_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 4);
}
void gpiodb_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 5);
}
void sysctl_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 9);
}
void dram_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 10);
}
void pb_clock_en()
{
	SYSCTL_MODEN1 |= (0x1 << 11);
}
//Debugger CPU Int Function
void CPU_Int_Enable(BYTE  type)
{
	uint32_t value = read_csr(0xBD1);
	write_csr(0xBD1,(value |= (0x1<<type)));
}

void CPU_Int_Disable(BYTE  type)
{
	uint32_t value = read_csr(0xBD1);
	write_csr(0xBD1,(value &= ~(0x1<<type)));
}

BYTE  CPU_Int_Enable_Read(BYTE  type)
{
	uint32_t value = read_csr(0xBD1);
	return ((value & (0x1<type)) != 0);
}

void CPU_Int_Type_Edge(BYTE  type)
{
	uint32_t value1,value2;
	value1 = read_csr(0xBD2);
	value2 = read_csr(0xBD3);
	write_csr(0xBD2,(value1 &= ~(0x1<<type)));
	write_csr(0xBD3,(value2 |= (0x1<<type)));
}

void CPU_Int_Type_Level(BYTE  type)
{
	uint32_t value1,value2;
	value1 = read_csr(0xBD2);
	value2 = read_csr(0xBD3);
	write_csr(0xBD2,(value1 |= (0x1<<type)));
	write_csr(0xBD3,(value2 &= ~(0x1<<type)));
}

BYTE  CPU_Int_Type_Read(BYTE  type)
{
	uint32_t value1,value2;
	value1 = (read_csr(0xBD2) & (0x1<type));
	value2 = (read_csr(0xBD3) & (0x1<type));

	if(value1 != 0 && value2 == 0)
	{
		return 0;
	}
	else if(value1 == 0 && value2 != 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

