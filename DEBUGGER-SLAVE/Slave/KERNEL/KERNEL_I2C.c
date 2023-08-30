/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-12 11:17:19
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
#include "KERNEL_I2C.H"
//
//*****************************************************************************

//*****************************************************************************
//
//  I2C channel number and base address conversion function
//
//  parameter :
//      I2C channel number
//
//  return :
//      I2C channel baseaddr
//
//*****************************************************************************
WORD I2c_Channel_Baseaddr(WORD i2c_channel)
{
	switch(i2c_channel)
	{
		case I2C_CHANNEL_0:
			return SMBUS0_BASE_ADDR;
		case I2C_CHANNEL_1:
			return SMBUS1_BASE_ADDR;
		case I2C_CHANNEL_2:
			return SMBUS2_BASE_ADDR;
		case I2C_CHANNEL_3:
			return SMBUS3_BASE_ADDR;
		#ifdef AE103
		case I2C_CHANNEL_4:
			return SMBUS4_BASE_ADDR;
		case I2C_CHANNEL_5:
			return SMBUS5_BASE_ADDR;
		#endif

		case SMBUS0_BASE_ADDR:
			return SMBUS0_BASE_ADDR;
		case SMBUS1_BASE_ADDR:
			return SMBUS1_BASE_ADDR;
		case SMBUS2_BASE_ADDR:
			return SMBUS2_BASE_ADDR;
		case SMBUS3_BASE_ADDR:
			return SMBUS3_BASE_ADDR;
		#ifdef AE103
		case SMBUS4_BASE_ADDR:
			return SMBUS4_BASE_ADDR;
		case SMBUS5_BASE_ADDR:
			return SMBUS5_BASE_ADDR;
		#endif			
		default:
			dprint("channel error \n");
			return 0;
	}
}

//*****************************************************************************
//
//  I2C register read by byte function
//
//  parameter :
//      regoffset : Offset register
//		baseaddr : baseaddr of I2C channel
//
//  return :
//      read value
//
//*****************************************************************************
char I2c_Readb(WORD regoffset,WORD i2c_channel)
{
	char reval = 0;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	reval = REG8(REG_ADDR(baseaddr,regoffset));
	return reval;
}


//*****************************************************************************
//
//  I2C register write by byte function
//
//  parameter :
//      value : value need to be write by byte
//      regoffset : Offset register
//		baseaddr : baseaddr of I2C channel
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Writeb(char value,WORD regoffset,WORD i2c_channel)
{
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	REG8(REG_ADDR(baseaddr,regoffset)) = value;
	return;
}
//*****************************************************************************
//
//  I2C Mux channel map 
//
//  parameter :
//      value : the channle of smbus0 mapping
//		0: smbus0 -> sda0/scl0   1: smbus0 -> sda1/scl1  .....
//  return :
//      none
//
//*****************************************************************************
void I2c_Mux_set(BYTE model)
{
	#if (defined(AE101)||defined(AE102))
    	SYSCTL_RESERVED &= ~(3<<9);//I2C_Mux: bit9~10  clear default: model: 0
		SYSCTL_RESERVED |= (model<<9); 
	#elif (defined(AE103))
    	SYSCTL_RESERVED &= ~(7<<16);//I2C_Mux: bit9~10  clear default: model: 0
		SYSCTL_RESERVED |= (model<<16); 
	#endif
	return;
}

//*****************************************************************************
//
//  Hcnt calculation.
//
//  parameter :
//      
//
//  return :
//      
//
//*****************************************************************************
DWORD I2c_Scl_Hcnt(DWORD ic_clk,DWORD tSYMBOL,DWORD tf,int cond,int offset)
{
	if(cond)
		return (ic_clk * tSYMBOL + 5000) / 10000  - 8 + offset;
	else
		return (ic_clk * (tSYMBOL + tf) + 5000) /10000 -3 + offset;
}

//*****************************************************************************
//
//  Lcnt calculation
//
//  parameter :
//      
//
//  return :
//      
//
//*****************************************************************************
DWORD H2c_Scl_Lcnt(DWORD  ic_clk, DWORD tLOW,DWORD tf,int offset)
{
	return ((ic_clk * (tLOW + tf) + 5000) /10000) -1 + offset;
}


//*****************************************************************************
//
//  I2C Sets the Slave address of the target device
//
//  parameter :
//      TAR : Slave address of the target device
//      the TAR is 7bit(default)
//  return :
//      none
//
//*****************************************************************************
void I2c_Master_Set_Tar(WORD TAR, WORD i2c_channel)
{
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);//Disable before setup
	I2c_Writeb(TAR,I2C_TAR_OFFSET,baseaddr);//Set TAR
	/*Added i2c 7bit register address operation*/
	I2c_Writeb(I2C_REGADDR_7BIT,I2C_REGADDR,baseaddr);
	I2c_Writeb(1,I2C_ENABLE_OFFSET,baseaddr);
}

//*****************************************************************************
//
//  I2C Sets the Slave address of the device
//
//  parameter :
//      TAR : Slave address of the device
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Slave_Set_Sar(WORD SAR, WORD i2c_channel)
{
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);//Disable before setup
	I2c_Writeb(SAR,I2C_SAR_OFFSET,baseaddr);//Set SAR
	I2c_Writeb(1,I2C_ENABLE_OFFSET,baseaddr);
}

//*****************************************************************************
//
//  Check I2C Send FIFO almost empty
//	The function will wait until the Send FIFO  less than TX_TL
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Check_TXE(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	//the loop will out while the txFIFO less TX_TL
	while(!(I2c_Readb(I2C_RAW_INTR_STAT_OFFSET,baseaddr) & TX_EMPTY))
	{
		--loop;
		if(0 == loop)
		{
			#if I2C_DEBUG
				dprint("i2c TXE error \n");
			#endif
			return -1;//return error
		}
	}
	return 0;
}

//*****************************************************************************
//
//  Check I2C Recieve FIFO almost full
//	The function will wait until the Rx FIFO more than RX_TL
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Check_RXF(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	//loop will out while RX more than RX_TL
	while(!(I2c_Readb(I2C_RAW_INTR_STAT_OFFSET,baseaddr) & RX_FULL))
	{
		--loop;
		if(0 == loop)
		{
			#if I2C_DEBUG
				dprint("i2c RXF error \n");
			#endif
			return -1;//return error
		}
	}
	return 0;
}

//*****************************************************************************
//
//  Check I2C Send FIFO empty
//	The function will wait until Send FIFO is empty
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Check_TFE(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	while(!(I2c_Readb(I2C_STATUS_OFFSET,baseaddr) & I2C_STATUS_TFE))//loop wait while Send FIFO not empty
	{
		--loop;
		if(0 == loop)
		{
			#if I2C_DEBUG
				dprint("i2c TFE error \n");
			#endif
			return -1;//return error
		}
	}
	return 0;
}

//*****************************************************************************
//
//  Check I2C Send FIFO full
//	The function will wait until Send FIFO is not full
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Check_TFNF(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	while(!(I2c_Readb(I2C_STATUS_OFFSET,baseaddr) & I2C_STATUS_TFNF))//loop wait while Send FIFO full
	{
		--loop;
		if(0 == loop)
		{
			#if I2C_DEBUG
				dprint("i2c TFNF error \n");
			#endif
			return -1;//return error
		}
	}
	return 0;
}

//*****************************************************************************
//
//  Check I2C Recieved FIFO empty
//	The function will wait until Recieved FIFO is not empty
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Check_RFNE(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	while(!(I2c_Readb(I2C_STATUS_OFFSET,baseaddr) & I2C_STATUS_RFNE))//loop wait while receive FIFO empty
	{
		--loop;
		if(0 == loop)
		{
			#if I2C_DEBUG
				dprint("i2c RFNE error \n");
			#endif
			return -1;//return error
		}
	}
	return 0;
}

//*****************************************************************************
//
//  Check I2C Stop signal
//	The function will wait until I2C send Stop
//	Timeout
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Check_Stop_Det(WORD i2c_channel)
{
	DWORD loop = I2CTimeOut;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	while(!(I2c_Readb(I2C_RAW_INTR_STAT_H, i2c_channel) & 0x2));
	{
		--loop;
		if(0 == loop)
		{
			#if I2C_DEBUG
				dprint("i2c wait stop error \n");
			#endif
			return -1;//return error
		}
	}
	return 0;

}

//*****************************************************************************
//
//  I2C writes in byte
//
//  parameter :
//		data : need to be write
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Master_Write_Byte(char data, WORD i2c_channel)
{	
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);

	/*write data and stop*/
	if(0 == I2c_Check_TFNF(i2c_channel))
	{
		I2c_Writeb(data,I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}
}

//*****************************************************************************
//
//  I2C read in byte
//
//  parameter :
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Master_Read_Byte(WORD i2c_channel)
{
	char data;	
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	//send the addr cmd to slave
	if(0 == I2c_Check_TFNF(i2c_channel))
	{
		I2c_Writeb(I2C_READ|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}
	//read the data from slave
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		data = I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xff;
	}
	return data ;
}

//*****************************************************************************
//
//  I2C writes in block
//
//  parameter :
//		*data : write data buffer
//		lenth : write length 
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      
// 
//*****************************************************************************
void I2c_Master_Write_Block(char *data, char  length, WORD i2c_channel)
{
	WORD i = 0;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	/*write data without stop*/
	while(i<=(length-2))
	{  
		if(0 == I2c_Check_TXE(i2c_channel))
		{
			I2c_Writeb((*(data + i)),I2C_DATA_CMD_OFFSET,baseaddr);
			I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
			i++;
		}
	}
	/*write data and stop*/
	if(0 == I2c_Check_TXE(i2c_channel))
	{
		I2c_Writeb((*(data + i)),I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	}
}

//*****************************************************************************
//
//  I2C read in block
//
//  parameter :
//		*data : read data buffer
//		lenth : read length
//		reg : register offset
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//  
//
//*****************************************************************************
void I2c_Master_Read_Block(char *data, BYTE  length, WORD i2c_channel)
{
	WORD i = 0;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);

	/*read without stop*/
	while(i<=(length-2))
	{
		I2c_Writeb(I2C_READ,I2C_DATA_CMD_RWDIR,baseaddr);
		if(0 == I2c_Check_RFNE(i2c_channel))
		{
			*(data + i) = I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xFF;
			dprint("i = %d, data = 0x%x \n",i,*(data + i));
		}
		i++;
	}
	/*read and stop*/
	I2c_Writeb(I2C_READ|I2C_STOP,I2C_DATA_CMD_RWDIR,baseaddr);
	if(0 == I2c_Check_RFNE(i2c_channel))
	{
		*(data + i) = I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xFF;
		dprint("i = %d, data = 0x%x \n",i,*(data + i));
	}
}

//*****************************************************************************
//
//  I2C slave writes in byte
//
//  parameter :
//		data : data to write
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Slave_Write_Byte(char data, WORD i2c_channel)
{	
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	/*write data and stop*/
	if(0 == I2c_Check_TFNF(i2c_channel))
	{
		I2c_Writeb(data,I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
	}
}

//*****************************************************************************
//
//  I2C slave read in byte
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
char I2c_Slave_Read_Byte(WORD i2c_channel)
{	
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	/*write data and stop*/
	if(0 == I2c_Check_RFNE(baseaddr))
	{
		return (I2c_Readb(I2C_DATA_CMD_OFFSET,baseaddr) & 0xff);
	}
}

//*****************************************************************************
//
//  I2C slave write in block
//
//  parameter :
//		*data : write data buffer
//		lenth : write length
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Slave_Write_Block(char *data, BYTE  length, WORD i2c_channel)
{	
	BYTE  i = 0;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	/*write data*/
	while(i<=(length-1))
	if(0 == I2c_Check_TFNF(i2c_channel))
	{
		I2c_Writeb(*(data + i),I2C_DATA_CMD_OFFSET,baseaddr);
		I2c_Writeb(I2C_WRITE,I2C_DATA_CMD_RWDIR,baseaddr);
		i++;
	}
}


//*****************************************************************************
//
//  I2C init as Master
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Master_Controller_Init(WORD i2c_channel)
{
	char hcnt,lcnt;   
	char ic_con,ic_tar;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	/*Disable*/
	I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	
	/* SET HCNT & LCNT*/
	#if I2C_STANDARD
		//standard mode
		hcnt = 0xF0;//100KHZ
		lcnt = 0xF0;
		I2c_Writeb(hcnt, I2C_SS_SCL_HCNT_OFFSET,baseaddr);
		I2c_Writeb(lcnt, I2C_SS_SCL_LCNT_OFFSET,baseaddr);
	#else
		//fast mode
		hcnt = 0x78;//200KHZ
		lcnt = 0x78;
		I2c_Writeb(hcnt, I2C_FS_SCL_HCNT_OFFSET,baseaddr);
		I2c_Writeb(lcnt, I2C_FS_SCL_LCNT_OFFSET,baseaddr);
	#endif
	
	/* SET I2C_CON*/	
	ic_con = I2c_Readb(I2C_CON_OFFSET,baseaddr);
	#if I2C_STANDARD
		ic_con &= ~ (3 << 1);//clear speed mode set
		ic_con |= I2C_CON_SPEED_STD; //speed mode set
	#else
		ic_con &= ~ (3 << 1);
		ic_con |= I2C_CON_SPEED_FAST;
	#endif
	ic_con |= I2C_CON_SLAVE_DISABLE;//disable slave
	ic_con |= I2C_CON_MASTER;//set master
	ic_con &= ~ I2C_CON_RESTART_EN;//disable restart
	ic_con &= ~I2C_CON_10BITADDR_MASTER;//7 bit ADDR

	I2c_Writeb(ic_con,I2C_CON_OFFSET,baseaddr);

	/* Set threshold*/
	I2c_Writeb(4,I2C_RX_TL_OFFSET,baseaddr);
	I2c_Writeb(4,I2C_TX_TL_OFFSET,baseaddr);
	
	/* Mask Interrupt */
	I2c_Writeb(0x0,I2C_INTR_MASK_OFFSET,baseaddr);
	
	/* Enable */
	I2c_Writeb(1,I2C_ENABLE_OFFSET,baseaddr);
}

//*****************************************************************************
//
//  I2C init as Slave
//
//  parameter :
//      i2c_channel : I2C channel number or baseaddr
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Slave_debugger_Init(WORD i2c_channel)
{
	char hcnt,lcnt;   
	char ic_con,ic_tar;
	WORD baseaddr = I2c_Channel_Baseaddr(i2c_channel);
	
	/*Disable*/
	I2c_Writeb(0, I2C_ENABLE_OFFSET,baseaddr);
	
	/* SET HCNT & LCNT*/
	#if I2C_STANDARD
		//standard mode
		hcnt = 0xF0;//100KHZ
		lcnt = 0xF0;
		I2c_Writeb(hcnt, I2C_SS_SCL_HCNT_OFFSET,baseaddr);
		I2c_Writeb(lcnt, I2C_SS_SCL_LCNT_OFFSET,baseaddr);
	#else
		//fast mode
		hcnt = 0x3c;//400KHZ
		lcnt = 0x3c;
		I2c_Writeb(hcnt, I2C_FS_SCL_HCNT_OFFSET,baseaddr);
		I2c_Writeb(lcnt, I2C_FS_SCL_LCNT_OFFSET,baseaddr);
	#endif
	
	/* SET I2C_CON*/	
	ic_con = I2c_Readb(I2C_CON_OFFSET,baseaddr);
	#if I2C_STANDARD
		ic_con &= ~ (3 << 1);//clear speed mode set
		ic_con |= I2C_CON_SPEED_STD; //speed mode set
	#else
		ic_con &= ~ (3 << 1);
		ic_con |= I2C_CON_SPEED_FAST;
	#endif
	ic_con &= ~I2C_CON_SLAVE_DISABLE;//Enable slave
	ic_con &= ~I2C_CON_MASTER;//Disable master
	ic_con |= I2C_CON_RESTART_EN;//enable restart
	ic_con &= ~I2C_CON_10BITADDR_MASTER;//7 bit ADDR

	I2c_Writeb(ic_con,I2C_CON_OFFSET,baseaddr);
   
	/* Set threshold*/
	I2c_Writeb(0,I2C_RX_TL_OFFSET,baseaddr);
	I2c_Writeb(15,I2C_TX_TL_OFFSET,baseaddr);
	
	/* Mask Interrupt */
	I2c_Writeb(I2C_INTR_RX_FULL|I2C_INTR_RD_REQ|I2C_INTR_TX_ABRT,I2C_INTR_MASK_OFFSET,baseaddr);
	
	/* Enable */
	I2c_Writeb(1,I2C_ENABLE_OFFSET,baseaddr);

}

//*****************************************************************************
//
//  I2C init
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void I2c_Channel_Init(void)
{
	I2c_Mux_set(DEBUGGER_I2C_CHANNEL);//mapping the smbus0 to sda/scl(0 1 2 3 4 5)	
	
	#if SMBUS0_CLOCK_EN//attention this smbus0 must be used to debugger 
		I2c_Slave_debugger_Init(0);
		I2c_Slave_Set_Sar(0x6A, 0);//use channel 0 to debugger 
	#endif
		
	#if SMBUS1_CLOCK_EN
		I2c_Master_Controller_Init(1);
		I2c_Master_Set_Tar(0x4C, 1);//use channel 1 to get temp default
	#endif

	#if SMBUS2_CLOCK_EN

	#endif

	#if SMBUS3_CLOCK_EN

	#endif

	#if (defined(AE103))
		#if SMBUS4_CLOCK_EN
			sysctl_iomux_config(GPIOA,4,3);
			sysctl_iomux_config(GPIOA,5,3);	
		#endif
		#if SMBUS5_CLOCK_EN
			sysctl_iomux_config(GPIOB,0,2);
			sysctl_iomux_config(GPIOB,7,2);
		#endif
	#endif
}

BYTE  I2c_Int_Mask(BYTE  channel, BYTE  type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			I2C0_INTR_MASK &= ~(0x1<<type);
			return 0;
		case I2C_CHANNEL_1:
			I2C1_INTR_MASK &= ~(0x1<<type);
			return 0;
		case I2C_CHANNEL_2:
			I2C2_INTR_MASK &= ~(0x1<<type);
			return 0;
		case I2C_CHANNEL_3:
			I2C3_INTR_MASK &= ~(0x1<<type);
			return 0;
		#ifdef AE103
		case I2C_CHANNEL_4:
			I2C4_INTR_MASK &= ~(0x1<<type);
			return 0;
		case I2C_CHANNEL_5:
			I2C5_INTR_MASK &= ~(0x1<<type);
			return 0;
		#endif						
		default:
			return -1;
	}
}


BYTE  I2c_Int_Unmask(BYTE  channel, BYTE  type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			I2C0_INTR_MASK |= (0x1<<type);
			return 0;
		case I2C_CHANNEL_1:
			I2C1_INTR_MASK |= (0x1<<type);
			return 0;
		case I2C_CHANNEL_2:
			I2C2_INTR_MASK |= (0x1<<type);
			return 0;
		case I2C_CHANNEL_3:
			I2C3_INTR_MASK |= (0x1<<type);
			return 0;
		#ifdef AE103
		case I2C_CHANNEL_4:
			I2C4_INTR_MASK |= (0x1<<type);
			return 0;
		case I2C_CHANNEL_5:
			I2C5_INTR_MASK |= (0x1<<type);
			return 0;						
		#endif
		default:
			return -1;
	}
}

BYTE  I2c_Int_Mask_Read(BYTE  channel, BYTE  type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			return ((I2C0_INTR_MASK & (0x1<<type)) == 0);
		case I2C_CHANNEL_1:
			return ((I2C1_INTR_MASK & (0x1<<type)) == 0);
		case I2C_CHANNEL_2:
			return ((I2C2_INTR_MASK & (0x1<<type)) == 0);
		case I2C_CHANNEL_3:
			return ((I2C3_INTR_MASK & (0x1<<type)) == 0);
		#ifdef AE103
		case I2C_CHANNEL_4:
			return ((I2C4_INTR_MASK & (0x1<<type)) == 0);
		case I2C_CHANNEL_5:
			return ((I2C5_INTR_MASK & (0x1<<type)) == 0);
		#endif			
		default:
			return -1;
	}
}

BYTE  I2c_Int_Status(BYTE  channel, BYTE  type)
{
	switch(channel)
	{
		case I2C_CHANNEL_0:
			return ((I2C0_INTR_STAT & (0x1<<type)) != 0);
		case I2C_CHANNEL_1:
			return ((I2C1_INTR_STAT & (0x1<<type)) != 0);
		case I2C_CHANNEL_2:
			return ((I2C2_INTR_STAT & (0x1<<type)) != 0);
		case I2C_CHANNEL_3:
			return ((I2C3_INTR_STAT & (0x1<<type)) != 0);
		#ifdef AE103
		case I2C_CHANNEL_4:
			return ((I2C4_INTR_STAT & (0x1<<type)) != 0);
		case I2C_CHANNEL_5:
			return ((I2C5_INTR_STAT & (0x1<<type)) != 0);
		#endif			
		default:
			return -1;
	}
}