/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:19:47
 * @Description: Define ANX7447 chip common interface.
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

#ifndef ANX7447_H
#define ANX7447_H

#define AUTO_RDO_ENABLE

#define PATCH_NOT_CALIBRATED_CHIP

#define _BIT0   0x01
#define _BIT1   0x02
#define _BIT2   0x04
#define _BIT3   0x08
#define _BIT4   0x10
#define _BIT5   0x20
#define _BIT6   0x40
#define _BIT7   0x80
/***************************************************************/
/*Register definition of device address 0x58*/
#define TCPC_ROLE_CONTROL        0x1A
#define DRP_CONTROL              _BIT6
#define RP_VALUE                 (_BIT5|_BIT4)
#define CC2_CONTROL              (_BIT3|_BIT2)
#define CC1_CONTROL              (_BIT1|_BIT0)
#define DRP_EN                   _BIT6 // DRP_CONTROL

#define TCPC_COMMAND             0x23

#define ANALOG_CTRL_6            0xA6
#define ADC_CTRL_2               0xC0

#define ANALOG_CTRL_0            0xA0
#define DFP_OR_UFP               _BIT6

#define INTR_ALERT_0             0xCB
#define INTR_RECEIVED_MSG        _BIT7
#define INTR_SOFTWARE_INT        _BIT6

#define INTR_ALERT_1             0xCC
#define INTR_INTP_POW_ON         _BIT7 // digital powerup indicator
#define INTR_INTP_POW_OFF        _BIT6 


#define TCPC_CTRL_2              0xCD
#define SOFT_INTP_1              _BIT1

#define  TCPC_CONTROL            0x19

#define  POWER_CONTROL           0x1C

#define TX_OBJ1_BYTE_0           0x54

#define VBUS_VOLTAGE_0           0x70
#define VBUS_VOLTAGE_BIT7_0      0xFF

#define VBUS_VOLTAGE_1           0x71
#define VBUS_VOLTAGE_BIT9_8      (_BIT0 | _BIT1)

#define  PD_1US_PERIOD           0x80
#define  PD_TX_BIT_PERIOD        0x86

#define ANALOG_CTRL_1            0xA1
#define R_TOGGLE_ENABLE          _BIT7
#define R_LATCH_TOGGLE_ENABLE    _BIT6
#define TOGGLE_CTRL_MODE         _BIT5
#define CC2_VRD_USB              _BIT2
#define CC2_VRD_1P5              _BIT1
#define CC2_VRD_3P0              _BIT0

#define  ANALOG_CTRL_9           0xA9

#define ANALOG_CTRL_10           0xAA
#define FRSWAP_CTRL              0xAB
#define FR_SWAP                  _BIT7
#define FR_SWAP_EN               _BIT6
#define R_FRSWAP_CONTROL_SELECT  _BIT3
#define R_SIGNAL_FRSWAP          _BIT2
#define TRANSMIT_FRSWAP_SIGNAL   _BIT1
#define FRSWAP_DETECT_ENABLE     _BIT0

#define RING_OSC_CTRL            0xD9
#define VBUS_OCP_0               0xE6 
#define VBUS_OCP_1               0xE7
#define VBUS_OCP_BIT9_8          (_BIT0 | _BIT1)

/***************************************************************/
/*Register definition of device address 0x7a*/
#define TX_DATA_BYTE_30          0x00
 
/***************************************************************/
/*Register definition of device address 0x7e*/
#define R_RAM_LEN_H              0x03
#define FLASH_ADDR_EXTEND        _BIT7
 
#define R_RAM_CTRL               0x05
#define FLASH_DONE               _BIT7
#define BOOT_LOAD_DONE           _BIT6
#define LOAD_CRC_OK              _BIT5 // CRC_OK
#define LOAD_DONE                _BIT4

#define R_FLASH_ADDR_H           0x0c
#define R_FLASH_ADDR_L           0x0d

#define FLASH_WRITE_DATA_0       0xe
#define FLASH_READ_DATA_0        0x3c

#define R_FLASH_LEN_H            0x2e
#define R_FLASH_LEN_L            0x2f

#define R_FLASH_RW_CTRL          0x30
#define GENERAL_INSTRUCTION_EN   _BIT6
#define FLASH_ERASE_EN           _BIT5
#define WRITE_STATUS_EN          _BIT2
#define FLASH_READ               _BIT1
#define FLASH_WRITE              _BIT0

#define R_FLASH_STATUS_0         0x31

#define  FLASH_INSTRUCTION_TYPE  0x33
#define FLASH_ERASE_TYPE         0x34

#define R_FLASH_STATUS_REGISTER_READ_0  0x35
#define WIP                      _BIT0

#define R_I2C_0                  0x5C
#define read_Status_en           _BIT7

#define  OCM_CTRL_0              0x6e
#define OCM_RESET                _BIT6

#define ADDR_GPIO_CTRL_0         0x88
#define SPI_WP                   _BIT7
#define SPI_CLK_ENABLE           _BIT6
/*
* For SKIP highest voltage
* Maximum Voltage for Request Data Object
* 100mv units
*/
#define MAX_VOLTAGE 0xAC // 0x7E:0xAC
/*
* For selection PDO
* Maximum Power for Request Data Object
* 500mW units
*/
#define MAX_POWER 0xAD // 0x7E:0xAD
/*
* For mismatch
* Minimum Power for Request Data Object
* 500mW units
*/
#define MIN_POWER 0xAE // 0x7E:0xAE
/*Show Maximum voltage of RDO*/
#define RDO_MAX_VOLTAGE 0xAF // 0x7E:0xAF
/*Show Maximum Powe of RDO*/
#define RDO_MAX_POWER 0xB0 // 0x7E:0xB0
/*Show Maximum current of RDO*/
#define RDO_MAX_CURRENT 0xB1 // 0x7E:0xB1

#define FIRMWARE_CTRL 0xB2 // 0x7E:0xB2
#define disable_usb30 _BIT0
#define auto_pd_en _BIT1
#define trysrc_en _BIT2
#define trysnk_en _BIT3
#define support_goto_min_power _BIT4
#define snk_remove_refer_cc _BIT5
#define force_send_rdo _BIT6
#define high_voltage_for_same_power _BIT7

#define FW_STATE_MACHINE 0xB3 // 0x7E:0xB3

#define OCM_VERSION_REG 0xB4

#define INT_MASK 0xB6 // 0x7E:0xB6
/*same with 0x28 interrupt mask*/
#define CHANGE_INT 0xB7 // 0x7E:0xB7
#define RESV0 _BIT0
#define RECEIVED_ACK _BIT1
#define VCONN_CHANGE _BIT2
#define VBUS_CHANGE _BIT3
#define CC_STATUS_CHANGE _BIT4
#define DATA_ROLE_CHANGE _BIT5
#define PR_CONSUMER_GOT_POWER _BIT6
#define RESV1 _BIT7

#define SYSTEM_STSTUS 0xB8 // 0x7E:0xB8
/*0: VCONN off; 1: VCONN on*/
#define VCONN_STATUS _BIT2
/*0: vbus off; 1: vbus on*/
#define VBUS_STATUS _BIT3
/*1: host; 0:device*/
#define S_DATA_ROLE _BIT5
/*0: Chunking; 1: Unchunked*/
#define SUPPORT_UNCHUNKING _BIT6
/*0: HPD low; 1: HPD high*/
#define HPD_STATUS _BIT7

#define NEW_CC_STATUS 0xB9 // 0x7E:0xB9

// PD Revision configure
// 0: default, 1:PD_REV20, 2:PD_REV30
#define PD_REV_INIT 0xBA // 0x7E:0xBA

#define PD_EXT_MSG_CTRL 0xBB // 0x7E:0xBB
#define SRC_CAP_EXT_REPLY _BIT0
#define MANUFACTURER_INFO_REPLY _BIT1
#define BATTERY_STS_REPLY _BIT2
#define BATTERY_CAP_REPLY _BIT3
#define ALERT_REPLY _BIT4
#define STATUS_REPLY _BIT5
#define PPS_STATUS_REPLY _BIT6
#define SNK_CAP_EXT_REPLY _BIT7
#define CC_STATUS	0x1D	//WN3_007A++

#define InterfaceSendBuf_Addr 0xC0
#define InterfaceRecvBuf_Addr 0xE0

#define recvd_msg_len()          ((context[port_id].InterfaceRecvBuf[0] & 0x1F) - 1)
#define recvd_msg_sop_type()     (context[port_id].InterfaceRecvBuf[0]  >> 6 )
#define recvd_msg_type()         (context[port_id].InterfaceRecvBuf[1])
#define recvd_msg_buf()          (&context[port_id].InterfaceRecvBuf[2])
#define send_msg_len()           (context[port_id].InterfaceSendBuf[0] & 0x1F)
#define RESPONSE_REQ_TYPE()      (context[port_id].InterfaceRecvBuf[2])
#define RESPONSE_REQ_RESULT()    (context[port_id].InterfaceRecvBuf[3])

/* define max request current 3A and voltage 5V */
#define MAX_REQUEST_VOLTAGE 20000
#define MAX_REQUEST_CURRENT 3250
#define set_rdo_value(v0, v1, v2, v3)   \
    do {                \
        context[port_id].pd_rdo[0] = (v0);  \
        context[port_id].pd_rdo[1] = (v1);  \
        context[port_id].pd_rdo[2] = (v2);  \
        context[port_id].pd_rdo[3] = (v3);  \
    } while (0)


#define BYTE_SWAP(word) do { (word) = ((word)<<8)|((word)>>8); } while(0)
//low byte + high byte
#define USB_PD_EXT_HEADER(sendbuf, size, request, number, chunk) \
    do { \
        ((struct ext_message_header *)sendbuf)->data_size = size; \
        ((struct ext_message_header *)sendbuf)->reserved = 0; \
        ((struct ext_message_header *)sendbuf)->request_chunk = request; \
        ((struct ext_message_header *)sendbuf)->chunk_number = number; \
        ((struct ext_message_header *)sendbuf)->chunked = chunk; \
     } while(0)

extern void PD_debug(void);

extern void dp_alt_enter_mode(void);
extern void dp_alt_exit_mode(void);
extern void send_source_sink_capability(void);
extern void chip_power_on(u8 port);
extern void chip_power_down(u8 port); 
extern void chip_reset(u8 port);

extern void anx7447_ucsi_init(void);
extern void anx_dead_battery_checking_1(void);
extern void send_initialized_setting(void);
extern void anx_alert_message_isr(void);


#endif
