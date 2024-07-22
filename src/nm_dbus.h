/*
 * Copyright (C) 2024 Furi Labs
 * Copyright (C) 2024 Bardia Moshiri <bardia@furilabs.com>
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
