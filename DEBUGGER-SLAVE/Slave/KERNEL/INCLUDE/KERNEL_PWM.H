/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-06 09:57:58
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


#ifndef __KERNEL_PWM_H
#define __KERNEL_PWM_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"

//*****************************************************************************
// define
//*****************************************************************************
#define PWM_BASE	PWM_BASE_ADDR   //modify register for risv
#if 0
#define PWM_CTRL            (*(VWORDP)(PWM_BASE+0x0))
#define PWM_MODE            (*(VWORDP)(PWM_BASE+0x2))
#define PWM_SCALER0         (*(VWORDP)(PWM_BASE+0x4))
#define PWM_SCALER1         (*(VWORDP)(PWM_BASE+0x6))
#define PWM_SCALER2         (*(VWORDP)(PWM_BASE+0x8))
#define PWM_SCALER3         (*(VWORDP)(PWM_BASE+0xa))
#define PWM_CTR0_1          (*(VWORDP)(PWM_BASE+0xc))
#define PWM_CTR2_3          (*(VWORDP)(PWM_BASE+0xe))
#define PWM_CLK_SEL         (*(VWORDP)(PWM_BASE+0x10))
#define PWM_CTR_SEL         (*(VWORDP)(PWM_BASE+0x12))
#define PWM_DCR0_1          (*(VWORDP)(PWM_BASE+0x14))
#define PWM_DCR2_3          (*(VWORDP)(PWM_BASE+0x16))
#define PWM_DCR4_5          (*(VWORDP)(PWM_BASE+0x18))
#define PWM_DCR6_7          (*(VWORDP)(PWM_BASE+0x1a))
#define PWM_STEP3_0         (*(VWORDP)(PWM_BASE+0x1c))
#define PWM_STEP7_4         (*(VWORDP)(PWM_BASE+0x1e))
#endif



#if 0
#define TACH_CTRL           (*(VWORDP)(PWM_BASE+0x20))
#define TACH_INT            (*(VWORDP)(PWM_BASE+0x22))
#define TACH0_DATA          (*(VWORDP)(PWM_BASE+0x24))
#define TACH1_DATA          (*(VWORDP)(PWM_BASE+0x26))
#define TACH2_DATA          (*(VWORDP)(PWM_BASE+0x28))
#define TACH3_DATA          (*(VWORDP)(PWM_BASE+0x2a))
#endif

#define CLK_MODE  4
#define TACH0_MODE 8
#define TACH1_MODE 10
#define TACH2_MODE 12
#define TACH3_MODE 14

#define TACH_MODE0  0x0
#define TACH_MODE1  0x1
#define TACH_MODE2  0x2
#define TACH_MODE3  0x3

#define Tach_Int_Enable(channel)	(TACH_INT |= (0x1 << channel))
#define Tach_Int_Disable(channel)	(TACH_INT &= ~(0x1 << channel))
#define Tach_Int_Enable_Read(channel)		((TACH_INT & (0x1 << channel)) != 0)

//*****************************************************************************
// pwm registers struct
//*****************************************************************************
typedef struct pwm_str
{
	BYTE   pwm_no;
	BYTE   prescale;
	BYTE   pwm_polarity;
	BYTE   pwm_softmode;
	WORD pwm_scale;
	BYTE    pwm_ctr;
	BYTE   pwm_clk_sel;
	BYTE   pwm_ctr_sel;
	BYTE    pwm_dcdr;
	BYTE   pwm_step;
} spwm_str;

typedef struct tach_str
{
	BYTE   tach_no;
	BYTE   tach_mode;
	BYTE   tach_intr;
} stach_str;

//*****************************************************************************
// Function prototype
//*****************************************************************************
extern BYTE  PWM_Enable(BYTE  pwm_no);
extern BYTE  PWM_Disable(BYTE  pwm_no);
extern BYTE  PWM_Set(spwm_str * pwm_ptr);
extern BYTE  PWM_Get(spwm_str * pwm_ptr);
extern BYTE  PWM_Init(BYTE channel, BYTE val);
extern BYTE  TACH_Enable(BYTE  tach_no);
extern BYTE  TACH_Disable(BYTE  tach_no);
extern BYTE  TACH_Int_Enable(BYTE  tach_no);
extern BYTE  TACH_Int_Disable(BYTE  tach_no);
extern BYTE  TACH_Set(stach_str * tach_ptr);
extern DWORD TACH_Init(void);
extern BYTE  TACH_Clear_IRQ(BYTE  tach_no);
extern WORD TACH_Get(BYTE  tach_no);
WORD TACH_Get_Polling(BYTE  tach_no);

#endif

