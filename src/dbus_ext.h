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

#ifndef DBUS_EXT_H
#define DBUS_EXT_H

#include <glib.h>

typedef struct mtk_radio_ext MtkRadioExt;

gboolean
dbus_ext_init(
  MtkRadioExt* self,
  const char* slot);

void
dbus_ext_cleanup(
    MtkRadioExt* self);

gboolean
dbus_ext_handle_atci_response(
    MtkRadioExt* self,
    guint32 serial,
    const void* response,
    gsize len);

typedef void (*AtCommandDispatchFunc)(MtkRadioExt* self, guint32 serial, const char* command);

void
dbus_ext_register_dispatch_func(
    MtkRadioExt* self,
    AtCommandDispatchFunc func);

#endif /* DBUS_EXT_H */

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
