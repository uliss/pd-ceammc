/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "array_play.h"
#include "ceammc_factory.h"

FSM ArrayPlayTilde::fsm_ = {
    StateTransitions {
        // STATE_STOPPED
        [](ArrayPlayTilde* ap) -> PlayState { // stop->stop
            return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // stop->play
            if (ap->checkArray()) {
                return STATE_PLAYING;
            } else
                return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // stop->pause
            Error(ap).stream() << "not playing";
            return STATE_STOPPED;
        },
    },
    StateTransitions {
        // STATE PLAYING
        [](ArrayPlayTilde* ap) -> PlayState { // play->stop
            ap->phase_ = 0;
            return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // play->play
            Error(ap).stream() << "already playing";
            return STATE_PLAYING;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // play->pause
            return STATE_PAUSED;
        },
    },
    StateTransitions {
        // STATE PAUSE
        [](ArrayPlayTilde* ap) -> PlayState { // pause->stop
            ap->phase_ = 0;
            return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // pause->play
            if (ap->checkArray()) {
                return STATE_PLAYING;
            } else
                return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // pause->pause
            Error(ap).stream() << "already paused";
            return STATE_PAUSED;
        },
    },
};

ArrayPlayTilde::ArrayPlayTilde(const PdArgs& args)
    : ArraySoundBase(args)
    , speed_(nullptr)
    , interp_(nullptr)
    , amp_(nullptr)
    , phase_(0)
    , state_(STATE_STOPPED)
    , done_([this]() {
        command(ACT_STOP);
        bangTo(1);
    })
{
    speed_ = new FloatProperty("@speed", 1);
    speed_->setArgIndex(1);
    addProperty(speed_);

    interp_ = new IntEnumProperty("@interp", { INTERP_LIN, INTERP_CUBIC, NO_INTERP });
    addProperty(interp_);

    amp_ = new FloatProperty("@amp", 1);
    amp_->checkNonNegative();
    addProperty(amp_);

    createSignalOutlet();
    createOutlet();
}

void ArrayPlayTilde::onBang()
{
    command(ACT_STOP);
    command(ACT_PLAY);
}

void ArrayPlayTilde::setupDSP(t_signal** sig)
{
    ArraySoundBase::setupDSP(sig);
    if (checkArray() && phase_ >= array_.size())
        phase_ = 0;
}

void ArrayPlayTilde::processBlock(const t_sample**, t_sample** out)
{
    const auto BS = blockSize();

    if (state_ != STATE_PLAYING) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = 0;
    } else {
        assert(phase_ >= 0);

        const auto AMP = amp_->value();
        const double SPEED = speed_->value();
        const double ASIZE = array_.size();

        if (SPEED > 0) {
            bool done = false;

            switch (interp_->value()) {
            case INTERP_LIN: {
                for (size_t i = 0; i < BS; i++) {
                    if (phase_ >= 0 && phase_ < (ASIZE - 1)) {
                        out[0][i] = AMP * readUnsafe1(phase_);
                        phase_ += SPEED;
                    } else {
                        out[0][i] = AMP * readSafe1(phase_);
                        done = true;
                    }
                }
            } break;
            case INTERP_CUBIC: {
                for (size_t i = 0; i < BS; i++) {
                    if (phase_ >= 0 && phase_ < (ASIZE - 2)) {
                        if (phase_ < 1)
                            out[0][i] = AMP * readSafe3(phase_);
                        else
                            out[0][i] = AMP * readUnsafe3(phase_);

                        phase_ += SPEED;
                    } else {
                        out[0][i] = AMP * readSafe3(phase_);
                        done = true;
                    }
                }
            } break;
            case NO_INTERP:
            default: {
                for (size_t i = 0; i < BS; i++) {
                    if (phase_ >= 0 && phase_ < ASIZE) {
                        out[0][i] = AMP * readUnsafe0(phase_);
                        phase_ += SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            }

            if (done)
                done_.delay(0);
        } else if (SPEED < 0) {
            bool done = false;
            const double ABACK = ASIZE - 1;

            switch (interp_->value()) {
            case INTERP_LIN: {

                for (size_t i = 0; i < BS; i++) {
                    auto ph0 = ABACK - phase_ - 1;

                    if (ph0 >= 0 && ph0 < (ASIZE - 1)) {
                        out[0][i] = AMP * readUnsafe1(ph0);
                        phase_ -= SPEED;
                    } else {
                        out[0][i] = AMP * readSafe1(ph0);
                        done = true;
                    }
                }
            } break;
            case INTERP_CUBIC: {
                for (size_t i = 0; i < BS; i++) {
                    auto ph0 = ABACK - phase_ - 2;

                    if (ph0 >= 1 && ph0 < (ASIZE - 2)) {
                        out[0][i] = AMP * readUnsafe3(ph0);
                        phase_ -= SPEED;
                    } else {
                        out[0][i] = AMP * readSafe3(ph0);
                        done = true;
                    }
                }
            } break;
            case NO_INTERP:
            default: {
                for (size_t i = 0; i < BS; i++) {
                    auto ph0 = ABACK - phase_;

                    if (ph0 >= 0 && ph0 < ASIZE) {
                        out[0][i] = AMP * readUnsafe0(ph0);
                        phase_ -= SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            }

            if (done)
                done_.delay(0);

        } else {
            for (size_t i = 0; i < BS; i++)
                out[0][i] = 0;
        }
    }
}

void ArrayPlayTilde::m_play(t_symbol* s, const AtomListView& lv)
{
    command(ACT_PLAY);
}

void ArrayPlayTilde::m_pause(t_symbol* s, const AtomListView& lv)
{
    if (lv.boolAt(0, true))
        command(ACT_PAUSE);
    else
        command(ACT_PLAY);
}

void ArrayPlayTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    command(ACT_STOP);
}

void ArrayPlayTilde::command(PlayAction act)
{
    state_ = fsm_[state_][act](this);
}

void setup_array_play()
{
    SoundExternalFactory<ArrayPlayTilde> obj("array.play~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("array.p~");

    obj.addMethod("play", &ArrayPlayTilde::m_play);
    obj.addMethod("pause", &ArrayPlayTilde::m_pause);
    obj.addMethod("stop", &ArrayPlayTilde::m_stop);

    obj.setDescription("array player with variable speed and amplitude");
    obj.setCategory("array");
}
