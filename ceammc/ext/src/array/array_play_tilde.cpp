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
#include "array_play_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

FSM ArrayPlayTilde::fsm_ = {
    StateTransitions {
        // STATE_STOPPED
        [](ArrayPlayTilde* ap) -> PlayState { // stop->stop
            if (ap->speed_->value() >= 0)
                ap->phase_ = 0;
            else
                ap->phase_ = ap->end_->samples();

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
            if (ap->speed_->value() >= 0)
                ap->phase_ = 0;
            else
                ap->phase_ = ap->end_->samples();

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
    , begin_(nullptr)
    , end_(nullptr)
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

    begin_ = new ArrayPositionProperty(&array_, "@begin", 0);
    addProperty(begin_);

    end_ = new ArrayPositionProperty(&array_, "@end", -1);
    addProperty(end_);

    createCbFloatProperty("@pos_samp", [this]() -> t_float { return playPos(); });
    createCbFloatProperty("@pos_sec", [this]() -> t_float { return playPos() / sys_getsr(); });
    createCbFloatProperty("@pos_ms", [this]() -> t_float { return (playPos() / sys_getsr()) * 1000; });
    createCbFloatProperty("@pos_phase", [this]() -> t_float {
        if (!array_.isValid() || array_.size() < 1)
            return 0;
        else
            return playPos() / array_.size();
    });

    createCbIntProperty("@state", [this]() { return state_; });

    createSignalOutlet();
    createOutlet();
}

void ArrayPlayTilde::onBang()
{
    begin_->setBegin();
    end_->setEnd();

    command(ACT_STOP);
    command(ACT_PLAY);
}

void ArrayPlayTilde::onFloat(t_float pos)
{
    if (!checkArray())
        return;

    command(ACT_STOP);

    if (pos < 0) {
        pos = t_sample(array_.size()) + pos;
        if (pos < 0) {
            OBJ_ERR << fmt::format("invalid negative play position ({}), too big negative offset", pos);
            return;
        }
        // break thru
    }

    // (!) not else if
    if (pos >= array_.size()) {
        OBJ_ERR << fmt::format("invalid play position: {}, expected >=0 and <{}", pos, array_.size());
        return;
    }

    begin_->setSamples(pos);
    end_->setEnd();
    phase_ = 0;
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
        const double FIRST = begin_->samples();
        const double LAST = end_->samples();

        // preconditions
        assert(FIRST >= 0);
        assert(LAST >= 0);
        assert(FIRST <= LAST);
        assert(ASIZE > 0);

        if (SPEED > 0) {
            bool done = false;

            switch (interp_->value()) {
            case INTERP_LIN: {
                for (size_t i = 0; i < BS; i++) {
                    const auto pos = FIRST + phase_;

                    if (pos >= FIRST && pos <= LAST) { // need to read one sample after read position
                        if (pos <= (LAST - 1))
                            out[0][i] = AMP * readUnsafe1(pos);
                        else
                            out[0][i] = AMP * readSafe1(pos);

                        phase_ += SPEED;
                    } else {
                        // stop playing, but fill rest of block with zeroes
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            case INTERP_CUBIC: {
                for (size_t i = 0; i < BS; i++) {
                    const auto pos = FIRST + phase_;

                    // need to read one sample before and two samples after read position
                    if (pos >= FIRST && pos <= LAST) {
                        if (pos < 1 || pos >= (LAST - 2))
                            out[0][i] = AMP * readSafe3(pos);
                        else
                            out[0][i] = AMP * readUnsafe3(pos);

                        phase_ += SPEED;
                    } else {
                        // stop playing, but fill rest of block with zeroes
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            case NO_INTERP:
            default: {
                for (size_t i = 0; i < BS; i++) {
                    const auto pos = FIRST + phase_;

                    if (pos >= FIRST && pos <= LAST) {
                        out[0][i] = AMP * readUnsafe0(pos);

                        phase_ += SPEED;
                    } else {
                        // stop playing, but fill rest of block with zeroes
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

            switch (interp_->value()) {
            case INTERP_LIN: {

                for (size_t i = 0; i < BS; i++) {
                    const auto pos = FIRST + phase_;

                    if (pos >= FIRST && pos <= LAST) {
                        if (pos <= LAST - 1) // need to read one sample after read position
                            out[0][i] = AMP * readUnsafe1(pos);
                        else
                            out[0][i] = AMP * readSafe1(pos);

                        phase_ += SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            case INTERP_CUBIC: {
                for (size_t i = 0; i < BS; i++) {
                    const auto pos = FIRST + phase_;

                    if (pos >= FIRST && pos <= LAST) {
                        // need to read one sample before and two samples after read position
                        if (pos < 1 || pos > (LAST - 2))
                            out[0][i] = AMP * readSafe3(pos);
                        else
                            out[0][i] = AMP * readUnsafe3(pos);

                        phase_ += SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            case NO_INTERP:
            default: {
                for (size_t i = 0; i < BS; i++) {
                    const auto pos = FIRST + phase_;

                    if (pos >= FIRST && pos <= LAST) {
                        out[0][i] = AMP * readUnsafe0(pos);
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

        } else { // ZERO speed
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

void ArrayPlayTilde::m_pos(t_symbol* s, const AtomListView& lv)
{
    if (lv.isFloat()) {
        const long N = static_cast<long>(array_.size());
        const long pos_samp = lv.asFloat();
        if (pos_samp < 0) {
            const auto abs_pos = end_->samples() + pos_samp;
            if (abs_pos < 0) {
                METHOD_ERR(s) << fmt::format("negative offset is too big: {}, clipping to 0", pos_samp);
                phase_ = 0;
                return;
            }

            phase_ = abs_pos;
        } else if (pos_samp >= N) {
            METHOD_ERR(s) << fmt::format("relative");
        }
    }
}

void ArrayPlayTilde::command(PlayAction act)
{
    state_ = fsm_[state_][act](this);
}

void setup_array_play_tilde()
{
    SoundExternalFactory<ArrayPlayTilde> obj("array.play~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("array.p~");

    obj.addMethod("play", &ArrayPlayTilde::m_play);
    obj.addMethod("pause", &ArrayPlayTilde::m_pause);
    obj.addMethod("stop", &ArrayPlayTilde::m_stop);
    obj.addMethod("pos", &ArrayPlayTilde::m_pos);
}
