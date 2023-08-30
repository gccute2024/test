/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:29:24
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

//#include "KERNEL_INCLUDE.H"
#include "KERNEL_ADC.H"

/**
 * @brief ADC触发模式选择
 *
 * @param mode 0软件 1硬件 2连续
 *
 * @return             无
 */
void ADC__TriggerMode_Config(uint8_t mode)
{
      switch (mode)
      {
      case 0:
            ADC_CTRL&=(~(0x3<<7)); //软件触发
            break;
      case 1:
            ADC_CTRL|=(0x1<<7);
            ADC_CTRL&=(~(0x1<<8)); //硬件触发
            break;
      case 2:
            ADC_CTRL|=(0x1<<8);
            ADC_CTRL&=(~(0x1<<7)); //连续触发
            break;
      default:
            break;
      }
}

/**
 * @brief 单端or差分模式选择
 *
 * @param mode 0单端  1差分
 *
 * @return             无
 */
void ADC_SampleMode_Config(uint8_t mode)
{
      switch (mode)
      {
      case 0:
            ADC_CTRL&=(~(0x1<<4));
            break;
      case 1:
            ADC_CTRL|=(0x1<<4);
            break;
      default:
            break;
      }
}

/**
 * @brief ADC通道使能
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param sw DISABLE  ENABLE
 *
 * @return             无
 */
void ADC_Channelx_Config(uint8_t channelx,uint8_t sw)
{
      if(sw==DISABLE)
      {
            ADC_CHAN_EN &=(~(0x1<<channelx));
      }
      else
      {
            ADC_CHAN_EN |=(0x1<<channelx);
      }
}

/**
 * @brief ADC中断使能or屏蔽
 *
 * @param channelx irq_type：uv_irq ov_irq af_irq
 * @param sw DISABLE  ENABLE
 *
 * @return             无
 */
void ADC_IRQ_Config(uint8_t irq_type,uint8_t sw)
{
      
      if(sw==DISABLE)
      {
            ADC_INTMASK |=(irq_type);
      }
      else
      {
            ADC_INTMASK &=(~(irq_type));
      }

}

/**
 * @brief ADC清除中断状态位
 *
 * @param channelx irq_type：uv_irq ov_irq 
 *
 * @return             无
 */
void ADC_Clear_IRQ(uint8_t irq_type)
{
      ADC_INTSTAT|=irq_type;
}

/**
 * @brief 检查ADC采样是否完成
 *
 * @return 采样到有效数据返回1，否则返回0
 */
uint8_t ADC_Ready(void)
{
      uint8_t adc_ready_state=0;
      adc_ready_state=ADC_READY;
      if(adc_ready_state&0x1)
      {
            return 1;
      }
      else
      {
            return 0;
      }
}

/**
 * @brief 配置定时器计数器值
 *
 * @param timercount 定时器计数器值
 *
 * @return 无
 *
 * @note 函数只在配置为硬件触发时，当 timercount 递减到 0 时才进行采样
 */
void ADC_Timercount_Set(uint32_t timercount)
{
      ADC_CNT0=(timercount);
      ADC_CNT1=(timercount>>16);
}

/**
 * @brief ADC初始化函数
 *
 * @param channelx 通道选择，取值范围为 0~11
 * @param trigger 触发模式选择，0表示软件触发，1表示硬件触发，2表示连续模式
 * @param mode 单端或差分模式选择，0表示单端，1表示差分
 *
 * @return 无
 *
 * @note 此函数用于初始化ADC模块，包括通道选择、触发模式选择和单端/差分模式选择等。
 */
void ADC_Init(uint8_t channelx,uint8_t trigger,uint8_t mode)
{
      REG32(0x30418)=0x8E30000; //PLL控制器
      ADC_CTRL=0x03;            //配置为原码 无衰减 fifo满阈值为3
      ADC_PM=0x7;               //关闭低功耗
      ADC_IRQ_Config(uv_irq|ov_irq|af_irq,DISABLE);//屏蔽所有中断
      ADC__TriggerMode_Config(trigger);         //触发模式选择
      ADC_SampleMode_Config(mode);              //单端or差分模式选择
      ADC_Channelx_Config(channelx,ENABLE);     //通道使能
}

/**
 * @brief 触发ADC采样操作
 *
 * @param 无
 *
 * @return 无
 *
 * @note 此函数用于触发ADC进行采样操作。
 */
void ADC_Trigger_operation(void)
{
      ADC_TRIGGER=0x1;
}

/**
 * @brief 读取ADC采集到的值
 *
 * @param 无
 *
 * @return 返回ADC采集到的短整型数据
 *
 * @note 此函数用于读取ADC采集到的值。
 */
short ADC_ReadData(void)
{
      uint16_t Data=0;
      Data=ADC_DATA;
      return Data;
}

/**
 * @brief 在软件模式下进行ADC采样
 *
 * @param 无
 *
 * @return 返回ADC采集到的短整型数据
 *
 * @note 此函数用于在软件模式下触发ADC进行采样操作，并返回采集到的数据。
 */
short ADC_SW_Sample(void)
{
      ADC_TRIGGER=0x1;
      while(!ADC_Ready());
      return ADC_DATA;
}

