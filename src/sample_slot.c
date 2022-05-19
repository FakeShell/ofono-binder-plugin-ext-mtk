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

#include "sample_slot.h"
#include "sample_ims.h"

#include <binder_ext_slot_impl.h>

#include <radio_instance.h>

typedef BinderExtSlotClass SampleSlotClass;
typedef struct sample_slot {
    BinderExtSlot parent;
    BinderExtIms* ims;
} SampleSlot;

GType sample_slot_get_type() G_GNUC_INTERNAL;
G_DEFINE_TYPE(SampleSlot, sample_slot, BINDER_EXT_TYPE_SLOT)

#define THIS_TYPE sample_slot_get_type()
#define THIS(obj) G_TYPE_CHECK_INSTANCE_CAST(obj, THIS_TYPE, SampleSlot)
#define PARENT_CLASS sample_slot_parent_class

static
void
sample_slot_terminate(
    SampleSlot* self)
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
sample_slot_get_interface(
    BinderExtSlot* slot,
    GType iface)
{
    SampleSlot* self = THIS(slot);

    if (iface == BINDER_EXT_TYPE_IMS) {
        return self->ims;
    } else {
        return BINDER_EXT_SLOT_CLASS(PARENT_CLASS)->get_interface(slot, iface);
    }
}

static
void
sample_slot_shutdown(
    BinderExtSlot* slot)
{
    sample_slot_terminate(THIS(slot));
    BINDER_EXT_SLOT_CLASS(PARENT_CLASS)->shutdown(slot);
}

/*==========================================================================*
 * API
 *==========================================================================*/

BinderExtSlot*
sample_slot_new(
    RadioInstance* radio,
    GHashTable* params)
{
    SampleSlot* self = g_object_new(THIS_TYPE, NULL);
    BinderExtSlot* slot = &self->parent;

    self->ims = sample_ims_new(radio->slot);
    return slot;
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

static
void
sample_slot_finalize(
    GObject* object)
{
    sample_slot_terminate(THIS(object));
    G_OBJECT_CLASS(PARENT_CLASS)->finalize(object);
}

static
void
sample_slot_init(
    SampleSlot* self)
{
}

static
void
sample_slot_class_init(
    SampleSlotClass* klass)
{
    klass->get_interface = sample_slot_get_interface;
    klass->shutdown = sample_slot_shutdown;
    G_OBJECT_CLASS(klass)->finalize = sample_slot_finalize;
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
