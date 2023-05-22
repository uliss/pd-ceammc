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
#include "snd_play_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_signal.h"
#include "ceammc_sound.h"
#include "ceammc_units.h"
#include "fmt/core.h"

constexpr int EVENT_DONE = 0;

SndPlayTilde::SndPlayTilde(const PdArgs& args)
    : SndPlayBase(args)
    , time_begin_(0)
    , time_end_(-1)
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
        [this](const Atom& a) -> bool { begin_ = a; return true; });
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

        begin.setSamplerate(f->sampleRate());
        std::int64_t begin_pos = begin.toSamples() + begin.endOffset() * f->sampleCount();
        begin_pos = clip_min<typeof(begin_pos)>(begin_pos, 0);
        if (begin_pos >= f->sampleCount()) {
            logger_.error(fmt::format("invalid begin position: {}, expected value < {}", begin_pos, f->sampleCount()));
            return;
        }

        logger_.debug(fmt::format("start playing at {} samp, loop={}, samples={}", begin_pos, atomic_loop_, f->sampleCount()));

        //        end.setSamplerate(f->sampleRate());
        //        auto end_pos = clip_max<int>() std::numeric_limits<std::int64_t>::max();
        //        const auto end_samp = end.toSamples();
        //        if (end.endOffset())
        //            end_pos = f->sampleCount() - end_samp;
        //        else if(end_samp > 0)
        //            end_pos = end_samp;

        //        end_pos = clip_min<typeof(end_pos)>(end_pos, 0);
        //        if (end_pos >= f->sampleCount()) {
        //            logger_.error(fmt::format("invalid end position: {}", end_pos));
        //            setQuit(true);
        //            return;
        //        }

        const auto NUM_CH = std::min<size_t>(nch, f->channels());
        const double SAMP_SPEED = f->sampleRate() / double(sr);
        const auto SLEEP_TIME = std::chrono::microseconds(1000000 * bs / sr);

        constexpr int BUF_SIZE = 4096;
        using Buffer = std::array<t_word, BUF_SIZE>;
        using BufferList = std::vector<Buffer>;

        BufferList data(NUM_CH);

        double buf_phase = 0;
        setQuit(false);

        while (!quit()) {
            std::int64_t nsamp = 0;

            // read samples from file to buffers
            for (size_t i = 0; i < NUM_CH; i++) {
                nsamp = f->read(data[i].data(), data[i].size(), i, begin_pos, data[i].size());
                if (nsamp < 0) { // read error
                    logger_.error(fmt::format("'{}': read error", fname));
                    break;
                } else if (nsamp == 0) // no more samples
                    break;
            }

            // check error
            if (nsamp < 0)
                break;
            else if (nsamp <= 1) { // last single sample or eof
                if (nsamp == 1) {
                    while (!quit() && outPipe().write_available() == nch)
                        std::this_thread::sleep_for(SLEEP_TIME); // sleep for one block period

                    for (size_t c = 0; c < nch && !quit(); c++)
                        outPipe().push(data[c][0].w_float);
                }

                if (!atomic_loop_) { // single run
                    break;
                } else {
                    begin_pos = 0;
                    buf_phase = 0;
                    continue;
                }
            }

            begin_pos += (nsamp - 1); // +1 for linear interpolation
            buf_phase = std::fmod(buf_phase, (nsamp - 1));

            // write samples from buffers to Pd
            while (buf_phase < (nsamp - 1)) {
                for (size_t c = 0; c < nch && !quit(); c++) {
                    // check queue is full
                    while (!quit() && outPipe().write_available() == 0)
                        std::this_thread::sleep_for(SLEEP_TIME); // sleep for one block period

                    if (c < NUM_CH) {
                        double fip;
                        auto fp = std::modf(buf_phase, &fip);
                        size_t idx = fip;
                        auto value = interpolate::linear<float>(data[c][idx].w_float, data[c][idx + 1].w_float, fp);
                        outPipe().push(value);
                    } else
                        outPipe().push(0);
                }

                buf_phase += (atomic_speed_ + SAMP_SPEED);
            }
        }

        f->close();
        Dispatcher::instance().send({ subscriberId(), EVENT_DONE });
    });
}

void SndPlayTilde::processTask(int event)
{
    if (event == EVENT_DONE)
        bangTo(numOutlets() - 1);
}

void setup_snd_play_tilde()
{
    SoundExternalFactory<SndPlayTilde> obj("snd.play~", OBJECT_FACTORY_DEFAULT);
}
