/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:25:55
 * @Description: Read temperature support
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

#include "CUSTOM_TEMPERATURE.H"
#include "AE_FUNC.H"

char get_temperature(WORD i2c_channel)
{
    if(i2c_channel == 0)
        {
            #if !(SMBUS0_CLOCK_EN)
                dprint("SMBUS0 CLOCK NOT ENABLE\n");
                return 1;
            #endif
        }else if (i2c_channel == 1)
            {
                #if !(SMBUS1_CLOCK_EN)
                    dprint("SMBUS1 CLOCK NOT ENABLE\n");
                    return 1;
                #endif
            }else if(i2c_channel == 2)
                {
                    #if !(SMBUS2_CLOCK_EN)
                        dprint("SMBUS2 CLOCK NOT ENABLE\n");
                        return 1;
                    #endif 
                }else if(i2c_channel == 3)
                    {
                        #if !(SMBUS3_CLOCK_EN)
                            dprint("SMBUS3 CLOCK NOT ENABLE\n");
                            return 1;
                        #endif   
                    }

    /* Channel 1 of I2C is used to get the temperature */
    char int_curtmpr;
    char ext_curtmpr;
    /* read temperaturn only when Debugger not working and chip set as Master*/
    if(Slave_flag == 0)
    {
        /* read temperature */
        /*select the tempertur of register */
        I2c_Master_Write_Byte(0x0, i2c_channel);
        int_curtmpr = I2c_Master_Read_Byte(i2c_channel);
        /*select the os tempertur of register */
        I2c_Master_Write_Byte(0x3, i2c_channel);
        ext_curtmpr = I2c_Master_Read_Byte(i2c_channel);
        /* Write temp into ECSPACE_BASE_ADDR */
        CPU_TEMP = int_curtmpr;
        SYSTEM_TEMP = ext_curtmpr;

        Req_flag = 0;
        dprint("I2C%d int_curtmpr=%d,ext_curtmpr=%d\n",i2c_channel,int_curtmpr,ext_curtmpr);
    }
	return 0;
}

