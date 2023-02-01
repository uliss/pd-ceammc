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
#include "hoa_process_instance.h"
#include "ceammc_canvas.h"
#include "fmt/core.h"
#include "hoa_process.h"

#include <algorithm>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

void ProcessInstance::setOutlet(t_outlet* outl, size_t idx)
{
    for (auto& out : f_outs) {
        if (out->extra() == idx)
            out->setOutlet(outl);
    }
}

void ProcessInstance::setInletBuffer(t_sample* s, size_t idx)
{
    for (auto& in : f_ins_sig) {
        if (in->extra() == idx)
            in->setSignal(s);
    }
}

void ProcessInstance::setOutletBuffer(t_sample* s, size_t idx)
{
    for (auto& out : f_outs_sig) {
        if (out->extra() == idx)
            out->setSignal(s);
    }
}

void ProcessInstance::doScanCanvas(t_canvas* cnv)
{
    for (t_gobj* y = cnv->gl_list; y; y = y->g_next) {
        const t_symbol* name = y->g_pd->c_name;
        if (name == gensym(HOA_STR_CANVAS)) {
            // recursive load
            doScanCanvas((t_canvas*)y);
        } else if (HoaIn::isA(y)) {
            f_ins.emplace_front(HoaIn::fromObject(y));
        } else if (HoaOut::isA(y)) {
            f_outs.emplace_front(HoaOut::fromObject(y));
        } else if (HoaInTilde::isA(y)) {
            f_ins_sig.emplace_front(HoaInTilde::fromObject(y));
        } else if (HoaOutTilde::isA(y)) {
            f_outs_sig.emplace_front(HoaOutTilde::fromObject(y));
        }
    }
}

void ProcessInstance::createSwitch()
{
    if (!canvas_)
        return;

    Canvas cnv(canvas_);

    // find [switch~]
    auto sym_block = gensym(HOA_STR_BLOCK);
    switch_ = cnv.findIf([sym_block](t_object* x) { return pd::object_name(x) == sym_block; });
    if (switch_)
        return;

    // create [switch~] object on instance canvas
    cnv.createPdObject(10, 10, gensym(HOA_STR_SWITCH));

    // find [switch~] again
    switch_ = cnv.findIf([sym_block](t_object* x) { return pd::object_name(x) == sym_block; });

    if (!switch_) {
        LIB_ERR << "[hoa.process~] can't create [switch~] for instance";
        return;
    }

    dspOn(true);
}

void ProcessInstance::dspOn(bool state)
{
    if (switch_)
        dsp_state_ = state;
}

void ProcessInstance::dspCalc()
{
    if (switch_ && dsp_state_)
        pd::object_bang(switch_);
}

bool ProcessInstance::init(t_symbol* name, const AtomListView& args)
{
    pd_this->pd_newest = nullptr;
    typedmess(&pd_objectmaker, name, (int)args.size(), args.toPdData());
    if (!pd_this->pd_newest) {
        LIB_ERR << fmt::format("can't create subpatch '{}'", name->s_name);
        return false;
    }

    if (*pd_this->pd_newest != canvas_class) {
        LIB_ERR << fmt::format("can't init instance '{}' because it's not an abstraction", name->s_name);
        pd_free(pd_this->pd_newest);
        pd_this->pd_newest = nullptr;
        return false;
    }

    t_canvas* new_c = (t_canvas*)pd_this->pd_newest;
    pd_this->pd_newest = nullptr;

    new_c->gl_owner = 0;
    new_c->gl_isclone = 1;

    setCanvas(new_c);
    setArgs(args);
    scanCanvas();
    createSwitch();
    return true;
}

ProcessInstance::ProcessInstance()
    : canvas_(nullptr)
    , switch_(nullptr)
    , dsp_state_(true)
{
}

ProcessInstance::~ProcessInstance()
{
    canvas_.free();
}

void ProcessInstance::setCanvas(t_canvas* c)
{
    canvas_ = Canvas(c);
}

void ProcessInstance::loadBang()
{
    canvas_.loadBang();
}

void ProcessInstance::show()
{
    canvas_.show();
}

void ProcessInstance::scanCanvas()
{
    doScanCanvas(canvas_.pd_canvas());
}

void ProcessInstance::bangTo(size_t inlet_idx)
{
    for (auto& in : f_ins) {
        if (in->extra() == inlet_idx)
            in->onBang();
    }
}

void ProcessInstance::floatTo(size_t inlet_idx, t_float v)
{
    for (auto& in : f_ins) {
        if (in->extra() == inlet_idx)
            in->onFloat(v);
    }
}

void ProcessInstance::symbolTo(size_t inlet_idx, t_symbol* s)
{
    for (auto& in : f_ins) {
        if (in->extra() == inlet_idx)
            in->onSymbol(s);
    }
}

void ProcessInstance::listTo(size_t inlet_idx, const AtomList& l)
{
    for (auto& in : f_ins) {
        if (in->extra() == inlet_idx)
            in->onList(l);
    }
}

void ProcessInstance::anyTo(size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    for (auto& in : f_ins) {
        if (in->extra() == inlet_idx)
            in->onAny(s, l);
    }
}

bool ProcessInstance::hasStaticInputSignal() const
{
    auto fx = [](const HoaInTilde* in) { return in->extra() == 0; };
    return std::find_if(f_ins_sig.begin(), f_ins_sig.end(), fx) != f_ins_sig.end();
}

bool ProcessInstance::hasStaticOutputSignal() const
{
    auto fx = [](const HoaOutTilde* out) { return out->extra() == 0; };
    return std::find_if(f_outs_sig.begin(), f_outs_sig.end(), fx) != f_outs_sig.end();
}

size_t ProcessInstance::numExtraSignalInputs() const
{
    size_t index = 0;
    for (auto& in : f_ins_sig)
        index = std::max(index, (size_t)in->extra());

    return index;
}

size_t ProcessInstance::numExtraSignalOutputs() const
{
    size_t index = 0;
    for (auto& out : f_outs_sig)
        index = std::max(index, (size_t)out->extra());

    return index;
}

size_t ProcessInstance::numControlInputs() const
{
    size_t index = 0;
    for (auto& in : f_ins)
        index = std::max(index, (size_t)in->extra());

    return index;
}

size_t ProcessInstance::numControlOutputs() const
{
    size_t index = 0;
    for (auto& out : f_outs)
        index = std::max(index, (size_t)out->extra());

    return index;
}
