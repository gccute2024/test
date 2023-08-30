/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-04 15:09:18
 * @Description: This file is used for SPI Interface
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

#include "KERNEL_SPI.H"
#include "KERNEL_TIMER.H"

/*------------------------------SPIM Interface Code-----------------------------*/
BYTE SPI_Write_Buff[256]={0};
struct SPI_INS
{
	uint8_t flash_id_cmd[4];
    uint8_t flash_busy_cmd[2];
    uint8_t flash_suspend_cmd[2];
    uint8_t flash_write_en_cmd;
    uint8_t flash_sector_erase_cmd[4];
    uint8_t flash_chip_erase_cmd;
    uint8_t flash_read_data_cmd[5];
    uint8_t flash_page_program_cmd[5];
    uint8_t flash_reset_cmd;
} spi_instruction = {
    {0x9f,0xff,0xff,0xff},
    {0x5, 0xff},
    {0x35,0xff},
    0x6,
    {0x20,0x0, 0x0, 0x0},
    0xc7,
    {0x3, 0x0, 0x0, 0x0, 0xff},
    {0x2, 0x0, 0x0, 0x0, 0xaf},
    0x99
};

/**
 * @brief 设置SPI中断屏蔽和开关
 *
 * @param irq_mask 中断屏蔽掩码，可选值为：txeim, txoim, rxuim, rxoim, rxfim
 * @param sw 开关控制，可选值为：ENABLE, DISABLE
 *
 * @return 无
 *
 * @note 此函数用于设置SPI中断屏蔽和开关，可选择的中断屏蔽掩码包括：TXEIM - 发送缓冲区为空中断屏蔽，
 *       TXOIM - 发送上溢中断屏蔽，RXUIM - 接收缓冲区下溢中断屏蔽，RXOIM - 接收溢出中断屏蔽，
 *       RXFIM - 接收缓冲区满中断屏蔽。开关控制可选值包括：ENABLE - 使能中断，DISABLE - 禁止中断。
 */
void SPI_IRQ_Config(uint8_t irq_mask,uint8_t sw)
{
    if(sw==0)
    {
        SPIM_IMSR|=irq_mask;
    }
    else
    {
        SPIM_IMSR&=(~irq_mask);
    }
}

/**
 * @brief SPI初始化
 *
 * @return 无
 *
 * @note 此函数用于SPI初始化。
 */ 
void SPI_Init(void)
{   
    #if !(SPIM_CLOCK_EN)
	return;
    #endif
    SPIM_CPSR=0x1;         //时钟四分频
    SPIM_CTRL0=0x4;        //不使用DMA，数据延迟为0 使能SPIM mode：0
    SPIM_CTRL1=0x67;       //低位先传 数据位数：8位
    SPIM_TXFTLR=0x1;        //发送fifo为1
    SPIM_RXFTLR=0x1;        //接收fifo为1
    SPI_IRQ_Config(txeim|txoim|rxuim|rxoim|rxfim,DISABLE); //屏蔽所有中断
}

/**
 * @brief 发送一字节数据
 *
 * @param send_data 要发送的数据
 *
 * @return 接收到的数据
 *
 * @note 此函数用于向SPI总线发送一字节数据并接收一字节数据。
 */ 
uint8_t SPI_Send_Byte(uint8_t send_data)
{
    SPI_Timeout=100000;
	while (!((SPIM_SR)&0x2))    //等待发送fifo空
    {
        SPI_Timeout--;
        if(SPI_Timeout==0)
        {
            dprint("spi等待超时\n");
            return 0;
        }
    }
    SPIM_DA0=send_data;        //发送数据
    SPI_Timeout=100000;
	while (!((SPIM_SR)&0x4))   //等待接收fifo不空
    {
        SPI_Timeout--;
        if(SPI_Timeout==0)
        {
            dprint("spi等待超时\n");
            return 0;
        }
    }
	return(SPIM_DA0);
}

/**
 * @brief 读取一字节数据
 *
 * @return 接收到的数据
 *
 * @note 此函数用于从SPI总线读取一字节数据。
 */ 
uint8_t SPI_Read_Byte(void)
{
    return SPI_Send_Byte(0xff);
}

/**
 * @brief 拉低片选
 *
 * @param cs_select 片选选择，可选值为0或1
 *
 * @return 无
 *
 * @note 此函数用于拉低SPI Flash的片选信号，使其处于选中状态。
 */ 
void SPI_Flash_CS_Low(uint8_t cs_select)
{
    if(cs_select == 0)
    {
        SPIM_CSN0=0x0;
        if(SPIM_CSN0 != 0x0)
        	dprint("CS0 select failed!\n");
        else if(SPIM_CSN0 == 0x0)
        	dprint("CS0 select success!\n");
    }
    else if(cs_select == 1)
    {
		SPIM_CSN1=0x0;
        if(SPIM_CSN1 != 0x0)
        	dprint("CS1 select failed!\n");
        else if(SPIM_CSN1 == 0x0)
        dprint("CS1 select success!\n");
    }
}

/**
 * @brief 拉高片选信号
 *
 * @param cs_select 片选信号选择，可选值为0或1
 *
 * @return 无
 *
 * @note 此函数用于拉高SPI Flash的片选信号，使其处于未选中状态。
 */
void SPI_Flash_CS_High(uint8_t cs_select)
{
    if(cs_select == 0)
    {
        SPIM_CSN0=0x1;

        if(SPIM_CSN0 != 0x1)
        	dprint("CS0 free failed!\n");
        else if(SPIM_CSN0 == 0x1)
        	dprint("CS0 free success!\n");
    }
    else if(cs_select == 1)
    {
        SPIM_CSN1=0x1;

        if(SPIM_CSN1 != 0x1)
        	dprint("CS1 free failed!\n");
        else if(SPIM_CSN1 == 0x1)
        	dprint("CS1 free success!\n");
    }
}

/**
 * @brief 读取SPI Flash0的ID信息
 *
 * @param 无
 *
 * @return 无
 *
 * @note 此函数用于读取SPI Flash0的ID信息，以便进行芯片型号和厂商的识别。
 */
void SPI_Read_ID0(void)
{
    SPI_Flash_CS_Low(0);
    vDelayXms(5);
    SPI_Send_Byte(CMD_READ_ID);  //发送读取flash id 命令(24位id)
    SPI_FLAHS_ID|=SPI_Send_Byte(0xff);
    SPI_FLAHS_ID<<=8;
    SPI_FLAHS_ID|=SPI_Send_Byte(0xff);
    SPI_FLAHS_ID<<=8;
    SPI_FLAHS_ID|=SPI_Send_Byte(0xff);
    vDelayXms(5);
    SPI_Flash_CS_High(0);
    dprint("flash id:0x%x\n",SPI_FLAHS_ID);
    

}

//读取id1
void SPI_Read_ID1(void)
{
	SPI_Flash_CS_Low(1);
    vDelayXms(5);
    SPI_Send_Byte(CMD_READ_ID);  //发送读取flash id 命令(24位id)
    SPI_FLAHS_ID|=SPI_Send_Byte(0xff);
    SPI_FLAHS_ID<<=8;
    SPI_FLAHS_ID|=SPI_Send_Byte(0xff);
    SPI_FLAHS_ID<<=8;
    SPI_FLAHS_ID|=SPI_Send_Byte(0xff);
    vDelayXms(5);
    SPI_Flash_CS_High(1);
    dprint("flash id:0x%x\n",SPI_FLAHS_ID);
    
}

//发送命令函数
uint8_t SPI_Send_Cmd(uint8_t cmd, uint8_t cs_select)
{
    SPI_WriteDATA=0;
 
    SPI_Flash_CS_Low(cs_select);
        
    SPI_WriteDATA = SPI_Send_Byte(cmd);
    SPI_WriteDATA = SPI_Send_Byte(0xFF);

    SPI_Flash_CS_High(cs_select);
     
    return SPI_WriteDATA;        
}

/**
 * @brief 判断flash是否忙碌
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 返回SPI控制器的状态，0表示空闲，1表示忙碌
 *
 * @note 此函数用于判断指定SPI控制器选中的flash是否忙碌
 */ 
uint8_t SPI_Busy(uint8_t cs_select)
{
    FLASH_SR=0;

    FLASH_SR = SPI_Send_Cmd(CMD_READ_STATUS1, cs_select);

    dprint("spi flash status is 0x%x\n", FLASH_SR);

    return (FLASH_SR & 0x1);
}

/**
 * @brief 等待flash空闲
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 无
 *
 */
void SPI_WaitnoBUSY(uint8_t cs_select)
{
    FLASH_SR=0;
    SPI_Flash_CS_Low(cs_select);
	SPI_Send_Byte(CMD_READ_STATUS1);
	do
	{
	    FLASH_SR=SPI_Send_Byte(0xff);
	}while((FLASH_SR&0x01)==1);
    SPI_Flash_CS_High(cs_select);
}

/**
 * @brief 判断SPI Flash是否处于挂起状态
 *
 * @param cs_select SPI控制器选择，可选值为0或1
 *
 * @return 返回值为0表示Flash未挂起，返回1表示Flash已挂起
 *
 * @note 此函数用于判断指定SPI控制器上的Flash是否处于挂起状态。当Flash在进行扇区擦除或
 *       编程操作时，若另一次操作的写入或读取命令到达，Flash将暂停当前操作并执行新命令，
 *       此时Flash处于挂起状态。通过本函数，可以判断Flash是否处于挂起状态，以便进行后续操作。
 */
uint8_t SPI_Suspend(uint8_t cs_select)
{
    FLASH_SR=0;
        
    FLASH_SR = SPI_Send_Cmd(CMD_READ_STATUS2, cs_select);

    dprint("spi flash status2 is 0x%x\n", FLASH_SR);

    return (FLASH_SR & 0x8);
}

/**
 * @brief SPI写使能
 *
 * @param cs_select 选择哪个SPI控制器，可选值为0或1
 *
 * @return 无
 *
 * @note 
 *  此函数用于SPI写使能，即向SPI Flash发送写使能命令。
 */ 
void SPI_Write_Enable(uint8_t cs_select)
{
    SPI_Flash_CS_Low(cs_select);
    SPI_Send_Byte(CMD_WRITE_ENABLE);
    SPI_Flash_CS_High(cs_select);
}

/**
 * @brief 读取flash的某个地址的内容
 * 
 * @param pbuffer 存储读取到的内容的地址
 * @param read_addr 读取的起始地址
 * @param num_byte_to_read 读取的字节数
 * @param cs_select 选择的SPI控制器
 * 
 * @return 无
 * 
 * @note 读取到的内容存储到pbuffer地址，读取num_byte_to_read个字节
 */
void SPI_Send_Cmd_Addr(uint8_t* pbuffer, uint32_t read_addr, uint16_t num_byte_to_read, uint8_t cs_select)
{
    BYTE  data;

    dprint("spi read flash start!\n");

    SPI_Flash_CS_Low(cs_select);
	
    SPI_Send_Byte(CMD_READ_DATA);            //读命令

    SPI_Send_Byte((read_addr & 0xFF0000) >> 16);
    SPI_Send_Byte((read_addr & 0xFF00) >> 8);
    SPI_Send_Byte(read_addr & 0xFF);

    while(num_byte_to_read--)
    {
        *pbuffer = SPI_Read_Byte();
        pbuffer++;
    }
    
    SPI_Flash_CS_High(cs_select);
        
    //while(SPI_Busy(0));
    dprint("read flash data completed!\n");        
}

/**
 * @brief  擦除SPI Flash所选扇区（大小为4KB） 
 * @param  address: 需要擦除的扇区地址
 * @param  cs_select: 选择SPI控制器
 * @retval 无
 */
void SPI_Block_Erase(uint32_t address, uint8_t cs_select)
{
    dprint("block erase start!\n");
        
    SPI_Write_Enable(cs_select);  //擦除扇区之前要执行写启用指令,位置必须在CS为0之前
       
    SPI_Flash_CS_Low(cs_select);
        
    SPI_Send_Byte(CMD_BLOCK_ERASE_64);

    SPI_Send_Byte((address & 0xFF0000) >> 16);
    SPI_Send_Byte((address & 0xFF00) >> 8);
    SPI_Send_Byte(address & 0xFF);
          
    SPI_Flash_CS_High(cs_select);
 
    SPI_WaitnoBUSY(cs_select);
        
    dprint("block erase completed!\n");
}

void SPI_Chip_Erase(uint8_t cs_select)
{
    dprint("chip erase start!\n");

    //Write Enable must be executed first
    SPI_Write_Enable(0);

    SPI_Flash_CS_Low(cs_select);

    //send chip erase cmd
    SPI_Send_Byte(CMD_CHIP_ERASE);

    SPI_Flash_CS_High(cs_select);

    while (SPI_Busy(cs_select));

    dprint("chip erase completed!\n");
}

/**
 * @brief 页写入函数
 * @param  write_addr: 写入的地址
 *         byte_num: 写入多少字节
 *         byte_data: 写入的数据指针
 *         cs_select: 选择哪个片选
 * @return None
 */
void SPI_Page_Program(uint32_t write_addr, uint32_t byte_num, uint8_t* byte_data, uint8_t cs_select)
{
    dprint("flash page program start!\n");

    //Write Enable must be executed first
    SPI_Write_Enable(cs_select);

    SPI_Flash_CS_Low(cs_select);

    //send page program cmd
    SPI_Send_Byte(CMD_PAGE_PROGRAM);            //Page Program 命令

    //send page program start address
    SPI_Send_Byte((write_addr & 0xFF0000) >> 16);
    SPI_Send_Byte((write_addr & 0xFF00) >> 8);
    SPI_Send_Byte(write_addr & 0xFF);
 
    //send data
    while(byte_num--)
    {
        SPI_Send_Byte(*byte_data);
        byte_data++;
    }
        

    SPI_Flash_CS_High(cs_select);
        
    while (SPI_Busy(cs_select));

    dprint("chip page program completed!\n");
}

/**
 * @brief 对SPI Flash进行复位
 * 
 * @param cs_select 选择哪个SPI控制器
 * 
 * @return None
 */
void SPI_Flash_Reset(uint8_t cs_select)
{
	dprint("spi flash reset start!\n");
            
    while (SPI_Busy(cs_select) | SPI_Suspend(cs_select));
 
    SPI_Send_Cmd(0x66, cs_select);
          
    SPI_Send_Cmd(0x99, cs_select);

    dprint("spi flash reset finish!\n");
}

/**
 * @brief 发送读取数据命令
 * 
 * @param cmd 发送的读还是写命令
 * @param read_addr 读取的起始地址
 * @param cs_select 选择的SPI控制器
 * 
 * @return 无
 * 
 * @note 这个函数配合中断使用，比如发送读命令，然后再使能SPI_IRQ_Config(rxfim,ENABLE) 接收满中断
 * 再发送SPIM_DA0=0xff这条命令进入中断中读取数据
 */
void SPI_Send_RWcmd(uint8_t cmd,uint32_t read_addr,uint8_t cs_select)
{
    SPI_Flash_CS_Low(cs_select);
    SPI_Send_Byte(cmd);            
    SPI_Send_Byte((read_addr & 0xFF0000) >> 16);
    SPI_Send_Byte((read_addr & 0xFF00) >> 8);
    SPI_Send_Byte(read_addr & 0xFF);
    SPI_IRQ_Config(rxfim,ENABLE);
    dprint("read flash data completed!\n");        
}

//进行读写擦除测试
void SPI_Flash_Test(void)
{
    int i;
    uint8_t read_buff[256] = {0};
    uint8_t write_buff[256] = {0x1,0x2,0x3,0x4,0x5,0x6};
 
    /**************CSN0***************/
    //SPI_Chip_Erase(0);
    SPI_Block_Erase(0x0, 0);
    SPI_Send_Cmd_Addr(read_buff, 0, 256, 0);
	
    for(i=0;i<256;i++)
    {
        dprint("read data buff is 0x%x\n", read_buff[i]);
    }

	SPI_Page_Program(0, 256, write_buff, 0);
    SPI_Send_Cmd_Addr(read_buff, 0, 256, 0); 

    for(i=0;i<256;i++)
	{
        dprint("read data buff is 0x%x\n", read_buff[i]);
    }
    /**************CSN0***************/
        
    /**************CSN1***************/
    //SPI_Chip_Erase(1);
    /*SPI_Block_Erase(0x0, 1);
    SPI_Send_Cmd_Addr(read_buff, 0, 256, 1);

    for(i=0;i<256;i++)
    {
        dprint("read data buff is 0x%x\n", read_buff[i]);
    }

    SPI_Page_Program(0, 256, write_buff, 1);
    SPI_Send_Cmd_Addr(read_buff, 0, 256, 1);

    for(i=0;i<256;i++)
    {
    	dprint("read data buff is 0x%x\n", read_buff[i]);
    }*/

}


