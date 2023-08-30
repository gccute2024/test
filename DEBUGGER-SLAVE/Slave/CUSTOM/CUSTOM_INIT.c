/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:23:17
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

#include "CUSTOM_INIT.H"

//----------------------------------------------------------------------------
// Oem_StartUp
//----------------------------------------------------------------------------
void Oem_StartUp(void)
{

}

//----------------------------------------------------------------------------
// Oem_Initialization - Initialize the registers, data variables, and oem 
// functions after kernel init. function.
//----------------------------------------------------------------------------
BYTE  LCDOK = 0x0;

void Oem_Initialization(void)
{
	EC_RESET_MODE = 0x12;
	EC_S3_TIMEOUT_L = 0;
	EC_S3_TIMEOUT_H = 0;
	BRIGHT_LEVEL_CTRL = 20;
	BRIGHT_LEVEL = 20;
	LCDOK =0xAA;
//	ECAutoPowerOn = 1;
	//Init_GPIO();
//	Init_GPIO_PD();
//	OEM_INIT_ACPI();


}


