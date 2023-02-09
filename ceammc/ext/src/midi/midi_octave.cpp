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
#include "midi_octave.h"
#include "ceammc_args.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include <memory>
#include <random>

static std::unique_ptr<ArgChecker> onlist_chk;
static std::mt19937 oct_gen;

CEAMMC_DEFINE_STR(transpose);
CEAMMC_DEFINE_HASH(set);

constexpr int MIN_OCT = -11;
constexpr int MAX_OCT = 11;

MidiOctave::MidiOctave(const PdArgs& args)
    : BaseObject(args)
    , oct_(nullptr)
    , mode_(nullptr)
    , a_(-1)
    , b_(1)
    , random_(false)
{
    oct_ = new IntProperty("@oct", 0);
    oct_->setArgIndex(0);
    oct_->checkClosedRange(MIN_OCT, MAX_OCT);
    addProperty(oct_);

    mode_ = new SymbolEnumProperty("@mode", { str_transpose, str_set });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@set", mode_, gensym(str_set)));
    addProperty(new SymbolEnumAlias("@transpose", mode_, gensym(str_transpose)));

    createCbListProperty(
        "@random",
        [this]() -> AtomList { return random_ ? AtomList { (t_float)a_, (t_float)b_ } : AtomList(); },
        [this](const AtomListView& lv) -> bool {
            if (lv.empty()) {
                random_ = false;
                return true;
            } else if (lv.size() == 2) {
                if (lv[0].isFloat() && lv[1].isFloat()) {
                    auto a = lv[0].asInt();
                    auto b = lv[1].asInt();

                    if (a == 0 && b == 0) {
                        random_ = false;
                        return true;
                    }

                    if (a < MIN_OCT || a > MAX_OCT) {
                        OBJ_ERR << MIN_OCT << "<=MIN<=" << MAX_OCT << " expected, got: " << a;
                        return false;
                    }

                    if (b < MIN_OCT || b > MAX_OCT) {
                        OBJ_ERR << MIN_OCT << "<=MAX<=" << MAX_OCT << " expected, got: " << b;
                        return false;
                    }

                    if (a >= b) {
                        OBJ_ERR << "MIN<MAX expected, got: " << lv;
                        return false;
                    }

                    random_ = true;
                    a_ = a;
                    b_ = b;
                    return true;
                } else {
                    OBJ_ERR << "usage: @random [MIN MAX]";
                    return false;
                }
            } else {
                OBJ_ERR << "usage: @random [MIN MAX]";
                return false;
            }
        });

    createInlet();
    createOutlet();
}

void MidiOctave::onFloat(t_float note)
{
    floatTo(0, octave(note));
}

void MidiOctave::onList(const AtomListView& lv)
{
    if (!onlist_chk->check(lv)) {
        OBJ_ERR << "NOTE VEL [DUR] expected, got: " << lv;
        return;
    }

    const size_t N = lv.size();
    assert(N == 2 || N == 3);

    Atom msg[3] = { octave(lv[0].asFloat()), lv[1], N == 3 ? lv[2] : 0. };
    listTo(0, AtomListView(msg, N));
}

void MidiOctave::onInlet(size_t, const AtomListView& lv)
{
    oct_->setList(lv);
}

t_float MidiOctave::octave(t_float note) const
{
    note = clip<t_float, 0, 127>(note);

    const int oct = random_
        ? std::uniform_int_distribution<int>(a_, b_)(oct_gen)
        : oct_->value();

    switch (crc32_hash(mode_->value())) {
    case hash_set:
        note = std::fmod(note, 12);
        break;
    default:
        break;
    }

    auto res = note + 12 * oct;
    return (res < 0 || res > 127) ? note : res;
}

void setup_midi_octave()
{
    onlist_chk.reset(new ArgChecker("f0..127 f0..127 f?"));

    ObjectFactory<MidiOctave> obj("midi.oct");
    obj.setXletsInfo({ "float: note\n"
                       "list: NOTE VEL \\[DUR\\]",
                         "int: set octave transposition" },
        { "float: note\n"
          "list: NOTE VEL \\[DUR\\]" });

    obj.setDescription("midi octave transpose");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "octave", "transpose"});
}
