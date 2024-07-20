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

#include "mtk_ims.h"
#include "mtk_ims_call.h"
#include "mtk_slot.h"
#include "mtk_radio_ext.h"
#include "mtk_radio_ext_types.h"

#include <binder_ext_ims_impl.h>

#include <ofono/log.h>
#include <gbinder.h>

#include <gutil_macros.h>

typedef GObjectClass MtkImsClass;
typedef struct mtk_ims {
    GObject parent;
    char* slot;
    MtkRadioExt* radio_ext;
    BINDER_EXT_IMS_STATE ims_state;
} MtkIms;

static
void
mtk_ims_iface_init(
    BinderExtImsInterface* iface);

GType mtk_ims_get_type() G_GNUC_INTERNAL;
G_DEFINE_TYPE_WITH_CODE(MtkIms, mtk_ims, G_TYPE_OBJECT,
G_IMPLEMENT_INTERFACE(BINDER_EXT_TYPE_IMS, mtk_ims_iface_init))

#define THIS_TYPE mtk_ims_get_type()
#define THIS(obj) G_TYPE_CHECK_INSTANCE_CAST(obj, THIS_TYPE, MtkIms)
#define PARENT_CLASS mtk_ims_parent_class

enum mtk_ims_signal {
    SIGNAL_STATE_CHANGED,
    SIGNAL_COUNT
};

#define SIGNAL_STATE_CHANGED_NAME    "mtk-ims-state-changed"

static guint mtk_ims_signals[SIGNAL_COUNT] = { 0 };

typedef struct mtk_ims_result_request {
    BinderExtIms* ext;
    BinderExtImsResultFunc complete;
    GDestroyNotify destroy;
    void* user_data;
} MtkImsResultRequest;

static
MtkImsResultRequest*
mtk_ims_result_request_new(
    BinderExtIms* ext,
    BinderExtImsResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    MtkImsResultRequest* req = g_slice_new(MtkImsResultRequest);

    req->ext = binder_ext_ims_ref(ext);
    req->complete = complete;
    req->destroy = destroy;
    req->user_data = user_data;
    return req;
}

static
void
mtk_ims_result_request_free(
    MtkImsResultRequest* req)
{
    binder_ext_ims_unref(req->ext);
    gutil_slice_free(req);
}

static
void
mtk_ims_result_request_complete(
    MtkRadioExt* radio_ext,
    int result,
    void* user_data)
{
    MtkImsResultRequest* req = user_data;

    req->complete(req->ext, result ? BINDER_EXT_IMS_RESULT_ERROR :
        BINDER_EXT_IMS_RESULT_OK, req->user_data);
}

static
void
mtk_ims_result_request_destroy(
    gpointer user_data)
{
    MtkImsResultRequest* req = user_data;

    if (req->destroy) {
        req->destroy(req->user_data);
    }
    mtk_ims_result_request_free(req);
}

static
void
mtk_ims_reg_status_changed(
    MtkRadioExt* radio,
    ImsRegStatusReportType status,
    void* user_data)
{
    MtkIms* self = THIS(user_data);
    BINDER_EXT_IMS_STATE ims_state;

    switch (status) {
    case IMS_REGISTERING:
        ims_state = BINDER_EXT_IMS_STATE_REGISTERING;
        break;
    case IMS_REGISTERED:
        ims_state = BINDER_EXT_IMS_STATE_REGISTERED;
        break;
    case IMS_REGISTER_FAIL:
        ims_state = BINDER_EXT_IMS_STATE_NOT_REGISTERED;
        break;
    default:
        ims_state = BINDER_EXT_IMS_STATE_UNKNOWN;
    }

    if (ims_state != self->ims_state) {
        self->ims_state = ims_state;
        g_signal_emit(self, mtk_ims_signals[SIGNAL_STATE_CHANGED], 0);
    }
}

/*==========================================================================*
 * BinderExtImsInterface
 *==========================================================================*/

static
BINDER_EXT_IMS_STATE
mtk_ims_get_state(
    BinderExtIms* ext)
{
    MtkIms* self = THIS(ext);

    DBG("%s ims_state=%d", self->slot, self->ims_state);
    return self->ims_state;
}

static
guint
mtk_ims_set_registration(
    BinderExtIms* ext,
    BINDER_EXT_IMS_REGISTRATION registration,
    BinderExtImsResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    MtkIms* self = THIS(ext);

    DBG("%s", self->slot);
    const gboolean enabled = (registration != BINDER_EXT_IMS_REGISTRATION_OFF);

    mtk_radio_ext_set_ims_cfg_feature_value(self->radio_ext,
        FEATURE_TYPE_VOICE_OVER_LTE, NETWORK_TYPE_LTE, enabled, ISLAST_NULL,
        NULL, NULL, NULL);

    mtk_radio_ext_set_ims_cfg(self->radio_ext,
        enabled /* volteEnable */, enabled /* vilteEnable */, enabled /* vowifiEnable */,
        enabled /* viwifiEnable */, enabled /* smsEnable */, enabled /* eimsEnable */,
        NULL, NULL, NULL);

    MtkImsResultRequest* req = mtk_ims_result_request_new(ext,
        complete, destroy, user_data);
    guint id = mtk_radio_ext_set_enabled(self->radio_ext,
        enabled,
        complete ? mtk_ims_result_request_complete : NULL,
        mtk_ims_result_request_destroy, req);

    if (id) {
        return id;
    } else {
        mtk_ims_result_request_free(req);
        return 0;
    }
}

static
void
mtk_ims_cancel(
    BinderExtIms* ext,
    guint id)
{
    MtkIms* self = THIS(ext);

    /*
     * Cancel a pending operation identified by the id returned by the
     * above mtk_ims_set_registration() call.
     */
    DBG("%s %u", self->slot, id);
}

static
gulong
mtk_ims_add_state_handler(
    BinderExtIms* ext,
    BinderExtImsFunc handler,
    void* user_data)
{
    MtkIms* self = THIS(ext);

    DBG("%s", self->slot);
    return G_LIKELY(handler) ? g_signal_connect(self,
        SIGNAL_STATE_CHANGED_NAME, G_CALLBACK(handler), user_data) : 0;
}

static
void
mtk_ims_iface_init(
    BinderExtImsInterface* iface)
{
    iface->version = BINDER_EXT_IMS_INTERFACE_VERSION;
    iface->get_state = mtk_ims_get_state;
    iface->set_registration = mtk_ims_set_registration;
    iface->cancel = mtk_ims_cancel;
    iface->add_state_handler = mtk_ims_add_state_handler;
}

/*==========================================================================*
 * API
 *==========================================================================*/

BinderExtIms*
mtk_ims_new(
    const char* slot,
    MtkRadioExt* radio_ext)
{
    MtkIms* self = g_object_new(THIS_TYPE, NULL);

    /*
     * This could be the place to register a listener that gets invoked
     * on registration state change and emits SIGNAL_STATE_CHANGED.
     */
    self->slot = g_strdup(slot);
    self->radio_ext = mtk_radio_ext_ref(radio_ext);
    self->ims_state = BINDER_EXT_IMS_STATE_NOT_REGISTERED;

    if (self->radio_ext) {
        mtk_radio_ext_add_ims_reg_status_handler(self->radio_ext,
            mtk_ims_reg_status_changed, self);
    }

    return BINDER_EXT_IMS(self);
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

static
void
mtk_ims_finalize(
    GObject* object)
{
    MtkIms* self = THIS(object);

    g_free(self->slot);
    mtk_radio_ext_unref(self->radio_ext);

    G_OBJECT_CLASS(PARENT_CLASS)->finalize(object);
}

static
void
mtk_ims_init(
    MtkIms* self)
{
}

static
void
mtk_ims_class_init(
    MtkImsClass* klass)
{
    G_OBJECT_CLASS(klass)->finalize = mtk_ims_finalize;
    mtk_ims_signals[SIGNAL_STATE_CHANGED] =
        g_signal_new(SIGNAL_STATE_CHANGED_NAME, G_OBJECT_CLASS_TYPE(klass),
            G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
