/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:30:35
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

#include<KERNEL_INCLUDE.H>
#include<AE_INCLUDE.H>

//tm1803寄存器初始化
void OWI_TM1803_Init(u8 One_Len, u8 Zero_Len)
{
    OWI_TM1803_CTRL |= Zero_Len;
    OWI_TM1803_CTRL |= One_Len << 8;
} 

//general寄存器初始化。注意：参数设置实际多少传入多少，内部已经处理。
void OWI_GENERAL_Init(u8 len, u8 cycle_num_clk)
{
    OWI_GENERAL_CTRL |= ((len-1) << 3);
    OWI_GENERAL_CTRL |= ((cycle_num_clk-1) << 15);
}

//OWI模块初始化
void Owi_Init(void)
{
    SYSCTL_CLKDIV_PCLK = 0x0;       //时钟1分频,分频数为0base

    #if (SYSCTL_CLOCK_EN)
    SYSCTL_MODEN1 |= 0x1 <<17;     //使能OWI
    #endif  
    
    sysctl_iomux_config(GPIOA,29,1);
    sysctl_iomux_config(GPIOB,5,1);     //复用引脚；
    
    OWI_TM1803_Init(20,10);
    OWI_GENERAL_Init(128,30); 
}

//灯带数据初始化
void Owi_Data_Ctrl(void)
{
    for (int i = 0; i < 5; i++) {
        unsigned int address = 0x32900UL + i*4;
        unsigned int data = 0x1f0000;  // 只保留低 24 位数据
        *(volatile unsigned int *)address = 0x1f0000;
    }
}
//开始发送数据
void Owi_SendData(void)
{
    //OWI_GENERAL_CTRL &= (~(0x1 <<24));
    Owi_Data_Ctrl();
    OWI_GENERAL_CTRL |= (0x1000004);

}

