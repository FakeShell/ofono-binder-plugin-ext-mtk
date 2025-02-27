/*
 *  oFono - Open Source Telephony - binder based adaptation MTK plugin
 *
 *  Copyright (C) 2025 Furi Labs
 *  Copyright (C) 2025 Jesus Higueras <jesus@furilabs.com>
 *  Copyright (C) 2025 Bardia Moshiri <bardia@furilabs.com>
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

#include <ofono/log.h>
#include <ofono/dbus.h>
#include <ofono/gdbus.h>
#include <ofono/modem.h>
#include <ofono/watch.h>

#include "dbus_ext.h"
#include "mtk_radio_ext.h"

#define AT_COMMAND_INTERFACE OFONO_SERVICE ".FuriLabs.AT"

typedef struct dbus_ext {
    MtkRadioExt* mtk_ext;
    struct ofono_watch* watch;
    GHashTable* atci_requests;
    int last_atci_serial;
    AtCommandDispatchFunc dispatch_func;
    char* ril_path;
} DBusExt;

/* Keep a global pointer to the D-Bus extension data */
static DBusExt* g_dbus_ext = NULL;

/* ofono does not expose ofono_modem's type definition, so we stub it
   to get just what we need. */
struct ofono_modem {
    const char *path;
};

static
DBusMessage*
dbus_ext_at_command_send(
    DBusConnection *conn,
    DBusMessage *msg,
    void *data)
{
    DBusExt *ext = data;
    const char *command;

    if (!ext || !ext->dispatch_func) {
        return dbus_message_new_error(msg, DBUS_ERROR_FAILED,
            "AT command interface not initialized");
    }

    int serial = ext->last_atci_serial++;
    if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING, &command, DBUS_TYPE_INVALID)) {
        return dbus_message_new_error(msg, DBUS_ERROR_INVALID_ARGS,
            "Failed to parse AT command");
    }

    g_hash_table_insert(ext->atci_requests, GINT_TO_POINTER(serial), dbus_message_ref(msg));
    ext->dispatch_func(ext->mtk_ext, serial, command);

    return NULL;
}

static const GDBusMethodTable at_command_methods[] = {
    { GDBUS_ASYNC_METHOD("SendCommand",
        GDBUS_ARGS({ "command", "s" }),
        GDBUS_ARGS({ "response", "s" }),
        dbus_ext_at_command_send) },
    { }
};

static const GDBusSignalTable at_command_signals[] = {
    { }
};

static
ofono_bool_t
dbus_ext_modem_find(
    struct ofono_modem* modem,
    void *user_data)
{
    DBusExt *ext = user_data;

    if (!modem || !ext || !ext->ril_path) {
        return FALSE;
    }

    if (g_str_equal(modem->path, ext->ril_path)) {
        DBusConnection *conn = ofono_dbus_get_connection();

        if (!g_dbus_register_interface(conn, ext->ril_path, AT_COMMAND_INTERFACE,
            at_command_methods, at_command_signals, NULL, ext, NULL)) {
            DBG("Failed to register AT command interface");
            return FALSE;
        }

        ofono_modem_add_interface(modem, AT_COMMAND_INTERFACE);
        return TRUE;
    }
    return FALSE;
}

static
void
dbus_ext_modem_watch(
    struct ofono_watch *watch,
    void *data)
{
    DBusExt* ext = data;

    if (ext && ext->ril_path) {
        ofono_modem_find(dbus_ext_modem_find, ext);
    }
}

gboolean
dbus_ext_init(
    MtkRadioExt* self,
    const char* slot)
{
    if (!self || !slot) {
        return FALSE;
    }

    g_dbus_ext = g_new0(DBusExt, 1);
    if (!g_dbus_ext) {
        return FALSE;
    }

    g_dbus_ext->mtk_ext = self;
    g_dbus_ext->atci_requests = g_hash_table_new_full(g_direct_hash, g_direct_equal,
        NULL, (GDestroyNotify)dbus_message_unref);

    if (g_str_has_prefix(slot, "imsSlot")) {
        gint slot_number = slot[strlen(slot) - 1] - '0';
        g_dbus_ext->ril_path = g_strdup_printf("/ril_%d", slot_number - 1);

        g_dbus_ext->watch = ofono_watch_new(g_dbus_ext->ril_path);
        ofono_watch_add_modem_changed_handler(g_dbus_ext->watch,
            dbus_ext_modem_watch, g_dbus_ext);

        ofono_modem_find(dbus_ext_modem_find, g_dbus_ext);
    } else {
        ofono_warn("Unexpected slot format: %s", slot);
        dbus_ext_cleanup(self);
        return FALSE;
    }

    return TRUE;
}

void
dbus_ext_cleanup(
    MtkRadioExt* self)
{
    if (g_dbus_ext) {
        if (g_dbus_ext->watch) {
            ofono_watch_unref(g_dbus_ext->watch);
        }

        if (g_dbus_ext->atci_requests) {
            g_hash_table_destroy(g_dbus_ext->atci_requests);
        }

        g_free(g_dbus_ext->ril_path);
        g_free(g_dbus_ext);
        g_dbus_ext = NULL;
    }
}

void
dbus_ext_register_dispatch_func(
    MtkRadioExt* self,
    AtCommandDispatchFunc func)
{
    if (g_dbus_ext) {
        g_dbus_ext->dispatch_func = func;
    }
}

gboolean
dbus_ext_handle_atci_response(
    MtkRadioExt* self,
    guint32 serial,
    const void* response_data,
    gsize len)
{
    if (!g_dbus_ext || !response_data) {
        return FALSE;
    }

    DBusConnection *conn = ofono_dbus_get_connection();
    DBusMessage *dbus_msg = g_hash_table_lookup(g_dbus_ext->atci_requests,
        GINT_TO_POINTER(serial));

    if (!dbus_msg) {
        DBG("No DBus message found for serial %d", serial);
        return FALSE;
    }

    char *response = g_malloc(len + 1);
    if (!response) {
        DBG("Failed to allocate memory for ATCI response data");
        return FALSE;
    }

    memcpy(response, response_data, len);
    response[len] = '\0';

    DBusMessage *reply = dbus_message_new_method_return(dbus_msg);
    if (!reply) {
        DBG("Failed to create DBus reply message");
        g_free(response);
        return FALSE;
    }

    if (!dbus_message_append_args(reply, DBUS_TYPE_STRING, &response, DBUS_TYPE_INVALID)) {
        DBG("Failed to append response to DBus reply message");
        dbus_message_unref(reply);
        g_free(response);
        return FALSE;
    }

    gboolean result = dbus_connection_send(conn, reply, NULL);
    if (!result) {
        DBG("Failed to send DBus reply message");
    }

    g_hash_table_remove(g_dbus_ext->atci_requests, GINT_TO_POINTER(serial));

    dbus_message_unref(reply);
    g_free(response);
    return result;
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
