/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "mul_samplerate.h"
#include "ceammc_factory.h"

extern "C" {
#include "m_imp.h"
}

static t_symbol* SYM_SAMPLERATE = gensym("samplerate~");
typedef t_object* (*t_newgimme)(t_symbol* s, int argc, t_atom* argv);

void sr_proxy_float(SamplerateProxy* x, t_float v)
{
    x->owner->setSamplerate(v);
}

MulToSamplerate::MulToSamplerate(const PdArgs& a)
    : BaseObject(a)
    , samplerate_(0)
    , proxy_(0)
    , sr_(0)
{
    createOutlet();

    proxy_ = SamplerateProxy::proxy_new();
    proxy_->owner = this;

    t_methodentry* m = pd_objectmaker->c_methods;
    for (int i = 0; i < pd_objectmaker->c_nmethod; i++) {
        if (m[i].me_name == SYM_SAMPLERATE) {
            samplerate_ = (*((t_newgimme)(m[i].me_fun)))(SYM_SAMPLERATE, 0, 0);
            if (obj_ninlets(samplerate_) > 0)
                obj_connect(samplerate_, 0, &proxy_->obj, 0);
        }
    }
}

MulToSamplerate::~MulToSamplerate()
{
    if (samplerate_)
        pd_free(&samplerate_->te_g.g_pd);

    if (proxy_)
        proxy_->free();
}

void MulToSamplerate::onFloat(t_float v)
{
    if (samplerate_) {
        pd_bang(&samplerate_->te_g.g_pd);
    } else
        sr_ = sys_getsr();

    floatTo(0, v * sr_);
}

void MulToSamplerate::setSamplerate(t_float v)
{
    sr_ = v;
}

extern "C" void setup_0x2aSR()
{
    ObjectFactory<MulToSamplerate> obj("*SR");
    SamplerateProxy::class_init("sr_proxy", sr_proxy_float);
}
