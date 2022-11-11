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
#include "conv_note2guido.h"
#include "ceammc_factory.h"
#include "ceammc_music_theory.h"
#include "ceammc_music_theory_pitch_class.h"

using namespace ceammc::music;

ConvNote2Guido::ConvNote2Guido(const PdArgs& args)
    : BaseObject(args)
    , pc_ {
        gensym("_"),
        gensym("c"),
        gensym("d"),
        gensym("e"),
        gensym("f"),
        gensym("g"),
        gensym("a"),
        gensym("b"),
    }
{
    createOutlet();
}

void ConvNote2Guido::onFloat(t_float f)
{
    if (f <= 0)
        return symbolTo(0, pc_[0]);

    auto n = guidoNote(f);
    symbolTo(0, gensym(n.c_str()));
}

void ConvNote2Guido::onList(const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, ARG_FLOAT)) {
        OBJ_ERR << "expected: PITCH DURATION, git: " << lv;
        return;
    }

    auto p = lv[0].asT<t_float>();
    auto d = lv[1].asT<t_float>();

    int dots = 0;
    const auto fm = std::fmod(d, 1);
    if (fm == 0.5)
        dots = 1;
    else if (fm == 0.25)
        dots = 2;
    else if (fm == 0.125)
        dots = 3;

    auto n = guidoNote(p, d, dots);
    symbolTo(0, gensym(n.c_str()));
}

ConvNote2Guido::GuidoNote ConvNote2Guido::guidoNote(size_t midiPitch, int dur, int dots) const
{
    Tonality t(PitchClass::C, MAJOR);
    auto pc = Tonality::correctAlteration(midiPitch, t, ALTERATE_UP);
    auto name = music::to_string(pc, NAMING_SCHEME_GUIDO);
    const int oct = int(midiPitch / 12) - 4;

    GuidoNote res = name.c_str();

    char buf[16];

    sprintf(buf, "%d", oct);
    res += buf;

    if (dur > 0) {
        sprintf(buf, "/%d", dur);
        res += buf;
    }

    if (dots-- > 0)
        res.push_back('.');

    return res;
}

void setup_conv_note2guido()
{
    ObjectFactory<ConvNote2Guido> obj("conv.note2guido");
}
