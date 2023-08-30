/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:20:02
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

#ifndef CUSTOM_BATTERY_H
#define CUSTOM_BATTERY_H

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"

#include "KERNEL_ACPI.H"
#include "KERNEL_I2C.H"
#include "KERNEL_TIMER.H"

/*-----------------------------------------------------------------------------
 * Parameter Definition
 *---------------------------------------------------------------------------*/
#define BAT_I2C_CHANNEL		1
#define BAT_ADDR	0x0B
#define CHARGER_ADDR	0x09

#define DisablePower 0
#define PDPowerMode	1
#define ACPowerMode	2

#define BAT_MODE			0x03
#define BAT_TIME_TO_FULL	0x05
#define BAT_TEMPERATURE		0x08
#define	BAT_VOLTAGE			0x09
#define	BAT_CURRENT			0x0A
#define	BAT_RSOC			0x0D
#define	BAT_ASOC			0x0E
#define	BAT_REMAINING_CAP	0x0F
#define	BAT_FULL_CHARGE_CAP	0x10
#define	BAT_DESIGN_CAP		0x18
#define	BAT_DESIGN_VOLTAGE	0x19
#define	BAT_SERIAL_NUM		0x1C


#define C_ChargeOptions			0x12	// 0:Enable charge; 1:inhibit charge
#define C_ChargeCurrent		    0x14    // Set charge current
#define C_ChargeVoltage		    0x15    // Set charge voltage
#define C_InputCurrent		    0x3F    // Set input current	
#define C_MFID				    0xFE	// Get Manufacturer ID
#define C_DeviceID			    0xFF	// Get device ID

#define InputCurrentSet         0x1100	//4352mA

#define BAT_VOLT_WUCHG         0x32F0	//13040mV
#define BAT_CURR_WUCHG		   0x0180	//384mA

#define BAT_VOLT_PRECHG        0x32F0
#define BAT_CURR_PRECHG        0x05C0	//1472mA

#define BAT_VOLT_NMCHG         0x32F0
#define BAT_CURR_NMCHG         0x0880	//2176mA

#define Charge_Cutoff_Current         0x80 //128mA

#define Step_ID	            0x01    // Identify main battery
#define Step_WC			    0x02    // Battery wake up charge
#define Step_PC		        0x03    // Battery pre-charge
#define Step_NC		        0x04    // Battery normal charge
#define Step_DC			    0x05    // Battery discharge

#define BTAlarmTemperature		0x1450		//52.00 Temp Alarm Discharge
#define MFID		0x0040

/*-----------------------------------------------------------------------------
 * Variable Declarations
 *---------------------------------------------------------------------------*/

extern void Power_Switch(void);
extern void GetBatteryINFO(void);
extern void ChargerCenter(void); 
	

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Reference Smart Battery Data Specification
 * The following definition summarizes the Smart Battery command set.
 *---------------------------------------------------------------------------*/
#define _SMB_BatteryAddr            0x16
#define _CMD_ManufacturerAccess     0x00
#define _CMD_RemainingCapacityAlarm 0x01
#define _CMD_RemainingTimeAlarm     0x02
#define _CMD_BatteryMode            0x03
#define _CMD_AtRate                 0x04
#define _CMD_AtRateTimeToFull       0x05
#define _CMD_AtRateTimeToEmpty      0x06
#define _CMD_AtRateOK               0x07
#define _CMD_Temperature            0x08
#define _CMD_Voltage                0x09
#define _CMD_Current                0x0A
#define _CMD_AverageCurrent         0x0B
#define _CMD_MaxError               0x0C
#define _CMD_RelativeStateOfCharge  0x0D
#define _CMD_AbsoluteStateOfCharge  0x0E
#define _CMD_RemainingCapacity      0x0F
#define _CMD_FullChargeCapacity     0x10
#define _CMD_RunTimeToEmpty         0x11
#define _CMD_AverageTimeToEmpty     0x12
#define _CMD_AverageTimeToFull      0x13
#define _CMD_ChargingCurrent        0x14
#define _CMD_ChargingVoltage        0x15
#define _CMD_BatteryStatus          0x16
#define _CMD_CycleCount             0x17
#define _CMD_DesignCapacity         0x18
#define _CMD_DesignVoltage          0x19
#define _CMD_SpecificationInfo      0x1A
#define _CMD_ManufactureDate        0x1B
#define _CMD_SerialNumber           0x1C
#define _CMD_ManufacturerName       0x20
#define _CMD_DeviceName             0x21
#define _CMD_DeviceChemistry        0x22
#define _CMD_ManufacturerData       0x23
#define _CMD_OptionalMfgFunction5   0x2F
#define _CMD_OptionalMfgFunction4   0x3C
#define _CMD_OptionalMfgFunction3   0x3D
#define _CMD_OptionalMfgFunction2   0x3E
#define _CMD_OptionalMfgFunction1   0x3F


#define _CMD_ChargerInputCurrent    0x3F

#define A_XBYTE  *(VBYTE *)
#define A_XWORD  *(VWORD *)

#define xEC_PowerState				(A_XBYTE( ECSPACE_BASE_ADDR + 0x80))

typedef struct BatData
{
	VBYTE Cmd;
	VWORD *Var;
}BatteryData;

void Check_PD_Plugin(void);
void Check_AC_Plugin(void);
void Check_BATTERY_Plugin(void);

//-----------------------------------------------------------------------------
#endif
