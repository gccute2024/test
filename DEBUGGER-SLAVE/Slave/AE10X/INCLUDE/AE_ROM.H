/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-02 15:47:26
 * @Description: 
 * 
 * 
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中⽂及英⽂版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效⼒，违反必究。
 * 
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023⻰晶⽯半导体科技（苏州）有限公司
 */
#ifndef AE_ROM_H
#define AE_ROM_H
#include "AE_GLOBAL.H"
#include "AE_CONFIG.H"
#include "AE_REG.H"	  
#include "AE_REG.H"	 
#include "AE_ROMPATCH.H"	  

#include "AE_UPDATE.H" 
#include "KERNEL_SOC_FUNC.H"
#include "KERNEL_SMx.H"
#include "KERNEL_MEMORY.H"
#include "KERNEL_GPIO.H"

void OPTIMEIZE0 SECTION(".ROM.Func.puts") USED rom_puts(register BYTEP s);
void OPTIMEIZE0 SECTION(".ROM.Func.uart") USED rom_uart(void);//uart初始化
void OPTIMEIZE0 SECTION(".ROM.Func.ejtag") USED rom_ejtag(void);//ejtag 初始化

#define EXTERNAL_FLASH              1//外部flash
#define INTERNAL_FLASH              0//内部flash


//extern of addr
extern BYTE _start;
extern BYTE _rom;
extern BYTE _rom_var;
extern BYTE _MAGIC_NUMBER;
extern BYTE RAM_ADDR;
#if 1
#define rom_bootmode           ((*(VDWORDP)(SYSCTL_BASE_ADDR+0x1c))&BIT4)
#else
#define rom_bootmode           (rom_dataONram.bootmode)
#endif
#define SYSCTL_ID               (((*(VDWORDP)(SYSCTL_BASE_ADDR+0x4))>>24)&0xff)

#define flash_cache_addr        FLASH_BASE_ADDR
#define flash_info_addr         0x100UL

#define magic_number_addr       ((srom_data *)(&_MAGIC_NUMBER))
#define ROM_RAM_BASE_ADDR       (DRAM_BASE_ADDR+0x800)
//通用
#define ROM_I                   (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x0))
#define ROM_J                   (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x4))
#define ROM_TEMP0               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x8))
#define ROM_TEMP1               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0xC))
#define ROM_TEMP2               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x10))
#define ROM_TEMP3               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x14))
#define ROM_TEMP4               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x18))
#define ROM_TEMP5               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x1C))
#define ROM_TEMP6               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x20))
#define ROM_TEMP7               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x24))
#define rom_smx_buffer          (*(uDword *)(ROM_RAM_BASE_ADDR+0x28))

#define ROM_COPY_CNT            (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x2C))
#define ROM_jump                (*(FUNCT_PTR_V_V *)(ROM_RAM_BASE_ADDR+0x30))
#define ROM_magic_number_addr   (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x34))
#define ROM_only_extern_flag    (*(VBYTEP)(ROM_RAM_BASE_ADDR+0x38))
#define ROM_flash_flag          (*(VBYTEP)(ROM_RAM_BASE_ADDR+0x39))
#define ROM_flashqe_flag        (*(VBYTEP)(ROM_RAM_BASE_ADDR+0x3A))
#define ROM_error_cnt           (*(VBYTEP)(ROM_RAM_BASE_ADDR+0x3B))
#define ROM_code_size           (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x3C))

//固件更新
#define update_state            (*(VWORDP)(ROM_RAM_BASE_ADDR+0x40))
#define update_cnt              (*(VWORDP)(ROM_RAM_BASE_ADDR+0x42))
#define update_addrs            (*(VWORDP)(ROM_RAM_BASE_ADDR+0x44))
#define update_rx_cnt           (*(VWORDP)(ROM_RAM_BASE_ADDR+0x46))

//安全
#define rom_T                   ((VDWORDP)(ROM_RAM_BASE_ADDR+0x60))
#define rom_R                   ((VDWORDP)(ROM_RAM_BASE_ADDR+0x80))
#define rom_P0                  (*(spoint *)(ROM_RAM_BASE_ADDR+0xA0))
#define rom_P1                  (*(spoint *)(ROM_RAM_BASE_ADDR+0xE0))
#define rom_P2                  (*(spoint *)(ROM_RAM_BASE_ADDR+0x120))

#define rom_SM3_size            (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x200))
#define ROM_key_id              (*(VBYTEP)(ROM_RAM_BASE_ADDR+0x204))
#define sm3_data_offset         (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x208))
#define SPIF_addrs              (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x20C))
#define SPIF_data               (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x210))
#define SPIF_I                  (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x214))
#define SPIF_J                  (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x218))
#define SPIF_READ_COMMAND       (*(BYTEP)(ROM_RAM_BASE_ADDR+0x21C))
#define SPIF_WRITE_COMMAND      (*(BYTEP)(ROM_RAM_BASE_ADDR+0x21D))

#define UART_PTR                (*(VDWORDP)(ROM_RAM_BASE_ADDR+0x220))

//mirror
#define rom_safeONram           (*(srom_data *)(ROM_RAM_BASE_ADDR+0x400))
#define rom_dataONram           (*(sflash_info *)(ROM_RAM_BASE_ADDR+0x500))
#define rom_PATCHONram           ((spatch_typdef *)(ROM_RAM_BASE_ADDR+0x600))

#define rom_SM2_E               ((DWORDP)(ROM_RAM_BASE_ADDR+0xC00))
#define rom_SM3_hash            rom_SM2_E

//大buffer
#define flash_string            ((BYTEP)(ROM_RAM_BASE_ADDR+0x1400))
#define update_buff             ((VBYTEP)(ROM_RAM_BASE_ADDR+0x1800))
//安全
#endif