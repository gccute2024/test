/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-24 10:40:38
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

#include "KERNEL_PNP.H"

#include "KERNEL_TIMER.H"
#include "KERNEL_SOC_FUNC.H"



//-----------------------------------------------------------------------------
// PNP config send requeset
//-----------------------------------------------------------------------------
void Config_PNP_Access_Request(void)
{
	int regaddr,rdata;
    unsigned long timeout = 1000;

	//send access request and read ready
	
	PNP_CONFIG = 0x2e2f0002;
	
	//judge access request clear or not
	/*rdata = REG32(regaddr);
	while((rdata & 0x00000001) != 1)
	{
		rdata = REG32(regaddr);
	}*/
    do {
        rdata = PNP_CONFIG;

		if((rdata & 0x00000001) == 1)
			break; /* OK */

		udelay(1000);
		timeout--;

	} while(timeout);
    if(timeout == 0)
        dprint("access request timeout\n");
}

//-----------------------------------------------------------------------------
// PNP config release request
//-----------------------------------------------------------------------------
void Config_PNP_Access_Release(void)
{
	int regaddr,rdata;
    unsigned long timeout = 1000;
	
	//clear access request
	regaddr = PNP_CONFIG_ADDR;
	REG32(regaddr) = 0x2e2f0000;

	//judge access request clear or not
	/*rdata=REG32(regaddr);
	while((rdata & 0x00000001) != 0)
	{
	   rdata = REG32(regaddr);
	}*/
    do {
        rdata = REG32(regaddr);

		if((rdata & 0x00000001) == 0)
			break; /* OK */

		udelay(1000);
		timeout--;

	} while(timeout);
    if(timeout == 0)
        dprint("access request clear timeout\n");
}

//-----------------------------------------------------------------------------
// PNP Config write
// Need send request before write
// and release request after write
//-----------------------------------------------------------------------------
void Config_PNP_Write(BYTE  idx,BYTE  ldn,BYTE  data)
{
	int wdata,regaddr,rdata;
	int e2pnp_reg_wc,e2pnp_reg_rc;
    unsigned long timeout = 1000;

	//send write request and write data
	wdata=((idx << 24) | (ldn << 16) | (1 << 8) | data);
	PNP_CTRL1 = wdata;
	
	//judge wirte done
	/*rdata=REG32(regaddr);
	e2pnp_reg_wc = (rdata & 0x00000400);
	while(!e2pnp_reg_wc)
	{
		 rdata=REG32(regaddr);
		 e2pnp_reg_wc = (rdata & 0x00000400);
	}*/
    do {
        rdata = PNP_CTRL1;
        e2pnp_reg_wc = (rdata & 0x00000400);

		if(e2pnp_reg_wc != 0)
			break; /* OK */

		udelay(1000);
		timeout--;

	} while(timeout);
    if(timeout == 0)
        dprint("pnp write done timeout\n");

	//clear write requeset
	PNP_CTRL1 &= 0xfffffeff;
}

//-----------------------------------------------------------------------------
// PNP Config read
// Need send request before read
// and release request after read
//-----------------------------------------------------------------------------
void Config_PNP_Read(BYTE  idx,BYTE  ldn)
{
	int wdata,regaddr,rdata,pnpdata;
	int e2pnp_reg_wc,e2pnp_reg_rc;
    unsigned long timeout,timeout1 = 1000;
	
	//send read request
	wdata=((idx << 24) | (ldn << 16) | (1 << 9));
	PNP_CTRL1=wdata;
	//read data
	/*rdata=REG32(regaddr);
	e2pnp_reg_rc = (rdata & 0x00000800);
	while(!e2pnp_reg_rc)
	{
		 rdata=REG32(regaddr);
		 e2pnp_reg_rc = (rdata & 0x00000800);
	}*/
    do {
        rdata = PNP_CTRL1;
        e2pnp_reg_rc = (rdata & 0x00000800);

		if(e2pnp_reg_rc != 0)
			break; /* OK */

		udelay(1000);
		timeout--;

	} while(timeout);
    if(timeout == 0)
        dprint("pnp read done timeout\n");
	
	idx = (rdata & 0xff000000) >> 24;
	ldn = (rdata & 0x00ff0000) >> 16;
	pnpdata = rdata & 0x000000ff;
	dprint("regaddr:0x%x, idx= 0x%x, ldn=0x%x, read value 0x%x\n",regaddr,idx,ldn,pnpdata);

	//clear
	PNP_CTRL1 &= 0xfffffdff;

	//judge finish
	/*rdata=REG32(regaddr);
	while(((rdata & 0x00000f00) >> 8) != 0xc)
	{
	   rdata=REG32(regaddr);
	}*/
    do
    {
        rdata=PNP_CTRL1;    
        if(((rdata & 0x00000f00) >> 8) == 0xc)
            break;

        udelay(1000);
        timeout1--;

    } while (timeout1);
    if(timeout1 == 0)
        dprint("pnp finish timeout\n");
    
}

//-----------------------------------------------------------------------------
// Clear Logical Device Default IRQ Num
//-----------------------------------------------------------------------------
void Clear_Logical_Device_Default_IRQ_Num(void)
{

	Config_PNP_Write(0x7,0x1,0x1);
	Config_PNP_Write(0x70,0x1,0x0);
	Config_PNP_Write(0x7,0x2,0x2);
	Config_PNP_Write(0x70,0x2,0x0);
	Config_PNP_Write(0x7,0x3,0x3);
	Config_PNP_Write(0x70,0x3,0x0);
	Config_PNP_Write(0x7,0x4,0x4);
	Config_PNP_Write(0x70,0x4,0x0);
	Config_PNP_Write(0x7,0x8,0x8);
	Config_PNP_Write(0x70,0x8,0x0);
	Config_PNP_Write(0x7,0xf,0xf);
	Config_PNP_Write(0x70,0xf,0x0);
	Config_PNP_Write(0x7,0x10,0x10);
	Config_PNP_Write(0x70,0x10,0x0);
	Config_PNP_Write(0x7,0x11,0x11);
	Config_PNP_Write(0x70,0x11,0x0);
	Config_PNP_Write(0x7,0x12,0x12);
	Config_PNP_Write(0x70,0x12,0x0);
	Config_PNP_Write(0x7,0x13,0x13);
	Config_PNP_Write(0x70,0x13,0x0);
	Config_PNP_Write(0x7,0x15,0x15);
	Config_PNP_Write(0x70,0x15,0x0);
	Config_PNP_Write(0x7,0x16,0x16);
	Config_PNP_Write(0x70,0x16,0x0);
	Config_PNP_Write(0x7,0x17,0x17);
	Config_PNP_Write(0x70,0x17,0x0);
	Config_PNP_Write(0x7,0x18,0x18);
	Config_PNP_Write(0x70,0x18,0x0);
	Config_PNP_Write(0x7,0x19,0x19);
	Config_PNP_Write(0x70,0x19,0x0);
	Config_PNP_Write(0x7,0x1a,0x1a);
	Config_PNP_Write(0x70,0x1a,0x0);
	Config_PNP_Write(0x7,0x1b,0x1b);
	Config_PNP_Write(0x70,0x1b,0x0);
	Config_PNP_Write(0x7,0x1c,0x1c);
	Config_PNP_Write(0x70,0x1c,0x0);
	Config_PNP_Write(0x7,0x1d,0x1d);
	Config_PNP_Write(0x70,0x1d,0x0);
	Config_PNP_Write(0x7,0x1e,0x1e);
	Config_PNP_Write(0x70,0x1e,0x0);
	Config_PNP_Write(0x7,0x1f,0x1f);
	Config_PNP_Write(0x70,0x1f,0x0);

}

BYTE Sio_PNP_Table[]=
{
	/*LD:MOUSE*/
	0x7,0x5,0x5,	//select ld mouse
	0x70,0x5,0xc,	//select serirq num:12
	0x71,0x5,0x1,	//set serirq level/edge
	0x30,0x5,0x1,	//enable ld

	/*LD:KEYBOARD*/
	0x7,0x6,0x6,	//select ld mouse
	0x61,0x6,0x60,	//config io port 60
	0x63,0x6,0x64,	//config io port 64
	0x70,0x6,0x1,	//select serirq num:1
	0x71,0x6,0x1,	//set serirq level/edge
	0x30,0x6,0x1,	//enable ld

	#if !(KBC_PNP_ONLY_SWITCH)
	/*LD:PM1*/
	0x7,0x11,0x11,	//select ld mouse
	0x61,0x11,0x62,	//config io port 62
	0x63,0x11,0x66,	//config io port 66
	0x70,0x11,0x5,	//select serirq num:5
	0x71,0x11,0x1,	//set serirq level/edge
	0x30,0x11,0x1,	//enable ld

	/*LD:PM2*/
	0x7,0x12,0x12,	//select ld mouse
	0x61,0x12,0x68,	//config io port 68
	0x63,0x12,0x6c,	//config io port 6c
	0x70,0x12,0x9,	//select serirq num:9
	0x71,0x12,0x1,	//set serirq level/edge
	0x30,0x12,0x1,	//enable ld
	#endif

};

void Sio_PNP_Config(void)
{

	BYTE * data_pntr;
	BYTE para0,para1,para2;
    BYTE cnt;

    cnt=0;
    data_pntr=Sio_PNP_Table;
    while(cnt < (sizeof(Sio_PNP_Table)/3))
    {
		para0=0x0;
		para1=0x0;
		para2=0x0;

		para0 = *data_pntr;
		data_pntr ++;

		para1 = *data_pntr;
		data_pntr ++;

		para2 = *data_pntr;
		data_pntr ++;

		Config_PNP_Write(para0,para1,para2);

        cnt ++;
    }

	dprint("Sio_PNP_Config finish\n");
}

//-----------------------------------------------------------------------------
// Config all logical device PNP when MACRO KBC_PNP_ONLY_SWITCH is 0, KBC_PNP_ONLY_SWITCH
// default value is 0
//-----------------------------------------------------------------------------
void Config_All_Logical_Device_PNP(void)
{

	//SWUC
	Config_PNP_Write(0x07, 0x04, 0x04);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x04, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x04, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x04, 0x0f);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x04, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x04, 0x00);
	vDelayXms(5);
	//dprint("SWUC PNP Config finish!\n"); 

	//HWMONITOR
	Config_PNP_Write(0x07, 0x03, 0x03);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x03, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x03, 0x05);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x03, 0x0f);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x03, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x03, 0x00);
	vDelayXms(5);
	//dprint("HWMonitor PNP Config finish!\n"); 

	//SMFI
	//Config SMFI
	Config_PNP_Write(0x23, 0x01, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x07, 0x0f, 0x0f);
	vDelayXms(5);
	//config firmware cycle 
	Config_PNP_Write(0xf0, 0x0f, 0xfe); 
	vDelayXms(5);
	Config_PNP_Write(0xf1, 0x0f, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0xf2, 0x0f, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0xf4, 0x0f, 0xf0);           //config FWID 0xf
	vDelayXms(5);
	Config_PNP_Write(0xf5, 0x0f, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0xf6, 0x0f, 0x00);
	vDelayXms(5);

	//Config SRAM offset map
	Config_PNP_Write(0x60, 0x0f, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x0f, 0x00);
	vDelayXms(5);
	//HOST Semaphore Config
	//Config_PNP_Write(0xf9, 0x0f, 0xff); //config HOST Semaphore interrupt enable
	vDelayXms(5);
	Config_PNP_Write(0xfa, 0x0f, 0x0);  //config HOST Semaphore address
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x0f, 0x01);
	vDelayXms(5);
	
	//UART0_BAUD
	Config_PNP_Write(0x07, 0x1b, 0x1b);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1b, 0x03);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1b, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1b, 0x04);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1b, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1b, 0x01);
	vDelayXms(5);
	//dprint("UART0_BAUD PNP Config finish!\n");  

	//UART1_BAUD
	Config_PNP_Write(0x07, 0x1c, 0x1c);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1c, 0x02);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1c, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1c, 0x03);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1c, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1c, 0x01);
	vDelayXms(5);
	//dprint("UART1_BAUD PNP Config finish!\n"); 

	//UART2_BAUD
	Config_PNP_Write(0x07, 0x1d, 0x1d);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1d, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1d, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1d, 0x0a);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1d, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1d, 0x01);
	vDelayXms(5);
	//dprint("UART2_BAUD PNP Config finish!\n"); 

	//UART3_BAUD
	Config_PNP_Write(0x07, 0x1e, 0x1e);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1e, 0x04);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1e, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1e, 0x0b);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1e, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1e, 0x01);
	vDelayXms(5);
	//dprint("UART3_BAUD PNP Config finish!\n"); 

	//UARTA_BAUD
	Config_PNP_Write(0x07, 0x01, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x01, 0x05);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x01, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x01, 0x0f);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x01, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x01, 0x01);
	vDelayXms(5);
	//dprint("UARTA_BAUD PNP Config finish!\n"); 

	//UARTB_BAUD
	Config_PNP_Write(0x07, 0x02, 0x02);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x02, 0x06);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x02, 0xf8);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x02, 0x0e);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x02, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x02, 0x01);
	vDelayXms(5);
	//dprint("UARTB_BAUD PNP Config finish!\n"); 

	//CAN0 Channel
	Config_PNP_Write(0x07, 0x15, 0x15);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x15, 0x10);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x15, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x15, 0x0a);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x15, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x15, 0x01);
	vDelayXms(5);
	//dprint("CAN0 PNP Config finish!\n"); 

	//CAN1 Channel
	Config_PNP_Write(0x07, 0x16, 0x16);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x16, 0x20);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x16, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x16, 0x0b);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x16, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x16, 0x01);
	vDelayXms(5);
	//dprint("CAN1 PNP Config finish!\n"); 

	//CAN2 Channel
	Config_PNP_Write(0x07, 0x1a, 0x1a);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1a, 0x30);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1a, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1a, 0x0d);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1a, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1a, 0x01);
	vDelayXms(5);
	//dprint("CAN2 PNP Config finish!\n"); 
 
	//CAN3 Channel
	Config_PNP_Write(0x07, 0x1f, 0x1f);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x1f, 0x40);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x1f, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x1f, 0x0e);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x1f, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x1f, 0x01);
	vDelayXms(5);
	//dprint("CAN3 PNP Config finish!\n");  

    //SPIM
	Config_PNP_Write(0x07, 0x13, 0x13);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x13, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x13, 0x00);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x13, 0x10);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x13, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x13, 0x01);
	vDelayXms(5);
	//dprint("SPIM Config finish!\n");  

	//Parallel
	Config_PNP_Write(0x07, 0x08, 0x08);
	vDelayXms(5);
	Config_PNP_Write(0x60, 0x08, 0x02);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x08, 0x78);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x08, 0x0f);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x08, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x08, 0x01);
	vDelayXms(5);
	//dprint("Parallel Config finish!\n"); 

	//PMC1
	Config_PNP_Write(0x07, 0x11, 0x11);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x11, 0x62);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x11, 0x66);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x11, 0x05);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x11, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x11, 0x01);
	vDelayXms(5);
	//dprint("PMC1 Config finish!\n"); 

	//PMC2
	Config_PNP_Write(0x07, 0x12, 0x12);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x12, 0x68);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x12, 0x6c);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x12, 0x09);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x12, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x12, 0x01);
	vDelayXms(5);
	//dprint("PMC2 Config finish!\n"); 

	//PMC3
	Config_PNP_Write(0x07, 0x17, 0x17);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x17, 0x6a);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x17, 0x6e);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x17, 0x0f);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x17, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x17, 0x01);
	vDelayXms(5);
	//dprint("PMC3 Config finish!\n"); 

	//PMC4
	Config_PNP_Write(0x07, 0x18, 0x18);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x18, 0x74);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x18, 0x78);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x18, 0x06);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x18, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x18, 0x01);
	vDelayXms(5);
	//dprint("PMC4 Config finish!\n"); 

	//PMC5
	Config_PNP_Write(0x07, 0x19, 0x19);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x19, 0x7a);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x19, 0x7c);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x19, 0x08);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x19, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x19, 0x01);
	vDelayXms(5);
	//dprint("PMC5 Config finish!\n");  

	//KBC MOUSE
	Config_PNP_Write(0x07, 0x05, 0x05);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x05, 0x60);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x05, 0x64);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x05, 0x0c);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x05, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x05, 0x01);
	vDelayXms(5);
	//dprint("KBC MOUSE Config finish!\n");  

	//KBC KEYBOARD
	Config_PNP_Write(0x07, 0x06, 0x06);
	vDelayXms(5);
	Config_PNP_Write(0x61, 0x06, 0x60);
	vDelayXms(5);
	Config_PNP_Write(0x63, 0x06, 0x64);
	vDelayXms(5);
	Config_PNP_Write(0x70, 0x06, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x71, 0x06, 0x01);
	vDelayXms(5);
	Config_PNP_Write(0x30, 0x06, 0x01);
	vDelayXms(5);
	//dprint("KBC KEYBOARD Config finish!\n"); 

    Port80_Bram_PNP_Config();
	BRAM_Config();

    //dprint("All logical device PNP config completed!\n");

}

//-----------------------------------------------------------------------------
// PNP config function
//-----------------------------------------------------------------------------
void LogicalDevice_PNP_Config(void)
{
	//dprint("LogicalDevice PNP Config start!\n");

	//LogicaLDevice Enable
	//*((VDWORD*)(0x30408)) |= 0x000000040; 
	
	//Send Request
	Config_PNP_Access_Request();	

	//PNP Default Config
	Config_PNP_Write(0x23, 0x01, 0x01);

    //set other logical device irq to 0
    Clear_Logical_Device_Default_IRQ_Num();

	Sio_PNP_Config();
	//Config_All_Logical_Device_PNP();

	Config_PNP_Access_Release();
	
	dprint("LogicalDevice PNP Config finish!\n");
}

//-----------------------------------------------------------------------------
// Logical Device PNP config function
//-----------------------------------------------------------------------------
BYTE LD_PNP_Config(void)
{
	LogicalDevice_PNP_Config();
}