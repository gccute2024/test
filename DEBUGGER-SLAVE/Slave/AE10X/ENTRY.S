/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-12 13:27:46
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

#ifndef ENTRY_S
#define ENTRY_S
#include "RISCV.H"

###############################################
###############################################
# Disable Interrupt
#
.macro DISABLE_MIE
  csrc CSR_MSTATUS, MSTATUS_MIE  
.endm


###############################################
###############################################
#Save caller registers
//该宏用于保存ABI定义的“调用者应保持的寄存器（Caller saved register）”进入堆栈
.macro SAVE_CONTEXT
//更改堆栈指针，分配20个单字（32位）的空间用于保存寄存器

  addi sp, sp, -20*(1 << 2)
  sw x1, 0*(1 << 2)(sp)
	sw x4, 1*(1 << 2)(sp)
	sw x5, 2*(1 << 2)(sp)
	sw x6, 3*(1 << 2)(sp)
	sw x7, 4*(1 << 2)(sp)
	sw x10, 5*(1 << 2)(sp)
	sw x11, 6*(1 << 2)(sp)
	sw x12, 7*(1 << 2)(sp)
	sw x13, 8*(1 << 2)(sp)
	sw x14, 9*(1 << 2)(sp)
	sw x15, 10*(1 << 2)(sp)
# #ifndef __riscv_32e
#   sw x16, 11*(1 << 2)(sp)
# 	sw x17, 12*(1 << 2)(sp)
# 	sw x28, 13*(1 << 2)(sp)
# 	sw x29, 14*(1 << 2)(sp)
# 	sw x30, 15*(1 << 2)(sp)
# 	sw x31, 16*(1 << 2)(sp)
# #endif

.endm


###############################################
###############################################
#restore caller registers
//该宏用于从堆栈中恢复ABI定义的“调用者应保存的寄存器（Caller saved register）”
.macro RESTORE_CONTEXT
  lw x1, 0*(1 << 2)(sp)
	lw x4, 1*(1 << 2)(sp)
	lw x5, 2*(1 << 2)(sp)
	lw x6, 3*(1 << 2)(sp)
	lw x7, 4*(1 << 2)(sp)
	lw x10, 5*(1 << 2)(sp)
	lw x11, 6*(1 << 2)(sp)
	lw x12, 7*(1 << 2)(sp)
	lw x13, 8*(1 << 2)(sp)
	lw x14, 9*(1 << 2)(sp)
	lw x15, 10*(1 << 2)(sp)
//#ifndef __riscv_32e
//  lw x16, 11*(1 << 2)(sp)
//	lw x17, 12*(1 << 2)(sp)
//	lw x28, 13*(1 << 2)(sp)
//	lw x29, 14*(1 << 2)(sp)
//	lw x30, 15*(1 << 2)(sp)
//	lw x31, 16*(1 << 2)(sp)
//#endif

// De-allocate the stack space
 addi sp, sp, 20*(1 << 2)
 //恢复寄存器之后，更改堆栈指针，回收19个单字（32位）的空间
//#endif
.endm

###############################################
###############################################
#restore caller registers

.macro RESTORE_CONTEXT_EXCPT_X5
  lw x1,  0*(1 << 2)(sp)
  lw x6,  2*(1 << 2)(sp)
  lw x7,  3*(1 << 2)(sp)
  lw x10, 4*(1 << 2)(sp)
  lw x11, 5*(1 << 2)(sp)
  lw x12, 6*(1 << 2)(sp)
  lw x13, 7*(1 << 2)(sp)
  lw x14, 8*(1 << 2)(sp)
  lw x15, 9*(1 << 2)(sp)
#ifndef __riscv_32e
  lw x16, 10*(1 << 2)(sp)
  lw x17, 11*(1 << 2)(sp)
  lw x28, 12*(1 << 2)(sp)
  lw x29, 13*(1 << 2)(sp)
  lw x30, 14*(1 << 2)(sp)
  lw x31, 15*(1 << 2)(sp)
#endif
.endm

###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_ONLY_X5
  lw x5,  1*(1 << 2)(sp)
.endm

###############################################
###############################################
# Save the mepc and mstatus
#
//该宏用于保存MEPC和MSTATUS寄存器进入堆栈
.macro SAVE_EPC_STATUS
  csrr x5, CSR_MEPC  
  sw x5,  16*(1 << 2)(sp)
  csrr x5, CSR_MSTATUS  
  sw x5,  17*(1 << 2)(sp)
  csrr x5, CSR_MCAUSE
  sw x5,  18*(1 << 2)(sp)
.endm

###############################################
###############################################
# Restore the mepc and mstatus
#
//该宏用于从堆栈中恢复MEPC和MSTATUS寄存器
.macro RESTORE_EPC_STATUS
  lw x5,  16*(1 << 2)(sp)
  csrw CSR_MEPC, x5  
  lw x5,  17*(1 << 2)(sp)
  csrw CSR_MSTATUS, x5  
  lw x5,  18*(1 << 2)(sp)
  csrw CSR_MCAUSE, x5  
.endm



###############################################
###############################################
// Trap entry point
//
  .section      .text.trap	
  .align 6// In CLIC mode, the trap entry must be 64bytes aligned
  .global trap_entry
.weak trap_entry
 //指定该标签为weak类型，标签为“弱（weak）”属性。“弱（weak）”属性是
            // C/C++语法中定义的一种属性，一旦有具体的“非弱”性质同名函数存在，将
            //会覆盖此函数。
trap_entry:
//定义标签名trap_entry，该标签名作为函数入口
  //进入异常处理函数之前必须先保存处理器的上下文
  // Allocate the stack space
 // addi sp, sp, -19*(1 << 2)

  // Save the caller saving registers (context)
  SAVE_CONTEXT
  //此处调用SAVE_CONTEXT保存ABI定义的“调用者应存储的寄存器（Caller saved 
//register）”进入堆栈

  // Save the MEPC/Mstatus/Msubm reg
  SAVE_EPC_STATUS
//此处调用SAVE_EPC_MSTATUS保存MEPC和MSTATUS寄存器进入堆栈

     // Set the function argument
     //调用handle_trap函数
//根据ABI调用原则，函数调用时由a0和a1寄存器传递参数，因此此处赋参数值给a0和a1
  csrr a0, mcause//参数1错误代码
  mv a1, sp//参数2，堆栈指针
     // Call the function
  call handle_trap//进入c的handle_treap

  // Restore the MEPC/Mstatus/Msubm reg
  //在退出异常处理函数之后需要恢复之前保存的处理器上下文
  RESTORE_EPC_STATUS//此处调用RESTORE_EPC_MSTATUS从堆栈中恢复MEPC和MSTATUS寄存器
  // Restore the caller saving registers (context)
  RESTORE_CONTEXT//调用RESTORE_CONTEXT从堆栈中恢复ABI定义的“调用者应存储的寄存器（Caller saved 
//register）”

  // De-allocate the stack space
 // addi sp, sp, 19*(1 << 2)
  // Return to regular code
  mret//使用mret指令从异常模式返回

#endif
#ifdef USER_AE10X_LIBC_A
  .section      .text.riscv_sr_x	
  .align 2

  .global __riscv_save_0
  .global __riscv_save_1
  .global __riscv_save_2
__riscv_save_0:
__riscv_save_1:
__riscv_save_2:
  addi	sp,sp,-12//加载sp现场保存
  sw	s1,0(sp)//保存s1，存储寄存器，切换上下文保存
  sw	s0,4(sp)//保存s0
  sw	ra,8(sp)//保存返回寄存器的值（保存返回位置）
  jr	t0
  .global __riscv_restore_0
  .global __riscv_restore_1
  .global __riscv_restore_2
__riscv_restore_0:
__riscv_restore_1:
__riscv_restore_2://恢复现场
  lw	s1,0(sp)
  lw	s0,4(sp)
  lw	ra,8(sp)
  addi	sp,sp,12
  ret

  .section      .text.riscv_sr_x	
  .align 4

  .global __mulsi3
  .type __mulsi3,@function//定义为函数
  //逻辑：如果低位为0则不变反之则加上对应的值
__mulsi3://A*B=C//a0=A,A2=B
   mv	a2,a0//a2=A
   li	a0,0//a0=0=C
   andi	a3,a1,1//a3=B&1
   beqz	a3,__mulsi3+0xc//if(a3==0)srli	a1,a1,0x1;else add	a0,a0,a2//a0=a0+a2
   add	a0,a0,a2//a0=a0+a2//
   srli	a1,a1,0x1//a1>>1//B/=2
   slli	a2,a2,0x1//a2<<1//A*=2
   bnez	a1,__mulsi3+0x4//if(a1!=0)andi	a3,a1,1; else ret
   ret

  .global __udivsi3
  .type __udivsi3,@function//定义为函数
__udivsi3://A/B=C//A=a0,B=a1
   mv	a2,a1//a2=B
   mv	a1,a0//a1=A
   li	a0,-1//a0=-1//0xffffffff
   beqz	a2,__udivsi3+0x2a//if(a2==0)ret(return -1);else  li	a3,1//除数B为0报错
   li	a3,1//a3=1
   bgeu	a2,a1,__udivsi3+0x1a//if(a2>=a1) li	a0,0 else blez	a2,__udivsi3+0x1a//除数B>=被除数A时C=0
   blez	a2,__udivsi3+0x1a//if (a2<=0)li	a0,0 else slli	a2,a2,0x1//此处判断B是否被分析完，如果B=0则退出C=0
   slli	a2,a2,0x1//a2<<=1;//B*=2
   slli	a3,a3,0x1//a3<<=1;//a3*=2
   bltu	a2,a1,__udivsi3+0xe//if(a2<a1)blez	a2,__udivsi3+0x1a else a0=0、、此处判断Ba2要×多少个2的n次方才能超过被除数a3作为计数
   li	a0,0//C=0
   bltu	a1,a2,__udivsi3+0x24//if(a1<a2) srli	a3,a3,0x1 else sub	a1,a1,a2 //如果a1<a2,此时无法再减去了故而跳过处理a3
   sub	a1,a1,a2//a1=a1-a2(A=A-B)
   or	a0,a0,a3//a0=a0|a3,C=C|a3//这里类似于填充运算，相当于通过不同数据的填充获取最大减的值
   srli	a3,a3,0x1//a3>>1//B
   srli	a2,a2,0x1//a2>>1//B/2,相当于从最大值开始加入内容
   bnez	a3,__udivsi3+0x1c//if(a3!=0)bltu	a1,a2,__udivsi3+0x24,else ret//此处判断a3处理完成与否，错误的状态此时a3为0，否则则会在a3的计数结束后退出
   ret

  .global __umodsi3
  .type __umodsi3,@function//定义为函数
__umodsi3://A%B//a0=a,a1=B
   mv	t0,ra//把返回地址调入t0保存
   jal	__udivsi3//跳转进行除法运算//返回a0为除数，a1即为余数
   mv	a0,a1//把a1作为返回值到a0
   jr	t0//返回地址
  .global __divsi3
  .type __divsi3,@function//定义为函数
__divsi3://signed  A/B 
   bltz	a0,__divsi3+0x8//if(a0<0)neg	a0,a0,else bltz	a1,__divsi3+0x16//A<0则取其补码负数是按补码存储的，所以转为负数才能运算
   bltz	a1,__divsi3+0x16//if(a1<0)neg	a1,a1,else bltz	a1,__divsi3+0x16//A<0则取其补码
   neg	a0,a0//a0取饭吗（A=-A）
   bgez	a1,__divsi3+0x1a//if(a1>=0)mv	t0,ra else neg	a1,a1//如果B》=0则跳转无符号获取后再取补即为-A/B=-C,结果不变，
   neg	a1,a1//a1取反(B=-B),此时a1,a0都是正数
   j	__udivsi3//进行无符号运算，得到正数相除结果，后通过_udivsi3直接返回ra的地址（此处之后不再运行）
   neg	a1,a1//a1取补B=-B
   mv	t0,ra//保留返回值
   jal	__udivsi3//跳转进行计算
   neg	a0,a0//对结果进行取反，同理B=0时候结果为-1，此时因为a0可能为负数-1，所以对报错-1没有绝对的办法，默认无处理
   jr	t0
  
  .global __modsi3
  .type __modsi3,@function//定义为函数
__modsi3://signed A%B
   mv	t0,ra
   bltz	a1,__modsi3+0x10//if(a1<0)neg	a1,a1 else bltz	a0,__modsi3+0x18//B<0
   bltz	a0,__modsi3+0x18//if(a0<0)neg	a0,a0 else jal	__udivsi3//A<0
   jal	__udivsi3
   mv	a0,a1//a0=余数
   jr	t0
   neg	a1,a1//B=-B
   bgez	a0,__modsi3+0xa//if(a0>=0)jal	__udivsi3 else neg	a0,a0,此时相当于B的正负不会影像到余数
   neg	a0,a0//a0取反
   jal	__udivsi3//求余数
   neg	a0,a1//a0=-a1
   jr	t0




#endif