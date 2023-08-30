/*
 * @Author: Linyu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-16 18:27:05
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

#include "AE_UPDATE.H"

/****************************************************************************************************************/
/***********************************************************************/
#define flash_write(addr,value)	((*((volatile uint32_t *)(addr))) = (value))
#define flash_read(addr)		(*((volatile uint32_t *)(addr)))
/***********************************************************************/
/***********************************************************************/
#define sms_write(addr,value)	((*((volatile uint8_t *)(addr))) = (value))
#define sms_read(addr)		(*((volatile uint8_t *)(addr)))

FUNCT_PTR_V_V update_ptr=NULL;

//初始化配置
void LPC_Update_Config_Init(void)
{
    SHAREMEMORY_INIT();
}
//更新主函数
void ALIGNED(4) OPTIMEIZE0 EC_Update(void)
{
    PRINTF_TX = 0xff;
    *((volatile uint8_t *)(SRAM_BASE_ADDR+0X100)) = 0xff;
    REG32(0x3042C)|=BIT(8);
	REG32(0x3042C)&=0xfffffeff;
    while (!(PRINTF_LSR & UART_LSR_TEMP)) ;
    if(UART_UPDATE&&update_mode==0x0)
    {
        PRINTF_TX = '0';//进入传输
        BYTE  buff[(update_size)];
        u16 state = 0;//状态位(0:正常传输,1:最后一个包)
        u16 cnt = 0;//计数位,最高为255位
        u16 rx_cnt = 0;//接收的数据位
        u32 addrs = 0;
        /***********数据擦写********/
        WDT_CRR =0x76;
        PRINTF_TX = '1';//开始擦写
        while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
        SPIF_FIFO_TOP=0xc7;
        while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
        while(SPIF_STATUS&0x1)WDT_CRR =0x76;//直到写完
        while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
        PRINTF_TX = '2';//结束擦写
        while (1)
        {
        /*****************************数据接收*****************/
        WDT_CRR =0x76;
        VBYTEP buf_p=buff;
        //请求传包
        //PRINTF_TX = 0x05;
        PRINTF_TX = 'A';//本轮开始
        //等待数据接收
        rx_cnt = 0;
        while (rx_cnt < update_size )
        {
            while (!(PRINTF_LSR & UART_LSR_DR))WDT_CRR =0x76;
            buff[rx_cnt] = PRINTF_RX;
            //PRINTF_TX = buff[rx_cnt];//模块测试
            rx_cnt++;
            WDT_CRR =0x76;
        } 
        PRINTF_TX = 'B';//结束传输块
        state = *(u16 *)buf_p;//记录状态位
        buf_p+=update_state_size;
        //PRINTF_TX = state;//模块测试
        u16 data_cnt = *(u16 *)buf_p;
        buf_p+=update_cnt_size;
        //PRINTF_TX = data_cnt;//模块测试
        if(cnt != data_cnt )
        {
            //PRINTF_TX = 0xfe;//报错计数
            PRINTF_TX = 'E';
            PRINTF_TX = cnt;//报错误计数
            while(PRINTF_RX != 0x06)WDT_CRR =0x76;//请求回应
            continue;//退出本次循环
        }
        WDT_CRR =0x76;
        VBYTEP check_p = buf_p;
        buf_p+=update_check_size;
        /*********************************校验码计算比较**************/
        PRINTF_TX = 'C';//正在校验
        if(update_flag&0x01)
        {
        PRINTF_TX = 'C';//进入校验
        VBYTEP check = check_p;//计算
        for(char i=0;i<update_check_size;WDT_CRR =0x76,i++)if(*(check_p+i) != *(check+i) )
        {
            //PRINTF_TX = 0xff;//报错重读
            PRINTF_TX = 'F';
            while(PRINTF_RX != 0x06)WDT_CRR =0x76;
            cnt--;
            continue;//退出本次循环
        }//比较
        }
        WDT_CRR =0x76;
        /**********************数据写入*************************/
        /*************数据写入******/
        PRINTF_TX = 'D';//正在写入
                for(DWORD j=0;j<4;j++)
                {
                u32 addr=(u32)cnt*(u32)update_code_size+j*(u32)256;

                addrs = ((addr&0xFF)<<24)+((addr&0xFF00)<<8)+((addr&0XFF0000)>>8)+0x2;//设置地址
                while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                SPIF_DBYTE=0xff;
                while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                SPIF_FIFO_TOP=addrs;//数据地址
                for(int i = 0;i < 64;i++)
                {
                        while((SPIF_FIFO_CNT&0x3) !=0)WDT_CRR =0x76;//判断数据是否写完
                        SPIF_FIFO_TOP=*(u32 *)(buf_p+j*256+i*4);
                        //while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                        WDT_CRR =0x76;
                }
                while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                while(SPIF_STATUS&0x1)WDT_CRR =0x76;//直到写完
		        while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                }
              /*************数据校验处理******/
        /**********************数据处理*************************/
        WDT_CRR =0x76;
        for(int i=0;i<update_size;i++){buff[i]=0;WDT_CRR =0x76;}
        cnt++;//下一个包的数字
        if(state)break;//判断继续/结束轮询
    }
    PRINTF_TX = 'Z';//结束//等待重启   
    }
    else if (SHAREMEM_UPDATE&&update_mode==0x1)
    {
    PRINTF_TX = '0' ; 
    *((volatile uint8_t *)(SRAM_BASE_ADDR+0X100)) = 0x00;
    u16 i,j;
    uint8_t Update_Addr0,Update_Addr1,Update_Addr2 = 0;
    uint32_t Update_Addr = 0;    
    u8 update_buf[256];
    u8 update_check[update_check_size];
    PRINTF_TX = '1' ;
    while(*((volatile uint8_t *)(SRAM_BASE_ADDR+0X100)) != 0X11)WDT_CRR =0x76;
    while(!(SPIF_READY&0x1))WDT_CRR =0x76;
    SPIF_FIFO_TOP=0xc7;
    while(!(SPIF_READY&0x1))WDT_CRR =0x76;
    while(SPIF_STATUS&0x1)WDT_CRR =0x76;
    while(!(SPIF_READY&0x1))WDT_CRR =0x76;
    *((volatile uint8_t *)(SRAM_BASE_ADDR+0X100)) = 0x22;
    PRINTF_TX = '2' ;
    while(1)
    {
            PRINTF_TX = 'A' ;
            while(*((volatile uint8_t *)(SRAM_BASE_ADDR+0x100)) != 0xAA);
            for(j=0;j<256;j++)
            {
                    update_buf[j] = *((volatile u8 *)(SRAM_BASE_ADDR +0x200 + j));//防止其他设备访问sharemem改变值
            }
            PRINTF_TX = 'B' ;
            *((volatile uint8_t *)(SRAM_BASE_ADDR+0x100)) = 0xBB;
            PRINTF_TX = 'C' ;
            while(*((volatile uint8_t *)(SRAM_BASE_ADDR+0x100)) != 0xCC);
            
            Update_Addr0 = *((volatile uint8_t *)(SRAM_BASE_ADDR+0x101));
            Update_Addr1 = *((volatile uint8_t *)(SRAM_BASE_ADDR+0x102));
            Update_Addr2 = *((volatile uint8_t *)(SRAM_BASE_ADDR+0x103));
            Update_Addr = (uint32_t)(Update_Addr0<<24) + (uint32_t)(Update_Addr1<<16) + (uint32_t)(Update_Addr2<<8) + (uint32_t)(0x2);
            PRINTF_TX = 'D' ;
            *((volatile uint8_t *)(SRAM_BASE_ADDR+0x100)) = 0xDD;
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            SPIF_DBYTE=0xff;
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            SPIF_FIFO_TOP= Update_Addr;
            for(i=0;i<64;i++)
            {
                    while((SPIF_FIFO_CNT&0x3) != 0)WDT_CRR =0x76;
                    SPIF_FIFO_TOP=*((volatile uint32_t *)(update_buf + i*4));
            }
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            while(SPIF_STATUS&0x1)WDT_CRR =0x76;
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;
            //该位置设置为写使能
             while(*((volatile uint8_t *)(SRAM_BASE_ADDR+0x100)) != 0xDD);
            if(*((volatile uint8_t *)(SRAM_BASE_ADDR+0x100)) == 0xFF)break;         
        }
        PRINTF_TX = 'Z' ;   
        *((volatile uint8_t *)(SRAM_BASE_ADDR+0X100)) = 'Z';
    }
    #if FIRMWARECYCTL_UPDATE
    else if (FIRMWARECYCTL_UPDATE&&update_mode==0x2)
    {
        u8 cmd,addr0,addr1,addr2,number,sram,dr0,dr1,dr2,dr3;
        u32 addrs,datas;
        while(1)
        {
            *((volatile uint8_t *)(0x2506))=~(*((volatile uint8_t *)(0x2506)));
            while(*((volatile uint8_t *)(0x2500))==0xfe);//判断设置完成
            cmd=*((volatile uint8_t *)(0x2500));
            addr0=*((volatile uint8_t *)(0x2501));
            addr1=*((volatile uint8_t *)(0x2502));
            addr2=*((volatile uint8_t *)(0x2503));
            dr0=*((volatile uint8_t *)(0x2506));
            dr1=*((volatile uint8_t *)(0x2507));
            dr2=*((volatile uint8_t *)(0x2508));
            dr3=*((volatile uint8_t *)(0x2509));
            addrs=((u32)cmd<<0)+((u32)addr0<<8)+((u32)addr1<<16)+((u32)addr2<<24);
            datas=((u32)dr0<<0)+((u32)dr1<<8)+((u32)dr2<<16)+((u32)dr3<<24);  
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
            while(SPIF_STATUS&0x1)WDT_CRR =0x76;//判断是否被占用
            while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙          
            switch(cmd)//run cmd
            {
                #if 0 
                //as flash cmd--------------------------------------------------------------------------------
                case 0xAB://release power-down/id
                case 0x90://manufacturer/device ID
                case 0x9F://jedec id
                case 0x4B://read unique id
                {
                    break;
                }
                case 0x03://read data
                {
                    u32 i;
                    sram=0x00;
                    flash_write(FLA_DBYTE_ADDR,(u8)(datas&0xff));//读取字节数
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    flash_write(FLA_FIFO_TOP_ADDR,addrs);//写入地址
                    while((SPIF_FIFO_CNT&0x3) == 0);
                    i=(u8)(datas&0xff)/4;
                    if(i*4 < (datas&0xff+1))i++;
                    u32 j=i;
                    for(;i;i--)
                    {
                        while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                        datas=flash_read(FLA_FIFO_TOP_ADDR);
                        *((volatile uint32_t *)(SRAM_BASE_ADDR + sram+(j-i)*4))=datas;
                        *((volatile uint8_t *)(0x2506))=(u8)(datas&0xff);
                        *((volatile uint8_t *)(0x2507))=(u8)((datas&0xff00)>>8);
                        *((volatile uint8_t *)(0x2506))=(u8)((datas&0xff0000)>>16);
                        *((volatile uint8_t *)(0x2506))=(u8)((datas&0xff000000)>>24);
                    }
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    break;
                }
                case 0x0B://fast read
                {
                    break;
                }
                case 0x02://page program
                {
                    u32 i;
                    u8 addr;
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    while(SPIF_STATUS&0x1)WDT_CRR =0x76;//判断是否被占用
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    flash_write(FLA_DBYTE_ADDR,(datas&0xff));//写入数据字节数
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    flash_write(FLA_FIFO_TOP_ADDR,addrs);//写入页地址
                    i=(datas&0xff+1)/4;
                    if(i*4 < (datas&0xff+1))i++;
                    for (;i;i--)
                    {
                        while((SPIF_FIFO_CNT&0x3) >= 2);
                        flash_write(FLA_FIFO_TOP_ADDR,datas);
                        do
                        {
                            addr=*((volatile uint8_t *)(0x2501));
                        }while(addr!=addr0);
                        addr0=addr;
                        dr0=*((volatile uint8_t *)(0x2506));
                        dr1=*((volatile uint8_t *)(0x2507));
                        dr2=*((volatile uint8_t *)(0x2508));
                        dr3=*((volatile uint8_t *)(0x2509));
                        datas=((u32)dr0<<0)+((u32)dr1<<8)+((u32)dr2<<16)+((u32)dr3<<24);
                    }
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    break;
                }
                case 0x5A://read sfdp register
                case 0x42://program security register
                case 0x48://read security register
                case 0x3D://read block lock

                //read status register  
                case 0x05://read status register-1
                case 0x35://read status register-2    
                case 0x15://read status register-3
                {
                    flash_write(FLA_DBYTE_ADDR,0x00);//读取字节数
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    flash_write(FLA_FIFO_TOP_ADDR,cmd);//写入命令
                    while((SPIF_FIFO_CNT&0x3) == 0);
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    datas=flash_read(FLA_FIFO_TOP_ADDR);
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    *((volatile uint32_t *)(SRAM_BASE_ADDR))=datas;
                    *((volatile uint8_t *)(0x2506))=(u8)(datas>>0);
                    *((volatile uint8_t *)(0x2507))=(u8)(datas>>8);
                    *((volatile uint8_t *)(0x2508))=(u8)(datas>>16);
                    *((volatile uint8_t *)(0x2509))=(u8)(datas>>24);
                    *((volatile uint8_t *)(0x250a)) |= 0xa;  //set serirq interrupt status      
                    break;
                }
                //write status register
                case 0x01://write status register-1
                case 0x31://write status register-2
                case 0x11://write status register-3
                {
                    flash_write(FLA_DBYTE_ADDR,0x00);//写入字节数
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    flash_write(FLA_FIFO_TOP_ADDR,cmd);//写入命令
                    while((SPIF_FIFO_CNT&0x3) == 0);
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    flash_write(FLA_FIFO_TOP_ADDR,datas);
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    break;
                }
                #endif
                //only addrs                
                case 0x20://sector erase(4KB)
                case 0x52://block erase(32kb)
                case 0xD8://block erase(64kb)
                //case 0x44://erase security register
                //case 0x36://individual block lock
                //case 0x39://individual block unlock
                {
                    SPIF_DBYTE=0x03;//写入字节数
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    SPIF_FIFO_TOP=addrs;
                    break;
                }
                
                //only cmd 
                case 0x06://write enable
                //case 0x50://volatile SR write enable
                //case 0x04://Write disable
                case 0xc7://chip erase
                case 0x60://chip erase
                //case 0x7E://global block lock
                //case 0x98://global block unlock
                //case 0x75://erase/program suspend
                //case 0x7A://erase/program resume
                //case 0xB9://power-down
                //case 0x66://enable reset
                //case 0x99://reset device
                {
                    SPIF_DBYTE=0x00;//写入字节数
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    SPIF_FIFO_TOP=(u32)cmd;//写入命令与地址  
                    break;
                }
                //as firmware cmd------------------------------------------------------------------------------------
                case 0x12://firmware_cycle_write
                {
                    addrs=addrs&0x0f;
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    while(SPIF_STATUS&0x1)WDT_CRR =0x76;//判断是否被占用
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    SPIF_DBYTE=0x00;
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    SPIF_FIFO_TOP=addrs;
                    while((SPIF_FIFO_CNT&0x3) >= 2);
                    SPIF_FIFO_TOP==datas;
                    break;
                }
                case 0x13://firmware_cycle_read
                {
                    addrs=addrs&0x0f;
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    while(SPIF_STATUS&0x1)WDT_CRR =0x76;//判断是否被占用
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    SPIF_DBYTE=0x00;
                    while(!(SPIF_READY&0x1))WDT_CRR =0x76;//读忙
                    SPIF_FIFO_TOP=addrs;
                    while((SPIF_FIFO_CNT&0x3) == 0);
                    datas=SPIF_FIFO_TOP;
                    *((volatile uint32_t *)(SRAM_BASE_ADDR))=datas;
                    *((volatile uint8_t *)(0x2506))=(u8)(datas>>0);
                    *((volatile uint8_t *)(0x2507))=(u8)(datas>>8);
                    *((volatile uint8_t *)(0x2508))=(u8)(datas>>16);
                    *((volatile uint8_t *)(0x2509))=(u8)(datas>>24);
                    break;
                }
                case 0xff://updaate over
                {
                     /********发送重启指令****/
                    WDT_REG(0x0) = 0x1D;
                    WDT_TORR = 0x0;
                    WDT_CRR =0x5A;
                    while (1)
                    {
                        while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
                        PRINTF_TX = 'z';
                        //for(int i=5000;i;i--)asm("nop");
                    }
                }
                default:
                {
                    break;
                }
            }
            *((volatile uint8_t *)(0x250a)) |= 0xa;  //使能中断位
            while(*((volatile uint8_t *)(0x2500))!=0xfe);//判断接收到执行完成
            while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
            PRINTF_TX = 0x0c;
            while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
            PRINTF_TX = cmd;
            while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
            PRINTF_TX = '\n';
        }
    }
    else 
    {
        PRINTF_TX = 'f';//退出
        return;
    }   
    #endif
    /********发送重启指令****/
    WDT_CR = 0x1D;
    WDT_TORR = 0x0;
    WDT_CRR =0x76;
    int i=1000;
    while (i--)
    {
        while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
        PRINTF_TX = 'z';
        *((volatile uint8_t *)(SRAM_BASE_ADDR+0X100)) = i;
        //for(int i=5000;i;i--)asm("nop");
    }
}
/****************************
 * 函数名：加载funcvv到dram
*****************************/

FUNCT_PTR_V_V Load_funVV_To_Dram(FUNCT_PTR_V_V funcpoint,const int malloc_size)
{
    dprint("Load void Function(VOID) to DRAM!\n");
    FUNCT_PTR_V_V Load_funVV=NULL;
    int i=0;
    Load_funVV = malloc(malloc_size);//申请dram空间
    if(!Load_funVV)return NULL;
    //判断函数偏移位置是否正确位置准确
    Tmp_XPntr =(VBYTE *) Load_funVV;
    Tmp_code_pointer =(VBYTE *) (funcpoint);
    for( i=0; i<malloc_size; i++)
    {
        *Tmp_XPntr=*Tmp_code_pointer;
        Tmp_XPntr++;
        Tmp_code_pointer++;
    }
    return Load_funVV;
}
/****************************
 * 函数名：加载func到dram
*****************************/
void Load_FuncToDram(void)
{
    update_ptr=Load_funVV_To_Dram(EC_Update,0x1000);
    dprint("update_ptr value is 0x%x\n",update_ptr);
}

extern void Disable_Interrupt_Main_Switch(void);
//flash更新程序
void Flash_Update_Function(void)
{
    BYTE mode=0xff;//mode=0x0：串口模式，0x1：sharemem模式,0x2:firmware，0xff无效
    BYTE flag=0x00;//0x0无flag，0x1:开启校验
    if(UART_UPDATE&&uart_updata_flag&0x1)
    {
        mode=0x0;
        if(uart_updata_flag&0x2)flag|=1;
        #ifdef AE103
        void (*pFunc)(BYTE,DWORD); 
        pFunc=( void (*)(BYTE,DWORD))(*((DWORDP)0x1003C));
        (*pFunc)(1,UART0_BASE_ADDR);
        #endif
    }
    #if 1
    else if(SHAREMEM_UPDATE&&!strcmp((const char *)(SRAM_BASE_ADDR+0x100),"update firmware\0"))
    #else
    else if(SHAREMEM_UPDATE&&*((volatile uint8_t *)(+0x100)) == 0xAB)
    #endif
    {
        mode=0x1;        
    }
    #if FIRMWARECYCTL_UPDATE
    else if(FIRMWARECYCTL_UPDATE&&*((volatile uint8_t *)(0x2500)) == 0xFE)
    {
        mode=0x2;
    }
    #endif 
    else 
    {}
    if(mode != 0xff)
    {
        Disable_Interrupt_Main_Switch();
        WDT_TORR=0xffff;//设置最长延时
        WDT_CRR =0x76;//重启计数器
        update_mode=mode;
        update_flag=flag;
        WDT_CR &= !0x02;//不进入wdt中断  
        WDT_CRR =0x76;//重启计数器
        while (!(PRINTF_LSR & UART_LSR_TEMP)) ;/*当此位为空发送fifo写入数据*/
        Load_FuncToDram();//加载func到dram
        if(update_ptr!=0x0)
        (*update_ptr)();//进入更新EC_Update();
    }
}


extern FUNCT_PTR_V_V Load_funVV_To_Dram(FUNCT_PTR_V_V funcpoint,const int malloc_size);
extern void Load_FuncToDram(void);
extern void Flash_Update_Function(void);