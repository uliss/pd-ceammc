/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef ARRAY_LTCPLAY_TILDE_H
#define ARRAY_LTCPLAY_TILDE_H

#ifdef WITH_LIBLTC
#include "array_base.h"
#include "ceammc_property_enum.h"

struct LTCDecoder;
using LTCDecoderPtr = std::unique_ptr<LTCDecoder, int (*)(LTCDecoder*)>;
struct LTCFrameExt;

class ArrayLtcPlayTilde : public ArraySoundBase {
    LTCDecoderPtr decoder_;
    EnumProperty<Atom>* fps_ { nullptr };
    size_t off_;
    size_t read_pos_, prev_frame_samp_pos_;
    double frame_rate_;
    int prev_frame_;
    bool zero_speed_;

public:
    ArrayLtcPlayTilde(const PdArgs& args);
    ~ArrayLtcPlayTilde();
    void initDone() final;

    void setupDSP(t_signal** sig) final;
    void processBlock(const t_sample** in, t_sample** out) final;

private:
    bool fpsAutoDetect() const { return fps_->value().isSymbol(); }
    float samplesPerFrame() const { return samplerate() / frame_rate_; }
};

void setup_array_ltcplay_tilde();

#else
#include "ceammc_stub.h"

AUDIO_OBJECT_STUB(ArrayLtcPlayTilde, 1, 1, 0, 0, "compiled without libltc support")
OBJECT_STUB_SETUP(ArrayLtcPlayTilde, array_ltcplay_tilde, "array.ltcplay~")

#endif

#endif // ARRAY_LTCPLAY_TILDE_H
