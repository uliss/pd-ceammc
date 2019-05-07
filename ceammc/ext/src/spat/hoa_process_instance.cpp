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

ProcessInstance::ProcessInstance()
    : canvas_(nullptr)
{
}

void ProcessInstance::setCanvas(t_canvas* c)
{
    canvas_ = c;
}

void ProcessInstance::loadBang()
{
    if (canvas_)
        canvas_loadbang(canvas_);
}

void ProcessInstance::show()
{
    if (canvas_)
        canvas_vis(canvas_, 1);
}

void ProcessInstance::scanCanvas(t_canvas* cnv)
{
    for (t_gobj* y = cnv->gl_list; y; y = y->g_next) {
        const t_symbol* name = y->g_pd->c_name;
        if (name == HoaProcess::SYM_CANVAS) {
            // recursive load
            scanCanvas((t_canvas*)y);
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
