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
#include "an_zero_tilde.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include <cstdint>

CEAMMC_DEFINE_HASH(count)
CEAMMC_DEFINE_HASH(rate)
CEAMMC_DEFINE_HASH(freq)

ZeroCrossingTilde::ZeroCrossingTilde(const PdArgs& args)
    : SoundExternal(args)
    , buffer_size_(nullptr)
    , mode_(nullptr)
    , out_(0)
    , t_(0)
{
    buffer_size_ = new IntProperty("@bs", 1024);
    buffer_size_->setInitOnly();
    buffer_size_->checkClosedRange(1, 1024 * 64);
    addProperty(buffer_size_);

    mode_ = new SymbolEnumProperty("@mode", { str_count, str_rate, str_freq });
    addProperty(mode_);

    createCbListProperty(
        "@clock",
        [this]() -> AtomList {
            return AtomList(crc32_hash(mode_->value()) == hash_count && buffer_size_->value() == 1 ? 1.0 : 0.0);
        },
        [this](const AtomListView&) -> bool {
            mode_->setValue(gensym(str_count));
            buffer_size_->setValue(1);
            return true; } //
        )
        ->setInitOnly();

    createSignalOutlet();
}

void ZeroCrossingTilde::initDone()
{
    buf_.reserve(buffer_size_->value() + 1);
    buf_.push_back(0);
}

void ZeroCrossingTilde::setupDSP(t_signal** s)
{
    SoundExternal::setupDSP(s);
}

void ZeroCrossingTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const size_t bufSize = buffer_size_->value() + 1;
    const t_sample nyqfreq = samplerate() / 2;

    switch (crc32_hash(mode_->value())) {
    case hash_count: // output zero crossing count on block borders

        for (size_t i = 0; i < bs; i++) {
            auto v = in[0][i];

            if (buf_.size() < bufSize)
                buf_.push_back(v);

            // not else(!)
            if (buf_.size() == bufSize) {
                auto b = buf_.back();
                out_ = zeroCrossingCount();
                buf_.clear();
                buf_.push_back(b);
            }

            out[0][i] = out_;
        }

        break;

    case hash_rate:

        for (size_t i = 0; i < bs; i++) {
            auto v = in[0][i];

            if (buf_.size() < bufSize)
                buf_.push_back(v);

            // not else(!)
            if (buf_.size() == bufSize) {
                auto b = buf_.back();
                out_ = zeroCrossingCount() / (buf_.size() - 1);
                buf_.clear();
                buf_.push_back(b);
            }

            out[0][i] = out_;
        }
        break;

    case hash_freq:

        for (size_t i = 0; i < bs; i++) {
            auto v = in[0][i];

            if (buf_.size() < bufSize)
                buf_.push_back(v);

            // not else(!)
            if (buf_.size() == bufSize) {
                auto b = buf_.back();
                out_ = (zeroCrossingCount() / (buf_.size() - 1)) * nyqfreq;
                buf_.clear();
                buf_.push_back(b);
            }

            out[0][i] = out_;
        }
        break;
    default:
        break;
    }
}

double ZeroCrossingTilde::zeroCrossingCount() const
{
    uint32_t zcn = 0;

    for (size_t i = 1; i < buf_.size(); i++) {
        if (zero_crossing(buf_[i - 1], buf_[i]))
            zcn++;
    }

    return zcn;
}

void setup_an_zero_tilde()
{
    SoundExternalFactory<ZeroCrossingTilde> obj("an.zero~");
    obj.setXletsInfo({ "signal: input" }, { "signal: zero crossing count/rate.freq" });
}
