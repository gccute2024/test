/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:22:02
 * @Description: Define ANX7447 chip operation interface.
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

#include "KERNEL_INCLUDE.H"
#include "CUSTOM_INCLUDE.H"


XBYTE CC_status[32];
XBYTE c;

// Battery Status
struct battery_status_data_obj
{
    u8 reserved; // Reserved
    u8 battery_info; // Battery Info
    u16 battery_pc; // Battery PC
};

// Alert Message
struct alert_data_obj
{
    u16 reserved     :16; // Reserved
    u8 hot_swappable_batteries :4; // Battery Info
    u8 fixed_batteries :4; // Battery Info
    u8 type_of_alert   :8; // Battery PC
};

// Source Capabilities Extended
struct source_cap_extended
{
    u16 VID;
    u16 PID;
    u32 XID;
    u8 fw_version;
    u8 hw_version;
    u8 voltage_regulation;
    u8 holdup_time;
    u8 compliance;
    u8 touch_current;
    u16 peak_current1;
    u16 peak_current2;
    u16 peak_current3;
    u8 touch_temp;
    u8 source_inputs;
    u8 batteries;
    u8 source_PDP;
};

// Sink Capabilities Extended
struct sink_cap_extended
{
    u16 VID;
    u16 PID;
    u32 XID;
    u8 fw_version;
    u8 hw_version;
    u8 Reserved[11];
    u8 sink_inputs;
    u8 batteries;
    u8 sink_PDP;
};


// Status Message
struct status_data
{
    u8 internal_temp;
    u8 present_input;
    u8 present_battery_input;
    u8 event_flags;
    u8 temperature_status;
};

// Battery Cap
struct battery_cap
{
    u16 VID;
    u16 PID;
    u16 battery_design_cap;
    u16 battery_last_full_charge_cap;
    u8 battery_type;
};

// Manufacturer Info
struct manufacturer_info
{
    u8 manufacturer_info_target;
    u8 manufacturer_info_ref;
};

// Manufacturer Info Data
struct manufacturer_info_data
{
    u16 VID;
    u16 PID;
    u8 manufacturer_string[22];
};

// PPS Status Message
struct pps_status_data
{
    u16 output_voltage;
    u8 output_current;
    u8 real_time_flags;
};

struct ext_message_header
{
    u16 data_size                  :9; // Data Size
    u16 reserved                   :1; // Reserved
    u16 request_chunk              :1; // Request Chunk
    u16 chunk_number               :4; // Chunk Number
    u16 chunked                    :1; // Chunked
};

static u8 interface_send_ctr_msg();
static u8 interface_send_msg();

u32 dfp_caps = PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA, PDO_FIXED_DATA_SWAP | PDO_FIXED_EXTERNAL);
u32 src_caps = PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA, PDO_FIXED_COMM_CAP);
u32 dfp_caps_default_ma = PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA, PDO_FIXED_DATA_SWAP | PDO_FIXED_EXTERNAL);
u32 src_caps_default_ma = PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_1500MA, PDO_FIXED_COMM_CAP);
u8 set_variable_1[] = {IF_VAR_fw_var_reg, 0x00, 0x02, 0x01, 0x0d};

u8 src_pdo[] = {0x5A, 0x90, 0x01, 0x2A, 0x96, 0x90, 0x01, 0x2A};

u8 snk_identity[] = {
    //JIRA LBT-413
    #ifdef ANXDONGLE
    0x00, 0x00, 0x00, 0xec,/*snk_id_hdr */
    #else
    0x00, 0x00, 0x00, 0x80,/*snk_id_hdr */
    #endif
	0x00, 0x00, 0x00, 0x00,	/*snk_cert */
	0x00, 0x00, 0x00, 0x00,	/*snk_prd*/
    //JIRA LBT-413
    #ifdef ANXDONGLE
	0x39, 0x00, 0x00, 0x51,	/*5snk_ama*/
    #endif
    };

u8 svid[] = {0x00, 0x00, 0x00, 0xff };

u8 enter_exit_svid[] = {0xff, 0x01, 0x01, 0x00};

VDWORD snk_cap[3] = {
			 /*5V, 0.9A, Fixed *///PDO_FIXED(PD_VOLTAGE_5V, PD_CURRENT_900MA, PDO_FIXED_FLAGS | PDO_FIXED_COMM_CAP),
			 /*20V, 3.25A, Fixed */PDO_FIXED(PD_MAX_VOLTAGE_20V, PD_CURRENT_3A, PDO_FIXED_FLAGS | PDO_FIXED_COMM_CAP),
             /*min 5V, max 20V, power 40W, battery */PDO_BATT(PD_VOLTAGE_5V, PD_MAX_VOLTAGE_20V, PD_POWER_40W),
             /*min5V, max 20V, current 3A, variable */PDO_VAR(PD_VOLTAGE_5V, PD_MAX_VOLTAGE_20V, PD_CURRENT_3A)
};
//JIRA LBT-411
u8 dp_source_dp_caps[4] = {0xC6, 0x00, 0x00, 0x00};
u8 Liberty_i2c_address[PD_MAX_INSTANCE][4] = {
	{0x2C, 0x3F, 0x3D, 0x42},
    {0x2B, 0x37, 0x3C, 0x43}
};

s8 *result_to_str(u8 result_type)
{
    return  (result_type == CMD_SUCCESS) ? "Accept" :
        (result_type == CMD_FAIL) ? "Fail" :
        (result_type == CMD_BUSY) ? "Busy" :
        (result_type == CMD_REJECT) ? "Reject":         
        "Unknown";
}

s8 *interface_to_str(u8 header_type)
{
#if 1
    s8 * str;
    switch (header_type) {
		case TYPE_PWR_SRC_CAP:
				str = "PWR_SRC_CAP";
				break;
		case TYPE_PWR_SNK_CAP:
				str = "PWR_SNK_CAP";
				break;
		case TYPE_PWR_OBJ_REQ:
				str = "PWR_OBJ_REQ";
				break;
		case TYPE_DP_SNK_IDENTITY:
				str = "DP_SNK_IDENTITY";
				break;
		case TYPE_SVID:
				str = "SVID";
				break;
		case TYPE_PSWAP_REQ:
				str = "PSWAP_REQ";
				break;
		case TYPE_DSWAP_REQ:
				str = "DSWAP_REQ";
				break;
		case TYPE_GOTO_MIN_REQ:
				str = "GOTO_MIN_REQ";
				break;
		case TYPE_DP_ALT_ENTER:
				str = "DPALT_ENTER";
				break;
		case TYPE_DP_ALT_EXIT:
				str = "DPALT_EXIT";
				break;
		case TYPE_GET_SRC_CAP:
				str = "GET_SRC_CAP";
				break;
		case TYPE_GET_RDO:
				str = "GET_RDO";
				break;
		case TYPE_GET_SNK_CAP:
				str = "GET_SNK_CAP";
				break;
		case TYPE_VCONN_SWAP_REQ:
				str = "VCONN_SWAP_REQ";
				break;
		case TYPE_GET_DP_SNK_CAP:
				str = "GET_DP_SINK_CAP";
				break;
		case TYPE_DP_SNK_CFG:
				str = "DP_SNK_CFG";
				break;
		case TYPE_SOFT_RST:
				str = "Software Reset";
				break;
		case TYPE_HARD_RST:
				str = "Hardware Reset";
				break;
		case TYPE_RESTART:
				str = "Restart";
				break;
		case TYPE_GET_PD_STATUS:
				str = "PD_STATUS_REQ";
				break;
		case TYPE_ACCEPT:
				str = "ACCEPT";
				break;
		case TYPE_REJECT:
				str = "REJECT";
				break;
		case TYPE_VDM:
				str = "VDM";
				break;
		case TYPE_RESPONSE_TO_REQ:
				str = "RESPONSE_TO_REQ";
				break;
		case TYPE_EXT_SRC_CAP_EXT:
				str = "PD3_SRC_CAP";
				break;
		case TYPE_EXT_STS:
				str = "PD3_STS";
				break;
		case TYPE_EXT_GET_BATT_CAP:
				str = "PD3_GET_BATT_CAP";
				break;
		case TYPE_EXT_GET_BATT_STS:
				str = "PD3_GET_BATT_STS";
				break;
		case TYPE_EXT_BATT_CAP:
				str = "PD3_BATT_CAP";
				break;
		case TYPE_EXT_GET_MFR_INFO:
				str = "PD3_GET_MFR_INFO";
				break;
		case TYPE_EXT_MFR_INFO:
				str = "PD3_MFR_INFO";
				break;
		case TYPE_EXT_PDFU_REQUEST:
				str = "PD3_PDFU_REQUEST";
				break;
		case TYPE_EXT_PDFU_RESPONSE:
				str = "PD3_PDFU_RESPONSE";
				break;
		case TYPE_BATT_STS:
				str = "PD3_BATT_STS";
				break;
		case TYPE_ALERT:
				str = "PD3_ALERT";
				break;
		case TYPE_NOT_SUPPORTED:
				str = "PD3_NOT_SUPPORTED";
				break;
		case TYPE_GET_SRC_CAP_EXT:
				str = "PD3_GET_SRC_CAP";
				break;
		case TYPE_GET_STS:
				str = "PD3_GET_SRC_STS";
				break;
		case TYPE_FR_SWAP:
				str = "PD3_FR_SWAP";
				break;
		case TYPE_GET_PPS_STS:
				str = "PD3_GET_PPS_STS";
				break;
		case TYPE_FR_SWAP_SIGNAL:
				str = "PD3_FR_SWAP_SIGNAL";
				break;
		case TYPE_GET_COUNTRY_INFO:
				str = "PD3_GET_COUNTRY_INFO";
				break;
		case TYPE_DP_DISCOVER_MODES_INFO:
				str = "TYPE_DP_DISCOVER_MODES_INFO";
				break;
		case TYPE_GET_DP_CONFIGURE:
				str = "TYPE_GET_DP_CONFIGURE";
				break;
		case TYPE_DP_CONFIGURE:
				str = "TYPE_DP_CONFIGURE";
				break;
		case TYPE_I2C_MASTER_WRITE:
				str = "TYPE_I2C_MASTER_WRITE";
				break;
		case TYPE_GET_COUNTRY_CODE:
				str = "PD3_GET_COUNTRY_CODE";
				break;
		case TYPE_GET_SINK_CAP_EXT:
				str = "PD3_GET_SINK_CAP_EXT";
				break;
		case TYPE_EXT_SINK_CAP_EXT:
				str = "PD3_EXT_SINK_CAP_EXT";
				break;
		case TYPE_EXT_PPS_STS:
				str = "PD3_EXT_PPS_CAP_STS";
				break;
		case TYPE_EXT_COUNTRY_INFO:
				str = "PD3_EXT_COUNTRY_INFO";
				break;
		case TYPE_EXT_COUNTRY_CODE:
				str = "PD3_EXT_COUNTRY_CODE";
				break;
		case TYPE_GET_VAR:
				str = "TYPE_GET_VAR";
				break;
		case TYPE_SET_VAR:
				str = "TYPE_SET_VAR";
				break;
		default:
				str = "Unknown";
				break;
    }
#endif
    return str;
}

u8 get_data_role()
{
    /*fetch the data role */
    PDk = anx_read_reg(context[port_id].spi, SYSTEM_STSTUS);

    return (PDk & S_DATA_ROLE) != 0;

}


u8 get_power_role()
{
    /*fetch the power role */
    PDk = anx_read_reg(context[port_id].spi, SYSTEM_STSTUS);

    return ((PDk & VBUS_STATUS) != 0);
}

#ifdef ANXREDRIVER
#define plug_orientation(port) (anx_read_reg(context[port].spi,NEW_CC_STATUS)& 0x0d)
void redriver_mux_control(BYTE  mode, BYTE  cc_direction)  //redriver control 
{
    BYTE  buf[3];
    buf[0] = REDRIVER_ADDRESS; //address
    buf[1] = REDRIVER_OFFSET; //offset
    if (cc_direction == CC1_CONNECTED) {
        if (mode == USB3_1_DP_2LANES) {
            buf[2] = 0xf8; //data
        } else if (mode == DP_ALT_4LANES) {
            buf[2] = 0xe8; //data
        }  else if (mode == USB3_1_CONNECTED) {
            buf[2] = 0xb8; //data
        }
    } else {
        if (mode == USB3_1_DP_2LANES) {
            buf[2] = 0xfc; //data
        } else if (mode == DP_ALT_4LANES) {
            buf[2] = 0xec; //data
		} else if (mode == USB3_1_CONNECTED) {
            buf[2] = 0xbc; //data
        }
    }
    send_pd_msg((PD_MSG_TYPE)TYPE_I2C_MASTER_WRITE, buf, 3, 0);
}
void dp_configure_redriver(BYTE  mode) 
{
    BYTE  mux_mode;
    //Fix JIRA LBT-302
    if((mode == SELECT_PIN_ASSIGMENT_C) || (mode == SELECT_PIN_ASSIGMENT_E) )
        mux_mode = DP_ALT_4LANES;
    else
        mux_mode = USB3_1_DP_2LANES;
    if(context[port_id].mux_mode != mux_mode) {
        redriver_mux_control(mux_mode, context[port_id].cc_orientation);
        context[port_id].mux_mode = mux_mode;
    }
}
#endif
/* default request max RDO */
void build_rdo_from_source_caps_new(u8 obj_cnt, u8 *buf)
{
	if(context[port_id].sel_voltage_pdo_index + 1 > obj_cnt)
	{
		for (i= 0; i < obj_cnt; i++){
		{
			((BYTEP )&pdo_max_tmp)[0] = buf[4*i+3];
			((BYTEP )&pdo_max_tmp)[1] = buf[4*i+2];
			((BYTEP )&pdo_max_tmp)[2] = buf[4*i+1];
			((BYTEP )&pdo_max_tmp)[3] = buf[4*i+0];
			switch(GET_PDO_TYPE(pdo_max_tmp))
			{	
				case (PDO_TYPE_FIXED >> 30):
					if((GET_PDO_FIXED_VOLT(pdo_max_tmp)) > pdo_max)
					{
						pdo_max = GET_PDO_FIXED_VOLT(pdo_max_tmp);
						context[port_id].sel_voltage_pdo_index = i;
					}
					break;
				case(PDO_TYPE_VARIABLE >> 30):
					if(GET_VAR_MAX_VOLT(pdo_max_tmp) > pdo_max)
					{
						pdo_max = GET_VAR_MAX_VOLT(pdo_max_tmp);
						context[port_id].sel_voltage_pdo_index = i;
					}
					break;
				case(PDO_TYPE_BATTERY >> 30):
					if(GET_BATT_MAX_VOLT(pdo_max_tmp) > pdo_max)
					{
						pdo_max = GET_BATT_MAX_VOLT(pdo_max_tmp);
						context[port_id].sel_voltage_pdo_index = i;
					}
					break;
				default:				
					break;
			}
			}
		}
	}
	else
	{
		((BYTEP )&pdo_max_tmp)[0] = buf[context[port_id].sel_voltage_pdo_index*4+3];
		((BYTEP )&pdo_max_tmp)[1] = buf[context[port_id].sel_voltage_pdo_index*4+2];
		((BYTEP )&pdo_max_tmp)[2] = buf[context[port_id].sel_voltage_pdo_index*4+1];
		((BYTEP )&pdo_max_tmp)[3] = buf[context[port_id].sel_voltage_pdo_index*4+0];
		switch(GET_PDO_TYPE(pdo_max_tmp))
		{
			case(PDO_TYPE_FIXED >> 30):
				pdo_max = GET_PDO_FIXED_VOLT(pdo_max_tmp);
				break;
			case(PDO_TYPE_VARIABLE >> 30):
				pdo_max = GET_VAR_MAX_VOLT(pdo_max_tmp);
				break;
			case(PDO_TYPE_BATTERY >> 30):
				pdo_max = GET_BATT_MAX_VOLT(pdo_max_tmp);
				break;
			default:
				break;
		}
	}
	((BYTEP )&pdo_max_tmp)[0] = buf[context[port_id].sel_voltage_pdo_index*4+3];
	((BYTEP )&pdo_max_tmp)[1] = buf[context[port_id].sel_voltage_pdo_index*4+2];
	((BYTEP )&pdo_max_tmp)[2] = buf[context[port_id].sel_voltage_pdo_index*4+1];
	((BYTEP )&pdo_max_tmp)[3] = buf[context[port_id].sel_voltage_pdo_index*4+0];
	switch(GET_PDO_TYPE(pdo_max_tmp))
	{
		case(PDO_TYPE_FIXED >> 30):
		case(PDO_TYPE_VARIABLE>>30):
			if(GET_PDO_FIXED_CURR(pdo_max_tmp) >= MAX_REQUEST_CURRENT)
			{
				context[port_id].ucsi_partner_rdo = RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1, MAX_REQUEST_CURRENT, MAX_REQUEST_CURRENT, 0);
			}
			else
			{
				context[port_id].ucsi_partner_rdo = RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1, GET_PDO_FIXED_CURR(pdo_max_tmp), GET_PDO_FIXED_CURR(pdo_max_tmp), RDO_CAP_MISMATCH);
			}
			break;
		case (PDO_TYPE_BATTERY >> 30):
			if((GET_BATT_OP_POWER(pdo_max_tmp)>>2) >= (MAX_REQUEST_CURRENT*(MAX_REQUEST_VOLTAGE/1000))>>2)
			{
				context[port_id].ucsi_partner_rdo = RDO_BATT(context[port_id].sel_voltage_pdo_index + 1, MAX_REQUEST_CURRENT*(MAX_REQUEST_VOLTAGE/1000), MAX_REQUEST_CURRENT*(MAX_REQUEST_VOLTAGE/1000), 0);
			}
			else
			{
				context[port_id].ucsi_partner_rdo = RDO_BATT(context[port_id].sel_voltage_pdo_index + 1, (unsigned long)(GET_BATT_OP_POWER(pdo_max_tmp)>>2)*4, (unsigned long)(GET_BATT_OP_POWER(pdo_max_tmp)>>2)*4, RDO_CAP_MISMATCH);
			}
			break;
		default:
			break;
	}
	

}

#ifndef AUTO_RDO_ENABLE
static u8 build_rdo_from_source_caps(u8 obj_cnt, u8 *buf)
{

    pdo_max = 0;
    obj_cnt &= 0x07;

    /* find the max voltage pdo */
    for (i = 0; i < obj_cnt; i++) {

        /* get max voltage now */
        pdo_max_tmp =
            (u16) ((((((((u16) buf[i * 4 + 3] << 8) | (buf[i * 4 + 2])) & 0xf) << 6) | ((((u16) buf[i * 4 + 1] << 8) | (buf[i * 4 + 0])) >> 10)) &
                    0x3ff) * 50);
        if (pdo_max_tmp > pdo_max) {
            pdo_max = pdo_max_tmp;
            pdo_l = (((u16) buf[i * 4 + 1] << 8) | (buf[i * 4 + 0]));
            pdo_h = (((u16) buf[i * 4 + 3] << 8) | (buf[i * 4 + 2]));
            context[port_id].sel_voltage_pdo_index = i;
        }
    }
#ifdef ANXDEBUG
    dprint("maxV=%d, cnt %d index %d\n", pdo_max_tmp, obj_cnt,
            context[port_id].sel_voltage_pdo_index);
#endif
    if ((pdo_h & (3 << 14)) != (PDO_TYPE_BATTERY >> 16)) {
        //max_request_ma = (u16) ((pdo_l & 0x3ff) * 10);
#ifdef ANXDEBUG
        dprint("maxMa %d\n", (u16) ((pdo_l & 0x3ff) * 10));
#endif
        /* less than 900mA */
        if ((u16) ((pdo_l & 0x3ff) * 10) < MAX_REQUEST_CURRENT) {
            pdo_max =
                RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1, (u16) ((pdo_l & 0x3ff) * 10),
                          (u16) ((pdo_l & 0x3ff) * 10), 0);
            pdo_max |= RDO_CAP_MISMATCH;
            set_rdo_value(pdo_max & 0xff, (pdo_max >> 8) & 0xff,
                          (pdo_max >> 16) & 0xff,
                          (pdo_max >> 24) & 0xff);
            return 1;
        } else {
            pdo_max =
                RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1,
                          MAX_REQUEST_CURRENT, MAX_REQUEST_CURRENT,
                          0);
            set_rdo_value(pdo_max & 0xff, (pdo_max >> 8) & 0xff,
                          (pdo_max >> 16) & 0xff,
                          (pdo_max >> 24) & 0xff);

            return 1;
        }
    } else {
        pdo_max =
            RDO_FIXED(context[port_id].sel_voltage_pdo_index + 1, MAX_REQUEST_CURRENT,
                      MAX_REQUEST_CURRENT, 0);
        set_rdo_value(pdo_max & 0xff, (pdo_max >> 8) & 0xff,
                      (pdo_max >> 16) & 0xff, (pdo_max >> 24) & 0xff);
        return 1;
    }
#ifdef ANXDEBUG
    dprint("RDO Mismatch !!!\n");
#endif
    set_rdo_value(0x0A, 0x28, 0x00, 0x10);

    return 0;
}
#endif

static u32 change_bit_order(const u8 *pbuf)
{
    return ((u32)pbuf[3] << 24) | ((u32)pbuf[2] << 16) 
        | ((u32)pbuf[1] << 8) | pbuf[0];
}

static u8 pd_check_requested_voltage()
{
    if (!(context[port_id].ucsi_partner_rdo >> 28) || (context[port_id].ucsi_partner_rdo >> 28) > context[port_id].pd_src_pdo_cnt) {
#ifdef ANXDEBUG
        dprint("rdo = %x, Requested RDO is %d, Provided RDO number is %d\n", context[port_id].ucsi_partner_rdo, (u16)(context[port_id].ucsi_partner_rdo >> 28), (u8)context[port_id].pd_src_pdo_cnt);
#endif
        return 0; /* Invalid index */
    }

#ifdef ANXDEBUG
    dprint("pdo_max = %x\n", (change_bit_order(context[port_id].pd_src_pdo + (((context[port_id].ucsi_partner_rdo >> 28) - 1) * 4)) & 0x3ff));
#endif

    /* check current ... */
    if (((context[port_id].ucsi_partner_rdo >> 10) & 0x3FF) > (change_bit_order(context[port_id].pd_src_pdo + (((context[port_id].ucsi_partner_rdo >> 28) - 1) * 4)) & 0x3ff))//Update to pass TD.PD.SRC.E12 Reject Request
        return 0; /* too much op current */
    if ((context[port_id].ucsi_partner_rdo & 0x3FF) > (change_bit_order(context[port_id].pd_src_pdo + (((context[port_id].ucsi_partner_rdo >> 28) - 1) * 4)) & 0x3ff))//Update to pass TD.PD.SRC.E12 Reject Request
        return 0; /* too much max current */

    return 1;
}

void send_source_capability(void)
{
    #ifdef ANX_OHO_DFP_ONLY
    memcpy(context[port_id].pd_src_pdo, (u8 *)&dfp_caps, sizeof(dfp_caps));
	#else
    #ifdef ECR_CHECKING
    if ((supply_1500ma_port == port_id) && (supply_1500ma_flag == 1))
        memcpy(context[port_id].pd_src_pdo, (u8 *)&src_caps, 4);
    else
    #endif
        memcpy(context[port_id].pd_src_pdo, (u8 *)&src_caps_default_ma, 4);
    #endif
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_DWORD(&context[port_id].pd_src_pdo);
    #endif
    //Fix JIRA LBT-410
    if ((context[port_id].data_operation_mode_uor & OP_DRP) &&
		((context[port_id].data_operation_mode_uom & OP_DRP))){
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
			((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_src_pdo[3] |=
                PDO_FIXED_HIGH_BYTE_DATA_SWAP | PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else
            context[port_id].pd_src_pdo[3] |= PDO_FIXED_HIGH_BYTE_DATA_SWAP;
    } else {
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
			((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_src_pdo[3] |= PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else
            ;//default is no drp, no dual role
    }
    context[port_id].pd_src_pdo_cnt = 1;
    send_pd_msg(TYPE_CMD_SRC_CAP_1, (u8 *)&context[port_id].pd_src_pdo,4, SOP_TYPE);
}

void send_sink_capability(void)
{
    memcpy(context[port_id].pd_snk_pdo, (u8 *)&snk_cap, 8);
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_DWORD(&context[port_id].pd_snk_pdo);
	SWAP_DWORD((u8 *)&context[port_id].pd_snk_pdo + 4);
    #endif
    //Fix JIRA LBT-410
    if ((context[port_id].data_operation_mode_uor & OP_DRP) &&
		((context[port_id].data_operation_mode_uom & OP_DRP))){
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
			((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_snk_pdo[3] |=
                PDO_FIXED_HIGH_BYTE_DATA_SWAP | PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else
            context[port_id].pd_snk_pdo[3] |= PDO_FIXED_HIGH_BYTE_DATA_SWAP;
    } else {
        if ((context[port_id].power_operation_mode_pdr & PW_DRP) &&
			((context[port_id].power_operation_mode_pdm & PW_DRP)))
            context[port_id].pd_snk_pdo[3] |= PDO_FIXED_HIGH_BYTE_DUAL_ROLE;
        else
            ;//default is no drp, no dual role
    }
    //dprint("Resend 1.5A sink caps, sink caps byte 3 is 0x%x\n",context[port_id].pd_snk_pdo[3]);
    context[port_id].pd_snk_pdo_cnt = 2;
    send_pd_msg(TYPE_PWR_SNK_CAP, (u8 *)&context[port_id].pd_snk_pdo,8, SOP_TYPE);
}

void send_source_sink_capability(void)
{
    send_sink_capability();
    send_source_capability();
}

#ifdef ECR_CHECKING
static void Send_mismatch_source_caps(void)
{
    #ifdef ANXDEBUG
    dprint("Send mismatch source caps, 1.5A source caps.\n");
    #endif
    supply_1500ma_flag = 1;
    supply_1500ma_port = port_id;

    send_source_capability();
}
#endif

/*ChengLin: To integrate all recv_pd_xxxxxx_default_callback() functions to single one for reduce code size*/
static u8 Recv_PD_Commands_Default_Callback()
{
#ifdef ANXDEBUG
    u8 *pdo;
    u16 usb_vendor_id, usb_product_id;
#endif

    PDl = 1; // 0: Fail, 1: Pass 
    
    if (TYPE_HARD_RST != recvd_msg_type())
        context[port_id].ct = CABLE_TYPE_C;


    /*ChengLin: Special Check for Specific Commands*/
    if(recvd_msg_type() == TYPE_PWR_SRC_CAP)
    {
        if ( recvd_msg_len()  %4 != 0)
            return 0;
    }
    else if(recvd_msg_type() == TYPE_PWR_SNK_CAP)
    {

    }
    else if(recvd_msg_type() == TYPE_PWR_OBJ_REQ)
    {
        if (recvd_msg_len()  != 4)
            return 1;
    }

    switch(recvd_msg_type())
    {
        case TYPE_PWR_SRC_CAP: //0x00
            #ifndef AUTO_RDO_ENABLE
            //build_rdo_from_source_caps(recvd_msg_len() /4, recvd_msg_buf());
            set_rdo_value(context[port_id].ucsi_partner_rdo & 0xff,
                      ((context[port_id].ucsi_partner_rdo >> 8) & 0xff),
                      ((context[port_id].ucsi_partner_rdo >> 16) & 0xff),
                      ((context[port_id].ucsi_partner_rdo >> 24) & 0xff))
            send_pd_msg(TYPE_PWR_OBJ_REQ, &context[port_id].pd_rdo, 4, SOP_TYPE);
            #endif
            build_rdo_from_source_caps_new(recvd_msg_len() /4, recvd_msg_buf());
            send_pd_msg(TYPE_GET_RDO, 0, 0, SOP_TYPE);
            context[port_id].partner_pdo_length = recvd_msg_len() ;
            context[port_id].partner_pdo_sink_or_source = 1; //partner as source
            context[port_id].anx_power_role = 0;
            /*voltage->0x7e:0xaf, current:0x7e:0xb1*/
            context[port_id].anx_current_in_50ma = anx_read_reg(context[port_id].spi, 0xb1);
            context[port_id].anx_voltage_in_100mv = anx_read_reg(context[port_id].spi, 0xaf);
            context[port_id].csc.csc.NegotiatedPowerLevel = 1;
            context[port_id].csc.csc.SupportedProviderCap = 1;
            context[port_id].csc.csc.ExternalSupply = 1;
            context[port_id].BatteryCharging = BCS_NOMINAL_CHARGING;
            //#ifdef ANXDEBUG
            dprint("current:%x, voltage:%x, rdo :%lx, rdo length(%.bx)\n", (u16)context[port_id].anx_current_in_50ma,
                (u16)context[port_id].anx_voltage_in_100mv, context[port_id].ucsi_partner_rdo, context[port_id].partner_pdo_length);
            //#endif
            ucsi_async_notify_raise_up(100);
            memcpy(context[port_id].partner_pdo, recvd_msg_buf(), min(VDO_SIZE, recvd_msg_len() ));
            break;
        case TYPE_PWR_SNK_CAP://0x01
            //received peer's sink caps, just store it in SINK_PDO array
            context[port_id].partner_pdo_length = recvd_msg_len() ;
            context[port_id].partner_pdo_sink_or_source = 0;
            memcpy(context[port_id].partner_pdo, recvd_msg_buf(), min(VDO_SIZE, recvd_msg_len() ));
            dprint("current:%x, voltage:%x, rdo :%lx, rdo length(%.bx)\n", (u16)context[port_id].anx_current_in_50ma,
                (u16)context[port_id].anx_voltage_in_100mv, context[port_id].ucsi_partner_rdo, context[port_id].partner_pdo_length);
            if(recvd_msg_len()  > VDO_SIZE)
            {
                PDl = 0;
                break;
            }

            //OHO-407: When Received Sink Capability, API Shall Not Overwrite Its Sink Capability By the Received One
            break;
        case TYPE_DP_SNK_IDENTITY://0x02
            #ifdef ANXDEBUG
            pdo = (u8 *)recvd_msg_buf();
            usb_vendor_id = (u16)pdo[1] << 8 | (u16)pdo[0];
            usb_product_id = (u16)pdo[11] << 8 | (u16)pdo[10];

            #endif
            dprint("TYPE_DP_CNK_IDENTITY\n");
            break;
        case TYPE_SVID://0x03
            break;
        case TYPE_ACCEPT://0x05
            break;
        case TYPE_REJECT://0x06
            break;
        case TYPE_PSWAP_REQ://0x10
            context[port_id].power_role_swap_flag = 1;
            context[port_id].anx_power_role = get_power_role();
            context[port_id].csc.csc.PowerOperationMode = 1;
            ucsi_async_notify_raise_up(50);
            break;
        case TYPE_DSWAP_REQ://0x11
            context[port_id].csc.csc.ConnectorPartner = 1;
            if (get_data_role() == 1)
                context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;
			else
                context[port_id].ucsi_connector_partner_type = CPT_DFP_ATTACHED;
            ucsi_async_notify_raise_up(50);
            break;
        case TYPE_GOTO_MIN_REQ://0x12
            break;
        case TYPE_DP_SNK_CFG:
            break;
        case TYPE_DP_DISCOVER_MODES_INFO:
            //pdo = (u8 *)para;
            context[port_id].peer_svid_length += 4;
            context[port_id].csc.csc.SupportedCAM = 1;
            context[port_id].ac[context[port_id].peer_svid_length/4 - 1].svid = ((u16)recvd_msg_buf()[3] << 8) | (u8)recvd_msg_buf()[2];
            memcpy(&context[port_id].ac[context[port_id].peer_svid_length/4 - 1].mid, recvd_msg_buf() + 4, 4);
            break;
        case TYPE_DP_ALT_ENTER://0x19
            context[port_id].csc.csc.SupportedCAM = 1;
            send_pd_msg(TYPE_GET_DP_CONFIGURE, 0, 0, SOP_TYPE);
            context[port_id].ConnectorPartner |= CP_ALT_MODE;
            context[port_id].csc.csc.ConnectorPartner = 1;
            context[port_id].csc.csc.SupportedCAM = 1;
            ucsi_async_notify_raise_up(50);
            break;
        case TYPE_DP_ALT_EXIT://0x1A
            //TODO....
            break;
        case TYPE_PWR_OBJ_REQ:  //0x16
            //pdo = (u8 *)para;

            if (recvd_msg_len() != 4)
                break;
            context[port_id].ucsi_partner_rdo = recvd_msg_buf()[0] | ((u32)recvd_msg_buf()[1] << 8) | ((u32)recvd_msg_buf()[2] << 16) | ((u32)recvd_msg_buf()[3] << 24);
            #ifdef ECR_CHECKING
            if ((context[port_id].anx_vbus_status == 1) &&
                    (recvd_msg_buf()[3] & _BIT2) && supply_1500ma_flag == 0)
                Send_mismatch_source_caps();
            #ifdef ANXDEBUG
            dprint("RDO : %lx, vbus(%.bx), supply_flag(%.bx).\n",
                context[port_id].ucsi_partner_rdo, context[port_id].anx_vbus_status, supply_1500ma_flag);
            #endif
            #endif
            context[port_id].csc.csc.NegotiatedPowerLevel = 1;
            context[port_id].csc.csc.SupportedProviderCap = 1;
            #ifdef AUTO_RDO_ENABLE
            //ucsi_async_notify_raise_up(50);
            break;
            #endif
            //if((anx_read_reg(context[port_id].spi, FIRMWARE_CTRL) & auto_pd_en))
            //  break;

            if (pd_check_requested_voltage()) {
                PDl = send_pd_msg(TYPE_ACCEPT, NULL, 0, SOP_TYPE);
            } else {
                PDl = send_pd_msg(TYPE_REJECT, NULL, 0, SOP_TYPE);
            }
            context[port_id].downstream_pd_cap = 1;
            break;
        case TYPE_GET_PPS_STS: //0x21
            //TRACE("recv: PD_CTRL_GET_PPS_STATUS\n");
            break;
        case TYPE_BATT_STS://0x24
            //TRACE("recv: PD_BATTERY_STATUS\n");
            break;
        case TYPE_ALERT: //0x25
            break;
        #ifdef ANXREDRIVER
        case TYPE_DP_CONFIGURE: //0x2a	redriver control		
            if ((context[port_id].pd_capability & PD_HAS_REDRIVER) &&
                  ((recvd_msg_buf()[0] != 0) || (recvd_msg_buf()[1] != 0) ||
                  (recvd_msg_buf()[2] != 0) || (recvd_msg_buf()[3] != 0)))
                dp_configure_redriver(recvd_msg_buf()[1]);
            break;
        #endif
        case TYPE_GET_SRC_CAP_EXT: //0x1d
            break;
        case TYPE_GET_STS: //0x1e           
            break;
        case TYPE_EXT_SRC_CAP_EXT: //0x30
            break;
        case TYPE_EXT_STS://0x31        
            break;
        case TYPE_EXT_GET_BATT_CAP://0x32
            break;
        case TYPE_EXT_GET_BATT_STS://0x33
            //TRACE("recv: PD_EXT_GET_BATTERY_STATUS\n");
            break;
        case TYPE_EXT_BATT_CAP://0x34
            //TRACE("recv: PD_EXT_BATTERY_CAP\n");
            break;
        case TYPE_EXT_GET_MFR_INFO://0x35
            break;
        case TYPE_EXT_MFR_INFO: //0x36
            break;
        case TYPE_EXT_COUNTRY_INFO: //0x38
            break;          
        case TYPE_EXT_COUNTRY_CODE: //0x39
            break;
        case TYPE_EXT_PPS_STS://0x37        
            break;
        case TYPE_EXT_SINK_CAP_EXT: //0x3a          
            break;
        case TYPE_NOT_SUPPORTED: //0x1c
            //TRACE("recv: PD_CTRL_NOT_SUPPORTED\n");
            break;
        case TYPE_SOFT_RST:
        case TYPE_HARD_RST:
            context[port_id].csc.csc.PDResetComplete = 1;
            ucsi_async_notify_raise_up(2);
            break;
        case TYPE_FR_SWAP: //0x1f
        default:
            break;
    }

 return PDl;

}

/* Recieve response message's callback function.
  * it can be rewritten by customer just reimmplement this function,
  * through register_default_pd_message_callbacku_func  
  *  void *para : should be null
  *   para_len : 0
  * return:  0, fail;   1, success
  */
static u8 Recv_PD_Cmd_Rsp_Default_Callback()
{
    //para = para;
    //para_len = para_len;
    #ifdef ANXDEBUG
    dprint("RESPONSE for %s is %s\n",
        interface_to_str(RESPONSE_REQ_TYPE()),result_to_str(RESPONSE_REQ_RESULT()));
    #endif

    switch(RESPONSE_REQ_TYPE()){
        case TYPE_DSWAP_REQ:
            //need_notice_pd_cmd  =1;
            //usb_pd_cmd_status = RESPONSE_REQ_RESULT();
            //dswap_response_got = 1;

            context[port_id].csc.csc.ConnectorPartner = 1;
            break;
            
        case TYPE_PSWAP_REQ:
            context[port_id].power_role_swap_flag = 1;

            break;
        case TYPE_VCONN_SWAP_REQ:
            break;
        case TYPE_GOTO_MIN_REQ:
            break;
        case TYPE_PWR_OBJ_REQ:
            break;
        case TYPE_SOFT_RST:
            context[port_id].csc.csc.PDResetComplete = 1;
            break;
        case TYPE_HARD_RST:
            context[port_id].csc.csc.PDResetComplete = 1;
            break;
            
        default:
            break;
    } 

    return 1;
}

/* Recieve Power Delivery Unstructured VDM message's callback function.
  * it can be rewritten by customer just reimmplement this function,
  * through register_default_pd_message_callbacku_func  
  *  void *para : in this function it means PDO pointer
  *   para_len : means PDO length
  * return:  0, fail;   1, success
  */
static u8 Recv_PD_VDM_Defalut_Callback()
{
    //Make sure message is SOP1 and type is IDENTITY
    if ((recvd_msg_sop_type() == SOP1_TYPE) && ((recvd_msg_buf()[0]&0x0F) == 0x1)) {
        if (recvd_msg_len() < 20) {
#ifdef ANXDEBUG
            dprint("no cable vdo.\n");
#endif
            return 1;
        }

        *(u32 *)&context[port_id].cv = ((u32)recvd_msg_buf()[19] << 24) | ((u32)recvd_msg_buf()[18] << 16) |
            ((u32)recvd_msg_buf()[17] << 8) | recvd_msg_buf()[16];;
    }
    return 1;
}

static u8 Recv_Debug_Callback()
{
    switch(recvd_msg_type()) {
    case TYPE_GET_VAR:
        if(recvd_msg_buf()[0] == 0) // idata
        {
#ifdef ANXDEBUG
            dprint("read 0x%x idata = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == 1) // xdata
        {
#ifdef ANXDEBUG
            dprint("read 0x%x xdata = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_fw_var_reg) // REG_FW_VAR
        {
#ifdef ANXDEBUG
            dprint("read REG_FW_VAR[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_pd_src_pdo)
        {
#ifdef ANXDEBUG
            dprint("read pd_src_pdo[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_pd_snk_pdo)
        {
#ifdef ANXDEBUG
            dprint("read pd_snk_pdo[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_pd_rdo_bak)
        {
#ifdef ANXDEBUG
            dprint("read pd_rdo_bak[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_pd_rdo)
        {
#ifdef ANXDEBUG
            dprint("read pd_rdo[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_DP_caps)
        {
#ifdef ANXDEBUG
            dprint("read DP_cap[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_configure_DP_caps) // REG_FW_VAR
        {
#ifdef ANXDEBUG
            dprint("read configure_DP_caps[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_src_dp_status) // REG_FW_VAR
        {
#ifdef ANXDEBUG
            dprint("read src_dp_status[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_sink_svid_vdo) // REG_FW_VAR
        {
#ifdef ANXDEBUG
            dprint("read sink_svid_vdo[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else if(recvd_msg_buf()[0] == IF_VAR_sink_identity) // REG_FW_VAR
        {
#ifdef ANXDEBUG
            dprint("read sink_identity[0x%x] = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        else 
        {
#ifdef ANXDEBUG
            dprint("read 0x%x ?data = ",((u16)recvd_msg_buf()[1]<<8) + recvd_msg_buf()[2]);
#endif
        }
        break;
    default:
        break;
    }

    return 1;
}

/*ChengLin: To integrate all send_xxx DATA Messages to one function for reduce code size*/
static u8 Send_PD_Data_Messages(u8 type_msg, u8 *DataBuff, u8 size)
{

    if ( NULL == DataBuff )
        return CMD_FAIL;

    if(type_msg == TYPE_DP_SNK_CFG)
    {
        memcpy(context[port_id].configure_DP_caps, DataBuff, 4);
        memcpy(context[port_id].InterfaceSendBuf + 2, DataBuff, 4); 
        //memcpy(context[port_id].InterfaceSendBuf + 2 + 4, DP_caps, 4);
        memset(context[port_id].InterfaceSendBuf + 2 + 4, 0, 4);
        context[port_id].InterfaceSendBuf[0] = 4 + 4 + 1; // + cmd
    }
    else
    {
        if (type_msg == TYPE_DP_SNK_IDENTITY)
            memcpy(context[port_id].src_dp_caps, DataBuff, size);
        memcpy(context[port_id].InterfaceSendBuf + 2, DataBuff, size);
        context[port_id].InterfaceSendBuf[0] = size + 1; // + cmd
    }
    
    context[port_id].InterfaceSendBuf[1] = type_msg;

    return interface_send_msg();
}

BYTE  Send_Interface_Messages(u8 type_msg, u8 *DataBuff, u8 size)
{
    memcpy(context[port_id].InterfaceSendBuf + 2, DataBuff, size);
    context[port_id].InterfaceSendBuf[0] = size + 1; // + cmd
    context[port_id].InterfaceSendBuf[1] = type_msg;
    return interface_send_msg();
}

/*ChengLin: To integrate all send_xxx_ control messages to one function for reduce code size*/
static u8 Send_PD_Control_Messages(u8 type_msg)
{
    context[port_id].InterfaceSendBuf[1] = type_msg;
    return interface_send_ctr_msg(); 
}

#define cac_checksum(buf, len) {\
    j = 0;\
    for (int k = 0; k < len; k++)\
        j += *(buf + k);\
}


u8 send_pd_msg(PD_MSG_TYPE type,
        u8 *buf, u8 size, u8 type_sop)
{
    //u8 rst = CMD_FAIL;
#ifdef ANXDEBUG
    dprint("SendMSG ->%s: \n", interface_to_str(type));
#endif
    #if 1
    switch (type) {
        //Send Control Messages
        case TYPE_GET_DP_SNK_CAP: //send 4      
        case TYPE_ACCEPT://send 5   
        case TYPE_REJECT://send 6
        case TYPE_GET_SRC_CAP://send 9
        case TYPE_GET_PD_STATUS://send 17   
        case TYPE_GET_SNK_CAP://0x1B
        case TYPE_GET_SRC_CAP_EXT://send 1d
        case TYPE_GET_STS://send 1e
        case TYPE_FR_SWAP_SIGNAL://send 20  
        case TYPE_GET_PPS_STS: // send 21
        case TYPE_GET_COUNTRY_CODE://send 22
        case TYPE_GET_SINK_CAP_EXT://send 23
        case TYPE_SOFT_RST://send f1
        case TYPE_HARD_RST://send f2        
            Send_PD_Control_Messages(type);
            break;
        case TYPE_GET_RDO:
        case TYPE_GET_DP_CONFIGURE:
        case TYPE_PSWAP_REQ://send 10
        case TYPE_DSWAP_REQ://send 11
        case TYPE_GOTO_MIN_REQ://send 12
        case TYPE_VCONN_SWAP_REQ://send 13      
            if(Send_PD_Control_Messages(type) == CMD_SUCCESS)
                PDl = CMD_SUCCESS;
            else
                PDl = CMD_FAIL;
            break;

        //Send Data Messages
        case TYPE_PWR_SRC_CAP: //send 0
        case TYPE_PWR_SNK_CAP: //send 1
        case TYPE_DP_SNK_IDENTITY: //send 2
        case TYPE_SVID: //send 3
        case TYPE_SET_SNK_DP_CAP: //send 8
        case TYPE_DP_SNK_CFG://send 15
        case TYPE_BATT_STS://send 24
        case TYPE_ALERT://send 25   
        case TYPE_GET_COUNTRY_INFO://send 26
        case TYPE_GET_VAR://send 0xfc
        case TYPE_SET_VAR://send 0xfd
        case TYPE_CMD_SRC_CAP_1:
            PDl = Send_PD_Data_Messages(type, buf, size);
            break;
        case TYPE_PWR_OBJ_REQ://send 16
            if(Send_PD_Data_Messages(type, buf, size) == CMD_SUCCESS)
                PDl = CMD_SUCCESS;
            else
                PDl = CMD_FAIL;
            break;

        //Send VDM Messages
        case TYPE_VDM://send 14
            //l =  send_vdm(port_id, buf, size, type_sop);
            break;
        case TYPE_I2C_MASTER_WRITE: //send 0x3e
            Send_Interface_Messages(type, buf, size);
            break;

        //Send Extend Messages  
        case TYPE_EXT_GET_BATT_CAP://send 32   size = 1
        case TYPE_EXT_GET_BATT_STS://send 33
        case TYPE_EXT_BATT_CAP://send 34
        case TYPE_EXT_GET_MFR_INFO://send 35
        case TYPE_EXT_MFR_INFO://send 36
        case TYPE_EXT_COUNTRY_INFO://send 38
        case TYPE_EXT_COUNTRY_CODE://send 39
        case TYPE_EXT_PDFU_REQUEST://send 3b
            //l = Send_PD_Extend_Messages(port_id,type,buf,size,type_sop);
            //rst =  send_ext_get_battery_cap(port, buf, size);  
            break;
        default:
#ifdef ANXDEBUG
            dprint("unknown type %2X\n", type);
#endif
            PDl = CMD_FAIL;
            break;
    }
#endif
      return PDl;
}


/**
 * @desc:   The Interface that AP handle the specific USB PD command from Ohio 
 *
 * @param: 
 *      type: PD message type, define enum PD_MSG_TYPE. 
 *      buf: the sepecific paramter pointer according to the message type:
 *                      eg: when AP update its source capability type=TYPE_PWR_SRC_CAP, 
 *          "buf" contains the content of PDO object,its format USB PD spec
 *                      customer can easily packeted it through PDO_FIXED_XXX macro:
 *                     default5Vsafe 5V, 0.9A fixed --> PDO_FIXED(5000,900, PDO_FIXED_FLAGS) 
 *                size: the paramter ponter's content length, if buf is null, it should be 0 
 *                  
 * @return:  0: success 1: fail 
 *  
 */ 
static u8 dispatch_rcvd_pd_msg()
{
    //u8 l = CMD_FAIL;
    switch (recvd_msg_type()) {
        case TYPE_VDM: //0x14
            PDl = Recv_PD_VDM_Defalut_Callback();
            break;
        case TYPE_EXT_PDFU_RESPONSE: //0x3c
            //rst = Recv_PD_Firmware_Update_Response_Callback(context, para, para_len, type_sop);
            break;
        case TYPE_RESPONSE_TO_REQ://0xf0
            PDl =  Recv_PD_Cmd_Rsp_Default_Callback();           
                break;
        case TYPE_GET_VAR://0xfc
            PDl = Recv_Debug_Callback();     
            break;
        default:
            PDl = Recv_PD_Commands_Default_Callback();
            break;
    }
    
    return PDl;
}


static void pd_cc_status_default_func(u8 cc_status)
{
    /* cc status */
    context[port_id].power_op_mode = PO_USB_DEFAULT;
    role = get_data_role();
	context[port_id].anx_data_role = role;


    if (role == 0)
        context[port_id].ucsi_connector_partner_type = CPT_DFP_ATTACHED;
    else
        context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;

    switch ((cc_status & 0xF)) {
		case 1:
			switch (((cc_status >> 4) & 0xF)) {
				case 0:
				// CC1 Rd
					//UFP, CC2 Open
					context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;
					break;
				case 1:
					//CC2 Rd, Debug
					context[port_id].ucsi_connector_partner_type = CPT_CAB_DEBUG_ACC;
					break;
				case 2:
					//CC2 Ra
					context[port_id].ucsi_connector_partner_type = CPT_CAB_UFP_ATTACHED;
					break;
			}
			break;
		case 2:
			switch (((cc_status >> 4) & 0xF)) {
				case 0:
				//CC1 Ra
					//UFP, CC2 Open
					context[port_id].ucsi_connector_partner_type = CPT_CAB_NO_UFP_ATTACHED;
					break;
				case 1:
					//CC2 Rd, Debug
					context[port_id].ucsi_connector_partner_type = CPT_CAB_UFP_ATTACHED;
					break;
				case 2:
					//CC2 Ra
					context[port_id].ucsi_connector_partner_type = CPT_AUDIO_ACC;
					break;
			}
			break;
		case 0:
			switch (((cc_status >> 4) & 0xF)) {
				case 1:
				//CC1 Open
					//CC2 Rd
					context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;
					break;
				case 2:
						//CC2 Ra
					context[port_id].ucsi_connector_partner_type = CPT_CAB_NO_UFP_ATTACHED;
					break;
				case 0:
					//context[port_id].ucsi_connector_partner_type = CPT_RESERVED;
					break;
			}
			break;
		case 0x04:
			//dprint("PO_USB_DEFAULT \n");
			context[port_id].power_op_mode = PO_USB_DEFAULT;
			break;
		case 0x08:
			//dprint("PO_TYPE_C_1_5A \n");
			context[port_id].power_op_mode = PO_TYPE_C_1_5A;
			break;
		case 0x0c:
			//dprint("PO_TYPE_C_3A \n");
			context[port_id].power_op_mode = PO_TYPE_C_3A;
			break;
    }

    switch (((cc_status >> 4) & 0xF)) {
		case 0x04:
			context[port_id].power_op_mode = PO_USB_DEFAULT;
			break;
		case 0x08:
			context[port_id].power_op_mode = PO_TYPE_C_1_5A;
			break;
		case 0x0c:
			context[port_id].power_op_mode = PO_TYPE_C_3A;
			break;
    }

}

static u8 interface_send_msg()
{
    cac_checksum(context[port_id].InterfaceSendBuf, send_msg_len()+1);
    context[port_id].InterfaceSendBuf[send_msg_len() + 1] = 0 - j; //cmd + checksum
    len = send_msg_len() + 2;
    c = anx_read_reg(context[port_id].spi, InterfaceSendBuf_Addr);
    // retry
    if(c) {
        count = 250;
        while(count) { 
            ANXmdelay(2);
            c = anx_read_reg(context[port_id].spi, InterfaceSendBuf_Addr);
            if (c == 0)
                break;
            count--;
        }
    }
    if (c == 0) {
        anx_write_block_reg(context[port_id].spi, InterfaceSendBuf_Addr+1 , len-1, &context[port_id].InterfaceSendBuf[1]);
        anx_write_reg(context[port_id].spi,InterfaceSendBuf_Addr, context[port_id].InterfaceSendBuf[0]);
    } else {
#ifdef ANXDEBUG
        dprint("Tx Buf Full\n");
#endif
        return CMD_FAIL;
    }

    return CMD_SUCCESS;
}

static u8 interface_send_ctr_msg()
{
    context[port_id].InterfaceSendBuf[0] = 1; // + cmd
    context[port_id].InterfaceSendBuf[2] = 0-(context[port_id].InterfaceSendBuf[0]+context[port_id].InterfaceSendBuf[1]);
    len = 3;
    c = anx_read_reg(context[port_id].spi, InterfaceSendBuf_Addr);
    // retry
    if(c) {
        count = 250;
        while(count) { 
           ANXmdelay(1);
           c = anx_read_reg(context[port_id].spi, InterfaceSendBuf_Addr);
           if (c == 0)
            break;
           count--;
        }
    }
    if (c == 0) {
        anx_write_block_reg(context[port_id].spi, InterfaceSendBuf_Addr+1 , len-1, &context[port_id].InterfaceSendBuf[1]);

        anx_write_reg(context[port_id].spi,InterfaceSendBuf_Addr, context[port_id].InterfaceSendBuf[0]);
    }
	else
	{
#ifdef ANXDEBUG
        dprint("Tx Buf Full\n");
#endif
        return CMD_FAIL;
    }

    return CMD_SUCCESS;
}

void dp_alt_enter_mode()
{
    Send_PD_Data_Messages(TYPE_DP_ALT_ENTER, enter_exit_svid, 4);
}

void dp_alt_exit_mode()
{
    Send_PD_Data_Messages(TYPE_DP_ALT_EXIT, enter_exit_svid, 4);
}


static u8 interface_recvd_msg()
{
    anx_read_block_reg(context[port_id].spi, InterfaceRecvBuf_Addr, 32, (u8 *)context[port_id].InterfaceRecvBuf);

    if (context[port_id].InterfaceRecvBuf[0]!=0) {

        anx_write_reg(context[port_id].spi,InterfaceRecvBuf_Addr,0);
        j = 0;
        for(i = 0; i < recvd_msg_len() + 2 + 1; i++) {
            j += context[port_id].InterfaceRecvBuf[i];
        }
        if(j == 0) {
            return CMD_SUCCESS;
        } 
		else 
		{
#ifdef ANXDEBUG
            dprint("checksum error: \n");
#endif
        }
        dprint("\n if (context[port_id].InterfaceRecvBuf[0]!=0) {\n");

    }
    return CMD_FAIL;
}

static void handle_intr_vector()
{
    if (interface_recvd_msg() == CMD_SUCCESS)
    {
        dprint("rev <- %s\n", interface_to_str((PD_MSG_TYPE)context[port_id].InterfaceRecvBuf[1]));
        dispatch_rcvd_pd_msg();
    }
}

#ifdef PATCH_NOT_CALIBRATED_CHIP
#define ANALOG_CTRL_6                           0xA6
#define ADC_CTRL_2                              0xC0
#define EFUSE_CTRL_10                           0x7C
#define EFUSE_CTRL_11                           0x7D
#define EFUSE_PROGRAM_DONE_CLR _BIT7
#define EFUSE_READ_DONE_CLR _BIT6
#define EFUSE_READ_DONE _BIT4
#define EFUSE_READ_DATA_0                       0x8D
void eFuseRead(BYTE  offset, BYTE  len, BYTEP buf)
{
	anx_read_block_reg(context[port_id].spi,EFUSE_READ_DATA_0+offset,len,buf);
}
BYTE  isCalibrated()
{
  BYTE  buf[3];
  eFuseRead(0,3,buf);
  if(!buf[0] || !buf[1] || !buf[2])
  	return 0;
  return 1;
}
#endif
static u8 wait_ocm_ready(u8 fource_use_pd_20)
{
    count = 3200;

    do {
        count--;
    } while(!(anx_read_reg(context[port_id].spi, R_RAM_CTRL)&BOOT_LOAD_DONE));

#ifdef PATCH_NOT_CALIBRATED_CHIP
	c = anx_read_reg(context[port_id].spi, 0x95);
   if(!isCalibrated())
   {
   #ifdef ANXDEBUG
   	dprint("Not calibrate chip.\n");
   #endif
	 anx_write_reg(context[port_id].addr, RING_OSC_CTRL, 0xb7); //27M clock
       if (c >= 0xAC && c != 0xFF) {
   	 anx_write_reg(context[port_id].addr, ADC_CTRL_2, 0x26);  ////adc                AA: 0x0f    AC:0x26
       }else{
    anx_write_reg(context[port_id].addr, ADC_CTRL_2, 0x0f);  //adc
	}
    anx_write_reg(context[port_id].addr, ANALOG_CTRL_6, 0x11);  //5.1K
   }
#endif
    // configure OCM init use PD2.0 
    if(fource_use_pd_20)
        anx_write_reg(context[port_id].spi, PD_REV_INIT, 0x01);
    else
        anx_write_reg(context[port_id].spi, PD_REV_INIT, 0x00);

    do{
        if(!count)
        {
#ifdef ANXDEBUG
            dprint("ocm not ready!\n");
#endif
            break;
        }
        count--;
        PDk = anx_read_reg(context[port_id].spi, OCM_VERSION_REG);
    } while((PDk == 0));

#ifdef ANXDEBUG
    dprint("ocm ready(%d)!\n",(3200-count));
#endif
   
    return((PDk & BOOT_LOAD_DONE) ? 1:0);
}
BYTE  pdss_src_pdo[4] = {
    /*5V 1.5A dual-role data-swap usb-comm */
    0x96,0x90,0x01,0x26
};

void send_initialized_setting()
{
	//dprint("pd init and step = %d\n", context[port_id].anx_initial_status);
    switch (context[port_id].anx_initial_status) {
		case INIT_STAGE_1:
			context[port_id].anx_initial_status++;
			break;
	   case INIT_STAGE_2:
			context[port_id].anx_initial_status++;
			break;
		case INIT_STAGE_3:

			/* send TYPE_DP_SNK_IDENTITY init setting */
			send_pd_msg(TYPE_DP_SNK_IDENTITY, snk_identity,sizeof(snk_identity), SOP_TYPE);
			context[port_id].anx_initial_status++;
			break;
		case INIT_STAGE_4:
			/* send TYPE_SVID init setting */
			context[port_id].anx_initial_status++;
			break;
		case INIT_STAGE_5:
			context[port_id].anx_initial_status++;
			break;
		case 55:
			context[port_id].anx_initial_status++;
			#if 1
			if ((context[port_id].data_operation_mode_uom == 0)||(context[port_id].data_operation_mode_uom & OP_DRP))
				break;
			switch (get_data_role()) 
			{
				case 1: //DFP
					role = OP_DFP;
					break;
				case 0: //UFP
					role = OP_UFP;
					break;
			}
			if (context[port_id].data_operation_mode_uom & role)
				break;
			send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
			#endif
			break;
		case 56:
			context[port_id].anx_initial_status ++;
			if ((context[port_id].power_operation_mode_pdm == 0) || (context[port_id].power_operation_mode_pdm & PW_DRP))
			{
				break;
			}
			switch (get_power_role()) 
			{
				case 1: //PW_PROVIDER
					role = PW_PROVIDER;
					break;
				case 0: //PW_CONSUMER
					role = PW_CONSUMER;
					break;
			}
			if (context[port_id].power_operation_mode_pdm & role)
				break;
			send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
			break;
		case 80:
			context[port_id].anx_initial_status = 0;
			context[port_id].enable_report_power_on = 1;
			break;
		default:
			context[port_id].anx_initial_status++;
			break;
		case 0:
			break;
    }

}

static void drp_toggle_enable() 
{     
     
	c = anx_read_reg(context[port_id].spi, 0x95);
    if (context[port_id].data_operation_mode_uom & OP_DFP) 
	{
        anx_write_reg(context[port_id].addr, TCPC_ROLE_CONTROL, 0x5); //CC with  Rp + 1.5A
        dprint("set tcpc role as Rp(0x5), reg 0xaa(%d)\n", c);
    } 
	else if (context[port_id].data_operation_mode_uom & OP_UFP) 
	{
        anx_write_reg(context[port_id].addr, TCPC_ROLE_CONTROL, 0xA); //CC with  RD
        dprint("-------  set tcpc role as Rd(0xa), reg 0xaa(%d)\n", c);
    } 
	else 
	{
		anx_write_reg(context[port_id].addr, ANALOG_CTRL_10, 0x80);
		anx_write_reg(context[port_id].addr, TCPC_ROLE_CONTROL, 0x4A); //CC with  RD
        dprint("-------  set tcpc role as Rd + DRP en(0x4A), %d chip\n", c);
		if (c >= 0xAC && c != 0xFF) 
		{     
    		anx_write_reg(context[port_id].addr,TCPC_COMMAND , 0x99);  //DRP en
      	}
		else
		{
			anx_write_reg(context[port_id].addr, ANALOG_CTRL_1, (anx_read_reg(context[port_id].addr, ANALOG_CTRL_1) | TOGGLE_CTRL_MODE | R_TOGGLE_ENABLE)); 
			anx_write_reg(context[port_id].addr, ANALOG_CTRL_1,(anx_read_reg(context[port_id].addr, ANALOG_CTRL_1) | R_LATCH_TOGGLE_ENABLE)); 
   	    }
   	}
}

static void usb_pd_function_init()
{
    wait_ocm_ready(0);  //wait ocm load done

    anx_write_reg(context[port_id].spi, MAX_VOLTAGE, 0xC8); /*20V, 100mv per unit*/     
    anx_write_reg(context[port_id].spi, MAX_POWER, 0x82); /*65W, 500mW per unit*/
        
    /*Minimum Power in 500mW units*/
    anx_write_reg(context[port_id].spi, MIN_POWER, 0x02);  /*1W*/
    if (context[0].anx_power_status == 0 && context[1].anx_power_status == 0)
        random_data = (timer1msCount % 256);
    anx_write_reg(context[port_id].addr, 0xef, random_data);
    PDk = 0;//anx_read_reg(context[port_id].addr, 0xee);
    //bit 0: enable unstructure VDM(fix self charging), bit 1: dead battery mode, OCM maximum delay 200ms
    PDk = PDk | 0x3;
	//bit 3 & 2 -> 00:vsafe 0v, 01:vsafe 1v, 10:vsafe 2v, 11:vsafe 3v.
	PDk = PDk | (0x1 << 2);
    if (anx_read_reg(context[port_id].spi, OCM_VERSION_REG) < 0x11)
        anx_write_reg(context[port_id].addr, 0xee, PDk);
    else
        anx_write_reg(context[port_id].addr, 0xf0, PDk);
     //pd function selection
    PDk = anx_read_reg(context[port_id].spi, FIRMWARE_CTRL);
    //Default enable auto pd mode
    #ifdef AUTO_RDO_ENABLE
    PDk = PDk | auto_pd_en;
    #endif
    // trySrc_trySnk_setting
    PDk = PDk & (~(trysrc_en | trysnk_en));

    if((!context[port_id].trysnk_enable)&&(context[port_id].trysrc_enable)) 
	{
        //k = k | trysrc_en;
        dprint("Prefer Try.Src\n");
    } 
	else if((!context[port_id].trysrc_enable) &&(context[port_id].trysnk_enable)) 
	{
        //k = k | trysnk_en;
        dprint("Prefer Try.Snk\n");
    } 

    anx_write_reg(context[port_id].spi, FIRMWARE_CTRL, PDk);
    //Disable OVP
    anx_write_reg(context[port_id].addr, 0xdd, 0xff);
    anx_write_reg(context[port_id].addr, 0xde, 0x03);
    //interrupt mask initial
    ANXmdelay(2);
    anx_write_reg(context[port_id].spi, INT_MASK,(anx_read_reg(context[port_id].spi, INT_MASK) & (~CC_STATUS_CHANGE)));

    // config OCM reply message
    anx_write_reg(context[port_id].spi, PD_EXT_MSG_CTRL, 0xFF);

}
void chip_power_on(u8 port)
{
    if (port != 0)
        return;

    anx_read_reg(0x2C,TCPC_COMMAND);
}
void chip_power_down(u8 port) 
{
	dprint("chip power down \n");
    anx_write_reg(context[port].spi, OCM_CTRL_0,(anx_read_reg(context[port].spi, OCM_CTRL_0) | OCM_RESET));
    drp_toggle_enable() ;
    anx_write_reg(context[port].addr, TCPC_COMMAND , 0xff);  //lpower down
}
void chip_reset(u8 port) 
{
	dprint("PD chip reset \n");
    anx_write_reg(context[port].spi, OCM_CTRL_0,(anx_read_reg(context[port].spi, OCM_CTRL_0) | OCM_RESET));
    drp_toggle_enable() ;
    anx_write_reg(context[port].addr, TCPC_COMMAND , 0xff);  //lpower down
}

#ifdef ECR_CHECKING
static void anx_ecr_checking(void)
{
    if (((context[port_id].ucsi_partner_rdo >> 10) & 0x3ff) == (PD_CURRENT_1500MA / 10)) {
        supply_1500ma_flag = 0;
        context[port_id].ucsi_partner_rdo = 0;
    }
}
#endif

static void anx_detect_voltage_and_current(u8 port){
	change_status = anx_read_reg(context[port].spi, SYSTEM_STSTUS);
    if (change_status & _BIT4) {
        self_charging_error = 1;
        return;
    }

	if ((change_status & VBUS_CHANGE) == 0) {
		context[port].anx_power_role = 0;
        context[port].BatteryCharging = BCS_NOMINAL_CHARGING;
		context[port].anx_current_in_50ma = anx_read_reg(context[port].spi, 0xb1);
		context[port].anx_voltage_in_100mv = anx_read_reg(context[port].spi, 0xaf);
		if ((context[port].anx_current_in_50ma == 0) || (context[port].anx_voltage_in_100mv == 0)) {
			change_status = anx_read_reg(context[port].spi, NEW_CC_STATUS);
			pd_cc_status_default_func(change_status);
			context[port].anx_voltage_in_100mv = 50;
			switch (context[port].power_op_mode) {
			default:
			case PO_USB_DEFAULT:
				context[port].anx_current_in_50ma = 10;
				break;
			case PO_TYPE_C_1_5A:
				context[port].anx_current_in_50ma = 30;
				break;
			case PO_TYPE_C_3A:
				context[port].anx_current_in_50ma = 60;
				break;
			}
		}
	} else {
        context[port].BatteryCharging = BCS_NOT_CHARGING;
		context[port].anx_power_role = 1;
	}
}
void anx_dead_battery_checking_2(u8 port)
{
    if (context[port].anx_power_status == 0) {
		change_status = anx_read_reg(context[port].spi, NEW_CC_STATUS);
		if (change_status == 0)
			return; //return if no cc connection
			context[port].anx_power_status = 1;
		}
	anx_detect_voltage_and_current(port);
}

void anx_dead_battery_checking_1()
{
    for (i = 0; i < PD_MAX_INSTANCE; i++) {
        if (context[i].anx_power_status == 0) {
            change_status = anx_read_reg(context[i].spi, NEW_CC_STATUS);
            if (change_status != 0) {
                context[i].anx_power_status = 1;
                pd_cc_status_default_func(change_status);
                change_status = anx_read_reg(context[i].spi, SYSTEM_STSTUS);
                if (change_status & VBUS_CHANGE)
                    context[i].anx_power_role = 1;
                else
                    //debug only
                    context[i].anx_power_role = 0;

                if (change_status & _BIT4) {
                    self_charging_error = 1;
                    return;
                }

                if (context[i].anx_power_role == 0) {
                    context[i].anx_current_in_50ma = anx_read_reg(context[port_id].spi, 0xb1);
                    context[i].anx_voltage_in_100mv = anx_read_reg(context[port_id].spi, 0xaf);
                    if (context[i].anx_current_in_50ma == 0 || context[i].anx_voltage_in_100mv == 0) 
					{
                        context[i].anx_voltage_in_100mv = 50;
                        switch (context[port_id].power_op_mode) {
                        default:
                        case PO_USB_DEFAULT:
                            context[i].anx_current_in_50ma = 10;
                            break;
                        case PO_TYPE_C_1_5A:
                            context[i].anx_current_in_50ma = 30;
                            break;
                        case PO_TYPE_C_3A:
                            context[i].anx_current_in_50ma = 60;
                            break;
                        }
                    }
                }
            }
        }
    }
}
void anx_alert_message_isr()
{
	if ((0 == context[port_id].anx_power_status) &&(anx_read_reg(context[port_id].spi, OCM_VERSION_REG) == 0x00)) 
	{
		anx_write_reg(context[port_id].addr, 0x10, 0xFF);
		anx_write_reg(context[port_id].addr, 0x11, 0xFF);
		return;
	}
    CC_status[port_id] = anx_read_reg(context[port_id].addr, CC_STATUS);	//WN3_007A++    
    change_int = anx_read_reg(context[port_id].spi, CHANGE_INT);
    change_status = anx_read_reg(context[port_id].spi, SYSTEM_STSTUS);
    intr_alert_0 = anx_read_reg(context[port_id].addr, INTR_ALERT_0);
    intr_alert_1 = anx_read_reg(context[port_id].addr, INTR_ALERT_1);

    //clear
    anx_write_reg(context[port_id].spi, CHANGE_INT, 0x00);
    anx_write_reg(context[port_id].addr, INTR_ALERT_0, intr_alert_0);
    anx_write_reg(context[port_id].addr, INTR_ALERT_1, intr_alert_1);

    /* power on process */
    if(intr_alert_1 & INTR_INTP_POW_ON) {
        usb_pd_function_init();
        j = anx_read_reg(context[port_id].spi, OCM_VERSION_REG);
        PDk = anx_read_reg(context[port_id].spi, OCM_VERSION_REG + 1);
        if (port_id == 0) 
		{
            xEC_PDPort0MainVersion = j;
            xEC_PDPort0SubVersion = PDk;
        } 
		else 
		{
            xEC_PDPort1MainVersion = j;
            xEC_PDPort1SubVersion = PDk;
        }

        anx_initial_context(port_id);
        context[port_id].anx_power_status = 1;
        context[port_id].csc.csc.Connect = 1;
        context[port_id].ConnectorPartner = CP_USB;
        context[port_id].csc.csc.ConnectorPartner = 1;
        context[port_id].csc.csc.BatteryChargingStatus = 1;
        context[port_id].anx_initial_status = INIT_STAGE_1;

        //Fix JIRA LBT-215
		send_pd_msg(TYPE_SVID, svid, sizeof(svid), SOP_TYPE);
		send_pd_msg(TYPE_SET_VAR,set_variable_1,5,0);
        //Fix JIRA LBT-411
		send_pd_msg(TYPE_SET_SNK_DP_CAP, dp_source_dp_caps, 4, SOP_TYPE);
        //Fix JIRA LBT-414
		send_source_capability();
		send_sink_capability();
        //send_initialized_setting();
	    ucsi_async_notify_raise_up(150);
    }
    
	if (intr_alert_1 & INTR_INTP_POW_OFF) 
	{
        chip_power_down(port_id);

        context[port_id].anx_power_status = 0;
        anx_initial_context(port_id);
        context[port_id].csc.csc.Connect = 1;
        ucsi_async_notify_raise_up(2);
        //anx_vbus_ctrl(context[port_id].vbus_gpio, 0);
        #ifdef ECR_CHECKING
        anx_ecr_checking();
        #endif
        context[port_id].anx_vbus_status = 0;
		dprint("after if (intr_alert_1 & INTR_INTP_POW_OFF)\n");
        return;
    }


    /*Received software interrupt*/
    if (intr_alert_0 & INTR_SOFTWARE_INT) {
    
        if (change_int & VBUS_CHANGE) {
            context[port_id].csc.csc.PowerDirection = 1;
            if (change_status & VBUS_CHANGE)
            {
                context[port_id].anx_vbus_status = 1;
                context[port_id].anx_power_role = 1;
                context[port_id].csc.csc.BatteryChargingStatus = 1;
                context[port_id].BatteryCharging = BCS_NOT_CHARGING;
                dprint("VBUS 5V OUT.\n");
            }
            else
            {
                //DISABLE_5V_VBUS_OUT();
                dprint("VBUS change to input.\n");
                #ifdef SUP_VBUS_CTL
                //anx_vbus_ctrl(context[port_id].vbus_gpio, 0);
                #endif
                context[port_id].anx_power_role = 0;
                context[port_id].anx_vbus_status = 0;
                context[port_id].csc.csc.ExternalSupply = 1;
                context[port_id].csc.csc.BatteryChargingStatus = 1;
                context[port_id].BatteryCharging = BCS_NOMINAL_CHARGING;
            }
            ucsi_async_notify_raise_up(100);
        }
        if (change_int & CC_STATUS_CHANGE) 
		{
            change_status = anx_read_reg(context[port_id].spi, NEW_CC_STATUS);
            pd_cc_status_default_func(change_status);
            #ifdef ANXREDRIVER
            if (context[port_id].pd_capability & PD_HAS_REDRIVER) 
			{
                if(plug_orientation(port_id))
                    context[port_id].cc_orientation = CC1_CONNECTED;
                else
                    context[port_id].cc_orientation = CC2_CONNECTED;
                if(context[port_id].mux_mode == NO_CONNECT) 
				{ //usb3.0 only mode at default
                    redriver_mux_control(USB3_1_CONNECTED, context[port_id].cc_orientation);
                    context[port_id].mux_mode = USB3_1_CONNECTED;
                }
            }
            #endif
        }
    }
    
    /*Received interface message*/
    if (intr_alert_0 & INTR_RECEIVED_MSG) {
        handle_intr_vector();
    }
    anx_dead_battery_checking_2(port_id);
}

static void anx7447_context_initial()
{
    //Addr select as : 0 0
    context[port_id].PD_standalone_slave_id1 = Liberty_i2c_address[port_id][0];
    context[port_id].PD_SPI_slave_id = Liberty_i2c_address[port_id][1];
    context[port_id].PD_EMTB_slave_id = Liberty_i2c_address[port_id][2];
    context[port_id].PD_EMRB_slave_id = Liberty_i2c_address[port_id][3];

    #ifdef ANX_OHO_DFP_ONLY
    context[port_id].dfp_only = 1;
    #else
    context[port_id].dfp_only = 0;
    #endif
    context[port_id].pd_src_pdo_cnt = 2;
    context[port_id].anx_power_status = 0;
    context[port_id].sel_voltage_pdo_index = 0x2;

    context[port_id].pd_capability = PD_HAS_RETURN_STATUS | PD_HAS_SVID_MID | PD_HAS_CABLE_VDO;
    #ifdef ANXREDRIVER
    if (port_id == 1) {
        context[port_id].pd_capability |= PD_HAS_REDRIVER;
    }
    #endif

    context[port_id].error_count = 0;
}

static void anx7447_pup_initial()
{
    pup.alt_mode[0].SVID0 = 0xFF01; 
    pup.alt_mode[0].MID0 = 0x00000405; 
    pup.alt_mode_support_num = 1;
    pup.alt_mode_support_bitmap = 0;
    pup.current_alt_mode = 0;
}

/*
 * @desc   : ANX7447 ucsi module initialize all data structure.
 * @param  : none
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
void anx7447_ucsi_init(void)
{
    i = 0;
    //dprint("anx7447_ucsi_init\n");
    anx7447_pup_initial();
    for (i = 0; i < PD_MAX_INSTANCE; i++) {
        port_id = i;

        anx7447_context_initial();
        context[i].InterfaceRecvBuf = InterfaceRecvBuf[i];
        context[i].InterfaceSendBuf = InterfaceSendBuf[i];
        context[i].connector_index = i+1;
    }

}

void PD_debug(void)//lyx PD debug add
{
    BYTE data=0;
	
	data=anx_read_reg(0x3F,0xAC);
    dprint("MAX_VOLTAGE = 0x%x\n",data);
	
	data=anx_read_reg(0x3F,0xAD);
    dprint("MAX_POWER = 0x%x\n",data);
	
	data=anx_read_reg(0x3F,0xAE);
    dprint("MIN_POWER = 0x%x\n",data);
	
	data=anx_read_reg(0x3F,0xB1);
    dprint("Current = 0x%x\n",data);
	
	data=anx_read_reg(0x3F,0xAF);
    dprint("Voltage = 0x%x\n",data);
}
