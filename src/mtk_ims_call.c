/*
 *  oFono - Open Source Telephony - binder based adaptation MTK plugin
 *
 *  Copyright (C) 2022 Jolla Ltd.
 *  Copyright (C) 2024 TheKit <thekit@disroot.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 */

#include <glib-object.h>

#include "mtk_ims_call.h"
#include "mtk_radio_ext.h"
#include "mtk_radio_ext_types.h"

#include <binder_ext_call_impl.h>

#include <ofono/log.h>

#include <gutil_idlepool.h>
#include <gutil_macros.h>
#include <gutil_misc.h>

typedef GObjectClass MtkImsCallClass;
typedef struct mtk_ims_call {
    GObject parent;
    GUtilIdlePool* pool;
    MtkRadioExt* radio_ext;
    GPtrArray* calls;
    GHashTable* id_map;
} MtkImsCall;

static
void
mtk_ims_call_iface_init(
    BinderExtCallInterface* iface);

GType mtk_ims_call_get_type() G_GNUC_INTERNAL;
G_DEFINE_TYPE_WITH_CODE(MtkImsCall, mtk_ims_call, G_TYPE_OBJECT,
G_IMPLEMENT_INTERFACE(BINDER_EXT_TYPE_CALL, mtk_ims_call_iface_init))

#define THIS_TYPE mtk_ims_call_get_type()
#define THIS(obj) G_TYPE_CHECK_INSTANCE_CAST(obj, THIS_TYPE, MtkImsCall)
#define PARENT_CLASS mtk_ims_call_parent_class

#define ID_KEY(id) GUINT_TO_POINTER(id)
#define ID_VALUE(id) GUINT_TO_POINTER(id)

typedef struct mtk_ims_call_result_request {
    int ref_count;
    guint id;
    guint id_mapped;
    guint param;
    BinderExtCall* ext;
    BinderExtCallResultFunc complete;
    GDestroyNotify destroy;
    void* user_data;
} MtkImsCallResultRequest;

enum mtk_ims_call_signal {
    SIGNAL_CALL_STATE_CHANGED,
    SIGNAL_CALL_DISCONNECTED,
    SIGNAL_CALL_RING,
    SIGNAL_CALL_SUPP_SVC_NOTIFY,
    SIGNAL_COUNT
};

#define SIGNAL_CALL_STATE_CHANGED_NAME    "mtk-ims-call-state-changed"
#define SIGNAL_CALL_DISCONNECTED_NAME     "mtk-ims-call-disconnected"
#define SIGNAL_CALL_RING_NAME             "mtk-ims-call-ring"
#define SIGNAL_CALL_SUPP_SVC_NOTIFY_NAME  "mtk-ims-call-supp-svc-notify"

static guint mtk_ims_call_signals[SIGNAL_COUNT] = { 0 };

static
MtkImsCallResultRequest*
mtk_ims_call_result_request_new(
    BinderExtCall* ext,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    MtkImsCallResultRequest* req =
        g_slice_new0(MtkImsCallResultRequest);

    req->ref_count = 1;
    req->ext = binder_ext_call_ref(ext);
    req->complete = complete;
    req->destroy = destroy;
    req->user_data = user_data;
    return req;
}

static
void
mtk_ims_call_result_request_free(
    MtkImsCallResultRequest* req)
{
    BinderExtCall* ext = req->ext;

    if (req->destroy) {
        req->destroy(req->user_data);
    }
    if (req->id_mapped) {
        g_hash_table_remove(THIS(ext)->id_map, ID_KEY(req->id_mapped));
    }
    binder_ext_call_unref(ext);
    gutil_slice_free(req);
}

static
gboolean
mtk_ims_call_result_request_unref(
    MtkImsCallResultRequest* req)
{
    if (!--(req->ref_count)) {
        mtk_ims_call_result_request_free(req);
        return TRUE;
    } else {
        return FALSE;
    }
}

static
void
mtk_ims_call_result_request_destroy(
    gpointer req)
{
    mtk_ims_call_result_request_unref(req);
}

/*==========================================================================*
 * BinderExtCallInterface
 *==========================================================================*/

static
const BinderExtCallInfo* const*
mtk_ims_call_get_calls(
    BinderExtCall* ext)
{
    static const BinderExtCallInfo* none = NULL;
    MtkImsCall* self = THIS(ext);

    return self->calls->len ? (const BinderExtCallInfo**)self->calls->pdata : &none;
}

static
guint
mtk_ims_call_dial(
    BinderExtCall* ext,
    const char* number,
    BINDER_EXT_TOA toa,
    BINDER_EXT_CALL_CLIR clir,
    BINDER_EXT_CALL_DIAL_FLAGS flags,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    DBG("dial is not implemented yet");
    return 0;
}

static
guint
mtk_ims_call_answer(
    BinderExtCall* ext,
    BINDER_EXT_CALL_ANSWER_FLAGS flags,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    DBG("answer is not implemented yet");
    return 0;
}

static
guint
mtk_ims_call_swap(
    BinderExtCall* ext,
    BINDER_EXT_CALL_SWAP_FLAGS swap_flags,
    BINDER_EXT_CALL_ANSWER_FLAGS answer_flags,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    DBG("swap is not implemented yet");
    return 0;
}

static
guint
mtk_ims_call_hangup(
    BinderExtCall* ext,
    guint call_id,
    BINDER_EXT_CALL_HANGUP_REASON reason,
    BINDER_EXT_CALL_HANGUP_FLAGS flags,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    DBG("hangup is not implemented yet");
    return 0;
}

static
guint
mtk_ims_call_conference(
    BinderExtCall* ext,
    BINDER_EXT_CALL_CONFERENCE_FLAGS flags,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    DBG("conference is not implemented yet");
    return 0;
}

static
guint
mtk_ims_call_send_dtmf(
    BinderExtCall* ext,
    const char* tones,
    BinderExtCallResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    DBG("send_dtmf is not implemented yet");
    return 0;
}

static
void
mtk_ims_call_cancel(
    BinderExtCall* ext,
    guint id)
{
    MtkImsCall* self = THIS(ext);
    const guint mapped = GPOINTER_TO_UINT(g_hash_table_lookup(self->id_map,
        ID_KEY(id)));

    mtk_radio_ext_cancel(self->radio_ext, mapped ? mapped : id);
}

static
gulong
mtk_ims_call_add_calls_changed_handler(
    BinderExtCall* ext,
    BinderExtCallFunc cb,
    void* user_data)
{
    return G_LIKELY(cb) ? g_signal_connect(THIS(ext),
        SIGNAL_CALL_STATE_CHANGED_NAME, G_CALLBACK(cb), user_data) : 0;
}

static
gulong
mtk_ims_call_add_disconnect_handler(
    BinderExtCall* ext,
    BinderExtCallDisconnectFunc cb,
    void* user_data)
{
    return G_LIKELY(cb) ? g_signal_connect(THIS(ext),
        SIGNAL_CALL_DISCONNECTED_NAME, G_CALLBACK(cb), user_data) : 0;
}

static
gulong
mtk_ims_call_add_ring_handler(
    BinderExtCall* ext,
    BinderExtCallFunc cb,
    void* user_data)
{
    return G_LIKELY(cb) ? g_signal_connect(THIS(ext),
        SIGNAL_CALL_RING_NAME, G_CALLBACK(cb), user_data) : 0;
}

static
gulong
mtk_ims_call_add_ssn_handler(
    BinderExtCall* ext,
    BinderExtCallSuppSvcNotifyFunc cb,
    void* user_data)
{
    return G_LIKELY(cb) ? g_signal_connect(THIS(ext),
        SIGNAL_CALL_SUPP_SVC_NOTIFY_NAME, G_CALLBACK(cb), user_data) : 0;
}

void
mtk_ims_call_iface_init(
    BinderExtCallInterface* iface)
{
    iface->flags |= BINDER_EXT_CALL_INTERFACE_FLAG_IMS_SUPPORT |
        BINDER_EXT_CALL_INTERFACE_FLAG_IMS_REQUIRED;
    iface->version = BINDER_EXT_CALL_INTERFACE_VERSION;
    iface->get_calls = mtk_ims_call_get_calls;
    iface->dial = mtk_ims_call_dial;
    iface->answer = mtk_ims_call_answer;
    iface->swap = mtk_ims_call_swap;
    iface->conference = mtk_ims_call_conference;
    iface->send_dtmf = mtk_ims_call_send_dtmf;
    iface->hangup = mtk_ims_call_hangup;
    iface->cancel = mtk_ims_call_cancel;
    iface->add_calls_changed_handler =
        mtk_ims_call_add_calls_changed_handler;
    iface->add_disconnect_handler = mtk_ims_call_add_disconnect_handler;
    iface->add_ring_handler = mtk_ims_call_add_ring_handler;
    iface->add_ssn_handler = mtk_ims_call_add_ssn_handler;
}

/*==========================================================================*
 * API
 *==========================================================================*/

BinderExtCall*
mtk_ims_call_new(
    MtkRadioExt* radio_ext)
{
    if (G_LIKELY(radio_ext)) {
        MtkImsCall* self = g_object_new(THIS_TYPE, NULL);

        self->radio_ext = mtk_radio_ext_ref(radio_ext);
        self->calls = g_ptr_array_new_with_free_func(g_free);

        return BINDER_EXT_CALL(self);
    }
    return NULL;
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

static
void
mtk_ims_call_finalize(
    GObject* object)
{
    MtkImsCall* self = THIS(object);

    mtk_radio_ext_unref(self->radio_ext);
    gutil_idle_pool_destroy(self->pool);
    gutil_ptrv_free((void**)self->calls);
    g_hash_table_unref(self->id_map);
    G_OBJECT_CLASS(PARENT_CLASS)->finalize(object);
}

static
void
mtk_ims_call_init(
    MtkImsCall* self)
{
    self->pool = gutil_idle_pool_new();
    self->id_map = g_hash_table_new(g_direct_hash, g_direct_equal);
}

static
void
mtk_ims_call_class_init(
    MtkImsCallClass* klass)
{
    GType type = G_OBJECT_CLASS_TYPE(klass);

    G_OBJECT_CLASS(klass)->finalize = mtk_ims_call_finalize;
    mtk_ims_call_signals[SIGNAL_CALL_STATE_CHANGED] =
        g_signal_new(SIGNAL_CALL_STATE_CHANGED_NAME, type,
            G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
    mtk_ims_call_signals[SIGNAL_CALL_DISCONNECTED] =
        g_signal_new(SIGNAL_CALL_DISCONNECTED_NAME, type,
            G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE,
            2, G_TYPE_INT, G_TYPE_INT);
    mtk_ims_call_signals[SIGNAL_CALL_RING] =
        g_signal_new(SIGNAL_CALL_RING_NAME, type, G_SIGNAL_RUN_FIRST, 0,
            NULL, NULL, NULL, G_TYPE_NONE, 0);
    mtk_ims_call_signals[SIGNAL_CALL_SUPP_SVC_NOTIFY] =
        g_signal_new(SIGNAL_CALL_SUPP_SVC_NOTIFY_NAME, type,
            G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE,
            1, G_TYPE_POINTER);
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
