/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-03 19:03:07
 * @Description: This file is used for handling Standard 8042 Keyboard Controller Commands
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

#include "KERNEL_PORT6064.H"
#include "AE_FUNC.H"
#include "KERNEL_TIMER.H"
#include "KERNEL_INTC.H"
#include "KERNEL_KBS.H"
#include "AE_DEBUGGER.H"
#include "CUSTOM_LED.H"


//-----------------------------------------------------------------------------
// Handle KBC command A1 -
//-----------------------------------------------------------------------------
void Command_A1(void)
{


}

void Command_C0(void)
{
    // Just return the compatibility value for now. //return (PCIN | 0x1F);
    Transmit_Data_To_Host( 0xBF );
}

void Command_90(void)
{
    if( Set_Port60_Data_Handle() )
    {
        if(KBHIData==0xFF)
        {
            //Scan_PS2_Device();
        }

	    KBC_STA = 0x24;                  // set error bit and AUX bit, source bits is 00
        KBC_CTL &= 0xfc;

        if( Host_Flag_INTR_AUX )        // If AUX IRQ bit of command is present 
        {
            SET_BIT(KBC_CTL,1);          // Enable IRQ
        }

	    vDelayXms(5);                   // Emulate transmission delay times 
	    KBC_MOB = 0xFC;                // timeout error  
    }
}

VBYTE Set_Port60_Multi_Data_Handle(void)
{
    if( KBHIStep == 0x00 )
    {
        KBHIStep=0x01;  // Set Next Port60 Data Handle again
        return(0);
    }
    else
    {
        KBHIStep++;
        return(1);
    }

}

void Command_D3(void)
{
    int num = 5;

    if( Set_Port60_Data_Handle() )
    {

	    Host_Flag_DISAB_AUX = 0;    // Enable aux device (mouse)
        Host_Flag &= 0xdf;

        Mouse_Data_To_Host(KBHIData);

    }
}


void KB_Cmd_Handle(BYTE nKB60DAT)
{ 
    char ack,ack1,ack2,index,cmdbk;
    char Led_Data;

    ack = 0x00;
    ack1 = 0x00;
    ack2 = 0x00;

    #if KB_MS_DEBUG
        dprint("KB_Command is 0x%x,nKB60DAT is 0x%x\n",KB_Command,nKB60DAT);
    #endif

    if( KB_Command )
    {
        cmdbk = KB_Command;
        
        switch(KB_Command)
        {
            case 0xED:  /* Update LEDs command. */
                        Led_Data = nKB60DAT;
                        ack = 0xFA;
                        KB_Command = 0x00;
                        OEM_Write_Leds();   //Hook Oem On-board LED control
                        /* Update scanner numlock state. */
                        Scanner_State_NUM_LOCK = Led_Data_NUM;
                        break;
            case 0xF0:  /* Set alternate KBD_SCAN codes. */
                        Keyboard_CodeSet = nKB60DAT;
                        ack = 0xFA;
                        KB_Command = 0x00;
                        break;
            case 0xF3:  /* Set typematic rate/delay. */
                        Keyboard_Typematic = nKB60DAT;
                        KBS_Set_Typematic(Keyboard_Typematic);
                        ack = 0xFA;
                        KB_Command = 0x00;
                        break;
            default:    //Unknown command request system resend   
                        ack = 0xFE;         //Resend         
                        break;
        }

    
        if((cmdbk==0xF3)||(cmdbk==0xED))
        {
            KB_Scan_Flag = 1;
        }

    #if 0   
        if(ack != 0x00)                         // if need send ack to host
        {
            Send_KB_Data_To_Host(ack);          // send to host
        }
	#endif

    if(KB_Main_CHN == 0)
    {
        if(ack != 0x00)                         // if need send ack to host
        {
            Transmit_Data_To_Host(ack);          // send to host
        }
    } else {

        #if !(PS2M_CLOCK_EN&PS2K_CLOCK_EN)
            dprint("PS2 CLOCK NOT ENABLE\n");
            return;
        #endif

        if(cmdbk==0xED)
        {
            Send_Data_To_PS2((KB_Main_CHN-1), (nKB60DAT&0x07));
        } else {
            Send_Data_To_PS2((KB_Main_CHN-1), nKB60DAT);
        }

        /*if((cmdbk==0xF3)||(cmdbk==0xED))
        {
            SetOtherKBNeedUpdataFlag(cmdbk);   
        }*/ 
    }

	    return ;
   }

    #if KB_MS_DEBUG
    dprint("Before nKB60DAT is 0x%x ,ack is 0x%x,ack1 is 0x%x,akc2 is 0x%x\n",nKB60DAT,ack,ack1,ack2);
    #endif

    KB_Command = 0x00;//zb 10.27 edit

    switch(nKB60DAT)
    {
        case 0xED:  /* Update LEDs command. */
                    ack = 0xFA;
                    KB_Command = 0xED;
		            //Send_KB_Data_To_Host(ack);
		            OEM_Write_Leds();   //Hook Oem On-board LED control

		            /* Update scanner numlock state. */
		            //Scanner_State_NUM_LOCK = Led_Data_NUM;
                    Scanner_State_NUM_LOCK = 0;
                    break;
	    case 0xEC:
		            ack = 0xFA;
		            //Send_KB_Data_To_Host(ack);
                    break;
        case 0xEE:  /* ECHO command. */
                    ack = 0xEE;
		            //Send_KB_Data_To_Host(ack);
                    break;
 
        case 0xF0:  /* Set alternate KBD_SCAN codes. */
                    ack = 0xFA;
                    KB_Command = 0xF0;
		            //Send_KB_Data_To_Host(ack);
                    break;
        case 0xF2:  /* Read manufacturers ID */
                    ack = 0xFA;
                    ack1 = 0xAB;
                    if( Host_Flag_XLATE_PC )
                    {
                        ack2 = 0x83;
                    }
                    else
                    {
                        ack2 = 0x41;
                    }
                    break; 
        case 0xF3:  /* Set typematic rate/delay. */
                    ack = 0xFA;
                    KB_Command = 0xF3;
                    break;
        case 0xF4:  /* Enable scanning. */
                    KB_Scan_Flag = 1;
                    ack = 0xFA;
                    break;
        case 0xF5:  /* Default disable. */
                    KB_Scan_Flag = 0;
		            //KBD_CLear_Buffer();
		            //KBD_Default_Typematic();
                     ack = 0xFA;
		            //Send_KB_Data_To_Host(ack);
                     break;
        case 0xF6:  /* Set defaults. */
		            //KBD_CLear_Buffer();
		            //KBD_Default_Typematic();
                    ack = 0xFA;
		            //Send_KB_Data_To_Host(ack);
                     break;
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
                    ack = 0xFA;
                    break;
        case 0xFF:  /* Reset keyboard. */
                    KBD_CLear_Buffer();
                    ack = 0xFA;
		            ack1 = 0xAA;
                    break;
	    default:    //Unknown command request system resend
                    ack = 0xFE;
                    
                    break;
    }

    #if KB_MS_DEBUG
	dprint("ack is 0x%x\n",ack);
	dprint("ack1 is 0x%x\n",ack1);
	dprint("ack2 is 0x%x\n",ack2);
    #endif

    #if 0
	if(ack != 0x00)
    {
        Send_KB_Data_To_Host(ack);
  
        if(ack1 != 0x00)
        {
            KBC_Data_Suspend(ack1);
 
            if(ack2 != 0x00)
            {
                KBC_Data_Suspend(ack2);
            }
        }
    }
    #endif

    if(KB_Main_CHN == 0)
    {
        if(ack != 0x00)
        {
            Transmit_Data_To_Host(ack);
  
            if(ack1 != 0x00)
            {
                KBC_Data_Suspend(ack1);
 
                if(ack2 != 0x00)
                {
                    KBC_Data_Suspend(ack2);
                }
            }
        }   
    } else {
        /*if(nKB60DAT==0xFF)                // reset command
        {
            KBCmdAckByteCunt(0x02);
        }
        else                             
        {
            if(nKB60DAT==0xF2)              // read keyboard ID command
            {
                KBCmdAckByteCunt(0x03);
            }
            else if((nKB60DAT==0xF3)||(nKB60DAT==0xED))
            {
                KBCmdAckByteCunt(0x02);
            }
            else                            // other
            {
                KBCmdAckByteCunt(0x01);
            }
        }*/

        #if !(PS2M_CLOCK_EN&PS2K_CLOCK_EN)
            dprint("PS2 CLOCK NOT ENABLE\n");
            return;
        #endif

	    Send_Data_To_PS2((KB_Main_CHN-1), nKB60DAT);

        /*if((nKB60DAT==0xF4)||(nKB60DAT==0xF5))
        {
            SetOtherKBNeedUpdataFlag(nKB60DAT);   
        }*/
    }
  
}   

//-----------------------------------------------------------------------------
void Command_A5Data(void)
{
    if( (Tmp_Load == 0) && (KBHIData == 0) )   // The first byte is NULL
    {
        // Flag.PASS_READY = 0;    // clear password ready flag and exit
    }
    else
    {
        if( Tmp_Load < 8 )       // PASS_SIZE = 8
        {                         // If there is room in the buffer
            //Pass_Buff[Tmp_Load] = KBHIData;   // Store KBD_SCAN code
            (*(VBYTE*)(Pass_BuffBase + Tmp_Load)) = KBHIData;   // Store KBD_SCAN code
            Tmp_Load++;           // Increment password buffer index
        }

        //Cmd_Byte = 0xA5;        // Set to keep waiting for next byte
        // The last byte (null terminated string) has been stored
        if( KBHIData == 0 )
        {
            //Cmd_Byte = 0;       // Clear out the command byte.
            //Flag.PASS_READY = 1;// Set password ready bit.
        }
    }

}

VBYTE Loop_Wait_Get_Port60_Data(void)
{
    int  iLOOP;
    iLOOP = WaitKBDataDelay;

    do
    {
        if ( IS_SET(KBC_STA,1) )
        {
            if ( KBC_STA & KBC_STA_A2 )
            {
                return 0x00;
            }
            else
            {
                KBHIData  = KBC_IB;
                return 0x01;
            }
        }

        iLOOP--;
    }
    while (iLOOP != 0 );

      //Failed Monitor

    return 0x00;

}

//-----------------------------------------------------------------------------
/* Read/Write KBC RAM Control Bytes*/
//-----------------------------------------------------------------------------
void Command_00_3F(void)    // Command_00_3F: Read KBC RAM Control Bytes
{

    #if KB_MS_DEBUG
        dprint("Command_00_3F \n");
    #endif

    switch( KBHICmd&0x1F )
    {
        case 0x00:  //Cmd_0x00 & Cmd_0x20
                    #if KB_MS_DEBUG
		            dprint("Host_Flag is 0x%x\n",Host_Flag);
                    #endif
                    Transmit_Data_To_Host( Host_Flag );
                    break;

        case 0x13:  //Cmd_0x13 & Cmd_0x33 Send Security Code On byte to host
                    Transmit_Data_To_Host( Pass_On );
                    break;

        case 0x14:  //Cmd_0x14 & Cmd_0x34 Send Security Code Off byte to host
                    Transmit_Data_To_Host( Pass_Off );
                    break;

        case 0x16:  //Cmd_0x16 & Cmd_0x36 Send Reject make code 1 to host
                    Transmit_Data_To_Host( Pass_Make1 );
                    break;

        case 0x17:  //Cmd_0x17 & Cmd_0x37 Send Reject make code 2 to host
                    Transmit_Data_To_Host( Pass_Make2 );
                    break;

        default:    Transmit_Data_To_Host( 0x00 );
                      //Failed Monitor
                    break;
    }


}

//----------------------------------------------------------------------------
// Set&Get Status for Next Port60 Data Handle
//----------------------------------------------------------------------------
VBYTE Set_Port60_Data_Handle(void)
{
    if( KBHIStep == 0x00 )
    {
        KBHIStep=0x01;  // Set Next Port60 Data Handle again
        return(0);
    }
    else
    {
        KBHIStep=0x00;  // Set Command Finished
        return(1);
    }

}

//-----------------------------------------------------------------------------
void Write_KCCB(BYTE NewKCCB)
{
    Host_Flag = NewKCCB;    // Write the data.

    if( Host_Flag & 0x1 )
    {
        Host_Flag_INTR_KEY = 1;       // Enable keyboard interrupt.
    }
    else
    {
        Host_Flag_INTR_KEY = 0;       // Disable keyboard interrupt.
    }

    if( Host_Flag & 0x2 ) 
    {
        Host_Flag_INTR_AUX = 1;       // Enable aux interrupt.
    }
    else
    {
        Host_Flag_INTR_AUX = 0;       // Disable aux interrupt.
    }

    if( Host_Flag & 0x4 )   
    {
        Host_Flag_SYS_FLAG = 1;      
    }
    else
    {
        Host_Flag_SYS_FLAG = 0;     
    }

    if( Host_Flag & 0x10 )   
    {
        Host_Flag_DISAB_KEY = 1;      
    }
    else
    {
        Host_Flag_DISAB_KEY = 0;     
    }

    if( Host_Flag & 0x20 )   
    {
        Host_Flag_DISAB_AUX = 1;      
    }
    else
    {
        Host_Flag_DISAB_AUX = 0;     
    }

    if( Host_Flag & 0x40 )   
    {
        Host_Flag_XLATE_PC = 1;      
    }
    else
    {
        Host_Flag_XLATE_PC = 0;     
    }

    /*update Host_Flag to PS2 mouse CR register*/
    if(MS_Main_CHN == 1)
    {
    *((volatile uint8_t *)(0x2003)) = 0x60;
    *((volatile uint8_t *)(0x2001)) = ((Host_Flag ^ (0x1<<5))&0xEE);    
    } else if (MS_Main_CHN == 2){
    *((volatile uint8_t *)(0x2203)) = 0x60;
    *((volatile uint8_t *)(0x2201)) = ((Host_Flag ^ (0x1<<5))&0xEE);    
    } else {

    }

    /*update Host_Flag to PS2 keyboard CR register*/
    if(KB_Main_CHN == 1)
    {
    *((volatile uint8_t *)(0x2003)) = 0x60;    
    *((volatile uint8_t *)(0x2001)) = ((Host_Flag ^ (0x1<<4))&0xDD);    
    } else if (MS_Main_CHN == 2){
    *((volatile uint8_t *)(0x2203)) = 0x60;    
    *((volatile uint8_t *)(0x2201)) = ((Host_Flag ^ (0x1<<4))&0xDD);
    } else {

    } 

    if( Host_Flag_SYS_FLAG )   
    {
        SET_BIT( KBC_STA, 2 );
    }
    else
    {
        CLEAR_BIT( KBC_STA, 2 );
    }
}

void Command_A5X(BYTE data)
{
	switch(data)
	{
	   case 0xA0:
			break;
	
	   case 0xA1:
			Transmit_Data_To_Host(0xB0);
			break;

	   case 0xA2:
			Transmit_Data_To_Host(0x6A);
			break;

	   case 0xA3:
			Transmit_Data_To_Host(0x08);
			break;

	   case 0xA4:
			Transmit_Data_To_Host(0x00);
			break;

	   case 0xA6:
			Transmit_Data_To_Host(0x0B);
			break;

	   case 0xA7:
			break;

	   case 0xA8:
			break;
	
	   default:
			break;

	}
}


//-----------------------------------------------------------------------------
void Command_40_7F(void)    // Command_40_7F: Write KBC RAM Control Bytes
{

    #if KB_MS_DEBUG
	    dprint("Command_40_7F \n");
    #endif

    if(KBHICmd == 0x60)
    {
        while(!(KBC_STA & 0x2));
        KBHIData  = KBC_IB;
        #if SUPPORT_8042DEBUG_OUTPUT
        Write_Debug_Data_To_Sram(KBHIData);
        #endif
        Write_KCCB(KBHIData);
        return;
    }

    if( Set_Port60_Data_Handle() )
    {
        KBHIStep = 0x00;// Set Command Finished

        switch( KBHICmd&0x1F )
        {
            case 0x00:  //Cmd_0x40 & Cmd_0x60
                        #if KB_MS_DEBUG
			            dprint("Write_KCCB KBHIData is 0x%x\n",KBHIData);
                        #endif
                        Write_KCCB( KBHIData );
                        break;
	        case 0x12:  //Cmd_0x52 & Cmd_0x72 
                        Command_A5X(KBHIData);
                        break;
            case 0x13:  //Cmd_0x53 & Cmd_0x73 Write Security Code On byte
                        Pass_On = KBHIData;     // Write the data.
                        break;
            case 0x14:  //Cmd_0x54 & Cmd_0x74 Write Security Code Off byte
                        Pass_Off = KBHIData;    // Write the data.
                        break;
            case 0x16:  //Cmd_0x56 & Cmd_0x76 Reject make code 1
                        Pass_Make1 = KBHIData;  // Write the data.
                        break;
            case 0x17:  //Cmd_0x57 & Cmd_0x77 Reject make code 2
                        Pass_Make2 = KBHIData;  // Write the data.
                        break;
            default:      //Failed Monitor
                        break;
        }
    }
}

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V Cmd8X_table[16] =
{
    Command_80,             // Process 64 port command 80
    Command_81,             // Process 64 port command 81
    Command_82,             // Process 64 port command 82
    Command_83,             // Process 64 port command 83
    Command_84,             // Process 64 port command 84
    Command_85,             // Process 64 port command 85
    Command_86,             // Process 64 port command 86
    Command_87,             // Process 64 port command 87
    Command_88,             // Process 64 port command 88
    Command_89,             // Process 64 port command 89
    Command_8A,             // Process 64 port command 8A
    Command_8B,             // Process 64 port command 8B
    Command_8C,             // Process 64 port command 8C
    Command_8D,             // Process 64 port command 8D
    Command_8E,             // Process 64 port command 8E
    Command_8F              // Process 64 port command 8F
};


//-----------------------------------------------------------------------------
void Command_8X(void)
{
    #if KB_MS_DEBUG
        dprint("Command_8X \n");
    #endif
    (Cmd8X_table[KBHICmd&0x0F])();
}

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V Cmd9X_table[16] =
{
    Command_90,             // Process 64 port command 90
    Command_91,             // Process 64 port command 91
    Command_92,             // Process 64 port command 92
    Command_93,             // Process 64 port command 93
    Command_94,             // Process 64 port command 94
    Command_95,             // Process 64 port command 95
    Command_96,             // Process 64 port command 96
    Command_97,             // Process 64 port command 97
    Command_98,             // Process 64 port command 98
    Command_99,             // Process 64 port command 99
    Command_9A,             // Process 64 port command 9A
    Command_9B,             // Process 64 port command 9B
    Command_9C,             // Process 64 port command 9C
    Command_9D,             // Process 64 port command 9D
    Command_9E,             // Process 64 port command 9E
    Command_9F              // Process 64 port command 9F
};

void Command_9X(void)
{
    #if KB_MS_DEBUG
        dprint("Command_9X \n");
    #endif
    (Cmd9X_table[KBHICmd&0x0F])();
}

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V CmdAX_table[16] =
{
    Command_A0,             // Process 64 port command A0
    Command_A1,             // Process 64 port command A1
    Command_A2,             // Process 64 port command A2
    Command_A3,             // Process 64 port command A3
    Command_A4,             // Process 64 port command A4
    Command_A5,             // Process 64 port command A5
    Command_A6,             // Process 64 port command A6
    Command_A7,             // Process 64 port command A7
    Command_A8,             // Process 64 port command A8
    Command_A9,             // Process 64 port command A9
    Command_AA,             // Process 64 port command AA
    Command_AB,             // Process 64 port command AB
    Command_AC,             // Process 64 port command AC
    Command_AD,             // Process 64 port command AD
    Command_AE,             // Process 64 port command AE
    Command_AF              // Process 64 port command AF
};

void Command_AX(void)
{
    #if KB_MS_DEBUG
        dprint("Command_AX \n");
    #endif
    (CmdAX_table[KBHICmd&0x0F])();
}

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V  CmdBX_table[16] =
{
    Command_B0,             // Process 64 port command B0
    Command_B1,             // Process 64 port command B1
    Command_B2,             // Process 64 port command B2
    Command_B3,             // Process 64 port command B3
    Command_B4,             // Process 64 port command B4
    Command_B5,             // Process 64 port command B5
    Command_B6,             // Process 64 port command B6
    Command_B7,             // Process 64 port command B7
    Command_B8,             // Process 64 port command B8
    Command_B9,             // Process 64 port command B9
    Command_BA,             // Process 64 port command BA
    Command_BB,             // Process 64 port command BB
    Command_BC,             // Process 64 port command BC
    Command_BD,             // Process 64 port command BD
    Command_BE,             // Process 64 port command BE
    Command_BF              // Process 64 port command BF
};

void Command_BX(void)
{
    #if KB_MS_DEBUG
        dprint("Command_BX \n");
    #endif
    (CmdBX_table[KBHICmd&0x0F])();
}

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V CmdCX_table[16] =
{
    Command_C0,             // Process 64 port command C0
    Command_C1,             // Process 64 port command C1
    Command_C2,             // Process 64 port command C2
    Command_C3,             // Process 64 port command C3
    Command_C4,             // Process 64 port command C4
    Command_C5,             // Process 64 port command C5
    Command_C6,             // Process 64 port command C6
    Command_C7,             // Process 64 port command C7
    Command_C8,             // Process 64 port command C8
    Command_C9,             // Process 64 port command C9
    Command_CA,             // Process 64 port command CA
    Command_CB,             // Process 64 port command CB
    Command_CC,             // Process 64 port command CC
    Command_CD,             // Process 64 port command CD
    Command_CE,             // Process 64 port command CE
    Command_CF              // Process 64 port command CF
};

void Command_CX(void)
{   
    #if KB_MS_DEBUG
        dprint("Command_CX\n");
    #endif
    (CmdCX_table[KBHICmd&0x0F])();
}

void Command_91(void)
{
    if ( Set_Port60_Data_Handle() )
    {
        #if 0

        #endif
    }
}

void Command_92(void)
{
    if ( Set_Port60_Data_Handle() )
    {
        #if 0

        #endif
    }
}

void Command_93(void)
{
    if ( Set_Port60_Data_Handle() )
    {
        #if 0

        #endif
    }
}

int STATE_GATEA20(void) 
{
	return 0;
}

void Command_D0(void)
{
    Emulate8042Port = 0xC1;

    if( STATE_GATEA20() )
    {
        Emulate8042Port |= 0x02;
    }

    Transmit_Data_To_Host( Emulate8042Port );
}


//-----------------------------------------------------------------------------
// Handle command A4 - Test Password Installed
// Return: data to send to HOST (FA or F1)
//-----------------------------------------------------------------------------
void Command_A4(void)
{
    #if 0

    #endif
}


//-----------------------------------------------------------------------------
// Handle command A5 - Load Password
//-----------------------------------------------------------------------------
void Command_A5(void)
{
    if( Set_Port60_Multi_Data_Handle() )
    {
        Tmp_Load = 0x00;
    }
    else
    {
        Command_A5Data();
    }

}

//-----------------------------------------------------------------------------
// Handle command A6 - Enable Password
//-----------------------------------------------------------------------
void Command_A6(void)
{
    #if 0

    #endif
}

//-----------------------------------------------------------------------------
// Handle command A7 - Disable Aux Device Interface
//-----------------------------------------------------------------------------
void Command_A7(void)
{
    Host_Flag_DISAB_AUX = 1;  // Disable auxiliary device (mouse)
    Host_Flag |= 0x20;

    if(MS_Main_CHN == 1)
    *((volatile uint8_t *)(0x2003)) = 0xA7;    
    else if(MS_Main_CHN == 2)
    *((volatile uint8_t *)(0x2203)) = 0xA7;
}

//-----------------------------------------------------------------------------
// Handle command A8 - Enable Auxiliary Device Interface
//-----------------------------------------------------------------------------
void Command_A8(void)
{
    Host_Flag_DISAB_AUX = 0; // Enable aux device (mouse)
    Host_Flag &= 0xdf;

    if(MS_Main_CHN == 1)
    *((volatile uint8_t *)(0x2003)) = 0xA8; 
    else if(MS_Main_CHN == 2)
    *((volatile uint8_t *)(0x2203)) = 0xA8;  
}

//-----------------------------------------------------------------------------
// Handle command A9 - Test Aux Device Interface
// Returns: test error code:
//             0 = no error
//             1 = Clock line stuck low
//             2 = Clock line stuck high
//             3 = Data line stuck low
//             4 = Data line stuck high
//-----------------------------------------------------------------------------
void Command_A9(void)
{
    //MULPX_Multiplex = 0;        // Reveret to Legacy Mode 
    Transmit_Data_To_Host( 0x00 );
}

void Command_AA(void)
{
    //Host_Flag_SYS_FLAG = 1;
   // MULPX_Multiplex = 0;        // Reveret to Legacy Mode 
    Transmit_Data_To_Host( 0x55 );
}

void Command_AB(void)
{
    Transmit_Data_To_Host( 0x00 );
}

//-----------------------------------------------------------------------------
// Handle command AC - Diagnostic Dump
//-----------------------------------------------------------------------------
void Command_AC(void)
{
    //Kbd_Response = respCommand_AC;// Send multibyte sequence.
    //Tmp_Byte[0] = 0;   Tmp_Byte[0] will be used as the index for data.
}

void Command_AD(void)
{
    Host_Flag_DISAB_KEY = 1;        // Disable auxiliary keyboard.
    Host_Flag |= 0x10;

    if(KB_Main_CHN == 1)
    *((volatile uint8_t *)(0x2003)) = 0xAD;
    else if(KB_Main_CHN ==2)
    *((volatile uint8_t *)(0x2203)) = 0xAD;
}

//-----------------------------------------------------------------------------
// Handle command AE - Enable Keyboard Interface
//-----------------------------------------------------------------------------
void Command_AE(void)
{
    Host_Flag_DISAB_KEY = 0;        // Enable auxiliary keyboard.
    Host_Flag &= 0xef;

    if(KB_Main_CHN == 1)
    *((volatile uint8_t *)(0x2003)) = 0xAE;
    else if(KB_Main_CHN ==2)
    *((volatile uint8_t *)(0x2203)) = 0xAE;
}

void A20GATE_ON(void)
{

}

void A20GATE_OFF(void)
{

}

void Command_D1(void)
{
    #if CONTROL_A20_WAY3
        FastA20 = 1;
        return;
    #endif

    #if CONTROL_A20_WAY2
        if(Loop_Wait_Get_Port60_Data())
        {
            if( (KBHIData & 0x02) != 0x00 )
            {
                A20GATE_ON();
            }
            else
            {
                A20GATE_OFF();
            }
        }
        return;
    #endif
 
     #if CONTROL_A20_WAY1
        if( Set_Port60_Data_Handle() )
        {
            if ( (KBHIData & 0x02) != 0x00 )
            {
                A20GATE_ON();
            }
            else
            {
                A20GATE_OFF();
            }
        }
     #endif
}


void Command_D2(void)
{
    if( Set_Port60_Data_Handle() )
    {
        Transmit_Data_To_Host(KBHIData);
    }

}

void Command_E0(void)
{
    Transmit_Data_To_Host(0x00);
}

//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V  CmdDX_table[16] =
{
    Command_D0,             // Process 64 port command D0
    Command_D1,             // Process 64 port command D1
    Command_D2,             // Process 64 port command D2
    Command_D3,             // Process 64 port command D3
    Command_D4,             // Process 64 port command D4
    Command_D5,             // Process 64 port command D5
    Command_D6,             // Process 64 port command D6
    Command_D7,             // Process 64 port command D7
    Command_D8,             // Process 64 port command D8
    Command_D9,             // Process 64 port command D9
    Command_DA,             // Process 64 port command DA
    Command_DB,             // Process 64 port command DB
    Command_DC,             // Process 64 port command DC
    Command_DD,             // Process 64 port command DD
    Command_DE,             // Process 64 port command DE
    Command_DF              // Process 64 port command DF
};

void Command_D4(void)
{
    #if KB_MS_DEBUG
        dprint("Command_D4 \n");
    #endif

    #if !(PS2M_CLOCK_EN&PS2K_CLOCK_EN)
        dprint("PS2 CLOCK NOT ENABLE\n");
        return;
    #endif

    BYTE index;
    //MULPX_Multiplex = 0;

    if( Set_Port60_Data_Handle() )               // Get driver command from host
    {
        #if 1
            if(KBHIData==0xFF)              // if is reset command
            {

            }

            if(MS_Main_CHN!=0x00)        // mouse device is attached 
            {
                Send_Cmd_To_PS2_Mouse((MS_Main_CHN-1));
            }else{
                Command_A8();
                vDelayXms(20);              // Emulate transmission delay times 
                Send_Aux_Data_To_Host(0xFC);
            }
	    #endif
    }
    else
    {

    }
}

void Command_DX(void)
{
    (CmdDX_table[KBHICmd&0x0F])();
}

#if SUPPORTED_KBC_EX
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V  CmdEX_table[16] =
{
    Command_E0,             // Process 64 port command E0
    Command_E1,             // Process 64 port command E1
    Command_E2,             // Process 64 port command E2
    Command_E3,             // Process 64 port command E3
    Command_E4,             // Process 64 port command E4
    Command_E5,             // Process 64 port command E5
    Command_E6,             // Process 64 port command E6
    Command_E7,             // Process 64 port command E7
    Command_E8,             // Process 64 port command E8
    Command_E9,             // Process 64 port command E9
    Command_EA,             // Process 64 port command EA
    Command_EB,             // Process 64 port command EB
    Command_EC,             // Process 64 port command EC
    Command_ED,             // Process 64 port command ED
    Command_EE,             // Process 64 port command EE
    Command_EF              // Process 64 port command EF
};
#endif

void Command_EX(void)
{
    #if KB_MS_DEBUG
        dprint("Command_EX \n");
    #endif

    #if SUPPORTED_KBC_EX
        (CmdEX_table[KBHICmd&0x0F])();
    #endif
}

void Command_FX(void)
{

    if( (KBHICmd % 2) == 0x00 )  // Even command
    {
        A20GATE_OFF();
        //Microsecond_Delay(64); // Delay.
        A20GATE_ON();
    }                            // Odd command do no thing

    //CLEAR_BIT( Host_Flag, 2 ); //FIXME xia
    CLEAR_BIT( KBC_STA, 2 );

}

//-----------------------------------------------------------------------------
// Process Command/Data received from System via the KBC interface
//-----------------------------------------------------------------------------
const FUNCT_PTR_V_V  Port64_Table[16] =
{
    Command_00_3F,          // Process command 0x
    Command_00_3F,          // Process command 1x
    Command_00_3F,          // Process command 2x
    Command_00_3F,          // Process command 3x
    Command_40_7F,          // Process command 4x
    Command_40_7F,          // Process command 5x
    Command_40_7F,          // Process command 6x
    Command_40_7F,          // Process command 7x
    Command_8X,             // Process command 8x
    Command_9X,             // Process command 9x
    Command_AX,             // Process command Ax
    Command_BX,             // Process command Bx
    Command_CX,             // Process command Cx
    Command_DX,             // Process command Dx
    Command_EX,             // Process command Ex
    Command_FX,             // Process command Fx
};

//-----------------------------------------------------------------------------
// Service Host Port 60/64 Keyboard Controller or Keyboard Command
//用于接收host发送到ec的指令和数据，并依据情况回传ack，回传通过调用Data_To_Host和KBC_Data_To_Host函数进行
//-----------------------------------------------------------------------------
void Service_PCI_Main(void)
{
    #if ENABLE_DEBUGGER_SUPPORT
        /* Debugger record */
        Debugger_KBC_PMC_Record(0,0,KBC_IB);
    #endif

    if( KBC_STA & KBC_STA_A2 )          // if Command
    {
        FastA20  = 0;
        if( KBHIStep > 0 )      // Command Start
        {
            KBHIStep = 0;
        }
        KBHICmd = KBC_IB;

        #if SUPPORT_8042DEBUG_OUTPUT
            Write_Debug_Data_To_Sram(KBHICmd);
        #endif

        #if KB_MS_DEBUG    
            dprint("KBHICmd is 0x%x \n",KBHICmd);
        #endif
            (Port64_Table[(KBHICmd >>4)])();//use Transmit_Data_To_Host() to send data
    }
    else    // Data
    {
        if( KBHIStep || FastA20 )       // If need data
        {
            KBHIData  = KBC_IB;

            #if SUPPORT_8042DEBUG_OUTPUT
	            Write_Debug_Data_To_Sram(KBHIData);
            #endif

            #if KB_MS_DEBUG    
	            dprint("KBHIData is 0x%x \n",KBHIData);
            #endif

            #if CONTROL_A20_WAY3
                if( FastA20 )
                {
                    FastA20 = 0;
                    if( (KBHIData & 0x02) != 0x00 )
                    {
                        A20GATE_ON();
                    }
                    else
                    {
                        A20GATE_OFF();
                    }
                    return;
                }
            #endif

            (Port64_Table[(KBHICmd >>4)])();//use Transmit_Data_To_Host() to send data

        }
        else
        {
            KBHIData  = KBC_IB;

            #if SUPPORT_8042DEBUG_OUTPUT
	            Write_Debug_Data_To_Sram(KBHIData);
            #endif
            #if KB_MS_DEBUG    
	            dprint("KB_Cmd_Handle is 0x%x \n",KBHIData);
            #endif

            Command_AE();
            KB_Cmd_Handle(KBHIData);         // Keyboard Command

        }
    }

    //irqc_enable_interrupt(KBC_IBF_INTERRUPT_OFFSET); //enable kbc ibf int

}

//----------------------------------------------------------------------------
// FUNCTION: Service_PCI
// KBC PORT service function
//----------------------------------------------------------------------------
void __weak Service_PCI(void)
{
#if (Service_PCI_START == 1)

    #if !(PMCKBC_CLOCK_EN)
        dprint("PMCKBC CLOCK NOT ENABLE\n");
        return;
    #endif

	#if LPC_WAY_OPTION_SWITCH

		if (Is_FLAG_CLEAR(KBC_STA,KBC_STA_IBF))
			return;
        if(TP_ACK_CUNT != 0x0) return;  //if ps2 device command response count != 0, then return
		Service_PCI_Main();

	#else
		if(F_Service_PCI == 1)
		{
			F_Service_PCI = 0;

			if (Is_FLAG_CLEAR(KBC_STA,KBC_STA_IBF))
				return;
				Service_PCI_Main();
		}
	#endif
#endif

}

//----------------------------------------------------------------------------
// FUNCTION: Write_Debug_Data_To_Sram
// Output port6064 data to sram for debug 8042 initial process
//----------------------------------------------------------------------------
#if SUPPORT_8042DEBUG_OUTPUT
void Write_Debug_Data_To_Sram(uint8_t Data)
{
        if(Debug_Num <= 4096)
        {
	 		*((volatile uint8_t *)(0x31000+Debug_Num)) = Data;
         	Debug_Num++;
        }
        else
            return;
}
#endif

//-----------------------------------------------------------------------------
