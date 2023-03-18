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
#include "proto_ltcout_tilde.h"
#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include "ltc.h"

#include <limits>

constexpr t_float PROP_SPEED_DEF = 1;
constexpr t_float PROP_SPEED_MIN = 1 / 4.0;
constexpr t_float PROP_SPEED_MAX = 4;
constexpr t_float PROP_VOLUME_DEF = -3;
constexpr t_float PROP_VOLUME_MIN = -42;
constexpr t_float PROP_VOLUME_MAX = 0;
constexpr t_float PROP_FPS_DEF = 25;
constexpr t_float PROP_FILTER_MIN_MS = 0;
constexpr t_float PROP_FILTER_MAX_MS = 500;
constexpr t_float PROP_FILTER_DEF_MS = 40;

namespace {

inline t_sample toSample(ltcsnd_sample_t v)
{
    constexpr t_sample IN_MAX = std::numeric_limits<ltcsnd_sample_t>::max();
    return (2 * v / IN_MAX) - 1;
}

inline LTC_TV_STANDARD toTvStandard(float fps)
{
    if (fps == 24)
        return LTC_TV_FILM_24;
    else if (fps == 25)
        return LTC_TV_625_50;
    else
        return LTC_TV_525_60;
}

}

LtcOutTilde::LtcOutTilde(const PdArgs& args)
    : SoundExternal(args)
    , encoder_(nullptr, &ltc_encoder_free)
    , on_(nullptr)
    , volume_(nullptr)
    , speed_(nullptr)
    , fps_(nullptr)
    , filter_(nullptr)
    , buf_beg_(nullptr)
    , buf_end_(nullptr)
    , err_clock_([this]() {
        OBJ_ERR << "LTC buffer overtflow";
    })
{
    createSignalOutlet();
    createOutlet();

    encoder_.reset(ltc_encoder_create(1, PROP_FPS_DEF, toTvStandard(PROP_FPS_DEF), LTC_USE_DATE));
    setTime(0, 0, 0, 0);

    on_ = new BoolProperty("@on", false);
    addProperty(on_);

    volume_ = new FloatProperty("@volume", PROP_VOLUME_DEF);
    volume_->checkClosedRange(PROP_VOLUME_MIN, PROP_VOLUME_MAX);
    volume_->setSuccessFn([this](Property*) {
        if (encoder_)
            ltc_encoder_set_volume(encoder_.get(), volume_->value());
    });
    volume_->setUnitsDb();
    addProperty(volume_);

    speed_ = new FloatProperty("@speed", PROP_SPEED_DEF);
    speed_->setFloatCheckFn([this](t_float v) -> bool {
        auto x = std::abs(v);
        return PROP_SPEED_MIN <= x && x <= PROP_SPEED_MAX;
    });
    addProperty(speed_);

    fps_ = new FloatProperty("@fps", PROP_FPS_DEF);
    fps_->setFloatCheckFn([this](t_float v) -> bool {
        return (v == 24 || v == 25 || v == 30 || (std::round(100 * v) == 2997));
    },
        "fps set error");
    addProperty(fps_);

    filter_ = static_cast<CallbackProperty*>(createCbFloatProperty(
        "@filter",
        [this]() -> t_float {
            auto enc = encoder_.get();
            return enc ? ltc_encoder_get_filter(enc) : 0;
        },
        [this](t_float v) -> bool {
            auto enc = encoder_.get();
            if (enc) {
                OBJ_DBG << "set filter: " << v;
                ltc_encoder_set_filter(enc, v);
                return true;
            } else
                return false;
        }));
    filter_->checkClosedRange(PROP_FILTER_MIN_MS, PROP_FILTER_MAX_MS);
}

void LtcOutTilde::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    ltc_encoder_set_buffersize(encoder_.get(), samplerate() / PROP_SPEED_MIN, fps_->value());
    ltc_encoder_reinit(encoder_.get(), samplerate(), fps_->value(), toTvStandard(fps_->value()), LTC_USE_DATE);

    updateFilter();

    ltc_encoder_set_volume(encoder_.get(), volume_->value());

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
                updateFrame();

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

void LtcOutTilde::onBang()
{
    SMPTETimecode tc;
    ltc_encoder_get_timecode(encoder_.get(), &tc);

    AtomArray<4> data(tc.hours, tc.mins, tc.secs, tc.frame);
    listTo(1, data.view());
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

    OBJ_POST << fmt::format("SMPTE: {:02d}-{:02d}-{:02}{} {:02d}:{:02d}:{:02d}:{:02}",
        tc.years, tc.months, tc.days, tc.timezone, tc.hours, tc.mins, tc.secs, tc.frame);
}

void LtcOutTilde::m_smpte(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("H:i[0,23] M:i[0,59] S:i[0,59] F:i[0,30]?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    setTime(lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0), lv.intAt(3, 0));
}

void LtcOutTilde::m_date(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("YEAR:i[0,3000] MONTH:i[1,12] DAY:i[1,31] ZONE:s?");
    if (!chk.check(lv, this))
        return chk.usage(this);

    SMPTETimecode tc;
    ltc_encoder_get_timecode(encoder_.get(), &tc);

    auto zone = lv.symbolAt(3, &s_);
    int h = 0, m = 0;
    if (zone != &s_ && 2 != sscanf(zone->s_name, "+%02d%02d", &h, &m)) {
        OBJ_ERR << "timezone in +HHMM format expected, got: " << zone->s_name;
        return;
    }
    fmt::format_to(tc.timezone, "+{:02}{:02}\0", clip<int, 0, 99>(h % 100), clip<int, 0, 59>(m));
    tc.years = lv.intAt(0, 0);
    tc.months = lv.intAt(1, 0);
    tc.days = lv.intAt(2, 0);

    ltc_encoder_set_timecode(encoder_.get(), &tc);
}

void LtcOutTilde::setTime(std::uint8_t hour, std::uint8_t min, std::uint8_t sec, std::uint8_t frame)
{
    SMPTETimecode tc;
    fmt::format_to(tc.timezone, "+{:02}{:02}\0", 0, 0);
    tc.years = 0;
    tc.months = 0;
    tc.days = 0;
    tc.hours = hour;
    tc.mins = min;
    tc.secs = sec;
    tc.frame = frame;

    ltc_encoder_set_timecode(encoder_.get(), &tc);
}

void LtcOutTilde::encodeFrame()
{
    auto e = encoder_.get();
    auto v = 1 / speed_->value();

    for (int i = 0; i < 10; i++) {
        auto rc = ltc_encoder_encode_byte(e, i, std::abs(v));
        if (rc != 0) {
            err_clock_.delay(0);
            return;
        }
    }
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

void LtcOutTilde::updateFrame()
{
    if (speed_->value() > 0)
        ltc_encoder_inc_timecode(encoder_.get());
    else
        ltc_encoder_dec_timecode(encoder_.get());
}

void LtcOutTilde::updateFilter()
{
    filter_->set(filter_->get());
}

void setup_proto_ltcout_tilde()
{
    LIB_DBG << fmt::format("libltc version: {}", LIBLTC_VERSION);

    SoundExternalFactory<LtcOutTilde> obj("proto.ltc.out~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("ltc.out~");
    obj.addMethod("smpte", &LtcOutTilde::m_smpte);
    obj.addMethod("date", &LtcOutTilde::m_date);
}
