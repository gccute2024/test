/*
 * @Author: Iversu
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-21 16:05:46
 * @Description: GPIO custom configuration
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

#include "CUSTOM_GPIO.H"


/*-----------------------------------------------------------------------------
 * Data Structure
 *---------------------------------------------------------------------------*/
typedef struct InitGPIOReg
{
    VBYTE   *reg;
    BYTE    value;
}sInitGPIOReg;


typedef struct InitGPIORegXWORD
{
    VDWORD   *reg;
    DWORD    value;
}sInitGPIORegXWORD;

const sInitGPIOReg  InitGPIORegXBYTE[] =
{
    { &GPIO0_DR0,				PinA0_7_Init            },  
    { &GPIO0_DDR0,				PinA0_7_InOut           },

	{ &GPIO0_DR1,               PinA8_15_Init            },
    { &GPIO0_DDR1,				PinA8_15_InOut           },


    { &GPIO0_DR2,               PinA16_23_Init            },  
    { &GPIO0_DDR2,				PinA16_23_InOut           },

	{ &GPIO0_DR3,               PinA24_31_Init            },  
    { &GPIO0_DDR3,				PinA24_31_InOut           },


    { &GPIO1_DR0,               PinB0_7_Init            },  
    { &GPIO1_DDR0,				PinB0_7_InOut           },
  
	{ &GPIO1_DR1,               PinB8_15_Init            },  
    { &GPIO1_DDR1,				PinB8_15_InOut           },


    { &GPIO1_DR2,				PinB16_23_Init  },  
    { &GPIO1_DDR2,				PinB16_23_InOut },
    
	{ &GPIO1_DR3,				PinB24_26_Init        },  
    { &GPIO1_DDR3,				PinB24_26_InOut       },


    { &GPIO2_DR0,               PinC0_7_Init            },  
    { &GPIO2_DDR0,				PinC0_7_InOut           },
    
	{ &GPIO2_DR1,				PinC8_13_Init        },  
    { &GPIO2_DDR1,				PinC8_13_InOut       },


    { &GPIO2_DR2,				PinD0_6_Init        },  
    { &GPIO2_DDR2,				PinD0_6_InOut       },

    #if (defined(AE101)||defined(AE102)||defined(AE103))
    { &GPIO3_DR0,               PinE0_7_Init            },  
    { &GPIO3_DDR0,				PinE0_7_InOut           },
  
	{ &GPIO3_DR1,               PinE8_15_Init            },  
    { &GPIO3_DDR1,				PinE8_15_InOut           },


    { &GPIO3_DR2,				PinE16_23_Init  },  
    { &GPIO3_DDR2,				PinE16_23_InOut },
    #endif
};



const sInitGPIORegXWORD  InitGPIORegXWORD[] =
{
    { &SYSCTL_PIO0_CFG,           PinA0_15_IoMux          },
    { &SYSCTL_PIO0_UDCFG,      PinA0_15_PullUp         },

    { &SYSCTL_PIO1_CFG,           PinA16_31_IoMux          },
    { &SYSCTL_PIO1_UDCFG,      PinA16_31_PullUp         },

    { &SYSCTL_PIO2_CFG,           PinB0_15_IoMux          },
    { &SYSCTL_PIO2_UDCFG,      PinB0_15_PullUp         },

    { &SYSCTL_PIO3_CFG,           PinB16_26_IoMux          },
    { &SYSCTL_PIO3_UDCFG,      PinB16_26_PullUp         },

    { &SYSCTL_PIO4_CFG,           PinC0_13_IoMux          },
    { &SYSCTL_PIO4_UDCFG,      PinC0_13_PullUp         },

    { &SYSCTL_PIO5_CFG,			PinDE_IoMux		     },
    { &SYSCTL_PIO5_UDCFG,    PinDE_PullUp          },
};
//-----------------------------------------------------------------------------
// The function of GPIO register init.
//-----------------------------------------------------------------------------
void Init_GPIO(void)
{
    BYTE iTemp = 0x00;
    BYTE Index = 0x00;

    while(iTemp<(sizeof(InitGPIORegXBYTE)/sizeof(sInitGPIOReg)))
    {
        *InitGPIORegXBYTE[iTemp].reg=InitGPIORegXBYTE[iTemp].value;
        iTemp++;
    }

    /*while(Index<(sizeof(InitGPIORegXWORD)/sizeof(sInitGPIORegXWORD)))
    {
        *InitGPIORegXWORD[Index].reg=InitGPIORegXWORD[Index].value;
        Index++;
    }*/
    printf("Init GPIO end\n");
}

