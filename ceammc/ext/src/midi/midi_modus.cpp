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

#include <algorithm>

CEAMMC_DEFINE_HASH(snap)
CEAMMC_DEFINE_HASH(skip)

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
        prop_scale_->appendEnum(gensym(s->name().c_str()));
    }

    prop_mode_ = new SymbolEnumProperty("@mode", { str_snap, str_skip });
    addProperty(prop_mode_);
}

void MidiModus::onFloat(t_float f)
{
    if (!scale_) {
        OBJ_ERR << "invalid scale: " << prop_scale_->value();
        return;
    }

    const int root = root_.absolutePitch();
    const t_float step = std::fmod(f + 12 - root, 12);

    switch (crc32_hash(prop_mode_->value())) {
    case hash_snap: {
        auto& steps = scale_->all();
        for (size_t i = 1; i < steps.size(); i++) {
            auto a = steps[i - 1];
            auto b = steps[i];

            if (a == step || b == step)
                return floatTo(0, f);
            else if (a < step && step < b) {
                const int oct = f / 12;
                const int fstep = (step - a) < (b - step) ? a : b;
                return floatTo(0, (root + fstep) + oct * 12);
            }
        }
        break;
    }
    case hash_skip:
    default:
        for (auto p : scale_->all()) {
            if (p == step)
                return floatTo(0, f);
        }
        break;
    }
}

void setup_midi_modus()
{
    ObjectFactory<MidiModus> obj("midi.modus");
}
