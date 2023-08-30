/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-25 18:15:44
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

#ifndef _KERNEL_INTC_H
#define _KERNEL_INTC_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

/*--------------------------------------------------------------------------*/

/*--------------------INTC1 Register Address definition---------------------*/

/*--------------------------------------------------------------------------*/

void core_wfe();
void wfe();

#define SWUC_OFFSET 0
#define GPE4_OFFSET 2
#define PS2_OFFSET 3
#define KBS_SDV_OFFSET 4
#define KBS_PRESS_OFFSET 5
#define TACH0_OFFSET 6
#define KBC_IBF_OFFSET 10
#define KBC_OBE_OFFSET 11
#define PMC1_IBF_OFFSET 12
#define PMC1_OBE_OFFSET 13
#define PMC2_IBF_OFFSET 14
#define PMC2_OBE_OFFSET 15
#define SM_EC_OFFSET 21       
#define SM_HOST_OFFSET 22     
#define TIMER0_OFFSET 23
#define TIMER1_OFFSET 24
#define TIMER2_OFFSET 25
#define TIMER3_OFFSET 26
#define INTR0_OFFSET 27
#define INTR1_OFFSET 28

#define IRQ_INT_DEVICE_LEFT_SHIFIT   3
#define IRQC_INT_DEVICE_SWUC        (IRQ_INT_DEVICE_LEFT_SHIFIT + SWUC_OFFSET)     
#define IRQC_INT_DEVICE_GPE4        (IRQ_INT_DEVICE_LEFT_SHIFIT + GPE4_OFFSET)          
#define IRQC_INT_DEVICE_PS2         (IRQ_INT_DEVICE_LEFT_SHIFIT + PS2_OFFSET)
#define IRQC_INT_DEVICE_KBS_SDV     (IRQ_INT_DEVICE_LEFT_SHIFIT + KBS_SDV_OFFSET)
#define IRQC_INT_DEVICE_KBS_PRESS   (IRQ_INT_DEVICE_LEFT_SHIFIT + KBS_PRESS_OFFSET)
#define IRQC_INT_DEVICE_TACH0       (IRQ_INT_DEVICE_LEFT_SHIFIT + TACH0_OFFSET)
#define IRQC_INT_DEVICE_TIMER0      (IRQ_INT_DEVICE_LEFT_SHIFIT + TIMER0_OFFSET)
#define IRQC_INT_DEVICE_TIMER1      (IRQ_INT_DEVICE_LEFT_SHIFIT + TIMER1_OFFSET)
#define IRQC_INT_DEVICE_TIMER2      (IRQ_INT_DEVICE_LEFT_SHIFIT + TIMER2_OFFSET)
#define IRQC_INT_DEVICE_TIMER3      (IRQ_INT_DEVICE_LEFT_SHIFIT + TIMER3_OFFSET)
#define IRQC_INT_DEVICE_KBC_IBF     (IRQ_INT_DEVICE_LEFT_SHIFIT + KBC_IBF_OFFSET)
#define IRQC_INT_DEVICE_KBC_OBE     (IRQ_INT_DEVICE_LEFT_SHIFIT + KBC_OBE_OFFSET)
#define IRQC_INT_DEVICE_PMC1_IBF    (IRQ_INT_DEVICE_LEFT_SHIFIT + PMC1_IBF_OFFSET)
#define IRQC_INT_DEVICE_PMC1_OBE    (IRQ_INT_DEVICE_LEFT_SHIFIT + PMC1_OBE_OFFSET)
#define IRQC_INT_DEVICE_PMC2_IBF    (IRQ_INT_DEVICE_LEFT_SHIFIT + PMC2_IBF_OFFSET)
#define IRQC_INT_DEVICE_PMC2_OBE    (IRQ_INT_DEVICE_LEFT_SHIFIT + PMC2_OBE_OFFSET)
#define IRQC_INT_DEVICE_INTR0       (IRQ_INT_DEVICE_LEFT_SHIFIT + INTR0_OFFSET)
#define IRQC_INT_DEVICE_INTR1       (IRQ_INT_DEVICE_LEFT_SHIFIT + INTR1_OFFSET)
#ifdef AE103
#define IRQC_INT_DEVICE_WU42 IRQC_INT_DEVICE_PMC2_IBF
#define IRQC_INT_DEVICE_RTC IRQC_INT_DEVICE_PMC2_OBE
#endif
/*IRQC relevant functions*/

extern void disable_irq(int index,int num);
extern void enable_irq(int index,int num);
extern void soc_irq_init();
extern void config_irqc_irqs (void);

extern void irqc_init ( uint32_t num_irq );

extern void irqc_enable_interrupt (uint32_t source);
extern void irqc_disable_interrupt (uint32_t source);

extern void irqc_set_pending(uint32_t source);
extern void irqc_clear_pending(uint32_t source);

extern void irqc_set_level_trig(uint32_t source);
extern void irqc_set_posedge_trig(uint32_t source);
extern void irqc_set_negedge_trig(uint32_t source);
/*----------------------------------LINE--------------------------------*/

#define IRQ_REG_INTEN_L            0x00//IRQ interrupt source enable register (low)
#define IRQ_REG_INTEN_H            0x04//IRQ interrupt source enable register (high)
#define IRQ_REG_INTMASK_L          0x08//IRQ interrupt source mask register (low).
#define IRQ_REG_INTMASK_H          0x0c//IRQ interrupt source mask register (high).
#define IRQ_REG_INTFORCE_L         0x14//IRQ interrupt force register
#define IRQ_REG_INTFORCE_H         0x14//
#define IRQ_REG_RAWSTATUS_L        0x18//IRQ raw status register
#define IRQ_REG_RAWSTATUS_H        0x1c//
#define IRQ_REG_STATUS_L           0x20//IRQ status register
#define IRQ_REG_STATUS_H           0x24//
#define IRQ_REG_MASKSTATUS_L       0x28//IRQ interrupt mask status register
#define IRQ_REG_MASKSTATUS_H       0x2c//
#define IRQ_REG_FINALSTATUS_L      0x30//IRQ interrupt final status
#define IRQ_REG_FINALSTATUS_H      0x34 
#define FIQ_REG_INTEN              0xc0//Fast interrupt enable register
#define FIQ_REG_INTMASK            0xc4//Fast interrupt mask register
#define FIQ_REG_INTFORCE           0xc8//Fast interrupt force register
#define FIQ_REG_RAWSTATUS          0xcc//Fast interrupt source raw status register
#define FIQ_REG_STATUS             0xd0//Fast interrupt status register
#define FIQ_REG_FINALSTATUS        0xd4//Fast interrupt final status register
#define IRQ_REG_PLEVEL             0xd8//IRQ System Priority Level Register

#define	irq_write(index, addr, val)		*(VBYTEP )(INTC0_BASE_ADDR + (index * 0x400)+ addr) = val
#define	irq_read(index, addr)			*(VBYTEP )(INTC0_BASE_ADDR + (index * 0x400)+ addr)

#define set_irq_reg(index,addr, val)          irq_write(index,addr, ((val) | irq_read(index,addr)))
#define clear_irq_reg(index,addr, val)        irq_write(index,addr, (~(val) & irq_read(index,addr)))


#define Pport_Int_Enable	(PP_CTL |= (0x1<<4))
#define Pport_Int_Disable	(PP_CTL &= ~(0x1<<4))
#define Pport_Int_Enable_Read	(PP_CTL & (0x1<<4) != 0)
#define Pport_Int_Status	(PP_STA & (0x1<<2) != 0)

#define MON_INTR_MASK	(*(VDWORDP) 0x30808)
#define MON_INTR_STS	(*(VDWORDP) 0x30810)

#define Mon_Int_Mask(type)		(MON_INTR_MASK |= (0x1<<(type)))
#define Mon_Int_Unmask(type)	(MON_INTR_MASK &= ~(0x1<<(type)))
#define Mon_Int_Mask_Read(type)	(MON_INTR_MASK & (0x1<<(type)) != 0)
#define Mon_Int_Status(type)	(MON_INTR_STS & (0x1<<(type)) != 0)

typedef void (*FUNCT_PTR_V_V)(void);
extern const FUNCT_PTR_V_V intr0_service[];
extern const FUNCT_PTR_V_V intr1_service[];

void Handle_Kbd(BYTE channel);
int i2c_dw_read_clear_intrbits(WORD baseaddr);
void i2c_dw_read(WORD baseaddr);
void i2c_dw_xfer_msg(WORD baseaddr);
void i2c_dw_req(WORD baseaddr);
void i2c_dw_tx_abrt(WORD baseaddr);



extern BYTE  Int_Control0_Enable(BYTE  int_number);
extern BYTE  Int_Control1_Enable(BYTE  int_num);
extern BYTE  Int_Control0_Disable(BYTE  int_num);
extern BYTE  Int_Control1_Disable(BYTE  int_num);
extern BYTE  Int_Control0_Mask(BYTE  int_num);
extern BYTE  Int_Control1_Mask(BYTE  int_num);
extern BYTE  Int_Control0_Unmask(BYTE  int_num);
extern BYTE  Int_Control1_Unmask(BYTE  int_num);
extern BYTE  Int_Control0_Status(BYTE  int_num);
extern BYTE  Int_Control1_Status(BYTE  int_num);
extern BYTE  Int_Control0_Enable_Read(BYTE  int_num);
extern BYTE  Int_Control1_Enable_Read(BYTE  int_num);
extern BYTE  Int_Control0_Mask_Read(BYTE  int_num);
extern BYTE  Int_Control1_Mask_Read(BYTE  int_num);
extern void CPU_Int_Enable(BYTE  type);
extern void CPU_Int_Disable(BYTE  type);
extern BYTE  CPU_Int_Enable_Read(BYTE  type);
extern void CPU_Int_Type_Edge(BYTE  type);
extern void CPU_Int_Type_Level(BYTE  type);
extern BYTE  CPU_Int_Type_Read(BYTE  type);

#endif
