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
#include "ceammc_factory.h"

#include <memory>
#include <random>

static std::unique_ptr<ArgChecker> onlist_chk;
static std::mt19937 oct_gen;

static t_symbol* SYM_TRANSPOSE;
static t_symbol* SYM_SET;
static t_symbol* SYM_RANDOM;

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

    mode_ = new SymbolEnumProperty("@mode", { SYM_TRANSPOSE, SYM_SET });
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@set", mode_, SYM_SET));
    addProperty(new SymbolEnumAlias("@transpose", mode_, SYM_TRANSPOSE));

    createCbListProperty(
        "@random",
        [this]() -> AtomList { return random_ ? AtomList { (t_float)a_, (t_float)b_ } : AtomList(); },
        [this](const AtomList& lst) -> bool {
            if (lst.empty()) {
                random_ = false;
                return true;
            } else if (lst.size() == 2) {
                if (lst[0].isFloat() && lst[1].isFloat()) {
                    auto a = lst[0].asInt();
                    auto b = lst[1].asInt();

                    if (a < MIN_OCT || a > MAX_OCT) {
                        OBJ_ERR << MIN_OCT << "<=MIN<=" << MAX_OCT << " expected, got: " << a;
                        return false;
                    }

                    if (b < MIN_OCT || b > MAX_OCT) {
                        OBJ_ERR << MIN_OCT << "<=MAX<=" << MAX_OCT << " expected, got: " << b;
                        return false;
                    }

                    if (a >= b) {
                        OBJ_ERR << "MIN<MAX expected, got: " << lst;
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

void MidiOctave::onList(const AtomList& lst)
{
    if (!onlist_chk->check(lst.view())) {
        OBJ_ERR << "NOTE VEL [DUR] expected, got: " << lst;
        return;
    }

    const size_t N = lst.size();
    assert(N == 2 || N == 3);

    Atom msg[3] = { octave(lst[0].asFloat()), lst[1], N == 3 ? lst[2] : 0. };
    listTo(0, AtomListView(msg, N));
}

void MidiOctave::onInlet(size_t, const AtomListView& lv)
{
    oct_->setList(lv);
}

t_float MidiOctave::octave(t_float note) const
{
    const int oct = random_
        ? std::uniform_int_distribution<int>(a_, b_)(oct_gen)
        : oct_->value();

    note = clip<t_float, 0, 127>(note);

    if (mode_->value() == SYM_SET)
        note = std::fmod(note, 12);

    auto res = note + 12 * oct;
    return (res < 0 || res > 127) ? note : res;
}

void setup_midi_octave()
{
    onlist_chk.reset(new ArgChecker("f0..127 f0..127 f?"));

    SYM_TRANSPOSE = gensym("transpose");
    SYM_SET = gensym("set");
    SYM_RANDOM = gensym("random");

    ObjectFactory<MidiOctave> obj("midi.oct");
    obj.setXletsInfo({ "float: note\n"
                       "list: NOTE VEL \\[DUR\\]",
                         "int: set octave transposition" },
        { "float: note\n"
          "list: NOTE VEL \\[DUR\\]" });
}
