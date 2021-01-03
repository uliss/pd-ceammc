/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_connections.h"
#include "ceammc_factory.h"

#include <stdexcept>

HoaXlet::HoaXlet(const PdArgs& args)
    : BaseObject(args)
    , extra_(nullptr)
{
    extra_ = new IntProperty("@index", 1);
    extra_->setArgIndex(0);
    extra_->checkMinEq(1);
    addProperty(extra_);
}

HoaIn::HoaIn(const PdArgs& args)
    : HoaXlet(args)
{
    createOutlet();
}

void HoaIn::onBang()
{
    bangTo(0);
}

void HoaIn::onFloat(t_float v)
{
    floatTo(0, v);
}

void HoaIn::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
}

void HoaIn::onList(const AtomList& l)
{
    listTo(0, l);
}

void HoaIn::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(0, s, lv);
}

HoaIn* HoaIn::fromObject(void* obj)
{
    if (!isA(obj))
        return nullptr;

    return reinterpret_cast<ObjectFactory<HoaIn>::ObjectProxy*>(obj)->impl;
}

bool HoaIn::isA(void* obj)
{
    return obj != nullptr
        && ((t_object*)obj)->te_g.g_pd == ObjectFactory<HoaIn>::classPointer();
}

HoaOut::HoaOut(const PdArgs& args)
    : HoaXlet(args)
    , outlet_(nullptr)
{
}

void HoaOut::onBang()
{
    if (outlet_)
        outlet_bang(outlet_);
}

void HoaOut::onFloat(t_float v)
{
    if (outlet_)
        outlet_float(outlet_, v);
}

void HoaOut::onSymbol(t_symbol* s)
{
    if (outlet_)
        outlet_symbol(outlet_, s);
}

void HoaOut::onList(const AtomList& l)
{
    if (outlet_)
        outlet_list(outlet_, &s_list, l.size(), l.toPdData());
}

void HoaOut::onAny(t_symbol* s, const AtomListView& l)
{
    if (outlet_)
        outlet_anything(outlet_, s, l.size(), l.toPdData());
}

bool HoaOut::processAnyProps(t_symbol* sel, const AtomListView& lst)
{
    return false;
}

void HoaOut::setOutlet(t_outlet* x)
{
    outlet_ = x;
}

HoaOut* HoaOut::fromObject(void* obj)
{
    if (!isA(obj))
        return nullptr;

    return reinterpret_cast<ObjectFactory<HoaOut>::ObjectProxy*>(obj)->impl;
}

bool HoaOut::isA(void* obj)
{
    return obj != nullptr
        && ((t_object*)obj)->te_g.g_pd == ObjectFactory<HoaOut>::classPointer();
}

HoaXletTilde::HoaXletTilde(const PdArgs& args)
    : SoundExternal(args)
    , extra_(nullptr)
    , signal_(nullptr)
{
    extra_ = new IntProperty("@extra", 0);
    extra_->setArgIndex(0);
    extra_->checkMinEq(0);
    addProperty(extra_);
}

void HoaXletTilde::setSignal(t_sample* sig)
{
    signal_ = sig;
}

void HoaXletTilde::processBlock(const t_sample** in, t_sample** out)
{
}

HoaInTilde::HoaInTilde(const PdArgs& args)
    : HoaXletTilde(args)
{
    createSignalOutlet();
}

void HoaInTilde::setupDSP(t_signal** sp)
{
    if (signal_)
        dsp_add_copy(signal_, sp[0]->s_vec, sp[0]->s_n);
    else
        dsp_add_zero(sp[0]->s_vec, sp[0]->s_n);
}

HoaInTilde* HoaInTilde::fromObject(void* obj)
{
    if (!isA(obj))
        return nullptr;

    return reinterpret_cast<ObjectFactory<HoaInTilde>::ObjectProxy*>(obj)->impl;
}

bool HoaInTilde::isA(void* obj)
{
    return obj != nullptr
        && ((t_object*)obj)->te_g.g_pd == ObjectFactory<HoaInTilde>::classPointer();
}

HoaOutTilde::HoaOutTilde(const PdArgs& args)
    : HoaXletTilde(args)
{
}

void HoaOutTilde::setupDSP(t_signal** sp)
{
    if (signal_)
        dsp_add_plus(sp[0]->s_vec, signal_, signal_, sp[0]->s_n);
    else
        OBJ_LOG << "empty signal...";
}

HoaOutTilde* HoaOutTilde::fromObject(void* obj)
{
    if (!isA(obj))
        return nullptr;

    return reinterpret_cast<ObjectFactory<HoaOutTilde>::ObjectProxy*>(obj)->impl;
}

bool HoaOutTilde::isA(void* obj)
{
    return obj != nullptr
        && ((t_object*)obj)->te_g.g_pd == ObjectFactory<HoaOutTilde>::classPointer();
}

void setup_spat_hoa_connections()
{
    ObjectFactory<HoaIn> hoa_in("hoa.in", OBJECT_FACTORY_NO_DEFAULT_INLET);
    ObjectFactory<HoaOut> hoa_out("hoa.out");

    SoundExternalFactory<HoaInTilde> hoa_in_tilde("hoa.in~", OBJECT_FACTORY_NO_DEFAULT_INLET);
    SoundExternalFactory<HoaOutTilde> hoa_out_tilde("hoa.out~");
}
