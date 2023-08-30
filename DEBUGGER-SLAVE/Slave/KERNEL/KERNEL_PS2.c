/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-10 17:27:30
 * @Description: This file is used to handling PS2 interface
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

#include "KERNEL_PS2.H"

#include "KERNEL_SOC_FUNC.H"
#include "AE_DEBUGGER.H"

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Read_Status
// 
//----------------------------------------------------------------------------
BYTE  PS2_PortN_Read_Status(BYTE channel)
{
	volatile u8 ps2_portn_status; 

	if(channel == 0){
		ps2_portn_status = PS2_PORT0_SR & 0xFF ; 
	} else if(channel == 1){
		ps2_portn_status = PS2_PORT1_SR & 0xFF ; 
	} else {
		dprint("channel error");
	}

	return ps2_portn_status;
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Read_Input
// 
//----------------------------------------------------------------------------
BYTE  PS2_PortN_Read_Input(BYTE channel) 
{
	volatile u8 ps2_portn_input;

	if(channel == 0){
		ps2_portn_input = PS2_PORT0_IBUF & 0xFF ; 
	} else if(channel == 1){
		ps2_portn_input = PS2_PORT1_IBUF & 0xFF ; 
	} else {
		dprint("channel error");
	}

	return ps2_portn_input;
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Output
// 
//----------------------------------------------------------------------------
void PS2_PortN_Write_Output(u8 data, BYTE channel) 
{
	if(channel == 0){
		PS2_PORT0_OBUF = (data & 0xFF); 
	} else if(channel == 1){
		PS2_PORT1_OBUF = (data & 0xFF); 
	} else {
		dprint("channel error");
	}
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Command
// 
//----------------------------------------------------------------------------
void PS2_PortN_Write_Command(u8 data, BYTE channel)
{
	if(channel == 0){
		PS2_PORT0_CR = (data & 0xFF); 
	} else if(channel == 1){
		PS2_PORT1_CR = (data & 0xFF); 
	} else {
		dprint("channel error");
	}
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Read_Data
// 
//----------------------------------------------------------------------------
static int PS2_PortN_Read_Data(BYTE channel)
{
	BYTE  val;
	BYTE  status;
	val = -1;

	status = PS2_PortN_Read_Status(channel);

	if(status & 0x1)
	{
		val = PS2_PortN_Read_Input(channel);
		dprint("PS2_PORT%x_Read_Data %x \n",channel,val);

		if(status & (KBD_STAT_GTO | KBD_STAT_PERR))
			val = -2;
	}

	return val;
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Wait_For_Input
// 
//----------------------------------------------------------------------------
static int PS2_PortN_Wait_For_Input(BYTE channel)
{
	unsigned long timeout;
	BYTE  val;

	timeout = KBD_TIMEOUT;
	val =  PS2_PortN_Read_Data(channel);

	while(val < 0)
	{
		if(timeout -- == 0)
			return -1;

		udelay(50000);
		val= PS2_PortN_Read_Data(channel);
	}

	return val;
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Wait_For_Input
// 
//----------------------------------------------------------------------------
static int PS2_PortN_Wait(BYTE channel)
{
	unsigned long timeout = KBC_TIMEOUT * 10;

	do{
		BYTE  status = PS2_PortN_Read_Status(channel);

		if(!(status & 0x2))
			return 0; /* OK */

		udelay(1000);
		timeout--;

	}while(timeout);

	return 1;
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Command_W
// 
//----------------------------------------------------------------------------
static void PS2_PortN_Write_Command_W(char data,BYTE channel)
{
	if(PS2_PortN_Wait(channel))
		dprint("timeout in PS2_Port%x_Write_Command_W\n",channel);
	PS2_PortN_Write_Command(data,channel);
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PORT1_Write_Output_W
// 
//----------------------------------------------------------------------------
void PS2_PortN_Write_Output_W(char data,BYTE channel)
{
	if(PS2_PortN_Wait(channel))
		dprint("timeout in PS2_PORT1_Write_Command_W \n");
	PS2_PortN_Write_Output(data,channel);
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Cmd
// 
//----------------------------------------------------------------------------
void PS2_PortN_Write_Cmd(char cmd,BYTE channel)
{
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Command(0x60,channel);   /* 0x60 */
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Output(cmd,channel);
}

//----------------------------------------------------------------------------
// FUNCTION: PS2_PortN_Write_Ack
// 
//----------------------------------------------------------------------------
static void PS2_PortN_Write_Ack(char val,BYTE channel)
{
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Command(0x60,channel);
	PS2_PortN_Wait(channel);
	PS2_PortN_Write_Output(val,channel);
	ps2_portn_reply_expected ++;
	PS2_PortN_Wait(channel);
}

//----------------------------------------------------------------------------
// FUNCTION: Send_Mouse_Data
// 
//----------------------------------------------------------------------------
static int Send_Mouse_Data(BYTE  data,BYTE channel)
{
	int retries = 3;

	do{
		unsigned long timeout = KBD_TIMEOUT;

		acknowledge = 0;
		resend = 0;
		PS2_PortN_Write_Output_W(data,channel);
		for(;;) {
			if(acknowledge)
				return 1;
			if(resend)
				break;
			udelay(1);
			if(!--timeout) {
				return 0;
			}
		}
	}while(retries-- > 0);
}

//----------------------------------------------------------------------------
// FUNCTION: Send_Kbd_Data
// 
//----------------------------------------------------------------------------
static int Send_Kbd_Data(BYTE  data,BYTE channel)
{
	int retries = 3;

	do{
		unsigned long timeout = KBD_TIMEOUT;

		acknowledge = 0;
		resend = 0;
		PS2_PortN_Write_Output_W(data,channel);
		for(;;) {
			if(acknowledge)
				return 1;
			if(resend)
				break;
			udelay(1);
			if(!--timeout) {
				return 0;
			}
		}
	}while(retries-- > 0);
}

//-----------------------------------------------------------------
// Function: Wait_PS2_Device_Ack_Timeout
// Description: wait PS2 device ack until time out 
//-----------------------------------------------------------------
BYTE Wait_PS2_Device_Ack_Timeout(BYTE channel)
{
	BYTE result;
	BYTE receive_id_ack[3];
	BYTE cnt = 0;
	result = 0x1;

	TIMER_Disable(0x3);
    TIMER_Init(TIMER3,24000*30,0x0,0x1);

	do{                   // Wait PS2 transaction Done Status
		BYTE  status = PS2_PortN_Read_Status(channel);
		BYTE  ack;
			
        if(status & PS2_STR_IBF)
        {
            ack = PS2_PortN_Read_Input(channel);
    
            if(!(status & (KBD_STAT_PERR))) {
                    
                receive_id_ack[cnt]= ack;

				dprint("0xF2 ack 0x%x,cnt:%x\n", receive_id_ack[cnt],cnt);

				if(cnt == 1)
				{
					if(receive_id_ack[1] == 0xAB)
					{
						if(channel == 0){

							PS2_PORT0_FLAG = PORT0_DEVICE_IS_KEYBOARD;

						} else if(channel == 1){

							PS2_PORT1_FLAG = PORT1_DEVICE_IS_KEYBOARD;

						} else 
							dprint("channel error\n");
					} else {
						if(channel == 0){

							PS2_PORT0_FLAG = PORT0_DEVICE_IS_MOUSE;

						} else if(channel == 1){

							PS2_PORT1_FLAG = PORT1_DEVICE_IS_MOUSE;
						} else 
							dprint("channel error\n");
					}
				}

				cnt++;	
                result = 0x0;

            } else {
                dprint("ack data error\n");
            }
        }
	}while(((*(VBYTE*)(0x18a8))&0x8) != 0x8);					// waitting for overflow flag

	TIMER_Disable(0x3);

	dprint("PS2_PORT0_FLAG:%x,PS2_PORT1_FLAG:%x\n",PS2_PORT0_FLAG,PS2_PORT1_FLAG);

	return(result);
}

//-----------------------------------------------------------------
// Function: PS2_Device_Identificate
// Description: automatic identificate ps2 device
//-----------------------------------------------------------------
BYTE PS2_Device_Identificate(BYTE PortNum, BYTE cmd)
{
	BYTE result;
	BYTE index;

	PS2_PortN_Write_Cmd(0x30,PortNum);
	PS2_PortN_Write_Output_W(cmd,PortNum);

	result = Wait_PS2_Device_Ack_Timeout(PortNum);

    return(result);	
}

//-----------------------------------------------------------------
// Function: PS2_Channel_Device_Scan
// Description: KBD_SCAN ps2 channel device
//-----------------------------------------------------------------
void PS2_Channel_Device_Scan(void)
{
	/*KBD_SCAN ps2 port0/1 device*/
	for(BYTE i=0;i<2;i++)
	{
		PS2_Device_Identificate(i, 0xF2);
	}
}

//-----------------------------------------------------------------
// Function: PS2_Main_Device_Confirm
// Description: confirm channel main device
//-----------------------------------------------------------------
//BYTE MS_Main_CHN,KB_Main_CHN = 0;
void PS2_Main_Device_Confirm(void)
{
	MS_Main_CHN = 0;
	KB_Main_CHN = 0;

	dprint("MS_Main_CHN:0x%x,KB_Main_CHN:0x%x\n",MS_Main_CHN,KB_Main_CHN);
	dprint("PS2_PORT_FLAG:0x%x\n",PS2_PORT0_FLAG|PS2_PORT1_FLAG);
	
	switch(PS2_PORT0_FLAG|PS2_PORT1_FLAG)
	{
		case 0x00:
			MS_Main_CHN = 0;
			KB_Main_CHN = 0;	
			break;
		case 0x01:
			if(PS2_PORT0_FLAG & PORT0_DEVICE_IS_MOUSE)
			{
				MS_Main_CHN = 0x1;
			} else {
				MS_Main_CHN = 0x2;
			}
			KB_Main_CHN = 0;	
			break;
		case 0x02:
			if(PS2_PORT0_FLAG & PORT0_DEVICE_IS_KEYBOARD)
			{
				KB_Main_CHN = 0x1;
			} else {
				KB_Main_CHN = 0x2;
			}
			MS_Main_CHN = 0;	
			break;
		case 0x03:
			if(PS2_PORT0_FLAG & PORT0_DEVICE_IS_MOUSE)
			{
				MS_Main_CHN = 0x1;
				KB_Main_CHN = 0x2;
			} else {
				MS_Main_CHN = 0x2;
				KB_Main_CHN = 0x1;
			}	
			break;
		default:
			MS_Main_CHN = 0;
			KB_Main_CHN = 0;	
			break;
	}
	dprint("MS_Main_CHN:0x%x,KB_Main_CHN:0x%x\n",MS_Main_CHN,KB_Main_CHN);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: PS2 controlller selftest
 * ------------------------------------------------------------------------- */
int PS2_PortN_Selftest(BYTE channel)
{
	BYTE status,value;
	BYTE retry_num = 0 ;

	PS2_PortN_Write_Command_W(0xAA,channel);
	if(PS2_PortN_Wait_For_Input(channel) != 0x55)
		return 1;

	PS2_PortN_Write_Command_W(0xAB,channel);
	if(PS2_PortN_Wait_For_Input(channel) != 0x00)
		return 1;

	return 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: all PS2 controlller selftest
 * ------------------------------------------------------------------------- */
void PS2_All_Port_Selftest(void)
{
	for(BYTE i=0;i<2;i++)
	{
		if(PS2_PortN_Selftest(i)==0){
			dprint("ps2 port%d selfetest success\n",i);
		} else {
			dprint("ps2 port%d selfetest failed\n",i);
		}
	}
}

//-----------------------------------------------------------------
// Function: Open_Aux
// 
//-----------------------------------------------------------------
void Open_Aux(BYTE channel)
{
	PS2_PortN_Write_Command_W(CCMD_MOUSE_ENABLE,channel);	/* 0xA8 */
	PS2_PortN_Write_Ack(AUX_ENABLE_DEV,channel);           	/* 0xF4 enable aux device */
	PS2_PortN_Write_Cmd(AUX_INTS_ON,channel);
	udelay(2000);
	Send_Mouse_Data(KBD_CMD_ENABLE,channel);      			/* 0xF4 Enable scanning */  
}

//-----------------------------------------------------------------
// Function: Open_Kbd
// 
//-----------------------------------------------------------------
void Open_Kbd(BYTE channel)
{
	PS2_PortN_Write_Command_W(CCMD_KBD_ENABLE,channel);		/* 0xAE */
	PS2_PortN_Write_Ack(AUX_ENABLE_DEV,channel);           	/* 0xF4 enable aux device */
	PS2_PortN_Write_Cmd(KBD_INTS_ON,channel);
	udelay(2000);
	Send_Kbd_Data(KBD_CMD_ENABLE,channel);      			/* 0xF4 Enable scanning */
}

//-----------------------------------------------------------------
// Function: Mouse_Initialize_Port
// 
//-----------------------------------------------------------------
static char * Mouse_Initialize_Port(BYTE port)
{
	dprint("Enter Mouse_Initialize \n");

	char status;
	char retry_num = 0 ;

	PS2_PortN_Write_Command_W(0xAA,port);
	if(PS2_PortN_Wait_For_Input(port) != 0x55)
		return "Mouse:	failed self test";

	PS2_PortN_Write_Command_W(0xAB,port);
	if(PS2_PortN_Wait_For_Input(port) != 0x00)
		return "Mouse: interface failed self test";

	PS2_PortN_Write_Command_W(0x60,port);
	PS2_PortN_Write_Output_W(0x22,port);
	PS2_PortN_Write_Command_W(0x20,port);
	if(PS2_PortN_Wait_For_Input(port) != 0x22)
		return "Mouse: irq enable failed";

	PS2_PortN_Write_Command_W(0xA8,port);
	udelay(1000);

	do {
		PS2_PortN_Write_Output_W(0xF4,port);
		//udelay(1000);
		status = PS2_PortN_Wait_For_Input(port);
		
	    if(status == 0xFA)
			break;
		if(status == 0xFE) {
			return "Mouse: reset failed, no ACK";
		}
		retry_num++;
	}while(retry_num < 5);

	dprint("Before Open_Aux \n");

	Open_Aux(port);

	return 0;

}

//-----------------------------------------------------------------
// Function: Kbd_Initialize_Port
// 
//-----------------------------------------------------------------
static char * Kbd_Initialize_Port(BYTE port)
{
	dprint("Enter Kbd_Initialize \n");

	char status;
	char retry_num = 0 ;

	PS2_PortN_Write_Command_W(0xAA,port);
	if(PS2_PortN_Wait_For_Input(port) != 0x55)
		return "Kbd:failed self test";

	PS2_PortN_Write_Command_W(0xAB,port);
	if(PS2_PortN_Wait_For_Input(port) != 0x00)
		return "Kbd:interface failed self test";

	PS2_PortN_Write_Command_W(0x60,port);
	PS2_PortN_Write_Output_W(0x11,port);
	PS2_PortN_Write_Command_W(0x20,port);
	if(PS2_PortN_Wait_For_Input(port) != 0x11)
		return "Kbd: irq enable failed";

	PS2_PortN_Write_Command_W(0xAE,port);
	udelay(1000);

	do {
		PS2_PortN_Write_Output_W(0xFF,port);
		//udelay(1000);
		status = PS2_PortN_Wait_For_Input(port);

	    if(status == 0xFA)
			break;
		if(status == 0xFE) {
			return "Kbd: reset failed, no ACK";
		}
		retry_num++;
	}while(retry_num < 5);

	dprint("Before Open_Kbd \n");

	Open_Kbd(port);

	return 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: PS2 Mouse Init Interface
 * ------------------------------------------------------------------------- */
int Mouse_Init_HW(BYTE channel)
{
	char * result;

	//result = Mouse_Initialize();
	result = Mouse_Initialize_Port(channel);

	if(result == NULL) {
		dprint("AT Mouse initialized \n");
		return 1;
	} else {
		dprint("%s\n",result);
		return -1;
	}

	return 1;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: PS2 Keyboard Init Interface
 * ------------------------------------------------------------------------- */
int Kbd_Init_HW(BYTE channel)
{
	char * result ;

	//result = Kbd_Initialize();
	result = Kbd_Initialize_Port(channel);

	if(result == NULL) {
		dprint("KBD initialized \n");
		return 1;
	} else {
		dprint("%s\n",result);
		return -1;
	}

	return 1;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Init PS2 Device
 * ------------------------------------------------------------------------- */
int AE10x_PS2_Init(void)
{
	/* init mouse regs */
		PS2_PORT0_CPSR  = 0x7d;
		PS2_PORT0_DVR  = 0xc; 
		PS2_PORT1_CPSR = 0x7d;
		PS2_PORT1_DVR  = 0xc;

	/*enable ps2 channel 1/2 device for scan ps2 channel*/
	PS2_PortN_Write_Command_W(0x60,1);
	PS2_PortN_Write_Output_W(0x30,1);
	PS2_PortN_Write_Command_W(0x60,2);
	PS2_PortN_Write_Output_W(0x30,2);

	//KBD_SCAN ps2 device
	PS2_Channel_Device_Scan();	

	//confirm ps2 main devices
	PS2_Main_Device_Confirm();

	//all ps2 port selftest
	PS2_All_Port_Selftest();

	/*disable ps2 channel 1/2 after scan ps2 channel*/
	PS2_PortN_Write_Command_W(0x60,1);
	PS2_PortN_Write_Output_W(0x0,1);
	PS2_PortN_Write_Command_W(0x60,2);
	PS2_PortN_Write_Output_W(0x0,2);

	/*if(MS_Main_CHN != 0)
	{
		if(Mouse_Init_HW(MS_Main_CHN - 1) == -1)
			return -1;
	}

	if(KB_Main_CHN != 0)
	{
		if(Kbd_Init_HW(KB_Main_CHN - 1) == -1)
			return -1;
	}*/

	return 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: PS2 Device Init Interface
 * ------------------------------------------------------------------------- */
int PS2_Init(void)
{
	dprint("init ps2 modules\n");

	if(0 != AE10x_PS2_Init())
	{
		dprint("init ps2 failed");
		return -1;
	}
	
	return 0;
}
BYTE  mouse_val;
/* ----------------------------------------------------------------------------
 * FUNCTION: Handle PS2 Mouse Data
 * ------------------------------------------------------------------------- */
void  Handle_Mouse(BYTE channel)
{
  	BYTE  status = PS2_PortN_Read_Status(channel);
  	BYTE  scancode;
 
  	if(status & PS2_STR_IBF)
  	{
		scancode = PS2_PortN_Read_Input(channel);
		if(!(status & (KBD_STAT_PERR))) 
		{
				mouse_val = scancode;

				Send_Aux_Data_To_Host(mouse_val);
		}
		else
		{
				dprint("mouse scan_code error \n");
		}
	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Handle PS2 Keyboard Data
 * ------------------------------------------------------------------------- */
void  Handle_Kbd(BYTE channel)
{
	BYTE  status = PS2_PortN_Read_Status(channel);
	BYTE  scancode;

	if(status & PS2_STR_IBF)
	{
		scancode = PS2_PortN_Read_Input(channel);	
		if(!(status & (KBD_STAT_PERR))) 
		{
			PS2_IRQ_Channel = 0x1;
			PS2_PortN_Data[1] = scancode;
			KBS_Buffer_Input(scancode);
		}
		else
		{
			dprint("kbd scan_code error \n");
		}
	}
}

BYTE  Handle_Mouse_Event(BYTE channel)
{
	BYTE  status = PS2_PortN_Read_Status(channel);
	DWORD work = 10000;
	BYTE  scancode ;

	dprint("handle status 0x%x \n",status);

	while((--work > 0) && status & 0x1)
	{
		scancode = PS2_PortN_Read_Input(channel);
		if(!(status & (KBD_STAT_GTO | KBD_STAT_PERR))) 
		{
			dprint("scan_code %d,%x \n",scancode,scancode);
		}
		else
		{
			dprint("mouse scan_code error \n");
		}
		status = PS2_PortN_Read_Status(channel);
	}

	if(!work)
		dprint("pc_mouse: controller jammed (0x%02X).\n",status);

	return status;
}

BYTE  Handle_Kbd_Event(BYTE channel)
{
	BYTE  status = PS2_PortN_Read_Status(channel);
	DWORD work = 10000;
	BYTE  scancode ;

	dprint("handle status 0x%x \n",status);

	while((--work > 0) && status & 0x1)
	{
		scancode = PS2_PortN_Read_Input(channel);

		if(!(status & (KBD_STAT_GTO | KBD_STAT_PERR)))
		{
			dprint("scan_code %d,%x \n",scancode,scancode);
		}
		else
		{
			dprint("mouse scan_code error \n");
		}
		status = PS2_PortN_Read_Status(channel);
	}

	if(!work)
		dprint("pc_mouse: controller jammed (0x%02X).\n",status);

	return status;
}
/*----------------------PS2 Interface Handle----------------------*/

//-----------------------------------------------------------------
// Send data from aux mouse 
//-----------------------------------------------------------------
int Send_Aux_Data_To_Host(BYTE auxdata)
{
	if(TP_ACK_CUNT == 0)  //ps2 pure
	{
		if(IS_SET( KBC_STA,1)||IS_SET( KBC_STA,0))
		{
			#if KB_MS_DEBUG
				dprint("MS_Data_Suspend \n"); 
			#endif
			MS_Data_Suspend(auxdata);
			return 0x00;
		}
	} else {
		while(IS_SET( KBC_STA,0));
	}

    KBC_STA &= 0x0f;    
    SET_BIT(KBC_STA,5);

    KBC_CTL &= 0xfc;    
    if(Host_Flag_INTR_AUX) //wzk_debug add
    {
        SET_BIT( KBC_CTL, 1 );
    }

    #if KB_MS_DEBUG
        dprint("adth:%x,icr:%x,isr:%x\n",auxdata,KBC_CTL,KBC_STA);
    #endif

	#if SUPPORT_8042DEBUG_OUTPUT
		Write_Debug_Data_To_Sram(auxdata);
	#endif

	if(TP_ACK_CUNT != 0x0)
	{
		TP_ACK_CUNT--;	
	}

	KBC_MOB = auxdata;

    #if ENABLE_DEBUGGER_SUPPORT
	    /* Debugger record */
	    Debugger_KBC_PMC_Record(1,0,auxdata);
    #endif
}

void MS_Data_Suspend(BYTE nPending)
{
    #if KB_MS_DEBUG
	    dprint("MSPendingRXCount is 0x%x \n",MSPendingRXCount);
    #endif

    if( MSPendingRXCount > 3 )  
	return;

	MSDataPending[(MSPendingRXCount&0x03)] = nPending;
        MSPendingRXCount++;
}

BYTE Release_MS_Data_Suspend(void)
{ 
    BYTE buffer_data;

    buffer_data=MSDataPending[(MSPendingTXCount&0x03)];
    MSPendingTXCount++;

    if( MSPendingTXCount >= MSPendingRXCount )
    {
        MSPendingTXCount = 0;
        MSPendingRXCount = 0;
    }

    #if KB_MS_DEBUG
        dprint("Release_MS_Data_Suspend is 0x%x \n",buffer_data);
    #endif
    return buffer_data;
}

//-----------------------------------------------------------------
// Send data to ps2 interface
//-----------------------------------------------------------------
void Send_Data_To_PS2( BYTE PortNum, BYTE PortData)
{
    #if KB_MS_DEBUG
        dprint("PortNum is 0x%x ,Send_Data_To_PS2 0x%x,MS_Main_CHN is 0x%x\n",PortNum,PortData,MS_Main_CHN);
    #endif

    PS2_ACK_Finish = 0;

    if (PortNum == 0)
	    PS2_PortN_Write_Output_W(PortData,PortNum);
    else if(PortNum == 1)
	    PS2_PortN_Write_Output_W(PortData,PortNum);

}

/* ----------------------------------------------------------------------------
 * FUNCTION: Handle PS2 Mouse Command
 * ------------------------------------------------------------------------- */
void Send_Cmd_To_PS2_Mouse(BYTE PortNum)
{
    BYTE index;
    Command_A8();   
    TP_ACK_CUNT = 0x00;
    MS_ID_Count = 0x00;

	if(AUX2ByteCommand)
	{	
		if(MS_Resolution == 0xFF)
		{
			MS_Resolution = KBHIData;
		}
		else if(MS_Sampling_Rate == 0xFF)
		{
			MS_Sampling_Rate = KBHIData;		
		}
        #if KB_MS_DEBUG
                dprint("AUX2ByteCommand \n");
        #endif	
		Send_Data_To_PS2(PortNum, KBHIData);	
		AUX2ByteCommand = 0;
		return;
	}

	switch(KBHIData)
	{
		case 0xE8:     			// Set Resolution (2 byte)
			MS_Resolution = 0xFF;
			AUX2ByteCommand=1;
			break;
		
		case 0xF3:     			// Set Sampling Rate (2 byte)
			MS_Sampling_Rate = 0xFF;	
			AUX2ByteCommand=1;
			break;	
			
		case 0xF4:	
			EnableTP = 1;
			MS_Report_Flag = 1;
			break;
			
		case 0xF5:
			MS_Report_Flag = 0;
			break;	
			
		case 0xE6:
			break;
			
		case 0xE7:
			break;
			
		case 0xEA:
			break;
			
		case 0xEC:
			break;
			
		case 0xEE:
			break;
			
		case 0xF0:
			break;
			
		case 0xF6:		
			break;	
			
		case 0xE9:	
			break;
			
		case 0xF2:	
            		MS_ID_Count = 2;
			break;
			
		case 0xEB:
		case 0xFE:	
			break;
			
		case 0xFF:
			MS_Report_Flag = 0;
			EnableTP = 0;
			//TP_ACK_CUNT = 0x03;		// ACK 3 bytes
			break;
			
		default:			            // Invalid command
			break;
	}

    if(KBHIData==0xFF)      // Reset command
    {
        //SetPS2CmdACKCounter(3);
		TP_ACK_CUNT = 0x3;
    }
    else if(KBHIData==0xF2) // Read ID command
    {
        //SetPS2CmdACKCounter(2);
		TP_ACK_CUNT = 0x2;
    }
    else if(KBHIData==0xE9) // Read status cmmand
    {
        //SetPS2CmdACKCounter(4);
		TP_ACK_CUNT = 0x4;
    }
    else                    // Other commands
    {
        //SetPS2CmdACKCounter(1);
		TP_ACK_CUNT = 0x1;
    }

	Send_Data_To_PS2(PortNum, KBHIData);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Service_Send_PS2
 * Send PS2 Mouse pending data 
 * ------------------------------------------------------------------------- */
void Service_Send_PS2(void)
{
    #if (Service_Send_PS2_START == 1)

            if ( MSPendingRXCount > 0 )
            {
                MSCmdAck = Release_MS_Data_Suspend();
                #if KB_MS_DEBUG
                    dprint("MSCmdAck is 0x%x\n",MSCmdAck);
                #endif
                Send_Aux_Data_To_Host(MSCmdAck);
                return;
            }
        #endif
}

/*-----------------------------------------------------------------------------
 * End
 *---------------------------------------------------------------------------*/
