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
#include "misc_ltcout_tilde.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include "ltc.h"

#include <limits>

namespace {
static inline t_sample toSample(ltcsnd_sample_t v)
{
    constexpr t_sample IN_MAX = std::numeric_limits<ltcsnd_sample_t>::max();
    return (2 * v / IN_MAX) - 1;
}

}

LtcOutTilde::LtcOutTilde(const PdArgs& args)
    : SoundExternal(args)
    , encoder_(nullptr, &ltc_encoder_free)
    , smpte_(new SMPTETimecode)
    , on_(nullptr)
    , volume_(nullptr)
    , speed_(nullptr)
    , buf_beg_(nullptr)
    , buf_end_(nullptr)
{
    createSignalOutlet();

    encoder_.reset(ltc_encoder_create(1, 1, LTC_TV_625_50, LTC_USE_DATE));

    const char timezone[6] = "+0100";
    auto st = smpte_.get();
    strcpy(st->timezone, timezone);
    st->years = 0;
    st->months = 0;
    st->days = 0;
    st->hours = 0;
    st->mins = 0;
    st->secs = 0;
    st->frame = 0;

    on_ = new BoolProperty("@on", false);
    addProperty(on_);

    volume_ = new FloatProperty("@volume", -3);
    volume_->checkClosedRange(-42, 0);
    volume_->setSuccessFn([this](Property*) {
        if (encoder_)
            ltc_encoder_set_volume(encoder_.get(), volume_->value());
    });
    volume_->setUnitsDb();
    addProperty(volume_);

    speed_ = new FloatProperty("@speed", 1);
    speed_->checkClosedRange(0.25, 4);
    addProperty(speed_);
}

void LtcOutTilde::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    ltc_encoder_set_buffersize(encoder_.get(), samplerate(), 25);
    ltc_encoder_reinit(encoder_.get(), samplerate(), 25, LTC_TV_625_50, LTC_USE_DATE);

    //        ltc_encoder_set_filter(encoder_.get(), 10);
    ltc_encoder_set_filter(encoder_.get(), 0);
    ltc_encoder_set_volume(encoder_.get(), volume_->value());

    ltc_encoder_set_timecode(encoder_.get(), smpte_.get());
    cnt_ = 0;

    buf_beg_ = nullptr;
    buf_end_ = nullptr;
}

LtcOutTilde::~LtcOutTilde() = default;

void LtcOutTilde::processBlock(const t_sample** in, t_sample** out)
{
    auto BS = blockSize();
    auto on = on_->value();

    for (size_t i = 0; i < BS; i++) {
        if (bufferEmpty()) {
            flushBuffer();
            encodeFrame();
            if (on)
                frameInc();

            updateBuffer();
        }

        if (!bufferEmpty()) {
            out[0][i] = toSample(bufferFront());
            bufferNext();
        } else {
            out[0][i] = 0;
        }
    }
}

void LtcOutTilde::onFloat(t_float t)
{
    on_->setValue(t > 0);
}

void LtcOutTilde::dump() const
{
    SoundExternal::dump();

    SMPTETimecode tc;
    ltc_encoder_get_timecode(encoder_.get(), &tc);

    OBJ_POST << fmt::format("SMPTE: {:02d}.{:02d}.{:02}{} {:02d}:{:02d}:{:02d}.{:02}",
        tc.years, tc.months, tc.days, tc.timezone, tc.hours, tc.mins, tc.secs, tc.frame);
}

void LtcOutTilde::encodeFrame()
{
    auto e = encoder_.get();
    auto v = speed_->value();

    for (int i = 0; i < 10; i++)
        ltc_encoder_encode_byte(e, i, v);
}

void LtcOutTilde::updateBuffer()
{
    ltcsnd_sample_t* buf = nullptr;
    auto n = ltc_encoder_get_bufferptr(encoder_.get(), &buf, 0);
    buf_beg_ = buf;
    buf_end_ = buf + n;

    static_assert(std::is_same<std::uint8_t, ltcsnd_sample_t>::value, "");
}

void LtcOutTilde::flushBuffer()
{
    ltc_encoder_buffer_flush(encoder_.get());
}

void LtcOutTilde::frameInc()
{
    ltc_encoder_inc_timecode(encoder_.get());
}

void setup_misc_ltcout_tilde()
{
    LIB_DBG << fmt::format("libltc version: {}", LIBLTC_VERSION);

    SoundExternalFactory<LtcOutTilde> obj("misc.ltcout~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("ltc.in~");
}
