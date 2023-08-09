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
#include "array_ltcplay_tilde.h"
#include "ceammc_factory.h"

#ifdef WITH_LIBLTC

#include "ltc.h"

static inline void ltc_write_sample(LTCDecoder* dec, const t_sample* in, ltc_off_t offset)
{
#if PD_FLOATSIZE == 32
    ltc_decoder_write_float(dec, const_cast<float*>(in), 1, offset);
#elif PD_FLOATSIZE == 64
    ltc_decoder_write_double(dec, const_cast<double*>(in), 1, offset);
#endif
}

ArrayLtcPlayTilde::ArrayLtcPlayTilde(const PdArgs& args)
    : ArraySoundBase(args)
    , decoder_(nullptr, &ltc_decoder_free)
    , off_(0)
    , read_pos_(0)
    , frame_rate_(25)
    , prev_frame_samp_pos_(std::numeric_limits<typeof(prev_frame_samp_pos_)>::max())

{
    createInlet();
    createSignalOutlet();

    fps_ = new EnumProperty<Atom>("@fps", { Atom(gensym("auto")), Atom(25), Atom(24), Atom(29.97), Atom(30) });
    fps_->setSuccessFn([this](Property*) {
        if (fps_->value().isFloat())
            frame_rate_ = fps_->value().asT<t_float>();
    });
    addProperty(fps_);
}

void ArrayLtcPlayTilde::initDone()
{
    const auto APV = samplerate() / (fpsAutoDetect() ? 25 : std::round(fps_->value().asFloat(25)));
    decoder_.reset(ltc_decoder_create(APV, 2));
}

void ArrayLtcPlayTilde::setupDSP(t_signal** sp)
{
    ArraySoundBase::setupDSP(sp);
    off_ = 0;
    ltc_decoder_queue_flush(decoder_.get());
    prev_frame_ = 0;
    zero_speed_ = false;
}

ArrayLtcPlayTilde::~ArrayLtcPlayTilde() = default;

void ArrayLtcPlayTilde::processBlock(const t_sample** in, t_sample** out)
{
    auto dec = decoder_.get();
    auto BS = blockSize();

    LTCFrameExt frame;

    for (size_t i = 0; i < BS; i++) {
        ltc_write_sample(dec, &in[0][i], off_++);

        if (ltc_decoder_read(dec, &frame)) {
            SMPTETimecode tc;
            ltc_frame_to_time(&tc, const_cast<LTCFrame*>(&frame.ltc), LTC_TC_CLOCK);

            if (fpsAutoDetect()) {
                if (tc.frame == 0) {
                    if (prev_frame_ == 24 || prev_frame_ == 25 || prev_frame_ == 30) {
                        if (frame_rate_ != prev_frame_)
                            frame_rate_ = prev_frame_;
                    }
                }

                prev_frame_ = (tc.frame + 1); // one based frame counter 1-25..
            }

            // NOTE: non-drop framerates only!
            // note: std::rint trick if SR/frame_rate if fractional
            read_pos_ = (((tc.hours * 60 * 60) + (tc.mins * 60) + tc.secs) * samplerate()) + std::rint(tc.frame * samplesPerFrame()); // seconds

            zero_speed_ = (prev_frame_samp_pos_ == read_pos_);
            prev_frame_samp_pos_ = read_pos_;
        }

        if (zero_speed_) {
            out[0][i] = 0;
        } else {
            // note: read_pos_++ only in true if branch
            out[0][i] = (read_pos_ < array_.size()) ? array_.readUnsafe0(read_pos_++) : 0;
        }
    }
}

void setup_array_ltcplay_tilde()
{
    SoundExternalFactory<ArrayLtcPlayTilde> obj("array.ltcplay~");
}

#endif
