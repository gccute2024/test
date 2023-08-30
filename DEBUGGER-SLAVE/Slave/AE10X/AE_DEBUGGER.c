/*
 * @Author: Linyu
 * @LastEditors: Yangxiaolong  1426192352@qq.com
 * @LastEditTime: 2023-08-09 21:54:49
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

// Include all header file
#include "AE_DEBUGGER.H"
#include "KERNEL_KBS.H"

//*****************************************************************************
//
// global variables for Debugger api
//
//*****************************************************************************
char Uart_data[256];//An array of data transferred by the debugger
char iic_data[256] = {0};//An array of data transferred by the debugger
char Intr_num[256] = {0};//Interrupts count array
char buf[7];//Used to record register addresses and values when reading or writing registers
char KBD_Event[256] = {0};//An array of keyboard mouse key values
static struct KBD_Event kbd_event = {{0}, -1, 0};
char KBC_PMC_Event[256] = {0};//An array of records for KBC/PMC data
unsigned char iicFeedback = 0,iic_flag = 0,iic_int_flag = 0;;//  The state back to host
WORD iic_count,pos,iic_pos = 0;
//*****************************************************************************
//
// local variables for Debugger api
//
//*****************************************************************************
BYTE  CMD_Temp_Record = 0;//Instruction record at jump time
BYTE  uart_data = 0x0;//Debugger CMD data
char KBC_PMC_Pending[3][256] = {0};//KBC/PMC record value Pending array
char KBC_PMC_Pending_Out[256] = {0};//get KBC/PMC Pending out array
int KBC_PMC_PendingRXCount=0;//KBC/PMC Pending in index
int KBC_PMC_PendingTXCount=0;//KBC/PMC Pending out index
char dbg_int_buf[3];//Debugger interrupt function instruction array

extern void Flash_Update_Function(void);
#if 1//DEBUGGER_SLAVE	//only when chip set as slave
/*------------Only the slave debugger function is available-------------*/

//*****************************************************************************
//
//  The Debugger KBD Log Fill in Function
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		This function will Fill in nonsense key codes 0x99 when there are no key codes
//		Should be placed in the 0.04ms timer interrupt function
//*****************************************************************************
void KBD_ListeningEvent(void)
{
	if(F_Service_KBD_Fill==0)
	{
		return;
	}
	else
	{
		F_Service_KBD_Fill=0;
	}
	if(F_Service_KBL && (Event_Num < 256))//listen keyboard and count less than 256
	{
		if(!Key_Flag)//if no key pressed
		{
			KBD_Event[Event_Num] = 0x99;//fill in non-meaning code
			Event_Num ++;
		}
		else
		{
			Key_Flag=0;//clear no key press flag
		}
	}
}

//*****************************************************************************
//
//  The Debugger KBC PMC DataPending
//
//  parameter :
//      KBC_PMC_PendingData : Data Point Need to Pending
//
//  return :
//      none
//
//	note :
//		This function will put the full array into the pending array 
//		and then empty the array for further data storage. 
//		Therefore, the data in the pending array should be printed first in order
//*****************************************************************************
void KBC_PMC_DataPending(char * KBC_PMC_PendingData)
{
	dprint("KBC_PMC_PendingRXCount is 0x%x \n",KBC_PMC_PendingRXCount);
    if( KBC_PMC_PendingRXCount >= 3 )
	{
		return;
	}
	dprint("pending in data = 0x%x and 0x%x",KBC_PMC_PendingData[2],KBC_PMC_PendingData[3]);
	for(int i=0;i<256;++i)
	{
		KBC_PMC_Pending[KBC_PMC_PendingRXCount][i] = KBC_PMC_PendingData[i];//Dumps the currently stored array to the pending array
	}
	dprint("pending store data = 0x%x and 0x%x",KBC_PMC_Pending[KBC_PMC_PendingRXCount][2],KBC_PMC_Pending[KBC_PMC_PendingRXCount][3]);
	for(int i=0;i<256;++i)
	{
		KBC_PMC_PendingData[i] = 0;//clear array
	}
    KBC_PMC_PendingRXCount++;
}

//*****************************************************************************
//
//  KBC PMC Log Pending Out Function
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void GetKBC_PMC_PendingData(void)
{ 
	for(int i=0;i<256;++i)
	{
		KBC_PMC_Pending_Out[i] = KBC_PMC_Pending[KBC_PMC_PendingTXCount][i];//Dumps the currently pending array to the output array
		KBC_PMC_Pending[KBC_PMC_PendingTXCount][i] = 0;//Clear the original pending array
	}
    KBC_PMC_PendingTXCount++;
    if( KBC_PMC_PendingTXCount >= KBC_PMC_PendingRXCount )//If output all of pendind's data
    {
        KBC_PMC_PendingTXCount = 0;
        KBC_PMC_PendingRXCount = 0;
    }
    dprint("GetKBC_PMC_PendingData \n");
	dprint("pending in data = 0x%x and 0x%x",KBC_PMC_Pending_Out[2],KBC_PMC_Pending_Out[3]);
}

//*****************************************************************************
//
//  The Debugger Function for Record KBD Value
//
//  parameter :
//      value : value need to be record
//
//  return :
//      Function running condition
//
//*****************************************************************************
BYTE  Debugger_KBD_Record(BYTE  value)
{
	if(F_Service_KBL)
	{
		if (kbd_event.tail == kbd_event.head)
		{
			dprint("kbd buffer is overflow!");
			return 1;
		}
		dprint("kdb_event.tail: %x\n", kbd_event.tail);
		kbd_event.data[kbd_event.tail++] = value;
		dprint("kdb_event write: %x\n", kbd_event.data[kbd_event.tail - 1]);
		if (kbd_event.tail == KBD_BUFF_SIZE)
		{
			kbd_event.tail = 0;
		}
	}
	Event_Num = 1;
	return 0;
}

//*****************************************************************************
//
//  The Debugger Function Uart Entrance
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Buf_flag to make sure after default not get in this if function
//		0x55--read request
//		0xff--write request
//		0x88--Cancel set to slave so can get Debug_Temp
//		0xDD--Set to slave so can read data
//		0xAA--KBD read request
//		0xBB--cancel KBD read request
//		0xE0--Reset chip (use watchdog)
//		0xB0--Switching keyboard Codes
//		0xA5--Firmware update
//		0x11--Interrupt function block
//****************************************************************************
void Debugger_Cmd_Uart(BYTE debug_data)
{
	#if I2CToUart_Uart
	// dprint ("Debugger handler\n");
	uart_data = debug_data ;
	if ((!Buf_flag) && (uart_data == READ_REQUEST || uart_data == WRITE_REQUEST || uart_data == GET_DEBUG_TEMP || 
	uart_data == KBD_READ_REQUEST || uart_data == CANCEL_KBD_READ_REQUEST || uart_data == RECOARD_KBC_PMC_DATA || 
	uart_data == STOP_RECOARD_KBC_PMC_DATA || uart_data == SLAVE_READ_DATA || uart_data == RESET_CHIP_USE_WATCHDOG || 
	uart_data == SWITCH_KEYBOARD_CODE || uart_data == FIRMWARE_UPDATE || uart_data == INTERRUPT_FUNCTION))
	{
		switch (uart_data)
		{
			case GET_DEBUG_TEMP:
				Slave_flag = 0;
				DEBUGER_putchar(GET_DEBUG_TEMP);
				break;
			
			case SLAVE_READ_DATA:
				Slave_flag = 1;
				DEBUGER_putchar(SLAVE_READ_DATA);
				break;
	
			case KBD_READ_REQUEST:
				F_Service_KBL = 1;
				DEBUGER_putchar(KBD_READ_REQUEST);
				for(int i=0;i<256;i++)
					KBD_Event[i] = 0;//clear
				Event_Num = 0;	
				break;

			case CANCEL_KBD_READ_REQUEST:
				DEBUGER_putchar(CANCEL_KBD_READ_REQUEST);
				F_Service_KBL = 0;
				break;

			case RECOARD_KBC_PMC_DATA:
				KBC_PMC_Flag = 1;
				DEBUGER_putchar(RECOARD_KBC_PMC_DATA);
				for(int i=0;i<256;i++)
					KBC_PMC_Event[i] = 0;//clear
				KBC_PMC_Event_Num = 0;	
				break;

			case STOP_RECOARD_KBC_PMC_DATA:
				DEBUGER_putchar(0xEE);
				KBC_PMC_Flag = 0;
				break;

			case RESET_CHIP_USE_WATCHDOG://reset chip
				DEBUGER_putchar(RESET_CHIP_USE_WATCHDOG);
				ResetChipFlag = 1;
				dprint("Chip will be reset!!!\n");
				break;

			case SWITCH_KEYBOARD_CODE://change keyboard code type
				DEBUGER_putchar(SWITCH_KEYBOARD_CODE);
				if(1 == Host_Flag_XLATE_PC)
				{
					Host_Flag_XLATE_PC = 0;
					dprint("Keyboard change to 2 \n");
				}
				else
				{
					Host_Flag_XLATE_PC = 1;
					dprint("Keyboard change to 1 \n");
				}
				break;
			
			case FIRMWARE_UPDATE:
				DEBUGER_putchar(FIRMWARE_UPDATE);
				uart_updata_flag = 0x1;
				Flash_Update_Function();
				break;

			default:
				Buf_num = 0;
				Buf_flag = 1;
				CMD_Temp_Record = uart_data;
				return;
		}
	}

	if(Buf_flag)
	{
		switch (CMD_Temp_Record)
		{
			case READ_REQUEST:
				buf[Buf_num] = uart_data;//store received addr
				Buf_num ++;
				if(Buf_num == 4)//received addr finished
				{
					Buf_flag = 0;//clear flag
					CMD_Temp_Record = 0;
					F_Service_Data = 1;//enable Service_Data function in main loop
				}
				break;

			case WRITE_REQUEST:
				buf[Buf_num] = uart_data;
				Buf_num ++;
				if(Buf_num == 7)
				{
					Buf_flag = 0;
					CMD_Temp_Record = 0;
					F_Service_Change = 1;//enable Service_Change function in main loop
				}
				break;

			case INTERRUPT_FUNCTION:
				dbg_int_buf[int_buf_index] = uart_data;
				int_buf_index ++;
				if(int_buf_index == 3)
				{
					int_buf_index = 0;
					Buf_flag = 0;//clear flag
					CMD_Temp_Record = 0;
					F_Service_Debugger_Int = 1;//enable Service_Debugger_Int function in main loop
				}
				break;

			default :
				break;
		}
	}
	#endif
}


//*****************************************************************************
//
//  The Debugger Function I2c Entrance
//
//  parameter :
//      baseaddr : I2C channel
//
//  return :
//      none
//
//	note :
//		Buf_flag to make sure after default not get in this if function
//		0x55--read request
//		0xff--write request
//		0x88--Cancel set to slave so can get Debug_Temp
//		0xDD--Set to slave so can read data
//		0xAA--KBD read request
//		0xBB--cancel KBD read request
//		0xE0--Reset chip
//		0xB0--Switching keyboard Codes
//		0xA5--Firmware update
//		0x11--Interrupt function block
//****************************************************************************
void Debugger_Cmd_I2c(WORD i2c_channel)
{
	int data,i;
	int wdata[16];
	WORD baseaddr;
	baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	data = I2c_Slave_Read_Byte(baseaddr);
	if ((!Buf_Flag) && (data == READ_REQUEST || data == WRITE_REQUEST || data == GET_DEBUG_TEMP || 
	data == KBD_READ_REQUEST || data == CANCEL_KBD_READ_REQUEST || data == RECOARD_KBC_PMC_DATA ||
	data == STOP_RECOARD_KBC_PMC_DATA || data == RESET_CHIP_USE_WATCHDOG || 
	data == SWITCH_KEYBOARD_CODE || data == FIRMWARE_UPDATE || data == INTERRUPT_FUNCTION))
	{
		switch (data)
		{
			case SLAVE_READ_DATA:
				Slave_flag = 1;
				break;
			
			case GET_DEBUG_TEMP:
				*((DWORD*)(0x3830)) &=  ~(1 << 2);
				Slave_flag = 0;
				break;
			
			case KBD_READ_REQUEST:
				F_Service_KBL = 1;//star the kbd record	
				break;

			case CANCEL_KBD_READ_REQUEST:
				iicFeedback = 0;	
				F_Service_KBL = 0;
				iic_pos = 0;
				break;

			case RECOARD_KBC_PMC_DATA:
				KBC_PMC_Flag = 1;
				for(int i=0;i<256;i++)
				{
					KBC_PMC_Event[i] = 0;//clear
				}
				KBC_PMC_Event_Num = 0;	
				break;

			case STOP_RECOARD_KBC_PMC_DATA:
				KBC_PMC_Flag = 0;
				break;
			
			case RESET_CHIP_USE_WATCHDOG://reset chip
				ResetChipFlag = 1;
				dprint("Chip will be reset!!!\n");
				break;

			case SWITCH_KEYBOARD_CODE://change keyboard code type
				if(1 == Host_Flag_XLATE_PC)
				{
					Host_Flag_XLATE_PC = 0;
					dprint("Keyboard change to 2 \n");
				}
				else
				{
					Host_Flag_XLATE_PC = 1;
					dprint("Keyboard change to 1 \n");
				}
				break;
			
			case FIRMWARE_UPDATE:
				uart_updata_flag=0x1;
				Flash_Update_Function();
				break;

			default:
				Buf_Num = 0;
				int_buf_index = 0;
				Buf_Flag = 1;
				Debug_Temp = data;
				return;
		}
	}
	if(Buf_Flag)
	{

		switch (Debug_Temp)
		{
			
			case READ_REQUEST:
				buf[Buf_Num] = data;
				Buf_Num ++;
				if(Buf_Num == 4)
				{
					F_Service_Data = 1;
					Buf_Flag = 0;
					Debug_Temp = 0;
				}
				break;

			case WRITE_REQUEST:
				buf[Buf_Num] = data;
				Buf_Num ++;
				if(Buf_Num == 7)
				{
					F_Service_Change = 1;
					Buf_Flag = 0;
					Debug_Temp = 0;
				}
				break;
			
			case INTERRUPT_FUNCTION:
				dbg_int_buf[int_buf_index] = data;
				int_buf_index ++;
				if(int_buf_index == 3)
				{
					int_buf_index = 0;
					Buf_Flag = 0;//clear flag
					Debug_Temp = 0;
					F_Service_Debugger_Int = 1;//enable Service_Debugger_Int function in main loop
				}
				break;
			default :
				break;
		}
	}
}


//*****************************************************************************
//
//  The Debugger Function I2C Request
//
//  parameter :
//      baseaddr : I2C channel
//
//  return :
//      none
//
//*****************************************************************************

void Debugger_I2c_Req(WORD baseaddr)
{
	int data;
	char wdata[16];
	Slave_flag = 1;//set slave
	if(iic_flag == 1){//if the feedback datas have been sent 
		volatile BYTE int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
		int_status = (int_status|I2C_INTR_TX_EMPTY)&(~I2C_INTR_RD_REQ);
		//enable the tx_empty irqc and mask the req irqc to star transmit datas
		I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
		iic_flag = 0;
	}else{
		switch (iicFeedback)
		{
		case 0:
		//return "fe dd" mean the master read while slave does not recive any command 
			I2c_Slave_Write_Byte(0xFE,baseaddr);
			I2c_Slave_Write_Byte(0xDD,baseaddr);
			break;
		case 1:{
				//return two byte data to indicate master the "255 +1" datas need to be read 
				I2c_Slave_Write_Byte(0x01,baseaddr);
				I2c_Slave_Write_Byte(buf[3],baseaddr);
				iic_count = 0;//the number of datas have send by iic
				Num_flag = 0;	
				iic_flag = 1;
			}
			break;
		case 2:{//int mode
				//return two byte data to indicate master the "0 +1" datas need to be read
				I2c_Slave_Write_Byte(0x00,baseaddr);
				I2c_Slave_Write_Byte(0x01,baseaddr); 
				iic_flag = 1;
			}
			break;
		case 3:{
				//return two byte data to indicate master the "0 +1" datas need to be read
				I2c_Slave_Write_Byte(0x00,baseaddr);
				I2c_Slave_Write_Byte(0x01,baseaddr);
				iic_flag = 1;
		}
			break;
		case 4:{//kbc send
				F_Service_KBL = 0;//stop the kbd record until the datas have been sent
				I2c_Slave_Write_Byte(0x00,baseaddr);
				I2c_Slave_Write_Byte(iic_pos,baseaddr);
				iic_flag = 1;
				iic_count = 0;
				Num_flag = 0;  //clear the count num
		}
			break;

		default:
			return;
		}
	}



}


//*****************************************************************************
//
//  The Debugger Function Data Send Function
//  When I2C almost empty interrupts
//  Fill data in FIFO to send from I2C
//
//  parameter :
//      baseaddr : I2C channel
//
//  return :
//      none
//
//*****************************************************************************
void Debugger_I2c_Send(WORD baseaddr)
{
	int data;
	char wdata[16];
	volatile BYTE int_status;
	switch (iicFeedback)
	{
		case 0:
			break;
		case 1:{
				for (int i=0;i<8;i++){
					wdata[i] = iic_data[i + Num_flag * 8];
					iic_count++;
					if(iic_count == buf[3] + 1){
						char temp = iic_count%8;//计算剩余待发送字节
						if(temp == 0){
							if(Num_flag == 31)break;
							temp = 8;
						}
						I2c_Slave_Write_Block(wdata,temp,baseaddr);

						int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
						int_status = int_status&(~I2C_INTR_TX_EMPTY)|I2C_INTR_RD_REQ;
						I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
						iicFeedback = 0;
						return;
					}
				}		
				Num_flag ++;
				I2c_Slave_Write_Block(wdata,8,baseaddr);//i2c slave send data to fifo
				/* Finish data send*/
				if(Num_flag == 32){
					int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
					int_status = int_status&(~I2C_INTR_TX_EMPTY)|I2C_INTR_RD_REQ;
					I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
					iicFeedback = 0;
				}
		}
		break;
		case 2:{//int mode
				//the real data master want to read
				I2c_Slave_Write_Byte(0xFF,baseaddr);

				int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
				int_status = int_status&(~I2C_INTR_TX_EMPTY)|I2C_INTR_RD_REQ;
				I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
				iicFeedback = 0;	
		}
		break;
		case 3:{
				if(iic_int_flag == 1){
					I2c_Slave_Write_Byte(0x01,baseaddr);
				}
				else if(iic_int_flag == 2){
					I2c_Slave_Write_Byte(0x00,baseaddr);
				}
				else if (iic_int_flag == 3){
					I2c_Slave_Write_Byte(0x11,baseaddr);
				}
				else{
					I2c_Slave_Write_Byte(0xDD,baseaddr);
				}
				int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
				int_status = int_status&(~I2C_INTR_TX_EMPTY)|I2C_INTR_RD_REQ;
				I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
				iicFeedback = 0;
		}
		break;
		case 4:{//kbc send
				for (int i=0;i<16;i++){
					wdata[i] = iic_data[i + Num_flag * 16];
					iic_count++;
					if(iic_count == iic_pos){
						char temp = iic_count%16;//计算剩余待发送字节
						if(temp == 0){
							if(Num_flag == 15)break;
							temp = 16;
						}
						I2c_Slave_Write_Block(wdata,temp,baseaddr);
						/* Complete sending; Restore the Context*/
						F_Service_KBL = 1;
						iicFeedback = 0;
						iic_pos = 0;
						int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
						int_status = int_status&(~I2C_INTR_TX_EMPTY)|I2C_INTR_RD_REQ;
						I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
						return;
					}
				}		
				Num_flag ++;
				I2c_Slave_Write_Block(wdata,16,baseaddr);//i2c slave send data to fifo
				/* Complete sending; Restore the Context*/
				if(Num_flag == 16){
					F_Service_KBL = 1;
					iicFeedback = 0;
					iic_pos = 0;
					int_status = I2c_Readb(I2C_INTR_MASK_OFFSET,baseaddr);
					int_status = int_status&(~I2C_INTR_TX_EMPTY)|I2C_INTR_RD_REQ;
					I2c_Writeb(int_status,I2C_INTR_MASK_OFFSET,baseaddr);
				}
	}
		break;

	default:
		return;
	}
}

//*****************************************************************************
//
//  The Debugger Function for Data Read
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void Service_Data(void)//Debugger Function read
{
	if (F_Service_Data == 0)
	{
		return;
	}
	else if(F_Service_Data == 1)
	{
		F_Service_Data = 0;
	}
	
	/* Declaration of variables */
	int data_base;
	int length =1;
	char wdata[16];//data send to i2c

	uint8_t temp1;//For data lenth convert 
	uint16_t temp2;
	uint32_t temp4;

	/* Addr sequence transformation */
	data_base = (buf[0] << 16) | (buf[1] << 8) | buf[2];

	/* Register length conversion */
	if(data_base >= 0x10000 && data_base <= 0xBFFFF)//Different addr part has different lenth
	{
		length = 4;
	}
	else if(data_base >= 0x4800 && data_base <= 0x4FFF)//register lenth change
	//else if(data_base >= 0x4000 && data_base <= 0x7BFF)
	{
		length = 2;
	}
	else
	{
		length = 1;//lyx debug --spi need to read as 1 byte
	}

	/* Cumulative number of interrupts */
    if(data_base == 0x10000)//this addr will get int num
	{
		for(int i=0;i<256;i++)
		{
			Uart_data[i] = Intr_num[i];
			iic_data[i] = Intr_num[i];
		}
	}
	/* KBD key code */
	else if(data_base == 0x10001)
	{	
		/* 
		 The command to read the keyboard code in the debugger,
		 because the web end polling tends to cause the interrupt nesting problem,
		 so the case is discarded and replaced with the active send.
		*/
		/* Debugger_Send_KBD */
	}
	/* KBC/PMC value */
	else if(data_base == 0x10002)//this addr will get kbc/pmc value
	{
		if(KBC_PMC_PendingRXCount>0)//get pending data if has
		{
			GetKBC_PMC_PendingData();
			for(int i=0;i<256;i++)
			{
				Uart_data[i] = KBC_PMC_Pending_Out[i];
				iic_data[i] = KBC_PMC_Pending_Out[i];
				KBC_PMC_Pending_Out[i] = 0;
			}
		}
		else//pending no data
		{
			for(int i=0;i<256;i++)
			{
				Uart_data[i] = KBC_PMC_Event[i];//get value
				iic_data[i] = KBC_PMC_Event[i];//get value
				KBC_PMC_Event[i] = 0;//clear
			}
		}
		KBC_PMC_Event_Num = 0;//256 count	
	}
	/* read value form addr */
	else
	{
		switch(length)
		{
			case 1:
				for (int i=0 ; i<256 ; i++)
				{
					temp1 = (*((uint8_t*)(data_base)));
					Uart_data[i] = temp1 & 0xff;
					iic_data[i] = temp1 & 0xff;
					data_base ++;
				}
				break;
			
			case 2:
				for (int i=0 ; i<128 ; i++)
				{
					temp2 = (*((uint16_t*)(data_base)));
					Uart_data[i*2] = temp2 & 0xff;
					Uart_data[i*2+1] = (temp2 >> 8) & 0xff;
					iic_data[i*2] = temp2 & 0xff;
					iic_data[i*2+1] = (temp2 >> 8) & 0xff;
					data_base += 2;
				}
				break;

			case 4:
				for (int i=0 ; i<64 ; i++)
				{
					temp4 = (*((uint32_t*)(data_base)));
					Uart_data[i*4] = temp4 & 0xff;
					Uart_data[i*4+1] = (temp4 >> 8) & 0xff;
					Uart_data[i*4+2] = (temp4 >> 16) & 0xff;
					Uart_data[i*4+3] = (temp4 >> 24) & 0xff;
					iic_data[i*4] = temp4 & 0xff;
					iic_data[i*4+1] = (temp4 >> 8) & 0xff;
					iic_data[i*4+2] = (temp4 >> 16) & 0xff;
					iic_data[i*4+3] = (temp4 >> 24) & 0xff;
					data_base += 4;
				}
				break;

			default:
				for(int i=0 ; i<256 ; i++){
					Uart_data[i] = i;
					iic_data[i] = i;
				}
				break;
		}
	}			

#if I2CToUart_Uart
	/* Output from UART */
	for(int i=0;i<buf[3]+1;i++)
	{
		DEBUGER_putchar(Uart_data[i]);
	}
	
#else
	iicFeedback = 1;//enable the iic feedback
#endif
}

void Debugger_Send_KBD(void)
{
	if (!F_Service_KBL) return;
	if(iic_pos >= 256)return;
	pos = 0;
	while ((((kbd_event.head + 1) % KBD_BUFF_SIZE) != kbd_event.tail))
	{
		iicFeedback = 0;//disable the iic feedback
		kbd_event.head++;
		iic_data[iic_pos++] = kbd_event.data[kbd_event.head];
		Uart_data[pos++] = kbd_event.data[kbd_event.head];
		dprint("%x ", kbd_event.data[kbd_event.head]);
		if (kbd_event.head == KBD_BUFF_SIZE)
		{
			kbd_event.head = 0;
		}
		iicFeedback = 4; //enable the kbc send
	}
	#if I2CToUart_Uart
		dprint("\n");
		for(int i = 0; i < pos; i++)
		{
			DEBUGER_putchar(Uart_data[i]);
		}
	#endif
}

//*****************************************************************************
//
//  The Debugger Function for Data Write
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void Service_Change(void)
{
	if (F_Service_Change == 0)
	{
		return;
	}
	else if(F_Service_Change == 1)
	{
		F_Service_Change = 0;
	}
	
	/* Declaration of variables */
	int data_base;
	int baseaddr = 0x3800;
	int length = 0;
	
	/* Addr sequence transformation */
	data_base = (buf[0] << 16) | (buf[1] << 8) | buf[2];
	
	/* Register length conversion */
	if(data_base >= 0x10000 && data_base <= 0xBFFFF)
	{
		length = 4;
	}
	else if(data_base >= 0x4800 && data_base <= 0x4FFF)
	{
		length = 2;
	}
	else
	{
		length = 1;
	}
	
	/* write value to addr */
	switch(length)
	{
		case 1:
			*((uint8_t*)(data_base)) = buf[6];
			break;

		case 2:
			*((uint16_t*)(data_base)) = (buf[6] | (buf[5] << 8));
			break;

		case 4:
			*((uint32_t*)(data_base)) = (buf[6] | (buf[5] << 8) | (buf[4] << 16) | (buf[3] << 24));
			break;
	}

#if I2CToUart_Uart
	/* Output from UART */
	DEBUGER_putchar(0xff);
#else
	/* Output from I2C */
	/*Write 0xFF as a signal through I2C*/
	iicFeedback = 2;
	
#endif
}


//*****************************************************************************
//
//  The Debugger Function for Interrupt
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void Service_Debugger_Int(void)//Debugger Interrupt Function
{
	BYTE  value;
	BYTE  read_flag = 0;
	BYTE  illegal_operation = 0;
	if (F_Service_Debugger_Int == 0)
	{
		return;
	}
	else if(F_Service_Debugger_Int == 1)
	{
		F_Service_Debugger_Int = 0;
	}
	dprint("aaa\n");
if(dbg_int_buf[0] >= 0x0 && dbg_int_buf[0] <= 0x2){
	 int index = dbg_int_buf[0] - 0x0;
	switch(dbg_int_buf[1]){
				case 0x0:
					CPU_Int_Enable(index);
					break;
				case 0x1:
					CPU_Int_Disable(index);
					break;
				case 0x4:
					CPU_Int_Type_Edge(index);
					break;
				case 0x5:
					CPU_Int_Type_Level(index);
					break;
				case 0x8:
					CPU_Int_Enable_Read(index);
					break;
				case 0xA:
					CPU_Int_Type_Read(index);
					break;
				default:
					illegal_operation = 1;
					break;
	}
}
		if(dbg_int_buf[0] == 0x3){
			switch(dbg_int_buf[1]){
				case 0x0:
					SWUC_Int_Enable(dbg_int_buf[2]);
					break;
				case 0x1:
					SWUC_Int_Disable(dbg_int_buf[2]);
					break;
				case 0x8:
					value = SWUC_Int_Enable_Read(dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0xC:
					value = SWUC_Int_Status(dbg_int_buf[2]);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
	 	if(dbg_int_buf[0]== 0x4){
			switch(dbg_int_buf[1]){
				case 0x2:
					Spif_Int_Mask;
					break;
				case 0x3:
					Spif_Int_Unmask;
					break;
				case 0x9:
					value = Spif_Int_Mask_Read;
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if(dbg_int_buf[0] == 0x5){
			switch(dbg_int_buf[1]){
				case 0x0:
					GPIOB0_7_Int_Enable(4);
					break;
				case 0x1:
					GPIOB0_7_Int_Disable(4);
					break;
				case 0x2:
					GPIOB0_7_Int_Mask(4);
					break;
				case 0x3:
					GPIOB0_7_Int_Unmask(4);
					break;
				case 0x8:
					value = GPIOB0_7_Int_Enable_Read(4);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOB0_7_Int_Mask_Read(4);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOB0_7_Int_Status(4);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if(dbg_int_buf[0] ==  0x6){
			switch(dbg_int_buf[1]){
				case 0x0:
					Ps2_Mouse_Int_Enable;
					break;
				case 0x1:
					Ps2_Mouse_Int_Disable;
					break;
				case 0x8:
					value = Ps2_Mouse_Int_Enable_Read;
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if (dbg_int_buf[0] >= 0x7 && dbg_int_buf[0] <= 0x8){
			switch(dbg_int_buf[1]){
				case 0x0:
					KBD_Int_Enable;
					break;
				case 0x1:
					KBD_Int_Disable;
					break;
				case 0x8:
					value = KBD_Int_Enable_Read;
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if (dbg_int_buf[0] >= 0x9 && dbg_int_buf[0] <= 0xC){
			 int index = dbg_int_buf[0] - 0x9;
			switch(dbg_int_buf[1]){
				case 0x0:
					Tach_Int_Enable(index);
					break;
				case 0x1:
					Tach_Int_Disable(index);
					break;
				case 0x8:
					value = Tach_Int_Enable_Read(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
		}
		if(dbg_int_buf[0] >= 0xD && dbg_int_buf[0] <= 0xE){
		 int index = dbg_int_buf[0] - 0xD;
		switch(dbg_int_buf[1]){
				case 0x0:
					KBC_Int_Enable(3-index);
					break;
				case 0x1:
					KBC_Int_Disable(3-index);
					break;
				case 0x8:
					value = KBC_Int_Enable_Read(3-index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0xF && dbg_int_buf[0] <= 0x10){
		 int index = dbg_int_buf[0] - 0xF;
		switch(dbg_int_buf[1])
		{
				case 0x0:
					PMC1_Int_Enable(index);
					break;
				case 0x1:
					PMC1_Int_Disable(index);
					break;
				case 0x8:
					value = PMC1_Int_Enable_Read(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0x11 && dbg_int_buf[0] <= 0x12){
		 int index = dbg_int_buf[0] - 0x11;
		switch(dbg_int_buf[1]){
				case 0x0:
					PMC2_Int_Enable(index);
					break;
				case 0x1:
					PMC2_Int_Disable(index);
					break;
				case 0x8:
					value = PMC2_Int_Enable_Read(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
	if(dbg_int_buf[0] == 0x13){
			switch(dbg_int_buf[1]){
				case 0xC:
					value = Wdt_Int_Status;
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
	}
	if(dbg_int_buf[0] == 0x14){
			switch(dbg_int_buf[1]){
				case 0x2:
					ADC_Int_Mask(dbg_int_buf[2]);
					break;
				case 0x3:
					ADC_Int_Unmask(dbg_int_buf[2]);
					break;
				case 0x9:
					value = ADC_Int_Mask_Read(dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0xC:
					value = ADC_Int_Status(dbg_int_buf[2]);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if(dbg_int_buf[0] == 0x15){
			switch(dbg_int_buf[1]){
				case 0x0:
					Uart_Int_Enable(0,dbg_int_buf[2]);
					break;
				case 0x1:
					Uart_Int_Disable(0,dbg_int_buf[2]);
					break;
				case 0x8:
					value = Uart_Int_Enable_Read(0,dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0xC:
					value = Uart_Int_Status(0,dbg_int_buf[2]);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
	    if(dbg_int_buf[0] >= 0x16 && dbg_int_buf[0] <= 0x17){
		 int index = dbg_int_buf[0] - 0x16;
		switch(dbg_int_buf[1]){
				case 0x0:
					Uart_Int_Enable(4+index,dbg_int_buf[2]);
					break;
				case 0x1:
					Uart_Int_Disable(4+index,dbg_int_buf[2]);
					break;
				case 0x8:
					value = Uart_Int_Enable_Read(4+index,dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0xC:
					value = Uart_Int_Status(4+index,dbg_int_buf[2]);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
	}
		if(dbg_int_buf[0] ==  0x18){
			switch(dbg_int_buf[1]){
				case 0x0:
					SMSEC_Int_Enable(dbg_int_buf[1]);
					break;
				case 0x1:
					SMSEC_Int_Disable(dbg_int_buf[1]);
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}		
		if(dbg_int_buf[0] == 0x19){
			switch(dbg_int_buf[1]){
				case 0x0:
					SMSHOST_Int_Enable(dbg_int_buf[1]);
					break;
				case 0x1:
					SMSHOST_Int_Disable(dbg_int_buf[1]);
					break;
				default:
					illegal_operation = 1;
					break;
			}	
		}
		if (dbg_int_buf[0] >= 0x1A && dbg_int_buf[0] <= 0x1D){
				 int index = dbg_int_buf[0] - 0x1A;
				switch (dbg_int_buf[1]){
				case 0x0:
					Timer_Int_Enable(index);
					break;
				case 0x1:
					Timer_Int_Disable(index);
					break;
				case 0x8:
					value = Timer_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = Timer_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
		}
		if(dbg_int_buf[0] ==  0x1E){
			switch(dbg_int_buf[1])
			{
				case 0x0:
					Int_Control0_Enable(dbg_int_buf[2]);
					break;
				case 0x1:
					Int_Control0_Disable(dbg_int_buf[2]);
					break;
				case 0x2:
					Int_Control0_Mask(dbg_int_buf[2]);
					break;
				case 0x3:
					Int_Control0_Unmask(dbg_int_buf[2]);
					break;
				case 0x8:
					value = Int_Control0_Enable_Read(dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0x9:
					value = Int_Control0_Mask_Read(dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0xC:
					value = Int_Control0_Status(dbg_int_buf[2]);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if(dbg_int_buf[0] ==  0x1F){
			switch(dbg_int_buf[1]){
				case 0x0:
					Int_Control1_Enable(dbg_int_buf[2]);
					break;
				case 0x1:
					Int_Control1_Disable(dbg_int_buf[2]);
					break;
				case 0x2:
					Int_Control1_Mask(dbg_int_buf[2]);
					break;
				case 0x3:
					Int_Control1_Unmask(dbg_int_buf[2]);
					break;
				case 0x8:
					value = Int_Control1_Enable_Read(dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0x9:
					value = Int_Control1_Mask_Read(dbg_int_buf[2]);
					read_flag = 1;
					break;
				case 0xC:
					value = Int_Control1_Status(dbg_int_buf[2]);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
			}
		}
		if(dbg_int_buf[0] >= 0x20 && dbg_int_buf[0] <= 0x27){
		 int index = dbg_int_buf[0] - 0x20;
			switch (dbg_int_buf[1]){
 				case 0x0:
					GPIOA0_7_Int_Enable(index);
					break;
				case 0x1:
					GPIOA0_7_Int_Disable(index);
					break;
				case 0x2:
					GPIOA0_7_Int_Mask(index);
					break;
				case 0x3:
					GPIOA0_7_Int_Unmask(index);
					break;
				case 0x4:
					GPIOA0_7_Int_Type_Edge(index);
					break;
				case 0x5:
					GPIOA0_7_Int_Type_Level(index);
					break;
				case 0x6:
					GPIOA0_7_Int_Polarity_High(index);
					break;
				case 0x7:
					GPIOA0_7_Int_Polarity_Low(index);
					break;
				case 0x8:
					value = GPIOA0_7_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOA0_7_Int_Mask_Read(index);
					read_flag = 1;
					break;
				case 0xA:
					value = GPIOA0_7_Int_Type_Read(index);
					read_flag = 1;
					break;
				case 0xB:
					value = GPIOA0_7_Int_Polarity_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOA0_7_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0x28 && dbg_int_buf[0] <= 0x2F){
		 int index = dbg_int_buf[0] - 0x28;
		switch (dbg_int_buf[1]){
 				case 0x0:
					GPIOA8_15_Int_Enable(index);
					break;
				case 0x1:
					GPIOA8_15_Int_Disable(index);
					break;
				case 0x2:
					GPIOA8_15_Int_Mask(index);
					break;
				case 0x3:
					GPIOA8_15_Int_Unmask(index);
					break;
				case 0x4:
					GPIOA8_15_Int_Type_Edge(index);
					break;
				case 0x5:
					GPIOA8_15_Int_Type_Level(index);
					break;
				case 0x6:
					GPIOA8_15_Int_Polarity_High(index);
					break;
				case 0x7:
					GPIOA8_15_Int_Polarity_Low(index);
					break;
				case 0x8:
					value = GPIOA8_15_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOA8_15_Int_Mask_Read(index);
					read_flag = 1;
					break;
				case 0xA:
					value = GPIOA8_15_Int_Type_Read(index);
					read_flag = 1;
					break;
				case 0xB:
					value = GPIOA8_15_Int_Polarity_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOA8_15_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0x30 && dbg_int_buf[0] <= 0x37){
		 int index = dbg_int_buf[0] - 0x30;
		switch (dbg_int_buf[1]){
				case 0x0:
					GPIOA16_23_Int_Enable(index);
					break;
				case 0x1:
					GPIOA16_23_Int_Disable(index);
					break;
				case 0x2:
					GPIOA16_23_Int_Mask(index);
					break;
				case 0x3:
					GPIOA16_23_Int_Unmask(index);
					break;
				case 0x4:
					GPIOA16_23_Int_Type_Edge(index);
					break;
				case 0x5:
					GPIOA16_23_Int_Type_Level(index);
					break;
				case 0x6:
					GPIOA16_23_Int_Polarity_High(index);
					break;
				case 0x7:
					GPIOA16_23_Int_Polarity_Low(index);
					break;
				case 0x8:
					value = GPIOA16_23_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOA16_23_Int_Mask_Read(index);
					read_flag = 1;
					break;
				case 0xA:
					value = GPIOA16_23_Int_Type_Read(index);
					read_flag = 1;
					break;
				case 0xB:
					value = GPIOA16_23_Int_Polarity_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOA16_23_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0x38 && dbg_int_buf[0] <= 0x3F){
		 int index = dbg_int_buf[0] - 0x38;
		switch (dbg_int_buf[1]){
 				case 0x0:
					GPIOA24_31_Int_Enable(index);
					break;
				case 0x1:
					GPIOA24_31_Int_Disable(index);
					break;
				case 0x2:
					GPIOA24_31_Int_Mask(index);
					break;
				case 0x3:
					GPIOA24_31_Int_Unmask(index);
					break;
				case 0x4:
					GPIOA24_31_Int_Type_Edge(index);
					break;
				case 0x5:
					GPIOA24_31_Int_Type_Level(index);
					break;
				case 0x6:
					GPIOA24_31_Int_Polarity_High(index);
					break;
				case 0x7:
					GPIOA24_31_Int_Polarity_Low(index);
					break;
				case 0x8:
					value = GPIOA24_31_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOA24_31_Int_Mask_Read(index);
					read_flag = 1;
					break;
				case 0xA:
					value = GPIOA24_31_Int_Type_Read(index);
					read_flag = 1;
					break;
				case 0xB:
					value = GPIOA24_31_Int_Polarity_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOA24_31_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0x40 && dbg_int_buf[0] <= 0x47){
		 int index = dbg_int_buf[0] - 0x40;
		switch (dbg_int_buf[1]){
 				case 0x0:
					GPIOB0_7_Int_Enable(index);
					break;
				case 0x1:
					GPIOB0_7_Int_Disable(index);
					break;
				case 0x2:
					GPIOB0_7_Int_Mask(index);
					break;
				case 0x3:
					GPIOB0_7_Int_Unmask(index);
					break;
				case 0x4:
					GPIOB0_7_Int_Type_Edge(index);
					break;
				case 0x5:
					GPIOB0_7_Int_Type_Level(index);
					break;
				case 0x6:
					GPIOB0_7_Int_Polarity_High(index);
					break;
				case 0x7:
					GPIOB0_7_Int_Polarity_Low(index);
					break;
				case 0x8:
					value = GPIOB0_7_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOB0_7_Int_Mask_Read(index);
					read_flag = 1;
					break;
				case 0xA:
					value = GPIOB0_7_Int_Type_Read(index);
					read_flag = 1;
					break;
				case 0xB:
					value = GPIOB0_7_Int_Polarity_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOB0_7_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
		if(dbg_int_buf[0] >= 0x48 && dbg_int_buf[0] <= 0x4F){
		int index = dbg_int_buf[0] - 0x48;
		switch (dbg_int_buf[1]){
 				case 0x0:
					GPIOB8_15_Int_Enable(index);
					break;
				case 0x1:
					GPIOB8_15_Int_Disable(index);
					break;
				case 0x2:
					GPIOB8_15_Int_Mask(index);
					break;
				case 0x3:
					GPIOB8_15_Int_Unmask(index);
					break;
				case 0x4:
					GPIOB8_15_Int_Type_Edge(index);
					break;
				case 0x5:
					GPIOB8_15_Int_Type_Level(index);
					break;
				case 0x6:
					GPIOB8_15_Int_Polarity_High(index);
					break;
				case 0x7:
					GPIOB8_15_Int_Polarity_Low(index);
					break;
				case 0x8:
					value = GPIOB8_15_Int_Enable_Read(index);
					read_flag = 1;
					break;
				case 0x9:
					value = GPIOB8_15_Int_Mask_Read(index);
					read_flag = 1;
					break;
				case 0xA:
					value = GPIOB8_15_Int_Type_Read(index);
					read_flag = 1;
					break;
				case 0xB:
					value = GPIOB8_15_Int_Polarity_Read(index);
					read_flag = 1;
					break;
				case 0xC:
					value = GPIOB8_15_Int_Status(index);
					read_flag = 1;
					break;
				default:
					illegal_operation = 1;
					break;
		}
	}
//sws----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if (dbg_int_buf[0] >= 0x50 && dbg_int_buf[0] <= 0x53)
	{
		int index = dbg_int_buf[0] - 0x50;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOB16_19_Int_Enable(index);
			break;
		case 0x1:
			GPIOB16_19_Int_Disable(index);
			break;
		case 0x2:
			GPIOB16_19_Int_Mask(index);
			break;
		case 0x3:
			GPIOB16_19_Int_Unmask(index);
			break;
		case 0x4:
			GPIOB16_19_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOB16_19_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOB16_19_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOB16_19_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOB16_19_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOB16_19_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOB16_19_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOB16_19_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOB16_19_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

	if (dbg_int_buf[0] >= 0x54 && dbg_int_buf[0] <= 0x5A)
	{
		 int index = dbg_int_buf[0] - 0x54;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOB20_26_Int_Enable(index);
			break;
		case 0x1:
			GPIOB20_26_Int_Disable(index);
			break;
		case 0x2:
			GPIOB20_26_Int_Mask(index);
			break;
		case 0x3:
			GPIOB20_26_Int_Unmask(index);
			break;
		case 0x4:
			GPIOB20_26_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOB20_26_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOB20_26_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOB20_26_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOB20_26_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOB20_26_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOB20_26_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOB20_26_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOB20_26_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}


	if (dbg_int_buf[0] >= 0x5B && dbg_int_buf[0] <= 0x62)
	{
		 int index = dbg_int_buf[0] - 0x5B;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOC0_7_Int_Enable(index);
			break;
		case 0x1:
			GPIOC0_7_Int_Disable(index);
			break;
		case 0x2:
			GPIOC0_7_Int_Mask(index);
			break;
		case 0x3:
			GPIOC0_7_Int_Unmask(index);
			break;
		case 0x4:
			GPIOC0_7_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOC0_7_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOC0_7_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOC0_7_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOC0_7_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOC0_7_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOC0_7_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOC0_7_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOC0_7_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}


		if (dbg_int_buf[0] >= 0x63 && dbg_int_buf[0] <= 0x68)
	{
		 int index = dbg_int_buf[0] - 0x63;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOC8_13_Int_Enable(index);
			break;
		case 0x1:
			GPIOC8_13_Int_Disable(index);
			break;
		case 0x2:
			GPIOC8_13_Int_Mask(index);
			break;
		case 0x3:
			GPIOC8_13_Int_Unmask(index);
			break;
		case 0x4:
			GPIOC8_13_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOC8_13_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOC8_13_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOC8_13_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOC8_13_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOC8_13_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOC8_13_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOC8_13_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOC8_13_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

		if (dbg_int_buf[0] >= 0x69 && dbg_int_buf[0] <= 0x70)
	{
		int index = dbg_int_buf[0] - 0x69;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOE0_7_Int_Enable(index);
			break;
		case 0x1:
			GPIOE0_7_Int_Disable(index);
			break;
		case 0x2:
			GPIOE0_7_Int_Mask(index);
			break;
		case 0x3:
			GPIOE0_7_Int_Unmask(index);
			break;
		case 0x4:
			GPIOE0_7_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOE0_7_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOE0_7_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOE0_7_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOE0_7_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOE0_7_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOE0_7_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOE0_7_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOE0_7_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

		if (dbg_int_buf[0] >= 0x71 && dbg_int_buf[0] <= 0x78)
	{
		 int index = dbg_int_buf[0] - 0x71;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOE8_15_Int_Enable(index);
			break;
		case 0x1:
			GPIOE8_15_Int_Disable(index);
			break;
		case 0x2:
			GPIOE8_15_Int_Mask(index);
			break;
		case 0x3:
			GPIOE8_15_Int_Unmask(index);
			break;
		case 0x4:
			GPIOE8_15_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOE8_15_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOE8_15_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOE8_15_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOE8_15_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOE8_15_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOE8_15_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOE8_15_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOE8_15_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
		if (dbg_int_buf[0] >= 0x79 && dbg_int_buf[0] <= 0x80)
	{
		 int index = dbg_int_buf[0] - 0x79;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			GPIOE16_23_Int_Enable(index);
			break;
		case 0x1:
			GPIOE16_23_Int_Disable(index);
			break;
		case 0x2:
			GPIOE16_23_Int_Mask(index);
			break;
		case 0x3:
			GPIOE16_23_Int_Unmask(index);
			break;
		case 0x4:
			GPIOE16_23_Int_Type_Edge(index);
			break;
		case 0x5:
			GPIOE16_23_Int_Type_Level(index);
			break;
		case 0x6:
			GPIOE16_23_Int_Polarity_High(index);
			break;
		case 0x7:
			GPIOE16_23_Int_Polarity_Low(index);
			break;
		case 0x8:
			value = GPIOE16_23_Int_Enable_Read(index);
			read_flag = 1;
			break;
		case 0x9:
			value = GPIOE16_23_Int_Mask_Read(index);
			read_flag = 1;
			break;
		case 0xA:
			value = GPIOE16_23_Int_Type_Read(index);
			read_flag = 1;
			break;
		case 0xB:
			value = GPIOE16_23_Int_Polarity_Read(index);
			read_flag = 1;
			break;
		case 0xC:
			value = GPIOE16_23_Int_Status(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

	if (dbg_int_buf[0] >= 0x81 && dbg_int_buf[0] <= 0x82)
	{
		 int index = dbg_int_buf[0] - 0x81;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			PMC3_Int_Enable(index);
			break;
		case 0x1:
			PMC3_Int_Disable(index);
			break;
		case 0x8:
			value = PMC3_Int_Enable_Read(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
		if (dbg_int_buf[0] >= 0x83 && dbg_int_buf[0] <= 0x84)
	{
		 int index = dbg_int_buf[0] - 0x83;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			PMC4_Int_Enable(index);
			break;
		case 0x1:
			PMC4_Int_Disable(index);
			break;
		case 0x8:
			value = PMC4_Int_Enable_Read(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
		if (dbg_int_buf[0] >= 0x85 && dbg_int_buf[0] <= 0x86)
	{
		 int index = dbg_int_buf[0] - 0x85;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			PMC5_Int_Enable(index);
			break;
		case 0x1:
			PMC5_Int_Disable(index);
			break;
		case 0x8:
			value = PMC5_Int_Enable_Read(index);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
		if (dbg_int_buf[0] == 0x87)
	{
		switch (dbg_int_buf[1])
		{
		case 0x0:
			Ps2_KBD_Int_Enable;
			break;
		case 0x1:
			Ps2_KBD_Int_Disable;
			break;
		case 0x8:
			value = Ps2_KBD_Int_Enable_Read;
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

		if (dbg_int_buf[0] >= 0x88 && dbg_int_buf[0] <= 0x8A)
	{
		 int index = dbg_int_buf[0] - 0x87;
		switch (dbg_int_buf[1])
		{
		case 0x0:
			Uart_Int_Enable(index, dbg_int_buf[2]);
			break;
		case 0x1:
			Uart_Int_Disable(index, dbg_int_buf[2]);
			break;
		case 0x8:
			value = Uart_Int_Enable_Read(index, dbg_int_buf[2]);
			read_flag = 1;
			break;
		case 0xC:
			value = Uart_Int_Status(index, dbg_int_buf[2]);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
		if (dbg_int_buf[0] == 0x8B)
	{
		switch (dbg_int_buf[1])
		{
		case 0x2:
			Spim_Int_Mask(dbg_int_buf[2]);
			break;
		case 0x3:
			Spim_Int_Unmask(dbg_int_buf[2]);
			break;
		case 0x9:
			value = Spim_Int_Mask_Read(dbg_int_buf[2]);
			read_flag = 1;
			break;
		case 0xC:
			value = Spim_Int_Status(dbg_int_buf[2]);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

		if (dbg_int_buf[0] >= 0x8C && dbg_int_buf[0] <= 0x8F)
	{
		 int index = dbg_int_buf[0] - 0x8C;
		switch (dbg_int_buf[1])
		{
		case 0x2:
			I2c_Int_Mask(index, dbg_int_buf[2]);
			break;
		case 0x3:
			I2c_Int_Unmask(index, dbg_int_buf[2]);
			break;
		case 0x9:
			value = I2c_Int_Mask_Read(index, dbg_int_buf[2]);
			read_flag = 1;
			break;
		case 0xC:
			value = I2c_Int_Status(index, dbg_int_buf[2]);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

	if (dbg_int_buf[0] >= 0x90 && dbg_int_buf[0] <= 0x93)
	{
		 int index = dbg_int_buf[0] - 0x90;
		switch (dbg_int_buf[1])
		{
		case 0x0:
#if TEST
			Can_Int_Enable(index, dbg_int_buf[2]);
#endif
			break;
		case 0x1:
#if TEST
			Can_Int_Disable(index, dbg_int_buf[2]);
#endif
			break;
		case 0x8:
#if TEST
			value = Can_Int_Enable_Read(index, dbg_int_buf[2]);
			read_flag = 1;
#endif
			break;
		case 0xC:
#if TEST
			value = Can_Int_Status(index, dbg_int_buf[2]);
			read_flag = 1;
#endif
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

	if (dbg_int_buf[0] == 0x94)
	{
		switch (dbg_int_buf[1])
		{
		case 0:
			Pport_Int_Enable;
			break;
		case 0x1:
			Pport_Int_Disable;
			break;
		case 0x8:
			value = Pport_Int_Enable_Read;
			read_flag = 1;
			break;
		case 0xC:
			value = Pport_Int_Status;
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}

		if (dbg_int_buf[0] == 0x95)
	{
		switch (dbg_int_buf[1])
		{
		case 0x0:
			Por_Int_Enable(dbg_int_buf[2]);
			break;
		case 0x1:
			Por_Int_Disable(dbg_int_buf[2]);
			break;
		case 0x8:
			value = Por_Int_Enable_Read(dbg_int_buf[2]);
			read_flag = 1;
			break;
		case 0xC:
			value = Por_Int_Status(dbg_int_buf[2]);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
		if (dbg_int_buf[0] == 0x96)
	{
		switch (dbg_int_buf[1])
		{
		case 0x2:
			Mon_Int_Mask(dbg_int_buf[2]);
			break;
		case 0x3:
			Mon_Int_Unmask(dbg_int_buf[2]);
			break;
		case 0x9:
			value = Mon_Int_Mask_Read(dbg_int_buf[2]);
			read_flag = 1;
			break;
		case 0xC:
			value = Mon_Int_Mask_Read(dbg_int_buf[2]);
			read_flag = 1;
			break;
		default:
			illegal_operation = 1;
			break;
		}
	}
	if (!(dbg_int_buf[0] >= 0x0 && dbg_int_buf[0] <= 0x96)){
		illegal_operation = 1;
	}

	if(0 == illegal_operation)
	{
		if(1 == read_flag)
		{
			dprint("send back read value \n");
			if(value != 0)
			{
				#if I2CToUart_Uart
					/* Output from UART */
					DEBUGER_putchar(0x01);
				#else
					/* Output from I2C */
					iicFeedback = 3;
					iic_int_flag = 1;
				#endif
				return;
			}
			else
			{
				#if I2CToUart_Uart
					/* Output from UART */
					DEBUGER_putchar(0x00);
				#else
					/* Output from I2C */
					iicFeedback = 3;
					iic_int_flag =2;
				#endif
				return;
			}
		}
		else
		{
			#if I2CToUart_Uart
				/* Output from UART */
				dprint("send back 0x11 \n");
				DEBUGER_putchar(0x11);
			#else
				/* Output from I2C */
				iicFeedback = 3;
				iic_int_flag = 3;
			#endif
			return;
		}
	}
	else
	{
			#if I2CToUart_Uart
				/* Output from UART */
				dprint("send back 0xDD \n");
				DEBUGER_putchar(0xDD);
			#else
				iicFeedback = 3;
				iic_int_flag = 4;
			#endif
			return;
	}
}

//*****************************************************************************
//
//  The Debugger Function for record KBC or PMC
//
//  parameter :
//		direction :  0-input;1-output
//		channel :  0-KBC;1-PMC1;2-PMC2;3-PMC3;4-PMC4
//      value : value need to be record
//
//  return :
//      Function running condition
//
//*****************************************************************************
BYTE  Debugger_KBC_PMC_Record(BYTE  direction, BYTE  channel, BYTE  value)
{
	if(KBC_PMC_Flag)
	{
		/*first  0-input;1-output*/
		/*second 0-KBC;1-PMC1;2-PMC2;3-PMC3;4-PMC4*/
		if(KBC_PMC_Event_Num == 256)
		{
			KBC_PMC_Event_Num=0;//clear index
			// the entire array will be stored in the pending array
			KBC_PMC_DataPending(KBC_PMC_Event);
		}
		KBC_PMC_Event[KBC_PMC_Event_Num] = (direction)<<4|(channel);//record direction and channel
		KBC_PMC_Event_Num ++;
		KBC_PMC_Event[KBC_PMC_Event_Num] = value;//record value
		KBC_PMC_Event_Num ++;
	}
	return 0;
}



//#else //when chip set as debugger master
/*------------Only the forwarding function is available-------------*/

//*****************************************************************************
//
//  Debugger Master read in byte
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char Debugger_Master_Read_Byte(WORD i2c_channel)
{
	char data;	
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	if(0 == I2c_Check_TFNF(i2c_channel))
	{
		I2c_Writeb(I2C_READ|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		data = I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xff;
	}
	return data ;
}
//*****************************************************************************
//
//  Debugger Master write in byte
//
//  parameter :
//		data : need to be write
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
void Debugger_Master_Write_Byte(char data, WORD i2c_channel)
{	
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	if(0 == I2c_Check_TFNF(i2c_channel))
	{
		I2c_Writeb(data,I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}
}

#if I2CToUart_Uart
//*****************************************************************************
//
//  Debugger I2c Connect
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Send to Slave for Connect Notification 
//*****************************************************************************
void I2c_Connect(void)
{
	Debugger_Master_Write_Byte(0xDD,0);
	BYTE  data = Debugger_Master_Read_Byte(0);
	if(data == 0xDD)
	{
		DEBUGER_putchar(0xDD);
	}
}

//*****************************************************************************
//
//  Debugger I2c Disconnect
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Send to Slave for Disconnect Notification 
//*****************************************************************************
void I2c_Disconnect(void)
{
	BYTE  data;
	Debugger_Master_Write_Byte(0x88,0);
	data = Debugger_Master_Read_Byte(0)&0xFF;
	if(data == 0x88)
	{
      	DEBUGER_putchar(0x88);
	}
	
}

//*****************************************************************************
//
//  Debugger Master Read Block
//
//  parameter :
//      buf[] : addr array
//		baseaddr : I2c channel
//
//  return :
//      none
//
//	note :
//		Send Addr to Slave and Read Data Back
//*****************************************************************************
void Debugger_Master_Read(char buf[],DWORD baseaddr)
{
	int i,j;
	int data[256];

	if(0 == I2c_Check_TFE(baseaddr))
	{
		I2c_Writeb(0x55,I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
	}
	if(0 == I2c_Check_TFNF(baseaddr))
	{
		I2c_Writeb(buf[0],I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
	}
	if(0 == I2c_Check_TFNF(baseaddr))
	{
		I2c_Writeb(buf[1],I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
	}

	if(0 == I2c_Check_TFNF(baseaddr))
	{
		I2c_Writeb(buf[2],I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}
	
	for (j=0;j<32;j++)
	{
		for(i = 0;i < 7 ;i ++)
		{
			if(0 == I2c_Check_TFNF(baseaddr))
			{
				I2c_Writeb(I2C_READ,I2C_DATA_CMD_RWDIR,baseaddr);	
			}
 		}
		if(j == 31)
		{
			if(0 == I2c_Check_TFNF(baseaddr))
			{
				I2c_Writeb(I2C_READ|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);	
			}
		}
		else
		{
			if(0 == I2c_Check_TFNF(baseaddr))
			{
				I2c_Writeb(I2C_READ,I2C_DATA_CMD_RWDIR,baseaddr);	
			}
		}	

		for( i =0 ; i < 8; i++)
		{
			if(0 == I2c_Check_RFNE(baseaddr))
			{
				data[i+j*8] = I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xff;
			}
		}
	}

	for (j = 0;j < buf[3]+1;j ++)
	{
      	DEBUGER_putchar(data[j]);
	}	
}

//*****************************************************************************
//
//  Debugger Write Read Block
//
//  parameter :
//      buf[] : addr array
//		baseaddr : I2c channel
//
//  return :
//      none
//
//	note :
//		Send Addr and Data to Slave
//*****************************************************************************
void Debugger_Master_Write(char buf[],DWORD baseaddr)
{
	int get_data;
	int i;

	if(0 == I2c_Check_TFNF(baseaddr))
	{
		I2c_Writeb(0xFF,I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
	}
	for(i=0 ; i<6 ; i++)
	{
		if(0 == I2c_Check_TFNF(baseaddr))
		{
			I2c_Writeb(buf[i],I2C_DATA_CMD_OFFSET,baseaddr);
			I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
		}
	}
	if(0 == I2c_Check_TFNF(baseaddr))
	{
		I2c_Writeb(buf[i],I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}

	if(0 == I2c_Check_TFNF(baseaddr))
	{
		I2c_Writeb(I2C_WRITE|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);	
	}

	if(0 == I2c_Check_RFNE(baseaddr))
	{
		get_data = I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xFF;
	}
    
	DEBUGER_putchar(get_data);
}

//*****************************************************************************
//
//  Debugger Master Data Forwarding Function
//
//  parameter :
//      none
//
//  return :
//      none
//
//	note :
//		Use This Chip as I2c Master to Forward Data to Target Slave Chip
//*****************************************************************************
void Debugger_Master_Retrans(BYTE debuger)
{
	#if !(SMBUS1_CLOCK_EN)
        dprint("SMBUS1 CLOCK NOT ENABLE\n");
        return;
    #endif

	uart_data = debuger & 0xff;
	dprint("uartdata = 0x%x\n",uart_data);
	if ((!Buf_Flag) && (uart_data == 0x55 || uart_data == 0xff || uart_data == 0xDD || uart_data == 0x88 || uart_data == 0xAA || uart_data == 0xBB))
	{
		switch (uart_data)
		{
			case 0x88:
				I2c_Disconnect();
				break;

			case 0xDD:
				I2c_Connect();
				break;

			case 0xAA:
      			DEBUGER_putchar(0xAA);
				Debugger_Master_Write_Byte(0xAA,0);
				break;

			case 0xBB:
      			DEBUGER_putchar(0xBB);
				Debugger_Master_Write_Byte(0xBB,0);
				break;
			
			case 0xE0://reset chip
      			DEBUGER_putchar(0xE0);
				Debugger_Master_Write_Byte(0xE0,0);
				break;

			case 0xB0://change keyboard code type
      			DEBUGER_putchar(0xB0);
				Debugger_Master_Write_Byte(0xB0,0);
				break;

			default:
				Buf_Num = 0;
				Buf_Flag = 1;
				Debug_Temp = uart_data;
				return;
		}
	}
	if(Buf_Flag)
	{
		switch (Debug_Temp)
		{
			case 0x55:
				buf[Buf_Num] = uart_data;
				Buf_Num ++;
				if(Buf_Num == 4)
				{
					Buf_Flag = 0;
					Debug_Temp = 0;
					Debugger_Master_Read(buf,0);
				}
				break;

			case 0xFF:
				buf[Buf_Num] = uart_data;
				Buf_Num ++;
				if(Buf_Num == 7)
				{
					Buf_Flag = 0;
					Debug_Temp = 0;
					Debugger_Master_Write(buf,0);
				}
				break;

			default :
				break;
		}
	}
}
#endif
#endif
