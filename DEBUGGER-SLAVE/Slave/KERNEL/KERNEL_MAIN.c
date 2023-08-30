/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-21 21:52:38
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

#include "AE_INCLUDE.H"
#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"

#define printf_instructions_msg " \
\n\
\n\
\n\
**********************************************\n\
*                                            *\n\
    Spksilicon Co., All rights reserved.      \n\
Solutions for Embedded Controller Applications\n\
\n\
           #######    #####    \n\
           #         #     #   \n\
           #         #         \n\
           ######    #         \n\
           #         #         \n\
           #         #     #   \n\
           #######    #####    \n\
\n\
                     Contact us\n\
                     Email:A@spksilicon.com\n\
                     URL:spksilicon.com\n\
*                                           *\n\
*********************************************\n\
\n\
 "


//------------------------------------------------------------
// 1ms events
//------------------------------------------------------------
void Event_1ms(void)
{

    Hook_1msEvent(timer_1ms_count);

}

//-----------------------------------------------------------------------------
// 5ms Events 
//-----------------------------------------------------------------------------
void Event_5ms(void)
{
    Hook_5msEvent();
}

//-----------------------------------------------------------------------------
// 10ms Events 
//-----------------------------------------------------------------------------
void EventA_10ms(void)
{
	Hook_10msEventA();
}

//-----------------------------------------------------------------------------
void EventB_10ms(void)
{
	Hook_10msEventB();
}

//-----------------------------------------------------------------------------
// 50ms Events 
//-----------------------------------------------------------------------------
void EventA_50ms(void)
{
    Hook_50msEventA();
}

//-----------------------------------------------------------------------------
void EventB_50ms(void)
{
    Hook_50msEventB();
}

//-----------------------------------------------------------------------------
void EventC_50ms(void)
{
    Hook_50msEventC();
}

//-----------------------------------------------------------------------------
// 100ms Events 
//-----------------------------------------------------------------------------
void EventA_100ms(void)
{
    Hook_100msEventA();
}

//-----------------------------------------------------------------------------
void EventB_100ms(void)
{
    Hook_100msEventB();
}

//-----------------------------------------------------------------------------
void EventC_100ms(void)
{
     Hook_100msEventC();
}

//-----------------------------------------------------------------------------
// 500ms Events 
//-----------------------------------------------------------------------------
void EventA_500ms(void)
{
    Hook_500msEventA();
}

//-----------------------------------------------------------------------------
void EventB_500ms(void)
{
    Hook_500msEventB();
}

//-----------------------------------------------------------------------------
void EventC_500ms(void)
{
    Hook_500msEventC();
}   

//-----------------------------------------------------------------------------
//1 second events
//-----------------------------------------------------------------------------   
void EventA_1s(void)
{
    Hook_1secEventA();
}

//-----------------------------------------------------------------------------
void EventB_1s(void)
{
    Hook_1secEventB();
}

//-----------------------------------------------------------------------------
void EventC_1s(void)
{
    Hook_1secEventC();
}

//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void Event_1min(void)
{   
    Hook_1minEvent();
}

//----------------------------------------------------------------------------
// FUNCTION: Service_MS_1
// 1ms fundation function
//----------------------------------------------------------------------------
void Service_MS_1(void)
{

	#define Stress_test_printf  0

	#if Stress_test_printf
		for(int i=4096;i;i--)dprint("AAAAA\n");
	#endif

	#if (Service_MS_1_START == 1)
    	if(F_Service_MS_1 == 0)
			return ;

    	else if(F_Service_MS_1 == 1)
    	{
			F_Service_MS_1 = 0;
    	}
		else
		{
			dprint("F_Service_MS_1 is 0x%x ...\n",F_Service_MS_1);
    	}
	Event_1ms();
	timer_1ms_count++;
    if(timer_1ms_count>=10)
    {
        timer_1ms_count = 0;
    }

    if ((timer_1ms_count % 5) == 0)		//5ms
    {
		Event_5ms();
        timer_5ms_count++;
		if ( timer_5ms_count%2==1)  	// 10ms events
	    {
            EventA_10ms();
	    }
	    else
	    {
			EventB_10ms();
		}
        if (timer_5ms_count == 2)
        {
            EventA_50ms();
        }
        else if (timer_5ms_count == 4)
        {
            EventB_50ms();
        }
        else if (timer_5ms_count == 6)
        {
            EventC_50ms();
        }
        else if (timer_5ms_count == 8)
        {
            timer_100ms_count++;
            if (timer_100ms_count%2==1)
            {
                EventA_100ms();
            }
            else
            {
                EventB_100ms();
            }
        }
        else if (timer_5ms_count >= 10)
        {
            timer_5ms_count = 0;
        }
    

		if (timer_5ms_count == 0)			//50ms
    	{
        	timer_50ms_count++;
			if (timer_50ms_count == 2)
			{
				EventC_100ms();
			}
			else if (timer_50ms_count == 4)
			{
				EventA_500ms();
			}
			else if (timer_50ms_count == 6)
			{
				EventB_500ms();
			}
			else if (timer_50ms_count == 8)
			{
				EventC_500ms();
			}
			else if (timer_50ms_count == 10)
			{
				EventA_1s();
			}
			else if (timer_50ms_count == 12)
			{
				EventB_1s();
			}
			else if (timer_50ms_count == 14)
			{
				EventA_500ms();
			}
			else if (timer_50ms_count == 16)
			{
				EventB_500ms();
			}
			else if (timer_50ms_count == 18)
			{
				EventC_500ms();
			}
			else if (timer_50ms_count == 20)
			{
				EventC_1s();
				timer_50ms_count = 0;
				timer_1s_count++;
				if (timer_1s_count == 60)
				{
					Event_1min();
					timer_1s_count = 0;
					timer_1min_count++;
					if(timer_1min_count ==60)
					{
						timer_1min_count = 0;
						timer_1hours_count++;
					}
				}
			}
		}

	} 

#endif
}

//----------------------------------------------------------------------------
// FUNCTION: Service_Reserved1
//----------------------------------------------------------------------------
static void Service_Reserved1(void)
{
#if (Service_Reserved1_START == 1)
	if(F_Service_Reserved1)
	{
		F_Service_Reserved1=0;
	}
#endif
}



//----------------------------------------------------------------------------
// FUNCTION: Service_Reserved2
//----------------------------------------------------------------------------
void Service_Reserved2(void)
{
#if (Service_Reserved2_START == 1)
	if(F_Service_Reserved2)
	{
		F_Service_Reserved2=0;
	}
#endif
}
//----------------------------------------------------------------------------
// FUNCTION: Service_Reserved2
//----------------------------------------------------------------------------
void Service_Reserved3(void)
{
#if (Service_Reserved3_START == 1)
	if(F_Service_Reserved3)
	{
		F_Service_Reserved3=0;
	}
#endif
}
 //----------------------------------------------------------------------------
// FUNCTION: Service_CPU_WakeUp
// CPU Wake Up finction 
//----------------------------------------------------------------------------

void Service_CPU_WakeUp(void)
{
		if(F_Service_WakeUp)
		{
			F_Service_WakeUp = 0;
			// user service()
			// batter service()
			// Check_AC_Plugin();
			// Check_BATTERY_Plugin();
			// GetBatteryINFO();
			dprint("cpu wake service complete!\n");
			// Low_Power_Flag = 1;
			// Save_Context();
			// Low_Power_Set();
			// CPU_Sleep();
			Enter_LowPower_Mode();
		}
}
//----------------------------------------------------------------------------
// FUNCTION: Service_PUTC
// UART putchar support function
//----------------------------------------------------------------------------

void Service_PUTC(void)
{
	if(F_Service_PUTC)return;
	#if SUPPORT_SLAVE_PRINTF
	
	//if(print_number&&(PRINTF_LSR & UART_LSR_TEMP) )
	while(print_number&&(!(PRINTF_LSR&UART_LSR_THRE))) 
	{		
		PRINTF_TX=print_buff[PRINT_SERVICE_CNT];
		//print_buff[PRINT_SERVICE_CNT]='\0';
		PRINT_SERVICE_CNT++;
		print_number--;
		if(PRINT_SERVICE_CNT>=PRINT_MAX_SIZE)PRINT_SERVICE_CNT=0;
	}

	#endif
}

//----------------------------------------------------------------------------
// FUNCTION: Service_CMD
// UART Command support function
//----------------------------------------------------------------------------
void Service_CMD(void)
{
  #if ENABLE_COMMAND_SUPPORT
	if(F_Service_CMD)
	{
		CMD_RUN(&CMD_UART_CNT,(char *)CMD_UART_BUFF);
		F_Service_CMD=0;
		CMD_UART_CNT=0;
	}
  #endif
}

//-----------------------------------------------------------------------------
//  Function Pointers
//-----------------------------------------------------------------------------
/* FUNCT_PTR_V_V is a pointer to a function that returns nothing
   (V for void) and takes nothing for a parameter (V for void). */
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V service_table[] =
{   // Hi-Level Service
	Service_CPU_WakeUp,		// cpu wake up
	Service_PUTC,			//Service printf 	
	Service_CMD,            // CMD Run
    Service_PCI,            // Host send to ec 60/64 Command/Data service
    Service_Send,           // Send byte from KBC to host
    Service_Send_PS2,       // Send PS2 interface pending data
    Service_PCI2,           // PMC1 Host Command/Data service
    Service_MS_1,           // 5 millisecond elapsed for CUSTOM
    Service_KBS,        	// Keyboard scanner service
	Service_Reserved1,      // Reserved
    // Lo-Level Service
    Service_PCI3,           // PMC2 Host Command/Data service
    Service_PCI4,           // PMC3 Host Command/Data service
    Service_MainLoTimer,    // Service Low Level 5ms Timer
    Service_OEM_LoTimer,    // Service Oem Low Level 5ms Timer
    Service_LPCRST,         // Service LPCRST
    Service_PCI5,           // PMC4 Host Command/Data service
    Service_PCI6,           // PMC5 Host Command/Data service
	Service_Reserved2,       // Reserved
#if ENABLE_DEBUGGER_SUPPORT
    Service_Data,
    Service_Change,
    Service_Debugger_Int,
    // KBD_ListeningEvent
	Debugger_Send_KBD,
#endif
	Service_Reserved3,       // Reserved
};

//----------------------------------------------------------------------------
// FUNCTION: main_service
// polling service table
//----------------------------------------------------------------------------
void main_service(void)
{
	F_Service_PUTC=0;
	while(1)
	{	
			if(_R1 >= sizeof(service_table)/sizeof(FUNCT_PTR_V_V))
				_R1 = 0;
		(service_table[_R1])();
		_R1++; 
	}

}

//----------------------------------------------------------------------------
// FUNCTION: main loop
// enter main loop
//----------------------------------------------------------------------------
int main_loop(void)
{
	dprint("Enter main_service \n");
	main_service();
}

//----------------------------------------------------------------------------
// FUNCTION: main
// edevice enable
//----------------------------------------------------------------------------
void device_enable(void)
{
#if Support_ANX7447
	ucsi_init();
#endif

	Init_GPIO();

}

static int mouse_num = 1;
static int keyboard_num = 1;
void test_kbc_mouse_keyboard()
{
	dprint("Enter test_kbc_mouse_keyboard \n");

	while(1)
	{

		dprint("before while mouse_num is 0x%x\n",mouse_num);
		while( Is_FLAG_SET(KBC_STA,KBC_STA_OBF) || Is_FLAG_SET(KBC_STA,KBC_STA_IBF));
		vDelayXms(1);

		if((mouse_num %2) == 1) 
		{
			CLEAR_BIT(KBC_STA,4);      //Keyboard Disabled
			SET_BIT(KBC_STA,5);
		}else{
			CLEAR_BIT(KBC_STA,5);      //Keyboard Disabled
			SET_BIT(KBC_STA,4);
		}

		KBC_CTL = 0x01;
		
		if((mouse_num % 2) ==1)
		{
				KBC_KOB = 0x5a;
		}else{
				KBC_KOB = 0x78;
		}

		mouse_num ++;	
		dprint("after while mouse_num is 0x%x\n",mouse_num);

		if( (mouse_num % 1000) == 0 )
			dprint("mouse_num is 0x%x\n",mouse_num);
		
		if( (keyboard_num %1000) == 0)
			dprint("keyboard_num is 0x%x\n",keyboard_num);

		if(keyboard_num == 0x1000000)
			keyboard_num = 0;

		if(mouse_num == 0x1000000)
			mouse_num = 0;
	}

}

//----------------------------------------------------------------------------
// FUNCTION: main
// main entry
//---------------------------------------------------------------------------- 

int __weak main(int argc, char **argv)
{
	unsigned long tmp;
	int ret = 0;
	F_Service_PUTC=1;
	dprint (printf_instructions_msg);
	F_Service_PUTC=0;
	config_irqc_irqs();
	
	set_csr(mstatus, MSTATUS_MIE);			// Enable interrupts in general.

	device_enable();

	//OEM_INIT_ACPI();


	dprint("this is %s\n",__FUNCTION__);
	
	//Oem_Initialization();

	#if SUPPORT_LD_PNP
		LogicalDevice_PNP_Config();
	#endif

	#if SUPPORT_SHAREMEM_PNP
		LPC_Update_Config_Init();
	#endif
	//RunSPIF_WriteFromRAM();
	#ifdef AE103
	if(REG32(0x3047C)&BIT1)dprint("This is external flash\n");
	else dprint("This is internal flash\n");
	#endif
  	init_memory_var();//memory init

	main_loop();

	return 0;
}

