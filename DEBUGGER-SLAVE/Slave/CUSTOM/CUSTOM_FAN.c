/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:22:50
 * @Description: Fan support
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

#include "CUSTOM_FAN.H"


/*风扇温度相关的变???*/
VBYTE  cpu_temp,sys_temp;

/*风扇level相关的处???*/
BYTE   FanLevel,FanRPM;

const thermal  ThermalTalbe1[]=
{
    {0,	38,	 0,		0,},	// Level 0 	
    {1,	48,	 36,   19,},	// Level 1 	
    {2,	58,	 46,   23,},	// Level 2 	
    {3,	68,	 56,   26,},	// Level 3 	
    {4,	78,  66,   28,},	// Level 4
    {5,	100, 76,   28,},	// Level 5	
};

//-----------------------------------------------------------------------------
// Function Name : ReadCPUTemp
//-----------------------------------------------------------------------------
VBYTE  ReadCPUTemp(void) 
{
	/*pcie读取cpu温度，暂时不做处???*/
	//cpu_temp
	return 0;
}

//-----------------------------------------------------------------------------
// Function Name : ReadSysTemp
//-----------------------------------------------------------------------------
VBYTE  ReadSysTemp(void) 
{
  /*iic接口读取温感*/
  //sys_temp
   return 0;
}

//-----------------------------------------------------------------------------
// The function of check fan rpm 1  每分钟转数（Revolutions Per Minute???//-----------------------------------------------------------------------------	
void CheckFanRPM(void)
{
	/*上次扫描的结???*/
	BYTE  FanLevel_t = FanLevel;	
	BYTE  Fan1RPM_t = FanRPM;
	VBYTE  temperature1;
	/*获取温度*/
	cpu_temp = ReadCPUTemp();
	sys_temp = ReadSysTemp();
	temperature1=(cpu_temp>sys_temp)?cpu_temp:sys_temp;
	dprint("temperature1 = %d\n",temperature1);

	/*如果风扇不是最大的转速，可以进入这个判断处理流程当中*/
	if(FanLevel_t <((sizeof(ThermalTalbe1)/sizeof(thermal))-1))
	{
		if(temperature1> ThermalTalbe1[FanLevel_t].CFanOn)
		{
			FanLevel_t += 1;
		}
	}

	/*如果风扇不是最低档转速的存在，就可以进入这个流程，处理降低风扇转???*/
	
	if (FanLevel_t>0)
	{
		if (temperature1< ThermalTalbe1[FanLevel_t].CFanOff)
		{						
			FanLevel_t-=1;
		}
	}
	/*取出智能调速的level和rpm，在后面定时执行风扇转速的时候是???*/
	FanLevel = FanLevel_t;	
	FanRPM = ThermalTalbe1[FanLevel_t].CRPM;

}

/*定时器处理中调用的定时事???*/
 void fan_scan_thread()
 {
	
    struct pwm_str  pwm_config; 
	int i = 0;
	/*1:获取新的等级和rpm参数*/
	CheckFanRPM();
	/*2:根据新的等级和参数完成智能调???*/
	for(i=0;i<8;i++)
	{
		memset(&pwm_config,0,sizeof( struct pwm_str));
		pwm_config.pwm_no = i;
		PWM_Get(&pwm_config);
		/*3:更新风扇的配置参???*/
		pwm_config.pwm_scale = FanRPM;
		/*4:配置pwm*/
		 PWM_Set(&pwm_config);
	}
	
 }
