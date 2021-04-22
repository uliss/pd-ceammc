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
#ifndef AN_AUBIO_TEMPO_TILDE_H
#define AN_AUBIO_TEMPO_TILDE_H

#include "aubio_base.h"
#include "ceammc_clock.h"
#include "ceammc_property_callback.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class AubioTempoTilde : public SoundExternal {
    IntProperty* buffer_size_;
    HopSizeProperty* hop_size_;
    OnsetMethodProperty* method_;
    OnsetFloatProperty* threshold_;
    OnsetFloatProperty* silence_threshold_;
    bool active_;

    int dsp_pos_;
    t_float last_period_ms_;
    t_float last_bpm_;
    t_float last_confidence_;

    FVecPtr in_, out_;
    TempoPtr tempo_;

    ClockLambdaFunction tick_;

public:
    AubioTempoTilde(const PdArgs& args);
    ~AubioTempoTilde();

    void initDone() final;

    void processBlock(const t_sample** in, t_sample** out) final;
    void samplerateChanged(size_t sr) final;

private:
    void clock_tick();
    void resetAubioTempo(uint_t sr);
    void saveSteadyProperties();
    void restoreSteadyProperties();
};

void setup_an_tempo_tilde();

#endif // AN_AUBIO_TEMPO_TILDE_H
