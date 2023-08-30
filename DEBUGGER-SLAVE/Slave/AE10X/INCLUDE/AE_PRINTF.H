/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-05 11:01:40
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

#ifndef AE_PRINTF_H
#define AE_PRINTF_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_UART.H"
/**********************define**************************************************************/
// #define DEBUG
#if DEBUG
#define dprint(fmt, args...) printf(fmt, ##args)
#else
#define dprint(fmt, args...)
#endif
#define PRINT_MAX_SIZE        0x400
extern VBYTE print_buff[];
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define is_digit(c)	((c) >= '0' && (c) <= '9')

#if (PRINTF_UART_SWITCH==0)
	#define PRINTF_LSR        UART0_LSR
  #define PRINTF_LCR        UART0_LCR
  #define PRINTF_IER        UART0_IER
  #define PRINTF_IIR        UART0_IIR
  #define PRINTF_TX         UART0_TX
  #define PRINTF_RX         UART0_RX
#elif (PRINTF_UART_SWITCH==1)
	#define PRINTF_LSR        UART1_LSR
  #define PRINTF_LCR        UART1_LCR
  #define PRINTF_IER        UART1_IER
  #define PRINTF_IIR        UART1_IIR
  #define PRINTF_TX         UART1_TX
  #define PRINTF_RX         UART1_RX
#elif (PRINTF_UART_SWITCH==2)
	#define PRINTF_LSR        UART2_LSR
  #define PRINTF_LCR        UART2_LCR
  #define PRINTF_IER        UART2_IER
  #define PRINTF_IIR        UART2_IIR
  #define PRINTF_TX         UART2_TX
  #define PRINTF_RX         UART2_RX
#elif (PRINTF_UART_SWITCH==3)
	#define PRINTF_LSR        UART3_LSR
  #define PRINTF_LCR        UART3_LCR
  #define PRINTF_IER        UART3_IER
  #define PRINTF_IIR        UART3_IIR
  #define PRINTF_TX         UART3_TX
  #define PRINTF_RX         UART3_RX
#elif (PRINTF_UART_SWITCH==4)
	#define PRINTF_LSR        UARTA_LSR
  #define PRINTF_LCR        UARTA_LCR
  #define PRINTF_IER        UARTA_IER
  #define PRINTF_IIR        UARTA_IIR
  #define PRINTF_TX         UARTA_TX
  #define PRINTF_RX         UARTA_RX
#elif (PRINTF_UART_SWITCH==5)
	#define PRINTF_LSR        UARTB_LSR
  #define PRINTF_LCR        UARTB_LCR
  #define PRINTF_IER        UARTB_IER
  #define PRINTF_IIR        UARTB_IIR
  #define PRINTF_TX         UARTB_TX
  #define PRINTF_RX         UARTB_RX
#else
  #define PRINTF_LSR        UARTA_LSR
  #define PRINTF_LCR        UARTA_LCR
  #define PRINTF_IER        UARTA_IER
    #define PRINTF_IIR      UARTA_IIR
  #define PRINTF_TX         UARTB_TX
  #define PRINTF_RX         UARTA_RX
#endif

#define __ismask(x) (_ctype[(BYTE )(x)])//_ctype[(int)(BYTE )(x)&0x07 != 0
#undef isalnum
#define isalnum(c)	((__ismask(c)&(HEX01|HEX02|HEX04)))//检查所传的字符是否是字母和数字
#undef isalpha
#define isalpha(c)	((__ismask(c)&(HEX01|HEX02)))//检查所传的字符是否是字母
#undef iscntrl
#define iscntrl(c)	(__ismask(c)&HEX20)//检查所传的字符是否是控制字符
#undef isdigit
#define	isdigit(c)	(__ismask(c)&HEX04)//检查所传的字符是否是十进制数字
#undef isgraph
#define	isgraph(c)	(__ismask(c)&(HEX10|HEX01|HEX02|HEX04))//检查所传的字符是否有图形表示法
#undef islower
#define	islower(c)	((__ismask(c)&(HEX01|HEX02))==HEX02)//检查所传的字符是否是小写字母
#undef isprint
#define isprint(c)	(__ismask(c)&(HEX10|HEX01|HEX02|HEX04|HEX80))//检查所传的字符是否是可打印的
#undef ispunct
#define ispunct(c)	(__ismask(c)&HEX10)//检查所传的字符是否是标点符号字符
#undef isspace
#define	isspace(c)	(__ismask(c)&HEX08)//检查所传的字符是否是空白字符
#undef isupper
#define	isupper(c)	((__ismask(c)&(HEX01|HEX02))==HEX01)//检查所传的字符是否是大写字母
#undef isxdigit
#define	isxdigit(c)	(__ismask(c)&(HEX40|HEX04))//检查所传的字符是否是十六进制数字
#undef tolower
#define tolower(c) (isupper(c)?((c)-'A'+'a'):(c))//大写转小写
#undef toupper
#define toupper(c) (islower(c)?((c)-'a'+'A'):(c))//小写转大写

#define CONFIG_ONE_PUT
#endif