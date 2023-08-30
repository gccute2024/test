/*
 * @Author: Iversu
 * @LastEditors: Iversu
 * @LastEditTime: 2023-04-02 21:24:28
 * @Description: PD public layer.
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

struct cable_property_data *cpd;
struct notification_enable_status ne;
struct bios_share_msg *msg;
struct cci_status *cci;
struct control_header *ch;
struct set_notification_enable *sne;
struct connector_reset *cr;
struct ack_cc_ci *acc;
struct capability_data *cd;
struct connector_capability_data *ccd;
struct get_connector_capability *gcc;
struct set_uor *suor;
struct set_uom *suom;
struct set_pdr *spdr;
struct set_pdm *spdm;
struct get_connector_status *gcs;
struct get_cable_property *gcp;
struct get_pdos *gp;
struct set_new_cam *snc;
struct get_alternate_modes *gam;
struct connector_status_data *csd;
struct alt_modes_data *amd;
struct pdos_data *pd;
struct ppm_ucsi_parameters pup;
struct bios_share_msg ppm2bios;
anx_context context[4];

BYTE  InterfaceSendBuf[PD_MAX_INSTANCE][INTERFACE_SEND_BUF_SIZE];
BYTE  InterfaceRecvBuf[PD_MAX_INSTANCE][INTERFACE_RECV_BUF_SIZE];

u16 count;
XBYTE port_index;
XBYTE len;
XBYTE port_id;
XBYTE random_data,timer1msCount;
XBYTE DataLength;
XBYTE i,j;
XBYTE xUCSI_DS_VERSION[2];
XBYTE xUCSI_DS_RESERVED[2];
XBYTE xUCSI_DS_MGI[16];
XBYTE xUCSI_DS_MGO[16];

void PD_TARSetup(BYTE i2c_addr)
{
	//setup Charger TAR
	I2c_Master_Write_Byte(0,0);//must be disable before setup
	I2c_Master_Write_Byte(i2c_addr,0);//Set TAR
	I2c_Master_Write_Byte(1,0);//enable
}

BYTE i2c_read_reg(BYTE i2c_addr, BYTE reg)
{
	PD_TARSetup(i2c_addr);
    return(I2c_Master_Read_Byte(0));
}


void i2c_write_reg(BYTE i2c_addr, BYTE reg, BYTE data1)
{
	PD_TARSetup(i2c_addr);
    I2c_Master_Write_Byte(data1, 0x3800);
}


void i2c_write_block_reg_b(BYTE i2c_addr, BYTE reg, BYTE length, BYTE *buf)
{
	I2c_Master_Write_Block(buf,length,0);
}

void cancel_all_timer()
{
    ucsi_async_checking_timer = 0;
    ucsi_async_restore = 0;
}

/*-----------------------------------------------------------------------------
 * @subroutine - i2c_read_block_reg
 * @function - i2c_read_block_reg
 * @Upstream - By call
 * @input    - None
 * @return   - None
 * @note     - None
 *---------------------------------------------------------------------------*/
void i2c_read_block_reg(BYTE i2c_addr, BYTE reg, BYTE length, BYTE *buf)
{
	I2c_Master_Read_Block(buf,length,0);
}

/*
 * @desc   : ucsi detect connector change
 * @param  :
 *           cci -> cci status structure
 * @return : void
 */
void ucsi_ppm_connector_change_detect()
{
    /*
     * Wait for OPM ACK back.
     */

    /*
     * Connector change notification is disabled
     */
    if (ne.ConnectChange == 0)
        return;

    for (i = 0; i < PD_MAX_INSTANCE; i++)
        if ((context[i].csc.connector_change_indicator) != 0) {
            cci->ConnectorChangeIndicator = i + 1;
            break;
        }
}

/*
 * @desc   : SET_NOTIFICATION_ENABLE ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void set_notification_enable_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC)
        cci->ErrorIndicator = 0;
    else
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : CANCEL ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static u8 cancel_ack()
{
    S_CLEAR(cci, struct cci_status);
    
    ucsi_ppm_connector_change_detect();

    cci->CommandCompletedIndicator = 1;

    return UCSI_COMMAND_SUCC;
}

/*
 * @desc   : COMMAND BUSY ACK setting
 * @param  :
 *           cci -> cci status structure
 * @return : void
 */
static void busy_ack()
{
    S_CLEAR(cci, struct cci_status);
    cci->BusyIndicator = 1;
    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : COMMAND ERROR ACK setting
 * @param  :
 *           cci -> cci status structure
 * @return : void
 */
static void error_ack()
{
    S_CLEAR(cci, struct cci_status);
    cci->ErrorIndicator = 1;
    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : GET_CONNECTOR_RESET ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void connector_reset_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result != UCSI_COMMAND_SUCC)
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : ACK_CC_CI ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void ack_cc_ci_ack()
{
    S_CLEAR(cci, struct cci_status);

    cci->AcknowledgeCommandIndicator = 1;
}

/*
 * @desc   : PPM_RESET ERROR ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void reset_ack()
{
    S_CLEAR(cci, struct cci_status);

    if (result == UCSI_COMMAND_SUCC)
        cci->ResetCompletedIndicator = 1;
}

/*
 * @desc   : GET_CAPABILITY ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void get_capability_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC)
        cci->DataLength = 0x10;
    else
        cci->ErrorIndicator = 0x1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : GET_CONNECTOR_CAPABILITY ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
*/
static void get_connector_capability_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC)
        cci->DataLength = 2;
    else
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : SET_UOR ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void set_uor_ack()
{
    S_CLEAR(cci, struct cci_status);

    //ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_FAIL)
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : SET_PDR ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void set_pdr_ack()
{
    S_CLEAR(cci, struct cci_status);

    if (result == UCSI_COMMAND_FAIL)
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : GET_CONNECTOR_STATUS ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void get_connector_status_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC)
        cci->DataLength = 0x09;
    else
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : GET_ERROR ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
static void get_error_status_ack()
{
    S_CLEAR(cci, struct cci_status);

    //ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC)
        //Currently, we do not support Vendor Defined Error Code
        cci->DataLength = 0x10;
    else
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}

/*
 * @desc   : GET_CABLE_PROPERTY ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
#if UCSI_CABLE_SUPPORT
static void get_cable_property_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC)
        //Currently, we do not support Vendor Defined Error Code
        cci->DataLength = 0x05;
    else
        cci->ErrorIndicator = 1;

    cci->CommandCompletedIndicator = 1;
}
#endif

/*
 * @desc   : GET_PDOS ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 *           DataLength -> message actually used byte
 * @return : void
 */
#if UCSI_PDOS_SUPPORT
static void get_pdos_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC) {
        cci->DataLength = DataLength;
    } else {
        cci->ErrorIndicator = 1;
    }

    cci->CommandCompletedIndicator = 1;
}
#endif

/*
 * @desc   : SET_NEW_CAM ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
#if UCSI_ALT_OR_SUPPORT
static void set_new_cam_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC) {
        cci->DataLength = 0;
    } else {
        cci->ErrorIndicator = 1;
    }

    cci->CommandCompletedIndicator = 1;
}
#endif

/*
 * @desc   : GET_CURRENT_CAM ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 * @return : void
 */
#if UCSI_ALT_SUPPORT
static void get_current_cam_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC) {
        cci->DataLength = 1;
    } else {
        cci->ErrorIndicator = 1;
    }

    cci->CommandCompletedIndicator = 1;
}
#endif

/*
 * @desc   : GET_CAM_SUPPORT ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 *           DataLength -> message actually used byte
 * @return : void
 */
#if UCSI_ALT_SUPPORT
static void get_cam_supported_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC) {
        cci->DataLength = DataLength;
    } else {
        cci->ErrorIndicator = 1;
    }

    cci->CommandCompletedIndicator = 1;
}
#endif

/*
 * @desc   : GET_ALT_MODES ACK setting
 * @param  :
 *           cci -> cci status structure
 *           result -> ACK success or fail
 *           DataLength -> message actually used byte
 * @return : void
 */
#if UCSI_ALT_SUPPORT
static void get_alt_modes_ack()
{
    S_CLEAR(cci, struct cci_status);

    ucsi_ppm_connector_change_detect();

    if (result == UCSI_COMMAND_SUCC) {
        cci->DataLength = DataLength;
    } else {
        cci->ErrorIndicator = 1;
    }

    cci->CommandCompletedIndicator = 1;
}
#endif

//-----------------------------------------------------------

/*
 * @desc   : store error info in error status structure -> esd
 * @param  :
 *           cci -> cci status structure
 *           errno -> error number
 * @return : void
 */
static void set_error_data()
{

    S_CLEAR(&esd, struct error_status_data);
    esd.ErrorInformation = ucsi_errno;
    
    error_ack();
}

/*
 * @desc   : process unrecognized command
 * @param  :
 *           cci -> cci status structure
 * @return : UCSI_COMMAND_SUCC
 */
static u8 unsupport_command_process()
{
    ucsi_errno = ERR_UNRECOGNIZED;
    set_error_data();
    cci->NotSupportedIndicator = 1;
    cci->CommandCompletedIndicator = 1;

    //Here we abused to use SUCCESS as return value
    return UCSI_COMMAND_SUCC;
}

/*
 * @desc   : OPM get alt modes from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
#if UCSI_ALT_SUPPORT
static u8 ppm_get_alt_modes()
{
    //u8 DataLength;
    //u8 i;
    //u8 Offset, Altnum;

    if (gam->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = gam->ConnectorNumber - 1;

    //Offset = gam->AlternateModeOffset;
    //Altnum = gam->NumofAlternateMode;
    
    if (gam->Recipient != R_CONNECTOR)
        DETECT_POWER_ON(port_index);
    if (gam->AlternateModeOffset > pup.alt_mode_support_num) {
        //ucsi_errno = ERR_INVALID;
        //set_error_data();
        DataLength = 0;
        goto out;;
    }

    if (gam->NumofAlternateMode > (pup.alt_mode_support_num - gam->AlternateModeOffset))
        gam->NumofAlternateMode = (pup.alt_mode_support_num - gam->AlternateModeOffset);

    /* Currently, we only support connector and SOP as recipient */
    switch (gam->Recipient) {
    case R_CONNECTOR:
        for (i = 0; i< gam->NumofAlternateMode; i++) {
            amd[i] = pup.alt_mode[gam->AlternateModeOffset +i];
            #ifdef EC_ANX_BIG_ENDIAN
            SWAP_WORD(&amd[i].SVID0);
            SWAP_DWORD(&amd[i].MID0);
            SWAP_WORD(&amd[i].SVID1);
            SWAP_DWORD(&amd[i].MID1);
            #endif
        }
        DataLength = gam->NumofAlternateMode * 6;
        break;
    case R_SOP1:
        DataLength = 0;
        switch (context[port_index].peer_svid_length) {
        case 4:
            pup.alt_mode_support_bitmap = 1;
            if (context[port_index].pd_capability & PD_HAS_SVID_MID) {
                amd[0].SVID0 = context[port_index].ac[0].svid;
                amd[0].MID0 = context[port_index].ac[0].mid;
                #ifdef EC_ANX_BIG_ENDIAN
                SWAP_WORD(&amd[0].SVID0);
                //SWAP_DWORD(&amd[0].MID0);
                #endif
            } else {
                amd[0].SVID0 = (context[port_index].peer_svid[3] << 8) | context[port_index].peer_svid[2];
                amd[0].MID0 = 0x405;
                #ifdef EC_ANX_BIG_ENDIAN
                SWAP_WORD(&amd[0].SVID0);
                SWAP_DWORD(&amd[0].MID0);
                #endif
            }
            DataLength = 6;
            break;
        case 8:
            pup.alt_mode_support_bitmap = 1;
            if (context[port_index].pd_capability & PD_HAS_SVID_MID) {
                amd[0].SVID0 = context[port_index].ac[0].svid;
                amd[0].MID0 = context[port_index].ac[0].mid;
                amd[0].SVID1 = context[port_index].ac[1].svid;
                amd[0].MID1 = context[port_index].ac[1].mid;
                #ifdef EC_ANX_BIG_ENDIAN
                SWAP_WORD(&amd[i].SVID0);
                //SWAP_DWORD(&amd[i].MID0);
                SWAP_WORD(&amd[i].SVID1);
                //SWAP_DWORD(&amd[i].MID1);
                #endif
            } else {
                amd[0].SVID0 = (context[port_index].peer_svid[3] << 8) | context[port_index].peer_svid[2];
                amd[0].MID0 = 0x405;
                amd[0].SVID1 = (context[port_index].peer_svid[1] << 8) | context[port_index].peer_svid[0];
                amd[0].MID1 = 0;
                #ifdef EC_ANX_BIG_ENDIAN
                SWAP_WORD(&amd[i].SVID0);
                SWAP_DWORD(&amd[i].MID0);
                SWAP_WORD(&amd[i].SVID1);
                SWAP_DWORD(&amd[i].MID1);
                #endif
            }
            DataLength = 12;
            break;
        case 0:
        default:
            DataLength = 0;
            break;
        }
        break;
    default:
        for (i = 0; i < gam->NumofAlternateMode; i++) {
            S_CLEAR(&amd[i], struct alt_modes_data);
        }
        DataLength = 0;
        break;
    }

out:
    result = UCSI_COMMAND_SUCC;
    get_alt_modes_ack();

    return result;
}
#endif

/*
 * @desc   : OPM get cam suported list from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
#if UCSI_ALT_SUPPORT
static u8 ppm_get_cam_supported()
{
    //u8 DataLength;

    if (gcs->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    if (context[gcs->ConnectorNumber].peer_svid_length == 0) {
        DataLength = 0;
        goto out;
    }

    *(char *)&msg->msgi = pup.alt_mode_support_bitmap;

    if (pup.alt_mode_support_bitmap)
        DataLength = (pup.alt_mode_support_num % 8) + 1;
    else
        DataLength = 0;

out:
    result = UCSI_COMMAND_SUCC;
    get_cam_supported_ack();

    return result;
}
#endif

/*
 * @desc   : OPM get current cam from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
#if UCSI_ALT_SUPPORT
static u8 ppm_get_current_cam()
{
    if (gcc->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    if (context[gcc->ConnectorNumber].peer_svid_length == 0) {
        *(char *)&msg->msgi = 0;
        goto out;
    }

    *(char *)&msg->msgi = 1;
out:
    result = UCSI_COMMAND_SUCC;
    get_current_cam_ack();

    return result;
}
#endif

/*
 * @desc   : OPM set new cam for PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
#if UCSI_ALT_OR_SUPPORT
static u8 ppm_set_new_cam()
{
    if (snc->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    if (context[gcc->ConnectorNumber].peer_svid_length == 0) {
        //*(char *)&msg->msgi = 0;
        goto out;
    }

    if (snc->EnterOrExit == 1) {
        dp_alt_enter_mode();
    } else {
        dp_alt_exit_mode();
    }

    //ucsi_errno = ERR_CMD_UNSUCCESS;
out:
    result = UCSI_COMMAND_SUCC;
    set_new_cam_ack();

    return result;
}
#endif

/*
 * @desc   : OPM get pdos from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
#if UCSI_PDOS_SUPPORT
static u8 ppm_get_pdos()
{
    //u8 DataLength;
    //u8 pdo_index;

    if (gp->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    result = UCSI_COMMAND_FAIL;
    port_index = gp->ConnectorNumber - 1;
    DETECT_POWER_ON(port_index);

#ifdef ANXDEBUG
    dprint("Get %s : %s PDOOffset (%.bx), numOfPDO(%.bx), partner_pdo_length is %.bx\n",
        (gp->PartnerPDO == 0)?"connector":"partner", (gp->SourceOrSinkPDOs)?"source":"sink",
        gp->PDOOffset, gp->NumberOfPDOs,
        context[port_index].partner_pdo_length);
#endif
    if (gp->PartnerPDO == 0) {
        if (gp->SourceOrSinkPDOs == 1) {
            switch (gp->SourceCapabilities) {
            case SCTCURRENT_SUPPORTED_SOURCE_CAPABILITIES:
            case SCTMAXIMUM_SUPPORTED_SOURCE_CAPABILITIES:
            case SCTADVERTISED_CAPABILITIES:
                if (gp->PDOOffset <= 1) {
                    memcpy(&pd->PDO[0], &context[port_index].pd_src_pdo[0], 4);
                    DataLength = 4;
                } else {
                    pd->PDO[0] = 0;
                    pd->PDO[1] = 0;
                    pd->PDO[2] = 0;
                    pd->PDO[3] = 0;
                    DataLength = 0;
                }
                break;
            default:
                ucsi_errno = ERR_INVALID;
                set_error_data();
                break;
            }
        } else {
            if (gp->PDOOffset <= 1) {
                memcpy(&pd->PDO[0], &context[port_id].pd_snk_pdo, 4);
                DataLength = 4;
            } else {
                pd->PDO[0] = 0;
                pd->PDO[1] = 0;
                pd->PDO[2] = 0;
                pd->PDO[3] = 0;
                DataLength = 0;
            }
        }
    } else {
        if (gp->PDOOffset * 4 < context[port_index].partner_pdo_length) {
            memcpy((void *)&pd->PDO, &context[port_index].partner_pdo[0],
                min((gp->NumberOfPDOs+1)*4,context[port_index].partner_pdo_length));
            DataLength = min((gp->NumberOfPDOs+1)*4,context[port_index].partner_pdo_length);
            
            #ifdef ANXDEBUG
            dprint("+++++++++++++++  executed to here, DataLength(%.bx).\n", DataLength);
            #endif
        } else {
            pd->PDO[0] = 0;
            pd->PDO[1] = 0;
            pd->PDO[2] = 0;
            pd->PDO[3] = 0;
            DataLength = 0;
        }
    }

    result = UCSI_COMMAND_SUCC;
    #ifdef ANXDEBUG
    dprint("***********   get PDO from OS : ");
    for (i = 0; i < DataLength; i++) {
        dprint("%.bx ", ((u8 *)&pd->PDO[0])[i]);
    }
    dprint("\n");
    #endif
out:
    get_pdos_ack();

    return result;
}
#endif

/*
 * @desc   : OPM get cable property from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
#if UCSI_CABLE_SUPPORT
static u8 ppm_get_cable_property()
{
    if (gcp->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = gcp->ConnectorNumber - 1;
    S_CLEAR(cpd, struct cable_property_data);

    DETECT_POWER_ON(port_index);
    
    if ((context[port_index].pd_capability & PD_HAS_CABLE_VDO) &&
        (*(u32 *)&context[port_index].cv != 0)) {
        cpd->VBUSInCable = context[port_index].cv.vbus_thru_cable;
            switch (context[port_index].cv.vbus_current) {
            case VBUS_3A:
                cpd->bCurrentCapability = 60;
                break;
            case VBUS_5A:
                cpd->bCurrentCapability = 100;
                break;
            default:
                cpd->bCurrentCapability = 0;
                break;
            }
    
            cpd->CableType = 0; //Passive cable
            cpd->ModeSupport = 0;
            cpd->PlugEndType = context[port_index].cv.cable_type;
            if (context[port_index].cv.SSTX1_sup || context[port_index].cv.SSTX2_sup ||
                context[port_index].cv.SSRX1_sup || context[port_index].cv.SSRX2_sup)
                cpd->Directionality = 1;
            else
                cpd->Directionality = 0;
    
            //need more investigation
            cpd->bmSpeedSupported = 0;
            switch (context[port_index].cv.supper_speed_signal) {
            case SUPPER_SPEED_USB_2_0:
                cpd->bmSpeedSupported = (480 << 2) | 2; //480Mbps
                break;
            case SUPPER_SPEED_USB_3_1:
                cpd->bmSpeedSupported = (5 << 2) | 3; //5Gbps
                break;
            case SUPPER_SPEED_USB_3_1_2:
                cpd->bmSpeedSupported = (10 << 2) | 3; //10Gbps
                break;
            default:
                cpd->bmSpeedSupported = (12 << 2) | 2; //480Mbps
                break;
            }
            cpd->Latency = context[port_index].cv.latency;
    
            result = UCSI_COMMAND_SUCC;
            goto out;
        }
    
        //Need firmware support
        switch (context[port_index].ct) {
        case CABLE_TYPE_A:
            cpd->PlugEndType = PLUG_TYPEA;
            break;
        case CABLE_TYPE_B:
            cpd->PlugEndType = PLUG_TYPEB;
            break;
        case CABLE_TYPE_C:
            cpd->PlugEndType = PLUG_TYPEC;
            break;
        default:
            cpd->PlugEndType = PLUG_OTHER;
        }
    
        //Set default cable property
        cpd->bmSpeedSupported = (480 << 2) | 2;
        cpd->bCurrentCapability = 60; //3A
        cpd->VBUSInCable = 1;
        cpd->CableType = 0; //Passive cable
        cpd->Directionality = 0;
        cpd->ModeSupport = 0;
        cpd->Latency = 0x5; //40ns to 50ns ~5m
    
        result = UCSI_COMMAND_SUCC;
out:
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD(&cpd->bmSpeedSupported);
    #endif

    get_cable_property_ack();

    return result;
}
#endif

/*
 * @desc   : OPM get specific error code from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_get_error_status()
{
    if (*(u16 *)&(esd.ErrorInformation) != 0) {
        result = UCSI_COMMAND_SUCC;
        get_error_status_ack();
        *(struct error_status_data *)&msg->msgi = esd;
        #ifdef EC_ANX_BIG_ENDIAN
        SWAP_WORD(&msg->msgi);
        #endif
    } else {
        ucsi_errno = ERR_INVALID;
        result = UCSI_COMMAND_FAIL;
        get_error_status_ack();
    }

    return result;

}

/*
 * @desc   : OPM get get connector status from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_get_connector_status()
{
    #ifdef ANXDEBUG
    dprint("********   connector number is %.bx **** \n", gcs->ConnectorNumber);
    #endif
    if (gcs->ConnectorNumber > PD_MAX_INSTANCE) {
        //dprint("didn't find connector .\n");
        #ifdef HLK_RS2
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        result = UCSI_COMMAND_SUCC;
        get_connector_status_ack();
        context[0].csc.csc.Error = 1;
        ucsi_async_checking_timer = 250;
        return result;
        #else
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        result = UCSI_COMMAND_FAIL;
        get_connector_status_ack();
        return result;
        #endif
    }

    port_index = gcs->ConnectorNumber - 1;

    csd->ConnectStatus = context[port_index].anx_power_status & context[port_index].enable_report_power_on;
    if (csd->ConnectStatus) {
        csd->RequestDataObject = context[port_index].ucsi_partner_rdo;
        #ifdef EC_ANX_BIG_ENDIAN
        SWAP_DWORD(&csd->RequestDataObject);
        #endif
        if (csd->ConnectStatus == 1) {
            if (context[port_index].ucsi_partner_rdo != 0)
                csd->PowerOperationMode = PO_PD;
            else
                csd->PowerOperationMode = context[port_id].power_op_mode;
        }

        if (ne.ConnectorPartnerChange) {
            //Add more flag settings
            //csd->ConnectorPartner = CP_USB | CP_ALT_MODE;
            if (context[port_index].ConnectorPartner)
                csd->ConnectorPartner = context[port_index].ConnectorPartner;
            else
                csd->ConnectorPartner = 1;
            switch (context[port_index].ucsi_connector_partner_type) {
            case CPT_AUDIO_ACC:
            case CPT_CAB_DEBUG_ACC:
                break;
            default:
                role = get_data_role();
                if (role == 0) {//UFP(DFP Attached)
                    context[port_id].ucsi_connector_partner_type = CPT_DFP_ATTACHED;
                } else {//DFP(UFP Attached)
                    if (context[port_id].ucsi_connector_partner_type == CPT_DFP_ATTACHED)
                        context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;
                }
                break;
            }
            csd->ConnectorPartnerType = context[port_index].ucsi_connector_partner_type;
            #ifdef ANXDEBUG
            dprint("-------  connectorPartner flag %.bx, type %.bx, drole %.bx.\n",
                csd->ConnectorPartner, csd->ConnectorPartnerType, role);
            #endif
        }
        if (ne.PowerDirectionChange) {
            #ifdef ANXDEBUG
            dprint("======   port index(%.bx), anx_vbus_status is %.bx.\n",
				port_index, context[port_index].anx_vbus_status);
            #endif
            if (context[port_index].anx_power_role == 1)
                csd->PowerDirection = PD_PROVIDER;
            else
                csd->PowerDirection = PD_CONSUMER;
        }
        if (ne.BatteryChargingStatusChange) {
            csd->BatteryChargingStatus = context[port_index].BatteryCharging;
        }
    }
    if (ne.ConnectChange && context[port_index].csc.csc.Connect) {
        csd->csc.Connect = 1;
        context[port_index].csc.csc.Connect = 0;
        goto out;
    }
    if (csd->ConnectStatus == 0)
        goto out;

    if (ne.SupportedProviderCapabilitiesChange && context[port_index].csc.csc.SupportedProviderCap) {
        context[port_index].csc.csc.SupportedProviderCap = 0;
        csd->csc.SupportedProviderCap = 1;
        goto out;
    }
        #if UCSI_EXTERNAL_SUPPORT
        if (ne.ExternalSupplyChange && context[port_index].csc.csc.ExternalSupply) {
            context[port_index].csc.csc.ExternalSupply = 0;
            csd->csc.ExternalSupply = 1;
        goto out;
    }
    #endif
    if (ne.PowerDirectionChange && context[port_index].csc.csc.PowerDirection) {
        context[port_index].csc.csc.PowerDirection = 0;
        csd->csc.PowerDirection = 1;
        goto out;
    }
    if (ne.NegotiatedPowerLevelChange && context[port_index].csc.csc.NegotiatedPowerLevel) {
        context[port_index].csc.csc.NegotiatedPowerLevel = 0;
        csd->csc.NegotiatedPowerLevel = 1;
        goto out;
    }
    if (ne.ConnectorPartnerChange && context[port_index].csc.csc.ConnectorPartner) {
        context[port_index].csc.csc.ConnectorPartner = 0;
        csd->csc.ConnectorPartner = 1;
        goto out;
    }
    if (ne.SupportedCAMChange && context[port_index].csc.csc.SupportedCAM) {
        context[port_index].csc.csc.SupportedCAM = 0;
        csd->csc.SupportedCAM = 1;
            goto out;
        }
	if (ne.PowerOperationModeChange && context[port_index].csc.csc.PowerOperationMode) {
        context[port_index].csc.csc.PowerOperationMode = 0;
        csd->csc.PowerOperationMode = 1;
        goto out;
	}
        if (ne.PDResetComplete && context[port_index].csc.csc.PDResetComplete) {
            context[port_index].csc.csc.PDResetComplete = 0;
            csd->csc.PDResetComplete = 1;
            goto out;
        }

        if (ne.SupportedCAMChange && context[port_index].csc.csc.SupportedCAM) {
            context[port_index].csc.csc.SupportedCAM = 0;
            csd->csc.SupportedCAM = 1;
        goto out;
    }
    if (ne.SupportedProviderCapabilitiesChange && context[port_index].csc.csc.SupportedProviderCap) {
        context[port_index].csc.csc.SupportedProviderCap = 0;
        csd->csc.SupportedProviderCap = 1;
        goto out;
    }
    if (ne.Error && context[port_index].csc.csc.Error) {
        context[port_index].csc.csc.Error = 0;
        csd->csc.Error = 1;
        goto out;
    }
    if (ne.BatteryChargingStatusChange && context[port_index].csc.csc.BatteryChargingStatus) {
        context[port_index].csc.csc.BatteryChargingStatus = 0;
        csd->csc.BatteryChargingStatus = 1;
    }

out:
    result = UCSI_COMMAND_SUCC;
    get_connector_status_ack();
	if (context[port_index].csc.connector_change_indicator)
		ucsi_async_checking_timer = 50;

    return result;
}

#if UCSI_SET_PDM_SUPPORT
/*
 * @desc   : OPM set power direction mode for PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_set_pdm()
{
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD((XBYTE *)spdm + 2);
    #endif
    if (spdm->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = spdm->ConnectorNumber - 1;
    if (context[port_index].dfp_only && spdm->PowerDirectionMode != PW_PROVIDER) {
        ucsi_errno = ERR_INVALID;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }
    context[port_index].power_operation_mode_pdm = 0;

    result = UCSI_COMMAND_SUCC;
    //context[port_index].power_operation_mode_pdm = (((((XBYTE *)spdm)[3])&0x80) >> 6) | (((((XBYTE *)spdm)[2])&0x3) << 1);;
    if ((((XBYTE *)spdm)[3])&0x80)
        context[port_index].power_operation_mode_pdm |= PW_PROVIDER;
    if ((((XBYTE *)spdm)[2])&0x1)
        context[port_index].power_operation_mode_pdm |= PW_CONSUMER;
    if ((((XBYTE *)spdm)[2])&0x2)
        context[port_index].power_operation_mode_pdm |= PW_DRP;

    switch (context[port_index].power_operation_mode_pdm) {
    case 1: //Provider
        context[port_index].trysrc_enable = 1;
        break;
    case 2: //Consumer
        context[port_index].trysnk_enable = 1;
        break;
    case 4: //Provider/Consumer
        context[port_index].trysrc_enable = 0;
        context[port_index].trysnk_enable = 0;
        break;
    case 5: //DUAL + Provider
        context[port_index].trysrc_enable = 1;
        break;
    case 6: //DUAL + Consumer
        context[port_index].trysnk_enable = 1;
        break;
    default:
        ucsi_errno = ERR_INVALID;
        set_error_data();
        result = UCSI_COMMAND_FAIL;
        break;
    }

    set_pdr_ack();

    return result;
}
#endif

static u8 ppm_set_pdr_process()
{
    u8 ret = UCSI_COMMAND_FAIL;

    if (ucsi_command_process_delay == 75)
        send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
    if (ucsi_command_process_delay) {
        if (--ucsi_command_process_delay == 0) {
            port_id = port_index;
            switch(get_power_role()) {
            case 1: //DFP
                role = PW_PROVIDER;
                break;
            case 0: //UFP
                role = PW_CONSUMER;
                break;
            }
            if (context[port_index].power_operation_mode_pdr & role) {
                set_uor_ack();
                cancel_all_timer();
                ret = UCSI_COMMAND_SUCC;
            } else {
                ucsi_errno = ERR_NEGOTIATION_FAIL;
                set_error_data();
                ret = UCSI_COMMAND_FAIL;
            }
        } else {
            ret = UCSI_COMMAND_CONTINUE;
        }
    }
    return ret;
}
/*
 * @desc   : OPM set power direction role for PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_set_pdr()
{
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD((XBYTE *)spdr + 2);
    #endif

    if (spdr->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = spdr->ConnectorNumber - 1;
    DETECT_POWER_ON(port_index);
    context[port_index].power_operation_mode_pdr = 0;
    if ((((XBYTE *)spdr)[3])&0x80)
        context[port_index].power_operation_mode_pdr |= PW_PROVIDER;
    if ((((XBYTE *)spdr)[2])&0x1)
        context[port_index].power_operation_mode_pdr |= PW_CONSUMER;
    if ((((XBYTE *)spdr)[2])&0x2)
        context[port_index].power_operation_mode_pdr |= PW_DRP;

    if (context[port_index].dfp_only && context[port_index].power_operation_mode_pdr != PW_PROVIDER) {
        ucsi_errno = ERR_INVALID;
        set_error_data();
        result = UCSI_COMMAND_FAIL;
        goto out;
    }

    port_id = port_index;

    send_source_sink_capability();

    switch(get_power_role()) {
    case 1: //DFP
        role = PW_PROVIDER;
        break;
    case 0: //UFP
        role = PW_CONSUMER;
        break;
    }

    #ifdef ANXDEBUG
    dprint("___________ pdr  %.bx, %.bx, %.bx, role %.bx.\n", ((XBYTE *)spdr)[2], ((XBYTE *)spdr)[3],
        context[port_index].power_operation_mode_pdr, role);
    #endif

    result = UCSI_COMMAND_SUCC;
    switch (context[port_index].power_operation_mode_pdr) {
    case PW_PROVIDER:
        if (role == PW_CONSUMER) {
            //send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(POWER_ROLE_SWAP_DELAY_MS);
                port_id = port_index;
                if (get_power_role() != 1) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                //Fix JIRA LBT-412
                ucsi_command_process_delay = 75;
                result = UCSI_COMMAND_CONTINUE;
            }
        }
        break;
    case PW_CONSUMER:
        if (role == PW_PROVIDER) {
            //send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(POWER_ROLE_SWAP_DELAY_MS);
                if (get_power_role() != 0) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                //Fix JIRA LBT-412
                ucsi_command_process_delay = 75;
                result = UCSI_COMMAND_CONTINUE;
            }
        }
        break;
    case PW_DRP:
        //Do nothing
        result = UCSI_COMMAND_SUCC;
        break;
    case PW_DRP_CONSUMER:
        if (role == PW_PROVIDER) {
            //send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(POWER_ROLE_SWAP_DELAY_MS);
                if (get_power_role() != 0) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                //Fix JIRA LBT-412
                ucsi_command_process_delay = 75;
                result = UCSI_COMMAND_CONTINUE;
            }
        }
        break;
    case PW_DRP_PROVIDER:
        if (role == PW_CONSUMER) {
            //send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_PSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(POWER_ROLE_SWAP_DELAY_MS);
                port_id = port_index;
                if (get_power_role() != 1) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                ucsi_command_process_delay = 75;
                result = UCSI_COMMAND_CONTINUE;
            }
        }
        break;
    default:
        ucsi_errno = ERR_UNRECOGNIZED;
		result = UCSI_COMMAND_FAIL;
        break;
    }

out:
    set_pdr_ack();

    return result;
}

#if UCSI_SET_UOM_SUPPORT
/*
 * @desc   : OPM set usb operation mode for PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_set_uom()
{
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD((XBYTE *)suom + 2);
    #endif

    if (suom->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = suom->ConnectorNumber - 1;

    if (context[port_index].dfp_only && suom->USBOperationMode != OP_DFP) {
        ucsi_errno = ERR_INVALID;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }
    context[port_index].data_operation_mode_uom = 0;

    result = UCSI_COMMAND_SUCC;
    switch (suom->USBOperationMode) {
    case 1: //UFP
    case 2: //DRP
    case 4: //DRP
    case 5: //DRP + DFP
    case 6: //DRP + UFP
        context[port_index].data_operation_mode_uom = suom->USBOperationMode;
        #ifdef ANXDEBUG
        dprint("___________ 1  data operation mode is %.bx, %.bx.\n", suom->USBOperationMode,
            context[port_index].data_operation_mode_uom);
        if ((((XBYTE *)suom)[3])&0x80)
            context[port_index].data_operation_mode_uom |= OP_DFP;
        if ((((XBYTE *)suom)[2])&0x1)
            context[port_index].data_operation_mode_uom |= OP_UFP;
        if ((((XBYTE *)suom)[2])&0x2)
            context[port_index].data_operation_mode_uom |= OP_DRP;
        dprint("___________ 2  %.bx, %.bx, %.bx.\n", ((XBYTE *)suom)[2], ((XBYTE *)suom)[3],
            context[port_index].data_operation_mode_uom);
        #endif
        break;
    default:
        ucsi_errno = ERR_INVALID;
        set_error_data();
        result = UCSI_COMMAND_FAIL;
        break;
    }

    set_uor_ack();
    if ((result == UCSI_COMMAND_SUCC) && (context[port_index].anx_power_status == 0))
        chip_power_on(port_index);

    return result;
}
#endif

static u8 ppm_set_uor_process()
{
    u8 ret = UCSI_COMMAND_FAIL;

    if (ucsi_command_process_delay == 50)
        send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
    if (ucsi_command_process_delay) {
        if (--ucsi_command_process_delay == 0) {
            port_id = port_index;
            switch (get_data_role()) {
            case 1: //DFP
                role = OP_DFP;
                context[port_id].ucsi_connector_partner_type = CPT_UFP_ATTACHED;
                break;
            case 0: //UFP
                role = OP_UFP;
                context[port_id].ucsi_connector_partner_type = CPT_DFP_ATTACHED;
                break;
            }
            if (context[port_id].data_operation_mode_uor & role) {
                if (context[port_id].csc.csc.PowerDirection == 1) {
                    context[port_id].csc.connector_change_indicator = 0;
                    context[port_id].csc.csc.PowerDirection = 1;
                } else {
                    context[port_id].csc.connector_change_indicator = 0;
                    cancel_all_timer();
                }
                set_uor_ack();
                ret = UCSI_COMMAND_SUCC;
            } else {
                ucsi_errno = ERR_NEGOTIATION_FAIL;
                set_error_data();
                ret = UCSI_COMMAND_FAIL;
            }
        } else {
            ret = UCSI_COMMAND_CONTINUE;
        }
    }
    return ret;
}
/*
 * @desc   : OPM set usb operation role for PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_set_uor()
{
    //u8 data_role = 0;
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD((XBYTE *)suor + 2);
    #endif

    if (suor->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = suor->ConnectorNumber - 1;
    
    DETECT_POWER_ON(port_index);
    context[port_index].data_operation_mode_uor = 0;

    //context[port_index].data_operation_mode_uor = (((((XBYTE *)suor)[3])&0x80) >> 6) | (((((XBYTE *)suor)[2])&0x3) << 1);
    if ((((XBYTE *)suor)[3])&0x80)
        context[port_index].data_operation_mode_uor |= OP_DFP;
    if ((((XBYTE *)suor)[2])&0x1)
        context[port_index].data_operation_mode_uor |= OP_UFP;
    if ((((XBYTE *)suor)[2])&0x2)
        context[port_index].data_operation_mode_uor |= OP_DRP;

    if (context[port_index].dfp_only && context[port_index].data_operation_mode_uor != OP_DFP) {
        ucsi_errno = ERR_INVALID;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }
    port_id = port_index;
    switch (get_data_role()) {
    case 1: //DFP
        role = OP_DFP;
        break;
    case 0: //UFP
        role = OP_UFP;
        break;
    }

    result = UCSI_COMMAND_SUCC;
    port_id = port_index;
    #ifdef ANXDEBUG
    dprint("___________ 3  %.bx, %.bx, %.bx, role %.bx.\n", ((XBYTE *)suor)[2], ((XBYTE *)suor)[3],
        context[port_index].data_operation_mode_uor, role);
    #endif

    send_source_sink_capability();

    switch (context[port_index].data_operation_mode_uor) {
    case OP_DFP:
        if (role == OP_UFP) {
            //send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(DATA_ROLE_SWAP_DELAY_MS);

                if (get_data_role() != OP_DFP) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                ucsi_command_process_delay = 50;
                result = UCSI_COMMAND_CONTINUE;
            }
        } else
            result = UCSI_COMMAND_SUCC;
        break;
    case OP_UFP:
        if (role == OP_DFP) {
            //send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(DATA_ROLE_SWAP_DELAY_MS);

                if (get_data_role() != OP_UFP) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                ucsi_command_process_delay = 50;
                result = UCSI_COMMAND_CONTINUE;
            }
        } else
            result = UCSI_COMMAND_SUCC;
        break;
    case OP_DRP:
        //Do nothing
        result = UCSI_COMMAND_SUCC;
        break;
    case OP_DRP_DFP:
        if (role == OP_UFP) {
            //send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(DATA_ROLE_SWAP_DELAY_MS);
                if (get_data_role() != OP_DFP) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                ucsi_command_process_delay = 50;
                result = UCSI_COMMAND_CONTINUE;
            }
        } else
        result = UCSI_COMMAND_SUCC;
        break;
    case OP_DRP_UFP:
        if (role == OP_DFP) {
            //send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
            if (!(context[port_index].pd_capability & PD_HAS_RETURN_STATUS)) {
                send_pd_msg(TYPE_DSWAP_REQ, 0, 0, SOP_TYPE);
                vDelayXms(DATA_ROLE_SWAP_DELAY_MS);
                if (get_data_role() != OP_UFP) {
                    ucsi_errno = ERR_NEGOTIATION_FAIL;
                    set_error_data();
                    result = UCSI_COMMAND_FAIL;
                }
            } else {
                ucsi_command_process_delay = 50;
                result = UCSI_COMMAND_CONTINUE;
            }
        } else
        result = UCSI_COMMAND_SUCC;
        break;
    }

    set_uor_ack();

    return result;
}

/*
 * @desc   : OPM get connector capability from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_get_connector_capability()
{
    if (gcc->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    port_index = gcc->ConnectorNumber - 1;

    S_CLEAR(ccd, struct connector_capability_data);

    //DETECT_POWER_ON(port_index);

    if (context[port_index].dfp_only)
        ccd->OperationMode = OM_DFP_ONLY | OM_USB2_SUP | OM_USB3_SUP |
                OM_AUDIO_SUP | OM_DEBUG_ACC_SUP;
    else
        ccd->OperationMode = OM_DRP | OM_DFP_ONLY | OM_UFP_ONLY |
            OM_USB2_SUP | OM_USB3_SUP;

    #if UCSI_ALT_SUPPORT
    ccd->OperationMode |= OM_ALT_SUP;
    #endif
    ccd->Provider = 1;
    if (!context[port_index].dfp_only)
        ccd->Consumer = 1;


    result = UCSI_COMMAND_SUCC;
    get_connector_capability_ack();

    return result;
}

/*
 * @desc   : OPM get capability from PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_get_capability()
{
    S_CLEAR(cci, struct cci_status);
    S_CLEAR(cd, struct capability_data);

    cd->bmAttributes.v1.DisabledStateSupport = 0;
    cd->bmAttributes.v1.BatteryChaging = 1;
    cd->bmAttributes.v1.USBPowerDelivery = 1;
    cd->bmAttributes.v1.USBTypeCCurrent = 1;
    cd->bmAttributes.v1.bmPowerSource.v1.UsesVBUS = 1;
    cd->bNumConnectors = PD_MAX_INSTANCE;

    cd->bmOptionalFeatures1 = CMD_OPTIONAL_SUPPORT;
#if UCSI_ALT_SUPPORT
    //support 2 alt mode, usb 3.x, DP.
    cd->bNumAltModes = 2;
#endif
    cd->bcdBCVersion = 0;
    cd->bcdPDVersion = PD_VERSION;
    cd->bcdUSBTypeCVersion = TYPEC_VERSION;
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD(&cd->bcdPDVersion);
    SWAP_WORD(&cd->bcdUSBTypeCVersion);
    #endif

    result = UCSI_COMMAND_SUCC;
    get_capability_ack();

    return UCSI_COMMAND_SUCC;
}

static void ppm_init()
{
    //dprint("ppm_init\n");
    for (i = 0; i < PD_MAX_INSTANCE; i++) {
        chip_power_on(i);
        S_CLEAR(&context[i].csc.csc, struct connector_status_change);
        context[i].power_operation_mode_pdm = PW_DRP;
        context[i].power_operation_mode_pdr = PW_DRP;
		context[i].data_operation_mode_uom = OP_DRP;
        context[i].data_operation_mode_uor = OP_DRP;
    }
}

static void ppm_init_source_sink_capability()
{
    for (port_id = 0; port_id < PD_MAX_INSTANCE; port_id++)
        if (context[port_id].anx_power_status == 1)
            send_source_sink_capability();
}

/*
 * @desc   : OPM reset PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_reset()
{
    S_CLEAR(&ne, struct notification_enable_status);

    cancel_all_timer();
    ppm_init();
    ppm_init_source_sink_capability();
    result = UCSI_COMMAND_SUCC;
    reset_ack();
    memcpy((BYTEP )&xUCSI_DS_CCI0, (BYTEP )cci, 4);   
    //PPM_RESET just need to to IDLE status
    //GOTO_STATE(PPM_IDLE_ND);
    return UCSI_COMMAND_SUCC;
}

/*
 * @desc   : OPM CANCEL command
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_cancel()
{
    if (current_pending_command) {
        result = UCSI_COMMAND_SUCC;
        cancel_ack();
    } else {
        result = UCSI_COMMAND_FAIL;
        cancel_ack();
        result = UCSI_COMMAND_NOT_NOTIFY;
    }

    return result;
}

/*
 * @desc   : OPM ACK to PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_ack_cc_ci()
{
#ifdef ANXDEBUG
    dprint("%s:%d ConnectorChangeAck(%.bx), CommandCompletedAck(%.bx).\n",
            __func__, __LINE__,
            acc->ConnectorChangeAcknowledge,
            acc->CommandCompletedAcknowledge);
#endif
    result = UCSI_COMMAND_SUCC;
    ack_cc_ci_ack();

    if (acc->ConnectorChangeAcknowledge)
        connector_change_need_ack = 0;

    if (acc->CommandCompletedAcknowledge)
        command_complete_need_ack = 0;

    return UCSI_COMMAND_SUCC;
}
static u8 ppm_connector_reset_process()
{
    u8 ret = UCSI_COMMAND_FAIL;
    if (ucsi_command_process_delay) {
        if (--ucsi_command_process_delay == 0) {
            if (context[port_index].csc.csc.PDResetComplete) {
                connector_reset_ack();
                ret = UCSI_COMMAND_SUCC;
            } else {
                ucsi_errno = ERR_UNRECOGNIZED;
                set_error_data();
                ret = UCSI_COMMAND_FAIL;
            }
        } else {
            ret = UCSI_COMMAND_CONTINUE;
        }
    }
    return ret;
}

/*
 * @desc   : OPM reset connector on PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_connector_reset()
{
    port_index = cr->ConnectorNumber - 1;

    if (cr->ConnectorNumber > PD_MAX_INSTANCE) {
        ucsi_errno = ERR_NON_EXIST;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }
    DETECT_POWER_ON(port_index);
	#if SUPPORT_BATTERY_IN
    if (IS_BATTERY1_OUT() || (IS_BATTERY1_IN() && xEC_Bt1RSOC <= 3))
	#endif
	{
        ucsi_errno = ERR_CMD_UNSUCCESS_DUE_TO_DEAD_BATTERY;
        set_error_data();
        return UCSI_COMMAND_FAIL;
    }

    result = UCSI_COMMAND_SUCC;
        port_id = port_index;

    if (cr->HardReset)
        send_pd_msg(TYPE_HARD_RST, NULL, 0, SOP_TYPE);
    else
        send_pd_msg(TYPE_SOFT_RST, NULL, 0, SOP_TYPE);

    connector_reset_ack();

    return result;
}

/*
 * @desc   : OPM set notification enable on PPM
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_set_notification_enable()
{
    ne.CommandCompleted = sne->status.CommandCompleted;
    ne.ExternalSupplyChange = sne->status.ExternalSupplyChange;
    ne.PowerOperationModeChange = sne->status.PowerOperationModeChange ;
    ne.SupportedProviderCapabilitiesChange = sne->status.SupportedProviderCapabilitiesChange ;
    ne.NegotiatedPowerLevelChange = sne->status.NegotiatedPowerLevelChange ;
    ne.PDResetComplete = sne->status.PDResetComplete ;
    ne.SupportedCAMChange = sne->status.SupportedCAMChange ;
    ne.BatteryChargingStatusChange = sne->status.BatteryChargingStatusChange ;
    ne.ConnectorPartnerChange = sne->status.ConnectorPartnerChange ;
    ne.PowerDirectionChange = sne->status.PowerDirectionChange ;
    ne.ConnectChange = sne->status.ConnectChange ;
    ne.Error = sne->status.Error ;

    result = UCSI_COMMAND_SUCC;
    set_notification_enable_ack();

    return UCSI_COMMAND_SUCC;
}

static void ppm_alert(void)
{
    #ifdef ANXDEBUG
    dprint("%s:%d, ------------- PPM alert OPM, cci -> %.bx %.bx %.bx %.bx.\n", __func__, __LINE__,
        xUCSI_DS_CCI0, xUCSI_DS_CCI1, xUCSI_DS_CCI2, xUCSI_DS_CCI3);
    #endif
    //ucsi_ppm_notify_opm();
}
static void ucsi_command_complete_notify_opm(void)
{
    if (ne.CommandCompleted) {
       memcpy((BYTEP )&xUCSI_DS_CCI0, (BYTEP )cci, 4);
       memcpy((BYTEP )&xUCSI_DS_MGI, (BYTEP )msg->msgi, 16);
       dprint("++++    CTRL message -> ");
	   for (i = 0; i < 16; i++) {
	       dprint("%.bx ", xUCSI_DS_MGI[i]);
       }
	   dprint("\n");
       ppm_alert();
   }
}
static u8 ucsi_command_process_timeout_checking(void)
{
    if (current_pending_command_timeout) {
        if (--current_pending_command_timeout == 0) {
        #ifdef ANXDEBUG
            dprint("%s:%d command timeout, pending command(%.bx), state(%.bx).\n",
                __func__, __LINE__, current_pending_command, ucsi_ppm_state);
        #endif
            ucsi_errno = ERR_UNRECOGNIZED;
            set_error_data();
            ucsi_command_complete_notify_opm();
            return UCSI_COMMAND_SUCC;
        }
    }
    return UCSI_COMMAND_FAIL;
}
static void async_notify_opm(void)
{
    S_CLEAR(cci, struct cci_status);
    if ((ucsi_async_restore >= 5) && (ucsi_async_checking_timer == 0)) {
        ucsi_async_checking_timer = ucsi_async_restore;
        ucsi_async_restore = 0;
    }
    if (ucsi_async_checking_timer) {
        if (--ucsi_async_checking_timer == 0) {
            ucsi_ppm_connector_change_detect();
            if (cci->ConnectorChangeIndicator) {
                ucsi_connector_change_notify_opm(0, cci->ConnectorChangeIndicator, 0);
            }
        }
    }
}
/*
 * @desc   : PPM IDLE processing
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_nd_process(void)
{
    u8 ret = UCSI_COMMAND_FAIL;
    switch (current_pending_command) {
    case UCSI_COMMAND_NOTIFICATION_ENABLE:
        ppm_set_notification_enable();

        if (ne.CommandCompleted) {
            wait_for_ack_back = 250;
            GOTO_STATE(PPM_WCCA);
            ret = UCSI_COMMAND_SUCC;
        }
        break;
    case UCSI_COMMAND_PPM_RESET:
        ppm_reset();
        GOTO_STATE(PPM_IDLE_ND);
        break;
    case UCSI_COMMAND_RESERVED:
        return ret;
        break;
    default:
        break;
    }

    current_pending_command = 0;
    dprint("%s:%d UCSI state(PPM IDLE), Command(%d).\n",__func__, __LINE__, sne->header.Command);
    if (ret == UCSI_COMMAND_SUCC)
        ucsi_command_complete_notify_opm();
    return ret;
}

/*
 * @desc   : PPM process state machine processing
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static void ppm_pc_process()
{
    u8 ret = UCSI_COMMAND_FAIL;

    ret = ucsi_command_process_timeout_checking();
    if (ret == UCSI_COMMAND_SUCC) {
        current_pending_command = 0;
        wait_for_ack_back = 250;
        GOTO_STATE(PPM_WCCA);
        return;
    }
        dprint("%s:%d ucsi pc process.\n",__func__, __LINE__);
    switch (ch->Command) {
    default:
        dprint("%s:%d Warning: received unsupported command(0x%x) at PPM_PC state.\n",__func__, __LINE__, ch->Command);
        ucsi_errno = ERR_UNRECOGNIZED;
        set_error_data();
        ret = UCSI_COMMAND_FAIL;
        break;
    case UCSI_COMMAND_CONNECTOR_RESET:
        ret = ppm_connector_reset_process();
        break;
    case UCSI_COMMAND_UOR:
        ret = ppm_set_uor_process();
        break;
    case UCSI_COMMAND_PDR:
        ret = ppm_set_pdr_process();
        break;
    }

    if (ret == UCSI_COMMAND_CONTINUE)
        return;
    current_pending_command = 0;
    ucsi_command_complete_notify_opm();
    wait_for_ack_back = 250;
    GOTO_STATE(PPM_WCCA);
}

/*
 * @desc   : PPM busy state machine processing
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_busy_process(u8 Command)
{
    u8 ret = UCSI_COMMAND_FAIL;

    S_CLEAR(cci, sizeof(struct cci_status));
    switch (Command) {
    default:
#ifdef ANXDEBUG
        dprint("%s:%d Warning: received unsupported command(0x%x)"
                " from OPM at PPM_BUSY state.\n",
                __func__, __LINE__, ch->Command);
#endif
        busy_ack();
        ret = UCSI_COMMAND_SUCC;
        break;
    case UCSI_COMMAND_PPM_RESET:
        ppm_reset();
        GOTO_STATE(PPM_IDLE_ND);
        ret = UCSI_COMMAND_NOT_NOTIFY;
        current_pending_command = 0;
        break;
    case UCSI_COMMAND_ACK_CC_CI:
        ret = ppm_ack_cc_ci();
        break;
    case UCSI_COMMAND_CANCEL:
        ret = ppm_cancel();
        GOTO_STATE(PPM_IDLE_NE);
        current_pending_command = 0;
        break;
    }
    if (ret != UCSI_COMMAND_NOT_NOTIFY)
        ucsi_command_complete_notify_opm();

    return ret;
}

/*
 * @desc   : ASYNC event wait for OPM ACK state machine processing
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_waea_process()
{
    u8 ret = UCSI_COMMAND_FAIL;

    switch (ch->Command) {
    case UCSI_COMMAND_ACK_CC_CI:
        ret = ppm_ack_cc_ci();
        GOTO_STATE(PPM_IDLE_NE);
        break;
    case UCSI_COMMAND_PPM_RESET:
        ppm_reset();
        ret = UCSI_COMMAND_NOT_NOTIFY;
        GOTO_STATE(PPM_IDLE_ND);
        break;
    case UCSI_COMMAND_GET_CONNECTOR_STATUS:
        ret = ppm_get_connector_status();
        GOTO_STATE(PPM_IDLE_NE);
        break;
    default:
#ifdef ANXDEBUG
        dprint("%s:%d, recevied not ACK_CC_CI command, continue waiting.\n",
                __func__, __LINE__);
#endif
        break;
    }

    current_pending_command = 0;
    if (UCSI_COMMAND_SUCC)
        ucsi_command_complete_notify_opm();
    return ret;
}


/*
 * @desc   : PPM wait for OPM ACK state machine processing
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_wcca_process()
{
    u8 ret = UCSI_COMMAND_FAIL;

    current_pending_command_timeout = 0;
    switch (ch->Command) {
    case UCSI_COMMAND_ACK_CC_CI:
        ret = ppm_ack_cc_ci();
        GOTO_STATE(PPM_IDLE_NE);
        break;
    case UCSI_COMMAND_PPM_RESET:
        ret = ppm_reset();
        GOTO_STATE(PPM_IDLE_ND);
        break;
    default:
        wait_for_ack_back--;
        if ((wait_for_ack_back == 120) || (wait_for_ack_back == 1)) 
		{
            dprint(" ack timeout, re-notify OPM.\n");
            ucsi_command_complete_notify_opm();
        }
		else if (wait_for_ack_back == 0)
		{
            dprint(" ack timeout, goto idle notification enable status.\n");
            GOTO_STATE(PPM_IDLE_NE);
        }
        return ret;
        dprint("%s:%d, recevied not ACK_CC_CI command, continue waiting.\n", __func__, __LINE__);
        break;
    }

    current_pending_command = 0;
    if (UCSI_COMMAND_SUCC)
        ucsi_command_complete_notify_opm();
    return ret;
}

/*
 * @desc   : PPM main process
 *           PPM finish all required command processing here
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
static u8 ppm_ne_process()
{
    u8 ret = UCSI_COMMAND_FAIL;

    switch (ch->Command) {
    case UCSI_COMMAND_PPM_RESET:
        ppm_reset();
        GOTO_STATE(PPM_IDLE_ND);
        ret = UCSI_COMMAND_NOT_NOTIFY;
        break;
    case UCSI_COMMAND_CANCEL:
        //Just drop CANCEL command
        ret = UCSI_COMMAND_NOT_NOTIFY;
        break;
    case UCSI_COMMAND_CONNECTOR_RESET:
        ret = ppm_connector_reset();
        break;
    case UCSI_COMMAND_ACK_CC_CI:
        ret = ppm_ack_cc_ci();
        break;
    case UCSI_COMMAND_NOTIFICATION_ENABLE:
        ret = ppm_set_notification_enable();
        break;
    case UCSI_COMMAND_CAPABILITY:
        ret = ppm_get_capability();
        break;
    case UCSI_COMMAND_CONNECTOR_CAPABILITY:
        ret = ppm_get_connector_capability();
        break;
#if UCSI_SET_UOM_SUPPORT
    case UCSI_COMMAND_UOM:
        ret = ppm_set_uom();
        break;
#endif
    case UCSI_COMMAND_UOR:
        ret = ppm_set_uor();
        break;
#if UCSI_SET_PDM_SUPPORT
    case UCSI_COMMAND_PDM:
        ret = ppm_set_pdm();
        break;
#endif
    case UCSI_COMMAND_PDR:
        ret = ppm_set_pdr();
        break;
#if UCSI_ALT_SUPPORT
    case UCSI_COMMAND_GET_ALTERNATE_MODES:
        ret = ppm_get_alt_modes();
        break;
#endif
#if UCSI_ALT_SUPPORT
    case UCSI_COMMAND_GET_CAM_SUPPORTED:
        ret = ppm_get_cam_supported();
        break;
#endif
#if UCSI_ALT_SUPPORT
    case UCSI_COMMAND_GET_CURRENT_CAM:
        ret = ppm_get_current_cam();
        break;
#endif
#if UCSI_ALT_OR_SUPPORT
    case UCSI_COMMAND_SET_NEW_CAM:
        ret = ppm_set_new_cam();
        break;
#endif
#if UCSI_PDOS_SUPPORT
    case UCSI_COMMAND_GET_PDOS:
        ret = ppm_get_pdos();
        break;
#endif
#if UCSI_CABLE_SUPPORT
    case UCSI_COMMAND_GET_CABLE_PROPERTY:
        ret = ppm_get_cable_property();
        break;
#endif
    case UCSI_COMMAND_GET_CONNECTOR_STATUS:
        ret = ppm_get_connector_status();
        break;
    case UCSI_COMMAND_GET_ERROR_STATUS:
        ret = ppm_get_error_status();
        break;
    case UCSI_COMMAND_RESERVED:
        async_notify_opm();
        return ret;
        break;
    default:
        ret = unsupport_command_process();
#ifdef ANXDEBUG
        dprint("%s:%d Cannot support command(0x%x) from OPM.\n",
            __func__, __LINE__, ch->Command);
#endif
        break;
    }

    switch (ret) {
    case UCSI_COMMAND_NOT_NOTIFY:
        current_pending_command = 0;
        break;
    case UCSI_COMMAND_CONTINUE:
        #if UCSI_COMMAND_RETURN_BUSY_ACK
        //busy_ack();
        //ucsi_command_complete_notify_opm();
#endif
        GOTO_STATE(PPM_PC);
        break;
    //case UCSI_COMMAND_BUSY:
    case UCSI_COMMAND_SUCC:
    case UCSI_COMMAND_FAIL:
        wait_for_ack_back = 250;
        GOTO_STATE(PPM_WCCA);
        current_pending_command = 0;
        ucsi_command_complete_notify_opm();
        break;
    }
    return ret;
}

/*
 * @desc   : UCSI connector change notify function
 *           PPM call this after detecting USB Type-C connector status calling
 * @param  :
 *           ack : Command Completed Indicator setting
 *           connector_index : registered Connector ID
 *           wait_for_ack : Change state machine to PPM_WCCA
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
BYTE  ucsi_connector_change_notify_opm(u8 ack, u8 connector_index, u8 wait_for_ack)
{
    u8 invalid = 1;

    //Ignore notify OPM under NOTIFICATION DISABLE STAE
    if (ucsi_ppm_state == PPM_IDLE_ND)
        return UCSI_COMMAND_FAIL;

    //If current PPM is not under PPM_IDLE_NE, no need notify OPM.
    if ((ack == 0) && (wait_for_ack == 0) &&
        (ucsi_ppm_state != PPM_IDLE_NE))
        return UCSI_COMMAND_SUCC;

    cci = (void *)&msg->CCI;
    S_CLEAR(cci, struct cci_status);
    S_CLEAR_L((void *)&msg->msgi, 16);

    if ((connector_index) && (ne.ConnectChange == 1)) {
        cci->ConnectorChangeIndicator = connector_index;
        invalid = 0;
    }

    if (ack && ne.CommandCompleted) {
        invalid = 0;
        cci->CommandCompletedIndicator = 1;
    }

    if (invalid)
        return UCSI_COMMAND_FAIL;

    if (ne.CommandCompleted) {

        memcpy((BYTEP )&xUCSI_DS_CCI0, (BYTEP )cci, 4);
        ppm_alert();
    }
}


/*
 * @desc   : UCSI PPM handler, EC should call this handler after received
 *           event from OPM/BIOS
 * @param  : none
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
void checking_opm_command(void)
{
	//dprint("checking_opm_command\n");
    if (ucsi_received_command == 1) 
	{
        ucsi_received_command = 0;

        if (current_pending_command == 0) 
		{
            memcpy((void *)&msg->CONTROL, (BYTEP )&xUCSI_COMMAND, 8);
            dprint("%s:%d received command 0x%x\n", __func__, __LINE__, current_pending_command);
            current_pending_command_timeout = 250;
			ucsi_errno = 0;
			S_CLEAR_L((void *)&msg->msgi, 16);
        } 
		else 
		{
            dprint("%s:%d, received command 0x%x at busy state.\n", __func__, __LINE__, xUCSI_COMMAND);
            ppm_busy_process(xUCSI_COMMAND);
		}
    }
}


/*
 * @desc   : EC should call this function to initial UCSI framework
 * @param  :
 *           msg -> shared message buffer between PPM and BIOS
 *           notify_callbak -> UCSI use this callbak function to notify BIOS
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
BYTE ucsi_init()
{
    msg = &ppm2bios;

//EVB only
#ifdef ITE_EC_EVB_PORT1_TEST
//#message "ANX INIT on EVB"

    /*Initial GPIO, H6->Liberty 0, H7->Liberty 1*/
    GPCRH5 = INPUT;
    GPCRH6 = INPUT;
    GPCRH7 = MaxD;

#else
//#message "ANX INIT on MRD"

#endif

    timer1msCount = 0;
    ucsi_ppm_state = PPM_IDLE_ND;
    connector_change_need_ack = 0;
    command_complete_need_ack = 0;
    need_ack_command = 0;
    current_pending_command = 0;
    self_charging_error = 0;
    cci = (void *)&msg->CCI;
    ch = (struct control_header *)&(msg->CONTROL);
    sne = (void *)&msg->CONTROL;
    cr = (void *)&msg->CONTROL;
    acc = (void *)&msg->CONTROL;
    gcc = (void *)&msg->CONTROL;
    suor = (void *)&msg->CONTROL;
    suom = (void *)&msg->CONTROL;
    spdr = (void *)&msg->CONTROL;
    spdm = (void *)&msg->CONTROL;
    gcs = (void *)&msg->CONTROL;
    gcp = (void *)&msg->CONTROL;
    gp = (void *)&msg->CONTROL;
    snc = (void *)&msg->CONTROL;
    gcc = (void *)&msg->CONTROL;
    gcs = (void *)&msg->CONTROL;
    gam = (void *)&msg->CONTROL;
    //message in
    csd = (void *)&msg->msgi;
    amd = (void *)&msg->msgi;
    pd = (void *)&msg->msgi;
    cpd = (void *)&msg->msgi;
    cd = (void *)&msg->msgi;
    ccd = (void *)&msg->msgi;
	supply_1500ma_flag = 0;
    
    msg->version = UCSI_COMPLIANT_VERSION;
    #ifdef EC_ANX_BIG_ENDIAN
    SWAP_WORD(&msg->version);
    memcpy((BYTEP )&xUCSI_DS_VERSION, &msg->version, 2);
    #endif

    ppm_init();
    cancel_all_timer();
    anx7447_ucsi_init();
    anx_dead_battery_checking_1();
    
    GOTO_STATE(PPM_IDLE_ND);

    dprint("ucsi init all done\n");
    return UCSI_COMMAND_SUCC;
}


/*
 * @desc   : ANX context data structure initialization
 * @param  :
 *           param : anx platform context parameters
 * @return : none
 */
void anx_initial_context(u8 port_index)
{
    context[port_index].ucsi_connector_partner_type = 0;
    context[port_index].peer_svid_length = 0;
    context[port_index].downstream_pd_cap = 0;
    context[port_index].ucsi_partner_rdo = 0;
    context[port_index].power_op_mode = 0;
    context[port_index].anx_current_in_50ma = 0;
    context[port_index].anx_voltage_in_100mv = 0;
    context[port_index].power_role_swap_flag = 0;
    context[port_index].partner_pdo_length = 0;
    context[port_index].partner_pdo_sink_or_source = 0;
    context[port_index].sys_sta_bak = 0;
    context[port_index].anx_vbus_status = 0;
    context[port_index].pbuf_rx_front = 0;
    context[port_index].pbuf_tx_rear = 0;
    context[port_index].anx_power_role = 1;
	context[port_index].anx_data_role = 0;
    context[port_index].anx_initial_status = 0;
    //context->hard_reset_done = 0;
    //context->soft_reset_done = 0;
    context[port_index].enable_report_power_on = 0;
    S_CLEAR(&context[port_index].cv, struct cable_vdo);
    context[port_index].ct = CABLE_TYPE_A;
    //context[port_index].connector_index = 0;
    self_charging_error = 0;
    context[port_index].trysnk_enable = 0;
    context[port_index].trysrc_enable = 0;
    context[port_index].ConnectorPartner = CP_USB;
    context[port_index].BatteryCharging = 0;
    context[port_index].data_operation_mode_uor = OP_DRP;
    context[port_index].power_operation_mode_pdr = PW_DRP;
    S_CLEAR(&context[port_index].csc.csc, struct connector_status_change);
    if (context[port_id].pd_capability & PD_HAS_REDRIVER)
        context[port_id].mux_mode = NO_CONNECT;
    if (port_index == supply_1500ma_port && supply_1500ma_flag == 1) {
        supply_1500ma_port = 0;
        supply_1500ma_flag = 0;
    }
    
}

static void pd_initialization(void)
{
    for (i = 0; i < PD_MAX_INSTANCE; i++) {
        if (context[i].anx_power_status == 1 && context[i].anx_initial_status) {
            port_id = i;
            send_initialized_setting();
        }
    }
}

void ucsi_async_notify_raise_up(u8 ms)
{
    switch (ucsi_ppm_state) {
    case PPM_IDLE_ND:
        break;
    case PPM_IDLE_NE:
        ucsi_async_checking_timer = ms;
        break;
    default:
        ucsi_async_restore = ms;
        break;
    }
}

/*
 * @desc   : UCSI PPM handler, EC should call this handler after received
 *           event from OPM/BIOS
 * @param  : none
 * @return :
 *           UCSI_COMMAND_SUCC : success
 *           UCSI_COMMAND_FAIL : fail
 */
void ucsi_opm_handler(void)
{
    //dprint("ucsi_opm_handler\n");
    switch (ucsi_ppm_state) {
        case PPM_IDLE_ND:
            ppm_nd_process();
            break;
        case PPM_IDLE_NE:
            ppm_ne_process();
            break;
        case PPM_PC:
            ppm_pc_process();
            break;
        case PPM_BUSY:
            break;
        case PPM_WCCA:
            ppm_wcca_process();
			break;
    }
}

void ANX_HOOK_10ms(void)
{
    pd_initialization();
    checking_opm_command();
    ucsi_opm_handler();
}

#define IS_PD_INT_LO()              IS_CLEAR(GPIO0_EXT2,6)//check PD in
void ANX_HOOK_1ms(void)
{
    timer1msCount++;
#if SUPPORT_TYPEC_7447
    port_id = 0;
    if (IS_PD_INT_LO()) 
    {
        if (context[port_id].error_count < 100)
        {
            anx_alert_message_isr();
            ++context[port_id].error_count;
        }
        else 
		{
			dprint("ANX_HOOK_1ms error\n");
        }
    }
    else
    {
        context[port_id].error_count = 0;
    }
#endif
}


/* timer schedule */

/*
 * @desc   : anx_request_voltage_in_100mv, EC should call this to check the
 *           voltage while Type-C connector work as power sink.
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : Type-C connector work as power source.
 *           other : voltage, unit is 100mv.
 */
u8 anx_request_voltage_in_100mv(u8 port_id)
{
    u8 vol = 0;
	//anx_dead_battery_checking();
    if (self_charging_error == 1)
        return vol;
    if (context[port_id].anx_power_status == 0 || context[port_id].anx_power_role == 1)
        return 0;

    vol = (context[port_id].anx_voltage_in_100mv == 0) ? 50 : context[port_id].anx_voltage_in_100mv;
    #ifdef ANXDEBUG
    //dprint("     +++++++++  voltage : %x, in 100mv   +++++++++\n", (u16)vol);
    #endif
    return vol;
}

/*
 * @desc   : anx_request_current_in_50ma, EC should call this to check the
 *           current while Type-C connector work as power sink.
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : Type-C connector work as power source.
 *           other : current, unit is 50ma.
 */
u8 anx_request_current_in_50ma(u8 port_id)
{
    u8 cur = 0;
    //anx_dead_battery_checking();
    if (self_charging_error == 1)
        return cur;
    if (context[port_id].anx_power_status == 0 || context[port_id].anx_power_role == 1)
        return cur;

        cur = context[port_id].anx_current_in_50ma;
    #ifdef ANXDEBUG
    dprint("     +++++++++  current : %x, in 50ma   +++++++++\n", (u16)cur);
    #endif

    return cur;
}

/*
 * @desc   : anx_connector_power_status, EC should call this to check whether
 *           the PD chip is power provider or consumer
 * @param  : port_id -> port id number (0 or 1)
 * @return :
 *           0 : power provider or PD standby
 *           other : power consumer
 */
u8 anx_connector_power_status(u8 port_id)
{
	//anx_dead_battery_checking();
    return (context[port_id].anx_power_role == 0);
}


void anx_ucsi_command_received(void)
{
    ucsi_received_command = 1;
}

