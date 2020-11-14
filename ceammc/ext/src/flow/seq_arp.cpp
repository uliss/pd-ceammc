/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "seq_arp.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <algorithm>

static t_symbol* SYM_FIRST;
static t_symbol* SYM_LAST;
static t_symbol* SYM_RANDOM;
static t_symbol* SYM_ALL;

SeqArp::SeqArp(const PdArgs& args)
    : BaseObject(args)
    , chord_(nullptr)
{
    on_offs_.reserve(32);

    chord_ = new ListProperty("@chord");
    chord_->setArgIndex(0);
    chord_->setAtomCheckFn([](const Atom& a) -> bool { return a.isFloat(); });
    chord_->setSuccessFn([this](Property* p) {
        on_offs_.assign(chord_->value().size(), 0);
    });
    addProperty(chord_);

    createOutlet();
    createOutlet();
}

void SeqArp::onBang()
{
}

void SeqArp::m_on(t_symbol* s, const AtomListView& lv)
{
    const auto& chord = chord_->value();
    if (chord.empty()) {
        METHOD_ERR(s) << "empty chord";
        return;
    }

    const t_symbol* mode = lv.symbolAt(0, SYM_FIRST);
    const auto cmode = mode->s_name[0];
    const auto velocity = clip<t_float, 1, 127>(lv.floatAt(1, 127));
    const int count = lv.intAt(2, 1);

    if (count < 1) {
        METHOD_ERR(s) << "invalid number of notes, expected >= 1\n"
                         "usage: on MODE(f[irst]|l[ast]|r[andom]|a[ll]) VELOCITY NUM";
        return;
    }

    Atom midi[2];
    midi[1].setFloat(velocity, true);
    AtomListView notev(&midi[0].atom(), 2);
    // should be equal
    const auto MAX = std::min<size_t>(on_offs_.size(), chord.size());

    if (mode == SYM_FIRST || cmode == 'f') {
        int num = count;

        for (size_t i = 0; i < MAX && num > 0; i++) {
            if (!on_offs_[i]) {
                on_offs_[i] = true;
                num -= 1;
                midi[0] = chord[i];
                listTo(0, notev);
            }
        }
    } else if (mode == SYM_LAST || cmode == 'l') {
        int num = count;

        for (size_t i = 0; i < MAX && num > 0; i++) {
            const auto idx = MAX - (i + 1);
            if (!on_offs_[idx]) {
                on_offs_[idx] = true;
                num -= 1;
                midi[0] = chord[idx];
                listTo(0, notev);
            }
        }
    } else if (mode == SYM_ALL || cmode == 'a') {
        for (size_t i = 0; i < MAX; i++) {
            if (!on_offs_[i]) {
                on_offs_[i] = true;
                midi[0] = chord[i];
                listTo(0, notev);
            }
        }
    }
}

void SeqArp::m_off(t_symbol* s, const AtomListView& lv)
{
    const t_symbol* mode = lv.symbolAt(0, SYM_FIRST);
    const auto cmode = mode->s_name[0];
    const int count = lv.intAt(1, 1);

    if (count < 1) {
        METHOD_ERR(s) << "invalid number of notes to off, expected >= 1\n"
                         "usage: off MODE(f[irst]|l[ast]|r[andom]|a[ll]) NUM";
        return;
    }

    Atom midi[2];
    midi[1].setFloat(0, true);
    AtomListView notev(&midi[0].atom(), 2);
    // should be equal
    const auto MAX = std::min<size_t>(on_offs_.size(), chord_->value().size());

    if (mode == SYM_FIRST || cmode == 'f') {
        int num = count;

        for (size_t i = 0; i < MAX && num > 0; i++) {
            if (on_offs_[i]) {
                on_offs_[i] = 0;
                num -= 1;
                midi[0].setFloat(chord_->value().at(i).asFloat(), true);
                listTo(0, notev);
            }
        }
    } else if (mode == SYM_LAST || cmode == 'l') {
        int num = count;

        for (size_t i = 0; i < MAX && num > 0; i++) {
            const auto idx = MAX - (i + 1);
            if (on_offs_[idx]) {
                on_offs_[idx] = 0;
                num -= 1;
                midi[0].setFloat(chord_->value().at(idx).asFloat(), true);
                listTo(0, notev);
            }
        }
    } else if (mode == SYM_ALL || cmode == 'a') {
        for (size_t i = 0; i < MAX; i++) {
            if (on_offs_[i]) {
                on_offs_[i] = 0;
                midi[0].setFloat(chord_->value().at(i).asFloat(), true);
                listTo(0, notev);
            }
        }
    }
}

void SeqArp::m_reset(t_symbol* s, const AtomListView& lv)
{
    std::fill(on_offs_.begin(), on_offs_.end(), 0);
}

void setup_seq_arp()
{
    SYM_FIRST = gensym("first");
    SYM_LAST = gensym("last");
    SYM_RANDOM = gensym("random");
    SYM_ALL = gensym("all");

    ObjectFactory<SeqArp> obj("seq.arp");
    obj.addMethod("on", &SeqArp::m_on);
    obj.addMethod("off", &SeqArp::m_off);
    obj.addMethod("reset", &SeqArp::m_reset);
}
