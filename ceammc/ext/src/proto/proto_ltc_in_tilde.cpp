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
#include "proto_ltc_in_tilde.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#ifdef WITH_LIBLTC
#include "ltc.h"

LtcInTilde::LtcInTilde(const PdArgs& args)
    : SoundExternal(args)
    , decoder_(ltc_decoder_create(2400, 2), &ltc_decoder_free)
    , use_date_(nullptr)
    , off_(0)
    , clock_([this]() { outputData(); })
    , prev_frame_(0)
    , frame_rate_(25)
{
    createOutlet();
    createOutlet();

    {
        auto p = createCbFloatProperty("@volume",
            [this]() -> t_float { return clip<t_float, -60, 0>(data_.volume); });
        p->setUnitsDb();
        p->infoT().setConstraints(PropValueConstraints::CLOSED_RANGE);
        (void)p->infoT().setRangeFloat(-60, 0);
    }

    use_date_ = new BoolProperty("@use_date", true);
    addProperty(use_date_);

    createCbIntProperty("@fps", [this]() -> int { return frame_rate_; });
}

LtcInTilde::~LtcInTilde() = default;

void LtcInTilde::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    off_ = 0;
    prev_frame_ = 0;
    ltc_decoder_queue_flush(decoder_.get());
}

void LtcInTilde::processBlock(const t_sample** in, t_sample** out)
{
    auto dec = decoder_.get();
    auto BS = blockSize();

#if PD_FLOATSIZE == 32
    ltc_decoder_write_float(dec, const_cast<float*>(in[0]), BS, off_);
#elif PD_FLOATSIZE == 64
    ltc_decoder_write_double(dec, const_cast<double*>(in[0]), BS, off_);
#endif

    off_ += BS;

    LTCFrameExt f;
    while (ltc_decoder_read(dec, &f)) {
        if (updateData(f))
            clock_.delay(0);
    }
}

bool LtcInTilde::updateData(const LTCFrameExt& frame)
{
#define ON_CHANGE(a, b)    \
    {                      \
        if (a != b) {      \
            a = b;         \
            update = true; \
        }                  \
    }

    bool update = false;
    // volume has fluctation even on idle signal, no update
    data_.volume = frame.volume;

    SMPTETimecode tc;
    ltc_frame_to_time(&tc, const_cast<LTCFrame*>(&frame.ltc),
        use_date_->value() ? LTC_USE_DATE : LTC_TC_CLOCK);

    if (tc.frame == 0) {
        if (prev_frame_ == 24 || prev_frame_ == 25 || prev_frame_ == 30) {
            frame_rate_ = prev_frame_;
        }
    }

    prev_frame_ = (tc.frame + 1);

    ON_CHANGE(data_.year, tc.years);
    ON_CHANGE(data_.month, tc.months);
    ON_CHANGE(data_.day, tc.days);
    ON_CHANGE(data_.hour, tc.hours);
    ON_CHANGE(data_.min, tc.mins);
    ON_CHANGE(data_.sec, tc.secs);
    ON_CHANGE(data_.frame, tc.frame);

    return update;
}

void LtcInTilde::outputData()
{
    AtomArray<3> date(data_.year, data_.month, data_.day);
    listTo(1, date.view());

    AtomArray<4> time(data_.hour, data_.min, data_.sec, data_.frame);
    listTo(0, time.view());
}

void setup_proto_ltcin_tilde()
{
    SoundExternalFactory<LtcInTilde> obj("proto.ltc.in~");
    obj.addAlias("ltc.in~");
    obj.setXletsInfo({ "signal: LTC input" }, { "list: HOUR MIN SEC FRAME", "list: YEAR MONTH DAY" });
}
#endif
