/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:23:34
 * @Description: This file is used for LED custom
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

#include "CUSTOM_LED.H"

#include "CUSTOM_POWER.H"
/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
//#define

/*-----------------------------------------------------------------------------
 * Local Process Definition
 *---------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------
 * FUNCTION: OEM_Write_Leds
 *
 * Write to SCROLL LOCK, NUM LOCK, CAPS LOCK, and any custom LEDs (including
 * an "Fn" key LED).
 *
 * Input: data (LED_ON bit) = Data to write to the LEDs
 *         bit 0 - SCROLL, Scroll Lock
 *         bit 1 - NUM,    Num Lock
 *         bit 2 - CAPS,   Caps Lock
 *         bit 3 - OVL,    Fn LED
 *         bit 4 - LED4,   Undefined
 *         bit 5 - LED5,   Undefined
 *         bit 6 - LED6,   Undefined
 *         bit 7 - LED7,   Undefined
 *
 * Updates Led_Data variable and calls send_leds if appropriate.
 * ------------------------------------------------------------------------- */

void SCROLED_ON()
{

}

void SCROLED_OFF()
{



}

void NUMLED_ON()
{


}

void NUMLED_OFF()
{

}

void CAPLED_ON()
{

}


void CAPLED_OFF()
{

}


void OEM_Write_Leds(void)
{
    #if SUPPORT_LED_SCROLLOCK
    if ( Led_Data_SCROLL )  // Check ScrollLock LED Status
    {
        SCROLED_ON();
    }
    else
    {
        SCROLED_OFF();
    }
    #endif

    #if SUPPORT_LED_NUMLOCK
    if ( Led_Data_NUM ) // Check NumLock LED Status
    {
        NUMLED_ON();
    }
    else
    {
        NUMLED_OFF();
    }
    #endif

    #if SUPPORT_LED_CAPSLOCK
    if ( Led_Data_CAPS )    // Check CapsLock LED Status
    {
        CAPLED_ON();
    }
    else
    {
        CAPLED_OFF();
    }
    #endif

}

void PWRLED1_ON()
{


}

void PWRLED1_TG()
{

}

int CARD_LED_ON()
{
	return 0;
}

int CARD_LED_OFF()
{/*zb add*/
	return 0;
}

int STATE_CARD()
{
	return 0;
}

int BATLED1_TG()
{
	return 0;
}

void Service_LED_Indicator(void)
{

    if (System_PowerState == SYSTEM_S5)
    {
        return;
    }

    if (ACPI_STATE_S0)
    {
        PWRLED1_ON();
    }
    else if (System_PowerState == SYSTEM_S3)
    {
        PWRLED1_TG();
    }

    if( STATE_CARD() )
    {
        CARD_LED_OFF(); //Hi->No Card
    }
    else
    {
        CARD_LED_ON();
    }
    //BATLED0_TG();   //BLUE COLOR
    BATLED1_TG();   //YELLOW COLOR
}

//*****************************************************************************
//
//  Breathing LED
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
static int divert_flag = -1;
//static float pwm_step;
BYTE  Breathing_LED_Switch(BYTE  channel,BYTE step)
{
	/*BYTE  integer_step;
	pwm_step = pwm_step + step;
	integer_step = (BYTE )pwm_step;
	pwm_step = pwm_step - integer_step;*/

	spwm_str *pwm_set_led;
	pwm_set_led = malloc(sizeof(spwm_str));
	memset(pwm_set_led, 0, sizeof(spwm_str));

	pwm_set_led->pwm_no = channel;
	if(PWM_Get(pwm_set_led)){   //get pwm dcdr
        dprint("PWM_Get error!");
        return -1;
    }
	if(divert_flag == -1 && pwm_set_led->pwm_dcdr < 3)
	{
		divert_flag = 1;
	}
	else if(divert_flag == 1 && pwm_set_led->pwm_dcdr > 96)
	{
		divert_flag = -1;
	}
	pwm_set_led->prescale = 0;
	pwm_set_led->pwm_polarity = 0;
	pwm_set_led->pwm_softmode = 0;
	pwm_set_led->pwm_scale = 0;//3Mhz
	pwm_set_led->pwm_clk_sel = 0;
	pwm_set_led->pwm_ctr_sel = 0;
	pwm_set_led->pwm_step = 0;
	pwm_set_led->pwm_ctr = 100;
	pwm_set_led->pwm_dcdr += divert_flag * step;
	PWM_Set(pwm_set_led);//set pwm dcdr
	PWM_Enable(channel);
	free(pwm_set_led);
	return 0;
}


//-----------------------------------------------------------------------------
// Oem LED Here
//-----------------------------------------------------------------------------






/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
