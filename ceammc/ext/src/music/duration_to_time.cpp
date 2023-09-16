/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "duration_to_time.h"
#include "ceammc_factory.h"
#include "lex/parser_music.h"

#include <boost/container/small_vector.hpp>

using SmallAtomVec = boost::container::small_vector<Atom, 16>;
using DurationVec = boost::container::small_vector<music::Duration, 16>;

Duration2Time::Duration2Time(const PdArgs& args)
    : BaseObject(args)
    , bpm_(nullptr)
    , seq_(nullptr)
{
    createOutlet();

    bpm_ = new BpmProperty("@bpm", 60);
    addProperty(bpm_);

    seq_ = new BoolProperty("@seq", true);
    addProperty(seq_);
}

void Duration2Time::onFloat(t_float f)
{
    const int dur = static_cast<int>(f);
    if (dur != f || dur <= 0) {
        OBJ_ERR << "int value (>0) expected, got: " << f;
        return;
    }

    const auto ms = bpm_->tempo().wholeNoteDurationMs() / f;
    floatTo(0, ms);
}

void Duration2Time::onSymbol(t_symbol* s)
{
    using namespace ceammc::parser;

    DurationVec out;
    if (!parse_duration(AtomListView(s), out)) {
        OBJ_ERR << "invalid duration: " << s->s_name;
        return;
    }

    SmallAtomVec atoms;
    atoms.reserve(out.size());

    t_float t = 0;
    const bool seq = seq_->value();

    for (size_t i = 0; i < out.size(); i++) {
        const auto ms = out[i].timeMs(bpm_->tempo());

        if (seq)
            t += ms;
        else
            t = ms;

        atoms.push_back(t);
    }

    listTo(0, AtomListView(atoms.data(), atoms.size()));
}

void Duration2Time::onList(const AtomListView& lv)
{
    using namespace ceammc::parser;

    DurationVec out;
    if (!parse_duration(lv, out)) {
        OBJ_ERR << "duration list expected, got: " << lv;
        return;
    }

    SmallAtomVec atoms;
    atoms.reserve(out.size());

    t_float t = 0;
    const bool seq = seq_->value();

    for (size_t i = 0; i < out.size(); i++) {
        const auto ms = out[i].timeMs(bpm_->tempo());

        if (seq)
            t += ms;
        else
            t = ms;

        atoms.push_back(t);
    }

    listTo(0, AtomListView(atoms.data(), atoms.size()));
}

void Duration2Time::onAny(t_symbol* s, const AtomListView& lv)
{
}

void setup_music_duration2time()
{
    ObjectFactory<Duration2Time> obj("music.dur2time");
    obj.addAlias("music.d->t");
}
