/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-06 09:41:12
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

//*****************************************************************************
// Include all header file
#include "KERNEL_PWM.H"
//
//*****************************************************************************

//*****************************************************************************
//
//  To enabel PWM channel [x] 
//
//  parameter :
//          pwm_no : pwm channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Enable(BYTE  pwm_no)
{	
	if((pwm_no < 0)||(pwm_no > 7))
	{
		return -1;
	}
	PWM_CTRL |= (0x1<<(pwm_no+8));
	return 0;
}

//*****************************************************************************
//
//  To disable PWM channel [x] 
//
//  parameter :
//          pwm_no : pwm channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Disable(BYTE  pwm_no)
{
	if((pwm_no < 0)||(pwm_no > 7))
	{
		return -1;
	}
	PWM_CTRL &= (~(0x1<<(pwm_no+8)));
	return 0;
}

//*****************************************************************************
//
//  To setup PWM channel [x] 
//
//  parameter :
//          pwm struct
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Set(spwm_str * pwm_ptr)
{
	if((pwm_ptr == NULL)||(pwm_ptr->pwm_no < 0)||(pwm_ptr->pwm_no  > 7))
	{
		return -1;
	}

	/*prescale select*/
	PWM_CTRL &= (~(0x1<<pwm_ptr->pwm_no));
	PWM_CTRL |= (pwm_ptr->prescale<<pwm_ptr->pwm_no);
	
	//DWORD pwm_softmode
	PWM_MODE &= (~(0x1<<(pwm_ptr->pwm_no)));
	PWM_MODE |= (pwm_ptr->pwm_softmode<<(pwm_ptr->pwm_no));
	/* pwm_polarity */
	PWM_MODE &= (~(0x1<<(pwm_ptr->pwm_no + 8)));
	PWM_MODE |= (pwm_ptr->pwm_polarity<<(pwm_ptr->pwm_no + 8));

	/* pwm_scale */
	switch(pwm_ptr->pwm_clk_sel)
	{
		case 0:
			PWM_SCALER0 = 0x0;
			PWM_SCALER0 = pwm_ptr->pwm_scale;
			break;
		case 1:
			PWM_SCALER1 = 0x0;
			PWM_SCALER1 = pwm_ptr->pwm_scale;
			break;
		case 2:
			PWM_SCALER2 = 0x0;
			PWM_SCALER2 = pwm_ptr->pwm_scale;
			break;
		case 3:
			PWM_SCALER3 = 0x0;
			PWM_SCALER3 = pwm_ptr->pwm_scale;
			break;
		default:
			return -1;
	}
	
	/* pwm_ctr */
	switch(pwm_ptr->pwm_ctr_sel)
	{
		case 0:
			PWM_CTR0_1 &= (~(0xFF));
			PWM_CTR0_1 |= (pwm_ptr->pwm_ctr);
			break;
		case 1:
			PWM_CTR0_1 &= (~(0xFF<<8));
			PWM_CTR0_1 |= (pwm_ptr->pwm_ctr<<8);
			break;
		case 2:
			PWM_CTR2_3 &= (~(0xFF));
			PWM_CTR2_3 |= (pwm_ptr->pwm_ctr);
			break;
		case 3:
			PWM_CTR2_3 &= (~(0xFF<<8));
			PWM_CTR2_3 |= (pwm_ptr->pwm_ctr<<8);
			break;
		default:
			return -1;
	}
	/* pwm_clk_sel */
	PWM_CLK_SEL &= (~(0x3<<pwm_ptr->pwm_no*2));
	PWM_CLK_SEL |= ((pwm_ptr->pwm_clk_sel)<<(pwm_ptr->pwm_no*2));
	/* DWORD pwm_ctr_sel */
	PWM_CTR_SEL &= (~(0x3<<pwm_ptr->pwm_no*2));
	PWM_CTR_SEL |= ((pwm_ptr->pwm_ctr_sel)<<(pwm_ptr->pwm_no*2));
	
	/* pwm_dcdr */
	if(pwm_ptr->pwm_no < 2)
	{		
		PWM_DCR0_1 &= (~(0xFF<<(pwm_ptr->pwm_no*8)));
		PWM_DCR0_1 |= (pwm_ptr->pwm_dcdr<<(pwm_ptr->pwm_no*8));
	}
	else if(pwm_ptr->pwm_no < 4)
	{
		PWM_DCR2_3 &= (~(0xFF<<((pwm_ptr->pwm_no-2)*8)));
		PWM_DCR2_3 |= (pwm_ptr->pwm_dcdr<<((pwm_ptr->pwm_no-2)*8));
	}
	else if(pwm_ptr->pwm_no < 6)
	{
		
		PWM_DCR4_5 &= (~(0xFF<<((pwm_ptr->pwm_no-4)*8)));
		PWM_DCR4_5 |= (pwm_ptr->pwm_dcdr<<((pwm_ptr->pwm_no-4)*8));
	}
	else
	{
		PWM_DCR6_7 &= (~(0xFF<<((pwm_ptr->pwm_no-6)*8)));
		PWM_DCR6_7 |= (pwm_ptr->pwm_dcdr<<((pwm_ptr->pwm_no-6)*8));
	}

	//DWORD pwm_step;/*pwm step设置*/
	if(pwm_ptr->pwm_no < 4)
	{
		PWM_STEP3_0 &= (~(0xF<<((pwm_ptr->pwm_no)*4)));
		PWM_STEP3_0 |= (pwm_ptr->pwm_step<<((pwm_ptr->pwm_no)*4));
	}
	else  
	{
		PWM_STEP7_4 &= (~(0xF<<((pwm_ptr->pwm_no-4)*4)));
		PWM_STEP7_4 |= (pwm_ptr->pwm_step<<((pwm_ptr->pwm_no-4)*4));
	}
	
	return 0;
}

//*****************************************************************************
//
//  To read PWM channel [x] config
//
//  parameter :
//          pwm_no : pwm struct
//			Write channel and sel in advance
//			config will write in the pwm struct
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  PWM_Get(spwm_str * pwm_ptr)
{
	if((pwm_ptr == NULL)||(pwm_ptr->pwm_no < 0)||(pwm_ptr->pwm_no  > 7))
	{
		return -1;
	}

	pwm_ptr->prescale = (PWM_CTRL&(0x1<<(pwm_ptr->pwm_no)))>>(pwm_ptr->pwm_no);
	
	pwm_ptr->pwm_polarity = PWM_MODE&(0x1<<(pwm_ptr->pwm_no+0x8))>>(pwm_ptr->pwm_no+0x8);
	
	pwm_ptr->pwm_softmode = (PWM_MODE&(0x1<<(pwm_ptr->pwm_no)))>>(pwm_ptr->pwm_no);
 
	/*scale*/
	switch(pwm_ptr->pwm_clk_sel)
	{
		case 0:
			pwm_ptr->pwm_scale = PWM_SCALER0;
			break;
		case 1:
			pwm_ptr->pwm_scale = PWM_SCALER1;
			break;
		case 2:
			pwm_ptr->pwm_scale = PWM_SCALER2;
			break;
		case 3:
			pwm_ptr->pwm_scale = PWM_SCALER3;
			break;
		default:
			return -1;
	}
 
	/*pwm_ctr*/
	switch(pwm_ptr->pwm_ctr_sel)
	{
		case 0:
			pwm_ptr->pwm_ctr =  (PWM_CTR0_1&0xFF);
			break;
		case 1:
			pwm_ptr->pwm_ctr = (PWM_CTR0_1&(0xFF<<8))>>(8);
			break;
		case 2:
			pwm_ptr->pwm_ctr =  (PWM_CTR2_3&0xFF);
			break;
		case 3:
			pwm_ptr->pwm_ctr = (PWM_CTR2_3&(0xff<<8))>>(8);
			break;
		default:
			return -1;
	}
 
	pwm_ptr->pwm_clk_sel = (PWM_CLK_SEL&(0x3<<(pwm_ptr->pwm_no*2)))>>(pwm_ptr->pwm_no*2);

	pwm_ptr->pwm_ctr_sel = (PWM_CTR_SEL&(0x3<<(pwm_ptr->pwm_no*2)))>>(pwm_ptr->pwm_no*2);

	/*pwm_dcdr*/
	if(pwm_ptr->pwm_no < 2)
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR0_1&(0xff<<(pwm_ptr->pwm_no*8)))>>(pwm_ptr->pwm_no*8);
	}
	else if(pwm_ptr->pwm_no < 4)
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR2_3&(0xff<<((pwm_ptr->pwm_no-2)*8)))>>((pwm_ptr->pwm_no-2)*8);
	}
	else if(pwm_ptr->pwm_no < 6)
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR4_5&(0xff<<((pwm_ptr->pwm_no-4)*8)))>>((pwm_ptr->pwm_no-4)*8);
	}
	else
	{
		pwm_ptr->pwm_dcdr = (PWM_DCR6_7&(0xff<<((pwm_ptr->pwm_no-6)*8)))>>((pwm_ptr->pwm_no-6)*8);
	}

	/*step*/
	if(pwm_ptr->pwm_no < 4)
	{
		pwm_ptr->pwm_step = (PWM_STEP3_0&(0xf<<(pwm_ptr->pwm_no*4)))>>(pwm_ptr->pwm_no*4);
	}
	else
	{
		pwm_ptr->pwm_step = (PWM_STEP7_4&(0xf<<((pwm_ptr->pwm_no-4)*4)))>>((pwm_ptr->pwm_no-4)*4);
	}

	return 0;
}


//*****************************************************************************
//
//  To init PWM channel 0
//
//  parameter :
//          none
//
//  return :
//          Program operation condition
//			 0 : normal
//
//	note :
//			use pwm struct as paremater and pwm_set function to init
//*****************************************************************************
BYTE  PWM_Init_channel(BYTE channel, BYTE val)
{
	spwm_str pwm_set;
	pwm_set.pwm_no = channel;	//使能PWM通道0～7
	pwm_set.prescale = 1;
	pwm_set.pwm_polarity = 0;	//0：输出高有效；1：输出低有效
	pwm_set.pwm_softmode = 0;	//0：禁用step模式；1：使能step模式
	pwm_set.pwm_scale = 1;		//分频设置，输出频率为预分频时钟除以(PWM_SCALER+1)
	pwm_set.pwm_ctr = 100;		//配置CTR周期
	pwm_set.pwm_clk_sel = 0;	//选择时钟CLK0～3
	pwm_set.pwm_ctr_sel = 0;	//选择周期CTR0～3
	pwm_set.pwm_dcdr = val;		//配置占空比
	pwm_set.pwm_step = 0;		//step值
	PWM_Set(&pwm_set);
	PWM_Enable(channel);
	return 0;
}

//*****************************************************************************
//
//  To init PWM channel [x] 
//
//  parameter :
//          none
//
//  return :
//          Program operation condition
//			 0 : normal
//
//	note :
//			use pwm struct as paremater and pwm_set function to init
//*****************************************************************************
BYTE  PWM_Init(BYTE channel, BYTE val)
{
	#if (!PWM_CLOCK_EN)
	return 0;
	#endif
	PWM_Init_channel(channel, val);
	return 0;
}


//*****************************************************************************
//
//  To enable tach channel [x] 
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Enable(BYTE  tach_no)
{
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	TACH_CTRL |= (0x1<<tach_no);
	return 0;
}

//*****************************************************************************
//
//  To disable tach channel [x] 
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Disable(BYTE  tach_no)
{
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	
	TACH_CTRL &= (~(0x1<<tach_no));
	
	return 0;
}

//*****************************************************************************
//
//  To enable tach channel [x] interrupt 
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Int_Enable(BYTE  tach_no)
{
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	TACH_INT |= (0x1<<tach_no);
	return 0;
}

//*****************************************************************************
//
//  To disable tach channel [x] interrupt
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Int_Disable(BYTE  tach_no)
{
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	TACH_INT &= (~(0x1<<tach_no));
	return 0;
}

//*****************************************************************************
//
//  To setup tach channel [x] 
//
//  parameter :
//          stach_str * : tach struct
//
//  return :
//          Program operation condition
//			 0 : normal
//
//*****************************************************************************
BYTE  TACH_Set(stach_str * tach_ptr)
{
	TACH_CTRL |= ((0x1<<tach_ptr->tach_no));
	TACH_CTRL |= (tach_ptr->tach_mode<<((tach_ptr->tach_no*2)+8));
	TACH_INT &= (~(0x1<<(tach_ptr->tach_no)));
	TACH_INT &= (tach_ptr->tach_intr<<(tach_ptr->tach_no));
	return 0;
}

//*****************************************************************************
//
//  To init tach channel [x] 
//
//  parameter :
//          none
//
//  return :
//          Program operation condition
//			 0 : normal
//
//	note:
//			use tach struct and tach_set to init
//*****************************************************************************
DWORD TACH_Init(void)
{
	#if (!PWM_CLOCK_EN)
	return 0;
	#endif
	stach_str tach_set_0;
	tach_set_0.tach_no = 0;
	tach_set_0.tach_mode = 0x1;
	tach_set_0.tach_intr = 0;
	TACH_Set(&tach_set_0);
	return 0;
}

//*****************************************************************************
//
//  To clear tach channel [x] init
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
BYTE  TACH_Clear_IRQ(BYTE  tach_no)
{
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	
	TACH_INT |= (0x1<<(tach_no+4));

	return 0;
}


//*****************************************************************************
//
//  To get tach channel [x] value
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
WORD TACH_Get(BYTE  tach_no)
{
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	
	switch(tach_no)
	{
		case 0:
			return TACH0_DATA;
		case 1:
			return TACH1_DATA;
		case 2:
			return TACH2_DATA;
		case 3:
			return TACH3_DATA;
		default:
			return -1;
			break;
	}	
	return 0;
}



//*****************************************************************************
//
//  To get tach channel [x] value  （用于轮询）
//
//  parameter :
//          tach_no : tach channel
//
//  return :
//          Program operation condition
//			 0 : normal
//			-1 : error
//
//*****************************************************************************
WORD TACH_Get_Polling(BYTE  tach_no)
{
	WORD stats=0;
	char timeout=100;
	if((tach_no < 0)||(tach_no > 3))
	{
		return -1;
	}
	while(--timeout)
	{
		stats=TACH_INT;
		if(stats&(0x1<<(4+tach_no)))
		{
			break;
		}
	}
	if(timeout==0)
	{
		dprint("tach get error\n");
		return -1;
	}
	TACH_Clear_IRQ(tach_no);
	switch(tach_no)
	{
		case 0:
			return TACH0_DATA;
		case 1:
			return TACH1_DATA;
		case 2:
			return TACH2_DATA;
		case 3:
			return TACH3_DATA;
		default:
			return -1;
			break;
	}	
	return 0;
}
