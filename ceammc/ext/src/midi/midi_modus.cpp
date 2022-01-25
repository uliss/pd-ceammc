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

MidiModus::MidiModus(const PdArgs& args)
    : BaseObject(args)
    , scale_(music::ScaleLibrary::instance().findByHash("chromatic"_hash))
    , prop_scale_(nullptr)
    , prop_mode_(nullptr)
    , root_(nullptr)
{
    createOutlet();

    prop_scale_ = new SymbolEnumProperty("@scale", { "chromatic" });
    prop_scale_->setSuccessFn([this](Property*) {
        scale_ = music::ScaleLibrary::instance().find(prop_scale_->value());
    });
    prop_scale_->setArgIndex(1);
    addProperty(prop_scale_);

    for (auto& s : music::ScaleLibrary::instance().all()) {
        if (s->name() == "chromatic")
            continue;

        prop_scale_->appendEnum(gensym(s->name().c_str()));
    }

    prop_mode_ = new SymbolEnumProperty("@mode", { str_snap, str_skip });
    addProperty(prop_mode_);

    root_ = new PropertyPitch("@root");
    root_->setArgIndex(0);
    addProperty(root_);

    createOutlet();
}

void MidiModus::onFloat(t_float f)
{
    t_float res = 0;

    if (mapNote(f, res) == OK)
        floatTo(0, res);
    else
        floatTo(1, f);
}

void MidiModus::onList(const AtomList& lst)
{
    if (!checkArgs(lst.view(), ARG_BYTE) && !checkArgs(lst.view(), ARG_BYTE, ARG_BYTE) && !checkArgs(lst.view(), ARG_BYTE, ARG_BYTE, ARG_FLOAT)) {
        OBJ_ERR << "PITCH VEL? [DUR] expected, got: " << lst;
        return;
    }

    t_float res = 0;
    if (mapNote(lst[0].asT<t_float>(), res) != OK)
        return listTo(1, lst);

    Atom msg[3] = { res };
    if (lst.size() > 1)
        msg[1] = lst[1];
    if (lst.size() > 2)
        msg[2] = lst[2];

    listTo(0, AtomListView(msg, lst.size()));
}

MidiModus::NoteStatus MidiModus::mapNote(t_float note, t_float& res) const
{
    if (note < 0 || note > 127) {
        OBJ_ERR << "invalid pitch: " << note;
        return INVALID_PITCH;
    }

    if (!scale_) {
        OBJ_ERR << "invalid scale: " << prop_scale_->value();
        return INVALID_SCALE;
    }

    const int root = root_->pitch().absolutePitch();
    const t_float step = std::fmod(note + 12 - root, 12);
    const bool oct_wrap = std::fmod(note, 12) < root;
    //    OBJ_DBG << "note=" << (int)note << ",root=" << root << ",step=" << step;

    switch (crc32_hash(prop_mode_->value())) {
    case hash_snap: {
        t_float degree = 0;
        if (scale_->findNearest(step, degree)) {
            const int oct = (static_cast<int>(note) / 12) - oct_wrap;
            res = degree + root + oct * 12;
            return OK;
        }

        break;
    }
    case hash_skip:
    default:
        if (scale_->find(step)) {
            res = note;
            return OK;
        }
        break;
    }
    return SKIP_NOTE;
}

void setup_midi_modus()
{
    ObjectFactory<MidiModus> obj("midi.modus");

    obj.setXletsInfo({ "float: PITCH\n"
                       "list: PITCH VEL DUR?" },
        { "float or list: accepted note", "float or list: discarded note" });
}
