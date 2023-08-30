/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:20:59
 * @Description: Define the basic UCSI data structure, macro and other initial interface.
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

#ifndef PD_H
#define PD_H

#define Support_ANX7447		0

#ifndef __func__
#define __func__    __FILE__
#endif

typedef VBYTE  VBYTE;
typedef VBYTE  XBYTE;
typedef BYTE           u8;
typedef signed char            s8;
typedef WORD         u16;
typedef DWORD           u32;

#define A_XBYTE  *(VBYTE *)

#define min(A, B) (A>B?B:A)
#define max(A, B) (A>B?A:B)

#define SUPPORT_TYPEC_7447 1
#define SUPPORT_DUAL_TYPEC 0
#define PDO_TYPE_FIXED       ((u32)0 << 30)
#define PDO_TYPE_BATTERY     ((u32)1 << 30)
#define PDO_TYPE_VARIABLE    ((u32)2 << 30)
#define PDO_TYPE_MASK        ((u32)3 << 30)
#define PDO_FIXED_DUAL_ROLE  ((u32)1 << 29)  /* Dual role device */
#define PDO_FIXED_SUSPEND    ((u32)1 << 28)    /* USB Suspend supported */
#define PDO_FIXED_EXTERNAL   ((u32)1 << 27)   /* Externally powered */
#define PDO_FIXED_COMM_CAP   ((u32)1 << 26)   /* USB Communications Capable */
#define PDO_FIXED_DATA_SWAP  ((u32)1 << 25)  /* Data role swap command */
#define PDO_FIXED_PEAK_CURR  ((u32)1 << 20)  /* [21..20] Peak current */
#define PDO_FIXED_HIGH_BYTE_DATA_SWAP ((u8)1 << 1)
#define PDO_FIXED_HIGH_BYTE_DUAL_ROLE ((u8)1 << 5)

/* Voltage in 50mV units */
#define PDO_FIXED_VOLT(mv) (u32)((((u32)mv)/50) << 10)
/* Max current in 10mA units */
#define PDO_FIXED_CURR(ma) (u32)((((u32)ma)/10))
/*build a fixed PDO packet*/
#define PDO_FIXED(mv, ma, flags)    (PDO_FIXED_VOLT(mv) | PDO_FIXED_CURR(ma) | (flags))

/*Pos in Data Object, the first index number begin from 0 */
#define PDO_INDEX(n, dat)     (dat << (n * PD_ONE_DATA_OBJECT_SIZE*sizeof(u8)))
#define PDO_VAR_MAX_VOLT(mv)  ((u32)((((u32)mv) / 50) & 0x3FF) << 20)
#define PDO_VAR_MIN_VOLT(mv)  ((u32)(((u32)(mv) / 50) & 0x3FF) << 10)
#define PDO_VAR_OP_CURR(ma)   ((u32)(((u32)(ma) / 10) & 0x3FF) << 0)
#define PDO_VAR(min_mv, max_mv, op_ma)  (PDO_VAR_MIN_VOLT(min_mv) | PDO_VAR_MAX_VOLT(max_mv) | PDO_VAR_OP_CURR(op_ma) | PDO_TYPE_VARIABLE)

#define PDO_BATT_MAX_VOLT(mv) ((((mv) / 50) & 0x3FF) << 20)
#define PDO_BATT_MIN_VOLT(mv) ((((mv) / 50) & 0x3FF) << 10)
#define PDO_BATT_OP_POWER(mw) ((((mw) / 250) & 0x3FF) << 0)
#define PDO_BATT(min_mv, max_mv, op_mw) (PDO_BATT_MIN_VOLT(min_mv) | PDO_BATT_MAX_VOLT(max_mv) | PDO_BATT_OP_POWER(op_mw) | PDO_TYPE_BATTERY)

#define GET_PDO_TYPE(PDO)            ((PDO & PDO_TYPE_MASK) >> 30)
#define GET_PDO_FIXED_DUAL_ROLE(PDO) ((PDO & PDO_FIXED_DUAL_ROLE) >> 29)
#define GET_PDO_FIXED_SUSPEND(PDO)   ((PDO & PDO_FIXED_SUSPEND) >> 28)
#define GET_PDO_FIXED_EXTERNAL(PDO)  ((PDO & PDO_FIXED_EXTERNAL) >> 27)
#define GET_PDO_FIXED_COMM_CAP(PDO)  ((PDO & PDO_FIXED_COMM_CAP) >> 26)
#define GET_PDO_FIXED_DATA_SWAP(PDO) ((PDO & PDO_FIXED_DATA_SWAP) >> 25)
#define GET_PDO_FIXED_PEAK_CURR(PDO) ((PDO >> 20) & 0x03)

#define GET_PDO_FIXED_VOLT(PDO)      (((PDO >> 10) & 0x3FF) * 50)
#define GET_PDO_FIXED_CURR(PDO)      ((PDO & 0x3FF) * 10)
#define GET_VAR_MAX_VOLT(PDO)        (((PDO >> 20) & 0x3FF) * 50)
#define GET_VAR_MIN_VOLT(PDO)        (((PDO >> 10) & 0x3FF) * 50)
#define GET_VAR_MAX_CURR(PDO)        ((PDO & 0x3FF) * 10)
#define GET_BATT_MAX_VOLT(PDO)       (((PDO >> 20) & 0x3FF) * 50)
#define GET_BATT_MIN_VOLT(PDO)       (((PDO >> 10) & 0x3FF) * 50)
#define GET_BATT_OP_POWER(PDO)       (((PDO) & 0x3FF) * 250)

#define ANX_UCSI_VERSION_MAJOR       0x1
#define ANX_UCSI_VERSION_MINOR       0x0
#define ANX_UCSI_VERSION_REV         0x1

#define POWER_ROLE_SWAP_DELAY_MS     50
#define DATA_ROLE_SWAP_DELAY_MS      50

#define PD_ONE_DATA_OBJECT_SIZE      4
#define PD_MAX_DATA_OBJECT_NUM       7
#define VDO_SIZE                     (PD_ONE_DATA_OBJECT_SIZE * PD_MAX_DATA_OBJECT_NUM)

#define PDO_FIXED_FLAGS              (PDO_FIXED_DUAL_ROLE | PDO_FIXED_DATA_SWAP)

/*5000mv voltage*/
#define PD_VOLTAGE_5V         5000
#define PD_MAX_VOLTAGE_20V    20000

/*0.9A current */
#define PD_CURRENT_900MA      900
#define PD_CURRENT_1500MA     1500
#define PD_CURRENT_3A         3250
#define PD_CURRENT_2A         2000
#define PD_POWER_15W          15000
#define PD_POWER_40W          40000
#define PD_POWER_60W          65000

/* RDO : Request Data Object */
#define RDO_OBJ_POS(n)             (((u32)(n) & 0x7) << 28)
#define RDO_POS(rdo)               ((((32)rdo) >> 28) & 0x7)
#define RDO_GIVE_BACK              ((u32)1 << 27)
#define RDO_CAP_MISMATCH           ((u32)1 << 26)
#define RDO_COMM_CAP               ((u32)1 << 25)
#define RDO_NO_SUSPEND             ((u32)1 << 24)
#define RDO_FIXED_VAR_OP_CURR(ma)  (((((u32)ma) / 10) & 0x3FF) << 10)
#define RDO_FIXED_VAR_MAX_CURR(ma) (((((u32)ma) / 10) & 0x3FF) << 0)
#define RDO_BATT_OP_POWER(mw)      (((((u32)mw) / 250) & 0x3FF) << 10)
#define RDO_BATT_MAX_POWER(mw)     (((((u32)mw) / 250) & 0x3FF) << 10)

#define RDO_FIXED(n, op_ma, max_ma, flags)  (RDO_OBJ_POS(n) | (flags) | RDO_FIXED_VAR_OP_CURR(op_ma) | RDO_FIXED_VAR_MAX_CURR(max_ma))
#define RDO_BATT(n, op_mw, max_mw, flags)   (RDO_OBJ_POS(n) | (flags) | RDO_BATT_OP_POWER(op_mw) | RDO_BATT_MAX_POWER(max_mw))

#define EXTERNALLY_POWERED  YES
/* Source Capabilities */
/* 1 to 5 */
#define SOURCE_PROFILE_NUMBER   1
/* 0 = Fixed, 1 = Battery, 2 = Variable */
#define SRC_PDO_SUPPLY_TYPE1    0
/* 0 to 3 */
#define SRC_PDO_PEAK_CURRENT1   0
/* 5000mV (5V) */
#define SRC_PDO_VOLTAGE1        5000
/* 500mA (0.5A) */
#define SRC_PDO_MAX_CURRENT1    500

//redriver control
#define ANXREDRIVER
#define NO_CONNECT              0x00
#define USB3_1_CONNECTED        0x01
#define DP_ALT_4LANES           0x02
#define USB3_1_DP_2LANES        0x03
#define CC1_CONNECTED           0x00
#define CC2_CONNECTED           0x01
#define SELECT_PIN_ASSIGMENT_C  0x04
#define SELECT_PIN_ASSIGMENT_D  0x08
#define SELECT_PIN_ASSIGMENT_E  0x10
#define REDRIVER_ADDRESS        0x20
#define REDRIVER_OFFSET         0x00

#define YES     1
#define NO      0
#define ERR_CABLE_UNPLUG -1

#define MV 1
#define MA 1
#define MW 1

#define EC_ANX_BIG_ENDIAN
//extern u8 SWAP_BYTE;
#define SWAP_BYTE (A_XBYTE (0x22000 + 0x1e2))

#define SWAP_WORD(W) do {\
    SWAP_BYTE = ((u8 *)W)[0];\
    ((u8 *)W)[0] = ((u8 *)W)[1];\
    ((u8 *)W)[1] = SWAP_BYTE;\
} while(0)

#define SWAP_DWORD(DW) do {\
    SWAP_BYTE = ((u8 *)DW)[0];\
    ((u8 *)DW)[0] = ((u8 *)DW)[3];\
    ((u8 *)DW)[3] = SWAP_BYTE;\
    SWAP_BYTE = ((u8 *)DW)[1];\
    ((u8 *)DW)[1] = ((u8 *)DW)[2];\
    ((u8 *)DW)[2] = SWAP_BYTE;\
} while (0)

#define SWAP_DWORD_WORD(DWW) {\
    SWAP_WORD(DWW);\
    SWAP_WORD(DWW+2);\
}

#define ANXREENTRANT reentrant
#define ANXXDATA    xdata
#define ANXXDATALOCAL
#define UNREFERENCE(X)  (X = X)
#define ANXINLINE
#define ANXPACK

#define anx_read_reg i2c_read_reg
#define anx_write_reg i2c_write_reg
#define anx_read_block_reg i2c_read_block_reg
#define anx_write_block_reg i2c_write_block_reg_b

/*
 * Command Code
 */
#define UCSI_COMMAND_RESERVED               0x00
#define UCSI_COMMAND_PPM_RESET              0x01
#define UCSI_COMMAND_CANCEL                 0x02
#define UCSI_COMMAND_CONNECTOR_RESET        0x03
#define UCSI_COMMAND_ACK_CC_CI              0x04
#define UCSI_COMMAND_NOTIFICATION_ENABLE    0x05
#define UCSI_COMMAND_CAPABILITY             0x06
#define UCSI_COMMAND_CONNECTOR_CAPABILITY   0x07
#define UCSI_COMMAND_UOM                    0x08
#define UCSI_COMMAND_UOR                    0x09
#define UCSI_COMMAND_PDM                    0x0A
#define UCSI_COMMAND_PDR                    0x0B
#define UCSI_COMMAND_GET_ALTERNATE_MODES    0x0C
#define UCSI_COMMAND_GET_CAM_SUPPORTED      0x0D
#define UCSI_COMMAND_GET_CURRENT_CAM        0x0E
#define UCSI_COMMAND_SET_NEW_CAM            0x0F
#define UCSI_COMMAND_GET_PDOS               0x10
#define UCSI_COMMAND_GET_CABLE_PROPERTY     0x11
#define UCSI_COMMAND_GET_CONNECTOR_STATUS   0x12
#define UCSI_COMMAND_GET_ERROR_STATUS       0x13

#define UCSI_COMMAND_SUCC                   1
#define UCSI_COMMAND_FAIL                   2
#define UCSI_COMMAND_NOT_NOTIFY             3
#define UCSI_COMMAND_BUSY                   4
#define UCSI_COMMAND_CONTINUE               5
#define UCSI_COMMAND_RETURN_BUSY_ACK        0x1

/*
 * Parameter values
 */
#define UCSI_MAX_DATA_LENGTH                0x10
#define UCSI_MAX_NUM_ALT_MODE               0x80
#define UCSI_MIN_TIME_TO_RESPOND_WITH_BUSY  0x0A
#define UCSI_GET_ERROR_STATUS_DATA_LENGTH   0x10

#define PD_VERSION              0x0200
#define TYPEC_VERSION           0x0110
#define UCSI_COMPLIANT_VERSION  0x0100

#define UCSI_SET_UOM_SUPPORT    (1<<0)
#define UCSI_SET_PDM_SUPPORT    (1<<1)
#define UCSI_ALT_SUPPORT        (1<<2)
#define UCSI_ALT_OR_SUPPORT     (1<<3)
#define UCSI_PDOS_SUPPORT       (1<<4)
#define UCSI_CABLE_SUPPORT      (1<<5)
#define UCSI_EXTERNAL_SUPPORT   (1<<6)
#define UCSI_PD_RESET_SUPPORT   (1<<7)

#define CMD_OPTIONAL_SUPPORT    \
        UCSI_SET_UOM_SUPPORT    | \
        UCSI_SET_PDM_SUPPORT    | \
        UCSI_ALT_SUPPORT        | \
        UCSI_ALT_OR_SUPPORT     | \
        UCSI_PDOS_SUPPORT       | \
        UCSI_CABLE_SUPPORT      | \
        UCSI_EXTERNAL_SUPPORT   | \
        UCSI_PD_RESET_SUPPORT

#define PD_MAX_INSTANCE    2
#define INTERFACE_SEND_BUF_SIZE 32
#define INTERFACE_RECV_BUF_SIZE 32
#define ANXmdelay vDelayXms
#define time_before(A, B) (((B-A)>0)?1:0)

#define change_int      (A_XBYTE (0x22000 + 0x1f0))
#define change_status   (A_XBYTE (0x22000 + 0x1f1))
#define intr_alert_0    (A_XBYTE (0x22000 + 0x1f2))
#define intr_alert_1    (A_XBYTE (0x22000 + 0x1f3))
#define pdo_h           (A_XBYTE (0x22000 + 0x1f4))
#define pdo_l           (A_XBYTE (0x22000 + 0x1f6))
#define pdo_max         (A_XBYTE (0x22000 + 0x1f8))
#define pdo_max_tmp     (A_XBYTE (0x22000 + 0x1fc))

#define self_charging_error         (A_XBYTE (0x21000 + 0x5))
#define connector_change_need_ack   (A_XBYTE (0x21000 + 0x16))
#define error_need_ack              (A_XBYTE (0x21000 + 0x17))
#define command_complete_need_ack   (A_XBYTE (0x21000 + 0x18))
#define need_ack_command            (A_XBYTE (0x21000 + 0x19))

#define wait_for_ack_back   (A_XBYTE (0x21000 + 0x24))
#define role                (A_XBYTE (0x21000 + 0x7a))

#define ucsi_errno (A_XBYTE (0x21000 + 0x1))

#define ANX_UCSI_VERSION_MAJOR       0x1
#define ANX_UCSI_VERSION_MINOR       0x0
#define ANX_UCSI_VERSION_REV         0x1
#define POWER_ROLE_SWAP_DELAY_MS     50
#define DATA_ROLE_SWAP_DELAY_MS      50
#define PD_ONE_DATA_OBJECT_SIZE      4
#define PD_MAX_DATA_OBJECT_NUM       7
#define VDO_SIZE                     (PD_ONE_DATA_OBJECT_SIZE * PD_MAX_DATA_OBJECT_NUM)
#define PDO_FIXED_FLAGS              (PDO_FIXED_DUAL_ROLE | PDO_FIXED_DATA_SWAP)
#define __UCSI__
#define __PPM__

#define result (A_XBYTE (0x21000 + 0x71))
#define ucsi_received_command (A_XBYTE (0x21000 + 0x79))
#define supply_1500ma_flag (A_XBYTE (0x22000 + 0x1e3))
#define supply_1500ma_port (A_XBYTE (0x22000 + 0x1e4))
#define current_pending_command (ch->Command)

#define current_pending_command_timeout (A_XBYTE (0x21000 + 0x28))
#define ucsi_async_checking_timer (A_XBYTE (0x21000 + 0x29))
#define ucsi_async_restore (A_XBYTE (0x21000 + 0x2A))
#define ucsi_command_process_delay (A_XBYTE (0x21000 + 0x2B))

#define S_CLEAR(X, S)   memset((void *)X, 0, sizeof(S))
#define S_CLEAR_L(X, L) memset((void *)X, 0, L)
#define PDk (A_XBYTE (0x21000 + 0x21))
#define PDl (A_XBYTE (0x21000 + 0x22))

#define DETECT_POWER_ON(port_index) \
    if (context[port_index].anx_power_status == 0) { \
        ucsi_errno = ERR_INVALID; \
        set_error_data(); \
        return UCSI_COMMAND_FAIL; \
    }

#ifdef ANXDEBUG
#define SET_STATE(X)    {\
    dprint("%s:%d change state(%d) to state(%d).\n", \
            __func__, __LINE__, \
            (u16)ucsi_ppm_state, (u16)X); \
    ucsi_ppm_state = X; \
}
#else
#define SET_STATE(X)    {\
    ucsi_ppm_state = X; \
}

#endif

#define GOTO_STATE(X)   SET_STATE(X)

/*enable ANX debug mode*/
#ifdef UART_Debug
/*set the INT and Address configuration for ANX EVB*/
//#define ITE_EC_EVB_PORT1_TEST
//#define ANXDEBUG
#define ucsi_debug dprint
#endif

typedef enum {
    TYPE_PWR_SRC_CAP = 0x00,
    TYPE_PWR_SNK_CAP = 0x01,
    TYPE_DP_SNK_IDENTITY = 0x02,
    TYPE_SVID = 0x03,
    TYPE_GET_DP_SNK_CAP = 0x04,
    TYPE_ACCEPT = 0x05,
    TYPE_REJECT  = 0x06,
    TYPE_DP_SNK_STATUS = 0x07,
    TYPE_SET_SNK_DP_CAP = 0x08,
    TYPE_GET_SRC_CAP = 0x09,
    TYPE_GET_RDO = 0x0a,

    TYPE_PSWAP_REQ = 0x10,
    TYPE_DSWAP_REQ = 0x11,
    TYPE_GOTO_MIN_REQ =  0x12,
    TYPE_VCONN_SWAP_REQ = 0x13,
    TYPE_VDM = 0x14,
    TYPE_DP_SNK_CFG = 0x15,
    TYPE_PWR_OBJ_REQ = 0x16,
    TYPE_GET_PD_STATUS = 0x17,
    TYPE_DP_ALT_ENTER = 0x19, 
    TYPE_DP_ALT_EXIT = 0x1A, 
    TYPE_GET_SNK_CAP = 0x1B, 
    TYPE_NOT_SUPPORTED = 0x1C, // PD_CTRL_NOT_SUPPORTED
    TYPE_GET_SRC_CAP_EXT = 0x1D, // PD_CTRL_GET_SOURCE_CAP_EXTENDED
    TYPE_GET_STS = 0x1E, // PD_CTRL_GET_STATUS
    TYPE_FR_SWAP = 0x1F, // PD_CTRL_FR_SWAP
    TYPE_FR_SWAP_SIGNAL = 0x20, // Fast Role Swap signal
    TYPE_GET_PPS_STS = 0x21, //PD_CTRL_GET_PPS_STS
    TYPE_GET_COUNTRY_CODE = 0x22, //PD_CTRL_GET_COUNTRY_CODE
    TYPE_GET_SINK_CAP_EXT = 0x23, //PD_CTRL_GET_SINK_CAP_EXTENDED
    TYPE_BATT_STS = 0x24, // PD_DATA_BATTERY_STATUS
    TYPE_ALERT = 0x25, // PD_DATA_ALERT
    TYPE_GET_COUNTRY_INFO = 0x26, //PD_DATA_GET_COUNTRY_INFO
    TYPE_DP_DISCOVER_MODES_INFO = 0x27, //PD DISCOVERY MODE
    TYPE_GET_DP_CONFIGURE   = 0x29,// for Pin Assignment
    TYPE_DP_CONFIGURE   = 0x2a,// for Pin Assignment
    TYPE_AMS_PROCESSING = 0x2b, // Processing AMS, Collision Avoidance
    TYPE_CMD_SRC_CAP_1 = 0x2c, // Update source capability to OCM without sending to CC line

    TYPE_EXT_SRC_CAP_EXT = 0x30, // Source_Capabilities_Extended
    TYPE_EXT_STS = 0x31, // Status
    TYPE_EXT_GET_BATT_CAP = 0x32, // Get_Battery_Cap
    TYPE_EXT_GET_BATT_STS = 0x33, // Get_Battery_ Status
    TYPE_EXT_BATT_CAP = 0x34, // Battery_Capabilities
    TYPE_EXT_GET_MFR_INFO = 0x35, // Get_Manufacturer_Info
    TYPE_EXT_MFR_INFO = 0x36, // Manufacturer_Info
    TYPE_EXT_PPS_STS  = 0x37, //PPS_status
    TYPE_EXT_COUNTRY_INFO  = 0x38, // Country_info
    TYPE_EXT_COUNTRY_CODE  = 0x39, // Country_code
    TYPE_EXT_SINK_CAP_EXT = 0x3a, // SINK_Capabilities_Extended
    TYPE_EXT_PDFU_REQUEST = 0x3b,
    TYPE_EXT_PDFU_RESPONSE = 0x3c,
    TYPE_I2C_MASTER_WRITE = 0x3e, //I2C Master write
    
    TYPE_RESPONSE_TO_REQ = 0xF0,
    TYPE_SOFT_RST = 0xF1,
    TYPE_HARD_RST = 0xF2,
    TYPE_RESTART = 0xF3,
    TYPE_GET_VAR = 0xFC, // get variable value
    TYPE_SET_VAR = 0xFD // set variable value
} PD_MSG_TYPE;

typedef enum {// TRANSMIT_SOP_TYPE
    SOP_TYPE  = 0x00, 
    SOP1_TYPE = 0x01, 
    SOP2_TYPE = 0x02, 
} PD_SOP_TYPE;

typedef struct cable_vdo {
    u8 hardware_version      : 4;
    u8 firmware_version      : 4;
    u16 reserved             : 4;
    u16 cable_type           : 2;
    u16 type_c_plug_to_p_r   : 1;
    u16 latency              : 4;
    u16 termination_type     : 2;
    u16 SSTX1_sup            : 1;
    u16 SSTX2_sup            : 1;
    u16 SSRX1_sup            : 1;
    u8 SSRX2_sup             : 1;
    u8 vbus_current          : 2;
	#define VBUS_NOT_THRU_CABLE    0
	#define VBUS_3A                1
	#define VBUS_5A                2
    u8 vbus_thru_cable       : 1;
    u8 sop2_present          : 1;
    u8 supper_speed_signal   : 3;
	#define SUPPER_SPEED_USB_2_0    0
	#define SUPPER_SPEED_USB_3_1    1
	#define SUPPER_SPEED_USB_3_1_2  2
} cable_vdo;

typedef enum {
    // for TYPE_GET_VAR and TYPE_SET_VAR
    // use correct offset and length for each variable type
    // do not use no define variable type
    IF_VAR_fw_var_reg = 0x10, // offset:0, length:8
    IF_VAR_pd_src_pdo = 0x11, // offset:0, length:28 (VDO_SIZE)
    IF_VAR_pd_snk_pdo = 0x12, // offset:0, length:28 (VDO_SIZE)
    IF_VAR_pd_rdo_bak = 0x13, // offset:0, length:4 (VDO_LEN)
    IF_VAR_pd_rdo = 0x14, // offset:0, length:4 (VDO_LEN)
    IF_VAR_DP_caps = 0x15, // offset:0, length:4 (VDO_LEN)
    IF_VAR_configure_DP_caps = 0x16, // offset:0, length:4 (VDO_LEN)
    IF_VAR_src_dp_status = 0x17, // offset:0, length:4 (VDO_LEN)
    IF_VAR_sink_svid_vdo = 0x18, // offset:0, length:4 (VDO_LEN)
    IF_VAR_sink_identity = 0x19, // offset:0, length:16
}PD_MSG_VAR_TYPE;

typedef enum {
    AttachWait_SNK = 0x00,
    AttachWait_SRC = 0x01,
    TRY_SNK= 0x02,
    TRY_SRC = 0x03,
    Attached_SNK  = 0x04,
    Attached_SRC = 0x05,
    Unattached_SNK  = 0x06,
    Unattached_SRC = 0x07,
    TRY_SNK_WaitVbus=0x08,
    TRY_SNK_WaitRp=0x09,
    TRY_SRC_WaitRd=0X0A,
    Error_SNK=0x0B
}TYPEC_STATE;

/*Comands status*/
enum interface_status {
    CMD_SUCCESS,
    CMD_REJECT,
    CMD_FAIL,
    CMD_BUSY,
    CMD_STATUS
};

struct connector_status_change1 {
    u8 reserved1               ;
    u8 ExternalSupply          ;
    u8 PowerOperationMode      ;
    u8 reserved2               ;
    u8 SupportedProviderCap    ;
    u8 NegotiatedPowerLevel    ;
    u8 PDResetComplete         ;
    u8 SupportedCAM            ;
    u8 BatteryChargingStatus   ;
    u8 Reserved3               ;
    u8 ConnectorPartner        ;
    u8 PowerDirection          ;
    u8 Reserved4               ;
    u8 Connect                 ;
    u8 Error                   ;
};

s8 *result_to_str(u8 result_type);
s8 *interface_to_str(u8 header_type);
void anx_initial_context(u8 port_index);
u8 send_pd_msg(PD_MSG_TYPE type, u8 *buf, u8 size, u8 type_sop);
u8 get_data_role();
u8 get_power_role();

typedef struct {
    u32 word1;
    u32 word2;
} u64;

typedef void (*ec_caller)(u8 param);

//extern struct bios_share_msg ANXXDATA ppm2bios;
/*
 * Shared Message Data Structure
 */
typedef struct bios_share_msg {
    u16 version;
    u16 reserved;
    u32 CCI;
    u64 CONTROL;
    u8  msgi[16];
    u8  msgo[16];
} bios_share_msg;

/*
 * USB Type-C Command Status and Connector Change Indication Data Structure.
 */
typedef struct cci_status {
    u8 Reserved1                   : 1;
    u8 ConnectorChangeIndicator    : 7;
    u8 DataLength;
    u8 Reserved2;
    u8 Reserved3                   : 1;
    u8 NotSupportedIndicator       : 1;
    u8 CancelCompletedIndicator    : 1;
    u8 ResetCompletedIndicator     : 1;
    u8 BusyIndicator               : 1;
    u8 AcknowledgeCommandIndicator : 1;
    u8 ErrorIndicator              : 1;
    u8 CommandCompletedIndicator   : 1;
} cci_status;

/*
 * USB Type-C Control Data Structure header.
 */
typedef struct control_header {
    u8 Command;
    u8 DataLength;
} control_header;

/*
 * PPM_RESET Command
 */
typedef struct reset {
    struct control_header header;
    u16 Reserved1;
    u32 Reserved2;
} reset;

/*
 * CANCEL Command
 */
typedef struct cancel {
    struct control_header header;
    u16 Reserved1;
    u32 Reserved2;
} cancel;

/*
 * CONNECTOR_RESET Command
 */
typedef struct connector_reset {
    struct control_header header;
    u8 ConnectorNumber         : 7;
    u8 HardReset               : 1;
    u8 Reserved1               : 8;
    u32 Reserved2;
} connector_reset;

/*
 * ACK_CC_CI Command
 */
typedef struct ack_cc_ci {
    struct control_header header;
    u8 ConnectorChangeAcknowledge      : 1;
    u8 CommandCompletedAcknowledge     : 1;
    u8 Reserved1                       : 6;
    u8 Reserved2;
    u32 Reserved3;
} ack_cc_ci;

/*
 * SET_NOTIFICATION_ENABLE status
 */
typedef struct notification_enable_status {
    u8 CommandCompleted                    : 1;
    u8 ExternalSupplyChange                : 1;
    u8 PowerOperationModeChange            : 1;
    u8 Reserved1                           : 1;
    u8 Reserved2                           : 1;
    u8 SupportedProviderCapabilitiesChange : 1;
    u8 NegotiatedPowerLevelChange          : 1;
    u8 PDResetComplete                     : 1;
    u8 SupportedCAMChange                  : 1;
    u8 BatteryChargingStatusChange         : 1;
    u8 Reserved3                           : 1;
    u8 ConnectorPartnerChange              : 1;
    u8 PowerDirectionChange                : 1;
    u8 Reserved4                           : 1;
    u8 ConnectChange                       : 1;
    u8 Error                               : 1;
} notification_enable_status;
extern struct notification_enable_status ne;

/*
 * SET_NOTIFICATION_ENABLE Command
 */
typedef struct set_notification_enable {
    struct control_header header;
    struct notification_enable_status status;
    u32 Reserved5;
} set_notification_enable;

/*
 * GET_CAPABILITY Command
 */
typedef struct get_capability {
    struct control_header header;
    u16 Reserved1;
    u32 Reserved2;
} get_capability;

/*
 * GET_CAPABILITY DATA
 */
struct capability_data {
    union {
        u32 v0;
        struct {
            u8 DisabledStateSupport : 1;
            u8 BatteryChaging       : 1;
            u8 USBPowerDelivery     : 1;
            u8 Reserved1            : 3;
            u8 USBTypeCCurrent      : 1;
            u8 Reserved2            : 1;
            union {
                u8 v0;
                struct {
                    u8 ACSupply     : 1;
                    u8 Reserved3    : 1;
                    u8 Other        : 1;
                    u8 Reserved4    : 3;
                    u8 UsesVBUS     : 1;
                    u8 Reserved5    : 1;
                } v1;
            } bmPowerSource;
            u16 Reserved6;
        } v1;
    } bmAttributes;
    u8 bNumConnectors               : 7;
    u8 Reserved7                    : 1;
    u8 bmOptionalFeatures1;
    u8 bmOptionalFeatures2;
    u8 bmOptionalFeatures3;
    u8 bNumAltModes;
    u8 Reserved8;
    u16 bcdBCVersion;
    u16 bcdPDVersion;
    u16 bcdUSBTypeCVersion;
} ANXPACK;


/*
 * GET_CONNECTOR_CAPABILITY Command
 */
struct get_connector_capability {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 Reserved1                   : 1;
    u8 Reserved2;
    u32 Reserved3;
} ANXPACK;

/*
 * GET_CONNECTOR_CAPABILITY DATA
 */
struct connector_capability_data {
    u8 OperationMode       : 8;
	#define OM_DFP_ONLY         (1<<0)
	#define OM_UFP_ONLY         (1<<1)
	#define OM_DRP              (1<<2)
	#define OM_AUDIO_SUP        (1<<3)
	#define OM_DEBUG_ACC_SUP    (1<<4)
	#define OM_USB2_SUP         (1<<5)
	#define OM_USB3_SUP         (1<<6)
	#define OM_ALT_SUP          (1<<7)
    u8 Provider            : 1;
    u8 Consumer            : 1;
    u8 Reserved            : 6;    
} ANXPACK;

#define OP_DFP                      1
#define OP_UFP                      2
#define OP_DRP                      4
#define OP_DRP_DFP                  5
#define OP_DRP_UFP                  6
/*
 * SET_UOM Command
 */
struct set_uom {
    struct control_header header;
    u16 ConnectorNumber             : 7;
    u16 USBOperationMode            : 3;
    u16 Reserved1                   : 6;
    u32 Reserved2;
} ANXPACK;

/*
 * SET_UOR Command
 */
struct set_uor {
    struct control_header header;
    u16 ConnectorNumber             : 7;
    u16 USBOperationMode            : 3;
    u16 Reserved1                   : 6;
    u32 Reserved2;
} ANXPACK;

#define PW_PROVIDER                 1
#define PW_CONSUMER                 2
#define PW_DRP                      4
#define PW_DRP_PROVIDER             5
#define PW_DRP_CONSUMER             6
/*
 * SET_PDM Command
 */
struct set_pdm {
    struct control_header header;
    u16 ConnectorNumber             : 7;
    u16 PowerDirectionMode          : 3;
    u16 Reserved1                   : 6;
    u32 Reserved2;
} ANXPACK;

/*
 * SET_PDR Command
 */
struct set_pdr {
    struct control_header header;
    u16 ConnectorNumber             : 7;
    u16 PowerDirectionMode          : 3;
    u16 Reserved1                   : 6;
    u32 Reserved2;
} ANXPACK;

/*
 * GET_ALTERNATE_MODES Command
 */
struct get_alternate_modes {
    struct control_header header;
    u8 Recipient                 : 3;
	#define R_CONNECTOR                0
	#define R_SOP1                     1
	#define R_SOP2                     2
	#define R_SOP3                     3
    u8 Reserved1                 : 5;
    u8 ConnectorNumber           : 7;
    u8 Reserved2                 : 1;
    u8 AlternateModeOffset       : 8;
    u8 NumofAlternateMode        : 2;
    u8 Reserved3                 : 6;
    u16 Reserved4;
} ANXPACK;

/*
 * GET_ALTERNATE_MODES DATA
 */
struct alt_modes_data {
    u16 SVID0;
    u32 MID0;
    u16 SVID1;
    u32 MID1;
} ANXPACK;

/*
 * GET_CAM_SUPPORTED Command
 */
struct get_cam_supported {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 Reserved1                   : 1;
    u8 Reserved2;
    u32 Reserved3;
} ANXPACK;

/*
 * GET_CURRENT_CAM Command
 */
struct get_current_cam {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 Reserved1                   : 1;
    u8 Reserved2;
    u32 Reserved3;
} ANXPACK;

/*
 * SET_NEW_CAM Command
 */
struct set_new_cam {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 EnterOrExit                 : 1;
    u8 NewCAM;
    u32 AMSpecific;
} ANXPACK;

/*
 * GET_PDOS Command
 */
struct get_pdos {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 PartnerPDO                  : 1;
    u8 PDOOffset;
    u8 NumberOfPDOs                : 2;
    u8 SourceOrSinkPDOs            : 1;
    u8 SourceCapabilities          : 2;
	#define SCTCURRENT_SUPPORTED_SOURCE_CAPABILITIES    0
	#define SCTADVERTISED_CAPABILITIES                  1
	#define SCTMAXIMUM_SUPPORTED_SOURCE_CAPABILITIES    2
	#define SCTNOT_USED                                 3
    u8 Reserved1                   : 3;
    u8 Reserved2;
    u16 Reserved3;
} ANXPACK;

/*
 * PDOS MESSAGE
 */
struct pdos_data {
    u32 PDO[4];
} ANXPACK;

/*
 * GET_CABLE_PROPERTY Command
 */
struct get_cable_property {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 Reserved1                   : 1;
    u8 Reserved2;
    u32 Reserved3;
} ANXPACK;

/*
 * CABLE_PROPERTY DATA
 */
struct cable_property_data {
    u16 bmSpeedSupported;
	#define SPEED_b_PER_SECOND  0
	#define SPEED_Kb_PER_SECOND 1
	#define SPEED_Mb_PER_SECOND 2
	#define SPEED_Gb_PER_SECOND 3
    u8 bCurrentCapability;
    u8 VBUSInCable                 : 1;
    u8 CableType                   : 1;
    u8 Directionality              : 1;
    u8 PlugEndType                 : 2;
	#define PLUG_TYPEA      0
	#define PLUG_TYPEB      1
	#define PLUG_TYPEC      2
	#define PLUG_OTHER      3 //Not USB
    u8 ModeSupport                 : 1;
    u8 Reserved1                   : 2;
    u8 Latency                     : 4;
    u8 Reserved2                   : 4;
} ANXPACK;

/*
 * GET_CONNECTOR_STATUS Command
 */
struct get_connector_status {
    struct control_header header;
    u8 ConnectorNumber             : 7;
    u8 Reserved1                   : 1;
    u8 Reserved2;
    u32 Reserved3;
} ANXPACK;

/*
 * ConnectorStatusChange Field Description
 */
struct connector_status_change {
    u8 reserved1               ;
    u8 ExternalSupply          ;
    u8 PowerOperationMode      ;
    u8 reserved2               ;
    u8 SupportedProviderCap    ;
    u8 NegotiatedPowerLevel    ;
    u8 PDResetComplete         ;
    u8 SupportedCAM            ;
    u8 BatteryChargingStatus   ;
    u8 Reserved3               ;
    u8 ConnectorPartner        ;
    u8 PowerDirection          ;
    u8 Reserved4               ;
    u8 Connect                 ;
    u8 Error                   ;
} ANXPACK;

/*
 * CONNECTOR_STATUS DATA
 */
struct connector_status_data {
    struct connector_status_change csc;
    u8 PowerOperationMode            : 3;
	#define PO_NO_CONSUMER                  0
	#define PO_USB_DEFAULT                  1
	#define PO_BC                           2
	#define PO_PD                           3
	#define PO_TYPE_C_1_5A                  4
	#define PO_TYPE_C_3A                    5
    u8 ConnectStatus                 : 1;
    u8 PowerDirection                : 1;
	#define PD_CONSUMER                     0
	#define PD_PROVIDER                     1
    u8 ConnectorPartner              : 3;
    u8 ConnectorPartnerReserved      : 5;
	#define CP_USB                          1
	#define CP_ALT_MODE                     2
    u8 ConnectorPartnerType          : 3;
	#define CPT_RESERVED                    0
	#define CPT_DFP_ATTACHED                1
	#define CPT_UFP_ATTACHED                2
	#define CPT_CAB_NO_UFP_ATTACHED         3
	#define CPT_CAB_UFP_ATTACHED            4
	#define CPT_CAB_DEBUG_ACC               5
	#define CPT_AUDIO_ACC                   6
    u32 RequestDataObject;
    u8  BatteryChargingStatus         : 2;
	#define BCS_NOT_CHARGING                0
	#define BCS_NOMINAL_CHARGING            1
	#define BCS_SLOW_CHARGING               2
	#define BCS_TRICKLE_CHARGING            3
    u8  ProviderCapLimited_Reason     : 4;
	#define PCL_PowerBudgetLowered          1
	#define PCL_REACH_POWER_BUDGET_LIMIT    2
    u8  Reserved                      : 2;
} ANXPACK;

/*
 * GET_ERROR_STATUS Command
 */
struct get_error_status {
    struct control_header header;
    u16 Reserved1;
    u32 Reserved2;
} ANXPACK;

/*
 * ERROR_STATUS DATA
 */
struct error_status_data {
    u16 ErrorInformation;
	#define ERR_UNRECOGNIZED        (1<<0)
	#define ERR_NON_EXIST           (1<<1)
	#define ERR_INVALID             (1<<2)
	#define ERR_INCOMPATIBLE        (1<<3)
	#define ERR_CC                  (1<<4)
	#define ERR_CMD_UNSUCCESS_DUE_TO_DEAD_BATTERY       (1<<5)
	#define ERR_NEGOTIATION_FAIL    (1<<6)
    u8 VendorDefined[14];
} ANXPACK;
//extern u8 ucsi_errno;

/*
 * UCSI state machine
 */
enum UCSI_PPM_STATE {
    PPM_IDLE_ND = 1,    //PPM IDLE, Notification disable
    PPM_IDLE_NE,        //PPM IDLE, Notification enable
    PPM_BUSY,       //PPM Busy
    PPM_PC,         //PPM Processing Command
    PPM_WCCA,       //PPM Wait for Command Completion Acknowledge
    //PPM_PCCA,     //PPM Process Command Completion Acknowledge
    //PPM_AEAC,     //PPM Asynch Event Ack Completion
    PPM_WAEA        //PPM Wait for Asynch Event Ack
};


struct ppm_ucsi_parameters {
    struct alt_modes_data alt_mode[1];
    u8 alt_mode_support_bitmap;
    u8 alt_mode_support_num;
    u8 current_alt_mode;
} ANXPACK;


BYTE ucsi_init();


extern BYTE  InterfaceSendBuf[PD_MAX_INSTANCE][INTERFACE_SEND_BUF_SIZE];
extern BYTE  InterfaceRecvBuf[PD_MAX_INSTANCE][INTERFACE_RECV_BUF_SIZE];
extern XBYTE i;
extern XBYTE j;
extern  XBYTE random_data,timer1msCount;
extern  XBYTE DataLength;
extern  XBYTE   len;     
extern  XBYTE   port_id; 
extern u16 count;
static u8 registered_connector_index;

extern struct ppm_ucsi_parameters pup;
static enum UCSI_PPM_STATE ucsi_ppm_state;
static struct error_status_data esd;

typedef struct anx_cam {
    u16 svid;
    u32 mid;
} anx_cam;

typedef enum {
    CABLE_TYPE_A = 1,
    CABLE_TYPE_B,
    CABLE_TYPE_C,
} cable_type;

typedef struct anx_context{
    /* initial data */
    u8 anx_power_status;
    u8 anx_power_role;
    u8 anx_data_role;
    u8 anx_current_in_50ma;
    u8 anx_voltage_in_100mv;
    u8 pd_snk_pdo_cnt;
    u8 pd_snk_pdo[VDO_SIZE];
    u8 pd_rdo[PD_ONE_DATA_OBJECT_SIZE];
    u8 DP_caps[PD_ONE_DATA_OBJECT_SIZE];
    u8 configure_DP_caps[PD_ONE_DATA_OBJECT_SIZE];
    u8 src_dp_caps[PD_ONE_DATA_OBJECT_SIZE];
    u8 downstream_pd_cap;
    struct anx_cam ac[2];
    struct cable_vdo cv;
    u8 peer_svid[16];
    u8 peer_svid_length;
    u8 pd_src_pdo_cnt;
    u8 pd_src_pdo[VDO_SIZE];

    cable_type ct;
    u8 ucsi_connector_partner_type;
    u8 ConnectorPartner;
    u8 BatteryCharging;
    u32 ucsi_partner_rdo;
    u8 power_op_mode;
    u8 power_role_swap_flag;
    u8 partner_pdo_length;
    u8 dfp_only;
    u8 partner_pdo_sink_or_source;
    u8 partner_pdo[VDO_SIZE];
    u8 sel_voltage_pdo_index;
    u8 sys_sta_bak;
    //u8 power_operation_mode;
    u8 enable_report_power_on;
    u8 data_operation_mode_uom;
    u8 power_operation_mode_pdm;
    u8 data_operation_mode_uor;
    u8 power_operation_mode_pdr;

    //struct connector_status_change anx_csc;
    union {
        u16 connector_change_indicator;
        struct connector_status_change1 csc;
    }csc;
    u8 anx_vbus_status;
    u8 anx_initial_status;
	#define INIT_STAGE_1    1
	#define INIT_STAGE_2    2
	#define INIT_STAGE_3    3
	#define INIT_STAGE_4    4
	#define INIT_STAGE_5    5
	#define INIT_DONE       0

    /* buffer pointer */
    u8 pbuf_rx_front;
    u8 pbuf_tx_rear;

    u8 *InterfaceRecvBuf;
    u8 *InterfaceSendBuf;

    u8 connector_index;

    u8 trysnk_enable;
    u8 trysrc_enable;
    u8 pd_capability;
	#define PD_HAS_RETURN_STATUS    (1<<0)
	#define PD_HAS_SVID_MID         (1<<1)
	#define PD_HAS_CABLE_VDO        (1<<2)
	#define PD_HAS_REDRIVER         (1<<3)

    /* I2C Slave address */
    u8 PD_standalone_slave_id1;
    u8 PD_standalone_slave_id2;
	#define addr    PD_standalone_slave_id1
	#define addr2   PD_standalone_slave_id2
    u8 PD_EMTB_slave_id;
    u8 PD_EMRB_slave_id;
    u8 PD_SPI_slave_id;
	#define spi PD_SPI_slave_id
    u8 PD_TCPM_slave_id;

    /* GPIO definition */
    u8 mux_mode;
    u8 cc_orientation;
    u8 error_count;
}anx_context;

extern struct anx_context context[];
extern struct bios_share_msg ppm2bios;

BYTE  ucsi_connector_change_notify_opm(u8 ack, u8 connector_index, u8 wait_for_ack);
extern void ucsi_async_notify_raise_up(u8 ms);

extern void PD_TARSetup(BYTE i2c_addr);
extern void i2c_write_reg(BYTE i2c_addr, BYTE reg, BYTE data1);
extern BYTE i2c_read_reg(BYTE i2c_addr, BYTE reg);
extern void i2c_write_block_reg_b(BYTE i2c_addr, BYTE reg, BYTE length, BYTE *buf);
extern void i2c_read_block_reg(BYTE i2c_addr, BYTE reg, BYTE length, BYTE *buf);

extern void ANX_HOOK_10ms(void);
extern void ANX_HOOK_1ms(void);

#endif
