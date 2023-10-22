/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "midi_tuning.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_music_temperament.h"
#include "ceammc_music_theory_pitch_class.h"

#include <algorithm>

using TPair = std::pair<t_symbol*, music::TemperamentType>;
static std::vector<TPair> TEMPS;

MidiTuning::MidiTuning(const PdArgs& args)
    : BaseObject(args)
    , deviations_ { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    , base_(nullptr)
{
    createOutlet();

    base_ = new IntProperty("@base", 9);
    base_->checkClosedRange(0, 11);
    addProperty(base_);

    createCbListProperty(
        "@dev",
        [this]() -> AtomList {
            AtomList res;
            res.reserve(12);
            for (auto d : deviations_)
                res.append(d);

            return res; },
        [this](const AtomListView& lv) -> bool {
            if (lv.isSymbol()) {
                const auto* s = lv.asT<t_symbol*>();
                auto it = std::find_if(TEMPS.cbegin(), TEMPS.cend(), [s](const TPair& p) { return p.first == s; });
                if (it == TEMPS.end()) {
                    Error log(this);
                    log.stream() << "unknown tuning name: " << lv << ", valid values are: ";
                    for (auto& t : TEMPS)
                        log << t.first->s_name << ' ';

                    return false;
                } else {
                    const auto temp = it->second;
                    const music::PitchClass base(base_->value());
                    for (size_t i = 0; i < deviations_.size(); i++)
                        deviations_[i] = music::to_deviation(i, temp, base);

                    return true;
                }
            }

            const auto N = std::min<size_t>(12, lv.size());
            if (lv.size() > 12)
                Error(this).stream() << "too many values in deviation list: " << lv.size();

            for (size_t i = 0; i < N; i++)
                deviations_[i] = lv.floatAt(i, 0);

            return true;
        })
        ->setArgIndex(0);
}

void MidiTuning::onList(const AtomListView& lv)
{
    if (lv.size() != 2) {
        OBJ_ERR << "NOTE VELOCITY pair expected, got: " << lv;
        return;
    }

    const auto note = lv.intAt(0, -1);
    const auto vel = lv.floatAt(1, -1);

    if (note < 0 || vel < 0) {
        OBJ_ERR << "unexpected note/velocity values: " << note << ' ' << vel;
        return;
    }

    StaticAtomList<2> res { t_float(note + (deviations_[note % 12]) * 0.01), vel };
    listTo(0, res.view());
}

void setup_midi_tuning()
{
    TEMPS.push_back({ gensym("eq"), music::TemperamentType::EQT_12_PERFECT_OCTAVE });
    TEMPS.push_back({ gensym("just"), music::TemperamentType::JUST });
    TEMPS.push_back({ gensym("ganassi"), music::TemperamentType::GANASSI_1543 });
    TEMPS.push_back({ gensym("meantone"), music::TemperamentType::MEANTONE });
    TEMPS.push_back({ gensym("kirnberger3"), music::TemperamentType::KIRNBERGER_III_1779 });
    TEMPS.push_back({ gensym("pythagorean"), music::TemperamentType::PYTHAGOREAN });
    TEMPS.push_back({ gensym("rameau"), music::TemperamentType::RAMEAU });
    TEMPS.push_back({ gensym("valotti"), music::TemperamentType::VALOTTI_1754 });
    TEMPS.push_back({ gensym("zarlino"), music::TemperamentType::ZARLINO_2_7 });

    ObjectFactory<MidiTuning> obj("midi.tuning");

    obj.setDescription("apply tuning to midi messages");
    obj.setCategory("midi");
    obj.setKeywords({"midi", "tuning"});
}
