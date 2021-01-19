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

static std::unique_ptr<ArgChecker> onlist_chk;

static t_symbol* SYM_TRANSPOSE;
static t_symbol* SYM_SET;

MidiOctave::MidiOctave(const PdArgs& args)
    : BaseObject(args)
    , oct_(nullptr)
    , mode_(nullptr)
{
    oct_ = new IntProperty("@oct", 0);
    oct_->setArgIndex(0);
    oct_->checkClosedRange(-11, 11);
    addProperty(oct_);

    mode_ = new SymbolEnumProperty("@mode", { SYM_TRANSPOSE, SYM_SET });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@set", mode_, SYM_SET));
    addProperty(new SymbolEnumAlias("@transpose", mode_, SYM_TRANSPOSE));

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
    const auto oct = oct_->value();
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

    ObjectFactory<MidiOctave> obj("midi.oct");
    obj.setXletsInfo({ "float: note\n"
                       "list: NOTE VEL \\[DUR\\]",
                         "int: set octave transposition" },
        { "float: note\n"
          "list: NOTE VEL \\[DUR\\]" });
}
