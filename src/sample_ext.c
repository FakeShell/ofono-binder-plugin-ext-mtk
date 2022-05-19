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

#include "sample_ext.h"
#include "sample_slot.h"

#include <binder_ext_plugin_impl.h>

typedef struct sample_ext {
    BinderExtPlugin parent;
} SampleExt;

typedef BinderExtPluginClass SampleExtClass;

GType sample_ext_get_type() G_GNUC_INTERNAL;
G_DEFINE_TYPE(SampleExt, sample_ext, BINDER_EXT_TYPE_PLUGIN)

#define THIS_TYPE sample_ext_get_type()
#define THIS(obj) G_TYPE_CHECK_INSTANCE_CAST(obj, THIS_TYPE, SampleExt)

const char sample_plugin_name[] = "sample";

/*==========================================================================*
 * BinderExtPluginClass
 *==========================================================================*/

static
BinderExtSlot*
sample_ext_new_slot(
    BinderExtPlugin* plugin,
    RadioInstance* radio,
    GHashTable* params)
{
    return sample_slot_new(radio, params);
}

/*==========================================================================*
 * API
 *==========================================================================*/

BinderExtPlugin*
sample_ext_new()
{
    return g_object_new(THIS_TYPE, NULL);
}

/*==========================================================================*
 * Internals
 *==========================================================================*/

static
void
sample_ext_init(
    SampleExt* self)
{
}

static
void
sample_ext_class_init(
    SampleExtClass* klass)
{
    klass->plugin_name = sample_plugin_name;
    klass->new_slot = sample_ext_new_slot;
}

/*
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
