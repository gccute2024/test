/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-25 10:43:14
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

#ifndef __AE_UPDATA_H_
#define __AE_UPDATA_H_

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

#include "KERNEL_SHAREMEMORY.H"
#include "KERNEL_UART.H"
#include "KERNEL_GPIO.H"
#include "KERNEL_SPIF.H"
//update config 
#define UART_UPDATE 1
#define SHAREMEM_UPDATE 1&SUPPORT_SHAREMEM_PNP
#define FIRMWARECYCTL_UPDATE 0
//update size
#define update_code_size 1024 // code_size =1000byte of code +16byte of size +8byte of cnt
#define update_cnt_size 2
#define update_state_size 2
#define update_check_size 16 //check_size 16*8=128bit
#define update_size (update_code_size+update_check_size+update_state_size+update_cnt_size)
#define flash_page_write 0x02
void Flash_Update_Function(void);
extern void LPC_Update_Config_Init(void);
#endif