/*
 * @Author: daweslinyu daowes.ly@qq.com
 * @Date: 2023-08-04 14:10:21
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-08-07 18:29:27
 * @FilePath: /Slave/TOOLS/drive/super_io_ec.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <linux/module.h>//模块信息，最基本的文件，支持动态添加和卸载模块
#include <linux/init.h>//初始化、退出等信息
#include <linux/errno.h>//错误
#include <linux/types.h>//定义
#include <linux/interrupt.h>//中断
#include <linux/ioport.h>//io口
#include <linux/serial.h>//串行总线
#include <linux/parport.h>//并口
#include <linux/parport_pc.h>//并口
#include <linux/termios.h>//终端
#include <linux/tty.h>//tty子系统
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
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
#include <linux/fb.h>//帧缓冲驱动

#include <asm/delay.h>//处理器延时
#include <asm/io.h>	//基本输入输出文件
#include <asm/hardware.h>	//硬件电路s
#include <asm/superio.h>	//super i/o

#include "ec_wpce775l.h"
//defined logical device number register
#define UARTA_LDN                   0x01
#define UARTA_IRQ                   4
#define UARTA_ADDR                  0x3F8

#define UARTB_IR_LDN                   0x02
#define UARTB_IR_IRQ                   3
#define UARTB_IR_ADDR                  0x2F8

#define HWMON_LDN                   0x03
#define HWMON_IRQ                   0
#define HWMON_ADDR                  0x2F8

#define HWUC_LDN                    0x04 
#define HWUC_IRQ                   0
#define HWUC_ADDR                  0x2F8

#define UART_TXRX                   0x0
#define UART_DLL                    0x0
#define UART_DLH                    0x1
#define UART_IER                    0x1
#define UART_IIR                    0x2
#define UART_FCR                   0x2
#define UART_LCR                   0x3

#define UART_MICR                   0x4
#define UART_LSR                   0x5
#define UART_MSR                   0x6





#define KBC_MOUSE_LDN               0x05
#define KBC_KEYBOARD_LDN            0x06 
#define HWUC_LDN                    0x08 

