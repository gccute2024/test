/*
 * @Author: Iversu
 * @LastEditors: dejavudwh
 * @LastEditTime: 2023-05-12 11:10:50
 * @Description: Platform battery & battery charger control code
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

#include "CUSTOM_BATTERY.H"

#include "CUSTOM_GPIO.H"
#include "CUSTOM_MAIN.H"
#include "KERNEL_MEMORY.H"
#include "CUSTOM_POWER.H"

static unsigned char AC_Plugin_Flag = 0;
static unsigned char BT_Plugin_Flag = 1;

static BYTE BT_First_Plugin = 0;

VWORD Rsoc, DesignCap, FullChargeCap, RemainCap, DesignVtg, Vtg, SN, CurrentNow, TemperatureNow;

#define DebugPrint 1
/*-----------------------------------------------------------------------------
 * Local Parameter Definition
 *---------------------------------------------------------------------------*/

void Check_PD_Plugin(void);
void Check_AC_Plugin(void);
void Check_BATTERY_Plugin(void);
void SwtichPowerMode(BYTE  PowerSupply);
void Power_Switch(void);

void BatteryTARSetup(void);
void ChargerTARSetup(void);
short int BatteryRead(WORD addr);
short int ChargerRead(WORD addr);

void SetSmartCharger(short ChargeCurrent, short ChargeVoltage, short InputCurrent);
void DisableSmartCharger(void);
void EnableSmartCharger(void);
void ForceDisCharge(void);
char BatteryStatusCheck(void);

void GetBatteryINFO(void);
void ChargerCenter(void); 

/*******************Get battery Info*****************/
const BatteryData BAT_Data_Table[] =
{
	//   command     		  address of variable  
    {BAT_TEMPERATURE,       &TemperatureNow       	},	
    {BAT_RSOC,   			&Rsoc        			},
    {BAT_DESIGN_CAP,   		&DesignCap     			},	
    {BAT_FULL_CHARGE_CAP,   &FullChargeCap       	},
    {BAT_REMAINING_CAP,     &RemainCap       		},
    {BAT_DESIGN_VOLTAGE,   	&DesignVtg          	},	
    {BAT_VOLTAGE,   	    &Vtg          			},
	{BAT_SERIAL_NUM,  		&SN        				},
    {BAT_CURRENT,  			&CurrentNow        		},
};

void Battery_Plugout_Clear(void)
{
		BATTERY1_RSOC = 0;

		BATTERY1_DCAP_L = 0;	
		BATTERY1_DCAP_H = 0;	

		BATTERY1_FCAP_L = 0;
		BATTERY1_FCAP_H = 0;

		BATTERY1_RMCAP_L = 0;
		BATTERY1_RMCAP_H = 0;
		
		BATTERY1_DVOL_L = 0;
		BATTERY1_DVOL_H = 0;
		
		BATTERY1_VOL_L = 0;
		BATTERY1_VOL_H = 0;
		
		BATTERY1_SN_L = 0;
		BATTERY1_SN_H = 0;

		Bat_Info_Cnt = 0;
		BatteryInfoRdy = 0;
}


void Check_PD_Plugin(void)
{
	//ac plug in
	if(1 == PD_INT)
	{
		PD_IN = 1;
	}
	else//no ac
	{
		PD_IN = 0;
	}
}

void Check_AC_Plugin(void)
{
	(*(unsigned char*)(0x2804)) &= ~(1<<6);//GPA6 set input
	//ac plug in
	if(*((VBYTE*)(0x2850))&0x40)//GPA6
	{
		CTRL_FLAG1 |= ac_in;
		AC_IN = 1;
		if(0 == AC_Plugin_Flag)//QEvent run only once
		{
			Gen_EC_QEvent(_SCIEVT_ADAPTER, SCIMode_Normal);
			AC_Plugin_Flag = 1;
		}
		
	}
	else//no ac
	{
		CTRL_FLAG1 &= ~(ac_in);
		AC_IN = 0;
		if(1 == AC_Plugin_Flag)
		{
			Gen_EC_QEvent(_SCIEVT_ADAPTER, SCIMode_Normal);
			AC_Plugin_Flag = 0;
		}
	}
}

void Check_BATTERY_Plugin(void)
{
		//dc battery plug in
    (*((unsigned char*)(0x2C07))) &= ~(1<<1);//GPH5(GPH1)
	if(*((VBYTE*)(0x2C53))&0x02)//GPH5(GPH1)
	{
		BATTERY_IN = 0;
		/* Update ECSPACE_BASE_ADDR */
		//xEC_PowerState &=~(0x1<<1); 
        CTRL_FLAG1 &= ~(power_on);
		CHARGE_LED_OFF();
		CHARGE_FULL_LED_OFF();

		/* Send Qevent once */
		if(0 == BT_Plugin_Flag)//run only once
		{
			Gen_EC_QEvent(_SCIEVT_BATTERY, SCIMode_Normal);
			Battery_Plugout_Clear();
			PD_Plugin_Once_Flag = 0;
			BT_Plugin_Flag = 1;
		}
	}
	else
	{
		BATTERY_IN = 1;
		/* Update ECSPACE_BASE_ADDR */
		//xEC_PowerState |=(0x1<<1); 
        CTRL_FLAG1 |= (power_on);
		/* Send Qevent once */
		if(1 == BT_Plugin_Flag)//run only once
		{
			//Gen_EC_QEvent(_SCIEVT_BATTERY, SCIMode_Normal);
			BT_Plugin_Flag = 0;
			BT_First_Plugin = 1;
			PD_Plugin_Once_Flag = 0;
		}
	}
}

void SwtichPowerMode(BYTE  PowerSupply)
{
	switch(PowerSupply)
	{
		case PDPowerMode:
			CLEAR_MASK(GPIO1_DR0,BIT(3));//GPE3 low
			CLEAR_MASK(GPIO1_DR0,BIT(3));//GPE2 low
			dprint("\n!!!WARNING Power Supply ERROR!!!\n");
			return;
		case ACPowerMode:
			CLEAR_MASK(GPIO1_DR0,BIT(3));//GPE3 low
			SET_MASK(GPIO1_DR0,BIT(3));//GPE2 high
			return;
		default:
			SET_MASK(GPIO1_DR0,BIT(3));//GPE3 high
			CLEAR_MASK(GPIO1_DR0,BIT(3));//GPE2 low
			return;
	}
}

void Power_Switch(void)
{
	Check_AC_Plugin();
	Check_PD_Plugin();
	Check_BATTERY_Plugin();
	
	if((1 == AC_IN)&&(1 == PD_IN))//ac pd all in (error)
	{
		/* Update ECSPACE_BASE_ADDR */
		xEC_PowerState &=~(0x1<<0);
		/* Switching power supply mode */
		SwtichPowerMode(DisablePower);
		return;
	}
	else if((0 == AC_IN)&&(0 == PD_IN))//no ac or pd
	{
		/* Update ECSPACE_BASE_ADDR */
		xEC_PowerState &=~(0x1<<0);
		/* Switching power supply mode */
		SwtichPowerMode(DisablePower);
		/* Send Qevent once */
		if(1 == AC_Plugin_Once_Flag || 1 == PD_Plugin_Once_Flag)//run only once
		{
			Gen_EC_QEvent(_SCIEVT_ADAPTER, SCIMode_Normal);
			AC_Plugin_Once_Flag = 0;
			PD_Plugin_Once_Flag = 0;
		}
		return;
	}
	else if((1 == AC_IN)&&(0 == PD_IN))//ac in
	{
		/* Update ECSPACE_BASE_ADDR */
		xEC_PowerState |=(0x1<<0); 
		/* Switching power supply mode */
		SwtichPowerMode(ACPowerMode);
		/* Send Qevent once */
		if(0 == AC_Plugin_Once_Flag)//run only once
		{
			Gen_EC_QEvent(_SCIEVT_ADAPTER, SCIMode_Normal);
			AC_Plugin_Once_Flag = 1;
		}
		return;
	}
	else if((0 == AC_IN)&&(1 == PD_IN))//pd in
	{
		/* Update ECSPACE_BASE_ADDR */
		xEC_PowerState |=(0x1<<0); 
		SwtichPowerMode(PDPowerMode);
		/* Send Qevent once */
		if(0 == PD_Plugin_Once_Flag)//run only once
		{
			Gen_EC_QEvent(_SCIEVT_ADAPTER, SCIMode_Normal);
			AC_Plugin_Once_Flag = 1;
		}
		return;
	}
}

void BatteryTARSetup(void)
{
	//setup Battery TAR
	I2c_Master_Set_Tar(BAT_ADDR,BAT_I2C_CHANNEL);
}
void ChargerTARSetup(void)
{
	//setup Charger TAR
	I2c_Master_Set_Tar(BAT_ADDR,BAT_I2C_CHANNEL);
}

short int BatteryRead(WORD addr)
{	
	VWORD data;
	VBYTE temp0,temp1;

	//write addr
	if(0 == I2c_Check_TFE(BAT_I2C_CHANNEL))
	{
	I2c_Writeb(addr,I2C_DATA_CMD_OFFSET,BAT_I2C_CHANNEL);
	I2c_Writeb(0x2,I2C_DATA_CMD_RWDIR,BAT_I2C_CHANNEL);
	}
	
	//send read*2
	if(0 == I2c_Check_TFE(BAT_I2C_CHANNEL))
	{
	I2c_Writeb(0x1,I2C_DATA_CMD_RWDIR,BAT_I2C_CHANNEL);
	}
	if(0 == I2c_Check_TFE(BAT_I2C_CHANNEL))
	{
	I2c_Writeb(0x3,I2C_DATA_CMD_RWDIR,BAT_I2C_CHANNEL);
	}

	//read data		
	if(0 == I2c_Check_RFNE(BAT_I2C_CHANNEL))
	{
	temp0 = I2c_Readb(I2C_DATA_CMD_OFFSET,BAT_I2C_CHANNEL);
	}
	if(0 == I2c_Check_RFNE(BAT_I2C_CHANNEL))
	{
	temp1 = I2c_Readb(I2C_DATA_CMD_OFFSET,BAT_I2C_CHANNEL);
	}
	data = ((temp1 << 8) | temp0);

	return data;
}

short int ChargerRead(WORD addr)
{	
	short int value;
	char data[2] = {0};
	I2c_Master_Read_Block(data,2,BAT_I2C_CHANNEL);
	value = ((data[1]<<8) | data[0]);
	return value;
}

void ChargerWrite(WORD addr, short int value)
{	
	char data[2] = {0};
	data[0] = (value&0xFF);
	data[1] = ((value>>8)&0xFF);
	I2c_Master_Write_Block(data,2,BAT_I2C_CHANNEL);
}

void SetSmartCharger(short ChargeCurrent, short ChargeVoltage, short InputCurrent)
{
	short int CC,CV,IC,CO,Temp;
	/* Disable Charger WDT */
	CO = ChargerRead(C_ChargeOptions)&0xFFFF;
	CO &= ~((0x3)<<13);//disable wdt
	vDelayXms(1);
	ChargerWrite(C_ChargeOptions,CO);
	vDelayXms(1);

	/* Write Voltage */
	ChargerWrite(C_ChargeVoltage,ChargeVoltage);
	vDelayXms(1);
	
	/* Write Current */
	ChargerWrite(C_ChargeCurrent,ChargeCurrent);
	vDelayXms(1);
	
	/* Write Input Current */
	ChargerWrite(C_InputCurrent,InputCurrent);
	vDelayXms(1);
}

void DisableSmartCharger(void)
{
	short int value;
	value = ChargerRead(C_ChargeOptions)&0xFFFF;
	value |= 0x1;//Disable charger
	vDelayXms(1);
	ChargerWrite(C_ChargeOptions,value);
}

void EnableSmartCharger(void)
{
	short int value;
	value = ChargerRead(C_ChargeOptions)&0xFFFF;
	value &=~(0x1);//Enable charger
	vDelayXms(1);
	ChargerWrite(C_ChargeOptions,value);
}

void ForceDisCharge(void)
{
	SetSmartCharger(0x0000, BAT_VOLT_NMCHG, InputCurrentSet);
}

char BatteryStatusCheck(void)
{
	if(TemperatureNow >= BTAlarmTemperature)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


void GetBatteryINFO(void)
{
	if(0==BATTERY_IN)
	{
		BatteryInfoRdy = 0;
	
		return;
	}
	
	/* Set BAT TAR */
	BatteryTARSetup();

	if(Bat_Info_Cnt < (sizeof(BAT_Data_Table)/sizeof(BatteryData)))
	{

		*(BAT_Data_Table[Bat_Info_Cnt].Var) = BatteryRead(BAT_Data_Table[Bat_Info_Cnt].Cmd);

		Bat_Info_Cnt++;

		if(Bat_Info_Cnt > ((sizeof(BAT_Data_Table)/sizeof(BatteryData)) - 1 ))
		{
		BATTERY1_RSOC = Rsoc & 0xFF;

		BATTERY1_DCAP_L = (VBYTE)(DesignCap & 0xFF);	
		BATTERY1_DCAP_H = (VBYTE)((DesignCap>>8) & 0xFF);	

		BATTERY1_FCAP_L = (VBYTE)(FullChargeCap & 0xFF);
		BATTERY1_FCAP_H = (VBYTE)((FullChargeCap>>8) & 0xFF);

		BATTERY1_RMCAP_L = (VBYTE)(RemainCap & 0xFF);
		BATTERY1_RMCAP_H = (VBYTE)((RemainCap>>8) & 0xFF);
		
		BATTERY1_DVOL_L = (VBYTE)(DesignVtg & 0xFF);
		BATTERY1_DVOL_H = (VBYTE)((DesignVtg>>8) & 0xFF);
		
		BATTERY1_VOL_L = (VBYTE)(Vtg & 0xFF);
		BATTERY1_VOL_H = (VBYTE)((Vtg>>8) & 0xFF);
		
		BATTERY1_SN_L = (VBYTE)(SN & 0xFF);
		BATTERY1_SN_H = (VBYTE)((SN>>8) & 0xFF);

		//printf("RMCAP_L:%x,RMCAP_H:%x,CurrentNow:%x\n",BATTERY1_RMCAP_L,BATTERY1_RMCAP_H,CurrentNow);

		Bat_Info_Cnt = 0;
		BatteryInfoRdy = 1;
		if((BT_First_Plugin == 1)&&(BatteryInfoRdy == 1))
		{

			BT_First_Plugin++;
			if(BT_First_Plugin == 2)
			{
			BT_First_Plugin = 0;
			printf("Rsoc:%x\n",BATTERY1_RSOC);
			Gen_EC_QEvent(_SCIEVT_BATTERY, SCIMode_Normal); //just generate battery Qevent only once when battery first in
			}
		}
		}
	}
}


void ChargerCenter(void) 
{
	if(0 == BatteryInfoRdy)
	{
		ChargerCount = 0;
		Charger_CtrlStep = 1;
		return;
	}

	if(!AC_IN)
	{
		CHARGE_LED_OFF();
		CHARGE_FULL_LED_OFF();
		ChargerCount = 0;
		Charger_CtrlStep = 1;//plug ac or battery out will reset the Step
		return;
	}

	ChargerTARSetup();

	if(!BATTERY_IN)
	{
		CHARGE_LED_OFF();
		ForceDisCharge();
		DisableSmartCharger();
		ChargerCount = 0;
		Charger_CtrlStep = 1;//plug ac or battery out will reset the Step
		return;
	}

	if(0 == BatteryStatusCheck())//check battery status
	{
		ForceDisCharge();
		ChargerCount = 0;
		Charger_CtrlStep = 1;
		return;
	}
	
	ChargerStep = Charger_CtrlStep;//Debug add get present step to print in CUSTOM 1sec event
	EnableSmartCharger();

	//Check RSOC and Discharge part
	// if(BATTERY1_RSOC>=95)//First check if charge to 95% then discharge
	if(Rsoc >= 95)//First check if charge to 95% then discharge
	{
		if(CurrentNow <= Charge_Cutoff_Current)//Meanwhile detect current
		{
			Charger_CtrlStep = Step_DC;
			ChargerCount = 1;
		}
	}

	short int ID;
	//Charge Sequnence
	switch(Charger_CtrlStep)//Charger step
	{
		case Step_ID:
			{
				ForceDisCharge();
				ID = ChargerRead(C_MFID);
				if(ID!=MFID)//check ID
				{
					dprint("Charger SMBus ERROR! \n");
					return;
				}
				Charger_CtrlStep = Step_WC;
				break;
			}
		case Step_WC:
			{
				CHARGE_LED_ON();
				CHARGE_FULL_LED_OFF();
				SetSmartCharger(BAT_CURR_WUCHG, BAT_VOLT_WUCHG, InputCurrentSet);
				if((ChargerCount%50)==0)
				{
					Charger_CtrlStep = Step_PC;
					ChargerCount = 1;
				}
				break;
			}
		case Step_PC:
			{
				CHARGE_LED_ON();
				CHARGE_FULL_LED_OFF();
				SetSmartCharger(BAT_CURR_PRECHG, BAT_VOLT_PRECHG, InputCurrentSet);
				if(CurrentNow >= BAT_CURR_PRECHG)
				{
					Charger_CtrlStep = Step_NC;
					ChargerCount = 1;
				}
				break;
			}
		case Step_NC:
			{
				SetSmartCharger(BAT_CURR_NMCHG, BAT_VOLT_NMCHG, InputCurrentSet);
				if(Rsoc>=95)//if charge to 95% then discharge
				{
					if(CurrentNow <= Charge_Cutoff_Current)
					{
						Charger_CtrlStep = Step_DC;
						ChargerCount = 1;
					}
				}
				break;
			}
		case Step_DC:
			{
				CHARGE_LED_OFF();
				CHARGE_FULL_LED_ON();
				ForceDisCharge();
				if(Rsoc<=90)//if battery less than 85% then precharge
				{
					Charger_CtrlStep = Step_PC;
					ChargerCount = 1;
				}
				break;
			}
		default:
			{
				break;
			}
	}
	return;
}

void Battery_Static_Mode_Judge(void)
{
    if((System_PowerState == SYSTEM_S5) && (AC_IN == 0))
    {
        noac_1s_cnt++;
        if(noac_1s_cnt >= 180)
        {
		Enter_LowPower_Mode();
        }
    } else {
		noac_1s_cnt = 0;
	}
}
