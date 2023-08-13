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
#include "ceammc_factory.h"
#include "ceammc_signal.h"
#include "ceammc_sound.h"
#include "ceammc_units.h"
#include "fmt/core.h"
#include "snd_play_tilde.h"

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
{
    n_ = new IntProperty("@n", 2, PropValueAccess::INITONLY);
    n_->checkClosedRange(1, 32);
    n_->setArgIndex(0);
    addProperty(n_);

    fname_ = new SymbolProperty("@name", &s_);
    fname_->setArgIndex(1);
    addProperty(fname_);

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
    if (f == 1) {
        if (fname_->value() == &s_) {
            OBJ_ERR << "empty filename";
            return;
        }

        runTask();
    } else
        setQuit(true);
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

    onFloat(lv.boolAt(0, true));
}

void SndPlayTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("b?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    onFloat(!lv.boolAt(0, true));
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
    const int nch = n_->value();
    const int sr = samplerate();
    const int bs = blockSize();
    auto begin = time_begin_;
    auto end = time_end_;

    return std::async([this, fname, nch, sr, bs, begin, end]() mutable {
        auto f = sound::SoundFileFactory::openRead(fname);
        if (!f) {
            logger_.error(fmt::format("can't read '{}'", fname));
            return;
        }

        file_begin_ = 0;
        if (!calcBeginSfPos(begin, f->sampleRate(), f->sampleCount(), file_begin_))
            return logger_.error(fmt::format("invalid begin position: {}, expected value in [{} ... {}) range", file_begin_, 0, f->sampleCount()));

        file_end_ = 0;
        if (!calcEndSfPos(end, f->sampleRate(), f->sampleCount(), file_begin_, file_end_))
            return logger_.error(fmt::format("invalid end position: {}, expected value in [{} ... {}] range", file_end_, file_begin_, f->sampleCount()));

        // store file info
        src_samplerate_ = f->sampleRate();
        src_frames_ = f->sampleCount();

        logger_.debug(fmt::format("start playing from {} to {} samp, loop={}, samples={}", file_begin_, file_end_, (bool)atomic_loop_, f->sampleCount()));

        const auto NUM_CH = std::min<size_t>(nch, f->channels());
        //        const double SAMP_SPEED = f->sampleRate() / double(sr);
        const auto BLOCK_TIME_USEC = std::chrono::microseconds(1000000 * bs / sr);

        constexpr int BUF_SIZE = 4096;
        using Buffer = std::array<t_word, BUF_SIZE>;
        using BufferList = std::vector<Buffer>;

        BufferList buf(NUM_CH);

        double buf_phase = 0;
        file_cur_pos_ = file_begin_;
        setQuit(false);

        while (!quit()) {
            std::int64_t nframes = 0;

            // read samples from file to buffers
            // then read buffers (maybe with different speed) and push to queue
            for (size_t i = 0; i < NUM_CH; i++) {
                // read channel by channel
                // expected that each channel contains same number of samples ;)
                nframes = f->read(buf[i].data(), buf[i].size(), i, file_cur_pos_, buf[i].size());
                if (nframes <= 0)
                    break; // break reading
            }

            // read error
            if (nframes < 0) {
                logger_.error(fmt::format("'{}': read error", fname));
                break; // quit
            } else if (nframes == 0) { // no samples (eof)
                if (!atomic_loop_)
                    break; // quit playing

                // reset read pos
                buf_phase = 0;
                seekToBeg();
                eventLoop(subscriberId());
                continue; // start new loop
            } else if (nframes > 1) {
                const auto UP_LIMIT = nframes - 1;
                // keep buf_phase in [0, UP_LIMIT) range
                buf_phase = std::fmod(buf_phase, UP_LIMIT);

                // write samples from buffers to Pd
                while (buf_phase < UP_LIMIT) {
                    for (size_t c = 0; c < nch && !quit(); c++) {
                        // wait while queue is full
                        while (!quit() && outPipe().write_available() == 0)
                            std::this_thread::sleep_for(BLOCK_TIME_USEC);

                        // push one channel sample
                        if (c < NUM_CH) {
                            // split buffer phase to fractional and integer part
                            double fip;
                            auto fp = std::modf(buf_phase, &fip);
                            size_t idx = fip;
                            // read interpolated value from buffer
                            auto value = interpolate::linear<float>(buf[c][idx].w_float, buf[c][idx + 1].w_float, fp);
                            outPipe().push((atomic_speed_ > 0) ? value : 0); // if pause - output 0
                        } else
                            outPipe().push(0);
                    }

                    buf_phase += atomic_speed_;
                }

                // move read position up one sample less number of frames for linear interpolation
                file_cur_pos_ += UP_LIMIT;
            } else {
                // one last sample
                assert(nframes == 1);

                // keep buf_phase in [0, 1) range
                buf_phase = std::fmod(buf_phase, 1);

                // write samples from buffers to Pd
                while (buf_phase < 1) {
                    for (size_t c = 0; c < nch && !quit(); c++) {
                        // wait while queue is full
                        while (!quit() && outPipe().write_available() == 0)
                            std::this_thread::sleep_for(BLOCK_TIME_USEC);

                        // push one channel sample
                        if (c < NUM_CH) {
                            // read interpolated value from buffer
                            auto value = interpolate::linear<float>(buf[c][0].w_float, 0, buf_phase);
                            outPipe().push((atomic_speed_ > 0) ? value : 0); // if pause - output 0
                        } else
                            outPipe().push(0);
                    }

                    buf_phase += atomic_speed_;
                }

                if (!atomic_loop_)
                    break; // quit playing

                seekToBeg(); // reset read position
                buf_phase = 0;
                eventLoop(subscriberId());
                continue;
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
        break;
    case EVENT_LOOP:
        anyTo(last_out_idx, gensym("loop"), AtomListView());
        break;
    default:
        break;
    }
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
