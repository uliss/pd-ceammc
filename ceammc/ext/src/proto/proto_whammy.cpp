/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "proto_whammy.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include <algorithm>

namespace {
using State = ProtoWhammy::State;

constexpr const State ST_UP_2OCT { 24, 0 };
constexpr const State ST_UP_OCT { 12, 0 };
constexpr const State ST_UP_5TH { 7, 0 };
constexpr const State ST_UP_4TH { 7, 0 };
constexpr const State ST_DOWN_2ND { -2, 0 };
constexpr const State ST_DOWN_4TH { -5, 0 };
constexpr const State ST_DOWN_5TH { -7, 0 };
constexpr const State ST_DOWN_OCT { -12, 0 };
constexpr const State ST_DOWN_2OCT { -24, 0 };
//
constexpr const State ST_DIVE_BOMB { -36, 0 };
constexpr const State ST_DETUNE_DEEP { 127, 1 };
constexpr const State ST_DETUNE_SHALLOW { 127, 2 };
//
constexpr const State ST_UP_2RD_UP_3RD { 2, 4 };
constexpr const State ST_UP_3RD_UP_3RD { 3, 4 };
constexpr const State ST_UP_3RD_UP_4TH { 4, 5 };
constexpr const State ST_UP_4TH_UP_5TH { 5, 7 };
constexpr const State ST_UP_5TH_UP_6TH { 7, 9 };
constexpr const State ST_UP_5TH_UP_7TH { 7, 11 };
constexpr const State ST_DOWN_4TH_DOWN_3TH { -5, -4 };
constexpr const State ST_DOWN_5TH_DOWN_4TH { -7, -5 };
constexpr const State ST_DOWN_OCT_UP_OCT { -8, +8 };

constexpr const auto STATE_DEFAULT = ST_UP_2OCT;

}

// NOTE: keep the order in stync with parser_whammy_common.h enum constants!
const ProtoWhammy::MidiMap ProtoWhammy::midi_classic_map_ = {
    { ST_UP_2OCT, 1, 22 },
    { ST_UP_OCT, 2, 23 },
    { ST_UP_5TH, 3, 24 },
    { ST_UP_4TH, 4, 25 },
    { ST_DOWN_2ND, 5, 26 },
    { ST_DOWN_4TH, 6, 27 },
    { ST_DOWN_5TH, 7, 28 },
    { ST_DOWN_OCT, 8, 29 },
    { ST_DOWN_2OCT, 9, 30 },
    { ST_DIVE_BOMB, 10, 31 },
    { ST_DETUNE_DEEP, 11, 32 },
    { ST_DETUNE_SHALLOW, 12, 33 },
    { ST_UP_2RD_UP_3RD, 13, 34 },
    { ST_UP_3RD_UP_3RD, 14, 35 },
    { ST_UP_3RD_UP_4TH, 15, 36 },
    { ST_UP_4TH_UP_5TH, 16, 37 },
    { ST_UP_5TH_UP_6TH, 17, 38 },
    { ST_UP_5TH_UP_7TH, 18, 39 },
    { ST_DOWN_4TH_DOWN_3TH, 19, 40 },
    { ST_DOWN_5TH_DOWN_4TH, 20, 41 },
    { ST_DOWN_OCT_UP_OCT, 21, 42 },
};

ProtoWhammy::ProtoWhammy(const PdArgs& args)
    : BaseObject(args)
    , mode_(nullptr)
    , chan_(nullptr)
    , active_(nullptr)
    , idx_(0)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { "classic", "chords" });
    addProperty(mode_);
    mode_->setArgIndex(0);

    addProperty(new SymbolEnumAlias("@classic", mode_, gensym("classic")));
    addProperty(new SymbolEnumAlias("@chords", mode_, gensym("chords")));

    chan_ = new IntProperty("@chan", 0);
    chan_->checkClosedRange(0, 15);
    addProperty(chan_);

    active_ = new BoolProperty("@active", true);
    active_->setArgIndexNext(mode_);
    active_->setSuccessFn([this](Property*) { output(); });
    addProperty(active_);

    addProperty(new random::SeedProperty(gen_));
}

void ProtoWhammy::onFloat(t_float f)
{
    active_->set(Atom(f));
    output();
}

void ProtoWhammy::m_reset(t_symbol*, const AtomListView& lv)
{
    active_->setValue(false);
    idx_ = 0;
    output();
}

void ProtoWhammy::m_toggle(t_symbol*, const AtomListView& lv)
{
    active_->setValue(!active_->value());
    output();
}

void ProtoWhammy::m_random(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        const auto N = midi_classic_map_.size();

        std::uniform_int_distribution<size_t> dist(0, N);
        idx_ = dist(gen_.get());
        active_->setValue(lv.boolAt(0, true));
    } else if (lv.isSymbol()) {
        auto sym_hash = crc32_hash(lv.asT<t_symbol*>());
        switch (sym_hash) {
        case "whammy"_hash: {
            std::uniform_int_distribution<size_t> dist(proto::WHAMMY_MODE_MIN_TRANSPOSE, proto::WHAMMY_MODE_MAX_TRANSPOSE + 1);
            idx_ = dist(gen_.get());
        } break;
        case "harm"_hash: {
            std::uniform_int_distribution<size_t> dist(proto::WHAMMY_MODE_MIN_HARMONIZER, proto::WHAMMY_MODE_MAX_HARMONIZER + 1);
            idx_ = dist(gen_.get());
        } break;
        case "detune"_hash: {
            std::uniform_int_distribution<size_t> dist(proto::WHAMMY_MODE_MIN_DETUNE, proto::WHAMMY_MODE_MAX_DETUNE + 1);
            idx_ = dist(gen_.get());
        } break;
        default:
            METHOD_ERR(s) << "unknown random method: " << s << ", expected 'whammy', 'harm' or 'detune'";
            return;
        }

        active_->setValue(lv.boolAt(0, true));
    } else {
        METHOD_ERR(s) << "usage: RANDOM_METHOD?";
        return;
    }

    output();
}

void ProtoWhammy::m_set(t_symbol* s, const AtomListView& lv)
{
    const auto N = midi_classic_map_.size();

    if (lv.size() == 2 && lv[0].isInteger() && lv[1].isBool()) {
        const auto i = lv.intAt(0, 0);
        if (i < 0 || i >= N) {
            METHOD_ERR(s) << "invalid index: " << i;
            return;
        }

        idx_ = i;
        active_->setValue(lv[1].asBool());
    } else if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isBool()) {
        auto sym = lv.symbolAt(0, &s_);
        proto::WhammyMode mode;
        if (!proto::nameToWhammyMode(sym->s_name, mode)) {
            METHOD_ERR(s) << "invalid mode name: " << sym;
            return;
        }

        if (mode < 0 || mode >= N) {
            METHOD_ERR(s) << "invalid index: " << mode;
            return;
        }

        idx_ = mode;
        active_->setValue(lv[1].asBool());
    } else if (lv.size() == 3 && lv[0].isInteger() && lv[1].isInteger() && lv[2].isBool()) {
        const auto up = lv[0].asInt();
        const auto down = lv[1].asInt();

        int idx = -1;
        for (size_t i = 0; i < N; i++) {
            auto& m = midi_classic_map_[i];
            if (m.state.pedal_up == up && m.state.pedal_down == down) {
                idx = i;
                break;
            }
        }

        if (idx < 0) {
            METHOD_ERR(s) << "mode not found: " << up << ' ' << down;
            return;
        }

        idx_ = idx;
        active_->setValue(lv[2].asBool());
    } else {
        METHOD_ERR(s) << "usage: \n"
                         "\t IDX STATE\n"
                         "\t MODE STATE\n"
                         "\t UP DOWN STATE";
        return;
    }

    output();
}

void ProtoWhammy::m_next(t_symbol* s, const AtomListView& lv)
{
    const auto N = midi_classic_map_.size();
    if (N == 0)
        return;

    int offset = 0;
    if (lv.empty()) {
        offset = 1;
    } else if (lv.isFloat()) {
        const int offset = lv.intAt(0, 1) % N;
        if (offset < 1) {
            METHOD_ERR(s) << "invalid offset: " << offset;
            return;
        }
    } else if (lv == gensym("random")) {
        std::uniform_int_distribution<size_t> dist(1, N);
        offset = dist(gen_.get());
    } else {
        METHOD_ERR(s) << "usage: INT or random";
        return;
    }

    idx_ = (idx_ + static_cast<size_t>(offset)) % N;

    output();
}

void ProtoWhammy::m_prev(t_symbol* s, const AtomListView& lv)
{
    const auto N = midi_classic_map_.size();
    if (N == 0)
        return;

    int offset = 0;

    if (lv.empty()) {
        offset = 1;
    } else if (lv.isFloat()) {
        const int offset = lv.intAt(0, 1) % N;
        if (offset < 1) {
            METHOD_ERR(s) << "invalid offset: " << offset;
            return;
        }
    } else if (lv == gensym("random")) {
        std::uniform_int_distribution<size_t> dist(1, N);
        offset = dist(gen_.get());
    } else {
        METHOD_ERR(s) << "usage: INT or random";
        return;
    }

    idx_ = (N + idx_ - static_cast<size_t>(offset)) % N;

    output();
}

void ProtoWhammy::output()
{
    if (idx_ >= midi_classic_map_.size())
        return;

    int chan = chan_->value();
    int val = active_->value() ? midi_classic_map_[idx_].active : midi_classic_map_[idx_].bypass;
    val += ((mode_->index() * 42) - 1);

    floatTo(0, 0xC0 | (chan & 0x7));
    floatTo(0, val & 0x7F);
}

void setup_proto_whammy()
{
    ObjectFactory<ProtoWhammy> obj("proto.whammy");
    obj.addMethod("reset", &ProtoWhammy::m_reset);
    obj.addMethod("toggle", &ProtoWhammy::m_toggle);
    obj.addMethod("random", &ProtoWhammy::m_random);
    obj.addMethod("set", &ProtoWhammy::m_set);
    obj.addMethod("next", &ProtoWhammy::m_next);
    obj.addMethod("prev", &ProtoWhammy::m_prev);
}
