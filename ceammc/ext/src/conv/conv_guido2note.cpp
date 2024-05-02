/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#include "conv_guido2note.h"
#include "ceammc_factory.h"
#include "ceammc_music_theory_tempo.h"
#include "fmt/core.h"
#include "lex/parser_guido.h"

ConvGuido2Note::ConvGuido2Note(const PdArgs& args)
    : BaseObject(args)
{
    tempo_ = new BpmProperty("@tempo", 60);
    tempo_->setArgIndex(0);
    addProperty(tempo_);

    createOutlet();
    createOutlet();
}

void ConvGuido2Note::onSymbol(t_symbol* s)
{
    auto pc = music::PitchClass::C;
    int octave = 1;
    music::Duration dur;

    if (!parser::parse_guido_note(s->s_name, pc, octave, dur)) {
        OBJ_ERR << fmt::format("invalid guido note: {}", s->s_name);
        return;
    }

    int pitch = pc.pitch() + ((octave + 4) * 12);
    auto length = dur.timeMs(tempo_->tempo());

    floatTo(1, length);
    floatTo(0, pitch);
}

void setup_conv_guido2note()
{
    ObjectFactory<ConvGuido2Note> obj("conv.guido2note");
    obj.addAlias("guido->note");
}
