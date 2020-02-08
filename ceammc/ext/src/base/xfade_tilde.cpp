/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "xfade_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

static t_symbol* SYM_POW;
static t_symbol* SYM_LIN;

static const int DEFAULT_INLETS = 2;
static const t_float DEFAULT_SMOOTH_MS = 20;
static const int MIN_INLETS = 2;
static const int MAX_INLETS = 16;

static int maxInlets(const PdArgs& args)
{
    if (args.flags & XFADE_STEREO)
        return MAX_INLETS / 2;
    else
        return MAX_INLETS;
}

XFadeTilde::XFadeTilde(const PdArgs& args)
    : SoundExternal(args)
    , n_(clip<int>(positionalFloatArgument(0, DEFAULT_INLETS), MIN_INLETS, maxInlets(args)))
    , smooth_ms_(DEFAULT_SMOOTH_MS)
    , prop_type_(nullptr)
{
    for (size_t i = 1; i < n_; i++)
        createSignalInlet();

    if (args.flags & XFADE_STEREO) {
        for (size_t i = 0; i < n_; i++)
            createSignalInlet();
    }

    createInlet();

    createSignalOutlet();
    if (args.flags & XFADE_STEREO)
        createSignalOutlet();

    prop_type_ = new SymbolEnumProperty("@type", SYM_POW);
    prop_type_->appendEnum(SYM_LIN);
    createProperty(prop_type_);

    createProperty(new SymbolEnumAlias("@pow", prop_type_, SYM_POW));
    createProperty(new SymbolEnumAlias("@lin", prop_type_, SYM_LIN));

    {
        auto p = createCbProperty("@smooth", &XFadeTilde::propSmooth, &XFadeTilde::propSetSmooth);
        p->info().setType(PropertyInfoType::FLOAT);
        p->info().setDefault(DEFAULT_SMOOTH_MS);
        p->info().setMin(1);
        p->info().setUnits(PropertyInfoUnits::MSEC);
    }

    gain_.assign(n_, t_smooth(0));
    gain_[0].setTargetValue(1);
}

void XFadeTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    const double SR = samplerate();

    for (size_t i = 0; i < n_; i++)
        gain_[i].setDurationMs(smooth_ms_, SR);
}

void XFadeTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        t_sample v = 0;
        for (size_t j = 0; j < n_; j++) {
            t_sample s = gain_[j]() * in[j][i];
            v += s;
        }

        out[0][i] = v;
    }
}

void XFadeTilde::onInlet(size_t n, const AtomList& lst)
{
    typedef std::function<t_float(t_float, t_float)> XFadeCurveFunction;
    typedef std::pair<t_symbol*, XFadeCurveFunction> XFadeCurvePair;

    static XFadeCurvePair xfade_fns[] = {
        { SYM_LIN, [](t_float a, t_float b) { return a - b; } },
        { SYM_POW, [](t_float a, t_float b) { return (a - b) * (a - b); } }
    };

    if (!checkArgs(lst, ARG_FLOAT)) {
        OBJ_ERR << "float expected: " << lst;
        return;
    }

    t_float v = lst.floatAt(0, 0);

    auto type = prop_type_->value();
    auto fn_it = std::find_if(std::begin(xfade_fns), std::end(xfade_fns),
        [type](const XFadeCurvePair& p) { return p.first == type; });

    if (fn_it == std::end(xfade_fns)) {
        OBJ_ERR << "unknown xfade type: " << type;
        return;
    }

    for (size_t i = 0; i < n_; i++) {
        if (v >= i && v < (i + 1) && (i + 1) < n_) {
            t_float k = fn_it->second(v, i);
            gain_[i].setTargetValue(1 - k);
            gain_[i + 1].setTargetValue(k);
            i++;
        } else if (v >= i && (i + 1) == n_) {
            gain_[i].setTargetValue(1);
        } else {
            gain_[i].setTargetValue(0);
        }
    }
}

AtomList XFadeTilde::propSmooth() const
{
    return Atom(smooth_ms_);
}

void XFadeTilde::propSetSmooth(const AtomList& ms)
{
    smooth_ms_ = std::max<t_float>(1, ms.floatAt(0, DEFAULT_SMOOTH_MS));
}

std::vector<float> XFadeTilde::gains() const
{
    std::vector<float> res;

    for (auto& i : gain_)
        res.push_back(i.target());

    return res;
}

void setup_base_xfade_tilde()
{
    SYM_POW = gensym("pow");
    SYM_LIN = gensym("lin");

    SoundExternalFactory<XFadeTilde> obj("xfade~");
}
