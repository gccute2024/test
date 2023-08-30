/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-25 09:47:20
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

#ifndef AE_FUNC_H
#define AE_FUNC_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_GPIO.H"

/*************时钟模块控制宏*************/
#define SPIM_CLOCK_EN     1
#define CAN3_CLOCK_EN     1
#define CAN2_CLOCK_EN     1
#define CAN1_CLOCK_EN     1
#define CAN0_CLOCK_EN     1
#define UART3_CLOCK_EN    1
#define UART2_CLOCK_EN    1
#define UART1_CLOCK_EN    1
#define UART0_CLOCK_EN    1
#define UARTB_CLOCK_EN    1
#define UARTA_CLOCK_EN    1
#define PWM_CLOCK_EN      1
#define ADC_CLOCK_EN      1
#define PPORT_CLOCK_EN    1
#define WDT_CLOCK_EN      1
#define SMBUS5_CLOCK_EN   1
#define SMBUS4_CLOCK_EN   1
#define SMBUS3_CLOCK_EN   1
#define SMBUS2_CLOCK_EN   1
#define SMBUS1_CLOCK_EN   1
#define SMBUS0_CLOCK_EN   1
#define GPIO_CLOCK_EN     1
#define BRAM_CLOCK_EN     1
#define SWUC_CLOCK_EN     1
#define SHM_CLOCK_EN      1
#define PMCKBC_CLOCK_EN   1
#define PS2M_CLOCK_EN     1
#define PS2K_CLOCK_EN     1
#define CEC_CLOCK_EN     1
#define KBS_CLOCK_EN      1
#define TIMER3_CLOCK_EN   1
#define TIMER2_CLOCK_EN   1
#define TIMER1_CLOCK_EN   1
#define TIMER0_CLOCK_EN   1
#define ICTL_CLOCK_EN     1   //中断控制器模块使能控制
#define H2E_CLOCK_EN      1   //H2E模块使能控制
#define SPIF_CLOCK_EN     1
#define SRAM_CLOCK_EN     1
#define GPIODB_CLOCK_EN   1   //GPIO去抖使能
#define SYSCTL_CLOCK_EN   1
#define DRAM_CLOCK_EN     1
#define PB_CLOCK_EN       1  //E-P总线使能控制
/*************时钟模块控制宏*************/

#define TIMER_FREQ                32768*2
__BEGIN_DECLS

void enable_mcycle_minstret();
void disable_mcycle_minstret();

void pmp_open_all_space();

void switch_m2u_mode();

uint32_t get_mtime_freq();

uint32_t mtime_lo(void);

// uint32_t mtime_hi(void);

uint32_t get_mtime_value();

uint32_t get_instret_value();

uint32_t get_cycle_value();

uint32_t get_cpu_freq();

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n);

uint32_t get_timer_value();
uint32_t get_timer_freq();

void Module_Clock_Config(void);
void s0_low_power_set();

void spim_clock_en();
void can3_clock_en();
void can2_clock_en();
void can1_clock_en();
void can0_clock_en();
void uart3_clock_en();
void uart2_clock_en();
void uart1_clock_en();
void uart0_clock_en();
void uartb_clock_en();
void uarta_clock_en();
void pwm_clock_en();
void adc_clock_en();
void cec_clock_en();
void pport_clock_en();
void wdt_clock_en();
void smbus3_clock_en();
void smbus2_clock_en();
void smbus1_clock_en();
void smbus0_clock_en();
void gpio_clock_en();
void bram_clock_en();
void swuc_clock_en();
void shm_clock_en();
void pmckbc_clock_en();
void ps2m_clock_en();
void ps2k_clock_en();
void kbs_clock_en();
void timer3_clock_en();
void timer2_clock_en();
void timer1_clock_en();
void timer0_clock_en();
void ictl_clock_en();
void h2e_clock_en();
void spif_clock_en();
void sram_clock_en();
void gpiodb_clock_en();
void sysctl_clock_en();
void dram_clock_en();
void pb_clock_en();
///////////////////////////////////////////////////////////////////


// void irqc_set_intctrl (uint32_t source, uint8_t intctrl);
// uint8_t irqc_get_intctrl  (uint32_t source);

// void irqc_set_intattr (uint32_t source, uint8_t intattr);
// uint8_t irqc_get_intattr  (uint32_t source);

// void irqc_set_cliccfg (uint8_t cliccfg);
// uint8_t irqc_get_cliccfg ();

// void irqc_set_mth (uint8_t mth);
// uint8_t irqc_get_mth();

// //sets nlbits 
// void irqc_set_nlbits(uint8_t nlbits);


// //get nlbits 
// uint8_t irqc_get_nlbits();

// void irqc_set_irq_lvl(uint32_t source, uint8_t lvl);
// uint8_t irqc_get_irq_lvl(uint32_t source);

// void irqc_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs);
// uint8_t irqc_get_irq_lvl_abs(uint32_t source);

// void irqc_mode_enable();

// void irqc_set_vmode(uint32_t source);
// void irqc_set_nonvmode(uint32_t source);



__END_DECLS

#endif
