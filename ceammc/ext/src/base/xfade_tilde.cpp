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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(lin);
CEAMMC_DEFINE_SYM_HASH(pow)

#include <algorithm>

constexpr t_float DEFAULT_SMOOTH_MS = 20;
constexpr int DEF_NCHAN = 2;
constexpr int MIN_NCHAN = 2;
constexpr int MAX_NCHAN = 16;

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
    , value_(nullptr)
{
    n_ = new IntProperty("@n", DEF_NCHAN);
    n_->checkClosedRange(MIN_NCHAN, MAX_NCHAN);
    n_->setInitOnly();
    n_->setArgIndex(0);
    addProperty(n_);

    prop_type_ = new SymbolEnumProperty("@type", { sym_pow(), sym_lin() });
    addProperty(prop_type_);

    addProperty(new SymbolEnumAlias("@pow", prop_type_, sym_pow()));
    addProperty(new SymbolEnumAlias("@lin", prop_type_, sym_lin()));

    {
        Property* p = createCbFloatProperty(
            "@smooth",
            [this]() -> t_float { return smooth_ms_; },
            [this](t_float f) -> bool { smooth_ms_ = f; return true; });
        p->setUnitsMs();
        p->setFloatCheck(PropValueConstraints::GREATER_EQUAL, 1);
    }

    value_ = new FloatProperty("@x", 0);
    value_->setArgIndex(1);
    addProperty(value_);
}

void XFadeTilde::initDone()
{
    const auto M = inMultiple(pdArgs());
    // create inlets
    for (size_t i = 1; i < n_->value() * M; i++)
        createSignalInlet();

    createInlet();

    // create outlets
    for (size_t i = 0; i < M; i++)
        createSignalOutlet();

    value_->setSuccessFn([this](Property* p) { setXFade(value_->value()); });
    value_->setFloatCheckFn([this](t_float x) -> bool { return 0 <= x && x <= n_->value() - 1; });
    value_->setValue(clip<t_float>(value_->value(), 0, n_->value() - 1));

    gain_.assign(n_->value(), t_smooth(0));
    setXFade(value_->value());
}

void XFadeTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    const double SR = samplerate();

    for (auto& g : gain_)
        g.setDurationMs(smooth_ms_, SR);
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
    value_->set(lv);
}

std::vector<float> XFadeTilde::gains() const
{
    std::vector<float> res;

    for (auto& i : gain_)
        res.push_back(i.target());

    return res;
}

void XFadeTilde::setXFade(t_float v)
{
    using XFadeCurveFunction = t_float (*)(t_float, t_float);

    XFadeCurveFunction curve_fn = nullptr;
    switch (crc32_hash(prop_type_->value())) {
    case hash_lin:
        curve_fn = [](t_float a, t_float b) -> t_float { return a - b; };
        break;
    case hash_pow:
        curve_fn = [](t_float a, t_float b) -> t_float { return (a - b) * (a - b); };
        break;
    default:
        OBJ_ERR << "unknown xfade type: " << prop_type_->value();
        return;
    }

    const size_t N = gain_.size();
    for (size_t i = 0; i < N; i++) {
        if (v >= i && v < (i + 1) && (i + 1) < N) {
            auto k = curve_fn(v, i);
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

void setup_base_xfade_tilde()
{
    SoundExternalFactory<XFadeTilde> obj("xfade~");

    obj.setDescription("multi signal crossfade");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "crossfade", "xfade" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}
