/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-28 12:42:46
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

#include "KERNEL_GPIO.H"
#include "AE_FUNC.H"
#include "AE_GLOBAL.H"

struct GPIOPinState gpio_state_table[] =
{
	// GPIOA
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 0, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 0, 0}},
	{0, {1, 1, 0, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {0, 0, 0, 0}},//23
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 0, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	// GPIOB
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 0}},
	{0, {1, 1, 1, 0}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 0}},
	{0, {1, 0, 1, 0}},
	{3, {1, 1, 1, 1}},
	{3, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 0, 0}},
	{0, {1, 0, 0, 0}},
	{0, {1, 1, 1, 1}},
	{0, {1, 1, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 1, 1, 1}},
	// GPIOC
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 1, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 1, 1, 0}},
	{0, {1, 0, 1, 0}},
	{0, {1, 1, 0, 1}},
	{0, {1, 0, 0, 1}},
	{0, {1, 0, 0, 0}},
	{0, {1, 1, 0, 0}},
	// GPIOD
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	// GPIOE
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 1, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 0, 1}},
	{1, {1, 0, 1, 0}},
	{1, {1, 0, 1, 0}},
	{1, {1, 0, 1, 0}},
	{1, {1, 0, 1, 0}},
};

//*****************************************************************************
//
//  Reserved
//
//  parameter :
//      module : 
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_mod_disable(DWORD module)
{
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_MODEN0 &= (~(1 << (module % 32)));
	#endif
}

void sysctl_mod_enable(DWORD module)
{
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_MODEN0 |= (1 << (module % 32));
	#endif
}

void sysctl_mod_reset_start(DWORD module)
{
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_RST0 |= (1 << (module % 32));
	#endif
}

void sysctl_mod_reset_finish(DWORD module)
{
	#if (SYSCTL_CLOCK_EN)
	SYSCTL_RST0 &= (~(1 << (module % 32)));
	#endif
}

void enable_necessary_input(DWORD port, DWORD io, unsigned port_type)
{
	int idx = 0;
	if (port == GPIOA) 
	{
		idx = io;
	}
	else if (port == GPIOB)
	{
		idx = NUM_OF_GPIOA + io;
	}
	else if (port == GPIOC)
	{
		idx = NUM_OF_GPIOA + NUM_OF_GPIOB + io;
	}
	else if (port == GPIOD)
	{
		idx = NUM_OF_GPIOA + NUM_OF_GPIOB + NUM_OF_GPIOC + io;
	}
	else if (port == GPIOE)
	{
		idx = NUM_OF_GPIOA + NUM_OF_GPIOB + NUM_OF_GPIOC + NUM_OF_GPIOD + io;
	}
	int state = gpio_state_table[idx].mux_state[port_type];
	if (state)
	{
		GPIO_Input_EN(port, io, DISABLE);
	}
	else
	{
		GPIO_Input_EN(port, io, ENABLE);
	}
	return;
}

/**
 * @brief GPIO引脚复用功能配置
 *
 * @param    port     		AE102可选参数：GPIOA ,GPIOB...GPIOE
 *               
 * @param    io      		GPIO序列号选择如：GPIOA10 
 *
 * @param    port_type      复用功能选择：可选参数0-3
 *
 * @return   无
 */
void sysctl_iomux_config(DWORD port, DWORD io, unsigned port_type)
{
//if (io != 24 && io != 25)
//{
//}
	#if SUPPORT_LOWPOWER
	enable_necessary_input(port, io, port_type);
	#endif

	// #if (defined(AE101)||defined(AE102))
    DWORD addr;
    DWORD  cfg_val;
    if(port<=3)
	{
		addr = SYSCTL_BASE_ADDR + SYSCTL_PIO0_CFG_OFFSET + ((port-1) * 0x8);
		if(io<=15)
		{
			cfg_val = *(DWORDP )addr; 
    		cfg_val &= ~(3 << (io * 2));
    		cfg_val |= port_type << (io * 2); 
			*(DWORDP )addr = cfg_val;
		}
		else if (io<=31)
		{
			cfg_val = *(DWORDP )(addr+0x4); 
			io=(io-16);
    		cfg_val &= ~(3 << (io * 2));
    		cfg_val |= port_type << (io * 2); 
			*(DWORDP )(addr+0x4) = cfg_val;
		}
		else
		{
			dprint("GPIO  number input error\n");
		}
	}
	else
	{
		addr = (SYSCTL_BASE_ADDR + SYSCTL_PIO0_CFG_OFFSET + 0x14);
		if(port==4)
		{
			cfg_val = *(DWORDP )addr; 
    		cfg_val &= ~(3 << (io * 2));
    		cfg_val |= port_type << (io * 2); 
			*(DWORDP )addr = cfg_val;
		}
		else if(port==5)
		{
			cfg_val = *(DWORDP )addr; 
    		cfg_val &= ~(3 << 14);
    		cfg_val |= port_type << 14; 
			*(DWORDP )addr = cfg_val;
		}
		else
		{
			dprint("GPIO group number input error\n");
		}
	}
    
	// #endif
}

//*****************************************************************************
//
//  To setup uart0 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart0()
{
	sysctl_iomux_config(GPIOA,24,2);
	sysctl_iomux_config(GPIOA,25,2);
}

//*****************************************************************************
//
//  To setup uart1 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart1()
{
	#if (defined(AE102)||defined(AE103))
	sysctl_iomux_config(GPIOB,1,1);
	sysctl_iomux_config(GPIOB,3,1);
	#endif
}

//*****************************************************************************
//
//  To setup uart2 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart2()
{
	#if (defined(AE101)||defined(AE102))
	sysctl_iomux_config(GPIOB,23,1);
	sysctl_iomux_config(GPIOB,25,1);
	#endif
	#ifdef AE103
	sysctl_iomux_config(GPIOB,27,1);
	sysctl_iomux_config(GPIOB,28,1);
	sysctl_iomux_config(GPIOB,29,1);
	#endif
}


//*****************************************************************************
//
//  To setup uart3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uart3()
{
	#if (defined(AE101)||defined(AE102))
	sysctl_iomux_config(GPIOA,15,2);
	sysctl_iomux_config(GPIOC,11,1);
	#endif
	#ifdef AE103
	sysctl_iomux_config(GPIOA,10,2);
	sysctl_iomux_config(GPIOC,11,1);
	#endif
}

//*****************************************************************************
//
//  To setup uarta iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uarta()
{
	sysctl_iomux_config(GPIOA,8,2);
	sysctl_iomux_config(GPIOA,9,2);
}

//*****************************************************************************
//
//  To setup uartb iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_uartb()
{
	#if (defined(AE101)||defined(AE102))
	sysctl_iomux_config(GPIOB,21,2);
	sysctl_iomux_config(GPIOB,22,2);
	#endif

	#ifdef AE103
	sysctl_iomux_config(GPIOB,25,2);
	sysctl_iomux_config(GPIOB,26,2);
	#endif
}

//*****************************************************************************
//
//  To setup ps2 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_ps2()
{
	
	sysctl_iomux_config(GPIOB,8,1);    
	sysctl_iomux_config(GPIOB,9,1);

	sysctl_iomux_config(GPIOB,12,1);    
	sysctl_iomux_config(GPIOB,13,1);

}

//*****************************************************************************
//
//  To setup spim iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_spim()
{
	sysctl_iomux_config(GPIOA,6,2); // spim_sck 
	sysctl_iomux_config(GPIOA,19,2); // spim_mosi
	sysctl_iomux_config(GPIOA,21,2); //spim_miso
	sysctl_iomux_config(GPIOB,16,2); // csn1
	sysctl_iomux_config(GPIOB,18,2); // csn0
}


//*****************************************************************************
//
//  To setup pwm iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_pwm()
{
	sysctl_iomux_config(GPIOA,0,1); //pwm0
	sysctl_iomux_config(GPIOA,1,1); //pwm1
	sysctl_iomux_config(GPIOA,2,1); //pwm2
	sysctl_iomux_config(GPIOA,3,1); //pwm3
	sysctl_iomux_config(GPIOA,4,1); //pwm4
	sysctl_iomux_config(GPIOA,5,1); //pwm5
	sysctl_iomux_config(GPIOA,6,1); //pwm6
    sysctl_iomux_config(GPIOA,7,1); //pwm7
}

//*****************************************************************************
//
//  To setup pwm0-7 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_pwm0()
{
	sysctl_iomux_config(GPIOA,0,1);
}
void sysctl_iomux_pwm1()
{
	sysctl_iomux_config(GPIOA,1,1);
}
void sysctl_iomux_pwm2()
{
	sysctl_iomux_config(GPIOA,2,1);
}
void sysctl_iomux_pwm3()
{
	sysctl_iomux_config(GPIOA,3,1);
}
void sysctl_iomux_pwm4()
{
	sysctl_iomux_config(GPIOA,4,1);
}
void sysctl_iomux_pwm5()
{
	sysctl_iomux_config(GPIOA,5,1);
}
void sysctl_iomux_pwm6()
{
	sysctl_iomux_config(GPIOA,6,1);
}
void sysctl_iomux_pwm7()
{
	sysctl_iomux_config(GPIOA,7,1);
}


//*****************************************************************************
//
//  To setup tach iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_tach()
{
	sysctl_iomux_config(GPIOA,30,1); //tach0
	sysctl_iomux_config(GPIOA,31,1); //tach1
	sysctl_iomux_config(GPIOC,10,2); //tach2
	sysctl_iomux_config(GPIOC,11,2); //tach3
}

//*****************************************************************************
//
//  To setup tach0-3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_tach0()
{
	sysctl_iomux_config(GPIOA,30,1); //tach0
}
void sysctl_iomux_tach1()
{
	sysctl_iomux_config(GPIOA,31,1); //tach1
}
void sysctl_iomux_tach2()
{
	sysctl_iomux_config(GPIOC,10,2); //tach2
}
void sysctl_iomux_tach3()
{
	sysctl_iomux_config(GPIOC,11,2); //tach3
}

//*****************************************************************************
//
//  To setup i2c iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_i2c()
{
	#if SMBUS0_CLOCK_EN
	sysctl_iomux_config(GPIOA,11,1);
	sysctl_iomux_config(GPIOA,12,1);
	#endif

	#if SMBUS1_CLOCK_EN
	sysctl_iomux_config(GPIOA,17,1);
	sysctl_iomux_config(GPIOA,18,1);
	#endif

	#if SMBUS2_CLOCK_EN
	sysctl_iomux_config(GPIOB,14,1);
	sysctl_iomux_config(GPIOB,15,1);
	#endif

	#if (defined(TEST101)||defined(AE101)||defined(AE102))
		#if SMBUS3_CLOCK_EN
		sysctl_iomux_config(GPIOB,21,2);
		sysctl_iomux_config(GPIOB,22,2);
		#endif
	#elif (defined(AE103))
		#if SMBUS3_CLOCK_EN
		sysctl_iomux_config(GPIOB,25,2);
		sysctl_iomux_config(GPIOB,26,2);
		#endif
//SMBUS4
		sysctl_iomux_config(GPIOA,4,3);
		sysctl_iomux_config(GPIOA,5,3);	
//SMBUS5	
		sysctl_iomux_config(GPIOB,0,2);
		sysctl_iomux_config(GPIOB,7,2);
	#endif
}

//*****************************************************************************
//
//  To setup i2c0-3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_i2c0()
{
	sysctl_iomux_config(GPIOA,11,1);
	sysctl_iomux_config(GPIOA,12,1);
}
void sysctl_iomux_i2c1()
{
	sysctl_iomux_config(GPIOA,17,1);   
	sysctl_iomux_config(GPIOA,18,1);
}
void sysctl_iomux_i2c2()
{
	sysctl_iomux_config(GPIOB,14,1);
	sysctl_iomux_config(GPIOB,15,1);
}
void sysctl_iomux_i2c3()
{
	sysctl_iomux_config(GPIOB,21,2);
	sysctl_iomux_config(GPIOB,22,2);
}

//*****************************************************************************
//
//  To setup pmc iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_pmc()
{
	sysctl_iomux_config(GPIOA,27,1);
	sysctl_iomux_config(GPIOA,28,1);
}

//*****************************************************************************
//
//  To setup adc iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_adc()
{
	sysctl_iomux_config(GPIOC,0,1);
	sysctl_iomux_config(GPIOC,1,1);
	sysctl_iomux_config(GPIOC,2,1);
	sysctl_iomux_config(GPIOC,3,1);
	sysctl_iomux_config(GPIOC,4,1);
	sysctl_iomux_config(GPIOC,5,1);
	sysctl_iomux_config(GPIOC,6,1);
	sysctl_iomux_config(GPIOC,7,1);
}


//*****************************************************************************
//
//  To setup cec iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_cec()
{
	sysctl_iomux_config(GPIOB,8,2);
}
//*****************************************************************************
//
//  To setup can iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_can(void)
{
	#ifdef AE101
	return;
	#endif
    sysctl_iomux_config(GPIOA,13,1);            //CAN0 tx
    sysctl_iomux_config(GPIOA,14,1);            //CAN0 rx
    sysctl_iomux_config(GPIOA,4,2);             //CAN1 tx
    sysctl_iomux_config(GPIOA,5,2);             //CAN1 rx
    sysctl_iomux_config(GPIOC,8,1);             //CAN2 tx
    sysctl_iomux_config(GPIOC,9,1);             //CAN2 rx
    sysctl_iomux_config(GPIOA,22,2);             //CAN3 
	#if (defined(AE101)||defined(AE102))
    sysctl_iomux_config(GPIOB,26,2);            //CAN3
	#endif
	#ifdef AE103
	sysctl_iomux_config(GPIOA,30,2);            //CAN3
	#endif
}

//*****************************************************************************
//
//  To setup can0-3 iomux
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void sysctl_iomux_can0(void)
{
    sysctl_iomux_config(GPIOA,13,1);            //CAN0 tx
    sysctl_iomux_config(GPIOA,14,1);            //CAN0 rx
}
void sysctl_iomux_can1(void)
{
    sysctl_iomux_config(GPIOA,4,2);             //CAN1 tx
    sysctl_iomux_config(GPIOA,5,2);             //CAN1 rx
}
void sysctl_iomux_can2(void)
{
    sysctl_iomux_config(GPIOC,8,1);             //CAN2 tx
    sysctl_iomux_config(GPIOC,9,1);             //CAN2 rx
}
void sysctl_iomux_can3(void)
{
    sysctl_iomux_config(GPIOA,22,2);             //CAN3 
    sysctl_iomux_config(GPIOB,26,2);            //CAN3
}


/**
 * @brief GPIO上拉设置
 *
 * @param    GPIO     AE102可选参数：GPIOA ,GPIOB...GPIOE
 *                    AE103可选参数：GPIOA ,GPIOB...GPIOE
 * @param    Num      号码选择如：GPIOA[Num] Num可选0-31
 *
 * @return   无
 */
BYTE  GPIO_Pullup_Config(BYTE  GPIO,BYTE  Num)
{
    #ifdef AE103
    switch (GPIO)
    {
        case GPIOA:
            SYSCTL_PIO0_UDCFG|=(0x1<<Num);
            break;
        case GPIOB:
            SYSCTL_PIO1_UDCFG|=(0x1<<Num);
            break;
        case GPIOC:
            SYSCTL_PIO2_UDCFG|=(0x1<<Num);
            break;
        case GPIOD:
            SYSCTL_PIO2_UDCFG|=(0x1<<(Num+15));
            break;
        case GPIOE:
            SYSCTL_PIO3_UDCFG|=(0x1<<Num);
            break;
        default:
            dprint("参数选择错误\n");
            break;
    }
    #endif

    #if (defined(AE101)||defined(AE102)) 
    switch (GPIO)
    {
        case GPIOA:
			if(Num<=15)
			{
				SYSCTL_PIO0_UDCFG|=(0x1<<Num);
			}
			else
			{
				SYSCTL_PIO1_UDCFG|=(0x1<<(Num-16));
			}
			break;
        case GPIOB:
            if(Num<=15)
			{
				SYSCTL_PIO2_UDCFG|=(0x1<<Num);
			}
			else
			{
				SYSCTL_PIO3_UDCFG|=(0x1<<(Num-16));
			}
		    break;
        case GPIOC:
            if(Num<=15)
			{
				SYSCTL_PIO4_UDCFG|=(0x1<<Num);
			}
			else
			{
				dprint("GPIO serial number input error");
			}
		    break;
        case GPIOD:
            SYSCTL_PIO5_UDCFG|=(0x1<<Num);
            break;
        case GPIOE:
			if(Num<=15)
			{
				SYSCTL_PIO5_UDCFG|=(0x1<<(Num+16));
			}
			else if(Num<=23)
			{
				SYSCTL_PIO5_UDCFG|=(0x1<<(Num-8));
			}
            break;
        default:
            dprint("参数选择错误\n");
            break;
    }
    #endif
	return 0;
}

/**
 * @brief GPIO配置函数，用于配置GPIO的工作模式及中断相关参数
 *
 * @param GPIO   GPIO组编号，参数可选：GPIOA GPIOB...
 * @param gpio_no   GPIO具体引脚号码，例如：GPIOA[5]则该参数为5
 * @param mode   GPIO的工作模式，0为输入模式，1为输出模式，2为中断模式
 * @param op_val   在输出模式下，该参数用于设置输出的电平，0为低电平，1为高电平，在其他模式下该参数无效
 * @param int_lv   在中断模式下，该参数用于设置中断触发电平或边沿，0为电平触发，1为边沿触发
 * @param pol   在中断模式下，该参数用于设置中断触发极性，1为上升沿（电平）触发  0为下降沿（低电平触发）
 *
 * @return 成功返回0，失败返回-1
 */
int GPIO_Config(int GPIO,int gpio_no,int mode,int op_val,int int_lv,int pol)
{
#if (!GPIO_CLOCK_EN)
	return 0;
#endif
#if (!GPIODB_CLOCK_EN)
	return 0;
#endif
	sysctl_iomux_config(GPIO,gpio_no,0);
    if(GPIO==1)
    {
        if(mode==1)     //output
        {	
            if(op_val>0)
            {	
                GPIOA_REG((GPIO_INPUT_VAL+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出值
                GPIOA_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
                GPIOA_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
                dprint("set GPIO%d,%d over val %d\n",GPIO,gpio_no,op_val);
            }
            else
            {
                GPIOA_REG((GPIO_INPUT_VAL+(gpio_no/8)))&=(~((0x1)<<(gpio_no%8)));//配置输出值
                GPIOA_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
                GPIOA_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
                dprint("set GPIO%d,%d over val %d\n",GPIO,gpio_no,op_val);
            }
        }
        else if(mode==0) //input
        {
            //(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
            GPIOA_REG(GPIO_INPUT_EN+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置输入模式
            GPIOA_REG(GPIO_LOW_IE+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
        }
        else             //interrupt
        {
            GPIOA_REG((GPIO_INPUT_EN+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//设置为输入模式
            GPIOA_REG((GPIO_LOW_IP+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//禁止中断屏蔽
            GPIOA_REG((GPIO_IOF_EN+(gpio_no/8)))=((int_lv)<<(gpio_no%8));//设置触发模式
            GPIOA_REG((GPIO_IOF_SEL+(gpio_no/8)))=((pol)<<(gpio_no%8));//设置触发极性
            GPIOA_REG((GPIO_DEBOUNCE_EN+(gpio_no/8)))=((0x1)<<(gpio_no%8));//设置消抖
            GPIOA_REG((GPIO_LOW_IE+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//使能中断
            dprint("触发方式:0x%x\n",GPIOA_REG((GPIO_IOF_EN+(gpio_no/8))));
            dprint("触发极性:0x%x\n",GPIOA_REG((GPIO_IOF_SEL+(gpio_no/8))));
        }
    }
    else if(GPIO==2)
    {
        if(mode==1)     //output
        {	
            if(op_val>0)
            {	
                GPIOB_REG((GPIO_INPUT_VAL+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出值
                GPIOB_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
                GPIOB_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
            }
            else
            {
                GPIOB_REG((GPIO_INPUT_VAL+(gpio_no/8)))&=(~((0x1)<<(gpio_no%8)));//配置输出值
                GPIOB_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
                GPIOB_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
            }
        }
        else if(mode==0) //input
        {
            //(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
            GPIOB_REG(GPIO_INPUT_EN+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置输入模式
            GPIOB_REG(GPIO_LOW_IE+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
        }
        else             //interrupt
        {
            GPIOB_REG((GPIO_INPUT_EN+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//设置为输入模式
            GPIOB_REG((GPIO_LOW_IP+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//禁止中断屏蔽
            GPIOB_REG((GPIO_IOF_EN+(gpio_no/8)))=((int_lv)<<(gpio_no%8));//设置触发模式
            GPIOB_REG((GPIO_IOF_SEL+(gpio_no/8)))=((pol)<<(gpio_no%8));//设置触发极性
            GPIOB_REG((GPIO_DEBOUNCE_EN+(gpio_no/8)))=((0x1)<<(gpio_no%8));//设置消抖
            GPIOB_REG((GPIO_LOW_IE+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//使能中断
            dprint("触发方式:0x%x\n",GPIOB_REG((GPIO_IOF_EN+(gpio_no/8))));
            dprint("触发极性:0x%x\n",GPIOB_REG((GPIO_IOF_SEL+(gpio_no/8))));
        }
    }
    else
    {
	    if(GPIO==4)
	    {
            if(mode==1)     //output
		    {	
			    if(op_val>0)
			    {	
				    GPIOC_REG((0xc))|=((0x1)<<gpio_no);//配置输出值
				    GPIOC_REG((0x10))|=((0x1)<<gpio_no);//配置输出模式
			    }
		   
			    else
			    {
				    GPIOC_REG((0xc))&=(~((0x1)<<gpio_no));//配置输出值
				    GPIOC_REG((0x10))|=((0x1)<<gpio_no);//配置输出模式
			    }
		    }
		    else if(mode==0) //input
		    {
				//(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
				GPIOC_REG(0x10)&=(~(0x1)<<gpio_no);//配置输入模式
	        }	

	    }
	    else if((GPIO==3))
	    {
		    if(mode==1)     //output
		    {	
			    if(op_val>0)
			    {	
				    GPIOC_REG((GPIO_INPUT_VAL+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出值
				    GPIOC_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
				    GPIOC_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
			    }
			    else
			    {
				    GPIOC_REG((GPIO_INPUT_VAL+(gpio_no/8)))&=(~((0x1)<<(gpio_no%8)));//配置输出值
				    GPIOC_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
				    GPIOC_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
			    }
		    }
		    else if(mode==0) //input
		    {
			    //(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
			    GPIOC_REG(GPIO_INPUT_EN+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置输入模式
			    GPIOC_REG(GPIO_LOW_IE+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
		    }
		    else             //interrupt
		    {
			    GPIOC_REG((GPIO_INPUT_EN+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//设置为输入模式
			    GPIOC_REG((GPIO_LOW_IP+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//禁止中断屏蔽
			    GPIOC_REG((GPIO_IOF_EN+(gpio_no/8)))=((int_lv)<<(gpio_no%8));//设置触发模式
			    GPIOC_REG((GPIO_IOF_SEL+(gpio_no/8)))=((pol)<<(gpio_no%8));//设置触发极性
			    GPIOC_REG((GPIO_DEBOUNCE_EN+(gpio_no/8)))=((0x1)<<(gpio_no%8));//设置消抖
			    GPIOC_REG((GPIO_LOW_IE+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//使能中断
			    dprint("触发方式:0x%x\n",GPIOC_REG((GPIO_IOF_EN+(gpio_no/8))));
		        dprint("触发极性:0x%x\n",GPIOC_REG((GPIO_IOF_SEL+(gpio_no/8))));
		    }
	    }
		#if (defined(AE101)||defined(AE102)||defined(AE103))
		else if((GPIO==5))
	    {
		    if(mode==1)     //output
		    {	
			    if(op_val>0)
			    {	
				    GPIOE_REG((GPIO_INPUT_VAL+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出值
				    GPIOE_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
				    GPIOE_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
			    }
			    else
			    {
				    GPIOE_REG((GPIO_INPUT_VAL+(gpio_no/8)))&=(~((0x1)<<(gpio_no%8)));//配置输出值
				    GPIOE_REG((GPIO_INPUT_EN+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//配置输出模式
				    GPIOE_REG((GPIO_LOW_IE+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
			    }
		    }
		    else if(mode==0) //input
		    {
			    //(*((BYTEP )(SYSCTL_PIO0_UDCFG)))|=((0x1)<<gpio_no);//配置上拉
			    GPIOE_REG(GPIO_INPUT_EN+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置输入模式
			    GPIOE_REG(GPIO_LOW_IE+(gpio_no/8))&=(~(0x1)<<(gpio_no%8));//配置GPIO模式(非中断)
		    }
		    else             //interrupt
		    {
			    GPIOE_REG((GPIO_INPUT_EN+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//设置为输入模式
			    GPIOE_REG((GPIO_LOW_IP+(gpio_no/8)))&=(~(0x1)<<(gpio_no%8));//禁止中断屏蔽
			    GPIOE_REG((GPIO_IOF_EN+(gpio_no/8)))=((int_lv)<<(gpio_no%8));//设置触发模式
			    GPIOE_REG((GPIO_IOF_SEL+(gpio_no/8)))=((pol)<<(gpio_no%8));//设置触发极性
			    GPIOE_REG((GPIO_DEBOUNCE_EN+(gpio_no/8)))=((0x1)<<(gpio_no%8));//设置消抖
			    GPIOE_REG((GPIO_LOW_IE+(gpio_no/8)))|=((0x1)<<(gpio_no%8));//使能中断
			    dprint("触发方式:0x%x\n",GPIOE_REG((GPIO_IOF_EN+(gpio_no/8))));
		        dprint("触发极性:0x%x\n",GPIOE_REG((GPIO_IOF_SEL+(gpio_no/8))));
		    }
	    }
		#endif
    }
    return 0;
}

/**
 * @brief GPIO引脚输入使能寄存器，当引脚不作为输入时不使能该寄存器能进一步降低功耗
 *
 * @param GPIO   	GPIO组编号，参数可选：GPIOA GPIOB...
 * @param gpio_no   GPIO具体引脚号码，例如：GPIOA[5]则该参数为5
 * @param sw   		可选参数：ENABLE 使能  DISABLE 失能
 *
 * @return 成功返回0，失败返回-1
 */
BYTE GPIO_Input_EN(int GPIO,int gpio_no,char sw)
{
	switch (GPIO)
	{
	case GPIOA:
		if(sw==ENABLE)
		{
			if(gpio_no<=15)
			{
				SYSCTL_PIO0_IECFG|=(0x1<<gpio_no);
			}
			else
			{
				SYSCTL_PIO1_IECFG|=(0x1<<(gpio_no-16));
			}
		}
		else
		{
			if(gpio_no<=15)
			{
				SYSCTL_PIO0_IECFG&=~(0x1<<gpio_no);
			}
			else
			{
				SYSCTL_PIO1_IECFG&=~(0x1<<(gpio_no-16));
			}
		}
		break;
	case GPIOB:
		if(sw==ENABLE)
		{
			if(gpio_no<=15)
			{
				SYSCTL_PIO2_IECFG|=(0x1<<gpio_no);
			}
			else
			{
				SYSCTL_PIO3_IECFG|=(0x1<<(gpio_no-16));
			}
			
		}
		else
		{
			if(gpio_no<=15)
			{
				SYSCTL_PIO2_IECFG&=~(0x1<<gpio_no);
			}
			else
			{
				SYSCTL_PIO3_IECFG&=~(0x1<<(gpio_no-16));
			}
		}
		break;
	case GPIOC:
		if(sw==ENABLE)
		{
			SYSCTL_PIO4_IECFG|=(0x1<<gpio_no);
		}
		else
		{
			SYSCTL_PIO4_IECFG&=~(0x1<<gpio_no);
		}
		break;
	case GPIOD:
		if(sw==ENABLE)
		{
			SYSCTL_PIO5_IECFG|=(0x1<<gpio_no);
		}
		else
		{
			SYSCTL_PIO5_IECFG&=~(0x1<<gpio_no);
		}
		break;
	case GPIOE:
		if(sw==ENABLE)
		{
			if(gpio_no<=15)
			{
				SYSCTL_PIO5_IECFG|=(0x1<<(gpio_no+16));
			}
			else
			{
				SYSCTL_PIO5_IECFG|=(0x1<<(gpio_no-8));
			}
		}
		else
		{
			if(gpio_no<=15)
			{
				SYSCTL_PIO2_IECFG&=~(0x1<<(gpio_no+16));
			}
			else
			{
				SYSCTL_PIO3_IECFG&=~(0x1<<(gpio_no-8));
			}
		}
		break;
	default:
		break;
	}
}
