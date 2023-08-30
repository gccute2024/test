/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:32:08
 * @Description: This file is used for SPI Flash Interface
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

#include "KERNEL_SPIF.H"

#include "KERNEL_TIMER.H"
#include "KERNEL_ACPI.H"
#pragma pack(4)

FUNCT_PTR_V_V Smf_Ptr;
FUNCT_PTR_V_D_BP Smft_Ptr;
FUNCT_PTR_V_D_BP Spif_Ptr;
FUNCT_PTR_V_D_BP ECU_Ptr;

BYTE Write_buff[256]={0,1,2,3,4,5,6};
//char Write_buff[256]="flash string test!\n";
BYTE Read_buff[256]={0};
//char Read_buff[]="";

void SPIF_Init(void)
{
   #if (!SPIF_CLOCK_EN)
	return;
   #endif
   SPIF_READ_ID();
}

//读取id
void SPIF_READ_ID(void)
{
   #if (!SPIF_CLOCK_EN)
	return;
   #endif
   uint32_t data;
   SPIF_CTRL0=0x1;
    while(!(SPIF_READY&0x1));
   SPIF_DBYTE=0x2;
   while(!(SPIF_READY&0x1));
   SPIF_FIFO_TOP=0x90;
   while(!(SPIF_READY&0x1));
   dprint("read flash id is 0x%x\n", SPIF_FIFO_TOP);
}

/**
 * @brief SPIF页写入
 *
 * @param addr          要进行页写入的地址
 * @param write_buff    传入一个256字节数组，数组中的内容是要页写入的值 
 *
 * @return 
 */
ALIGNED(4) void SPIF_Write(DWORD addr,BYTEP write_buff)
{
   #if (!SPIF_CLOCK_EN)
	return;
   #endif
   DWORD i,j,write_data=0;
   uint32_t temp_addrs=((addr&0xFF)<<24)+((addr&0xFF00)<<8)+((addr&0xFF0000)>>8);//设置地址
   BYTE temp_status=0;

   *((volatile uint8_t *) (0x5800)) = 'a' ; 

   while(!(SPIF_READY&0x1));
  
   SPIF_FIFO_TOP=temp_addrs+0x20;   //Sector Erase
  while(!(SPIF_READY&0x1));

   while(SPIF_STATUS&0x1);

   while(!(SPIF_READY&0x1));
	*((volatile uint8_t *) (0x5800)) = 'b' ; 
   //写
   SPIF_DBYTE=0xff;                 //准备写256字节
   while(!(SPIF_READY&0x1));
   
   SPIF_FIFO_TOP=(temp_addrs+0x2);  //Page Program
   
   for(i=0;i<64;i++)
   {
      j=i*4;
      write_data=write_buff[j]|(write_buff[j+1]<<8)|(write_buff[j+2]<<16)|(write_buff[j+3]<<24);
      temp_status = SPIF_FIFO_CNT;
		while((temp_status&0x3) >= 2)
		{
		   temp_status = SPIF_FIFO_CNT;
      }
		SPIF_FIFO_TOP=write_data;
   }
  while(!(SPIF_READY&0x1));

	while(SPIF_STATUS&0x1);

	while(!(SPIF_READY&0x1));
   *((volatile uint8_t *) (0x5800)) = 'c' ;
   while(!(SPIF_READY&0x1));
   
}

/*函数有待调整*/
ALIGNED(4) void SPIF_Read(DWORD addr,BYTEP read_buff)
{
   #if (!SPIF_CLOCK_EN)
	return;
   #endif
   uint32_t temp_addrs=((addr&0xFF)<<24)+((addr&0xFF00)<<8)+((addr&0xFF0000)>>8);//设置地址
   int i ,j, k= 0;
   BYTE temp_status=0;
   uint32_t temp_data[64];

   while(!(SPIF_READY&0x1));

   SPIF_DBYTE=0xff;

	while(!(SPIF_READY&0x1));

	SPIF_FIFO_TOP=(temp_addrs+0x3);

	for(j=0;j<64;j++)
	{
      i=j*4;
		temp_status = SPIF_FIFO_CNT;
		while((temp_status&0x3) == 0)
		{
		   temp_status = SPIF_FIFO_CNT;
      }
		temp_data[j]=SPIF_FIFO_TOP;
      read_buff[i]=(temp_data[j]&0xff);
      read_buff[i+1]=(temp_data[j]&0xff00)>>8;
      read_buff[i+2]=(temp_data[j]&0xff0000)>>16;
      read_buff[i+3]=(temp_data[j]&0xff000000)>>24;
	}

	while(!(SPIF_READY&0x1));

	while(SPIF_STATUS&0x1);

	while(!(SPIF_READY&0x1));

	*((volatile uint8_t *) (0x5800)) = 'd' ;

} 

void Smfi_Ram_Code(void)
{
   int i ,j, k= 0;
   uint32_t temp_status,temp_data;
   uint32_t data[64];
   uint8_t SmfiCmd,Smf_Addr0,Smf_Addr1,Smf_Addr2,Smf_Data0,Smf_Data1,Smf_Data2,Smf_Data3,Smf_Num,Smf_Sram_Base;
   uint32_t Smfi_Write_Addr, Smfi_Write_Data, Smfi_Custom_Addr;
   uint8_t Read_Num;

   //Disable_Interrupt_Main_Switch();
   //get command addr data
   SmfiCmd   = *((volatile uint8_t *)(0x2500));
   Smf_Addr0 = *((volatile uint8_t *)(0x2501));
   Smf_Addr1 = *((volatile uint8_t *)(0x2502));
   Smf_Addr2 = *((volatile uint8_t *)(0x2503));
   Smf_Data0 = *((volatile uint8_t *)(0x2506));
   Smf_Data1 = *((volatile uint8_t *)(0x2507));
   Smf_Data2 = *((volatile uint8_t *)(0x2508));
   Smf_Data3 = *((volatile uint8_t *)(0x2509));

   //compute flash address&data
   Smfi_Write_Addr = (uint32_t)(Smf_Addr2<<24) + (uint32_t)(Smf_Addr1<<16) + (uint32_t)(Smf_Addr0<<8) + (uint32_t)SmfiCmd;
   Smfi_Write_Data = (uint32_t)(Smf_Data3<<24) + (uint32_t)(Smf_Data2<<16) + (uint32_t)(Smf_Data1<<8) + (uint32_t)Smf_Data0;

   *((volatile uint8_t *) (0x5800)) = 'a' ;

   switch(SmfiCmd)
   {
      case 0x9f:                //read id
      {
         //SPIF_READ_ID();
      }
         break;

      case 0xc7:                //chip erase
      {

      }
         break;

      case 0x20:                //sector erase
      {
         temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
			   temp_status = SPIF_READY;
         }

		   //fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80020);
         SPIF_FIFO_TOP=Smfi_Write_Addr;
         while(!(SPIF_READY&0x1));

         temp_status = SPIF_STATUS;
         while(temp_status&0x1)
         {
	         temp_status = SPIF_STATUS;
         }
         
         while(!(SPIF_READY&0x1));
		   *((volatile uint8_t *) (0x5800)) = 'b' ; 
      }
         break;

      case 0x52:                //block erase 32kb
      {

      }
         break;

      case 0xd8:                //block erase 64kb
      {

      }
         break;

      case 0x02:                //page program
      {
         
         SPIF_DBYTE=0xff;

         temp_status = SPIF_READY;
         while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   //fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80002);
         SPIF_FIFO_TOP=Smfi_Write_Addr;

         for(i=0;i<64;i++)
         {
            temp_status = SPIF_FIFO_CNT;
		      while((temp_status&0x3) >= 2)
		      {
		         temp_status = SPIF_FIFO_CNT;
            }

		      //fla_if_write(FLA_FIFO_TOP_ADDR, ((i<<24)|(i<<16)|(i<<8)|(i)));
            SPIF_FIFO_TOP=Smfi_Write_Data;
         }

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   temp_status = SPIF_STATUS;
		   while(temp_status&0x1)
         {
			   temp_status = SPIF_STATUS;
         }
		     
         temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   *((volatile uint8_t *) (0x5800)) = 'c' ;
      }
         break;

      case 0x03:                //read data
      {
         Smf_Sram_Base = 0x00;       //set rewrite sram addr[11:4]

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   SPIF_DBYTE=0xff;
		   //fla_if_write(FLA_DBYTE_ADDR, Smf_Num-1);

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }
		     
         //fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80003);
         SPIF_FIFO_TOP=Smfi_Write_Addr;
         //if((Smf_Num%4) != 0)
         //Read_Num = Smf_Num/4 + 1;
         //else Read_Num = Smf_Num/4;
		   *((volatile uint8_t *) (0x5800)) = 's' ;

         for(j=0;j<64;j++)
         //for(j=0;j<Read_Num;j++)
            {
               temp_status = SPIF_FIFO_CNT;
		         while((temp_status&0x3) == 0)
		         {
		            temp_status = SPIF_FIFO_CNT;
               }
		         //data[j] = SPIF_FIFO_TOP;
		         *((volatile uint32_t *)(0x31000 + Smf_Sram_Base + j*4)) = SPIF_FIFO_TOP;
            }

		   *((volatile uint8_t *) (0x5800)) = 'x' ;

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   temp_status = SPIF_STATUS;
		   while(temp_status&0x1)
         {
			   temp_status = SPIF_STATUS;
         }

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   *((volatile uint8_t *) (0x5800)) = 'd' ;
      }
         break;

      case 0x12:            //firmware cycle write
      {
         Smfi_Custom_Addr = (uint32_t)(Smf_Addr0<<24) + (uint32_t)(Smf_Addr1<<16) + (uint32_t)(Smf_Addr2<<8) + 0x02;
         SPIF_DBYTE=0x0;

         temp_status = SPIF_READY;
         while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   //fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80002);
         SPIF_FIFO_TOP=Smfi_Custom_Addr;

   
         temp_status = SPIF_FIFO_CNT;
		   while((temp_status&0x3) >= 2)
		   {
		      temp_status = SPIF_FIFO_CNT;
         }

		   //fla_if_write(FLA_FIFO_TOP_ADDR, ((i<<24)|(i<<16)|(i<<8)|(i)));
         SPIF_FIFO_TOP=Smfi_Write_Data;

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   temp_status = SPIF_STATUS;
		   while(temp_status&0x1)
         {
			   temp_status = SPIF_STATUS;
         }

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   *((volatile uint8_t *) (0x5800)) = 'e' ;
      }
         break;

      case 0x13:             //firmware cycle read
      {
         Smfi_Custom_Addr = (uint32_t)(Smf_Addr0<<24) + (uint32_t)(Smf_Addr1<<16) + (uint32_t)(Smf_Addr2<<8) + 0x03;

         temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   SPIF_DBYTE=0x0;
         
		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   //fla_if_write(FLA_FIFO_TOP_ADDR, 0x00c80003);
         SPIF_FIFO_TOP=Smfi_Custom_Addr;

         temp_status = SPIF_FIFO_CNT;
		   while((temp_status&0x3) == 0)
		   {
		      temp_status = SPIF_FIFO_CNT;
         }

		   temp_data = SPIF_FIFO_TOP;

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

		   temp_status = SPIF_STATUS;
		   while(temp_status&0x1)
         {
			   temp_status = SPIF_STATUS;
         }

		   temp_status = SPIF_READY;
		   while(!(temp_status&0x1))
         {
	         temp_status = SPIF_READY;
         }

         Smf_Data0 = (uint8_t) temp_data;
         Smf_Data1 = (uint8_t) (temp_data >> 8);
         Smf_Data2 = (uint8_t) (temp_data >> 16);
         Smf_Data3 = (uint8_t) (temp_data >> 24);
         *((volatile uint8_t *)(0x250a)) |= 0xa;  //set serirq interrupt status
		   *((volatile uint8_t *) (0x5800)) = 'f' ;
      }
         break;

      default: break;
   }

	*((volatile uint8_t *) (0x5800)) = 'g' ;
   //main();
}

/**
 * @brief IO方式更新EC
 *
 * @param addr          要进行页写入的地址
 * @param write_buff    传入一个256字节数组，数组中的内容是要页写入的值 
 *
 * @return 
 */
void IO_ECU(void)
{

   //int i,j = 0;
   _R7,_R8 = 0;
   uint32_t temp_status,temp_data;
   uint32_t data[64];
   uint8_t Smf_Addr0,Smf_Addr1,Smf_Addr2,Smf_Data0,Smf_Data1,Smf_Data2,Smf_Data3;
   uint32_t Page_Write_Addr;

   while(!(SPIF_READY&0x1))WDT_CRR =0x76;
   SPIF_FIFO_TOP=0xc7;
   while(!(SPIF_READY&0x1))WDT_CRR =0x76;
   while(SPIF_STATUS&0x1)WDT_CRR =0x76;
   while(!(SPIF_READY&0x1))WDT_CRR =0x76;
   
   *((volatile uint8_t *) (0x5800)) = 'a';
   PMC2_DOR = 0x5A;  //response to erase flash ok

   while(1)
   {

		if( IS_MASK_CLEAR(PMC2_STR,IBF2) ) continue;
      *((volatile uint8_t *) (0x5800)) = 'b';
      ECU_Cmd = PMC2_DIR;
      
      if(ECU_Cmd == 0xB5)    //start page write flash
      {
         *((volatile uint8_t *) (0x5800)) = 'c' ;
         ECU_Cmd = 0x0;

         //get address
         Smf_Addr0 = SMF_FADDR0;
         Smf_Addr1 = SMF_FADDR1;
         Smf_Addr2 = SMF_FADDR2;
         Page_Write_Addr = (uint32_t)(Smf_Addr0<<24) + (uint32_t)(Smf_Addr1<<16) + (uint32_t)(Smf_Addr2<<8) + (uint32_t)(0x2);
         //printf("Page_Write_Addr:0x%lx\n",Page_Write_Addr);
         *((volatile uint8_t *) (0x5800)) = 'd' ;
         PMC2_DOR = 0x5B;

      } else if(ECU_Cmd == 0xC5){
         *((volatile uint8_t *) (0x5800)) = 'e' ;
         if(_R8 >= 64) _R8 = 0;
         ECU_Cmd = 0x0;

         Smf_Data0 = SMF_DR0;
         Smf_Data1 = SMF_DR1;
         Smf_Data2 = SMF_DR2;
         Smf_Data3 = SMF_DR3;
   
         data[_R8] = (uint32_t)(Smf_Data3<<24) + (uint32_t)(Smf_Data2<<16) + (uint32_t)(Smf_Data1<<8) + (uint32_t)Smf_Data0;
         //printf("_R8:%d,data:0x%lx\n",_R8,data[_R8]);
         _R8++;

         if(_R8 >= 64)
         {
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            SPIF_DBYTE=0xff;
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            SPIF_FIFO_TOP= Page_Write_Addr;
            for(_R7=0;_R7<64;_R7++)
            {
                    while((SPIF_FIFO_CNT&0x3) != 0)WDT_CRR =0x76;
                    SPIF_FIFO_TOP=data[_R7];
            }
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            while(SPIF_STATUS&0x1)WDT_CRR =0x76;
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            *((volatile uint8_t *) (0x5800)) = 'g' ;
            //printf("1 page\n");
         }
            *((volatile uint8_t *) (0x5800)) = 'f' ;
            PMC2_DOR = _R8;

      } else {
         *((volatile uint8_t *) (0x5800)) = 'h' ;
      }
   }
}

FUNCT_PTR_V_D_BP Load_Fla_If_To_Ram(FUNCT_PTR_V_D_BP funcpoint,const int malloc_size)
{
   int i;
   Smft_Ptr = malloc(malloc_size);
   if(!Smft_Ptr)
   {
      dprint("attention! malloc failed!\n");
      return 0;
   }
	Tmp_XPntr = (VBYTE *)Smft_Ptr;
	Tmp_code_pointer = (VBYTE *)funcpoint;
   for( i=0; i<malloc_size; i++)
   {
      *Tmp_XPntr = *Tmp_code_pointer;
	   Tmp_XPntr++;
   	Tmp_code_pointer++;
   }
   dprint("Load Function to DRAM finish!\n");
   return Smft_Ptr;
}

void Dram_Part_Init(void)
{
   int i = 0;

   for( i = 0 ; i < 0x800 ;i ++)
   {
	   (*(BYTEP )(0x21000 + i)) = 0;
   }
   
   dprint("Dram Part Init finish!\n");
}

void Dram_Read(void)
{
   int i = 0;
   BYTE  data;

   for( i = 0 ; i < 0x800 ;i ++)
   {
      data = (*(BYTEP )(0x21000 + i));
      dprint("Read DRAM data is 0x%x\n", data);
   }
}

void Load_Smfi_To_Dram(FUNCT_PTR_V_V funcpoint)
{
   int i;

   Smf_Ptr = malloc(0x1000);
   dprint("Smf_Ptr value is 0x%x\n",Smf_Ptr);

   if(!Smf_Ptr)
      dprint("attention! malloc failed!\n");

	Tmp_XPntr = (VBYTE *)Smf_Ptr;
	Tmp_code_pointer = (VBYTE *)funcpoint;

   for( i=0; i<0x1000; i++)
   {
      *Tmp_XPntr = *Tmp_code_pointer;
	   Tmp_XPntr++;
   	Tmp_code_pointer++;
   }

   dprint("Load Smfi Function to DRAM finish!\n");
}

/**
 * @brief 先把SPIF页写入函数搬入内存中，在内存执行
 *
 * @param addr          要进行页写入的地址
 * @param write_buff    传入一个256字节数组，数组中的内容是要页写入的值 
 *
 * @return 
 */
void RunSPIF_WriteFromRAM(DWORD addr,BYTEP write_buff)
{
   Disable_Interrupt_Main_Switch();      //Disable All Interrupt
   WDT_REG(0x4)=0xffff;//设置最长延时
   WDT_CRR =0x76;//重启计数器
   Spif_Ptr=Load_Fla_If_To_Ram(SPIF_Write,0x1000);//Load Fla_If Code to DRAM
   (*Spif_Ptr)(addr,write_buff);            //Do Function at 0x21000,this 0x21000 just for test,maybe cause some problem
   Enable_Interrupt_Main_Switch();
   free(Spif_Ptr); // 释放通过 malloc 分配的内存空间
	Spif_Ptr = NULL; // 将指针设置为 NULL，以避免悬空指针问题
}

/**
 * @brief 先把SPIF页读出函数搬入内存中，在内存执行
 *
 * @param addr          要进行页写入的地址
 * @param read_buff    传入一个256字节数组，数组中的内容是要页读出的值 
 *
 * @return 
 */
void RunSPIF_ReadFromRAM(DWORD addr,BYTEP read_buff)
{
   Disable_Interrupt_Main_Switch();      //Disable All Interrupt
   WDT_REG(0x4)=0xffff;//设置最长延时
   WDT_CRR =0x76;//重启计数器
   Spif_Ptr=Load_Fla_If_To_Ram(SPIF_Read,0x1000);//Load Fla_If Code to DRAM
   (*Spif_Ptr)(addr,read_buff);            //Do Function at 0x21000,this 0x21000 just for test,maybe cause some problem
   Enable_Interrupt_Main_Switch();
   free(Spif_Ptr); // 释放通过 malloc 分配的内存空间
	Spif_Ptr = NULL; // 将指针设置为 NULL，以避免悬空指针问题
   for(short i=0;i<256;i++)
	{
		printf("Read_buff:0x%x\n",read_buff[i]);
	}
}
 
//-----------------------------------------------------------------------------
// The function for ITE flash utility
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void SPK_SHIO_ECUpdate_Utility(void)
{
	Disable_Interrupt_Main_Switch();				// Disable all interrupt 
	Load_Smfi_To_Dram(IO_ECU);	               // Load function to ram
   (*Smf_Ptr)();                             // Jump to RAM and excute IO_ECU function
}
