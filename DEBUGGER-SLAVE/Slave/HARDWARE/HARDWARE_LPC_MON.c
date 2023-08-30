/*
 * @Author: Maple
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-02 21:35:42
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

#include "HARDWARE_LPC_MON.H"

uint32_t* Lpc_Mon_Ptr; 

lpc_mon lpc_stat;

void Lpc_To_Ram_Config(void)
{

	uint8_t data;

	dprint("This is Lpc to Ram!\n");
	data = SYSCTL_RESERVED;
	dprint("0x304c0 data is 0x%x\n", data);
	SYSCTL_RESERVED|= 0x100 ; //enable lpc_mon
	data = SYSCTL_RESERVED;
	dprint("0x304c0 data is 0x%x\n", data);

	//config interrupt configuration
	WMARK_FULL=0x1; //set data full region 4DWORD
	//WMARK_FULL,0x8); //8DWORD
	RMARK_EMPTY=0x0;
	INTR_MASK=0x3;//屏蔽fifo空中断
			   
	Lpc_Mon_Ptr = (malloc(0xff));
	dprint("Lpc_Mon_Ptr is 0x%p\n", Lpc_Mon_Ptr);
    dprint("INTR_MASK is 0x%x\n", INTR_MASK);
}

void LPC_Monitor(void)
{
    BYTE  temp_addr0=0;
    BYTE  temp_addr1=0;
    BYTE  temp_addr2=0;
    BYTE  temp_addr3=0;
    BYTE  temp_addr4=0;
    BYTE  temp_addr5=0;
    BYTE  temp_addr6=0;
    BYTE  temp_addr7=0;

    BYTE  temp_data0=0;
    BYTE  temp_data1=0;
    if(LPC_SOF==1)
    {
        if(((*(Lpc_Mon_Ptr + LPC_MON_CNT))&0x0c00)==0)
        {
            if(((*(Lpc_Mon_Ptr + LPC_MON_CNT))&0x0200)==0)
            {
                lpc_stat.cyctpe_dir="I/O Read";
            }
            else
            {
                lpc_stat.cyctpe_dir="I/O Write";
            }
        }
        else if(((*(Lpc_Mon_Ptr + LPC_MON_CNT))&0x0800)==0)
        {
            if(((*(Lpc_Mon_Ptr + LPC_MON_CNT))&0x0200)==0)
            {
                lpc_stat.cyctpe_dir="MEM Read";
            }
            else
            {
                lpc_stat.cyctpe_dir="MEM Write";
            }
        }
/*         if(((*(Lpc_Mon_Ptr + LPC_MON_CNT))&0xf)==0xe)
        {
            lpc_stat.cyctpe_dir="Firmware Write";
        }
        if(((*(Lpc_Mon_Ptr + LPC_MON_CNT))&0xf)==0xd)
        {
            lpc_stat.cyctpe_dir="Firmware Read";
        } */ 
        LPC_SOF=0;
    }
    if(LPC_EOF==1)
    {
        if(strstr(lpc_stat.cyctpe_dir,"I/O Read"))
        {
            temp_addr0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>16)&0x0f;
            temp_addr1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>24)&0x0f;
            temp_addr2=((*(Lpc_Mon_Ptr + LPC_MON_CNT-2)))&0x0f;
            temp_addr3=((*(Lpc_Mon_Ptr + LPC_MON_CNT-2))>>8)&0x0f;

            temp_data0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-1))>>8)&0x0f;
            temp_data1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-1))>>16)&0x0f;

            lpc_stat.addr=temp_addr3|(temp_addr2<<4)|(temp_addr1<<8)|(temp_addr0<<12);
            lpc_stat.data=temp_data0|(temp_data1<<4);
        }
        if(strstr(lpc_stat.cyctpe_dir,"I/O Write"))
        {
            temp_addr0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>16)&0x0f;
            temp_addr1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>24)&0x0f;
            temp_addr2=((*(Lpc_Mon_Ptr + LPC_MON_CNT-2)))&0x0f;
            temp_addr3=((*(Lpc_Mon_Ptr + LPC_MON_CNT-2))>>8)&0x0f;

            temp_data0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-2))>>16)&0x0f;
            temp_data1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-2))>>24)&0x0f;

            lpc_stat.addr=temp_addr3|(temp_addr2<<4)|(temp_addr1<<8)|(temp_addr0<<12);
            lpc_stat.data=temp_data0|(temp_data1<<4);
        }
        if(strstr(lpc_stat.cyctpe_dir,"MEM Read"))
        {
            temp_addr0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-5))>>16)&0x0f;
            temp_addr1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-5))>>24)&0x0f;
            temp_addr2=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4)))&0x0f;
            temp_addr3=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4))>>8)&0x0f;
            temp_addr4=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4))>>16)&0x0f;
            temp_addr5=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4))>>24)&0x0f;
            temp_addr6=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3)))&0x0f;
            temp_addr7=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>8)&0x0f;

            temp_data0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-1))>>24)&0x0f;
            temp_data1=((*(Lpc_Mon_Ptr + LPC_MON_CNT)))&0x0f;

            lpc_stat.addr=temp_addr7|(temp_addr6<<4)|(temp_addr5<<8)|(temp_addr4<<12)|
            (temp_addr3<<16)|(temp_addr2<<20)|(temp_addr1<<24)|(temp_addr0<<28);
            lpc_stat.data=temp_data0|(temp_data1<<4);
        }
        if(strstr(lpc_stat.cyctpe_dir,"MEM Write"))
        {
            temp_addr0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-5))>>16)&0x0f;
            temp_addr1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-5))>>24)&0x0f;
            temp_addr2=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4)))&0x0f;
            temp_addr3=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4))>>8)&0x0f;
            temp_addr4=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4))>>16)&0x0f;
            temp_addr5=((*(Lpc_Mon_Ptr + LPC_MON_CNT-4))>>24)&0x0f;
            temp_addr6=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3)))&0x0f;
            temp_addr7=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>8)&0x0f;

            temp_data0=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>16)&0x0f;
            temp_data1=((*(Lpc_Mon_Ptr + LPC_MON_CNT-3))>>24)&0x0f;

            lpc_stat.addr=temp_addr7|(temp_addr6<<4)|(temp_addr5<<8)|(temp_addr4<<12)|
            (temp_addr3<<16)|(temp_addr2<<20)|(temp_addr1<<24)|(temp_addr0<<28);
            lpc_stat.data=temp_data0|(temp_data1<<4);
        }
        
    }

}
