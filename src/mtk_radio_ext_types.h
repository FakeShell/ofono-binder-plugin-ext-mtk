/*
 *  oFono - Open Source Telephony - binder based adaptation MTK plugin
 *
 *  Copyright (C) 2024 TheKit <thekit@disroot.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef MTK_RADIO_EXT_TYPES_H
#define MTK_RADIO_EXT_TYPES_H

#define MTK_RADIO_IFACE_PREFIX      "vendor.mediatek.hardware.mtkradioex@"
#define MTK_RADIO_IFACE(x)          MTK_RADIO_IFACE_PREFIX "3.0::" x
#define MTK_RADIO                   MTK_RADIO_IFACE("IMtkRadioEx")
#define MTK_RADIO_RESPONSE          MTK_RADIO_IFACE("IImsRadioResponse")
#define MTK_RADIO_INDICATION        MTK_RADIO_IFACE("IImsRadioIndication")

/* c(req, resp, callName, CALL_NAME) */
#define MTK_RADIO_EXT_IMS_CALL_3_0(c) \
    c(10, 3, videoCallAccept, VIDEO_CALL_ACCEPT) \
    c(11, 4, imsEctCommand, IMS_ECT_COMMAND) \
    c(12, 5, controlCall, CONTROL_CALL) \
    c(13, 6, imsDeregNotification, IMS_DEREG_NOTIFICATION) \
    c(14, 7, setImsEnabled, SET_IMS_ENABLED) \
    c(15, 8, setImsCfg, SET_IMS_CFG) \
    c(16, 9, getProvisionValue, GET_PROVISION_VALUE) \
    c(48, 1, hangupAll, HANGUP_ALL) \
    c(49, 2, setCallIndication, SET_CALL_INDICATION) \
    c(151, 43, setImsCfgFeatureValue, SET_IMS_CFG_FEATURE_VALUE)

typedef enum mtk_radio_req {
    /* vendor.mediatek.hardware.mtkradioex@3.0::IMtkRadioExt */
    MTK_RADIO_REQ_RESPONSE_ACKNOWLEDGEMENT_MTK = 1, /* responseAcknowledgementMtk */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_MTK = 2, /* setResponseFunctionsMtk */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_IMS = 3, /* setResponseFunctionsIms */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_MWI = 4, /* setResponseFunctionsMwi */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_SE = 5, /* setResponseFunctionsSE */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_EM = 6, /* setResponseFunctionsEm */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_ASSIST = 7, /* setResponseFunctionsAssist */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_CAP = 8, /* setResponseFunctionsCap */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_RSU = 9, /* setResponseFunctionsRsu */
    MTK_RADIO_REQ_VIDEO_CALL_ACCEPT = 10, /* videoCallAccept */
    MTK_RADIO_REQ_IMS_ECT_COMMAND = 11, /* imsEctCommand */
    MTK_RADIO_REQ_CONTROL_CALL = 12, /* controlCall */
    MTK_RADIO_REQ_IMS_DEREG_NOTIFICATION = 13, /* imsDeregNotification */
    MTK_RADIO_REQ_SET_IMS_ENABLED = 14, /* setImsEnable */
    MTK_RADIO_REQ_SET_IMS_CFG = 15, /* setImscfg */
    MTK_RADIO_REQ_GET_PROVISION_VALUE = 16, /* getProvisionValue */
    MTK_RADIO_REQ_SET_PROVISION_VALUE = 17, /* setProvisionValue */
    MTK_RADIO_REQ_CONTROL_IMS_CONFERENCE_CALL_MEMBER = 18, /* controlImsConferenceCallMember */
    MTK_RADIO_REQ_SET_WFC_PROFILE = 19, /* setWfcProfile */
    MTK_RADIO_REQ_CONFERENCE_DIAL = 20, /* conferenceDial */
    MTK_RADIO_REQ_SET_MODEM_IMS_CFG = 21, /* setModemImsCfg */
    MTK_RADIO_REQ_DIAL_WITH_SIP_URI = 22, /* dialWithSipUri */
    MTK_RADIO_REQ_VT_DIAL_WITH_SIP_URI = 23, /* vtDialWithSipUri */
    MTK_RADIO_REQ_VT_DIAL = 24, /* vtDial */
    MTK_RADIO_REQ_FORCE_RELEASE_CALL = 25, /* forceReleaseCall */
    MTK_RADIO_REQ_IMS_BEARER_STATE_CONFIRM = 26, /* imsBearerStateConfirm */
    MTK_RADIO_REQ_SET_IMS_RTP_REPORT = 27, /* setImsRtpReport */
    MTK_RADIO_REQ_PULL_CALL = 28, /* pullCall */
    MTK_RADIO_REQ_SET_IMS_REGISTRATION_REPORT = 29, /* setImsRegistrationReport */
    MTK_RADIO_REQ_SEND_EMBMS_AT_COMMAND = 30, /* sendEmbmsAtCommand */
    MTK_RADIO_REQ_SET_ROAMING_ENABLE = 31, /* setRoamingEnable */
    MTK_RADIO_REQ_GET_ROAMING_ENABLE = 32, /* getRoamingEnable */
    MTK_RADIO_REQ_SET_BARRING_PASSWORD_CHECKED_BY_NW = 33, /* setBarringPasswordCheckedByNW */
    MTK_RADIO_REQ_SET_CLIP = 34, /* setClip */
    MTK_RADIO_REQ_GET_COLP = 35, /* getColp */
    MTK_RADIO_REQ_GET_COLR = 36, /* getColr */
    MTK_RADIO_REQ_SEND_CNAP = 37, /* sendCnap */
    MTK_RADIO_REQ_SET_COLP = 38, /* setColp */
    MTK_RADIO_REQ_SET_COLR = 39, /* setColr */
    MTK_RADIO_REQ_QUERY_CALL_FORWARD_IN_TIME_SLOT_STATUS = 40, /* queryCallForwardInTimeSlotStatus */
    MTK_RADIO_REQ_SET_CALL_FORWARD_IN_TIME_SLOT = 41, /* setCallForwardInTimeSlot */
    MTK_RADIO_REQ_RUN_GBA_AUTHENTICATION = 42, /* runGbaAuthentication */
    MTK_RADIO_REQ_SEND_USSI = 43, /* sendUssi */
    MTK_RADIO_REQ_CANCEL_USSI = 44, /* cancelUssi */
    MTK_RADIO_REQ_GET_XCAP_STATUS = 45, /* getXcapStatus */
    MTK_RADIO_REQ_RESET_SUPP_SERV = 46, /* resetSuppServ */
    MTK_RADIO_REQ_SETUP_XCAP_USER_AGENT_STRING = 47, /* setupXcapUserAgentString */
    MTK_RADIO_REQ_HANGUP_ALL = 48, /* hangupAll */
    MTK_RADIO_REQ_SET_CALL_INDICATION = 49, /* setCallIndication */
    MTK_RADIO_REQ_SET_ECC_MODE = 50, /* setEccMode */
    MTK_RADIO_REQ_SET_ECC_NUM = 51, /* setEccNum */
    MTK_RADIO_REQ_GET_ECC_NUM = 52, /* getEccNum */
    MTK_RADIO_REQ_SET_CALL_SUB_ADDRESS = 53, /* setCallSubAddress */
    MTK_RADIO_REQ_GET_CALL_SUB_ADDRESS = 54, /* getCallSubAddress */
    MTK_RADIO_REQ_QUERY_PHB_STORAGE_INFO = 55, /* queryPhbStorageInfo */
    MTK_RADIO_REQ_WRITE_PHB_ENTRY = 56, /* writePhbEntry */
    MTK_RADIO_REQ_READ_PHB_ENTRY = 57, /* readPhbEntry */
    MTK_RADIO_REQ_QUERY_UPB_CAPABILITY = 58, /* queryUPBCapability */
    MTK_RADIO_REQ_EDIT_UPB_ENTRY = 59, /* editUPBEntry */
    MTK_RADIO_REQ_DELETE_UPB_ENTRY = 60, /* deleteUPBEntry */
    MTK_RADIO_REQ_READ_UPB_GAS_LIST = 61, /* readUPBGasList */
    MTK_RADIO_REQ_READ_UPB_GRP_ENTRY = 62, /* readUPBGrpEntry */
    MTK_RADIO_REQ_WRITE_UPB_GRP_ENTRY = 63, /* writeUPBGrpEntry */
    MTK_RADIO_REQ_GET_PHONE_BOOK_STRINGS_LENGTH = 64, /* getPhoneBookStringsLength */
    MTK_RADIO_REQ_GET_PHONE_BOOK_MEM_STORAGE = 65, /* getPhoneBookMemStorage */
    MTK_RADIO_REQ_SET_PHONE_BOOK_MEM_STORAGE = 66, /* setPhoneBookMemStorage */
    MTK_RADIO_REQ_READ_PHONE_BOOK_ENTRY_EXT = 67, /* readPhoneBookEntryExt */
    MTK_RADIO_REQ_WRITE_PHONE_BOOK_ENTRY_EXT = 68, /* writePhoneBookEntryExt */
    MTK_RADIO_REQ_QUERY_UPB_AVAILABLE = 69, /* queryUPBAvailable */
    MTK_RADIO_REQ_READ_UPB_EMAIL_ENTRY = 70, /* readUPBEmailEntry */
    MTK_RADIO_REQ_READ_UPB_SNE_ENTRY = 71, /* readUPBSneEntry */
    MTK_RADIO_REQ_READ_UPB_ANR_ENTRY = 72, /* readUPBAnrEntry */
    MTK_RADIO_REQ_READ_UPB_AAS_LIST = 73, /* readUPBAasList */
    MTK_RADIO_REQ_SET_PHONEBOOK_READY = 74, /* setPhonebookReady */
    MTK_RADIO_REQ_SET_MODEM_POWER = 75, /* setModemPower */
    MTK_RADIO_REQ_TRIGGER_MODE_SWITCH_BY_ECC = 76, /* triggerModeSwitchByEcc */
    MTK_RADIO_REQ_GET_ATR = 77, /* getATR */
    MTK_RADIO_REQ_GET_ICCID = 78, /* getIccid */
    MTK_RADIO_REQ_SET_SIM_POWER = 79, /* setSimPower */
    MTK_RADIO_REQ_ACTIVATE_UICC_CARD = 80, /* activateUiccCard */
    MTK_RADIO_REQ_DEACTIVATE_UICC_CARD = 81, /* deactivateUiccCard */
    MTK_RADIO_REQ_GET_CURRENT_UICC_CARD_PROVISIONING_STATUS = 82, /* getCurrentUiccCardProvisioningStatus */
    MTK_RADIO_REQ_DO_GENERAL_SIM_AUTHENTICATION = 83, /* doGeneralSimAuthentication */
    MTK_RADIO_REQ_QUERY_NETWORK_LOCK = 84, /* queryNetworkLock */
    MTK_RADIO_REQ_SET_NETWORK_LOCK = 85, /* setNetworkLock */
    MTK_RADIO_REQ_SUPPLY_DEPERSNALIZATION = 86, /* supplyDepersonalization */
    MTK_RADIO_REQ_SEND_VSIM_NOTIFICATION = 87, /* sendVsimNotification */
    MTK_RADIO_REQ_SEND_VSIM_OPERATION = 88, /* sendVsimOperation */
    MTK_RADIO_REQ_GET_SMS_PARAMETERS = 89, /* getSmsParameters */
    MTK_RADIO_REQ_SET_SMS_PARAMETERS = 90, /* setSmsParameters */
    MTK_RADIO_REQ_GET_SMS_MEM_STATUS = 91, /* getSmsMemStatus */
    MTK_RADIO_REQ_SET_ETWS = 92, /* setEtws */
    MTK_RADIO_REQ_REMOVE_CB_MSG = 93, /* removeCbMsg */
    MTK_RADIO_REQ_SET_GSM_BROADCAST_LANGS = 94, /* setGsmBroadcastLangs */
    MTK_RADIO_REQ_GET_GSM_BROADCAST_LANGS = 95, /* getGsmBroadcastLangs */
    MTK_RADIO_REQ_GET_GSM_BROADCAST_ACTIVATION = 96, /* getGsmBroadcastActivation */
    MTK_RADIO_REQ_SEND_IMS_SMS_EX = 97, /* sendImsSmsEx */
    MTK_RADIO_REQ_ACKNOWLEDGE_LAST_INCOMING_GSM_SMS_EX = 98, /* acknowledgeLastIncomingGsmSmsEx */
    MTK_RADIO_REQ_ACKNOWLEDGE_LAST_INCOMING_CDMA_SMS_EX = 99, /* acknowledgeLastIncomingCdmaSmsEx */
    MTK_RADIO_REQ_SEND_REQUEST_RAW = 100, /* sendRequestRaw */
    MTK_RADIO_REQ_SEND_REQUEST_STRINGS = 101, /* sendRequestStrings */
    MTK_RADIO_REQ_SET_RESUME_REGISTRATION = 102, /* setResumeRegistration */
    MTK_RADIO_REQ_MODIFY_MODEM_TYPE = 103, /* modifyModemType */
    MTK_RADIO_REQ_GET_SMS_RUIM_MEMORY_STATUS = 104, /* getSmsRuimMemoryStatus */
    MTK_RADIO_REQ_SET_NETWORK_SELECTION_MODE_MANUAL_WITH_ACT = 105, /* setNetworkSelectionModeManualWithAct */
    MTK_RADIO_REQ_GET_AVAILABLE_NETWORKS_WITH_ACT = 106, /* getAvailableNetworksWithAct */
    MTK_RADIO_REQ_GET_SIGNAL_STRENGTH_WITH_WCDMA_ECIO = 107, /* getSignalStrengthWithWcdmaEcio */
    MTK_RADIO_REQ_CANCEL_AVAILABLE_NETWORKS = 108, /* cancelAvailableNetworks */
    MTK_RADIO_REQ_GET_FEMTOCELL_LIST = 109, /* getFemtocellList */
    MTK_RADIO_REQ_ABORT_FEMTOCELL_LIST = 110, /* abortFemtocellList */
    MTK_RADIO_REQ_SELECT_FEMTOCELL = 111, /* selectFemtocell */
    MTK_RADIO_REQ_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE = 112, /* queryFemtoCellSystemSelectionMode */
    MTK_RADIO_REQ_SET_FEMTOCELL_SYSTEM_SELECTION_MODE = 113, /* setFemtoCellSystemSelectionMode */
    MTK_RADIO_REQ_SET_SERVICE_STATE_TO_MODEM = 114, /* setServiceStateToModem */
    MTK_RADIO_REQ_CFG_A2_OFFSET = 115, /* cfgA2offset */
    MTK_RADIO_REQ_CFG_B1_OFFSET = 116, /* cfgB1offset */
    MTK_RADIO_REQ_ENABLE_SCG_FAILURE = 117, /* enableSCGfailure */
    MTK_RADIO_REQ_SET_TX_POWER = 118, /* setTxPower */
    MTK_RADIO_REQ_SET_SEARCH_STORED_FREQ_INFO = 119, /* setSearchStoredFreqInfo */
    MTK_RADIO_REQ_SET_SEARCH_RAT = 120, /* setSearchRat */
    MTK_RADIO_REQ_SET_BGSRCH_DELTA_SLEEP_TIMER = 121, /* setBgsrchDeltaSleepTimer */
    MTK_RADIO_REQ_SET_RX_TEST_CONFIG = 122, /* setRxTestConfig */
    MTK_RADIO_REQ_GET_RX_TEST_RESULT = 123, /* getRxTestResult */
    MTK_RADIO_REQ_GET_POL_CAPABILITY = 124, /* getPOLCapability */
    MTK_RADIO_REQ_GET_CURRENT_POL_LIST = 125, /* getCurrentPOLList */
    MTK_RADIO_REQ_SET_POL_ENTRY = 126, /* setPOLEntry */
    MTK_RADIO_REQ_SET_FD_MODE = 127, /* setFdMode */
    MTK_RADIO_REQ_SET_TRM = 128, /* setTrm */
    MTK_RADIO_REQ_HANDLE_STK_CALL_SETUP_REQUEST_FROM_SIM_WITH_RES_CODE = 129, /* handleStkCallSetupRequestFromSimWithResCode */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_FOR_ATCI = 130, /* setResponseFunctionsForAtci */
    MTK_RADIO_REQ_SEND_ATCI_REQUEST = 131, /* sendAtciRequest */
    MTK_RADIO_REQ_RESTART_RILD = 132, /* restartRILD */
    MTK_RADIO_REQ_SYNC_DATA_SETTINGS_TO_MD = 133, /* syncDataSettingsToMd */
    MTK_RADIO_REQ_RESET_MD_DATA_RETRY_COUNT = 134, /* resetMdDataRetryCount */
    MTK_RADIO_REQ_SET_REMOVE_RESTRICT_EUTRAN_MODE = 135, /* setRemoveRestrictEutranMode */
    MTK_RADIO_REQ_SET_VOICE_DOMAIN_PREFERENCE = 136, /* setVoiceDomainPreference */
    MTK_RADIO_REQ_SET_WIFI_ENABLED = 137, /* setWifiEnabled */
    MTK_RADIO_REQ_SET_WIFI_ASSOCIATED = 138, /* setWifiAssociated */
    MTK_RADIO_REQ_SET_WIFI_SIGNAL_LEVEL = 139, /* setWifiSignalLevel */
    MTK_RADIO_REQ_SET_WIFI_IP_ADDRESS = 140, /* setWifiIpAddress */
    MTK_RADIO_REQ_SET_WFC_CONFIG = 141, /* setWfcConfig */
    MTK_RADIO_REQ_GET_WFC_CONFIG = 142, /* getWfcConfig */
    MTK_RADIO_REQ_QUERY_SSAC_STATUS = 143, /* querySsacStatus */
    MTK_RADIO_REQ_SET_LOCATION_INFO = 144, /* setLocationInfo */
    MTK_RADIO_REQ_SET_EMERGENCY_ADDRESS_ID = 145, /* setEmergencyAddressId */
    MTK_RADIO_REQ_SET_NATT_KEEP_ALIVE_STATUS = 146, /* setNattKeepAliveStatus */
    MTK_RADIO_REQ_SET_WIFI_PING_RESULT = 147, /* setWifiPingResult */
    MTK_RADIO_REQ_SET_APC_MODE = 148, /* setApcMode */
    MTK_RADIO_REQ_GET_APC_INFO = 149, /* getApcInfo */
    MTK_RADIO_REQ_SET_IMS_BEARER_NOTIFICATION = 150, /* setImsBearerNotification */
    MTK_RADIO_REQ_SET_IMS_CFG_FEATURE_VALUE = 151, /* setImsCfgFeatureValue */
    MTK_RADIO_REQ_GET_IMS_CFG_FEATURE_VALUE = 152, /* getImsCfgFeatureValue */
    MTK_RADIO_REQ_SET_IMS_CFG_PROVISION_VALUE = 153, /* setImsCfgProvisionValue */
    MTK_RADIO_REQ_GET_IMS_CFG_PROVISION_VALUE = 154, /* getImsCfgProvisionValue */
    MTK_RADIO_REQ_GET_IMS_CFG_RESOURCE_CAP_VALUE = 155, /* getImsCfgResourceCapValue */
    MTK_RADIO_REQ_DATA_CONNECTION_ATTACH = 156, /* dataConnectionAttach */
    MTK_RADIO_REQ_DATA_CONNECTION_DETACH = 157, /* dataConnectionDetach */
    MTK_RADIO_REQ_RESET_ALL_CONNECTIONS = 158, /* resetAllConnections */
    MTK_RADIO_REQ_SET_LTE_RELEASE_VERSION = 159, /* setLteReleaseVersion */
    MTK_RADIO_REQ_GET_LTE_RELEASE_VERSION = 160, /* getLteReleaseVersion */
    MTK_RADIO_REQ_SET_TX_POWER_STATUS = 161, /* setTxPowerStatus */
    MTK_RADIO_REQ_SET_SUPP_SERV_PROPERTY = 162, /* setSuppServProperty */
    MTK_RADIO_REQ_SUPPLY_DEVICE_NETWORK_DEPERSNALIZATION = 163, /* supplyDeviceNetworkDepersonalization */
    MTK_RADIO_REQ_NOTIFY_EPDG_SCREEN_STATE = 164, /* notifyEPDGScreenState */
    MTK_RADIO_REQ_HANGUP_WITH_REASON = 165, /* hangupWithReason */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_SUBSIDY_LOCK = 166, /* setResponseFunctionsSubsidyLock */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_RCS = 167, /* setResponseFunctionsRcs */
    MTK_RADIO_REQ_SEND_SUBSIDY_LOCK_REQUEST = 168, /* sendSubsidyLockRequest */
    MTK_RADIO_REQ_SET_VENDOR_SETTING = 169, /* setVendorSetting */
    MTK_RADIO_REQ_SET_RTT_MODE = 170, /* setRttMode */
    MTK_RADIO_REQ_SEND_RTT_MODIFY_REQUEST = 171, /* sendRttModifyRequest */
    MTK_RADIO_REQ_SEND_RTT_TEXT = 172, /* sendRttText */
    MTK_RADIO_REQ_RTT_MODIFY_REQUEST_RESPONSE = 173, /* rttModifyRequestResponse */
    MTK_RADIO_REQ_TOGGLE_RTT_AUDIO_INDICATION = 174, /* toggleRttAudioIndication */
    MTK_RADIO_REQ_QUERY_VOPS_STATUS = 175, /* queryVopsStatus */
    MTK_RADIO_REQ_NOTIFY_IMS_SERVICE_READY = 176, /* notifyImsServiceReady */
    MTK_RADIO_REQ_GET_PLMN_NAME_FROM_SE13_TABLE = 177, /* getPlmnNameFromSE13Table */
    MTK_RADIO_REQ_ENABLE_CA_PLUS_BAND_WIDTH_FILTER = 178, /* enableCAPlusBandWidthFilter */
    MTK_RADIO_REQ_GET_VOICE_DOMAIN_PREFERENCE = 179, /* getVoiceDomainPreference */
    MTK_RADIO_REQ_SET_SIP_HEADER = 180, /* setSipHeader */
    MTK_RADIO_REQ_SET_SIP_HEADER_REPORT = 181, /* setSipHeaderReport */
    MTK_RADIO_REQ_SET_IMS_CALL_MODE = 182, /* setImsCallMode */
    MTK_RADIO_REQ_SET_GWSD_MODE = 183, /* setGwsdMode */
    MTK_RADIO_REQ_SET_CALL_VALID_TIMER = 184, /* setCallValidTimer */
    MTK_RADIO_REQ_SET_IGNORE_SAME_NUMBER_INTERVAL = 185, /* setIgnoreSameNumberInterval */
    MTK_RADIO_REQ_SET_KEEP_ALIVE_BY_PDCPC_CTRL_PDU = 186, /* setKeepAliveByPDCPCtrlPDU */
    MTK_RADIO_REQ_SET_KEEP_ALIVE_BY_IP_DATA = 187, /* setKeepAliveByIpData */
    MTK_RADIO_REQ_ENABLE_DSDA_INDICATION = 188, /* enableDsdaIndication */
    MTK_RADIO_REQ_GET_DSDA_STATUS = 189, /* getDsdaStatus */
    MTK_RADIO_REQ_REGISTER_CELL_QLTY_REPORT = 190, /* registerCellQltyReport */
    MTK_RADIO_REQ_GET_SUGGESTED_PLMN_LIST = 191, /* getSuggestedPlmnList */
    MTK_RADIO_REQ_ROUTE_CERTIFICATE = 192, /* routeCertificate */
    MTK_RADIO_REQ_ROUTE_AUTH_MESSAGE = 193, /* routeAuthMessage */
    MTK_RADIO_REQ_ENABLE_CAPABILITY = 194, /* enableCapability */
    MTK_RADIO_REQ_ABORT_CERTIFICATE = 195, /* abortCertificate */
    MTK_RADIO_REQ_ECC_REDIAL_APPROVE = 196, /* eccRedialApprove */
    MTK_RADIO_REQ_DEACTIVATE_NR_SCG_COMMUNICATION = 197, /* deactivateNrScgCommunication */
    MTK_RADIO_REQ_GET_DEACTIVATE_NR_SCG_COMMUNICATION = 198, /* getDeactivateNrScgCommunication */
    MTK_RADIO_REQ_SET_MAX_UL_SPEED = 199, /* setMaxUlSpeed */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_SMART_RAT_SWITCH = 200, /* setResponseFunctionsSmartRatSwitch */
    MTK_RADIO_REQ_SMART_RAT_SWITCH = 201, /* smartRatSwitch */
    MTK_RADIO_REQ_GET_SMART_RAT_SWITCH = 202, /* getSmartRatSwitch */
    MTK_RADIO_REQ_SET_SMART_SCENE_SWITCH = 203, /* setSmartSceneSwitch */
    MTK_RADIO_REQ_SEND_SAR_INDICATOR = 204, /* sendSarIndicator */
    MTK_RADIO_REQ_SET_CALL_ADDITIONAL_INFO = 205, /* setCallAdditionalInfo */
    MTK_RADIO_REQ_SEND_RSU_REQUEST = 206, /* sendRsuRequest */
    MTK_RADIO_REQ_SEND_WIFI_ENABLED = 207, /* sendWifiEnabled */
    MTK_RADIO_REQ_SEND_WIFI_ASSOCIATED = 208, /* sendWifiAssociated */
    MTK_RADIO_REQ_SEND_WIFI_IP_ADDRESS = 209, /* sendWifiIpAddress */
    MTK_RADIO_REQ_VIDEO_RINGTONE_EVENT_REQUEST = 210, /* videoRingtoneEventRequest */
    MTK_RADIO_REQ_SET_NR_OPTION = 211, /* setNROption */
    MTK_RADIO_REQ_GET_TOE_INFO = 212, /* getTOEInfo */
    MTK_RADIO_REQ_DISABLE_ALL_CA_LINKS = 213, /* disableAllCALinks */
    MTK_RADIO_REQ_GET_CA_LINK_ENABLE_STATUS = 214, /* getCALinkEnableStatus */
    MTK_RADIO_REQ_SET_CA_LINK_ENABLE_STATUS = 215, /* setCALinkEnableStatus */
    MTK_RADIO_REQ_GET_CA_LINK_CAPABILITY_LIST = 216, /* getCALinkCapabilityList */
    MTK_RADIO_REQ_GET_LTE_DATA = 217, /* getLteData */
    MTK_RADIO_REQ_GET_LTE_RRC_STATE = 218, /* getLteRRCState */
    MTK_RADIO_REQ_SET_LTE_BAND_ENABLE_STATUS = 219, /* setLteBandEnableStatus */
    MTK_RADIO_REQ_GET_BAND_PRIORITY_LIST = 220, /* getBandPriorityList */
    MTK_RADIO_REQ_SET_BAND_PRIORITY_LIST = 221, /* setBandPriorityList */
    MTK_RADIO_REQ_SET_4X4_MIMO_ENABLED = 222, /* set4x4MimoEnabled */
    MTK_RADIO_REQ_GET_4X4_MIMO_ENABLED = 223, /* get4x4MimoEnabled */
    MTK_RADIO_REQ_SET_LTE_BSR_TIMER = 224, /* setLteBsrTimer */
    MTK_RADIO_REQ_GET_LTE_BSR_TIMER = 225, /* getLteBsrTimer */
    MTK_RADIO_REQ_GET_LTE_1XRTT_CELL_LIST = 226, /* getLte1xRttCellList */
    MTK_RADIO_REQ_CLEAR_LTE_AVAILABLE_FILE = 227, /* clearLteAvailableFile */
    MTK_RADIO_REQ_GET_BAND_MODE = 228, /* getBandMode */
    MTK_RADIO_REQ_GET_CA_BAND_MODE = 229, /* getCaBandMode */
    MTK_RADIO_REQ_GET_CAMPED_FEMTO_CELL_INFO = 230, /* getCampedFemtoCellInfo */
    MTK_RADIO_REQ_SET_QAM_ENABLED = 231, /* setQamEnabled */
    MTK_RADIO_REQ_GET_QAM_ENABLED = 232, /* getQamEnabled */
    MTK_RADIO_REQ_SET_TM9_ENABLED = 233, /* setTm9Enabled */
    MTK_RADIO_REQ_GET_TM9_ENABLED = 234, /* getTm9Enabled */
    MTK_RADIO_REQ_SET_LTE_SCAN_DURATION = 235, /* setLteScanDuration */
    MTK_RADIO_REQ_GET_LTE_SCAN_DURATION = 236, /* getLteScanDuration */
    MTK_RADIO_REQ_SET_DISABLE_2G = 237, /* setDisable2G */
    MTK_RADIO_REQ_GET_DISABLE_2G = 238, /* getDisable2G */
    MTK_RADIO_REQ_GET_ENGINEERING_MODE_INFO = 239, /* getEngineeringModeInfo */
    MTK_RADIO_REQ_GET_IWLAN_REGISTRATION_STATE = 240, /* getIWlanRegistrationState */
    MTK_RADIO_REQ_GET_ALL_BAND_MODE = 241, /* getAllBandMode */
    MTK_RADIO_REQ_SET_NR_BAND_MODE = 242, /* setNrBandMode */
} MTK_RADIO_REQ;

typedef enum ims_radio_resp {
    /* vendor.mediatek.hardware.mtkradioex@3.0::IImsRadioResponse */
#define MTK_RADIO_RESP_(req,resp,Name,NAME) IMS_RADIO_RESP_##NAME = resp,
    MTK_RADIO_EXT_IMS_CALL_3_0(MTK_RADIO_RESP_)
#undef MTK_RADIO_RESP_
} IMS_RADIO_RESP;

/* e(code, name, NAME) */
#define IMS_RADIO_INDICATION_3_0(e) \
    e(1, incomingCallIndication, INCOMING_CALL_INDICATION) \
    e(2, callInfoIndication, CALL_INFO_INDICATION) \
    e(3, econfResultIndication, ECONF_RESULT_INDICATION) \
    e(4, sipCallProgressIndicator, SIP_CALL_PROGRESS_INDICATOR) \
    e(5, callmodChangeIndicator, CALLMOD_CHANGE_INDICATOR) \
    e(6, videoCapabilityIndicator, VIDEO_CAPABILITY_INDICATOR) \
    e(7, onUssi, ON_USSI) \
    e(8, getProvisionDone, GET_PROVISION_DONE) \
    e(9, onXui, ON_XUI) \
    e(10, onVolteSubscription, ON_VOLTE_SUBSCRIPTION) \
    e(11, suppSvcNotify, SUPP_SVC_NOTIFY) \
    e(12, imsEventPackageIndication, IMS_EVENT_PACKAGE_INDICATION) \
    e(13, imsRegistrationInfo, IMS_REGISTRATION_INFO) \
    e(14, imsEnableDone, IMS_ENABLE_DONE) \
    e(15, imsDisableDone, IMS_DISABLE_DONE) \
    e(16, imsEnableStart, IMS_ENABLE_START) \
    e(17, imsDisableStart, IMS_DISABLE_START) \
    e(18, ectIndication, ECT_INDICATION) \
    e(19, volteSetting, VOLTE_SETTING) \
    e(20, imsBearerStateNotify, IMS_BEARER_STATE_NOTIFY) \
    e(21, imsBearerInit, IMS_BEARER_INIT) \
    e(22, imsDeregDone, IMS_DEREG_DONE) \
    e(23, imsSupportEcc, IMS_SUPPORT_ECC) \
    e(24, imsRadioInfoChange, IMS_RADIO_INFO_CHANGE) \
    e(25, speechCodecInfoIndication, SPEECH_CODEC_INFO_INDICATION) \
    e(26, imsConferenceInfoIndication, IMS_CONFERENCE_INFO_INDICATION) \
    e(27, lteMessageWaitingIndication, LTE_MESSAGE_WAITING_INDICATION) \
    e(28, imsDialogIndication, IMS_DIALOG_INDICATION) \
    e(29, imsCfgDynamicImsSwitchComplete, IMS_CFG_DYNAMIC_IMS_SWITCH_COMPLETE) \
    e(30, imsCfgFeatureChanged, IMS_CFG_FEATURE_CHANGED) \
    e(31, imsCfgConfigChanged, IMS_CFG_CONFIG_CHANGED) \
    e(32, imsCfgConfigLoaded, IMS_CFG_CONFIG_LOADED) \
    e(33, imsDataInfoNotify, IMS_DATA_INFO_NOTIFY) \
    e(34, newSmsEx, NEW_SMS_EX) \
    e(35, newSmsStatusReportEx, NEW_SMS_STATUS_REPORT_EX) \
    e(36, cdmaNewSmsEx, CDMA_NEW_SMS_EX) \
    e(37, noEmergencyCallbackMode, NO_EMERGENCY_CALLBACK_MODE) \
    e(38, imsRedialEmergencyIndication, IMS_REDIAL_EMERGENCY_INDICATION) \
    e(39, imsRtpInfo, IMS_RTP_INFO) \
    e(40, rttCapabilityIndication, RTT_CAPABILITY_INDICATION) \
    e(41, rttModifyResponse, RTT_MODIFY_RESPONSE) \
    e(42, rttTextReceive, RTT_TEXT_RECEIVE) \
    e(43, rttModifyRequestReceive,RTT_MODIFY_REQUEST_RECEIVE) \
    e(44, audioIndication, AUDIO_INDICATION) \
    e(45, sendVopsIndication, SEND_VOPS_INDICATION) \
    e(46, callAdditionalInfoInd, CALL_ADDITIONAL_INFO_IND) \
    e(47, sipHeaderReport, SIP_HEADER_REPORT) \
    e(48, callRatIndication, CALL_RAT_INDICATION) \
    e(49, sipRegInfoInd, SIP_REG_INFO_IND) \
    e(50, imsRegStatusReport, IMS_REG_STATUS_REPORT) \
    e(51, imsRegInfoInd, IMS_REG_INFO_IND) \
    e(52, onSsacStatus, ON_SSAC_STATUS) \
    e(53, eregrtInfoInd, EREG_RT_INFO_IND) \
    e(54, videoRingtoneEventInd, VIDEO_RINGTONE_EVENT_IND) \
    e(55, onMDInternetUsageInd, ON_MD_INTERNET_USAGE_IND) \
    e(56, imsRegFlagInd, IMS_REG_FLAG_IND)

typedef enum ims_radio_ind {
    /* vendor.mediatek.hardware.mtkradioex@3.0::IImsRadioIndication */
#define IMS_RADIO_IND_(code, name, NAME) IMS_RADIO_IND_##NAME = code,
    IMS_RADIO_INDICATION_3_0(IMS_RADIO_IND_)
#undef IMS_RADIO_IND_
} IMS_RADIO_IND;

/* ImsConfig.FeatureConstants in AOSP */
typedef enum ims_feature_type {
    FEATURE_TYPE_VOICE_OVER_LTE = 0,
    FEATURE_TYPE_VIDEO_OVER_LTE = 1,
    FEATURE_TYPE_VOICE_OVER_WIFI = 2,
    FEATURE_TYPE_VIDEO_OVER_WIFI = 3,
    FEATURE_TYPE_UT_OVER_LTE = 4,
    FEATURE_TYPE_UT_OVER_WIFI = 5,
} IMS_FEATURE_TYPE;

/* TelephonyManager.NETWORK_TYPES */
/* note: the numeric values are different from RADIO_TECH enum */
typedef enum network_type {
    NETWORK_TYPE_UNKNOWN = 0,
    NETWORK_TYPE_GPRS = 1,
    NETWORK_TYPE_EDGE = 2,
    NETWORK_TYPE_UMTS = 3,
    NETWORK_TYPE_CDMA = 4,
    NETWORK_TYPE_EVDO_0 = 5,
    NETWORK_TYPE_EVDO_A = 6,
    NETWORK_TYPE_1XRTT = 7,
    NETWORK_TYPE_HSDPA = 8,
    NETWORK_TYPE_HSUPA = 9,
    NETWORK_TYPE_HSPA = 10,
    NETWORK_TYPE_IDEN = 11,
    NETWORK_TYPE_EVDO_B = 12,
    NETWORK_TYPE_LTE = 13,
    NETWORK_TYPE_EHRPD = 14,
    NETWORK_TYPE_HSPAP = 15,
    NETWORK_TYPE_GSM = 16,
    NETWORK_TYPE_TD_SCDMA = 17,
    NETWORK_TYPE_IWLAN = 18,
    NETWORK_TYPE_LTE_CA = 19,
    NETWORK_TYPE_NR = 20,
} NETWORK_TYPE;

typedef enum multival_req_islast {
    ISLAST_NULL = -1,
    ISLAST_FALSE = 0,
    ISLAST_TRUE = 1,
} MULTIVAL_REQ_ISLAST;

enum incoming_call_indication_mode {
    IMS_ALLOW_INCOMING_CALL_INDICATION = 0,
    IMS_DISALLOW_INCOMING_CALL_INDICATION = 1,
};

typedef enum ims_reg_status_report_type {
    IMS_REGISTERING,
    IMS_REGISTERED,
    IMS_REGISTER_FAIL
} ImsRegStatusReportType;

typedef enum call_info_msg_type {
    CALL_INFO_MSG_TYPE_SETUP = 0,
    CALL_INFO_MSG_TYPE_ALERT = 2,
    CALL_INFO_MSG_TYPE_CONNECTED = 6,
    CALL_INFO_MSG_TYPE_MO_CALL_ID_ASSIGN = 130,
    CALL_INFO_MSG_TYPE_HELD = 131,
    CALL_INFO_MSG_TYPE_ACTIVE = 132,
    CALL_INFO_MSG_TYPE_DISCONNECTED = 133,
    CALL_INFO_MSG_TYPE_REMOTE_HOLD = 135,
    CALL_INFO_MSG_TYPE_REMOTE_RESUME = 136
} CallInfoMsgType;

typedef struct incoming_call_notification {
    GBinderHidlString callId RADIO_ALIGNED(8);
    GBinderHidlString number RADIO_ALIGNED(8);
    GBinderHidlString type RADIO_ALIGNED(8);
    GBinderHidlString callMode RADIO_ALIGNED(8);
    GBinderHidlString seqNo RADIO_ALIGNED(8);
    GBinderHidlString redirectNumber RADIO_ALIGNED(8);
    GBinderHidlString toNumber RADIO_ALIGNED(8);
} IncomingCallNotification;

typedef struct ims_reg_status_info {
    guint32 report_type RADIO_ALIGNED(4); /* ImsRegStatusReportType */
    guint32 account_id RADIO_ALIGNED(4);
    guint32 expire_time RADIO_ALIGNED(4);
    guint32 error_code RADIO_ALIGNED(4);
    GBinderHidlString uri RADIO_ALIGNED(8);
    GBinderHidlString error_msg RADIO_ALIGNED(8);
} ImsRegStatusInfo;

#endif /* MTK_RADIO_EXT_TYPES_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
