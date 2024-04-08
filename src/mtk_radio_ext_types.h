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
    c(14, 7, setImsEnabled, SET_IMS_ENABLED) \
    c(151, 43, setImsCfgFeatureValue, SET_IMS_CFG_FEATURE_VALUE)

typedef enum mtk_radio_req {
    /* vendor.mediatek.hardware.mtkradioex@3.0::IMtkRadioExt */
    MTK_RADIO_REQ_RESPONSE_ACKNOWLEDGEMENT_MTK = 1, /* responseAcknowledgementMtk */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_MTK = 2, /* setResponseFunctionsMtk */
    MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_IMS = 3, /* setResponseFunctionsIms */
#define MTK_RADIO_REQ_(req,resp,Name,NAME) MTK_RADIO_REQ_##NAME = req,
    MTK_RADIO_EXT_IMS_CALL_3_0(MTK_RADIO_REQ_)
#undef MTK_RADIO_REQ_
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
    e(6, videoCapabilityIndication, VIDEO_CAPABILITY_INDICATOR) \
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

#endif /* MTK_RADIO_EXT_TYPES_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
