/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2023-07-18 18:39:13
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

#ifndef KERNEL_SMx_H
#define KERNEL_SMx_H
//head include
#include <AE_GLOBAL.H>
#include <AE_REG.H>
#include <AE_CONFIG.H>
//typedef


#define SM4_ECB_ENCODE                          0x00
#define SM4_ECB_DECODE                          0x01
#define SM4_CBC_ENCODE                          0x02
#define SM4_CBC_DECODE                          0x03

//bit set
#define SM2_CONTROL_MODN_INV                    0x00//模N反
#define SM2_CONTROL_MODN_MUL                    0x01//模N乘
#define SM2_CONTROL_MODN_ADD                    0x02//模N加
#define SM2_CONTROL_MODN_SUB                    0x03//模N减
#define SM2_CONTROL_DOT_ADD                     0x04//点加
#define SM2_CONTROL_DOT_DBL                     0x05//倍点
#define SM2_CONTROL_DOT_MUL                     0x06//点乘
#define SM2_CONTROL_MODP_INV                    0x08//模P反
#define SM2_CONTROL_MODP_MUL                    0x09//模P乘
#define SM2_CONTROL_MODP_ADD                    0x0A//模P加
#define SM2_CONTROL_MODP_SUB                    0x0B//模P减
#define SM2_CONTROL_FREE                        0x0F//空闲

#define SM2_CONTROL_EN                          BIT4
#define SM2_CONTROL_SOFTRESET                   (~BIT5)

#define SM2_STATUS_FREE                         0x00
#define SM2_STATUS_BUSY                         0x01
#define SM2_STATUS_DONE                         0x02
#define SM2_STATUS_ERROR                        0x03

#define SM3_CONFIG_ENABLE                       HEX01
#define SM3_CONFIG_NEW_DATA                     HEX02
#define SM3_STATE_FINISH                        HEX01
#define SM3_STATE_FIFO_FULL                     HEX02
#define SM3_STATE_FIFO_EMPTY                    HEX04
#define SM3_STATE_FIFO_COUNT                    0xF8
#define SM3_STATE_STATE_ROUND                   0x7F00

#define SM4_MODE_ENDE0                          HEX00
#define SM4_MODE_ENDE1                          HEX01
#define SM4_MODE_KEYEX                          HEX02
#define SM4_MODE_XOR                            HEX04
#define SM4_CTRL_INTEN                          HEX02
#define SM4_CTRL_START                          HEX01
#define SM4_STATUS_BUSY                         HEX02
#define SM4_STATUS_DONE                         HEX01
#define SM4_INT_INT                             HEX01
//extern
/**************************************************************************************
 * function:SM2_STATTE_ERROR  run code
 * note : weak code
 * ************************************************************************************/
extern void SM2_Error(void);
/**************************************************************************************
 * function:SM2_VERIFY (Visa inspection)
 * note :
 *          r :r of the digital signature {r,s}
 *          s:s of the digital signature {r,s}
 *          e:Password hash function value
 *          g:Base point of an elliptic curve
 *          PA:public key
 * ************************************************************************************/
BYTE SM2_VERIFY(DWORDP r,DWORDP s,DWORDP e,spoint g,spoint PA);

extern BYTE Smx_output[32];
/**************************************************************************************
 * function:SM3_Run
 * note :
 *          bit:data bit size
 *          input: Plaintext password
 *          output: Ciphertext storage
 * ************************************************************************************/
extern void SM3_Run(DWORD bit,BYTEP input,BYTEP output);
/**************************************************************************************
 * function:SM4_Run
 * note :
 *          mode:0 ECB加密,1 ECB解码/2 CBC加密/3 CBC解码
 *          input: Plaintext password
 *          key:Extended secret key
 *          output: Ciphertext storage
 *          xordr :lv
 * ************************************************************************************/
extern void SM4_Run(DWORD bit,BYTE mode,BYTEP input,BYTEP key,BYTEP output,BYTEP xordr);//to run sm4
#if SMx_TEST
extern void SM_Test_main(void);//run SMx test code
#endif
#endif