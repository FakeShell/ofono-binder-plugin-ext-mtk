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

#ifndef MTK_RADIO_EXT_H
#define MTK_RADIO_EXT_H

#include <radio_types.h>

typedef struct mtk_radio_ext MtkRadioExt;
typedef enum call_info_msg_type CallInfoMsgType;

typedef void (*MtkRadioExtResultFunc)(
    MtkRadioExt* radio,
    int result,
    void* user_data);

typedef void (*MtkRadioExtImsRegStatusFunc)(
    MtkRadioExt* radio,
    guint status,
    void* user_data);

typedef void (*MtkRadioExtCallInfoFunc)(
    MtkRadioExt* radio,
    guint call_id,
    CallInfoMsgType msg_type,
    guint call_mode,
    char* number,
    void* user_data);

MtkRadioExt*
mtk_radio_ext_new(
    const char* dev,
    const char* slot);

MtkRadioExt*
mtk_radio_ext_ref(
    MtkRadioExt* self);

void
mtk_radio_ext_unref(
    MtkRadioExt* self);

void
mtk_radio_ext_cancel(
    MtkRadioExt* self,
    guint id);

guint
mtk_radio_ext_set_enabled(
    MtkRadioExt* self,
    gboolean enabled,
    MtkRadioExtResultFunc complete,
    GDestroyNotify destroy,
    void* user_data);

guint
mtk_radio_ext_set_ims_cfg_feature_value(
    MtkRadioExt* self,
    guint32 feature_id,
    guint32 network,
    guint32 value,
    guint32 is_last,
    MtkRadioExtResultFunc complete,
    GDestroyNotify destroy,
    void* user_data);

guint
mtk_radio_ext_set_ims_cfg(
    MtkRadioExt* self,
    gboolean volte_enable,
    gboolean vilte_enable,
    gboolean vowifi_enable,
    gboolean viwifi_enable,
    gboolean sms_enable,
    gboolean eims_enable,
    MtkRadioExtResultFunc complete,
    GDestroyNotify destroy,
    void* user_data);

gulong
mtk_radio_ext_add_ims_reg_status_handler(
    MtkRadioExt* self,
    MtkRadioExtImsRegStatusFunc handler,
    void* user_data);

gulong
mtk_radio_ext_add_call_info_handler(
    MtkRadioExt* self,
    MtkRadioExtCallInfoFunc handler,
    void* user_data);

#endif /* MTK_RADIO_EXT_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
