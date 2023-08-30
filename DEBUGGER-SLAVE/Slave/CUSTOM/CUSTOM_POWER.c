/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-08 16:36:38
 * @Description: Power sequnce control function example
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

#include "CUSTOM_POWER.H"
#include "CUSTOM_GPIO.H"
#include "KERNEL_PWM.H"
#include "KERNEL_PS2.H"

/***************Power Sequence Debugger Information Record ******************/
sPowSeq_Pin_Record PowSeq_Record[40];

/*******************************************************/
//记录每条时序动作信息到PowSeq_Record_To_Array
/*******************************************************/
void PowerSequence_Record_To_Array(char* group, BYTE pin, BYTE level, WORD delay)
{

    PowSeq_Record_Cnt++;

    //Record GPIO Group information

    if(!strcmp(group,"PIOA"))
    {
        PowSeq_Record[PowerSequence_Step].pin_group = 0x0;
    }else if(!strcmp(group,"PIOB"))
    {
        PowSeq_Record[PowerSequence_Step].pin_group = 0x1;
    }else if(!strcmp(group,"PIOC"))
    {
        PowSeq_Record[PowerSequence_Step].pin_group = 0x2;
    }else if(!strcmp(group,"PIOD"))
    {
        PowSeq_Record[PowerSequence_Step].pin_group = 0x3;

    }else if(!strcmp(group,"PIOE"))
    {
        PowSeq_Record[PowerSequence_Step].pin_group = 0x4;
    }else{
        dprint("GPIO group input error\n");
    }
    //Record GPIO Pin Information
    switch(pin)
    {
        case 0x0:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x0;
                break;
        case 0x1:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1;
                break;
        case 0x2:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x2;
                break;
        case 0x3:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x3;
                break;
        case 0x4:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x4;
                break;
        case 0x5:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x5;
                break;
        case 0x6:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x6;
                break;
        case 0x7:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x7;
                break;
        case 0x8:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x8;
                break;
        case 0x9:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x9;
                break;
        case 0xA:
                PowSeq_Record[PowerSequence_Step].pin_location = 0xA;
                break;
        case 0xB:
                PowSeq_Record[PowerSequence_Step].pin_location = 0xB;
                break;
        case 0xC:
                PowSeq_Record[PowerSequence_Step].pin_location = 0xC;
                break;
        case 0xD:
                PowSeq_Record[PowerSequence_Step].pin_location = 0xD;
                break;
        case 0xE:
                PowSeq_Record[PowerSequence_Step].pin_location = 0xE;
                break;
        case 0xF:
                PowSeq_Record[PowerSequence_Step].pin_location = 0xF;
                break;
        case 0x10:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x10;
                break;
        case 0x11:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x11;
                break;
        case 0x12:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x12;
                break;
        case 0x13:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x13;
                break;
        case 0x14:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x14;
                break;
        case 0x15:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x15;
                break;
        case 0x16:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x16;
                break;
        case 0x17:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x17;
                break;
        case 0x18:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x18;
                break;
        case 0x19:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x19;
                break;
        case 0x1A:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1A;
                break;
        case 0x1B:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1B;
                break;
        case 0x1C:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1C;
                break;
        case 0x1D:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1D;
                break;
        case 0x1E:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1E;
                break;
        case 0x1F:
                PowSeq_Record[PowerSequence_Step].pin_location = 0x1F;
                break;
        default:
                dprint("gpio pin input error\n");
                break;
    }



    //Record GPIO Output Level Information

    if(level == 0)
    {
        PowSeq_Record[PowerSequence_Step].pin_level = 0x0; 
    }else if(level == 1)
    {
        PowSeq_Record[PowerSequence_Step].pin_level = 0x1;
    }else
    {
        dprint("gpio level input error\n");
    }

    //Record GPIO Delay Information

    PowSeq_Record[PowerSequence_Step].pin_delay = delay;



    //printf("PowerSequence_Record_To_Array\n");

}

/*******************************************************/
//将时序记录信息拷贝到SRAM中，以便调试器读取该信息
/*******************************************************/
extern void Copy_Power_Sequence_Record_To_Sram(VBYTE Record_Flag);

void Copy_Power_Sequence_Record_To_Sram(VBYTE Record_Flag)
{
    BYTE * PowSeq_Mem_Ptr = (BYTE *)0x31002;
    BYTE length_record;

    /*Record PowerSequence type flag to 0x31000*/
    switch(Record_Flag)
    {
        case 1:
                *((VBYTE *)(0x31000)) = 0x1;
                break;
        case 2:
                *((VBYTE *)(0x31000)) = 0x2;
                break;
        case 3:
                *((VBYTE *)(0x31000)) = 0x3;
                break;
        case 4:
                *((VBYTE *)(0x31000)) = 0x4;
                break;
        case 5:
                *((VBYTE *)(0x31000)) = 0x5;
                break;
        case 6:
                *((VBYTE *)(0x31000)) = 0x6;
                break;
        default:
                *((VBYTE *)(0x31000)) = 0x0;
                break;    
    }

    /*Record PowerSequence Count to 0x31001 */
    *((VBYTE *)(0x31001)) = PowSeq_Record_Cnt;

    /*Copy PowerSequence information to Sram*/
    length_record = sizeof(PowSeq_Record);
    memcpy(PowSeq_Mem_Ptr, PowSeq_Record, length_record);
    //printf("memcpy to sram\n");
}

//-----------------------------------------------------------------------------
// The example of S5 -> S0 sequence for example
//-----------------------------------------------------------------------------
BYTE S5S0_EXAMPLE_1(void)
{
    /*仅为示例时序，需根据实际需求进行修改*/
    //This is S5S0 power sequence 1   
    ALW_ON(); //操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array("PIOA", 7, 1, 5); //record ALW_ON for debugger
	return 0;
}
BYTE S5S0_EXAMPLE_2(void)
{
    //This is S5S0 power sequence 2
    ALW_OFF(); //操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array("PIOA", 7, 0, 10); //record ALW_OFF for debugger 

	return 0;
}
BYTE S5S0_EXAMPLE_3(void)
{
    //This is S5S0 power sequence 3   
    HOT_LED_H(); //操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array("PIOA", 31, 1, 5); //record HOT_LED_H for debugger
	return 0;
}
BYTE S5S0_EXAMPLE_4(void)
{
    //This is S5S0 power sequence 4
    HOT_LED_L(); //操作GPIO的时序函数需要按照后缀为_ON、_OFF、_H、_L的格式来编写，以便调试器识别
    PowerSequence_Record_To_Array("PIOC", 20, 0, 10); //record HOT_LED_L for debugger
	return 0;
}
BYTE S5S0_EXAMPLE_5(void)
{
    //This is S5S0 power sequence 5
    if(Read_ALW_PWRGD()) //调试器不需要识别状态判定函数
	{
		return(1);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// The example of S0 -> S5 sequence
//-----------------------------------------------------------------------------
BYTE S0S5_EXAMPLE_1(void)
{
	//This is S5S0_EXAMPLE 1
	return 0;
}
BYTE S0S5_EXAMPLE_2(void)
{
	//This is S5S0_EXAMPLE 2
	return 0;
}

//-----------------------------------------------------------------------------
// The example of S4 -> S0 sequence
//-----------------------------------------------------------------------------
BYTE S4S0_EXAMPLE_1(void)
{
	//This is S5S0_EXAMPLE 1
	return 0;
}
BYTE S4S0_EXAMPLE_2(void)
{
	//This is S5S0_EXAMPLE 2
	return 0;
}

//-----------------------------------------------------------------------------
// The example of S0 -> S4 sequence
//-----------------------------------------------------------------------------
BYTE S0S4_EXAMPLE_1(void)
{
	//This is S5S0_EXAMPLE 1
	return 0;
}
BYTE S0S4_EXAMPLE_2(void)
{
	//This is S5S0_EXAMPLE 2
	return 0;
}

//-----------------------------------------------------------------------------
// The example of S3 -> S0 sequence
//-----------------------------------------------------------------------------
BYTE S3S0_EXAMPLE_1(void)
{
	//This is S5S0_EXAMPLE 1
	return 0;
}
BYTE S3S0_EXAMPLE_2(void)
{
	//This is S5S0_EXAMPLE 2
	return 0;
}

//-----------------------------------------------------------------------------
// The example of S0 -> S3 sequence
//-----------------------------------------------------------------------------
BYTE S0S3_EXAMPLE_1(void)
{
	//This is S5S0_EXAMPLE 1
	return 0;
}
BYTE S0S3_EXAMPLE_2(void)
{
	//This is S5S0_EXAMPLE 2
	return 0;
}

//-----------------------------------------------------------------------------
//  The CUSTOM variable of Sx -> S0
//-----------------------------------------------------------------------------
void Custom_SX_S0_Var(void)
{
    PowerState_Monitor_Index = 0x00;
}

//-----------------------------------------------------------------------------
//  The variable of Sx -> S0
//-----------------------------------------------------------------------------
void SX_S0_Var(void)
{
	System_PowerState=SYSTEM_S0;
    Custom_SX_S0_Var();
}

//-----------------------------------------------------------------------------
//  The CUSTOM variable of S0 -> Sx
//-----------------------------------------------------------------------------
void Custom_S0_SX_Var(void)
{
    PowerState_Monitor_Index = 0x00;
    CLEAR_MASK(SYSTEM_MISC1,ACPI_OS);
}

//-----------------------------------------------------------------------------
//  The variable of S0 -> Sx
//-----------------------------------------------------------------------------
void S0_SX_Var(void)
{
	CLEAR_MASK(KBC_STA,KBC_STA_GPF);
    Host_Flag_SYS_FLAG = 0;
    Custom_S0_SX_Var();
}

//-----------------------------------------------------------------------------
// The variable of S5 -> S0
//-----------------------------------------------------------------------------
BYTE S5_S0_Variable(void)
{
    dprint("S5_S0_Variable\n");
	SX_S0_Var();
	return 0;
}

//-----------------------------------------------------------------------------
// The variable of S4 -> S0
//-----------------------------------------------------------------------------
BYTE S4_S0_Variable(void)
{
    SX_S0_Var();
	return 0;
}

//-----------------------------------------------------------------------------
// The variable of S3 -> S0
//-----------------------------------------------------------------------------
BYTE S3_S0_Variable(void)
{
	SX_S0_Var();
	return 0;
}

//-----------------------------------------------------------------------------
//  The variable of S0 -> S5
//-----------------------------------------------------------------------------
BYTE S0_S5_Variable(void)
{
    System_PowerState=SYSTEM_S5;
    S0_SX_Var();
	return 0;
}

//-----------------------------------------------------------------------------
//  The variable of S0 -> S4
//-----------------------------------------------------------------------------
BYTE S0_S4_Variable(void)
{
    System_PowerState=SYSTEM_S4;
    S0_SX_Var();
	return 0;    
}

//-----------------------------------------------------------------------------
// The variable of S0 -> S3
//-----------------------------------------------------------------------------
BYTE S0_S3_Variable(void)
{
	System_PowerState=SYSTEM_S3;
	S0_SX_Var();
	return 0;    
}

//-------------------------------------------------------------------------------------
//  Do S5 -> S0 power sequnce
//  S5S0时序仅为支持调试器的示例时序，客户可根据需求更改时序
//  指示信息-> 0：GPIO时序 1：GPIO输入判定状态 2：其他执行函数，如配置PNP，修改变量等，不涉及时序
//-------------------------------------------------------------------------------------
const sPowerSEQ  PowerSequence_S5S0[]=  
{
    //函数指针              延时  指示信息
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S5S0_EXAMPLE_1, 		5,      0,  },
	{ S5S0_EXAMPLE_2, 	    10,     0,  },
    { S5S0_EXAMPLE_3, 		5,      0,  },
	{ S5S0_EXAMPLE_4, 	    10,     0,  },
    { S5S0_EXAMPLE_5, 	    0,      1,  },
	{ LD_PNP_Config, 		0,      2,  },
	{ S5_S0_Variable, 	    0,		2,  },	// always in the end of tabel
};

//-------------------------------------------------------------------------------------
//  Do S0 -> S5 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ  PowerSequence_S0S5[]=  
{
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S0S5_EXAMPLE_1, 		0,      0,  },
	{ S0S5_EXAMPLE_2, 		0,      0,  },
	{ S0_S5_Variable, 	    0,		0,  },  // always in the end of tabel	
}; 

//-------------------------------------------------------------------------------------
//  Do S4 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ  PowerSequence_S4S0[]= 
{
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S4S0_EXAMPLE_1, 		0,      0,  },
	{ S4S0_EXAMPLE_2, 		0,      0,  },
    { S4_S0_Variable, 	    0,		0	},	// always in the end of tabel
}; 

//-------------------------------------------------------------------------------------
//  Do S0 -> S4 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ  PowerSequence_S0S4[]=  
{
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S0S4_EXAMPLE_1, 		0,      0,  },
	{ S0S4_EXAMPLE_2, 		0,      0,  },
    { S0_S4_Variable, 	    0,		0	},  // always in the end of tabel	
}; 

//-------------------------------------------------------------------------------------
//  Do S3 -> S0 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ  PowerSequence_S3S0[]= 
{
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S3S0_EXAMPLE_1, 		0,      0,  },
	{ S3S0_EXAMPLE_2, 		0,      0,  },
	{ S3_S0_Variable, 	    0,		0,  },	// always in the end of tabel
}; 

//-------------------------------------------------------------------------------------
//  Do S0 -> S3 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ  PowerSequence_S0S3[]=  
{
	{ 0x0000, 				0,		0,  },	// Dummy step always in the top of tabel
	{ S0S3_EXAMPLE_1, 		0,      0,  },
	{ S0S3_EXAMPLE_2, 		0,      0,  },
	{ S0_S3_Variable, 	    0,		0,  },  // always in the end of tabel	
}; 


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S3_S0_Trigger(void)
{
    if(SystemIsS0)
    {
        return;
    }
    
	System_PowerState=SYSTEM_S3_S0;
	PowerSequence_Step = 1;
	PowerSequence_Delay = 0x00;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S4_S0_Trigger(void)
{
    if(SystemIsS0)
    {
        return;
    }
    
    System_PowerState=SYSTEM_S4_S0;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S5_S0_Trigger(void)
{
    if(SystemIsS0)
    {
        return;
    }
    System_PowerState=SYSTEM_S5_S0;
	PowerSequence_Step = 1;
	PowerSequence_Delay = 0x00;
}

//----------------------------------------------------------------------------
// S5 to S0 sequnce
//----------------------------------------------------------------------------
void Custom_S5S0_Sequence(void)
{
    if ( PowerSequence_Delay != 0x00 )
    {
        PowerSequence_Delay--; 				//1m Sec count base
        if ( PowerSequence_Delay != 0x00 )
        {
            return;
        } 
    }

	while(PowerSequence_Step<(sizeof(PowerSequence_S5S0)/sizeof(sPowerSEQ)))
	{
		if(PowerSequence_S5S0[PowerSequence_Step].checkstatus==0x00)	// Do function
		{
			(PowerSequence_S5S0[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
			
		}
		else if(PowerSequence_S5S0[PowerSequence_Step].checkstatus==0x01)											// Check input status pin
		{

            (PowerSequence_S5S0[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
			
		}
        else
        {
           (PowerSequence_S5S0[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S5S0[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;

        }	
	}

    PowSeq_Record_Flag = 1;

    if((PowerSequence_Step == ((sizeof(PowerSequence_S5S0)/sizeof(sPowerSEQ))) - 1))
    {
        PowerSequence_Step++;

        #if ENABLE_DEBUGGER_SUPPORT
            Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
        #endif
    }

}

//----------------------------------------------------------------------------
// S4 to S0 sequnce
//----------------------------------------------------------------------------
void Custom_S4S0_Sequence(void)
{
    if ( PowerSequence_Delay != 0x00 )
    {
        PowerSequence_Delay--; 				//1m Sec count base
        if ( PowerSequence_Delay != 0x00 )
        {
            return;
        }
    }

    while(PowerSequence_Step<(sizeof(PowerSequence_S4S0)/sizeof(sPowerSEQ)))
    {	
        if(PowerSequence_S4S0[PowerSequence_Step].checkstatus==0x00)	// Do function
        {
            (PowerSequence_S4S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            break;
           
        }
        else if(PowerSequence_S4S0[PowerSequence_Step].checkstatus==0x01)	 // Check input status pin
        {
            (PowerSequence_S4S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            break;
        }
        else
        {
            (PowerSequence_S4S0[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S4S0[PowerSequence_Step].delay;
            PowerSequence_Step++;
            break; 
        }	
    }

    PowSeq_Record_Flag = 2;

    if(PowerSequence_Step == ((sizeof(PowerSequence_S4S0)/sizeof(sPowerSEQ)) - 1))
    {
        PowerSequence_Step++;
        #if ENABLE_DEBUGGER_SUPPORT
            Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
        #endif
    }
}

//----------------------------------------------------------------------------
// S3 to S0 sequnce
//----------------------------------------------------------------------------
void Custom_S3S0_Sequence(void)
{
    if ( PowerSequence_Delay != 0x00 )
    {
        PowerSequence_Delay--; 				//1m Sec count base
        if ( PowerSequence_Delay != 0x00 )
        {
            return;
        }
    }

	while(PowerSequence_Step<(sizeof(PowerSequence_S3S0)/sizeof(sPowerSEQ)))
	{	
		if(PowerSequence_S3S0[PowerSequence_Step].checkstatus==0x00)	// Do function
		{
			(PowerSequence_S3S0[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
			
		}
		else if(PowerSequence_S3S0[PowerSequence_Step].checkstatus==0x01) // Check input status pin
		{

            (PowerSequence_S3S0[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
			PowerSequence_Step++;
            break;
		}
        else
        {
            (PowerSequence_S3S0[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S3S0[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
        }	
	}

    PowSeq_Record_Flag = 3;

    if(PowerSequence_Step  == ((sizeof(PowerSequence_S3S0)/sizeof(sPowerSEQ)) - 1))
    {
        PowerSequence_Step++;

        #if ENABLE_DEBUGGER_SUPPORT
            Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
        #endif
    }
     
}

//-----------------------------------------------------------------------------
// Check SUSB and SUSC
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
//The function of checking slp s3
//------------------------------------------------------------------------------
void Sleep_S3_Monitor(void)
{
	CLEAR_MASK(CTRL_FLAG2, SLEEP_S3_ON);
	if(Read_SLPS3())
    {
        SET_MASK(CTRL_FLAG2, SLEEP_S3_ON);
    }   
}

//------------------------------------------------------------------------------
//The function of checking slp s4
//------------------------------------------------------------------------------
void Sleep_S4_Monitor(void)
{
	CLEAR_MASK(CTRL_FLAG2, SLEEP_S4_ON);
	if(Read_SLPS4())
    {
        SET_MASK(CTRL_FLAG2, SLEEP_S4_ON);
    } 
}

//------------------------------------------------------------------------------
//The function of checking slp s5
//------------------------------------------------------------------------------
void Sleep_S5_Monitor(void)
{
    CLEAR_MASK(CTRL_FLAG2, SLEEP_S5_ON);
	if(Read_SLPS5())
    {
        SET_MASK(CTRL_FLAG2, SLEEP_S5_ON);
    }  
}

void Change_PowerState(void)
{
    BYTE presentstatus;

    presentstatus = (CTRL_FLAG2&(SLEEP_S3_ON+SLEEP_S4_ON+SLEEP_S5_ON));
    if(PowerStatusBK != presentstatus)      // To make sure all SLP is checked.
    {
        PowerStatusBK = presentstatus;
        return;
    }
    
	switch(CTRL_FLAG2 & (SLEEP_S3_ON+SLEEP_S4_ON+SLEEP_S5_ON)) 
	{
		case 0: 	                        // goto S5
            switch (System_PowerState) 
            {
                case SYSTEM_S0 :
                    Custom_S0_S5_Trigger(SC_S0SLPOff);
                    break;
                    
                case SYSTEM_S4 :
                    break;
                    
                case SYSTEM_S3 :
                    Custom_S0_S5_Trigger(SC_S3S4SLPOff);
                    break;           
            }
           	break;

		case (SLEEP_S3_ON+SLEEP_S4_ON+SLEEP_S5_ON): // goto S0
            switch (System_PowerState) 
            {
                case SYSTEM_S3 :
                    Custom_S3_S0_Trigger();
                    break;
                    
                case SYSTEM_S4 :
                    Custom_S4_S0_Trigger();
                    break;
                    
                case SYSTEM_S5 :			             
                    Custom_S5_S0_Trigger();
                     break;
            }
            break;            

        case (SLEEP_S4_ON+SLEEP_S5_ON):             // goto S3
            switch (System_PowerState) 
            {
                case SYSTEM_S0 :
                    Custom_S0_S3_Trigger(0x30);
                    break;
            }
            break;

        case SLEEP_S5_ON:                         // goto S4
            switch (System_PowerState) 
            {
                case SYSTEM_S0 :
                    Custom_S0_S4_Trigger(0x40);
                    break;
            }
            break;
	}
}

//------------------------------------------------------------------------------
void PowerState_Monitor(void)
{
    switch(PowerState_Monitor_Index)
    {
        case 0:
            Sleep_S3_Monitor();          // Check SLP_S3#(or SUS_B#)
            PowerState_Monitor_Index ++;
            break;

        case 1:
            Sleep_S4_Monitor();          // Check SLP_S4#
            PowerState_Monitor_Index ++;
            break;
            
        case 2:
            Sleep_S5_Monitor();          // Check SLP_S5#(or SUS_C#)
            PowerState_Monitor_Index ++;
            break;

        case 3:
            Change_PowerState();
            PowerState_Monitor_Index = 0x00;
            break;

        default:
            PowerState_Monitor_Index = 0x00;
            break;
    }
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S0_S3_Trigger(BYTE cause)
{
    if(SystemIsS3)
    {
        return;
    }
    
	ShutDnCause = cause;		// setup shutdown case
	System_PowerState=SYSTEM_S0_S3;
	PowerSequence_Step = 1;
	PowerSequence_Delay = 0x00;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S0_S4_Trigger(BYTE cause)
{
    if(SystemIsS4)
    {
        return;
    }
    
    ShutDnCause = cause;		// setup shutdown case
    System_PowerState=SYSTEM_S0_S4;
    PowerSequence_Step = 1;
    PowerSequence_Delay = 0x00;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Custom_S0_S5_Trigger(BYTE cause)
{
    if(SystemIsS5)
    {
        return;
    }
    
	ShutDnCause = cause;		// setup shutdown case 
    System_PowerState=SYSTEM_S0_S5;
	PowerSequence_Step = 1;
	PowerSequence_Delay = 0x00;
}

//-----------------------------------------------------------------------------
// process POWER OFF sequence
// entry: 	1 power shutdown case
//			2 S5 off or not
// Shutdown cause:
//	1:	0x8A command of 66 port
// 	2:	0x8B command of 66 port
//	3:	4secs Overrided
// 	4:	HWPG low
//	5:	0x8C command of 66 port
// 	6:	0x8D command of 66 port
//	7:	temperature to high
// 	8:
//	9:	Init. thermal chip fail
// 	10:	Check SLP // S0 -> S4
//	11:	Check SLP // S3 -> S5
// 	12:	Check SLP // S0 -> S5

//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// S0 to S3 sequnce
//----------------------------------------------------------------------------
void Custom_S0S3_Sequence(void)
{
    if ( PowerSequence_Delay != 0x00 )
    {
        PowerSequence_Delay--; 				//1m Sec count base
        if ( PowerSequence_Delay != 0x00 )
        {
            return;
        }
    }

	while(PowerSequence_Step<(sizeof(PowerSequence_S0S3)/sizeof(sPowerSEQ)))
	{	
		if(PowerSequence_S0S3[PowerSequence_Step].checkstatus==0x00)	// Do function
		{
			(PowerSequence_S0S3[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
			
		}
		else if(PowerSequence_S0S3[PowerSequence_Step].checkstatus==0x01)	// Check input status pin
		{

            (PowerSequence_S0S3[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
		}
        else
        {
            (PowerSequence_S0S3[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S0S3[PowerSequence_Step].delay;
			PowerSequence_Step++;
		    break;
			 
        }	
	}

    PowSeq_Record_Flag = 4;

    if(PowerSequence_Step == ((sizeof(PowerSequence_S0S3)/sizeof(sPowerSEQ)) - 1))
    {
        PowerSequence_Step++;

        #if ENABLE_DEBUGGER_SUPPORT
            Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
        #endif
    }
}


//----------------------------------------------------------------------------
// S0 to S4 sequnce
//----------------------------------------------------------------------------
void Custom_S0S4_Sequence(void)
{
    if ( PowerSequence_Delay != 0x00 )
    {
        PowerSequence_Delay--; 				//1m Sec count base
        if ( PowerSequence_Delay != 0x00 )
        {
            return;
        }
    }

    while(PowerSequence_Step<(sizeof(PowerSequence_S0S4)/sizeof(sPowerSEQ)))
    {	
        if(PowerSequence_S0S4[PowerSequence_Step].checkstatus==0x00)	// Do function
        {
            (PowerSequence_S0S4[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
            PowerSequence_Step++;
            break;
            
        }
        else if(PowerSequence_S0S4[PowerSequence_Step].checkstatus==0x01)	// Check input status pin
        {

            (PowerSequence_S0S4[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
            PowerSequence_Step++;
            break;
        }
        else
        {
            (PowerSequence_S0S4[PowerSequence_Step].func)();
            PowerSequence_Delay = PowerSequence_S0S4[PowerSequence_Step].delay;
            PowerSequence_Step++;
            break;
        }	
    }


    PowSeq_Record_Flag = 5;

    if(PowerSequence_Step == ((sizeof(PowerSequence_S0S4)/sizeof(sPowerSEQ)) - 1))
    {
        PowerSequence_Step++;

        #if ENABLE_DEBUGGER_SUPPORT
            Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
        #endif
    }       
}

//-----------------------------------------------------------------
// S0 to S5 sequnce
//-----------------------------------------------------------------
void Custom_S0S5_Sequence(void)
{
    if ( PowerSequence_Delay != 0x00 )
    {
        PowerSequence_Delay--; 				//1m Sec count base
        if ( PowerSequence_Delay != 0x00 )
        {
            return;
        }
    }

	while(PowerSequence_Step<(sizeof(PowerSequence_S0S5)/sizeof(sPowerSEQ)))
	{	
		if(PowerSequence_S0S5[PowerSequence_Step].checkstatus==0x00)	// Do function
		{
			(PowerSequence_S0S5[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
			
		}
		else if(PowerSequence_S0S5[PowerSequence_Step].checkstatus==0x01) // Check input status pin
		{

            (PowerSequence_S0S5[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
			PowerSequence_Step++;
			break;
		}
        else
        {
            (PowerSequence_S0S5[PowerSequence_Step].func)();
			PowerSequence_Delay = PowerSequence_S0S5[PowerSequence_Step].delay;
			PowerSequence_Step++;
		    break;
			
        }	
	}

    PowSeq_Record_Flag = 6;

    if(PowerSequence_Step == ((sizeof(PowerSequence_S0S5)/sizeof(sPowerSEQ)) - 1))
    {
        PowerSequence_Step++;

        #if ENABLE_DEBUGGER_SUPPORT
            Copy_Power_Sequence_Record_To_Sram(PowSeq_Record_Flag);
        #endif
    }  
}


//-----------------------------------------------------------------------------
// The function of checking HWPG
//-----------------------------------------------------------------------------
void HWPG_Monitor(void)
{
    if(SystemIsS0)
    {
        if(!Read_HWPG()) 
        {
            HWPG_TIMER++;
            if(HWPG_TIMER>T_HWPG)
            {
                Custom_S0_S5_Trigger(SC_HWPS0Off);
                HWPG_TIMER = 0x00;
            }
        }
        else
        {
             HWPG_TIMER = 0x00;
        }
    }
}

//----------------------------------------------------------------------------
// EC auto power on function
//----------------------------------------------------------------------------
void Auto_PowerOn_Monitor(void)
{
	if(ECAutoPowerOn==1)
	{
		ECAutoPowerOn = 0;
		DelayDoPowerSeq = 1000;
		Custom_S5_S0_Trigger();
	}
	
	if(ECWDTRSTNow)
	{
		ECWDTRSTNow = 0;
		System_PowerState = SYSTEM_EC_WDTRST;
	}
}

//-----------------------------------------------------------------------------
// Handle System Power Control
// Called by service_1mS -> Event_1ms -> Hook_1msEvent
//-----------------------------------------------------------------------------
void Sys_PowerState_Control(void)
{
	if(SystemIsS0)
	{
		//reserved
	}

    // Check if delay doing PowerSeq is needed.
    if ( DelayDoPowerSeq != 0x00 )
    {
        DelayDoPowerSeq--; 		
        if ( DelayDoPowerSeq != 0x00 )
        {
            return;
        }
    }

    /*if((SystemIsS0)||(SystemIsS3)||(SystemIsS4)||(SystemIsS5))
    {
        PowerState_Monitor();
    }*/

    // Check current System PowerState to see what should we do.
    switch (System_PowerState)
    {
        case SYSTEM_S0:
			//HWPG_Monitor();
			break;

        case SYSTEM_S3: 
         	break;

        case SYSTEM_S4:
            break;

        case SYSTEM_S5:
			//Auto_PowerOn_Monitor();
            break;

        case SYSTEM_S4_S0:
            //dprint("Custom_S4S0_Sequence\n");
            Custom_S4S0_Sequence();
            break;

        case SYSTEM_S5_S0:
            //dprint("Custom_S5S0_Sequence\n");
			Custom_S5S0_Sequence();
			break;
				
        case SYSTEM_S3_S0:
            //dprint("Custom_S3S0_Sequence\n");
			Custom_S3S0_Sequence();
            break;

        case SYSTEM_S0_S3:
            //dprint("Custom_S0S3_Sequence\n");
			Custom_S0S3_Sequence();
            break;

        case SYSTEM_S0_S4:
            //dprint("Custom_S0S4_Sequence\n");
            Custom_S0S4_Sequence();
            break;

        case SYSTEM_S0_S5:
            //dprint("Custom_S0S5_Sequence\n");
			Custom_S0S5_Sequence();
         	break;
				
		case SYSTEM_EC_WDTRST:
			//InternalWDTNow();
			break;

        default:
         	System_PowerState=SYSTEM_S5;
         	break;
    }
}


void ResetSource_Monitor(void)
{
    switch(RSTStatus&0x03)
    {
        case 0:
        case 1:
            if(GPCRA0==0x84)
            {
                ShutDnCause = SC_ECColdBoot;
            }
            else
            {
                ShutDnCause = SC_EC_0x0000;
            }
            break;
            
        case 2:
            if(GPCRA0==0x84)
            {
                ShutDnCause = SC_IntWatchDog;
            }
            else
            {
                ShutDnCause = SC_EC_0x0000;
            }
            break;         

        case 3:
            if(GPCRA0==0x84)
            {
                ShutDnCause = SC_ExtWatchDog;
            }
            else
            {
                ShutDnCause = SC_EC_0x0000;
            }
            
            break;
    }
}

//-----------------------------------------------------------------------------
// The function of pulsing S.B. power button 100ms
//-----------------------------------------------------------------------------
void SB_PowerButton_Enable(void)
{
    DNBSWON_ON();
    SBSWReleaseCunt = 20;   // decrease timer base is 10ms total time 200ms
}

//-----------------------------------------------------------------------------
// The function of releasing S.B. power button
//-----------------------------------------------------------------------------
void SB_PowerButton_Monitor(void)
{
	if(SBSWReleaseCunt!=0x00)
	{
		SBSWReleaseCunt--;
		if(SBSWReleaseCunt==0x00)
		{
			DNBSWON_OFF();
		}
	}
}

/**
 * @brief POWER SWITCH初始化模块
 *
 * @param    timeout     timeout后产生power switch timeout 事件
 *           可选参数：   000：1000ms  001：4000ms  010：5000ms ... 111:12000ms
 * @param    mode        0:WDT timeout后产生中断，不复位     1:WDT timeout后直接复位
 *
 * @return   无
 */
void PWMSW_Config(BYTE  timeout,BYTE  mode)
{
    sysctl_iomux_config(GPIOB,4,1);//设置GPE4复用功能
    #if (SYSCTL_CLOCK_EN)
    SYSCTL_PWRSWCSR |= 0x80;    //去抖64ms
    if(mode==1)
    {
      SYSCTL_PWRSWCSR |=((0x1)|(timeout<<1)|(0x1<<4));
    }
    else if(mode==0)
    {
       SYSCTL_PWRSWCSR |=((0x1)|(timeout<<1));
       SYSCTL_PWRSWCSR &=~(0x1<<4);
    }
    #endif
}


//-----------------------------------------------------------------------------
// WDT timeout产生复位前令GPI5输出100ms长度的低电平(GPI5此功能优先级高于其他复用功能)，
// 因此WDT timeout时间将比 pwrsw_sel 设置长100ms
//-----------------------------------------------------------------------------
void PWMSW_Rstoutputen(void)
{
    sysctl_iomux_config(GPIOC,5,3);//设置GPI5复用功能
    #if (SYSCTL_CLOCK_EN)
    SYSCTL_PWRSWCSR |=0x100;
    #endif
}
