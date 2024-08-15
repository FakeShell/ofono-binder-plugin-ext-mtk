/*
 *  oFono - Open Source Telephony - binder based adaptation MTK plugin
 *
 *  Copyright (C) 2024 Furi Labs
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

#ifndef NM_DBUS_H
#define NM_DBUS_H

#include <gio/gio.h>

typedef struct {
    GDBusConnection* connection;
    GDBusProxy* iface_proxy;
    GDBusProxy* ip4config_proxy;
    char* ipv4_addr;
    guint32 ipv4_prefix_len;
    char* ipv4_gateway;
    guint32 dns_count;
    char* dns_servers;
    void (*callback)(const char* ipv4_addr, guint32 ipv4_prefix_len, const char* ipv4_gateway, guint32 dns_count, const char* dns_servers, void* user_data);
    void* user_data;
} NMInfo;

gboolean
nm_initialize(
    NMInfo* nm_info,
    const char* iface_name,
    void (*callback)(const char* ipv4_addr, guint32 ipv4_prefix_len, const char* ipv4_gateway, guint32 dns_count, const char* dns_servers, void* user_data),
    void* user_data);

void
nm_info_free(
    NMInfo* nm_info);

#endif /* NM_DBUS_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
