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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <glib-object.h>

#include "mtk_radio_ext.h"
#include "mtk_radio_ext_types.h"

#include <ofono/log.h>
#include <gbinder.h>

#include <gutil_idlepool.h>
#include <gutil_log.h>
#include <gutil_macros.h>

#define MTK_RADIO_CALL_TIMEOUT (3*1000) /* ms */

typedef GObjectClass MtkRadioExtClass;
typedef struct mtk_radio_ext {
    GObject parent;
    char* slot;
    GBinderClient* client;
    GBinderRemoteObject* remote;
    GBinderLocalObject* response;
    GBinderLocalObject* indication;
    GUtilIdlePool* pool;
    GHashTable* requests;
} MtkRadioExt;

GType mtk_radio_ext_get_type() G_GNUC_INTERNAL;
G_DEFINE_TYPE(MtkRadioExt, mtk_radio_ext, G_TYPE_OBJECT)

#define THIS_TYPE mtk_radio_ext_get_type()
#define THIS(obj) G_TYPE_CHECK_INSTANCE_CAST(obj, THIS_TYPE, MtkRadioExt)
#define PARENT_CLASS mtk_radio_ext_parent_class
#define KEY(serial) GUINT_TO_POINTER(serial)

typedef struct mtk_radio_ext_request MtkRadioExtRequest;

typedef void (*MtkRadioExtArgWriteFunc)(
    GBinderWriter* args,
    va_list va);

typedef void (*MtkRadioExtRequestHandlerFunc)(
    MtkRadioExtRequest* req,
    const GBinderReader* args);

typedef void (*MtkRadioExtResultFunc)(
    MtkRadioExt* radio,
    int result,
    void* user_data);

struct mtk_radio_ext_request {
    guint id;  /* request id */
    gulong tx; /* binder transaction id */
    MtkRadioExt* radio;
    gint32 response_code;
    MtkRadioExtRequestHandlerFunc handle_response;
    void (*free)(MtkRadioExtRequest* req);
    GDestroyNotify destroy;
    void* user_data;
};

typedef struct mtk_radio_ext_result_request {
    MtkRadioExtRequest base;
    MtkRadioExtResultFunc complete;
} MtkRadioExtResultRequest;

static GLogModule mtk_radio_ext_binder_log_module = {
    .max_level = GLOG_LEVEL_VERBOSE,
    .level = GLOG_LEVEL_VERBOSE,
    .flags = GLOG_FLAG_HIDE_NAME
};

static GLogModule mtk_radio_ext_binder_dump_module = {
    .parent = &mtk_radio_ext_binder_log_module,
    .max_level = GLOG_LEVEL_VERBOSE,
    .level = GLOG_LEVEL_INHERIT,
    .flags = GLOG_FLAG_HIDE_NAME
};

static
void
mtk_radio_ext_log_notify(
    struct ofono_debug_desc* desc)
{
    mtk_radio_ext_binder_log_module.level = (desc->flags &
        OFONO_DEBUG_FLAG_PRINT) ? GLOG_LEVEL_VERBOSE : GLOG_LEVEL_INHERIT;
}

static
void
mtk_radio_ext_dump_notify(
    struct ofono_debug_desc* desc)
{
    mtk_radio_ext_binder_dump_module.level = (desc->flags &
        OFONO_DEBUG_FLAG_PRINT) ? GLOG_LEVEL_VERBOSE : GLOG_LEVEL_INHERIT;
}

static struct ofono_debug_desc logger_trace OFONO_DEBUG_ATTR = {
    .name = "mtk_binder_trace",
    .flags = OFONO_DEBUG_FLAG_DEFAULT | OFONO_DEBUG_FLAG_HIDE_NAME,
    .notify = mtk_radio_ext_log_notify
};

static struct ofono_debug_desc logger_dump OFONO_DEBUG_ATTR = {
    .name = "mtk_binder_dump",
    .flags = OFONO_DEBUG_FLAG_DEFAULT | OFONO_DEBUG_FLAG_HIDE_NAME,
    .notify = mtk_radio_ext_dump_notify
};

static
guint
mtk_radio_ext_new_req_id()
{
    static guint last_id = 0;
    return last_id++;
}

static
void
mtk_radio_ext_log_req(
    MtkRadioExt* self,
    guint32 code,
    guint32 serial)
{
    static const GLogModule* log = &mtk_radio_ext_binder_log_module;
    const int level = GLOG_LEVEL_VERBOSE;
    const char* name;

    if (!gutil_log_enabled(log, level))
        return;

    name = mtk_radio_ext_req_name(code);

    if (serial) {
        gutil_log(log, level, "%s< [%08x] %u %s",
            self->slot, serial, code, name ? name : "???");
    } else {
        gutil_log(log, level, "%s< %u %s",
            self->slot, code, name ? name : "???");
    }
}

void
mtk_radio_ext_log_resp(
    MtkRadioExt* self,
    guint32 code,
    guint32 serial)
{
    static const GLogModule* log = &mtk_radio_ext_binder_log_module;
    const int level = GLOG_LEVEL_VERBOSE;
    const char* name;

    if (!gutil_log_enabled(log, level))
        return;

    name = mtk_radio_ext_resp_name(code);

    gutil_log(log, level, "%s> [%08x] %u %s",
        self->slot, serial, code, name ? name : "???");
}

static
void
mtk_radio_ext_log_ind(
    MtkRadioExt* self,
    guint32 code)
{
    static const GLogModule* log = &mtk_radio_ext_binder_log_module;
    const int level = GLOG_LEVEL_VERBOSE;
    const char* name;

    if (!gutil_log_enabled(log, level))
        return;

    name = mtk_radio_ext_ind_name(code);

    gutil_log(log, level, "%s > %u %s", self->slot, code,
        name ? name : "???");
}

static
void
mtk_radio_ext_dump_data(
    const GBinderReader* reader)
{
    static const GLogModule* log = &mtk_radio_ext_binder_dump_module;
    const int level = GLOG_LEVEL_VERBOSE;
    gsize size;
    const guint8* data;

    if (!gutil_log_enabled(log, level))
        return;

    data = gbinder_reader_get_data(reader, &size);
    gutil_log_dump(log, level, "  ",  data, size);
}

static
void
mtk_radio_ext_dump_request(
    GBinderLocalRequest* args)
{
    static const GLogModule* log = &mtk_radio_ext_binder_dump_module;
    const int level = GLOG_LEVEL_VERBOSE;
    GBinderWriter writer;
    const guint8* data;
    gsize size;

    if (!gutil_log_enabled(log, level))
        return;

    /* Use writer API to fetch the raw data */
    gbinder_local_request_init_writer(args, &writer);
    data = gbinder_writer_get_data(&writer, &size);
    gutil_log_dump(log, level, "  ", data, size);
}

static
GBinderLocalReply*
mtk_radio_ext_indication(
    GBinderLocalObject* obj,
    GBinderRemoteRequest* req,
    guint code,
    guint flags,
    int* status,
    void* user_data)
{
    MtkRadioExt* self = THIS(user_data);
    const char* iface = gbinder_remote_request_interface(req);
    GBinderReader args;

    gbinder_remote_request_init_reader(req, &args);
    mtk_radio_ext_log_ind(self, code);
    mtk_radio_ext_dump_data(&args);

#pragma message("TODO: implement indication handling")
    DBG("Unexpected indication %s %u", iface, code);
    *status = GBINDER_STATUS_FAILED;

    return NULL;
}

static
GBinderLocalReply*
mtk_radio_ext_response(
    GBinderLocalObject* obj,
    GBinderRemoteRequest* req,
    guint code,
    guint flags,
    int* status,
    void* user_data)
{
    MtkRadioExt* self = THIS(user_data);
    const char* iface = gbinder_remote_request_interface(req);
    GBinderReader reader;
    guint32 serial = 0;

    gbinder_remote_request_init_reader(req, &reader);

    gbinder_reader_read_uint32(&reader, &serial);
    mtk_radio_ext_log_resp(self, code, serial);
    mtk_radio_ext_dump_data(&reader);

    if (serial) {
        MtkRadioExtRequest* req = g_hash_table_lookup(self->requests,
            KEY(serial));

        if (req && req->response_code == code) {
            g_object_ref(self);
            if (req->handle_response) {
                req->handle_response(req, &reader);
            }
            g_hash_table_remove(self->requests, KEY(serial));
            g_object_unref(self);
        } else {
            DBG("Unexpected response %s %u", iface, code);
            *status = GBINDER_STATUS_FAILED;
        }
    }

    return NULL;
}

static
void
mtk_radio_ext_result_response(
    MtkRadioExtRequest* req,
    const GBinderReader* args)
{
    gint32 result;
    GBinderReader reader;
    MtkRadioExt* self = req->radio;
    MtkRadioExtResultRequest* result_req = G_CAST(req,
        MtkRadioExtResultRequest, base);

    gbinder_reader_copy(&reader, args);
    if (!gbinder_reader_read_int32(&reader, &result)) {
        ofono_warn("Failed to parse response");
        result = -1;
    }
    if (result_req->complete) {
        result_req->complete(self, result, req->user_data);
    }
}

static
void
mtk_radio_ext_request_default_free(
    MtkRadioExtRequest* req)
{
    if (req->destroy) {
        req->destroy(req->user_data);
    }
    g_free(req);
}

static
void
mtk_radio_ext_request_destroy(
    gpointer user_data)
{
    MtkRadioExtRequest* req = user_data;

    gbinder_client_cancel(req->radio->client, req->tx);
    req->free(req);
}

static
gpointer
mtk_radio_ext_request_alloc(
    MtkRadioExt* self,
    gint32 resp,
    MtkRadioExtRequestHandlerFunc handler,
    GDestroyNotify destroy,
    void* user_data,
    gsize size)
{
    MtkRadioExtRequest* req = g_malloc0(size);

    req->radio = self;
    req->response_code = resp;
    req->handle_response = handler;
    req->id = mtk_radio_ext_new_req_id(self);
    req->free = mtk_radio_ext_request_default_free;
    req->destroy = destroy;
    req->user_data = user_data;
    g_hash_table_insert(self->requests, KEY(req->id), req);
    return req;
}

static
MtkRadioExtResultRequest*
mtk_radio_ext_result_request_new(
    MtkRadioExt* self,
    gint32 resp,
    MtkRadioExtResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    MtkRadioExtResultRequest* req =
        (MtkRadioExtResultRequest*)mtk_radio_ext_request_alloc(self, resp,
            mtk_radio_ext_result_response, destroy, user_data,
            sizeof(MtkRadioExtResultRequest));

    req->complete = complete;
    return req;
}

static
void
mtk_radio_ext_request_sent(
    GBinderClient* client,
    GBinderRemoteReply* reply,
    int status,
    void* user_data)
{
    ((MtkRadioExtRequest*)user_data)->tx = 0;
}

static
gulong
mtk_radio_ext_call(
    MtkRadioExt* self,
    gint32 code,
    gint32 serial,
    GBinderLocalRequest* req,
    GBinderClientReplyFunc reply,
    GDestroyNotify destroy,
    void* user_data)
{
    mtk_radio_ext_log_req(self, code, serial);
    mtk_radio_ext_dump_request(req);

    return gbinder_client_transact(self->client, code,
        GBINDER_TX_FLAG_ONEWAY, req, reply, destroy, user_data);
}

static
gulong
mtk_radio_ext_submit_request(
    MtkRadioExtRequest* request,
    gint32 code,
    gint32 serial,
    GBinderLocalRequest* args)
{
    return (request->tx = mtk_radio_ext_call(request->radio,
        code, serial, args, mtk_radio_ext_request_sent, NULL, request));
}

static
guint
mtk_radio_ext_result_request_submit(
    MtkRadioExt* self,
    gint32 req_code,
    gint32 resp_code,
    MtkRadioExtArgWriteFunc write_args,
    MtkRadioExtResultFunc complete,
    GDestroyNotify destroy,
    void* user_data,
    ...)
{
    if (G_LIKELY(self)) {
        GBinderLocalRequest* args;
        GBinderWriter writer;
        MtkRadioExtResultRequest* req =
            mtk_radio_ext_result_request_new(self, resp_code,
                complete, destroy, user_data);
        const guint req_id = req->base.id;

        args = gbinder_client_new_request2(self->client, req_code);
        gbinder_local_request_init_writer(args, &writer);
        gbinder_writer_append_int32(&writer, req_id);
        if (write_args) {
            va_list va;

            va_start(va, user_data);
            write_args(&writer, va);
            va_end(va);
        }

        /* Submit the request */
        mtk_radio_ext_submit_request(&req->base, req_code, req_id, args);
        gbinder_local_request_unref(args);
        if (req->base.tx) {
            /* Success */
            return req_id;
        }
        g_hash_table_remove(self->requests, KEY(req_id));
    }
    return 0;
}

static
MtkRadioExt*
mtk_radio_ext_create(
    GBinderServiceManager* sm,
    GBinderRemoteObject* remote,
    const char* slot)
{
    MtkRadioExt* self = g_object_new(THIS_TYPE, NULL);
    const gint code = MTK_RADIO_REQ_SET_RESPONSE_FUNCTIONS_IMS;
    GBinderLocalRequest* req;
    GBinderWriter writer;
    int status;

    self->slot = g_strdup(slot);
    self->client = gbinder_client_new(remote, MTK_RADIO);
    self->response = gbinder_servicemanager_new_local_object(sm,
        MTK_RADIO_RESPONSE, mtk_radio_ext_response, self);
    self->indication = gbinder_servicemanager_new_local_object(sm,
        MTK_RADIO_INDICATION, mtk_radio_ext_indication, self);

    /* IMtkRadioEx:setResponseFunctionsIms */
    req = gbinder_client_new_request2(self->client, code);
    gbinder_local_request_init_writer(req, &writer);
    gbinder_writer_append_local_object(&writer, self->response);
    gbinder_writer_append_local_object(&writer, self->indication);

    mtk_radio_ext_log_req(self, code, 0 /*serial*/);
    mtk_radio_ext_dump_request(req);
    gbinder_remote_reply_unref(gbinder_client_transact_sync_reply(self->client,
        code, req, &status));

    DBG("setResponseFunctionsIms status %d", status);
    gbinder_local_request_unref(req);
    return self;
}

/*==========================================================================*
 * API
 *==========================================================================*/

MtkRadioExt*
mtk_radio_ext_new(
    const char* dev,
    const char* slot)
{
    MtkRadioExt* self = NULL;

    GBinderServiceManager* sm = gbinder_servicemanager_new(dev);
    if (sm) {
        char* fqname = g_strconcat(MTK_RADIO, "/", slot, NULL);
        GBinderRemoteObject* obj = /* autoreleased */
            gbinder_servicemanager_get_service_sync(sm, fqname, NULL);

        if (obj) {
            DBG("Connected to %s", fqname);
            self = mtk_radio_ext_create(sm, obj, slot);
        }

        g_free(fqname);
        gbinder_servicemanager_unref(sm);
    }

    return self;
}

MtkRadioExt*
mtk_radio_ext_ref(
    MtkRadioExt* self)
{
    if (G_LIKELY(self)) {
        g_object_ref(self);
    }
    return self;
}

void
mtk_radio_ext_unref(
    MtkRadioExt* self)
{
    if (G_LIKELY(self)) {
        g_object_unref(self);
    }
}

static
void
mtk_radio_ext_set_enabled_args(
    GBinderWriter* args,
    va_list va)
{
    gbinder_writer_append_bool(args, va_arg(va, gboolean));
}

guint
mtk_radio_ext_set_enabled(
    MtkRadioExt* self,
    gboolean enabled,
    MtkRadioExtResultFunc complete,
    GDestroyNotify destroy,
    void* user_data)
{
    return mtk_radio_ext_result_request_submit(self,
        MTK_RADIO_REQ_SET_IMS_ENABLED,
        IMS_RADIO_RESP_SET_IMS_ENABLED,
        mtk_radio_ext_set_enabled_args,
        complete, destroy, user_data,
        enabled);
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

static
void
mtk_radio_ext_finalize(
    GObject* object)
{
    MtkRadioExt* self = THIS(object);

    g_free(self->slot);
    G_OBJECT_CLASS(PARENT_CLASS)->finalize(object);
}

static
void
mtk_radio_ext_init(
    MtkRadioExt* self)
{
    self->pool = gutil_idle_pool_new();
    self->requests = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL,
        mtk_radio_ext_request_destroy);
}

static
void
mtk_radio_ext_class_init(
    MtkRadioExtClass* klass)
{
    G_OBJECT_CLASS(klass)->finalize = mtk_radio_ext_finalize;
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
