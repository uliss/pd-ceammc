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
#ifndef MISC_LTCOUT_TILDE_H
#define MISC_LTCOUT_TILDE_H

#include "ceammc_property_callback.h"
#include "ceammc_sound_external.h"
#ifdef WITH_LIBLTC

using namespace ceammc;

struct LTCEncoder;
using LTCEncoderPtr = std::unique_ptr<LTCEncoder, void (*)(LTCEncoder*)>;

class LtcOutTilde : public SoundExternal {
    LTCEncoderPtr encoder_;
    BoolProperty* on_;
    FloatProperty* volume_;
    FloatProperty* speed_;
    FloatProperty* fps_;
    CallbackProperty* filter_;
    std::uint8_t *buf_beg_, *buf_end_;

public:
    LtcOutTilde(const PdArgs& args);
    ~LtcOutTilde();

    void setupDSP(t_signal** sp) final;
    void processBlock(const t_sample** in, t_sample** out) final;
    void onBang() override;
    void onFloat(t_float t) override;

    void dump() const override;

    void m_smpte(t_symbol* s, const AtomListView& lv);
    void m_date(t_symbol* s, const AtomListView& lv);

    void setTime(std::uint8_t hour, std::uint8_t min, std::uint8_t sec, std::uint8_t frame);

private:
    bool bufferEmpty() const { return buf_beg_ == buf_end_; }
    std::uint8_t bufferFront() const { return *buf_beg_; }
    void bufferNext() { buf_beg_++; }
    void encodeFrame();
    void flushBuffer();
    void updateBuffer();
    void updateFrame();
    void updateFilter();
};

void setup_proto_ltcout_tilde();

#else
#include "ceammc_stub.h"

AUDIO_OBJECT_STUB(0, 1, 1, 0, "compiled without libltc support")
OBJECT_STUB_SETUP("proto.ltc.out~", proto_ltcout_tilde, "ltc.out~");
#endif

#endif // MISC_LTCOUT_TILDE_H
