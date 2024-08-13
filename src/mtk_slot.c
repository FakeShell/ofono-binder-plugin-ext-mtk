/*
 * Copyright (C) 2022 Jolla Ltd.
 * Copyright (C) 2022 Slava Monich <slava.monich@jolla.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3. Neither the names of the copyright holders nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * any official policies, either expressed or implied.
 */

#include "mtk_slot.h"
#include "mtk_ims.h"
#include "mtk_ims_call.h"
#include "mtk_ims_sms.h"
#include "mtk_radio_ext.h"

#include <binder_ext_slot_impl.h>

#include <radio_client.h>
#include <radio_instance.h>

typedef BinderExtSlotClass MtkSlotClass;
typedef struct mtk_slot {
    BinderExtSlot parent;
    BinderExtIms* ims;
    BinderExtCall* ims_call;
    BinderExtSms* ims_sms;
    MtkRadioExt* radio_ext;
    RadioInstance* ims_aosp_instance;
    RadioClient* ims_aosp_client;
} MtkSlot;

GType mtk_slot_get_type() G_GNUC_INTERNAL;
G_DEFINE_TYPE(MtkSlot, mtk_slot, BINDER_EXT_TYPE_SLOT)

#define THIS_TYPE mtk_slot_get_type()
#define THIS(obj) G_TYPE_CHECK_INSTANCE_CAST(obj, THIS_TYPE, MtkSlot)
#define PARENT_CLASS mtk_slot_parent_class

static
void
mtk_slot_terminate(
    MtkSlot* self)
{
    if (self->ims) {
        binder_ext_ims_unref(self->ims);
        self->ims = NULL;
    }
}

/*==========================================================================*
 * BinderExtSlot
 *==========================================================================*/

static
gpointer
mtk_slot_get_interface(
    BinderExtSlot* slot,
    GType iface)
{
    MtkSlot* self = THIS(slot);

    if (iface == BINDER_EXT_TYPE_IMS) {
        return self->ims;
    } else if (iface == BINDER_EXT_TYPE_CALL) {
        return self->ims_call;
    } else if (iface == BINDER_EXT_TYPE_SMS) {
        return self->ims_sms;
    } else {
        return BINDER_EXT_SLOT_CLASS(PARENT_CLASS)->get_interface(slot, iface);
    }
}

static
void
mtk_slot_shutdown(
    BinderExtSlot* slot)
{
    mtk_slot_terminate(THIS(slot));
    BINDER_EXT_SLOT_CLASS(PARENT_CLASS)->shutdown(slot);
}

/*==========================================================================*
 * API
 *==========================================================================*/

BinderExtSlot*
mtk_slot_new(
    RadioInstance* radio,
    GHashTable* params)
{
    MtkSlot* self = g_object_new(THIS_TYPE, NULL);
    BinderExtSlot* slot = &self->parent;
    char* slot_name =  g_strdup_printf("imsSlot%d", radio->slot_index + 1);
    char* aosp_slot_name =  g_strdup_printf("imsAospSlot%d", radio->slot_index + 1);

    self->radio_ext = mtk_radio_ext_new(radio->dev, slot_name);

    // Connect to imsAospSlotN on the IRadio interface
    self->ims_aosp_instance = radio_instance_new_with_modem_slot_and_version(
        radio->dev, aosp_slot_name, radio->modem, radio->slot_index, radio->version);

    self->ims_aosp_client = radio_client_new(self->ims_aosp_instance);

    /* Not getting rilConnected indication on imsAospSlot, so force it */
    self->ims_aosp_instance->connected = TRUE;
    radio_instance_set_enabled(self->ims_aosp_instance, TRUE);

    if (self->radio_ext) {
        self->ims = mtk_ims_new(slot_name, self->radio_ext);
        self->ims_call = mtk_ims_call_new(self->radio_ext, self->ims_aosp_client);
        self->ims_sms = mtk_ims_sms_new(self->radio_ext, self->ims_aosp_client);
    }

    return slot;
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

static
void
mtk_slot_finalize(
    GObject* object)
{
    mtk_slot_terminate(THIS(object));
    G_OBJECT_CLASS(PARENT_CLASS)->finalize(object);
}

static
void
mtk_slot_init(
    MtkSlot* self)
{
}

static
void
mtk_slot_class_init(
    MtkSlotClass* klass)
{
    klass->get_interface = mtk_slot_get_interface;
    klass->shutdown = mtk_slot_shutdown;
    G_OBJECT_CLASS(klass)->finalize = mtk_slot_finalize;
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
