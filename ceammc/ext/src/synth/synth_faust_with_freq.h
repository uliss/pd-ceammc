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
#ifndef SYNTH_FAUST_WITH_FREQ_H
#define SYNTH_FAUST_WITH_FREQ_H

#include "ceammc_convert.h"
#include "ceammc_faust.h"
#include "ceammc_object.h"

using namespace ceammc;

template <class T>
class SynthWithFreq : public T {
public:
    t_symbol* SYM_NOTE;

protected:
    ceammc::faust::UIProperty* pitch_;
    ceammc::faust::UIProperty* gate_;

public:
    SynthWithFreq(const PdArgs& args)
        : T(args)
        , SYM_NOTE(gensym("note"))
        , pitch_(static_cast<ceammc::faust::UIProperty*>(T::property(gensym("@pitch"))))
        , gate_(static_cast<ceammc::faust::UIProperty*>(T::property(gensym("@gate"))))
    {
        if (!pitch_ || !gate_)
            OBJ_ERR << "dev error: @pitch and @gate property not found";
        else
            T::createCbFloatProperty(
                "@freq",
                [this]() -> t_float { return convert::midi2freq(pitch_->value()); },
                [this](t_float f) -> bool {
                    pitch_->setValue(convert::freq2midi(f), true);
                    return true;
                })
                ->setUnits(PropValueUnits::HZ);
    }

    void onList(const AtomListView& lv) override
    {
        if (lv.size() != 2) {
            OBJ_ERR << "list: NOTE VEL expected, got: " << lv;
            return;
        }

        m_note(SYM_NOTE, lv);
    }

    const char* annotateInlet(size_t) const override
    {
        return "list: NOTE VEL\n"
               "note NOTE VEL";
    }

    const char* annotateOutlet(size_t) const override
    {
        return "synth output";
    }

    void m_note(t_symbol* s, const AtomListView& lv)
    {
        if (!pitch_ || !gate_) {
            METHOD_ERR(s) << "dev error: @pitch and @gate property not found";
            return;
        }

        if (lv.size() != 2 && !lv.allOf(isFloat)) {
            METHOD_ERR(s) << "usage: " << s->s_name << " NOTE VELOCITY";
            return;
        }

        const auto note = lv.floatAt(0, -1);
        const auto vel = lv.floatAt(1, -1);
        const auto nmin = pitch_->infoT().minFloat();
        const auto nmax = pitch_->infoT().maxFloat();

        if (note < nmin || note > nmax) {
            METHOD_ERR(s) << "note value is out of [" << nmin << ".." << nmax << "] range: " << note;
            return;
        }

        if (vel < 0 || vel > 127) {
            METHOD_ERR(s) << "velocity value is out of [0..127] range: " << vel;
            return;
        }

        pitch_->setValue(note, true);
        gate_->setValue(convert::lin2lin_clip<t_float, 0, 127>(vel, 0, 1));
    }
};

#endif // SYNTH_FAUST_WITH_FREQ_H
