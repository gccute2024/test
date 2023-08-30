/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-15 14:30:53
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

#include <RISCV.H>
	.section .vtable
    .globl vector_base
vector_base:
    .word  CPUS_HANDLER//0
    .word  CPUT_HANDLER//1  
    .word  CPUR_HANDLER//2
    .word  SWUC_HANDLER//3
    .word  LPCRST_HANDLER//4
    .word  PWRSW_HANDLER//5
    .word  PS2_0_HANDLER//6
    .word  KBS_SDV_HANDLER//7
    .word  KBS_PRESS_HANDLER//8
    .word  TACH0_HANDLER//9
    .word  TACH1_HANDLER//10
    .word  TACH2_HANDLER//11
    .word  TACH3_HANDLER//12
    .word  KBC_IBF_HANDLER//13
    .word  KBC_OBE_HANDLER//14
    .word  PMC1_IBF_HANDLER//15
    .word  PMC1_OBE_HANDLER//16
#if (defined(TEST101)||defined(AE101)||defined(AE102))
    .word  PMC2_IBF_HANDLER//17
    .word  PMC2_OBE_HANDLER//18
#elif defined(AE103)
    .word  WU42_HANDLER//17
    .word  RTC_HANDLER//18
#endif
    .word  WDT_HANDLER//19
    .word  ADC_HANDLER//20
    .word  UART0_HANDLER//21
    .word  UARTA_HANDLER//22
    .word  UARTB_HANDLER//23
    .word  SM_EC_HANDLER//24
    .word  SM_HOST_HANDLER//25
    .word  TIMER0_HANDLER//26
    .word  TIMER1_HANDLER//27
    .word  TIMER2_HANDLER//28
    .word  TIMER3_HANDLER//29
    .word  INTR0_HANDLER//30
    .word  INTR1_HANDLER//31
    j trap_entry
    call _start
	/* Call global constructors */
	 /* 以下调用Newlib全局的构造函数（Global constructors） */
	call init//调用__libc_init_array（Newlib的函数）
    //
    //注意：上述的__libc_fini_array，atexit和__libc_init_array函数都是Newlib C
    //       运行库的特殊库函数，用于处理一些C/C++程序中的全局性的构造和析构函数。本文档在
    //      此对其不做详细介绍，请用户自行查阅相关资料学习。
    //
    //值得注意的是：__libc_init_array函数中会调用一个名为_init的函数，n100-sdk
    //环境中的_init函数定义在bsp/core/env/init.c中，因此此处会执行该函数，后
    //文对此_init.c文件将进行进一步介绍。

	//调用main函数
    //根据ABI调用原则，函数调用时由a0和a1寄存器传递参数，因此此处赋参数值给a0和a1
	/* argc = argv = 0 */
	li a0, 0
	li a1, 0
	call main //调用main函数，开始执行main函数
	tail exit//如果完成了main函数后，调用exit函数（Newlib桩函数之一，参见第2.3.1
            //节了解Newlib桩函数的更多信息）
1:
	j 1b //最后的死循环，程序理论上不可能执行到此处
    call _start
	/* Call global constructors */
	 /* 以下调用Newlib全局的构造函数（Global constructors） */
	call main_loop//调用__libc_init_array（Newlib的函数）
1:
    j 1b //最后的死循环，程序理论上不可能执行到此处
    .align 2
	.section .init//声明section段名.init
	.globl _start//全局标签_start
	.type _start,@function//定义为函数
_start://标签位置
	csrc CSR_MSTATUS, MSTATUS_MIE//中断全局关闭
    	/* Set the the NMI base to share with mtvec by setting CSR_MMISC_CTL */
   // 	li t0, (0x1 << 9);
   // 	csrs CSR_MMISC_CTL, t0

	 /* Intial the mtvt*/
    //  	la t0, vector_base
    //  	csrw CSR_MTVT, t0

	/* Intial the mtvt2 and enable it*/
    //	la t0, irq_entry
    //	csrw CSR_MTVT2, t0
    //	csrs CSR_MTVT2, 0x1

    	/* Intial the CSR MTVEC for the Trap ane NMI base addr*/
    //	la t0, trap_entry
    //	csrw CSR_MTVEC, t0
#ifdef __riscv_flen
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrs mstatus, t0 
	csrw fcsr, x0   
#endif

    	.option push
    	.option norelax
		//设置全局指针
	la gp, __global_pointer$//将标签__global_pointer$所处的地址赋值给gp寄存器
	//注意：标签__global_pointer在链接脚本中定义参见链接脚本的__global_pointer$标签
    	.option pop
		//设置堆栈指针
	la sp, _sp //将标签_sp所处的地址赋值给sp寄存器
 //注意：标签_sp在链接脚本中定义，参见链接脚本的_sp标签

  //下列代码判断_ilm_lma与_ilm标签的地址值是否相同：
    // 如果相同则意味着代码直接从Flash中进行执行（link_flashxip.lds中定义的_ilm_lma与_ilm标签地址相等），那么直接跳转到后面数字标签2所在的代码继续执行；
//如果不相同则意味着代码需要从Flash中上载至ILM中进行执行（link_flash.lds中定义的_ilm_lma与_ilm标签地址不相等），因此使用lw指令逐条地将指令从Flash中读取出来，然后使用sw指令逐条地写入ILM中，通过此方式完成指令的上载至ILM中。
	/* Bob: Load code section from flash to ILM */
//	la a0, _ilm_lma//将标签_ilm_lma所处的地址赋值给a0寄存器
 //注意：标签_ilm_lma在链接脚本中定义，参见链接脚本的_ilm_lma标签
//	la a1, _ilm//将标签_ilm所处的地址赋值给a1寄存器
//将标签_ilm所处的地址赋值给a1寄存器
  //  beq a0, a1, 2f  //a0和a1的值分别为标签_ilm_lma和_ilm标签的地址，判断其
                    //是否相等，如果相等则直接跳到后面的数字“2”标签所在的地方，
                    //如果不等则继续向下执行
//	la a2, _eilm  //将标签_eilm所处的地址赋值给a2寄存器
 //注意：标签_eilm在链接脚本中定义，参见链接脚本的_eilm标签
//	bgeu a1, a2, 2f //通过一个循环，将指令从Flash中搬到ILM中
//如果_ilm标签地址比_eilm标签地址还大，则属于不正常的配置，放弃搬运，直接跳转到后面数字标签2所在的位置
//1:
//	lw t0, (a0) //从地址指针a0所在的位置（Flash中）读取32位数
//	sw t0, (a1)//将读取的32位数写入地址指针a1所在的位置（ILM中）
//	addi a0, a0, 4 //将地址指针a0寄存器加4（即32位）
//	addi a1, a1, 4//将地址指针a0寄存器加4（即32位）
//	bltu a1, a2, 1b //跳转回之前数字标签1所在的位置
//2:
 #if 0
#ifdef AE103   
#ifndef DUBLE_FIRMWARE2
	la a0, _rom_head
    la a1, _rom
	la a2, _rom_end
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
#endif
#endif
#endif
	 /* 使用与上述相同的原理，通过一个循环，将 数据从Flash中搬运到DLM中*/
#ifdef flash	 
2:
	la a0, _data_lma
	la a1, _data
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:

	//BSS段是链接器预留的未初始化变量所处的地址段，引导程序必须对其初始化为0
	//此处通过一个循环来初始化BSS段
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
	
2:	
    //ec space
    la a0, MEM_ADDR
	la a1, DATA_ADDR
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
#endif
2:
	/*enable mcycle_minstret*/
    csrci CSR_MCOUNTINHIBIT, 0x5//开启全局计数
    ret

	.global disable_mcycle_minstret
disable_mcycle_minstret: //用于控制计数器的关闭(低功耗考虑) 
        csrsi CSR_MCOUNTINHIBIT, 0x5
	ret

	.global enable_mcycle_minstret
enable_mcycle_minstret: //用于控制计数器的开启    
        csrci CSR_MCOUNTINHIBIT, 0x5
	ret

	.global core_wfe
core_wfe: //休眠时，Wait for Event
    	csrc CSR_MSTATUS, MSTATUS_MIE
    	csrs CSR_WFE, 0x1
    	wfi   //WFI的休眠模式
    	csrc CSR_WFE, 0x1
    	csrs CSR_MSTATUS, MSTATUS_MIE
	ret
#ifdef AE103
#ifndef DUBLE_FIRMWARE2
.align 2
	.section .rom_start//声明section段名.init
	.globl rom_start//全局标签_start
	.type rom_start,@function//定义为函数
rom_start://标签位置
    .word  0x10084//0
    .word  rom_main//4
    .word  rom_wdt_feed//8
    .word  rom_putc//C
    .word  rom_puts//10
    .word  rom_get_cpu_freq//14
    .word  rom_uart//18
    .word  rom_ejtag//1C
    .word  rom_wdt//20
    .word  select_flash//24
    .word  rom_memset//28
    .word  ROM_strcmp//2C
    .word  ROM_SPIF_ERASE//30
    .word  SPIF_Write_Interface//34
    .word  SPIF_Read_Interface//38
    .word  ROM_Update//3C
    .word  ROM_SHIO_Update//40
    .word  rom_exit//44
    .word  rom_SM2Pulib_Key//48
    .word  rom_SM2_VERIFY//4C
    .word  rom_SM4_Run//50
    .word  rom_SM3_Run//54
    .word  0//58
    .word  0//5C
    .word  0//60
    .word  0//64
    .word  0//68
    .word  0//6C
    .word  0//70
    .word  0//74
    .word  0//78
    .word  0//7C
    .word  0//80
    #j 0x10084
    la sp, _sp //将标签_sp所处的地址赋值给sp寄存器//0x10084启动位置,sp为0x28000   
    call rom_main
    tail rom_exit
1:
	j 1b //最后的死循环，程序理论上不可能执行到此处

    #unimp
    #unimp
    #unimp
    #unimp
    #unimp
    #unimp
    #.word  0x0      
    #j rom_trap
#endif
#endif