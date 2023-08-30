/*
 * @Author: Yangxiaolong
 * @LastEditors: Yangxiaolong
 * @LastEditTime: 2023-08-09 10:29:28
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

#include "KERNEL_CEC.H"

//*****************************************************************************
//
//  CEC Write in two byte
// !!!the cec only supported 16-bit write
//  parameter :
//              value: the value written to CEC_register
//              offset: CEC_register_offset
//  return :
//      none
//
//*****************************************************************************
void CEC_Write(WORD value,BYTE offset){
    CEC_REG(offset) = value;
	return;
}
//*****************************************************************************
//
//  CEC read in two byte
// !!!the cec only supported 16-bit read 
//  parameter :
//              offset: CEC_register_offset
//  return :
//              none
//
//*****************************************************************************
WORD CEC_Read(BYTE offset){
	return CEC_REG(offset);
}

//*****************************************************************************
//
//  CEC initiator Initialization function
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_initiator_init(void){
    //enable the interrup
    CEC_Write(CEC_Int_Send_Frame|CEC_Int_Send_Block,CEC_IER_OFFSET);//enable the send int
    //CEC_Write(0x95F,CEC_CPSR_OFFSET);  //103 no need 
    CEC_Write(CEC_Ctr_sdb|CEC_Ctr_en,CEC_CTRL_OFFSET);//enable the broadcast frame send

}
//*****************************************************************************
//
//  CEC follower Initialization function
//  parameter :
//              none
//  return :
//              none
//******************************************************************************
void CEC_follower_init(void){
    //enable the interrup
    CEC_Write(CEC_Int_Receive_Head|CEC_Int_Receive_Block|CEC_Int_Receive_Frame,CEC_IER_OFFSET);
    //CEC_Write(0x95F,CEC_CPSR_OFFSET);  //103 no need 
    CEC_Write(CEC_Initiator_address,CEC_ADDR_OFFSET);//"Initiator_address" as device_addr while follower
    CEC_Write(CEC_Ctr_eba|CEC_Ctr_rad|CEC_Ctr_en,CEC_CTRL_OFFSET);//receive data no matter address
}
//*****************************************************************************
//
//  CEC  frame sending that include one "head block" and lots of "data blocks"
//  parameter :
//              none
//  return :
//              none
//******************************************************************************

void CEC_Frame_send(char *data,BYTE len){
    if(len > 15){
        dprint("the frame too long !! \n");
        return;
    }
    DWORD head = 0,temp = 0;
    head = (CEC_Destination_address<<4)|CEC_Initiator_address;
    CEC_Write(head,CEC_DA_OFFSET);

    for(int i =0; i < len - 1; i++){
        CEC_Write(data[i],CEC_DA_OFFSET);
    }
    temp = data[len-1]|CEC_send_EOM;
    CEC_Write(temp,CEC_DA_OFFSET);

    temp = CEC_Read(CEC_CTRL_OFFSET)&0x3F;
    CEC_Write(temp|CEC_Ctr_send,CEC_DA_OFFSET);// Send fifo
    return;
}

