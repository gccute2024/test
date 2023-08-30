/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-04-03 14:59:18
 * @Description: This is about the  national crypto algorithm implementation
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

#include <KERNEL_SMx.H>

#ifdef AE103
//SM2 elipse
#if 1
VBYTE SM2_ID[]="L-silicon";
VBYTE SM2_DATA[]="SPK32AE103";
DWORD SM2_Elipse_P[8]={
    0xFFFFFFFF,
    0xFFFFFFFF,
    0x00000000,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE};
DWORD SM2_Elipse_A[8]={
    0xFFFFFFFC,
    0xFFFFFFFF,
    0x00000000,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE};
DWORD SM2_Elipse_B[8]={
    0x4D940E93,
    0xDDBCBD41,
    0x15AB8F92,
    0xF39789F5,
    0xCF6509A7,
    0x4D5A9E4B,
    0x9D9F5E34,
    0x28E9FA9E};
DWORD SM2_Elipse_N[8]={
    0x39D54123,
    0x53BBF409,
    0x21C6052B,
    0x7203DF6B,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE};
spoint SM2_Elipse_G={
    .x[0]=0x334C74C7,
    .x[1]=0x715A4589,
    .x[2]=0xF2660BE1,
    .x[3]=0x8FE30BBF,
    .x[4]=0x6A39C994,
    .x[5]=0x5F990446,
    .x[6]=0x1F198119,
    .x[7]=0x32C4AE2C,
    .y[0]=0x2139F0A0,
    .y[1]=0x02DF32E5,
    .y[2]=0xC62A4740,
    .y[3]=0xD0A9877C,
    .y[4]=0x6B692153,
    .y[5]=0x59BDCEE3,
    .y[6]=0xF4F6779C,
    .y[7]=0xBC3736A2};
#endif

//SM4
static DWORD SMx_Change(DWORD data)
{
    return (((data&0x000000ff)<<24)|((data&0x0000ff00)<<8)|((data&0x00ff0000)>>8)|((data&0xff000000)>>24));
}
static void SM4_Key_Set(DWORDP key)
{
		SM4_MODE=SM4_MODE_KEYEX;
		SM4_KEY0=SMx_Change(*(key+0));
        SM4_KEY1=SMx_Change(*(key+1));
        SM4_KEY2=SMx_Change(*(key+2));
        SM4_KEY3=SMx_Change(*(key+3));
        SM4_STATUS|=SM4_STATUS_DONE;
        SM4_CTRL&=~SM4_CTRL_INTEN;
		SM4_CTRL|=SM4_CTRL_START;
		while (!(SM4_STATUS&SM4_STATUS_DONE)){}
}
static void SM4_Data_IN(DWORDP input)
{
		SM4_DATA0=SMx_Change(*(input+0));
        SM4_DATA1=SMx_Change(*(input+1));
        SM4_DATA2=SMx_Change(*(input+2));
        SM4_DATA3=SMx_Change(*(input+3));
}
static void SM4_XORDR_IN(DWORDP xordr)
{
        SM4_XORD0=SMx_Change(*(xordr+0));
        SM4_XORD1=SMx_Change(*(xordr+1));
        SM4_XORD2=SMx_Change(*(xordr+2));
        SM4_XORD3=SMx_Change(*(xordr+3));

}
static void SM4_Mode_Set(BYTE mode)
{
		if(mode == SM4_ECB_ENCODE)
            SM4_MODE=SM4_MODE_ENDE1;
		else if(mode==SM4_ECB_DECODE)
            SM4_MODE=SM4_MODE_ENDE0;
		else if(mode==SM4_CBC_ENCODE)
            SM4_MODE=SM4_MODE_XOR|SM4_MODE_ENDE1;
        else if(mode==SM4_CBC_DECODE)
            SM4_MODE=SM4_MODE_XOR|SM4_MODE_ENDE0;
}
static void SM4_Data_Out(DWORDP output)
{
		SM4_STATUS|=SM4_STATUS_DONE;
        SM4_CTRL&=~SM4_CTRL_INTEN;
		SM4_CTRL|=SM4_CTRL_START;
		while (!(SM4_STATUS&SM4_STATUS_DONE)){}
		*(output+0)=SMx_Change(SM4_DATA0);
        *(output+1)=SMx_Change(SM4_DATA1);
        *(output+2)=SMx_Change(SM4_DATA2);
        *(output+3)=SMx_Change(SM4_DATA3);
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM4
 * note :
 *          mode:0 ECB加密,1 ECB解码/2 CBC加密/3 CBC解码
 *          input: Plaintext password
 *          key:Extended secret key
 *          output: Ciphertext storage
 *          xordr :lv
 * ************************************************************************************/
/* void SM4_Run(BYTE mode,BYTEP input,BYTEP key,BYTEP output,BYTEP xordr)
{
        SM4_Key_Set((DWORDP)key);//set key
        SM4_Data_IN((DWORDP)input);//input data
        SM4_Mode_Set(mode);
        if(mode == 2 || mode == 3)
        {
            SM4_XORDR_IN((DWORDP)xordr);//Functionality not implemented
        }
        SM4_Data_Out((DWORDP)output);
} */

void SM4_Run(DWORD bit, BYTE mode, BYTEP input, BYTEP key, BYTEP output, BYTEP xordr)
{
    if(bit%128)
    {
        dprint("SM4 data bit size ERROR\n");
        return;
    }
    SM4_Key_Set((DWORDP)key);
    SM4_Mode_Set(mode);
    if(mode == SM4_CBC_ENCODE || mode == SM4_CBC_DECODE)
    {
        SM4_XORDR_IN((DWORDP)xordr);    //set xordr
    }
    for(int i = 0; i * 128 < bit; i++)
    {
        SM4_Data_IN((DWORDP)(input + i * 16));   //input data
        SM4_Data_Out((DWORDP)(output + i * 16)); //output data
    }
}

//SM3

static BYTE data_offset = 0x0;
static void SM3_Auto_Write(DWORD data)
{
    SM3_REG(SM3_DATA0_OFFSET+data_offset) = data;
    data_offset = (data_offset+0x4)%0x40;
    while(!(data_offset||(SM3_INT&0x1))){}
    if(SM3_INT&0x1)SM3_INT = 0x1;
}

static void SM3_Data_In(DWORD bit,DWORDP input)
{
    DWORD dword=bit/DEC32;//double word size
    DWORD rbyte=(bit/HEX08)%HEX04;//Remainder byte
    DWORD rbit=bit%DEC32-rbyte*HEX08;//Remainder bit
    DWORD block=(bit%DEC512)/DEC32;//final block byte size
    uDword buffer;
    DWORD i=0,j=0;
    for(i=0;i<dword;i++)
    {
        buffer.dword=(*(input+i));
        SM3_Auto_Write(buffer.dword);
        //SM3_IN=buffer.dword;
    }
    for(j=0;j<rbyte;j++)
    {
        buffer.byte[j]=*((BYTEP)input+i*8+j);
    }
    rbit%=HEX08;
    if(rbit)buffer.byte[j]=*((BYTEP)input+i*8+j);
    else buffer.byte[j]=0x0;
    for(i=0;i<rbit;i++)
    {
        buffer.byte[j]&=(~(0x80>>i));
    }
    if(rbit||rbyte)
    {
        buffer.byte[j]|=(0x80>>i);
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    else
    {
        buffer.dword=0x80;
        SM3_Auto_Write(buffer.dword);
        block++;
    } 
    if(block >=16)block%=16;
    while (block<15)
    {
        buffer.dword=0;
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    buffer.dword=SMx_Change(bit);
    SM3_Auto_Write(buffer.dword);
    data_offset = 0x0;
}
static void SM3_Data_Out(DWORDP output)
{
    *(output+0)=SMx_Change(SM3_OUT7);
    *(output+1)=SMx_Change(SM3_OUT6);
    *(output+2)=SMx_Change(SM3_OUT5);
    *(output+3)=SMx_Change(SM3_OUT4);
    *(output+4)=SMx_Change(SM3_OUT3);
    *(output+5)=SMx_Change(SM3_OUT2);
    *(output+6)=SMx_Change(SM3_OUT1);
    *(output+7)=SMx_Change(SM3_OUT0);

}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM3
 * note :
 *          bit:data bit size
 *          input: Plaintext password
 *          output: Ciphertext storage
 * ************************************************************************************/
void SM3_Run(DWORD bit,BYTEP input,BYTEP output)
{
    int i,j;
    DWORD wdata;
    SM3_CONIFG|=SM3_CONFIG_ENABLE;//使能sm3   
    SM3_CONIFG&=~SM3_CONFIG_NEW_DATA;//data in
    SM3_Data_In(bit,(DWORDP)input);//data in
    SM3_CONIFG|=SM3_CONFIG_ENABLE|SM3_CONFIG_NEW_DATA;//config run
    while (!(SM3_STATE&SM3_STATE_FINISH)){}
    SM3_Data_Out((DWORDP)output);
}
//SM2
static void SM2_IO(VDWORDP addr1,VDWORDP addr2)
{
    VBYTE i=0;
    while(i<8){*(addr1+i)=*(addr2+i);i++;}
}
static void SM2_Data_IN(DWORDP X1,DWORDP X2,DWORDP Y1,DWORDP Y2,DWORDP K)
{
    if(X1!=NULL)SM2_IO(&SM2_P1X0,(VDWORDP)X1);
    if(X2!=NULL)SM2_IO(&SM2_P2X0,(VDWORDP)X2);
    if(Y1!=NULL)SM2_IO(&SM2_P1Y0,(VDWORDP)Y1);
    if(Y2!=NULL)SM2_IO(&SM2_P2Y0,(VDWORDP)Y2);
    if(K!=NULL)SM2_IO(&SM2_K0,(VDWORDP)K);
}
static void SM2_Data_Out(DWORDP X3,DWORDP Y3)
{
    if(X3!=NULL)SM2_IO((VDWORDP)X3,&SM2_P3X0);
    if(Y3!=NULL)SM2_IO((VDWORDP)Y3,&SM2_P3Y0);
}
static void SM2_ALU(DWORDP X1,DWORDP X2,DWORDP Y1,DWORDP Y2,DWORDP K,DWORD control,DWORDP X3,DWORDP Y3)
{
    SM2_Data_IN(X1,X2,Y1,Y2,K);//input
    SM2_CONTROL=SM2_CONTROL_EN|control;//run
    while (SM2_STATUS!=SM2_STATUS_DONE)//wait
    {
        if(SM2_STATUS==SM2_STATUS_ERROR)
        {
            SM2_Error();
        }
    }
    SM2_Data_Out(X3,Y3);//out
    SM2_CONTROL=SM2_CONTROL_FREE;//free
}

void SM2_N_ADD(DWORDP input1,DWORDP input2,DWORDP output)
{
    SM2_ALU(input1,input2,NULL,NULL,NULL,SM2_CONTROL_MODN_ADD,NULL,output);
}
void SM2_DOT_ADD(spoint input1,spoint input2,spoint *output)
{
    SM2_ALU(input1.x,input2.x,input1.y,input2.y,NULL,SM2_CONTROL_DOT_ADD,output->x,output->y);
    
}
void SM2_DOT_DBL(spoint input,spoint *output)
{
    SM2_ALU(input.x,NULL,input.y,NULL,NULL,SM2_CONTROL_DOT_DBL,output->x,output->y);
}
void SM2_DOT_MUL(spoint input,DWORDP k,spoint *output)
{
    SM2_ALU(input.x,NULL,input.y,NULL,k,SM2_CONTROL_DOT_MUL,output->x,output->y);
}
#if 0
void SM2_SIGN(spoint g,DWORDP k,DWORDP e,DWORDP key,ssign ouutput)
{
    BYTE i = 0;
    DWORD s[8];
    DWORD s0[8];
    DWORD s1[8];
    spoint P0;
    SM2_DOT_MUL(g,k,&P0);//p0 =[k']G
    SM2_N_ADD(e,P0.x,ouutput.r);//r=(e+p0.x)modn
    for (i=0;i<8;i++)
    {
        s0[i]=key[i];
    }
    for (i=0;i<8;i++)
    {
        if(s0[i]!=0xFFFFFFFF)
        {
            s0[i]++;
            break;
        }
        else if(i==7)return;
        else s0[i]=0x0;
    }
    SM2_ALU(s0,NULL,NULL,NULL,NULL,SM2_CONTROL_MODN_INV,NULL,s0);
    SM2_ALU(s0,s1,NULL,NULL,NULL,SM2_CONTROL_MODN_MUL,NULL,s);//s=(1+dA)-1(k-rdA)modn
}  
#endif 
BYTE SM2_VERIFY(DWORDP r,DWORDP s,DWORDP e,spoint g,spoint PA)
{
    VBYTE i=0;   
    DWORD T[8];
    DWORD R[8];
    spoint P0;spoint P1;spoint P2;
    SM2_N_ADD(r,s,T);//t=(r'+s')modn
    SM2_DOT_MUL(g,s,&P0);//p0 =[s']G
    SM2_DOT_MUL(PA,T,&P1);//p1 = [t]PA
    SM2_DOT_ADD(P0,P1,&P2);//P2=P1+P0
    SM2_N_ADD(e,P2.x,R);//R=(e'+P2.x')modn
    for(i=0;i<8;i++){if(r[i]!=R[i])return 1;/*printf("r[%d]=%x,R[%d]=%x\n",i,r[i],i,R[i]);*/}
    return 0;
}
WEAK void SM2_Error(void)
{
    dprint("SM2 error of 0x%x\n",SM2_STATUS);
}
extern spoint SM2Pulib_Key;
extern ssign SM2_signer;
extern DWORD SM2_E[8];
DWORD SM2_K[8]={
    0xEAC1BC21,
    0x6D54B80D,
    0x3CDBE4CE,
    0xEF3CC1FA,
    0xD9C02DCC,
    0x16680F3A,
    0xD506861A,
    0x59276E27};
DWORD SM2Private_Key[8]={
    0x4DF7C5B8,
    0x42FB81EF,
    0x2860B51A,
    0x88939369,
    0xC6D39F95,
    0x3F36E38A,
    0x7B2144B1,
    0x3945208F};//da
#if 0
 spoint SM2Pulib_Key={
    .x[0]=0x56F35020,
    .x[1]=0x6BB08FF3,
    .x[2]=0x1833FC07,
    .x[3]=0x72179FAD,
    .x[4]=0x1E4BC5C6,
    .x[5]=0x50DD7D16,
    .x[6]=0x1E5421A1,
    .x[7]=0x09F9DF31,

    .y[0]=0x2DA9AD13,
    .y[1]=0x6632F607,
    .y[2]=0xF35E084A,
    .y[3]=0x0AED05FB,
    .y[4]=0x8CC1AA60,
    .y[5]=0x2DC6EA71,
    .y[6]=0xE26775A5,
    .y[7]=0xCCEA490C};
 ssign SM2_signer={
    .r[0]=0xEEE720B3,
    .r[1]=0x43AC7EAC,
    .r[2]=0x27D5B741,
    .r[3]=0x5944DA38,
    .r[4]=0xE1BB81A1,
    .r[5]=0x0EEAC513,
    .r[6]=0x48D2C463,
    .r[7]=0xF5A03B06,
    .s[0]=0x85BBC1AA,
    .s[1]=0x840B69C4,
    .s[2]=0x1F7F42D4,
    .s[3]=0xBB9038FD,
    .s[4]=0x0D421CA1,
    .s[5]=0x763182BC,
    .s[6]=0xDF212FD8,
    .s[7]=0xB1B6AA29};
 DWORD SM2_E[8]={
    0xC0D28640,
    0xF4486FDF,
    0x19CE7B31,
    0x17E6AB5A,
    0x534382EB,
    0xACE692ED,
    0xBA45ACCA,
    0xF0B43E94
};
#endif
VBYTE SM2_RUN(void)
{
    VBYTE i=0;   
    //SM2_Key_Set(SM2_Elipse_G,SM2_Elipse_K,SM2Private_Key,&SM2Pulib_Key);//生成公钥
    //SM2_SIGN(SM2_Elipse_G,SM2_K,SM2_E,SM2Private_Key,SM2_signer);  
    return SM2_VERIFY(SM2_signer.r,SM2_signer.s,SM2_E,SM2_Elipse_G,SM2Pulib_Key);
}
//eg:
#if SMx_TEST
BYTE Smx_input[]= {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
BYTE Smx_key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
BYTE Smx_lv[]= {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
BYTE Smx_output[32];
BYTE Sm3_input[]={0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64};
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SMx test code
 * note :
 * ************************************************************************************/

void SM_Test_main(void)
{
    BYTE i=0;
    //SM2 debug
    memset(Smx_output,0,32);
    i=SM2_RUN();
    if(i)dprint("SM2 RUN ERROR\n");
    else dprint("SM2 RUN SUCCEED\n");
    //sm3 debug
    /* dprint("SM3 \n");
    memset(Smx_output,0,32);
    SM3_Run(512,Sm3_input,Smx_output);
    dprint("encryption:\n");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");

    memset(Smx_output,0,32);
    SM3_Run(128,Sm3_input,Smx_output);
    dprint("encryption:\n");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");

    memset(Smx_output,0,32);
    SM3_Run(32,Sm3_input,Smx_output);
    dprint("encryption:\n");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");

    memset(Smx_output,0,32);
    SM3_Run(24,Sm3_input,Smx_output);
    dprint("encryption:\n");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");
    memset(Smx_output,0,32);
    SM3_Run(16,Sm3_input,Smx_output);
    dprint("encryption:\n");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");
    memset(Smx_output,0,32);
    SM3_Run(8,Sm3_input,Smx_output);
    dprint("encryption:\n");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n"); */
    dprint("--------------------------------------------------------------\n");
    //sm4_debug
    dprint("SM4 encryption and decryption:\n");
    memset(Smx_output,0,32);
    SM4_Run(256,SM4_ECB_ENCODE,Smx_input,Smx_key,Smx_output,NULL);
    dprint("ECB encryption:");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");
    SM4_Run(256,SM4_ECB_DECODE,Smx_output,Smx_key,Smx_output,NULL);
    dprint("ECB decryption:");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");

    memset(Smx_output,0,32);
    SM4_Run(256,SM4_CBC_ENCODE,Smx_input,Smx_key,Smx_output,Smx_lv);
    dprint("CBC encryption:");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");
    SM4_Run(256,SM4_CBC_DECODE,Smx_output,Smx_key,Smx_output,Smx_lv);
    dprint("CBC decryption:");
    for (i = 0; i < 32; i++){dprint("%02x ", Smx_output[i]);}dprint("\n");
}
#endif
#endif

spoint SM2Pulib_Key={
	.x[0]=0x80b876a1,
	.x[1]=0xbfd216e4,
	.x[2]=0xfafc4a40,
	.x[3]=0x7aa35d31,
	.x[4]=0x97610239,
	.x[5]=0xc6d44810,
	.x[6]=0x0b593495,
	.x[7]=0x7aa28ca3,

	.y[0]=0x1b044541,
	.y[1]=0x81788221,
	.y[2]=0x23ca3752,
	.y[3]=0xe9e0ceb7,
	.y[4]=0x7d446169,
	.y[5]=0xd28b0d28,
	.y[6]=0xe2a98a76,
	.y[7]=0xb5ed19c9,
};
DWORD SM2_E[8]={
	0xb0920555,
	0x5d237c29,
	0xcfba9077,
	0x14cd1550,
	0xb6e73f74,
	0x33b99034,
	0xb90cd5df,
	0x7ee06a90,
};
ssign SM2_signer={
	.r[0]=0x50227612,
	.r[1]=0xaef7aa60,
	.r[2]=0xb903baba,
	.r[3]=0x02784804,
	.r[4]=0x87286dd2,
	.r[5]=0xc3c71ea2,
	.r[6]=0xdc4c6c99,
	.r[7]=0x6da4c4ed,

	.s[0]=0xe5231de0,
	.s[1]=0x9cfced64,
	.s[2]=0xf23d34b4,
	.s[3]=0xf98c0c4d,
	.s[4]=0x4f537fa6,
	.s[5]=0xa3a3f3d8,
	.s[6]=0x024143a4,
	.s[7]=0xae94050c,
};
