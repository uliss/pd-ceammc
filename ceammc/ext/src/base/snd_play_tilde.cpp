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
#include <cassert>
#include <thread>

#include "args/argcheck2.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_sound.h"
#include "ceammc_soxr_resampler.h"
#include "ceammc_units.h"
#include "fmt/core.h"
#include "snd_play_tilde.h"

CEAMMC_DEFINE_SYM_HASH(wait)
CEAMMC_DEFINE_SYM_HASH(now)
CEAMMC_DEFINE_SYM_HASH(defer)

enum {
    EVENT_DONE = 0,
    EVENT_LOOP = 1,
};

namespace {

void eventDone(SubscriberId id)
{
    Dispatcher::instance().send({ id, EVENT_DONE });
}

void eventLoop(SubscriberId id)
{
    Dispatcher::instance().send({ id, EVENT_LOOP });
}

}

SndPlayTilde::SndPlayTilde(const PdArgs& args)
    : SndPlayBase(args)
    , time_begin_(0)
    , time_end_(POS_END)
    , clock_play_([this] {
        if (isRunning()) {
            setQuit(true);
            clock_play_.delay(0);
        } else
            runTask();
    })
{
    n_ = new IntProperty("@n", 2, PropValueAccess::INITONLY);
    n_->checkClosedRange(1, 32);
    n_->setArgIndex(0);
    addProperty(n_);

    fname_ = new SymbolProperty("@name", &s_);
    fname_->setArgIndex(1);
    addProperty(fname_);

    sync_mode_ = new SymbolEnumProperty("@sync", { sym_now(), sym_wait(), sym_defer() });
    addProperty(sync_mode_);

    auto speed = createCbFloatProperty(
        "@speed",
        [this]() -> t_float { return atomic_speed_; },
        [this](t_float f) -> bool { atomic_speed_ = f; return true; });
    speed->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 0.25, 4);

    createCbBoolProperty(
        "@loop",
        [this]() -> t_float { return atomic_loop_; },
        [this](bool x) -> bool { atomic_loop_ = x; return true; });

    auto begin = createCbAtomProperty(
        "@begin",
        [this]() -> Atom { return begin_; },
        [this](const Atom& a) -> bool {
        begin_ = a;
        return true; });
    begin->setAtomCheckFn([this](const Atom& a) -> bool {
        units::UnitParseError err;
        if (!time_begin_.setParsed(a, err)) {
            OBJ_ERR << err.msg;
            return false;
        } else
            return true;
    });
    auto end = createCbAtomProperty(
        "@end",
        [this]() -> Atom { return end_; },
        [this](const Atom& a) -> bool { end_ = a; return true; });
    end->setAtomCheckFn([this](const Atom& a) -> bool {
        units::UnitParseError err;
        if (!time_end_.setParsed(a, err)) {
            OBJ_ERR << err.msg;
            return false;
        } else
            return true;
    });
}

void SndPlayTilde::initDone()
{
    for (int i = 0; i < n_->value(); i++)
        createSignalOutlet();

    createOutlet();
}

void SndPlayTilde::onFloat(t_float f)
{
    start(f == 1);
}

void SndPlayTilde::onSymbol(t_symbol* s)
{
    fname_->setValue(s);
}

void SndPlayTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto NCH = n_->value();
    const auto BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        for (size_t k = 0; k < NCH; k++) {
            t_sample x = 0;
            if (outPipe().read_available())
                outPipe().pop(x);

            out[k][i] = x;
        }
    }
}

void SndPlayTilde::m_start(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("b?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    start(lv.boolAt(0, true));
}

void SndPlayTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("b?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    start(!lv.boolAt(0, true));
}

void SndPlayTilde::m_pause(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("b?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (lv.boolAt(0, true)) { // save current speed
        speed_pause_ = atomic_speed_;
        atomic_speed_ = 0;
    } else { // restore speed
        atomic_speed_ = speed_pause_;
    }
}

void SndPlayTilde::m_ff(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TIME:a?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (taskState() != TASK_RUNNING)
        return;

    using namespace units;

    TimeValue x(0, TimeValue::MS, src_samplerate_);
    auto res = TimeValue::parse(lv.atomAt(0, Atom(1000)));
    if (res.matchValue(x)) {
        file_cur_pos_ += x.toSamples();
        if (file_cur_pos_ < file_begin_)
            file_cur_pos_ = file_begin_;
        else if (file_cur_pos_ > file_end_)
            file_cur_pos_ = file_end_;
    } else {
        OBJ_ERR << res.error().msg;
    }
}

void SndPlayTilde::m_rewind(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("RELTIME:a?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (taskState() != TASK_RUNNING)
        return;

    using namespace units;

    TimeValue x(0, TimeValue::MS, src_samplerate_);
    auto res = TimeValue::parse(lv.atomAt(0, Atom(1000)));
    if (res.matchValue(x)) {
        file_cur_pos_ -= x.toSamples();
        if (file_cur_pos_ < file_begin_)
            file_cur_pos_ = file_begin_;
        else if (file_cur_pos_ > file_end_)
            file_cur_pos_ = file_end_;
    } else {
        OBJ_ERR << res.error().msg;
    }
}

void SndPlayTilde::m_seek(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("POS:a?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (taskState() != TASK_RUNNING)
        return;

    using namespace units;

    TimeValue x(0, TimeValue::MS, src_samplerate_);
    auto res = TimeValue::parse(lv.atomAt(0, Atom(0.)));
    if (res.matchValue(x)) {
        auto samp = x.toSamples();
        if (samp >= 0)
            file_cur_pos_ = samp + file_begin_;

        if (file_cur_pos_ > file_end_)
            file_cur_pos_ = file_end_;

    } else {
        OBJ_ERR << res.error().msg;
    }
}

SndPlayBase::Future SndPlayTilde::createTask()
{
    std::string fname = fname_->str();
    const int out_ch = n_->value();
    const int sr = samplerate();
    const int bs = blockSize();
    auto begin = time_begin_;
    auto end = time_end_;

    return std::async([this, fname, out_ch, sr, bs, begin, end]() mutable {
        auto f = sound::SoundFileFactory::openRead(fname.c_str());
        if (!f) {
            logger_.error(fmt::format("can't read '{}'", fname));
            return;
        }

        logger_.debug(fmt::format("play SR={}, file SR={}, CH={}", sr, f->sampleRate(), f->channels()));

        file_begin_ = 0;
        if (!calcBeginSfPos(begin, sr, f->frameCount(), file_begin_))
            return logger_.error(fmt::format("invalid begin position: {}, expected value in [{} ... {}) range", file_begin_, 0, f->frameCount()));

        file_end_ = 0;
        if (!calcEndSfPos(end, sr, f->frameCount(), file_begin_, file_end_))
            return logger_.error(fmt::format("invalid end position: {}, expected value in [{} ... {}] range", file_end_, file_begin_, f->frameCount()));

        // store file info
        src_samplerate_ = f->sampleRate();
        src_frames_ = f->frameCount();

        logger_.debug(fmt::format("start playing from {} to {} samp, loop={}, samples={}", file_begin_, file_end_, (bool)atomic_loop_, f->frameCount()));

        const auto SAMPLE_USEC = std::chrono::microseconds(1000000 / sr);
        std::array<float, 4096> buf;

        file_cur_pos_ = file_begin_;
        setQuit(false);

        SoxrResampler resampler(f->sampleRate(), sr, f->channels(), SoxrResampler::QUICK, true);
        const auto file_chan = f->channels();

        auto res_fn = [this, file_chan, out_ch, SAMPLE_USEC, &buf](const t_sample* x, size_t rframes) -> bool {
            for (size_t i = 0; i < rframes; i++) {
                while (!quit() && atomic_speed_ == 0)
                    std::this_thread::sleep_for(64 * SAMPLE_USEC);

                while (!quit() && outPipe().write_available() < out_ch)
                    std::this_thread::sleep_for(out_ch * SAMPLE_USEC);

                for (auto c = 0; c < out_ch; c++) {
                    if (c < file_chan) {
                        outPipe().push(x[(i * file_chan) + c]);
                    } else
                        outPipe().push(0);
                }
            }

            return true;
        };

        while (!quit()) {
            // sleep on pause
            while (atomic_speed_ == 0)
                std::this_thread::sleep_for(bs * SAMPLE_USEC);

            // read
            const auto buf_frames = buf.size() / f->channels();
            auto nframes = f->readFrames(buf.data(), buf_frames, file_cur_pos_);

            // read error
            if (nframes < 0) {
                logger_.error(fmt::format("'{}': read error", fname));
                break; // quit
            } else if (nframes == 0) { // no samples (eof)
                resampler.processDone(res_fn);
                resampler.reset();
                if (!atomic_loop_)
                    break; // quit playing

                // reset read pos
                seekToBeg();
                eventLoop(subscriberId());
                continue; // start new loop
            } else /* (nframes > 0) */ {
                while (atomic_speed_ == 0)
                    std::this_thread::sleep_for(bs * SAMPLE_USEC);

                const auto nsamp = file_chan * nframes;
                resampler.setResampleRatio(atomic_speed_);
                resampler.process(buf.data(), nframes, res_fn);
                file_cur_pos_ += nframes;
            }
        }

        f->close();
        eventDone(subscriberId());
    });
}

void SndPlayTilde::processTask(int event)
{
    auto last_out_idx = numOutlets() - 1;

    switch (event) {
    case EVENT_DONE:
        anyTo(last_out_idx, gensym("done"), AtomListView());
        if (defer_play_) {
            defer_play_ = false;
            clock_play_.delay(0);
        }
        break;
    case EVENT_LOOP:
        anyTo(last_out_idx, gensym("loop"), AtomListView());
        if (defer_play_) {
            defer_play_ = false;
            clock_play_.delay(0);
        }
        break;
    default:
        break;
    }
}

void SndPlayTilde::start(bool value)
{
    if (value) {
        if (fname_->value() == &s_) {
            OBJ_ERR << "empty filename";
            return;
        }

        if (isRunning()) {
            switch (crc32_hash(sync_mode_->value())) {
            case hash_now:
                clock_play_.delay(0);
                break;
            case hash_wait:
                OBJ_ERR << "soundfile is playing, wait until it will finished or stop it";
                break;
            case hash_defer:
            default:
                defer_play_ = true;
                break;
            }

        } else {
            runTask();
        }

    } else
        setQuit(true);
}

bool SndPlayTilde::calcBeginSfPos(const units::TimeValue& tm, size_t sr, size_t sampleCount, std::int64_t& result)
{
    auto t = tm;
    t.setSamplerate(sr);
    std::int64_t pos = t.toSamples() + t.endOffset() * sampleCount;
    result = clip_min<typeof(pos)>(pos, 0);
    return result < sampleCount;
}

bool SndPlayTilde::calcEndSfPos(const units::TimeValue& tm, size_t sr, size_t sampleCount, std::int64_t begin, std::int64_t& result)
{
    auto t = tm;
    t.setSamplerate(sr);
    std::int64_t pos = t.toSamples() + t.endOffset() * sampleCount;
    result = pos;
    if (result == POS_END) {
        result = sampleCount;
        return true;
    }

    result = clip_min<std::int64_t>(pos, sampleCount);
    return result > begin;
}

void setup_snd_play_tilde()
{
    SoundExternalFactory<SndPlayTilde> obj("snd.play~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("start", &SndPlayTilde::m_start);
    obj.addMethod("stop", &SndPlayTilde::m_stop);
    obj.addMethod("pause", &SndPlayTilde::m_pause);
    obj.addMethod("ff", &SndPlayTilde::m_ff);
    obj.addMethod("rewind", &SndPlayTilde::m_rewind);
    obj.addMethod("seek", &SndPlayTilde::m_seek);
}
