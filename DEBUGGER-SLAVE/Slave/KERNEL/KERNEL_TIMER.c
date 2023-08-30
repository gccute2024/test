/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-10 17:25:58
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

#include "KERNEL_TIMER.H"

#include "AE_FUNC.H"	
#include "KERNEL_ACPI.H" //电源管理

/**
 * @brief reload timer count
 *
 * @param index : which timer to reload, must be 0~3.
 * @param load_count : count to reload
 */
void TIMER_Reload (u8 index, u16 load_count)
{
	my_write8(TIMER_BASE_ADDR, 0x14 * index + TIMERn_TLC, load_count&0xFF);
	my_write8(TIMER_BASE_ADDR, 0x14 * index + TIMERn_TLC + 0x1, (load_count>>8) & 0xFF );
}


/**
 * @brief TIMER初始化
 *
 * @param index        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 * @param load_count   重装载值  
 * @param loop_enable  1：循环定时模式  0：单次定时模式
 * @param int_mask     1：屏蔽定时中断  0：不屏蔽定时中断
 *
 * @return 
 */
int TIMER_Init(u8 index, u32 load_count, u8 loop_enable, u8 int_mask)
{
#if !(TIMER0_CLOCK_EN | TIMER1_CLOCK_EN | TIMER3_CLOCK_EN | TIMER4_CLOCK_EN)
	return 0;
#endif
	volatile u8 ena_val;
	u8 i;
	u8 control_val;

	//enable TIMER 
	//timer_enable(index, 1);

	//set control value.	
#if 1
	control_val = (0x1 << 0) | (loop_enable << 1) | (int_mask << 2);
	TIMER_Reload(index, load_count);
	my_write8(TIMER_BASE_ADDR, 0x14 * index + TIMERn_TCR, control_val);
#else

	my_write8(TIMER_BASE_ADDR, TIMERn_TCR, 0x2);
	TIMER_Reload(0, 0x50);
	my_write8(TIMER_BASE_ADDR,TIMERn_TCR, 0x3);

#endif
		
    return 0;
}

/**
 * @brief 关闭timer定时器
 *
 * @param index        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 *
 * @return 
 */
int TIMER_Disable(u8 index)
{
	u8 control_val;
	control_val = 0x0;
	my_write8(TIMER_BASE_ADDR, 0x14 * index + TIMERn_TCR, control_val);
	return 0;
}

/**
 * @brief 使能timer定时器
 *
 * @param index        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 * @param enable       0：关闭定时器  1：打开定时器
 *
 * @return 
 */
void TIMER_Enable(u8 index , u8 enable)
{
   int offset;
   int control_reg;
	   offset = 0x14 * index;
	   control_reg	= my_readl(TIMER_BASE_ADDR , TIMERn_TCR + offset);
	   	if (enable == 0)
		   {
			   control_reg = control_reg & ~(0x01);
		   }
		else
		   {
			   control_reg = control_reg | 0x01;
		   }
	   my_write8(control_reg, TIMER_BASE_ADDR , TIMERn_TCR + offset);
}

/**
 * @brief timer定时中断屏蔽
 *
 * @param index        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 * @param int_mask     0：不屏蔽定时中断  1：屏蔽定时中断
 *
 * @return 
 */
void TIMER_Int_Mask(u8 index, u8 int_mask)
{
	int offset;
	int control_reg;
	offset = 0x14 * index;
	control_reg	= my_readl(TIMER_BASE_ADDR , TIMERn_TCR + offset);
	if (int_mask == 0)
	{
		control_reg = control_reg & ~(0x04);
	}
	else
	{
		control_reg = control_reg | 0x04;
	}
	my_write8(control_reg, TIMER_BASE_ADDR , TIMERn_TCR + offset);

}


void InternalWDTNow(void)
{
	//reserved
	//WDT reset chip
	//while(1);
}

/**
 * @brief timer定时中断使能
 *
 * @param channel        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 *
 * @return 
 */
BYTE  Timer_Int_Enable(BYTE  channel)
{
	switch(channel)
	{
		case TIMER0:
			TIMER0_TCR |= (0x1<<2);
			return 0;
		case TIMER1:
			TIMER1_TCR |= (0x1<<2);
			return 0;
		case TIMER2:
			TIMER2_TCR |= (0x1<<2);
			return 0;
		case TIMER3:
			TIMER3_TCR |= (0x1<<2);
			return 0;
		default:
			return -1;
	}
}

/**
 * @brief timer定时中断屏蔽
 *
 * @param channel        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 *
 * @return 
 */
BYTE  Timer_Int_Disable(BYTE  channel)
{
	switch(channel)
	{
		case TIMER0:
			TIMER0_TCR &= ~(0x1<<2);
			return 0;
		case TIMER1:
			TIMER1_TCR &= ~(0x1<<2);
			return 0;
		case TIMER2:
			TIMER2_TCR &= ~(0x1<<2);
			return 0;
		case TIMER3:
			TIMER3_TCR &= ~(0x1<<2);
			return 0;
		default:
			return -1;
	}
}

/**
 * @brief 读出timer中断是否使能
 *
 * @param channel        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 *
 * @return 1：屏蔽中断  0：允许中断  -1：错误
 */
BYTE  Timer_Int_Enable_Read(BYTE  channel)
{
	switch(channel)
	{
		case TIMER0:
			return (TIMER0_TCR & (0x1<<2) != 0);
		case TIMER1:
			return (TIMER1_TCR & (0x1<<2) != 0);
		case TIMER2:
			return (TIMER2_TCR & (0x1<<2) != 0);
		case TIMER3:
			return (TIMER3_TCR & (0x1<<2) != 0);
		default:
			return -1;
	}
}

/**
 * @brief 读出timer中断状态
 *
 * @param channel        timer0-4选择  可选参数：0x0 0x1 0x2 0x3 分别对应timer0-4选择
 *
 * @return 1：有中断  0：无中断  -1：错误
 */
BYTE  Timer_Int_Status(BYTE  channel)
{
	switch(channel)
	{
		case TIMER0:
			return (TIMER0_TIS & (0x1<<0) != 0);
		case TIMER1:
			return (TIMER1_TIS & (0x1<<0) != 0);
		case TIMER2:
			return (TIMER2_TIS & (0x1<<0) != 0);
		case TIMER3:
			return (TIMER3_TIS & (0x1<<0) != 0);
		default:
			return -1;
	}
}

//-----------------------------------------------------------------------------
// Delay 1ms function use timer 1
//-----------------------------------------------------------------------------
void vDelayXms(WORD bMS)
{
	
    TIMER_Disable(TIMER1);
    for (; bMS != 0; bMS--)
    {   
	    TIMER_Init(TIMER1, TIMER1_1ms, 0x0, 0x1);
        while ((TIMER_TRIS & BIT(TIMER1)) == 0);
        TIMER1_TEOI;
        TIMER_Disable(TIMER1);
    }
}


//----------------------------------------------------------------------------
// Delay Xus function                  
//----------------------------------------------------------------------------
void vDelayXus(BYTE bUS)
{       
	TIMER_Disable(TIMER3);
    TIMER_Init(TIMER3, TIMER3_1us * bUS, 0x0, 0x1);
	while ((TIMER_TRIS & BIT(TIMER3)) == 0);
	TIMER3_TEOI;
	TIMER_Disable(TIMER3);
}

void wait_seconds(size_t n)
{
	unsigned long start_mtime, delta_mtime;
	// Don't start measuruing until we see an mtime tick
	unsigned long tmp = mtime_lo();
	do{
	start_mtime = mtime_lo();
	}while(start_mtime == tmp);

	do{
	delta_mtime = mtime_lo() - start_mtime;
	}while(delta_mtime < (n * TIMER_FREQ));
//  dprint("-----------------Waited %d seconds.\n", n);
}

//-----------------------------------------------------------------------------
// Low Level Timer 5ms Handle
//-----------------------------------------------------------------------------
void Service_MainLoTimer(void)
{
#if 1
    VBYTE bTemp=FALSE;

    SMB_Check_Timer++;

    if( SMB_Check_Timer > 20 )
    {
        SMB_Check_Timer = 0;
    }

    //-------------------------------------------------------------------------
    if( !ACPI_STATE_S0 )   //System Power On Flag
    {
        #if ENABLE_S5_KEYBOARD_INTR
        if( ACPI_STATE_S5 )        bTemp = TRUE;
        #endif
        #if ENABLE_S3_KEYBOARD_INTR
        if( ACPI_STATE_S3 )        bTemp = TRUE;
        #endif
        if( bTemp )
        {
 //           IER1 |= 0x08;               //Enable KSI interrupt FIXME

        }
        return;
    }
    //-------------------------------------------------------------------------
    // CORE S0 (Host Power On)
    //-------------------------------------------------------------------------
    #if Service_EVENT_CENTER_START
    Service_Event_Center();
    #endif
    //-------------------------------------------------------------------------
  //  IER1 |= 0x08;               // Enable KSI interrupt
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------

    if( KBPendingRXCount > 0 )
    {
		//F_Service_Send = 1;
    }
    //-------------------------------------------------------------------------
    if( !( Host_Flag_DISAB_KEY )   )
    {
        if( (KBD_SCAN.head != KBD_SCAN.tail) )
        {
			//   F_Service_Send = 1;
        }
    }
#endif

}