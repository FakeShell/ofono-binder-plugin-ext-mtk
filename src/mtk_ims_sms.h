/*
 *  oFono - Open Source Telephony - binder based adaptation MTK plugin
 *
 *  Copyright (C) 2024 TheKit <thekit@disroot.org>
 *  Copyright (C) 2024 Bardia Moshiri <bardia@furilabs.com>
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

#ifndef MTK_IMS_SMS_H
#define MTK_IMS_SMS_H

#include <binder_ext_sms.h>

typedef struct mtk_radio_ext MtkRadioExt;
typedef struct radio_client RadioClient;

BinderExtSms*
mtk_ims_sms_new(
    MtkRadioExt* radio_ext,
    RadioClient* ims_aosp_client)
    G_GNUC_INTERNAL;

#endif /* MTK_IMS_SMS_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
