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

#include "nm_dbus.h"
#include <gio/gio.h>
#include <arpa/inet.h>

#define NM_DBUS_SERVICE "org.freedesktop.NetworkManager"
#define NM_DBUS_PATH "/org/freedesktop/NetworkManager"
#define NM_DBUS_INTERFACE "org.freedesktop.NetworkManager"
#define NM_DEVICE_INTERFACE "org.freedesktop.NetworkManager.Device"
#define NM_IP4CONFIG_INTERFACE "org.freedesktop.NetworkManager.IP4Config"

void
nm_info_free(
    NMInfo* nm_info)
{
    if (!nm_info) {
        return;
    }
    g_free(nm_info->ipv4_addr);
    g_free(nm_info->ipv4_gateway);
    g_free(nm_info->dns_servers);
    if (nm_info->iface_proxy) {
        g_object_unref(nm_info->iface_proxy);
    }
    if (nm_info->ip4config_proxy) {
        g_object_unref(nm_info->ip4config_proxy);
    }
    if (nm_info->connection) {
        g_object_unref(nm_info->connection);
    }
    memset(nm_info, 0, sizeof(NMInfo));
}

static
void
update_dns_servers(
    NMInfo* nm_info,
    GVariant* nameservers)
{
    if (nameservers) {
        GString* dns_string = g_string_new(NULL);
        gsize i, length;

        length = g_variant_n_children(nameservers);
        nm_info->dns_count = length;

        for (i = 0; i < length; i++) {
            GVariant* addr_variant = g_variant_get_child_value(nameservers, i);
            if (g_variant_is_of_type(addr_variant, G_VARIANT_TYPE_UINT32)) {
                guint32 addr = g_variant_get_uint32(addr_variant);
                char ip_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr, ip_str, INET_ADDRSTRLEN);
                g_string_append(dns_string, ip_str);
            }
            if (i < length - 1) {
                g_string_append(dns_string, ", ");
            }
            g_variant_unref(addr_variant);
        }

        g_free(nm_info->dns_servers);
        nm_info->dns_servers = g_string_free(dns_string, FALSE);
    }
}

static
void
update_ip4_info(
    NMInfo* nm_info,
    GDBusProxy* proxy)
{
    GVariant* addresses, * gateway, * nameservers;
    GVariantIter iter;
    GVariant* child;

    g_free(nm_info->ipv4_addr);
    g_free(nm_info->ipv4_gateway);
    g_free(nm_info->dns_servers);
    nm_info->ipv4_addr = NULL;
    nm_info->ipv4_gateway = NULL;
    nm_info->dns_servers = NULL;
    nm_info->ipv4_prefix_len = 0;
    nm_info->dns_count = 0;

    addresses = g_dbus_proxy_get_cached_property(proxy, "AddressData");
    if (addresses) {
        g_variant_iter_init(&iter, addresses);
        if ((child = g_variant_iter_next_value(&iter))) {
            GVariant* address_variant, * prefix_variant;

            address_variant = g_variant_lookup_value(child, "address", G_VARIANT_TYPE_STRING);
            if (address_variant) {
                nm_info->ipv4_addr = g_variant_dup_string(address_variant, NULL);
                g_variant_unref(address_variant);
            }

            prefix_variant = g_variant_lookup_value(child, "prefix", G_VARIANT_TYPE_UINT32);
            if (prefix_variant) {
                nm_info->ipv4_prefix_len = g_variant_get_uint32(prefix_variant);
                g_variant_unref(prefix_variant);
            }

            g_variant_unref(child);
        }
        g_variant_unref(addresses);
    }

    gateway = g_dbus_proxy_get_cached_property(proxy, "Gateway");
    if (gateway) {
        nm_info->ipv4_gateway = g_variant_dup_string(gateway, NULL);
        g_variant_unref(gateway);
    }

    nameservers = g_dbus_proxy_get_cached_property(proxy, "Nameservers");
    update_dns_servers(nm_info, nameservers);
    if (nameservers) {
        g_variant_unref(nameservers);
    }

    if (nm_info->callback) {
        nm_info->callback(nm_info->ipv4_addr, nm_info->ipv4_prefix_len, nm_info->ipv4_gateway, nm_info->dns_count, nm_info->dns_servers, nm_info->user_data);
    }
}

static
void
on_ip4config_properties_changed(
    GDBusProxy* proxy,
    GVariant* changed_properties,
    GStrv invalidated_properties,
    gpointer user_data)
{
    NMInfo* nm_info = (NMInfo*)user_data;
    GVariantIter iter;
    const gchar* key;
    GVariant* value;

    g_variant_iter_init(&iter, changed_properties);
    while (g_variant_iter_loop(&iter, "{&sv}", &key, &value)) {
        if (g_strcmp0(key, "AddressData") == 0 ||
            g_strcmp0(key, "Gateway") == 0 ||
            g_strcmp0(key, "Nameservers") == 0) {
            update_ip4_info(nm_info, proxy);
            break;
        }
    }
}

static
void
setup_ip4config_proxy(
    NMInfo* nm_info,
    const gchar* ip4config_path)
{
    GError* error = NULL;

    if (nm_info->ip4config_proxy)
        g_object_unref(nm_info->ip4config_proxy);

    nm_info->ip4config_proxy = g_dbus_proxy_new_sync(nm_info->connection,
                                            G_DBUS_PROXY_FLAGS_NONE,
                                            NULL,
                                            NM_DBUS_SERVICE,
                                            ip4config_path,
                                            NM_IP4CONFIG_INTERFACE,
                                            NULL,
                                            &error);
    if (error) {
        g_error_free(error);
    } else {
        g_signal_connect(nm_info->ip4config_proxy, "g-properties-changed",
                         G_CALLBACK(on_ip4config_properties_changed), nm_info);
        update_ip4_info(nm_info, nm_info->ip4config_proxy);
    }
}

static
void
on_interface_properties_changed(
    GDBusProxy* proxy,
    GVariant* changed_properties,
    GStrv invalidated_properties,
    gpointer user_data)
{
    NMInfo* nm_info = (NMInfo*)user_data;
    GVariantIter iter;
    const gchar* key;
    GVariant* value;

    g_variant_iter_init(&iter, changed_properties);
    while (g_variant_iter_loop(&iter, "{&sv}", &key, &value)) {
        if (g_strcmp0(key, "Ip4Config") == 0) {
            const gchar* ip4config_path;

            ip4config_path = g_variant_get_string(value, NULL);
            if (g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH)) {
                setup_ip4config_proxy(nm_info, ip4config_path);
            }
        }
    }
}

static
gboolean
find_iface(
    NMInfo* nm_info,
    const char* iface_name)
{
    GDBusProxy* nm_proxy;
    GError* error = NULL;
    GVariant* result;
    GVariantIter* iter;
    const gchar* object_path;
    gboolean found = FALSE;

    nm_proxy = g_dbus_proxy_new_sync(nm_info->connection,
                                     G_DBUS_PROXY_FLAGS_NONE,
                                     NULL,
                                     NM_DBUS_SERVICE,
                                     NM_DBUS_PATH,
                                     NM_DBUS_INTERFACE,
                                     NULL,
                                     &error);
    if (error) {
        g_error_free(error);
        return FALSE;
    }

    result = g_dbus_proxy_call_sync(nm_proxy,
                                    "GetAllDevices",
                                    NULL,
                                    G_DBUS_CALL_FLAGS_NONE,
                                    -1,
                                    NULL,
                                    &error);
    if (error) {
        g_error_free(error);
        g_object_unref(nm_proxy);
        return FALSE;
    }

    g_variant_get(result, "(ao)", &iter);
    while (g_variant_iter_loop(iter, "o", &object_path)) {
        GDBusProxy* device_proxy;

        device_proxy = g_dbus_proxy_new_sync(nm_info->connection,
                                             G_DBUS_PROXY_FLAGS_NONE,
                                             NULL,
                                             NM_DBUS_SERVICE,
                                             object_path,
                                             NM_DEVICE_INTERFACE,
                                             NULL,
                                             &error);
        if (error) {
            g_error_free(error);
            error = NULL;
            continue;
        }

        GVariant* interface = g_dbus_proxy_get_cached_property(device_proxy, "Interface");
        if (interface) {
            const gchar* iface = g_variant_get_string(interface, NULL);
            if (g_strcmp0(iface, iface_name) == 0) {
                nm_info->iface_proxy = device_proxy;
                g_signal_connect(nm_info->iface_proxy, "g-properties-changed",
                                 G_CALLBACK(on_interface_properties_changed), nm_info);

                GVariant* ip4config = g_dbus_proxy_get_cached_property(nm_info->iface_proxy, "Ip4Config");
                if (ip4config) {
                    const gchar* ip4config_path = g_variant_get_string(ip4config, NULL);
                    setup_ip4config_proxy(nm_info, ip4config_path);
                    g_variant_unref(ip4config);
                }
                found = TRUE;
            } else {
                g_object_unref(device_proxy);
            }
            g_variant_unref(interface);
        } else {
            g_object_unref(device_proxy);
        }

        if (found) {
            break;
        }
    }

    g_variant_iter_free(iter);
    g_variant_unref(result);
    g_object_unref(nm_proxy);

    return found;
}

gboolean
nm_initialize(
    NMInfo* nm_info,
    const char* iface_name,
    void (*callback)(const char* ipv4_addr, guint32 ipv4_prefix_len, const char* ipv4_gateway, guint32 dns_count, const char* dns_servers, void* user_data),
    void* user_data)
{
    GError* error = NULL;

    memset(nm_info, 0, sizeof(NMInfo));
    nm_info->connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (error) {
        g_error_free(error);
        return FALSE;
    }

    nm_info->callback = callback;
    nm_info->user_data = user_data;

    return find_iface(nm_info, iface_name);
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
