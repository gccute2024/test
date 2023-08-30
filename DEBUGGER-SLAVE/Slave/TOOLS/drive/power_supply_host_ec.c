/*
 * @Author: Linyu
 * @LastEditors: Linyu
 * @LastEditTime: 2023-04-02 21:18:15
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

#define LOCK_DEF 1//锁
#define MISC_RUN 0//杂项设备
#define POWER_SUPPLY_RUN 1//供电信息
#define HEAD_FILE 1//头文件
#if HEAD_FILE
#include <linux/module.h>//模块信息，最基本的文件，支持动态添加和卸载模块
#include <linux/init.h>//初始化、退出等信息
#include <linux/serio.h>//串行输入输出虚拟总线
#include <linux/platform_device.h>//平台设备（实现不同总线外设的统一驱动）
#include <linux/hwmon.h>//hardware monitoring framework硬件监视框架
#include <linux/hwmon-sysfs.h>//hwmon子系统的虚拟文件系统
#include <linux/pci.h>//pci设备总线相关
#include <linux/pci_ids.h>//pci的硬件相关信息
#include <linux/poll.h>//文件监视,事件监测
#include <linux/slab.h>//内存缓存管理
#include <linux/proc_fs.h>//内核文件系统
#include <linux/miscdevice.h>//杂项设备
#include <linux/apm_bios.h>//BIOS电源管理方案
#include <linux/capability.h>//权限设置
#include <linux/sched.h>//进程相关信息
#include <linux/pm.h>//电源管理
#include <linux/apm-emulation.h>//apm仿真（用于无BIOS，如arm架构）
#include <linux/device.h>//设备
#include <linux/kernel.h>//内核
#include <linux/list.h>//链表
#include <linux/completion.h>//等待队列
#include <linux/kthread.h>//线程
#include <linux/delay.h>//延时
#include <linux/timer.h>//内核定时器
#include <linux/reboot.h>//重启、挂机等
#include <linux/input.h>//输入子系统
#include <linux/input/sparse-keymap.h>//输入键映射
//#include <linux/input-event-codes.h>//输入事件处理
#include <linux/fb.h>//帧缓冲驱动
#include <linux/power_supply.h>//供电设备

#include <asm/delay.h>//处理器延时
#include <asm/io.h>	//基本输入输出文件

#include "ec_wpce775l.h"
#endif
#define EC_MOD_DEF 1//宏定义
#if EC_MOD_DEF
    //typedef abb
    typedef unsigned char  u8;
    typedef unsigned short u16;
    typedef unsigned int   u32;
    typedef unsigned long long  u64;
    typedef short s16;
    typedef int   s32;
    typedef long long  s64;
    #define dbg 	printk
    #define I8042_STR_PARITY	0x80
    #define I8042_STR_TIMEOUT	0x40
    #define I8042_STR_AUXDATA	0x20
    #define I8042_STR_KEYLOCK	0x10
    #define I8042_STR_CMDDAT	0x08
    #define I8042_STR_MUXERR	0x04
    #define I8042_STR_IBF		0x02
    #define I8042_STR_OBF		0x01
    /* Power info cached timeout */
    #define POWER_INFO_CACHED_TIMEOUT	100	/* jiffies */
    //define statues of fan control
    #define CONSTANT_SPEED_POLICY	0  /* at constant speed */
    #define STEP_SPEED_POLICY	1  /* use up/down arrays to describe policy */
    #define KERNEL_HELPER_POLICY	2  /* kernel as a helper to fan control */
    //EC_PHY_ADDRS
    #define EC_BASE_ADDR (void *)0x18000000
    #define EC_IRQ_BASE_ADDR (char *)0x10002000
    #define EC_OFFSET_MAX 0x1000
    #define EC_IRQ_OFFSET_MAX 0x1000
    //timer
    #define delay mdelay(1);
    #ifndef EC_REG_DELAY
    #define EC_REG_DELAY		30000
    #endif
    #define SIO_INDEX_PORT		0x2E
    #define SIO_DATA_PORT		0x2F
    #define EC_CMD_TIMEOUT		0x1000
    #define EC_SEND_TIMEOUT		0xffff
    #define EC_RECV_TIMEOUT		0xffff
    #define EC_CMD_PORT		0x66
    #define EC_STS_PORT		0x66
    #define EC_DAT_PORT		0x62
    #define CMD_READ_EC		0x80	/* Read EC command. */
    #define CMD_WRITE_EC		0x81	/* Write EC command. */
    #define CMD_GET_EVENT_NUM	0x84	/* Query EC command, for get SCI event number. */
    #define CMD_RESET		0x4E	/* Reset and poweroff the machine auto-clear: rd/wr */
    #define CMD_EC_VERSION		0x4F	/* EC Version OEM command: 36 Bytes */
    #define EC_IO_PORT_LOW EC_CMD_PORT
    #define EC_IO_PORT_DATA EC_DAT_PORT
    #define EC_IO_PORT_HIGH EC_STS_PORT
    /* >>> Read/Write temperature & fan index for ACPI 80h/81h command. */
    #define INDEX_TEMPERATURE_VALUE		0x1B	/* Current CPU temperature value, Read and Write(81h command). */
    #define INDEX_FAN_MAXSPEED_LEVEL	0x5B	/* Fan speed maxinum levels supported. Defaut is 6. */
    #define INDEX_FAN_SPEED_LEVEL		0x5C	/* FAn speed level. [0,5] or [0x06, 0x38]*/
    #define INDEX_FAN_CTRLMOD		0x5D	/* Fan control mode, 0 = by EC, 1 = by Host.*/
    #define INDEX_FAN_STSCTRL		0x5E	/* Fan status/control, 0 = stop, 1 = run. */
    #define INDEX_FAN_ERRSTS		0x5F	/* Fan error status, 0 = no error, 1 = has error. */
    #define INDEX_FAN_SPEED_LOW		0x08	/* Fan speed low byte.*/
    #define INDEX_FAN_SPEED_HIGH		0x09	/* Fan speed high byte. */
    /* <<< End Temp & Fan */
    /* >>> Read/Write LCD backlight information/control index for ACPI 80h/81h command. */
    #define INDEX_BACKLIGHT_CTRLMODE	0x57	/* LCD backlight control mode: 0 = by EC, 1 = by HOST */
    #define INDEX_BACKLIGHT_STSCTRL		0x58	/* LCD backlight status or control: 0 = turn off, 1 = turn on */
    #define	INDEX_DISPLAY_MAXBRIGHTNESS_LEVEL	0x59	/* LCD backlight brightness max level */
    #define	INDEX_DISPLAY_BRIGHTNESS	0x5A	/* 10 stages (0~9) LCD backlight brightness adjust */
    /* <<< End Backlight */
    #define INDEX_TOUCHPAD_ENABLE_LED	0x56

    /* >>> Read battery(BQ3060) index for ACPI 80h command */
    /*
    * The reported battery die temperature.
    * The temperature is expressed in units of 0.25 seconds and is updated every 2.56 seconds.
    * The equation to calculate reported pack temperature is:
    * Temperature = 0.1 * (256 * TEMPH + TEMPL) Kelvin
    * Temperature -= 273 Degrees Celsius
    * The host sytem has read-only access to this register pair.
    */
    #define INDEX_BATTERY_TEMP_LOW		0x20	/* Battery temperature low byte. */
    #define INDEX_BATTERY_TEMP_HIGH		0x21	/* Battery temperature high byte. */
    #define INDEX_BATTERY_VOL_LOW		0x22	/* Battery Voltage Low byte. */
    #define INDEX_BATTERY_VOL_HIGH		0x23	/* Battery Voltage High byte. */
    #define INDEX_BATTERY_CURRENT_LOW	0x24	/* Battery Current Low byte. */
    #define INDEX_BATTERY_CURRENT_HIGH	0x25	/* Battery Current High byte. */
    #define INDEX_BATTERY_AC_LOW		0x26	/* Battery AverageCurrent Low byte. */
    #define INDEX_BATTERY_AC_HIGH		0x27	/* Battery AverageCurrent High byte. */
    #define INDEX_BATTERY_CAPACITY		0x2A	/* Battery RemainingCapacity percent. */
    #define INDEX_BATTERY_STATUS_LOW	0x2C	/* Battery Status low byte. */
    #define INDEX_BATTERY_STATUS_HIGH	0x2D	/* Battery Status high byte. */
    #define INDEX_BATTERY_RC_LOW		0x2E	/* Battery RemainingCapacity Low byte. */
    #define INDEX_BATTERY_RC_HIGH		0x2F	/* Battery RemainingCapacity High byte. */
    #define INDEX_BATTERY_ATTE_LOW		0x30	/* Battery AverageTimeToEmpty Low byte. */
    #define INDEX_BATTERY_ATTE_HIGH		0x31	/* Battery AverageTimeToEmpty High byte. */
    #define INDEX_BATTERY_ATTF_LOW		0x32	/* Battery AverageTimeToFull Low byte. */
    #define INDEX_BATTERY_ATTF_HIGH		0x33	/* Battery AverageTimeToFull High byte. */
    #define INDEX_BATTERY_FCC_LOW		0x34	/* Battery FullChargeCapacity Low byte. */
    #define INDEX_BATTERY_FCC_HIGH		0x35	/* Battery FullChargeCapacity High byte. */
    #define INDEX_BATTERY_CC_LOW		0x36	/* Battery ChargingCurrent Low byte. */
    #define INDEX_BATTERY_CC_HIGH		0x37	/* Battery ChargingCurrent High byte. */
    #define INDEX_BATTERY_CV_LOW		0x38	/* Battery ChargingVoltage Low byte. */
    #define INDEX_BATTERY_CV_HIGH		0x39	/* Battery ChargingVoltage High byte. */
    #define INDEX_BATTERY_CHGSTS_LOW	0x3A	/* Battery ChargingStatus Low byte. */
    #define INDEX_BATTERY_CHGSTS_HIGH	0x3B	/* Battery ChargingStatus High byte. */
    #define INDEX_BATTERY_CYCLECNT_LOW	0x3C	/* Battery CycleCount Low byte. */
    #define INDEX_BATTERY_CYCLECNT_HIGH	0x3D	/* Battery CycleCount High byte. */
    /* Battery static information. */
    #define INDEX_BATTERY_DC_LOW		0x60	/* Battery DesignCapacity Low byte. */
    #define INDEX_BATTERY_DC_HIGH		0x61	/* Battery DesignCapacity High byte. */
    #define INDEX_BATTERY_DV_LOW		0x62	/* Battery DesignVoltage Low byte. */
    #define INDEX_BATTERY_DV_HIGH		0x63	/* Battery DesignVoltage High byte. */
    #define INDEX_BATTERY_MFD_LOW		0x64	/* Battery ManufactureDate Low byte. */
    #define INDEX_BATTERY_MFD_HIGH		0x65	/* Battery ManufactureDate High byte. */
    #define INDEX_BATTERY_SN_LOW		0x66	/* Battery SerialNumber Low byte. */
    #define INDEX_BATTERY_SN_HIGH		0x67	/* Battery SerialNumber High byte. */
    #define INDEX_BATTERY_MFN_LENG		0x68	/* Battery ManufacturerName string length. */
    #define INDEX_BATTERY_MFN_START		0x69	/* Battery ManufacturerName string start byte. */
    #define INDEX_BATTERY_DEVNAME_LENG	0x74	/* Battery DeviceName string length. */
    #define INDEX_BATTERY_DEVNAME_START	0x75	/* Battery DeviceName string start byte. */
    #define INDEX_BATTERY_DEVCHEM_LENG	0x7C	/* Battery DeviceChemitry string length. */
    #define INDEX_BATTERY_DEVCHEM_START	0x7D	/* Battery DeviceChemitry string start byte. */
    #define INDEX_BATTERY_MFINFO_LENG	0x81	/* Battery ManufacturerInfo string length. */
    #define INDEX_BATTERY_MFINFO_START	0x82	/* Battery ManufacturerInfo string start byte. */
    #define INDEX_BATTERY_CELLCNT_START	0x95    /* Battery packaging fashion string start byte(=4). Unit: ASCII. */
    #define BATTERY_CELLCNT_LENG	  	4		/* Battery packaging fashion string size. */
    #define FLAG_BAT_CELL_3S1P 		"3S1P"
    #define BIT_BATTERY_CURRENT_PN		7       /* Battery current sign is positive or negative */
    #define BIT_BATTERY_CURRENT_PIN		0x07	/* Battery current sign is positive or negative.*/
    /* <<< End Battery */
    #define MASK(x)	(1 << x)
    #define INDEX_STOPCHG_STATUS	0xA1	/* Read currently stop charge status. */
    #define INDEX_POWER_STATUS		0xA2	/* Read current power status. */
    #define INDEX_DEVICE_STATUS		0xA3	/* Read Current Device Status */
    #define INDEX_SHUTDOWN_ID		0xA4	/* Read Shutdown ID */
    #define INDEX_SYSTEM_CFG		0xA5		/* Read System config */
    #define INDEX_VOLUME_LEVEL		0xA6		/* Read Volume Level command */
    #define INDEX_VOLUME_MAXLEVEL		0xA7		/* Volume MaxLevel */   
    /* Camera control */
    #define INDEX_CAM_STSCTRL			0xAA

    /* data destroy led control */
    #define INDEX_DATA_DESTROY			0xB0
    /* The led of board healthy */
    #define INDEX_BOARD_HEALTHY			0xB1

    #define VOLUME_MAX_LEVEL		0x0A		/* Volume level max is 11 */
    #define BIT_SYSCFG_TPSWITCH		(1 << 0)	/* TouchPad switch */
    #define BIT_SYSCFG_WLANPRES		(1 << 1)	/* WLAN present */
    #define BIT_SYSCFG_NB3GPRES		(1 << 2)	/* 3G present */
    #define BIT_SYSCFG_CAMERAPRES		(1 << 3)	/* Camera Present */
    #define BIT_SYSCFG_VOLCTRLEC		(1 << 4)	/* Volume control by EC */
    #define BIT_SYSCFG_BLCTRLEC		(1 << 5)	/* Backlight control by EC */
    #define BIT_SYSCFG_AUTOBRIGHT		(1 << 7)	/* Auto brightness */
    /* EC_SC input */
    /* EC Status query, by direct read 66h port. */
    #define EC_SMI_EVT		(1 << 6)	/* 1 = SMI event padding */
    #define EC_SCI_EVT		(1 << 5)	/* 1 = SCI event padding */
    #define EC_BURST		(1 << 4)	/* 1 = Controller is in burst mode */
    #define EC_CMD			(1 << 3)	/* 1 = Byte in data register is command */
    #define EC_IBF			(1 << 1)	/* 1 = Input buffer full (data ready for ec) */
    #define EC_OBF			(1 << 0)	/* 1 = Output buffer full (data ready for host) */
    /* SCI Event Number from EC */
    #define MAX_STEP_NUM	16
    #define MAX_FAN_LEVEL	255
    #define SCI_EVENT_NUM_START		SCI_EVENT_NUM_WLAN
    #define SCI_EVENT_NUM_END		SCI_EVENT_RECOVERY

#endif 
#define VAL 1//值
#if VAL
    #define ADDRS_DEF 1//地址定义
    #if ADDRS_DEF
        char *base=NULL;//EC端地址
        char *irq_base=NULL;//EC端地址
        void init_addr(void){//地址注册
        base = ioremap((phys_t)EC_BASE_ADDR,EC_OFFSET_MAX);
        irq_base = ioremap((phys_t)EC_IRQ_BASE_ADDR,EC_IRQ_OFFSET_MAX);
        }
        void exit_addr(void){//地址注销
        iounmap(EC_BASE_ADDR);
        iounmap(EC_IRQ_BASE_ADDR);
        }
    #else
        void *base=EC_BASE_ADDR;//EC端地址
        void *irq_base=EC_IRQ_BASE_ADDR;//EC端地址
    #endif
    #define ENUM_STRUCT_UNION_SET 1//声明
    #ifdef ENUM_STRUCT_UNION_SET
    /* Power information structure */
        typedef struct ec_mod_power_supply_info
        {
            unsigned int ac_in;/* 是否接入AC*/
            unsigned int bat_in;/* Battery是否存在 */
            unsigned int health;/* Battery健康状况 */
            unsigned int design_capacity;/* Battery设计容量 */
            unsigned int design_voltage;/* Battery设计电压 */
            unsigned int full_charged_capacity;/* Battery满电容量 */
            unsigned char manufacture_date[11]; /* Battery生产日期 */
            unsigned char serial_number[8];/* Battery序列号 */
            unsigned char manufacturer_name[12];/* Battery制造商名称，最大11 + 1(长度)字节 */
            unsigned char device_name[8];/* Battery设备名称，最大7 + 1(长度)字节 */
            unsigned int technology;/* Battery技术 */
            unsigned char cell_count;/* Battery单元数 */
            unsigned int voltage_now;/* Battery动态充放电电压 */
            /* Battery 动态充放电平均电流 */
            int current_now;//当前电流
            int current_sign;//电流信号
            int current_average;//平均电流
            unsigned int remain_capacity;/* Battery当前剩余容量 */
            unsigned int remain_capacity_percent;/* Battery当前剩余容量百分比 */
            unsigned int temperature;/* Battery当前温度 */
            unsigned int remain_time;/* Battery当前剩余时间(平均清空时间) */
            unsigned int fullchg_time;/* Battery充满电时间(平均满充电时间) */
            unsigned int charge_status;/* Battery状态 */
            unsigned int cycle_count;/* Battery当前循环计数 (循环计数) */
        }ec_mod_power_supply_info;
        ec_mod_power_supply_info power_supply_info={0};
		ec_mod_power_supply_info *power_info=&power_supply_info;
        enum
        {
            DATA_DESTROY_OFF = 0,
            DATA_DESTROY_ON
        }; 
        enum fan_control_mode {
            FAN_FULL_MODE = 0,
            FAN_MANUAL_MODE = 1,
            FAN_AUTO_MODE = 2,
            FAN_MODE_END
        };
        /* the register operation access struct */
        struct ec_reg {
            u32	addr;	/* 地址为EC flash地址和ACPI命令 */
            u8	index;	/* 索引是ACPI命令的索引 */
            u16	val;	/* val为EC flash数据和EC空间值 */
            u8	flag;	/*不同的访问方法. */
        };
        struct temp_range {
            u8 low;
            u8 high;
            u8 level;
        };
                /* Power supply */
        #define BIT_BAT_POWER_ACIN		(1 << 0)
        #ifndef APM_AC_OFFLINE
        enum
        {
            APM_AC_OFFLINE = 0,
            APM_AC_ONLINE,
            APM_AC_BACKUP,
            APM_AC_UNKNOWN = 0xff
        };
        #endif
        enum APM_BAT_STATUS
        {
            APM_BAT_STATUS_HIGH =		0,
            APM_BAT_STATUS_LOW,
            APM_BAT_STATUS_CRITICAL,
            APM_BAT_STATUS_CHARGING,
            APM_BAT_STATUS_NOT_PRESENT,
            APM_BAT_STATUS_UNKNOWN =	0xff
        };
        enum BAT_REG/* bat_reg_flag */
        {
            BAT_REG_TEMP_FLAG = 1,
            BAT_REG_VOLTAGE_FLAG,
            BAT_REG_CURRENT_FLAG,
            BAT_REG_AC_FLAG,
            BAT_REG_RC_FLAG,
            BAT_REG_FCC_FLAG,
            BAT_REG_ATTE_FLAG,
            BAT_REG_ATTF_FLAG,
            BAT_REG_RSOC_FLAG,
            BAT_REG_CYCLCNT_FLAG
        };
    #endif
    const char *version = EC_VERSION;//版本

    //serio set port
    struct serio * ec_kbc_port=NULL;
    struct serio * ec_mouse_port=NULL;
    //应用访问ec标志
    extern unsigned char app_access_ec_flag;
    //kthread 
    static struct task_struct *tsk=NULL;
    static struct task_struct *task;
    //read val of ec
    unsigned char kbc1,kbc2;
    unsigned char mouse1,mouse2;
    unsigned char kthread1;
    //lock
    extern spinlock_t index_access_lock;
    extern spinlock_t port_access_lock;
    DEFINE_SPINLOCK(index_access_lock);
    DEFINE_SPINLOCK(port_access_lock);
#endif
#define FUNC 1//func声明
#ifdef FUNC
#define ec_write(addr,val)	(*((char*)(base + addr))) = (val)
#define ec_read(addr)	(*((char*)(base + addr)))
/* The general ec index-io port read action */
unsigned char ec_read_all(unsigned char command, unsigned char index);
/* The general ec index-io port write action */
unsigned char i8042_read_status(void);
unsigned char i8042_read_data(void);
#endif
static int send_ec_command(unsigned char command)
{
	int timeout, ret = 0;
	timeout = EC_SEND_TIMEOUT;
	while ((inb(EC_STS_PORT) & EC_IBF) && --timeout)
	if (!timeout) {
		//printk(KERN_ERR "Timeout while sending command 0x%02x to EC!\n", command);
		ret = -1;
		goto out;
	}
        /**************wzk_debug_interface******************/
	//printk("send_ec_command\n");
	//outb(command, EC_CMD_PORT);
	ec_write(EC_CMD_PORT,command); //wzk_debug_interface
out:
	return ret;
}
static int send_ec_data(unsigned char data)
{
	int timeout, ret = 0;
	timeout = EC_SEND_TIMEOUT;
	while ((inb(EC_STS_PORT) & EC_IBF) && --timeout)
	if (!timeout) {
		printk(KERN_ERR "Timeout while sending data 0x%02x to EC!\n", data);
		ret = -1;
		goto out;
	}
        /**************wzk_debug_interface******************/
	//printk("send_ec_data\n");
	//outb(data, EC_DAT_PORT);
	ec_write(EC_DAT_PORT,data);  //wzk_debug_interface

out:
	return ret;
}
static unsigned char recv_ec_data(void)
{
	int timeout;
	unsigned char data;
	timeout = EC_RECV_TIMEOUT;
	while (!(inb(EC_STS_PORT) & EC_OBF) && --timeout)
	if (!timeout) {
		printk(KERN_ERR "Timeout while receiving data from EC! status 0x%x.\n", inb(EC_STS_PORT));
		data = 0;
		goto skip_data;
	}
        /**************wzk_debug_interface******************/
	//printk("recv_ec_data\n");
	//data = inb(EC_DAT_PORT);
	data = ec_read(EC_DAT_PORT); 
	printk("data:0x%x\n",data);

skip_data:
	return data;
}
unsigned char ec_read_all(unsigned char command, unsigned char index)
{
	unsigned char value = 0;
	unsigned long flags;
	int ret = 0;
	spin_lock_irqsave(&index_access_lock, flags);
	ret = send_ec_command(command);
	if (ret < 0) {
		printk(KERN_ERR "Send command fail!\n");
		goto out;
	}
    printk("index:0x%x\n",index);
	ret = send_ec_data(index);
	if (ret < 0) {
		printk(KERN_ERR "Send data fail!\n");
		goto out;
	}
	value = recv_ec_data();
out:
	spin_unlock_irqrestore(&index_access_lock, flags);
	return value;
}
EXPORT_SYMBOL(ec_read_all);
#if POWER_SUPPLY_RUN //供电信息
	int ac=0;
	int bat=0;
        /* Update power_status value */
    static void loongson_power_info_power_status_update(void)
    {
        unsigned int power_status = 0;
        power_status = ec_read_all(CMD_READ_EC,INDEX_POWER_STATUS);
        power_info->ac_in = (power_status & MASK(BIT_POWER_ACPRES)) ?
                        APM_AC_ONLINE : APM_AC_OFFLINE;
        power_info->bat_in = (power_status & MASK(BIT_POWER_BATPRES)) ? 1 : 0;
        if( power_info->bat_in && ((ec_read_all(CMD_READ_EC,INDEX_BATTERY_DC_LOW) | (ec_read_all(CMD_READ_EC,INDEX_BATTERY_DC_HIGH) << 8)) == 0) )
            power_info->bat_in = 0;
        power_info->health = (power_info->bat_in) ?	POWER_SUPPLY_HEALTH_GOOD :
                                POWER_SUPPLY_HEALTH_UNKNOWN;
        if (power_status & (MASK(BIT_POWER_BATL) | MASK(BIT_POWER_BATVL))) {
            power_info->health = POWER_SUPPLY_HEALTH_DEAD;
        }
        if (!power_info->bat_in) {
            power_info->charge_status = POWER_SUPPLY_STATUS_UNKNOWN;
        }
        else {
            if (power_status & MASK(BIT_POWER_BATFCHG)) {
                power_info->charge_status = POWER_SUPPLY_STATUS_FULL;
            }
            else if (power_status & MASK(BIT_POWER_BATCHG)) {
                power_info->charge_status = POWER_SUPPLY_STATUS_CHARGING;
            }
            else if (power_status & MASK(BIT_POWER_TERMINATE)) {
                power_info->charge_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
            }
            else {
                power_info->charge_status = POWER_SUPPLY_STATUS_DISCHARGING;
            }
        }
    }
    /* Get battery static information string */
    static void loongson_bat_get_string(unsigned char index, unsigned char *bat_string)
    {
        unsigned char length, i;
        length = ec_read_all(CMD_READ_EC,index);
        for (i = 0; i < length; i++) {
            *bat_string++ = ec_read(++index);
        }
        *bat_string = '\0';
    }
    /* Get battery static information. */
    static void loongson_power_info_battery_static_update(void)
    {
        unsigned int manufacture_date, bat_serial_number;
        char device_chemistry[5];
        manufacture_date = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_MFD_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_MFD_LOW);
        sprintf(power_info->manufacture_date, "%d-%d-%d", (manufacture_date >> 9) + 1980,
                (manufacture_date & 0x01E0) >> 5, manufacture_date & 0x001F);
        loongson_bat_get_string(INDEX_BATTERY_MFN_LENG, power_info->manufacturer_name);
        loongson_bat_get_string(INDEX_BATTERY_DEVNAME_LENG, power_info->device_name);
        loongson_bat_get_string(INDEX_BATTERY_DEVCHEM_LENG, device_chemistry);
        if ((device_chemistry[2] == 'o') || (device_chemistry[2] == 'O')) {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_LION;
        }
        else if (((device_chemistry[1] = 'h') && (device_chemistry[2] == 'm')) ||
                ((device_chemistry[1] = 'H') && (device_chemistry[2] == 'M'))) {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_NiMH;
        }
        else if ((device_chemistry[2] == 'p') || (device_chemistry[2] == 'P')) {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_LIPO;
        }
        else if ((device_chemistry[2] == 'f') || (device_chemistry[2] == 'F')) {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_LiFe;
        }
        else if ((device_chemistry[2] == 'c') || (device_chemistry[2] == 'C')) {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_NiCd;
        }
        else if (((device_chemistry[1] = 'n') && (device_chemistry[2] == 'm')) ||
                ((device_chemistry[1] = 'N') && (device_chemistry[2] == 'M'))) {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_LiMn;
        }
        else {
            power_info->technology = POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
        }
        bat_serial_number = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_SN_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_SN_LOW);
        snprintf(power_info->serial_number, 8, "%x", bat_serial_number);
        power_info->cell_count = ((ec_read_all(CMD_READ_EC,INDEX_BATTERY_CV_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_CV_LOW)) / 4200;
        power_info->design_capacity = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_DC_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_DC_LOW);
        power_info->design_voltage = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_DV_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_DV_LOW);
        power_info->full_charged_capacity = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_FCC_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_FCC_LOW);
        printk(KERN_INFO "LS3ANB Battery Information:\nManufacturerName: %s, DeviceName: %s, DeviceChemistry: %s\n",
                power_info->manufacturer_name, power_info->device_name, device_chemistry);
        printk(KERN_INFO "SerialNumber: %s, ManufactureDate: %s, CellNumber: %d\n",
                power_info->serial_number, power_info->manufacture_date, power_info->cell_count);
        printk(KERN_INFO "DesignCapacity: %dmAh, DesignVoltage: %dmV, FullChargeCapacity: %dmAh\n",
                power_info->design_capacity, power_info->design_voltage, power_info->full_charged_capacity);
    }
    /* Update battery information handle function. */
    static void loongson_power_battery_info_update(unsigned char bat_reg_flag)
    {
        short bat_info_value = 0;
        switch (bat_reg_flag) {
            /* Update power_info->temperature value */
            case BAT_REG_TEMP_FLAG:
                loongson_power_info_power_status_update();
                bat_info_value = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_TEMP_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_TEMP_LOW);
                power_info->temperature = (power_info->bat_in) ? (bat_info_value / 10 - 273) : 0;
                break;
            /* Update power_info->voltage value */
            case BAT_REG_VOLTAGE_FLAG:
                loongson_power_info_power_status_update();
                bat_info_value = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_VOL_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_VOL_LOW);
                power_info->voltage_now = (power_info->bat_in) ? bat_info_value : 0;
                break;
            /* Update power_info->current_now value */
            case BAT_REG_CURRENT_FLAG:
                loongson_power_info_power_status_update();
                bat_info_value = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_CURRENT_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_CURRENT_LOW);
                power_info->current_now = (power_info->bat_in) ? bat_info_value : 0;
                break;
            /* Update power_info->current_avg value */
            case BAT_REG_AC_FLAG:
                loongson_power_info_power_status_update();
                bat_info_value = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_AC_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_AC_LOW);
                power_info->current_average = (power_info->bat_in) ? bat_info_value : 0;
                break;
            /* Update power_info->remain_capacity value */
            case BAT_REG_RC_FLAG:
                power_info->remain_capacity = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_RC_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_RC_LOW);
                break;
            /* Update power_info->full_charged_capacity value */
            case BAT_REG_FCC_FLAG:
                power_info->full_charged_capacity = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_FCC_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_FCC_LOW);
                break;
            /* Update power_info->remain_time value */
            case BAT_REG_ATTE_FLAG:
                power_info->remain_time = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_ATTE_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_ATTE_LOW);
                break;
            /* Update power_info->fullchg_time value */
            case BAT_REG_ATTF_FLAG:
                power_info->fullchg_time = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_ATTF_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_ATTF_LOW);
                break;
            /* Update power_info->curr_cap value */
            case BAT_REG_RSOC_FLAG:
                power_info->remain_capacity_percent = ec_read_all(CMD_READ_EC,INDEX_BATTERY_CAPACITY);
                break;
            /* Update power_info->cycle_count value */
            case BAT_REG_CYCLCNT_FLAG:
                power_info->cycle_count = (ec_read_all(CMD_READ_EC,INDEX_BATTERY_CYCLECNT_HIGH) << 8) | ec_read_all(CMD_READ_EC,INDEX_BATTERY_CYCLECNT_LOW);
                break;
            default:
                break;
        }
    }
    /* Power supply AC get property handler */
    static enum power_supply_property ec_bat_props[] ={
        POWER_SUPPLY_PROP_STATUS,
        POWER_SUPPLY_PROP_HEALTH,
        POWER_SUPPLY_PROP_PRESENT,
        POWER_SUPPLY_PROP_TECHNOLOGY,
        POWER_SUPPLY_PROP_CYCLE_COUNT,
        POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
        POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
        POWER_SUPPLY_PROP_CURRENT_NOW,
        POWER_SUPPLY_PROP_CURRENT_AVG,
        POWER_SUPPLY_PROP_VOLTAGE_NOW,
        POWER_SUPPLY_PROP_CHARGE_FULL, /* in uAh */
        POWER_SUPPLY_PROP_CHARGE_NOW,
        POWER_SUPPLY_PROP_CAPACITY, /* in percents! */
        POWER_SUPPLY_PROP_TEMP,
        POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
        POWER_SUPPLY_PROP_TIME_TO_FULL_AVG,
        /* Properties of type `const char *' */
        POWER_SUPPLY_PROP_MODEL_NAME,
        POWER_SUPPLY_PROP_MANUFACTURER,
        POWER_SUPPLY_PROP_SERIAL_NUMBER,
    };
    //wzk_debug_interface >>>>
    static int thread_read_ecinfo(void *data) {
        int time_count = 0;
        do {
                msleep(5000);
                    loongson_power_info_power_status_update(); 
                    loongson_power_battery_info_update(BAT_REG_TEMP_FLAG);
                    loongson_power_battery_info_update(BAT_REG_VOLTAGE_FLAG);
                    loongson_power_battery_info_update(BAT_REG_CURRENT_FLAG);
                    loongson_power_battery_info_update(BAT_REG_AC_FLAG);
                    loongson_power_battery_info_update(BAT_REG_RC_FLAG);
                    loongson_power_battery_info_update(BAT_REG_FCC_FLAG);
                    loongson_power_battery_info_update(BAT_REG_ATTE_FLAG);
                    loongson_power_battery_info_update(BAT_REG_ATTF_FLAG);
                    loongson_power_battery_info_update(BAT_REG_RSOC_FLAG);
                    loongson_power_battery_info_update(BAT_REG_CYCLCNT_FLAG);
            //power_info->ac_in = (unsigned short)ec_read_all(CMD_READ_EC, INDEX_POWER_STATUS);
            //power_info->design_capacity = (unsigned short)ec_read_all(CMD_READ_EC, INDEX_BATTERY_CAPACITY);
            //power_info->design_voltage = (unsigned short)ec_read_all(CMD_READ_EC, INDEX_BATTERY_DV_LOW);
            //power_info->full_charged_capacity = (unsigned short)ec_read_all(CMD_READ_EC, INDEX_BATTERY_FCC_LOW);
        }while(!kthread_should_stop());
        return time_count;
    }
    //wzk_debug_interface <<<<
    static int ec_bat_get_property(struct power_supply * pws,
                enum power_supply_property psp, union power_supply_propval * val)
    {
            //printk("ec_bat_get_property\n");
        switch (psp) {
            /* Get battery static information. */
            case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
                val->intval = power_info->design_voltage * 1000; /* mV -> uV */
                break;
            case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
                val->intval = power_info->design_capacity * 1000; /* mAh -> uAh */
                break;
            case POWER_SUPPLY_PROP_MODEL_NAME:
                val->strval = power_info->device_name;
                break;
            case POWER_SUPPLY_PROP_MANUFACTURER:
                val->strval = power_info->manufacturer_name;
                break;
            case POWER_SUPPLY_PROP_SERIAL_NUMBER:
                val->strval = power_info->serial_number;
                break;
            case POWER_SUPPLY_PROP_TECHNOLOGY:
                val->intval = power_info->technology;
                break;
            /* Get battery dynamic information. */
            case POWER_SUPPLY_PROP_STATUS:
                            //loongson_power_info_power_status_update(); //wzk_debug_interface
                val->intval = power_info->charge_status;
                break;
            case POWER_SUPPLY_PROP_PRESENT:
                            //loongson_power_info_power_status_update(); //wzk_debug_interface
                val->intval = power_info->bat_in;
                break;
            case POWER_SUPPLY_PROP_HEALTH:
                            //loongson_power_info_power_status_update(); //wzk_debug_interface
                val->intval = power_info->health;
                break;
            case POWER_SUPPLY_PROP_CURRENT_NOW:
                            //loongson_power_battery_info_update(BAT_REG_CURRENT_FLAG); //wzk_debug_interface
                val->intval = power_info->current_now * 1000; /* mA -> uA */
                break;
            case POWER_SUPPLY_PROP_CURRENT_AVG:
                            //loongson_power_battery_info_update(BAT_REG_AC_FLAG); //wzk_debug_interface
                val->intval = power_info->current_average * 1000; /* mA -> uA */
                break;
            case POWER_SUPPLY_PROP_VOLTAGE_NOW:
                            //loongson_power_battery_info_update(BAT_REG_VOLTAGE_FLAG); //wzk_debug_interface
                val->intval =  power_info->voltage_now * 1000; /* mV -> uV */
                break;
            case POWER_SUPPLY_PROP_CHARGE_NOW:
                            //loongson_power_battery_info_update(BAT_REG_RC_FLAG); //wzk_debug_interface
                val->intval = power_info->remain_capacity * 1000; /* mAh -> uAh */
                break;
            case POWER_SUPPLY_PROP_CAPACITY:
                            //loongson_power_battery_info_update(BAT_REG_RSOC_FLAG); //wzk_debug_interface
                val->intval = power_info->remain_capacity_percent;	/* Percentage */
                break;
            case POWER_SUPPLY_PROP_TEMP:
                            //loongson_power_battery_info_update(BAT_REG_TEMP_FLAG); //wzk_debug_interface
                val->intval = power_info->temperature;	 /* Celcius */
                break;
            case POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG:
                            //loongson_power_battery_info_update(BAT_REG_ATTE_FLAG); //wzk_debug_interface
                if (power_info->remain_time == 0xFFFF) {
                    power_info->remain_time = 0;
                }
                val->intval = power_info->remain_time * 60;  /* seconds */
                break;
            case POWER_SUPPLY_PROP_TIME_TO_FULL_AVG:
                            //loongson_power_battery_info_update(BAT_REG_ATTF_FLAG); //wzk_debug_interface
                if (power_info->fullchg_time == 0xFFFF) {
                    power_info->fullchg_time = 0;
                }
                val->intval = power_info->fullchg_time * 60;  /* seconds */
                break;
            case POWER_SUPPLY_PROP_CHARGE_FULL:
                            //loongson_power_battery_info_update(BAT_REG_FCC_FLAG); //wzk_debug_interface
                val->intval = power_info->full_charged_capacity * 1000;/* mAh -> uAh */
                break;
            case POWER_SUPPLY_PROP_CYCLE_COUNT:
                            //loongson_power_battery_info_update(BAT_REG_CYCLCNT_FLAG); //wzk_debug_interface
                val->intval = power_info->cycle_count;
                break;
            default:
                return -EINVAL;
        }
        return 0;
    }
    /* Power supply Battery device object */
    #if 0
    static struct power_supply_desc ec_bat_desc =
    {
        .name = "ec-bat",
        .type = POWER_SUPPLY_TYPE_BATTERY,
        .properties = ec_bat_props,
        .num_properties = ARRAY_SIZE(ec_bat_props),
        .get_property = ec_bat_get_property,
    };
    static struct power_supply *ec_bat;
    #else
    static struct power_supply ec_bat_desc = 
    {
        .name = "ec-bat",
        .type = POWER_SUPPLY_TYPE_BATTERY,
        .properties = ec_bat_props,
        .num_properties = ARRAY_SIZE(ec_bat_props),
        .get_property = ec_bat_get_property,
    };
    #endif
    /* Power supply AC property object */
    static enum power_supply_property ec_ac_props[] =
    {
        POWER_SUPPLY_PROP_ONLINE,
    };
    static int ec_ac_get_property(struct power_supply * pws,
                enum power_supply_property psp, union power_supply_propval * val)
    {
        switch (psp) {
            case POWER_SUPPLY_PROP_ONLINE:
                            //loongson_power_info_power_status_update(); //wzk_debug_interface
                val->intval = power_info->ac_in;
                break;
            default:
                return -EINVAL;
        }
        return 0;
    }
    /* Power supply AC device object */
    #if 0
    static struct power_supply_desc ec_ac_desc =
    {
        .name = "ec-ac",
        .type = POWER_SUPPLY_TYPE_MAINS,
        .properties = ec_ac_props,
        .num_properties = ARRAY_SIZE(ec_ac_props),
        .get_property = ec_ac_get_property,
    };
    #else 
    static struct power_supply ec_ac_desc =
    {
        .name = "ec-ac",
        .type = POWER_SUPPLY_TYPE_MAINS,
        .properties = ec_ac_props,
        .num_properties = ARRAY_SIZE(ec_ac_props),
        .get_property = ec_ac_get_property,
    };
    #endif
#endif
#define MODULE_SET 1//模块设置
#if MODULE_SET
static int __init ec_init(void)
{
    int ret=0;
    init_addr();
    #if POWER_SUPPLY_RUN
    loongson_power_info_power_status_update();
    if (power_info->bat_in) {
		/* Get battery static information. */
		loongson_power_info_battery_static_update();
	}
	else {
		printk(KERN_ERR "loonhson Laptop Platform Driver: The battery does not exist!!\n");
	}
    ac = power_supply_register(NULL, &ec_ac_desc);
	if (ac) {
		pr_err("%s: failed to register %s\n",__func__, ec_ac_desc.name);
		ret = ac;
		goto failed_ac;
		}
	bat = power_supply_register(NULL, &ec_bat_desc);
	if (bat) {
		pr_err("%s: failed to register %s\n", __func__, ec_bat_desc.name);
		ret = bat;
		goto failed_bat;
	}
            task = kthread_run(thread_read_ecinfo,NULL,"mythread%d",1);
	if(IS_ERR(task)){
		printk("Create thread failed \n");
	}else{
		printk("Create thread Ok \n");
	}
    printk("EC power supply init now\n");
    if(0)
    {
failed_bat:
        power_supply_unregister(&ec_ac_desc);
failed_ac:
        power_supply_unregister(&ec_bat_desc);
    }
    #endif
    return ret;
}
static void __exit ec_exit(void)
{
    #if POWER_SUPPLY_RUN
            kthread_stop(task);
        printk("stop kthread \n");    
        power_supply_unregister (&ec_ac_desc);
        power_supply_unregister (&ec_bat_desc);
        printk("EC power supply exit now\n");
    #endif
    exit_addr();
}
module_init(ec_init);//模块初始化
module_exit(ec_exit);//模块注销
MODULE_LICENSE("GPL");//版权声明
MODULE_AUTHOR("yunfeng");//作者声明
MODULE_DESCRIPTION("For study EC power module");//模块描述
MODULE_ALIAS("host_ec");//模块别名
MODULE_VERSION("1.2");//模块版本
#endif
