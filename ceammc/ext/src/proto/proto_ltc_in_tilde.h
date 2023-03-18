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
#ifndef MISC_LTC_IN_TILDE_H
#define MISC_LTC_IN_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_sound_external.h"
#ifdef WITH_LIBLTC

using namespace ceammc;

struct LTCDecoder;
using LTCDecoderPtr = std::unique_ptr<LTCDecoder, int (*)(LTCDecoder*)>;

struct LTCFrameExt;
struct LtcDecoderData {
    float volume;
    std::uint8_t year, month, day;
    std::uint8_t hour, min, sec, frame;
};

class LtcInTilde : public SoundExternal {
    LTCDecoderPtr decoder_;
    BoolProperty* use_date_;
    size_t off_;
    ClockLambdaFunction clock_;
    LtcDecoderData data_;

public:
    LtcInTilde(const PdArgs& args);
    ~LtcInTilde();

    void setupDSP(t_signal** sp) final;
    void processBlock(const t_sample** in, t_sample** out) final;

private:
    void updateData(const LTCFrameExt& frame);
    void outputData();
};

void setup_proto_ltcin_tilde();

#else
#include "ceammc_stub.h"

AUDIO_OBJECT_STUB(1, 0, 0, 1, "compiled without libltc support")
OBJECT_STUB_SETUP("proto.ltc.in~", proto_ltcin_tilde, "ltc.in~");
#endif

#endif // MISC_LTC_IN_TILDE_H
