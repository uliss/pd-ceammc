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
#include "mix.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_property_callback.h"

constexpr t_float DEFAULT_XFADE = 20;
constexpr size_t DEF_NCHAN = 2;
constexpr size_t MIN_NCHAN = 2;
constexpr size_t MAX_NCHAN = 16;

static t_float toDb(t_float amp)
{
    return std::max<t_float>(-144, convert::amp2dbfs(amp));
}

static t_float fromDb(t_float db)
{
    if (db <= -144)
        return 0;

    return convert::dbfs2amp(db);
}

Mix::Mix(const PdArgs& args)
    : SoundExternal(args)
    , is_solo_(false)
    , n_(positionalConstant<DEF_NCHAN, MIN_NCHAN, MAX_NCHAN>(0))
    , xfade_time_(DEFAULT_XFADE)
{
    for (size_t i = 1; i < n_; i++)
        createSignalInlet();

    createSignalOutlet();

    gain_.assign(n_, t_smooth(0));
    mute_.assign(n_, t_smooth(0));
    solo_.assign(n_, t_smooth(0, 1));
    solo_values_.assign(n_, 0);

    {
        Property* p = createCbFloatProperty(
            "@xfade_time",
            [this]() -> t_float { return xfade_time_; },
            [this](t_float f) -> bool { xfade_time_ = f; return true; });
        p->setFloatCheck(PropValueConstraints::GREATER_EQUAL, 1);
        p->setUnitsMs();
    }

    createCbListProperty(
        "@db",
        [this]() { return propDb(); },
        [this](const AtomList& l) { setPropDb(l); return true; })
        ->setUnitsDb();
    createCbProperty("@value", &Mix::propValue, &Mix::setPropValue);

    createCbListProperty(
        "@mute",
        [this]() { return propMute(); },
        [this](const AtomList& l) { setPropMute(l); return true; });

    createCbProperty("@solo", &Mix::propSolo, &Mix::setPropSolo);
}

void Mix::onList(const AtomList& lst)
{
    const size_t N = std::min(lst.size(), gain_.size());
    for (size_t i = 0; i < N; i++)
        gain_[i].setTargetValue(std::max<t_float>(0, lst[i].asFloat()));
}

void Mix::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    const double SR = samplerate();

    const size_t N = gain_.size();
    for (size_t i = 0; i < N; i++) {
        gain_[i].setDurationMs(xfade_time_, SR);
        mute_[i].setDurationMs(10, SR);
        solo_[i].setDurationMs(10, SR);
    }
}

void Mix::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v = 0;
        for (size_t j = 0; j < N; j++) {
            t_sample s = gain_[j]() * in[j][i];
            s *= (1 - mute_[j]());
            s *= solo_[j]();
            v += s;
        }

        out[0][i] = v;
    }
}

AtomList Mix::propValue() const
{
    AtomList res;
    res.reserve(gain_.size());

    for (size_t i = 0; i < gain_.size(); i++)
        res.append(gain_[i].target());

    return res;
}

void Mix::setPropValue(const AtomList& lst)
{
    onList(lst);
}

AtomList Mix::propDb() const
{
    return propValue().mapFloat(toDb);
}

void Mix::setPropDb(const AtomList& lst)
{
    setPropValue(lst.mapFloat(fromDb));
}

AtomList Mix::propMute() const
{
    AtomList res;
    res.reserve(mute_.size());

    for (size_t i = 0; i < mute_.size(); i++)
        res.append(mute_[i].target());

    return res;
}

void Mix::setPropMute(const AtomList& lst)
{
    for (size_t i = 0; i < mute_.size(); i++) {
        int v = int(lst.floatAt(i, 0)) != 0 ? 1 : 0;
        mute_[i].setTargetValue(v);
    }
}

AtomList Mix::propSolo() const
{
    AtomList res;
    res.reserve(solo_values_.size());

    for (size_t i = 0; i < solo_values_.size(); i++)
        res.append(solo_values_[i]);

    return res;
}

void Mix::setPropSolo(const AtomList& lst)
{
    is_solo_ = false;

    for (size_t i = 0; i < solo_values_.size(); i++) {
        int v = (int(lst.floatAt(i, 0)) == 0) ? 0 : 1;
        solo_values_[i] = v;
    }

    is_solo_ = std::find(solo_values_.begin(), solo_values_.end(), 1) != solo_values_.end();

    if (is_solo_) {
        const auto N = std::min<size_t>(solo_values_.size(), solo_.size());

        for (size_t i = 0; i < N; i++)
            solo_[i].setTargetValue(solo_values_[i]);
    } else {
        for (size_t i = 0; i < solo_.size(); i++)
            solo_[i].setTargetValue(1);
    }
}

void Mix::m_mute(t_symbol* s, const AtomListView& lst)
{
    if (!checkArgs(lst, ARG_NATURAL, ARG_BOOL, s))
        return;

    int idx = lst.intAt(0, -1);
    int v = lst.intAt(1, -1);

    if (idx < 0 || idx >= n_) {
        OBJ_ERR << "invalid input index: " << lst[0];
        return;
    }

    if (v == 0)
        mute_[idx].setTargetValue(0);
    else if (v == 1)
        mute_[idx].setTargetValue(1);
    else
        OBJ_ERR << "invalid mute value: " << lst[1];
}

void Mix::m_solo(t_symbol* s, const AtomListView& lst)
{
    if (!checkArgs(lst, ARG_NATURAL, ARG_BOOL, s))
        return;

    int idx = lst.intAt(0, -1);
    int v = lst.intAt(1, -1);

    if (idx < 0 || idx >= n_ || idx >= solo_.size() || idx >= solo_values_.size()) {
        OBJ_ERR << "invalid input index: " << lst[0];
        return;
    }

    if (v == 0) {
        solo_[idx].setTargetValue(0);
        solo_values_[idx] = 0;
    } else if (v == 1) {
        solo_[idx].setTargetValue(1);
        solo_values_[idx] = 1;
    } else {
        OBJ_ERR << "invalid mute value: " << lst[1];
        return;
    }

    is_solo_ = std::find(solo_values_.begin(), solo_values_.end(), 1) != solo_values_.end();
}

void setup_base_mix()
{
    SoundExternalFactory<Mix> obj("mix~");
    obj.addMethod("mute", &Mix::m_mute);
    obj.addMethod("solo", &Mix::m_solo);

    obj.setDescription("multislot signal mixer");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "mix", "amplitude", "decibel" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}
