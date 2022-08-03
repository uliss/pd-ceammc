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
#include "ceammc_property_callback.h"

#include <algorithm>

static t_symbol* SYM_POW;
static t_symbol* SYM_LIN;

constexpr t_float DEFAULT_SMOOTH_MS = 20;
constexpr size_t DEF_NCHAN = 2;
constexpr size_t MIN_NCHAN = 2;
constexpr size_t MAX_NCHAN = 16;

static size_t inMultiple(const PdArgs& args)
{
    if (args.flags & XFADE_STEREO)
        return 2;
    else
        return 1;
}

XFadeTilde::XFadeTilde(const PdArgs& args)
    : SoundExternal(args)
    , smooth_ms_(DEFAULT_SMOOTH_MS)
    , prop_type_(nullptr)
{
    const size_t N = positionalConstant<DEF_NCHAN, MIN_NCHAN, MAX_NCHAN>(0);
    for (size_t i = 1; i < N * inMultiple(args); i++)
        createSignalInlet();

    createInlet();

    createSignalOutlet();
    if (args.flags & XFADE_STEREO)
        createSignalOutlet();

    prop_type_ = new SymbolEnumProperty("@type", { SYM_POW, SYM_LIN });
    addProperty(prop_type_);

    addProperty(new SymbolEnumAlias("@pow", prop_type_, SYM_POW));
    addProperty(new SymbolEnumAlias("@lin", prop_type_, SYM_LIN));

    {
        Property* p = createCbFloatProperty(
            "@smooth",
            [this]() -> t_float { return smooth_ms_; },
            [this](t_float f) -> bool { smooth_ms_ = f; return true; });
        p->setUnitsMs();
        p->setFloatCheck(PropValueConstraints::GREATER_EQUAL, 1);
    }

    gain_.assign(N, t_smooth(0));
    gain_[0].setTargetValue(1);
}

void XFadeTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    const double SR = samplerate();

    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setDurationMs(smooth_ms_, SR);
}

void XFadeTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v = 0;
        for (size_t j = 0; j < N; j++) {
            t_sample s = gain_[j]() * in[j][i];
            v += s;
        }

        out[0][i] = v;
    }
}

void XFadeTilde::onInlet(size_t n, const AtomListView& lv)
{
    typedef std::function<t_float(t_float, t_float)> XFadeCurveFunction;
    typedef std::pair<t_symbol*, XFadeCurveFunction> XFadeCurvePair;

    static XFadeCurvePair xfade_fns[] = {
        { SYM_LIN, [](t_float a, t_float b) { return a - b; } },
        { SYM_POW, [](t_float a, t_float b) { return (a - b) * (a - b); } }
    };

    if (!checkArgs(lv, ARG_FLOAT)) {
        OBJ_ERR << "float expected: " << lv;
        return;
    }

    t_float v = lv.floatAt(0, 0);

    auto type = prop_type_->value();
    auto fn_it = std::find_if(std::begin(xfade_fns), std::end(xfade_fns),
        [type](const XFadeCurvePair& p) { return p.first == type; });

    if (fn_it == std::end(xfade_fns)) {
        OBJ_ERR << "unknown xfade type: " << type;
        return;
    }

    const size_t N = gain_.size();
    for (size_t i = 0; i < N; i++) {
        if (v >= i && v < (i + 1) && (i + 1) < N) {
            t_float k = fn_it->second(v, i);
            gain_[i].setTargetValue(1 - k);
            gain_[i + 1].setTargetValue(k);
            i++;
        } else if (v >= i && (i + 1) == N) {
            gain_[i].setTargetValue(1);
        } else {
            gain_[i].setTargetValue(0);
        }
    }
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

    obj.setDescription("multi signal crossfade");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "crossfade", "xfade" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}
