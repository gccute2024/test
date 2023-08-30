/*
 * @Author: Maple
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-13 20:08:01
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

#include "HARDWARE_CAN.H"

#if (defined(AE102)||defined(AE103))

uint8_t CAN_DATA[8]={0,1,2,3,4,5,6,7};
VBYTE CAN0_ReadBuff[8]={0};
VBYTE CAN0_WriteBuff[8]={0};
VBYTE CAN1_ReadBuff[8]={0};
VBYTE CAN1_WriteBuff[8]={0};
VBYTE CAN2_ReadBuff[8]={0};
VBYTE CAN2_WriteBuff[8]={0};
VBYTE CAN3_ReadBuff[8]={0};
VBYTE CAN3_WriteBuff[8]={0};

/**
 * @brief CAN中断使能
 *
 * @param    channel   		通道选择 可选参数：CAN0 CAN1 CAN2 CAN3
 * @param    int_type    	中断类型 可选参数：
 * 
 * @return   无
 */
BYTE  Can_Int_Enable(BYTE  channel, BYTE  int_type)
{
	switch(channel)
	{
		case CAN0:
			CAN0_IER |= (0x1<<int_type);
			return 0;
		case CAN1:
			CAN1_IER |= (0x1<<int_type);
			return 0;
		case CAN2:
			CAN2_IER |= (0x1<<int_type);
			return 0;
		case CAN3:
			CAN3_IER |= (0x1<<int_type);
			return 0;
		default:
			return -1;
	}
}

BYTE  Can_Int_Disable(BYTE  channel, BYTE  int_type)
{
	switch(channel)
	{
		case CAN0:
			CAN0_IER &= ~(0x1<<int_type);
			return 0;
		case CAN1:
			CAN1_IER &= ~(0x1<<int_type);
			return 0;
		case CAN2:
			CAN2_IER &= ~(0x1<<int_type);
			return 0;
		case CAN3:
			CAN3_IER &= ~(0x1<<int_type);
			return 0;
		default:
			return -1;
	}
}

BYTE  Can_Int_Enable_Read(BYTE  channel, BYTE  int_type)
{
	switch(channel)
	{
		case CAN0:
			return ((CAN0_IR & (0x1<<int_type)) != 0);
		case CAN1:
			return ((CAN1_IR & (0x1<<int_type)) != 0);
		case CAN2:
			return ((CAN2_IR & (0x1<<int_type)) != 0);
		case CAN3:
			return ((CAN3_IR & (0x1<<int_type)) != 0);
		default:
			return -1;
	}
}

BYTE  Can_Int_Status(BYTE  channel, BYTE  int_type)
{
	switch(channel)
	{
		case CAN0:
			return ((CAN0_IER & (0x1<<int_type)) != 0);
		case CAN1:
			return ((CAN1_IER & (0x1<<int_type)) != 0);
		case CAN2:
			return ((CAN2_IER & (0x1<<int_type)) != 0);
		case CAN3:
			return ((CAN3_IER & (0x1<<int_type)) != 0);
		default:
			return -1;
	}
}


/**
 * @brief CAN0初始化函数
 *
 * @param    baudrate   波特率配置 可选参数：_5kbs _10kbs...
 * @param    acr        验证码
 * @param    amr        验证掩码
 * 
 * @return   无
 */
void CAN0_Init(uint16_t baudrate,uint32_t acr,uint32_t amr)
{
	uint8_t reg5=0;
	uint8_t reg6=0;
	reg5=(baudrate>>8)&0xff;
	reg6=baudrate&0xff;
	CAN0_MOD=0x9;		//单滤波 进入复位模式
	CAN0_BTR0=reg5;
	CAN0_BTR1=reg6;		//设置波特率
	CAN0_ACR3=(acr>>24)&0xff;
	CAN0_ACR2=(acr>>16)&0xff;
	CAN0_ACR1=(acr>>8)&0xff;
	CAN0_ACR0=(acr)&0xff;
	dprint("CAN0_ACR1:0x%x\n",CAN0_ACR0);
	dprint("CAN0_ACR2:0x%x\n",CAN0_ACR1);
	dprint("CAN0_ACR3:0x%x\n",CAN0_ACR2);
	dprint("CAN0_ACR4:0x%x\n",CAN0_ACR3);

	CAN0_AMR3=(amr>>24)&0xff;
	CAN0_AMR2=(amr>>16)&0xff;
	CAN0_AMR1=(amr>>8)&0xff;
	CAN0_AMR0=(amr)&0xff;

	dprint("CAN0_AMR0:0x%x\n",CAN0_AMR0);
	dprint("CAN0_AMR1:0x%x\n",CAN0_AMR1);
	dprint("CAN0_AMR2:0x%x\n",CAN0_AMR2);
	dprint("CAN0_AMR3:0x%x\n",CAN0_AMR3);

	CAN0_MOD&=~0x1;		//进入工作模式
}

/**
 * @brief CAN1初始化函数
 *
 * @param    baudrate   波特率配置 可选参数：_5kbs _10kbs...
 * @param    acr        验证码
 * @param    amr        验证掩码
 * 
 * @return   无
 */
void CAN1_Init(uint16_t baudrate,uint32_t acr,uint32_t amr)
{
	uint8_t reg5=0;
	uint8_t reg6=0;
	reg5=(baudrate>>8)&0xff;
	reg6=baudrate&0xff;
	CAN1_MOD=0x9;		//单滤波 进入复位模式
	CAN1_BTR0=reg5;
	CAN1_BTR1=reg6;		//设置波特率
	CAN1_ACR3=(acr>>24)&0xff;
	CAN1_ACR2=(acr>>16)&0xff;
	CAN1_ACR1=(acr>>8)&0xff;
	CAN1_ACR0=(acr)&0xff;

	CAN1_AMR3=(amr>>24)&0xff;
	CAN1_AMR2=(amr>>16)&0xff;
	CAN1_AMR1=(amr>>8)&0xff;
	CAN1_AMR0=(amr)&0xff;

	CAN1_MOD&=~0x1;		//进入工作模式
}

/**
 * @brief CAN2初始化函数
 *
 * @param    baudrate   波特率配置 可选参数：_5kbs _10kbs...
 * @param    acr        验证码
 * @param    amr        验证掩码
 * 
 * @return   无
 */
void CAN2_Init(uint16_t baudrate,uint32_t acr,uint32_t amr)
{
	uint8_t reg5=0;
	uint8_t reg6=0;
	reg5=(baudrate>>8)&0xff;
	reg6=baudrate&0xff;
	CAN2_MOD=0x9;		//单滤波 进入复位模式
	CAN2_BTR0=reg5;
	CAN2_BTR1=reg6;		//设置波特率
	CAN2_ACR3=(acr>>24)&0xff;
	CAN2_ACR2=(acr>>16)&0xff;
	CAN2_ACR1=(acr>>8)&0xff;
	CAN2_ACR0=(acr)&0xff;

	CAN2_AMR3=(amr>>24)&0xff;
	CAN2_AMR2=(amr>>16)&0xff;
	CAN2_AMR1=(amr>>8)&0xff;
	CAN2_AMR0=(amr)&0xff;

	CAN2_MOD&=~0x1;		//进入工作模式
}

/**
 * @brief CAN3初始化函数
 *
 * @param    baudrate   波特率配置 可选参数：_5kbs _10kbs...
 * @param    acr        验证码
 * @param    amr        验证掩码
 * 
 * @return   无
 */
void CAN3_Init(uint16_t baudrate,uint32_t acr,uint32_t amr)
{
	uint8_t reg5=0;
	uint8_t reg6=0;
	reg5=(baudrate>>8)&0xff;
	reg6=baudrate&0xff;
	CAN3_MOD=0x9;		//单滤波 进入复位模式
	CAN3_BTR0=reg5;
	CAN3_BTR1=reg6;		//设置波特率
	CAN3_ACR3=(acr>>24)&0xff;
	CAN3_ACR2=(acr>>16)&0xff;
	CAN3_ACR1=(acr>>8)&0xff;
	CAN3_ACR0=(acr)&0xff;

	CAN3_AMR3=(amr>>24)&0xff;
	CAN3_AMR2=(amr>>16)&0xff;
	CAN3_AMR1=(amr>>8)&0xff;
	CAN3_AMR0=(amr)&0xff;

	CAN3_MOD&=~0x1;		//进入工作模式
}

//打印接收到的数据
void CAN_Printf_Receive(void)
{
  #if CAN0_Receive
  dprint("SR=%x\n",can_read(0,0x02));
  for(BYTE  i=0;i<8;i++)
  {
    if(i==0)
    {dprint("CAN0_Receivedata:");}
    dprint(" %x",CAN0_ReadBuff[i]);
    if(i==7)
    {dprint("\n");}
  }
  #endif


  #if CAN1_Receive
  dprint("SR=%x\n",can_read(1,0x02));
  for(BYTE  i=0;i<8;i++)
  {
     if(i==0)
     {dprint("CAN1_Receivedata:");}
     dprint(" %x",CAN1_ReadBuff[i]);
     if(i==7)
     {dprint("\n");}
  }
  #endif
  
   #if CAN2_Receive
  dprint("SR=%x\n",can_read(2,0x02));
  for(BYTE  i=0;i<8;i++)
  {
    if(i==0)
    {dprint("CAN2_Receivedata:");}
    dprint(" %x",CAN2_ReadBuff[i]);
    if(i==7)
    {dprint("\n");}
  }
  #endif

  #if CAN3_Receive
  dprint("SR=%x\n",can_read(3,0x02));
  for(BYTE  i=0;i<8;i++)
  {
    if(i==0)
    {dprint("CAN3_Receivedata:");}
    dprint(" %x",CAN2_ReadBuff[i]);
    if(i==7)
    {dprint("\n");}
  }
  #endif
}

/***********后续增加代码有待整理***************/

/***********后续增加代码有待整理***************/

void CAN0_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data)
{
	uint8_t id0=0;
	uint8_t id1=0;
	uint8_t id2=0;
	uint8_t id3=0;
	while(!(CAN0_SR&0x4));  //等待发送缓冲可用
	switch (num)
	{
	case 1:
		CAN0_TXB5=*data;
		break;
	case 2:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		break;
	case 3:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		CAN0_TXB7=*(data+2);
		break;
	case 4:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		CAN0_TXB7=*(data+2);
		CAN0_TXB8=*(data+3);
		break;
	case 5:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		CAN0_TXB7=*(data+2);
		CAN0_TXB8=*(data+3);
		CAN0_TXB9=*(data+4);
		break;
	case 6:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		CAN0_TXB7=*(data+2);
		CAN0_TXB8=*(data+3);
		CAN0_TXB9=*(data+4);
		CAN0_TXB10=*(data+5);
		break;
	case 7:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		CAN0_TXB7=*(data+2);
		CAN0_TXB8=*(data+3);
		CAN0_TXB9=*(data+4);
		CAN0_TXB10=*(data+5);
		CAN0_TXB11=*(data+6);
		break;
	case 8:
		CAN0_TXB5=*data;
		CAN0_TXB6=*(data+1);
		CAN0_TXB7=*(data+2);
		CAN0_TXB8=*(data+3);
		CAN0_TXB9=*(data+4);
		CAN0_TXB10=*(data+5);
		CAN0_TXB11=*(data+6);
		CAN0_TXB12=*(data+7);
		break;
	default:
		dprint("发送数据帧大小为0~8字节\n");
		break;
	}
	if(ff==Extended_frame)	//扩展帧
	{
		id0=id&0x1f;
		id1=(id>>5)&0xff;
		id2=(id>>13)&0xff;
		id3=(id>>21)&0xff;
		CAN0_TXB0=0x80+num;
		CAN0_TXB1=(id0<<3);
		CAN0_TXB2=id1;
		CAN0_TXB3=id2;
		CAN0_TXB4=id3;
	}
	else					//标准帧
	{
		id0=id&0x07;
		id1=(id>>3)&0xff;
		CAN0_TXB0=num;
		CAN0_TXB3=(id0<<5);
		CAN0_TXB4=id1;
	}
	CAN0_CMR=0x1;
	while((CAN0_SR&0x20));
	//CAN0_CMR=0x2;
	dprint("数据发送完毕\n");
}


void CAN1_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data)
{
	uint8_t id0=0;
	uint8_t id1=0;
	uint8_t id2=0;
	uint8_t id3=0;
	while(!(CAN1_SR&0x4));  //等待发送缓冲可用
	switch (num)
	{
	case 1:
		CAN1_TXB5=*data;
		break;
	case 2:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		break;
	case 3:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		CAN1_TXB7=*(data+2);
		break;
	case 4:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		CAN1_TXB7=*(data+2);
		CAN1_TXB8=*(data+3);
		break;
	case 5:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		CAN1_TXB7=*(data+2);
		CAN1_TXB8=*(data+3);
		CAN1_TXB9=*(data+4);
		break;
	case 6:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		CAN1_TXB7=*(data+2);
		CAN1_TXB8=*(data+3);
		CAN1_TXB9=*(data+4);
		CAN1_TXB10=*(data+5);
		break;
	case 7:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		CAN1_TXB7=*(data+2);
		CAN1_TXB8=*(data+3);
		CAN1_TXB9=*(data+4);
		CAN1_TXB10=*(data+5);
		CAN1_TXB11=*(data+6);
		break;
	case 8:
		CAN1_TXB5=*data;
		CAN1_TXB6=*(data+1);
		CAN1_TXB7=*(data+2);
		CAN1_TXB8=*(data+3);
		CAN1_TXB9=*(data+4);
		CAN1_TXB10=*(data+5);
		CAN1_TXB11=*(data+6);
		CAN1_TXB12=*(data+7);
		break;
	default:
		dprint("发送数据帧大小为0~8字节\n");
		break;
	}
	if(ff==Extended_frame)	//扩展帧
	{
		id0=id&0x1f;
		id1=(id>>5)&0xff;
		id2=(id>>13)&0xff;
		id3=(id>>21)&0xff;
		CAN1_TXB0=0x80+num;
		CAN1_TXB1=(id0<<3);
		CAN1_TXB2=id1;
		CAN1_TXB3=id2;
		CAN1_TXB4=id3;
	}
	else					//标准帧
	{
		id0=id&0x07;
		id1=(id>>3)&0xff;
		CAN1_TXB0=num;
		CAN1_TXB3=(id0<<5);
		CAN1_TXB4=id1;
	}
	CAN1_CMR=0x1;
	while((CAN1_SR&0x20));
	//CAN1_CMR=0x2;
	dprint("数据发送完毕\n");
}


void CAN2_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data)
{
	uint8_t id0=0;
	uint8_t id1=0;
	uint8_t id2=0;
	uint8_t id3=0;
	while(!(CAN2_SR&0x4));  //等待发送缓冲可用
	switch (num)
	{
	case 1:
		CAN2_TXB5=*data;
		break;
	case 2:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		break;
	case 3:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		CAN2_TXB7=*(data+2);
		break;
	case 4:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		CAN2_TXB7=*(data+2);
		CAN2_TXB8=*(data+3);
		break;
	case 5:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		CAN2_TXB7=*(data+2);
		CAN2_TXB8=*(data+3);
		CAN2_TXB9=*(data+4);
		break;
	case 6:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		CAN2_TXB7=*(data+2);
		CAN2_TXB8=*(data+3);
		CAN2_TXB9=*(data+4);
		CAN2_TXB10=*(data+5);
		break;
	case 7:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		CAN2_TXB7=*(data+2);
		CAN2_TXB8=*(data+3);
		CAN2_TXB9=*(data+4);
		CAN2_TXB10=*(data+5);
		CAN2_TXB11=*(data+6);
		break;
	case 8:
		CAN2_TXB5=*data;
		CAN2_TXB6=*(data+1);
		CAN2_TXB7=*(data+2);
		CAN2_TXB8=*(data+3);
		CAN2_TXB9=*(data+4);
		CAN2_TXB10=*(data+5);
		CAN2_TXB11=*(data+6);
		CAN2_TXB12=*(data+7);
		break;
	default:
		dprint("发送数据帧大小为0~8字节\n");
		break;
	}
	if(ff==Extended_frame)	//扩展帧
	{
		id0=id&0x1f;
		id1=(id>>5)&0xff;
		id2=(id>>13)&0xff;
		id3=(id>>21)&0xff;
		CAN2_TXB0=0x80+num;
		CAN2_TXB1=(id0<<3);
		CAN2_TXB2=id1;
		CAN2_TXB3=id2;
		CAN2_TXB4=id3;
	}
	else					//标准帧
	{
		id0=id&0x07;
		id1=(id>>3)&0xff;
		CAN2_TXB0=num;
		CAN2_TXB3=(id0<<5);
		CAN2_TXB4=id1;
	}
	CAN2_CMR=0x1;
	while((CAN2_SR&0x20));
	//CAN2_CMR=0x2;
	dprint("数据发送完毕\n");
}


void CAN3_SendData(uint8_t ff,uint32_t id,uint8_t num,uint8_t *data)
{
	uint8_t id0=0;
	uint8_t id1=0;
	uint8_t id2=0;
	uint8_t id3=0;
	while(!(CAN3_SR&0x4));  //等待发送缓冲可用
	switch (num)
	{
	case 1:
		CAN3_TXB5=*data;
		break;
	case 2:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		break;
	case 3:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		CAN3_TXB7=*(data+2);
		break;
	case 4:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		CAN3_TXB7=*(data+2);
		CAN3_TXB8=*(data+3);
		break;
	case 5:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		CAN3_TXB7=*(data+2);
		CAN3_TXB8=*(data+3);
		CAN3_TXB9=*(data+4);
		break;
	case 6:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		CAN3_TXB7=*(data+2);
		CAN3_TXB8=*(data+3);
		CAN3_TXB9=*(data+4);
		CAN3_TXB10=*(data+5);
		break;
	case 7:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		CAN3_TXB7=*(data+2);
		CAN3_TXB8=*(data+3);
		CAN3_TXB9=*(data+4);
		CAN3_TXB10=*(data+5);
		CAN3_TXB11=*(data+6);
		break;
	case 8:
		CAN3_TXB5=*data;
		CAN3_TXB6=*(data+1);
		CAN3_TXB7=*(data+2);
		CAN3_TXB8=*(data+3);
		CAN3_TXB9=*(data+4);
		CAN3_TXB10=*(data+5);
		CAN3_TXB11=*(data+6);
		CAN3_TXB12=*(data+7);
		break;
	default:
		dprint("发送数据帧大小为0~8字节\n");
		break;
	}
	if(ff==Extended_frame)	//扩展帧
	{
		id0=id&0x1f;
		id1=(id>>5)&0xff;
		id2=(id>>13)&0xff;
		id3=(id>>21)&0xff;
		CAN3_TXB0=0x80+num;
		CAN3_TXB1=(id0<<3);
		CAN3_TXB2=id1;
		CAN3_TXB3=id2;
		CAN3_TXB4=id3;
	}
	else					//标准帧
	{
		id0=id&0x07;
		id1=(id>>3)&0xff;
		CAN3_TXB0=num;
		CAN3_TXB3=(id0<<5);
		CAN3_TXB4=id1;
	}
	CAN3_CMR=0x1;
	while((CAN3_SR&0x20));
	//CAN3_CMR=0x2;
	dprint("数据发送完毕\n");
}

void CAN0_ReceiveData(uint8_t *Rbuff)
{
	for(BYTE  i=0;i<8;i++)
	{
		Rbuff[i] = (*((VBYTEP )(CAN0_REG_ADDR(CAN_RXB5_OFFECT+i))));
	}
}

void CAN1_ReceiveData(uint8_t *Rbuff)
{
	for(BYTE  i=0;i<8;i++)
	{
		Rbuff[i] = (*((VBYTEP )(CAN1_REG_ADDR(CAN_RXB5_OFFECT+i))));
	}
}

void CAN2_ReceiveData(uint8_t *Rbuff)
{
	for(BYTE  i=0;i<8;i++)
	{
		Rbuff[i] = (*((VBYTEP )(CAN2_REG_ADDR(CAN_RXB5_OFFECT+i))));
	}
}

void CAN3_ReceiveData(uint8_t *Rbuff)
{
	for(BYTE  i=0;i<8;i++)
	{
		Rbuff[i] = (*((VBYTEP )(CAN3_REG_ADDR(CAN_RXB5_OFFECT+i))));
	}
}
#endif