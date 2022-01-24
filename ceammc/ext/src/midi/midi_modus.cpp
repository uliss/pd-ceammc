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
#include "midi_modus.h"
#include "ceammc_factory.h"
#include "ceammc_music_scale.h"
#include "lex/parser_music.h"

#include <algorithm>

CEAMMC_DEFINE_HASH(snap)
CEAMMC_DEFINE_HASH(skip)

enum {
    INVALID_PITCH = -1,
    INVALID_SCALE = -2,
    SKIP_NOTE = -3
};

MidiModus::MidiModus(const PdArgs& args)
    : BaseObject(args)
    , scale_(music::ScaleLibrary::instance().findByHash("chromatic"_hash))
    , root_(music::PITCH_NAME_C)
    , prop_scale_(nullptr)
    , prop_mode_(nullptr)
{
    createOutlet();

    prop_scale_ = new SymbolEnumProperty("@scale", { "chromatic" });
    prop_scale_->setSuccessFn([this](Property*) {
        scale_ = music::ScaleLibrary::instance().find(prop_scale_->value());
    });
    addProperty(prop_scale_);

    for (auto& s : music::ScaleLibrary::instance().all()) {
        if (s->name() == "chromatic")
            continue;

        prop_scale_->appendEnum(gensym(s->name().c_str()));
    }

    prop_mode_ = new SymbolEnumProperty("@mode", { str_snap, str_skip });
    addProperty(prop_mode_);
}

void MidiModus::onFloat(t_float f)
{
    const auto res = mapNote(f);
    if (res < 0)
        return;

    floatTo(0, res);
}

void MidiModus::onList(const AtomList& lst)
{
    if (!checkArgs(lst.view(), ARG_BYTE, ARG_BYTE) && !checkArgs(lst.view(), ARG_BYTE, ARG_BYTE, ARG_FLOAT)) {
        OBJ_ERR << "PITCH VEL [DUR] expected, got: " << lst;
        return;
    }

    const t_float note = mapNote(lst[0].asT<t_float>());
    if (note < 0)
        return;

    Atom msg[3] = { note, lst[1] };
    if (lst.size() == 3)
        msg[2] = lst[2];

    listTo(0, AtomListView(msg, lst.size()));
}

t_float MidiModus::mapNote(t_float note) const
{
    if (note < 0 || note > 127) {
        OBJ_ERR << "invalid pitch: " << note;
        return INVALID_PITCH;
    }

    if (!scale_) {
        OBJ_ERR << "invalid scale: " << prop_scale_->value();
        return INVALID_SCALE;
    }

    const int root = root_.absolutePitch();
    const t_float step = std::fmod(note + 12 - root, 12);

    switch (crc32_hash(prop_mode_->value())) {
    case hash_snap: {
        t_float degree = 0;
        if (scale_->findNearest(step, degree))
            return (root + degree) + (int(note) / 12) * 12;

        break;
    }
    case hash_skip:
    default:
        if (scale_->find(step))
            return note;
        break;
    }
    return SKIP_NOTE;
}

void setup_midi_modus()
{
    ObjectFactory<MidiModus> obj("midi.modus");
}
