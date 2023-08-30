/*
 * @Author: Maple
 * @LastEditors: Maple
 * @LastEditTime: 2023-04-02 21:36:11
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

#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H
/****************************************************************************
 * Include Headfile
 *****************************************************************************/


/******************************************************************************************
 *Communication port selection
 ******************************************************************************************/
#define CAN0_Send     0
#define CAN1_Send     0
#define CAN2_Send     0
#define CAN3_Send     0
#define CAN0_Receive  0 
#define CAN1_Receive  0
#define CAN2_Receive  0
#define CAN3_Receive  0
/****************************************************************************
 * Init Enable
 *****************************************************************************/

#define CAN_INIT        0
#define LPC_INIT        0

/****************************************************************************
 * Debug enable
 *****************************************************************************/


/****************************************************************************
 * Test enable
 *****************************************************************************/

#define SCI_Test                0
#define SMI_Test                0
/* ACPI EC SCI Interrupt Response Timing ( 8-16uSec).*/
#define LPC_PRESSURE_TEST       0       //LPC压力测试


#endif /* AE_CONFIG_H */

