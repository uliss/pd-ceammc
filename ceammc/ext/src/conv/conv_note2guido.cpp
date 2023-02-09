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
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_music_theory.h"
#include "ceammc_music_theory_pitch_class.h"

#include <cmath>

using namespace ceammc::music;

ConvNote2Guido::ConvNote2Guido(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ConvNote2Guido::onFloat(t_float f)
{
    auto n = guidoNote(f);
    symbolTo(0, gensym(n.c_str()));
}

void ConvNote2Guido::onList(const AtomListView& lv)
{
    AtomList64 data;
    data.insert_back(lv.begin(), lv.end());

    for (auto& a : data) {
        if (a.isFloat())
            a.setSymbol(gensym(guidoNote(a.asT<t_float>()).c_str()), true);
    }

    listTo(0, data.view());
}

void ConvNote2Guido::m_note(t_symbol* s, const AtomListView& lv)
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

ConvNote2Guido::GuidoNote ConvNote2Guido::guidoNote(int midiPitch, int dur, int dots) const
{
    const bool is_note = midiPitch >= 0;
    GuidoNote res;
    char buf[16];

    if (is_note) {
        Tonality t(PitchClass::C, MAJOR);
        auto pc = Tonality::correctAlteration(midiPitch, t, ALTERATE_UP);
        auto name = music::to_string(pc, NAMING_SCHEME_GUIDO);
        res = name.c_str();

        const int oct = int(clip<int, 0, 128>(midiPitch) / 12) - 4;
        sprintf(buf, "%d", oct);
        res += buf;
    } else
        res += '_';

    if (dur > 0) {
        sprintf(buf, "/%d", dur);
        res += buf;
    }

    while (dots-- > 0)
        res.push_back('.');

    return res;
}

void setup_conv_note2guido()
{
    ObjectFactory<ConvNote2Guido> obj("conv.note2guido");
    obj.addAlias("note->guido");

    obj.addMethod("note", &ConvNote2Guido::m_note);

    obj.setDescription("convert MIDI note to Guido notation");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "guido", "midi"});
}
