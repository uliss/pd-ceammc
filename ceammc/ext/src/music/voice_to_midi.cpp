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
#include "voice_to_midi.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "lex/parser_music.h"

Voice2Midi::Voice2Midi(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void Voice2Midi::onSymbol(t_symbol* s)
{
    onList(AtomList(s));
}

void Voice2Midi::onList(const AtomListView& lv)
{
    using namespace ceammc::parser;
    using NoteVec = boost::container::small_vector<music::Notation, 16>;

    NoteVec out;
    if (!parser::parse_notation(lv, out)) {
        OBJ_ERR << "note list expected, got: " << lv;
        return;
    }

    SmallAtomList atoms;
    atoms.reserve(out.size());

    int prev_oct = 4;
    music::Notation::setAbsOctaves(out.begin(), out.end(), prev_oct);

    for (auto& n : out)
        atoms.push_back(n.isRest() ? 0 : n.asMidi());

    listTo(0, atoms.view());
}

void setup_music_voice2midi()
{
    ObjectFactory<Voice2Midi> obj("music.voice2midi");
    obj.addAlias("music.v->m");
}
