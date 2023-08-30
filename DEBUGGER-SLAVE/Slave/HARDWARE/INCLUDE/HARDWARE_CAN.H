/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-13 20:06:00
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

#ifndef _HARDWARE_CAN_H_
#define _HARDWARE_CAN_H_

#include "AE_GLOBAL.H"
#include "AE_REG.H"
#include "AE_CONFIG.H"
#include "KERNEL_MEMORY.H"
#include "TEST_CONFIG.H"

#define can_read(index,addr)             (*((VBYTEP )(0x6800 + 0x400*index + addr)))
#define can_write(index,addr,value)      ((*((VBYTEP )(0x6800 + 0x400*index + addr))) = (value))

#define Extended_frame HEX01
#define Standard_frame HEX00


#define CAN0  		HEX00
#define CAN1  		HEX01
#define CAN2  		HEX02
#define CAN3  		HEX03

/*****中断类型******/
#define	beie	HEX07	//总线错误中断使能
#define	alie	HEX06	//仲裁丢失中断使能
#define	epie	HEX05	//错误消极中断使能
#define	wuie	HEX04	//唤醒中断使能
#define	doie	HEX03	//数据溢出中断使能
#define	eie		HEX02	//错误报警中断使能
#define	tie		HEX01	//发送中断使能
#define	rie		HEX00	//接收中断使能


#define _5kbs		0x3f7f
#define _10kbs  	0x377c
#define _20kbs		0x2875
#define _40kbs		0x0d7c
#define _50kbs		0x2830
#define _80kbs		0x067c
#define _100kbs		0x2800
#define _125kbs		0x0674
#define _200kbs		0x027a
#define _250kbs		0x0640
#define _400kbs		0x0175
#define _500kbs		0x007f
#define _666kbs		0x0078
#define _800kbs		0x0075
#define _1000kbs	0x0072


extern VBYTE CAN0_ReadBuff[8];
extern VBYTE CAN0_WriteBuff[8];
extern VBYTE CAN1_ReadBuff[8];
extern VBYTE CAN1_WriteBuff[8];
extern VBYTE CAN2_ReadBuff[8];
extern VBYTE CAN2_WriteBuff[8];
extern VBYTE CAN3_ReadBuff[8];
extern VBYTE CAN3_WriteBuff[8];
extern uint8_t CAN_DATA[8];
/*函数声明*/
void CAN0_Init(uint16_t baudrate,uint32_t acr,uint32_t amr);
void CAN1_Init(uint16_t baudrate,uint32_t acr,uint32_t amr);
void CAN2_Init(uint16_t baudrate,uint32_t acr,uint32_t amr);
void CAN3_Init(uint16_t baudrate,uint32_t acr,uint32_t amr);
BYTE  Can_Int_Enable(BYTE  channel, BYTE  int_type);
BYTE  Can_Int_Disable(BYTE  channel, BYTE  int_type);
BYTE  Can_Int_Enable_Read(BYTE  channel, BYTE  int_type);
BYTE  Can_Int_Status(BYTE  channel, BYTE  int_type);
void CAN_Send(void);
void CAN_Printf_Receive(void);
void CAN0_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data);
void CAN1_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data);
void CAN2_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data);
void CAN3_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data);
void CAN0_ReceiveData(uint8_t *Rbuff);
void CAN1_ReceiveData(uint8_t *Rbuff);
void CAN2_ReceiveData(uint8_t *Rbuff);
void CAN3_ReceiveData(uint8_t *Rbuff);
#endif
