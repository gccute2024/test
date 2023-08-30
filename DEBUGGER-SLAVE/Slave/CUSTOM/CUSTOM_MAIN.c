/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-03 18:30:27
 * @Description: Custom function support
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

#include "CUSTOM_MAIN.H"

#include "CUSTOM_EVENT.H"
#include "CUSTOM_TEMPERATURE.H"
#include "CUSTOM_POWER.H"
#include "CUSTOM_PMC1.H"
#include "CUSTOM_LED.H"
#include "AE_UPDATE.H"
#include "KERNEL_SLEEP_RESET.H"

extern void CAN_Printf_Receive(void);

extern void A20GATE_ON(void);//zb 10.26 add

static BYTE sec_cnt = 0;
/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_GET_FAN1_RPM            0
#define SUPPORT_GET_FAN2_RPM            0

//-----------------------------------------------------------------------------
// Oem 1ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_1msEvent(BYTE EventId)
{
#if Support_ANX7447
	ANX_HOOK_1ms();
#endif
    Event_Center(EventId);          // Polling system event
    Sys_PowerState_Control();       // System Power Control
    //KBD_ListeningEvent();
}
//-----------------------------------------------------------------------------
// Oem 5ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_5msEvent(void)
{
    #if SUPPORT_QEvent_Pending
    Get_PendingECQEvent();
    #endif
}

//-----------------------------------------------------------------------------
// Oem 10ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_10msEventA(void)
{
    #if SUPPORT_BREATHING_LED
        Breathing_LED_Switch(0,1);
    #endif
}

void __weak Hook_10msEventB(void)
{
#if Support_ANX7447
	ANX_HOOK_10ms();
#endif
}

//-----------------------------------------------------------------------------
// Oem 50ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_50msEventA(void)
{

}

//-----------------------------------------------------------------------------
void __weak Hook_50msEventB(void)
{

}

//-----------------------------------------------------------------------------
void __weak Hook_50msEventC(void)
{
}

//-----------------------------------------------------------------------------
// Oem 100ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_100msEventA(void)
{   

}
//-----------------------------------------------------------------------------
void __weak Hook_100msEventB(void)
{


}
//-----------------------------------------------------------------------------
void __weak Hook_100msEventC(void)
{
    Service_LED_Indicator();

}
//-----------------------------------------------------------------------------
// Oem 500ms Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_500msEventA(void)
{

}
//-----------------------------------------------------------------------------
void __weak Hook_500msEventB(void)
{

}
//-----------------------------------------------------------------------------

void __weak Hook_500msEventC(void)
{
    //record kbc communicattion date to sram when 0x31fff is 0xBB
    #if SUPPORT_8042DEBUG_OUTPUT
        if((*((volatile uint8_t *)(0x31fff)) == 0xBB))
         {
          *((volatile uint8_t *)(0x31fff)) = 0x0;
          for(int i=0;i<Debug_Num;i++)
           {
           dprint("0x%x\n", *((volatile uint8_t *)(0x31000+i)));
           *((volatile uint8_t *)(0x31000+i)) = 0x0;
           }
          Debug_Num = 0;
          dprint("debug sram data output finish!\n");
         }
	#endif

	#if SUPPORT_FIRMWARE_UPDATE
	Flash_Update_Function();
	#endif
}
//-----------------------------------------------------------------------------
// Oem 1sec Events/Hook Here
//-----------------------------------------------------------------------------
void __weak Hook_1secEventA(void)
{
    // printf("sec_cnt:%x\n",sec_cnt);
    // sec_cnt++;
    // if(sec_cnt == 10)
    // {
    //     sec_cnt = 0;
    //     RTC_Deep_Sleep_Test();
    //     //Enter_LowPower_Mode();
    // }

}
//-----------------------------------------------------------------------------
void __weak Hook_1secEventB(void)
{     
    #if SUPPORT_GET_FAN2_RPM
        DWORD Polling0 = TACH_Get_Polling(0);
        if(Polling0 != -1)
        {
            TACH0_Speed=(DWORD)6000000/Polling0;
            dprint("FAN2 RPM is %u\n",TACH0_Speed);
        }
    #endif

    //TACH_Int_Enable(0);  
}


//-----------------------------------------------------------------------------
void __weak Hook_1secEventC(void)
{
    #if (I2C_MODULE_EN && SUPPORT_I2C_TEMPERATURE)
        get_temperature(0);
    #endif

}

//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void __weak Hook_1minEvent(void)
{

}

//-----------------------------------------------------------------------------
// End of Handle Timer Events
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION: Service_OEM_LoTimer (Low Level 5ms handle)
//
//-----------------------------------------------------------------------------
void Service_OEM_LoTimer(void)
{
    ;
}

//-----------------------------------------------------------------------------
// FUNCTION: Service_WaitPS2_Handle
//
//-----------------------------------------------------------------------------
void Service_WaitPS2_Handle(void)
{

}

/* ----------------------------------------------------------------------------
 * FUNCTION: Service_LPCRST
 * LPC RESET
 * ------------------------------------------------------------------------- */
void Service_LPCRST(void)
{
    if(F_Service_LPCRST == 1)
    {
        F_Service_LPCRST = 0;    
        A20GATE_ON();		//FIXME xia
        OS_ACPI_Mode = 0;
        //Host_Flag = 0x38;
        Host_Flag_DISAB_KEY = 1;
        Host_Flag_DISAB_AUX = 1; 
    }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const BYTE   _ACPI_INI_Table[] =
{   
//  0x0  0x1  0x2  0x3  0x4  0x5  0x6  0x7  0x8  0x9  0xA  0xB  0xC  0xD  0xE  0xF
    0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,  //0x0
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x1
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x2
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x3
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x4
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x5
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x6
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x8
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0x9
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0xA
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0xB
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0xC
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0xD
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0xE
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //0xF
};

void OEM_INIT_ACPI(void)
{
    //Sample Code
    BYTE index = 0x00;

    while(index < ( sizeof(_ACPI_INI_Table)))
    {
        Tmp_XPntr = (BYTEP )( ECSPACE_BASE_ADDR + index );
        *Tmp_XPntr = _ACPI_INI_Table[index];
        index ++; 
    }

}

