/*
 * @Author: Maple
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-02 21:38:30
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

#include "TEST_TIMEEVENT.H"
#include "CUSTOM_LED.H"
#include "CUSTOM_TEMPERATURE.H"
#include "AE_UPDATE.H"
#include "HARDWARE_CAN.H"
#include "HARDWARE_LPC_MON.H"
#if TEST
extern void CAN_Printf_Receive(void);

/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/
#define SUPPORT_GET_FAN2_RPM            0

//-----------------------------------------------------------------------------
// Oem 1ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_1msEvent(BYTE EventId)
{
#if Support_ANX7447
	ANX_HOOK_1ms();
#endif
    //Custom_S5S0_Sequence();
    //Event_Center(EventId);          // Polling system event 
    //Sys_PowerState_Control();       // System Power Control  
    //KBD_ListeningEvent();
}
//-----------------------------------------------------------------------------
// Oem 5ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_5msEvent(void)
{
    #if SUPPORT_QEvent_Pending
    Get_PendingECQEvent();
    #endif
}

//-----------------------------------------------------------------------------
// Oem 10ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_10msEventA(void)
{
    #if SUPPORT_BREATHING_LED
        Breathing_LED_Switch(0,0.8);
    #endif
}

void Hook_10msEventB(void)
{
#if Support_ANX7447
	ANX_HOOK_10ms();
#endif
}

//-----------------------------------------------------------------------------
// Oem 50ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_50msEventA(void)
{

}

//-----------------------------------------------------------------------------
void Hook_50msEventB(void)
{

}

//-----------------------------------------------------------------------------
void Hook_50msEventC(void)
{
}

//-----------------------------------------------------------------------------
// Oem 100ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_100msEventA(void)
{   

}
//-----------------------------------------------------------------------------
void Hook_100msEventB(void)
{


}
//-----------------------------------------------------------------------------
void Hook_100msEventC(void)
{
    Service_LED_Indicator();

}
//-----------------------------------------------------------------------------
// Oem 500ms Events/Hook Here
//-----------------------------------------------------------------------------
void Hook_500msEventA(void)
{

}
//-----------------------------------------------------------------------------
void Hook_500msEventB(void)
{

    #if CAN0_Receive
        if(CAN0_ReadFlag==1)
        {
        CAN0_ReadFlag=0;
        CAN_Printf_Receive();
        }
    #endif

    #if CAN1_Receive
        if(CAN1_ReadFlag==1)
        {
        CAN1_ReadFlag=0;
        CAN_Printf_Receive();
        }
    #endif

    #if CAN2_Receive
    if(CAN2_ReadFlag==1)
     {
      CAN2_ReadFlag=0;
      CAN_Printf_Receive();
     }
    #endif

    #if CAN3_Receive
        if(CAN3_ReadFlag==1)
        {
        CAN3_ReadFlag=0;
        CAN_Printf_Receive();
        }
    #endif

}
//-----------------------------------------------------------------------------

void Hook_500msEventC(void)
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
void Hook_1secEventA(void)
{

}
//-----------------------------------------------------------------------------
void Hook_1secEventB(void)
{     
    #if SUPPORT_GET_FAN2_RPM
        TACH0_Speed=0x5b8d80/TACH_Get_Polling(0);
        dprint("value:0x%d\n",TACH0_Speed);
    #endif

    //TACH_Int_Enable(0);  

    #if CAN0_Send
    CAN0_SendData(Standard_frame,0x5ab,2,CAN_DATA);
    #endif
    #if CAN1_Send
    CAN1_SendData(Standard_frame,0x5ab,2,CAN_DATA);
    #endif
    #if CAN2_Send
    CAN2_SendData(Standard_frame,0x5ab,2,CAN_DATA);
    #endif
    #if CAN3_Send
    CAN3_SendData(Standard_frame,0x5ab,2,CAN_DATA);
    #endif

}


//-----------------------------------------------------------------------------
void Hook_1secEventC(void)
{
    #if (I2C_MODULE_EN && SUPPORT_I2C_TEMPERATURE)
        get_temperature(1);
    #endif
    dprint("test_1s\n");
}

//-----------------------------------------------------------------------------
// 1 min events
//-----------------------------------------------------------------------------
void Hook_1minEvent(void)
{

}
#endif