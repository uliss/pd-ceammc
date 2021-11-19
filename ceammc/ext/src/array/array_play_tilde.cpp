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

static t_symbol* SYM_MS;
static t_symbol* SYM_SEC;
static t_symbol* SYM_SAMP;
static t_symbol* SYM_PHASE;

FSM ArrayPlayTilde::fsm_ = {
    StateTransitions {
        // STATE_STOPPED
        [](ArrayPlayTilde* ap) -> PlayState { // stop->stop
            ap->resetPlayPosition();
            return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // stop->play
            if (ap->isOk()) {
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
            ap->resetPlayPosition();
            return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // play->play
            Debug(ap).stream() << "already playing";
            return STATE_PLAYING;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // play->pause
            return STATE_PAUSED;
        },
    },
    StateTransitions {
        // STATE PAUSE
        [](ArrayPlayTilde* ap) -> PlayState { // pause->stop
            ap->resetPlayPosition();
            return STATE_STOPPED;
        },
        [](ArrayPlayTilde* ap) -> PlayState { // pause->play
            if (ap->isOk()) {
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
    , cursor_(nullptr)
    , clock_interval_(nullptr)
    , clock_format_(nullptr)
    , pos_(0)
    , block_counter_(0)
    , state_(STATE_STOPPED)
    , done_([this]() {
        command(ACT_STOP);
        bangTo(2);
    })
    , cursor_tick_([this]() {
        auto* s = clock_format_->value();
        if (s == SYM_SEC)
            floatTo(1, cursor_->seconds(sys_getsr()));
        else if (s == SYM_MS)
            floatTo(1, cursor_->seconds(sys_getsr()));
        else if (s == SYM_SAMP)
            floatTo(1, cursor_->samples());
        else if (s == SYM_PHASE)
            floatTo(1, cursor_->phase());
        else
            ; // none
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

    clock_interval_ = new FloatProperty("@clock", 0);
    clock_interval_->checkClosedRange(0, 20);
    clock_interval_->setUnits(PropValueUnits::SEC);
    addProperty(clock_interval_);

    clock_format_ = new SymbolEnumProperty("@cfmt", { "sec", "ms", "samp", "phase" });
    addProperty(clock_format_);

    cursor_ = new ArrayPositionProperty(&array_, "@cursor_samp", 0);
    cursor_->setSuccessFn([this](Property*) { pos_ = cursor_->value(); });
    addProperty(cursor_);

    createCbFloatProperty(
        "@cursor_sec",
        [this]() -> t_float { return cursor_->seconds(sys_getsr()); },
        [this](t_float pos) -> bool { return cursor_->setSeconds(pos, sys_getsr()); })
        ->setUnits(PropValueUnits::SEC);

    createCbFloatProperty(
        "@cursor_ms",
        [this]() -> t_float { return cursor_->ms(sys_getsr()); },
        [this](t_float pos) -> bool { return cursor_->setMs(pos, sys_getsr()); })
        ->setUnits(PropValueUnits::MSEC);

    createCbFloatProperty(
        "@cursor_phase",
        [this]() -> t_float { return cursor_->phase(); },
        [this](t_float v) -> bool {
            const bool ok = cursor_->setPhase(v);
            if (!ok)
                return false;

            // sync cursor
            pos_ = cursor_->value();
            return true;
        });

    createCbIntProperty("@state", [this]() { return state_; });

    createCbListProperty(
        "@select_phase",
        [this]() -> AtomList { return { begin_->phase(), end_->phase() }; },
        [this](const AtomList& pos) -> bool {
            return begin_->setPhase(pos.floatAt(0, 0))
                && end_->setPhase(pos.floatAt(1, -1));
        })
        ->setListCheckFn([](const AtomList& l) -> bool { return l.size() == 2 && l[0].isFloat() && l[1].isFloat(); });

    {
        auto p = createCbListProperty(
            "@select_sec",
            [this]() -> AtomList { return { begin_->seconds(sys_getsr()), end_->seconds(sys_getsr()) }; },
            [this](const AtomList& pos) -> bool { return begin_->setSeconds(pos.floatAt(0, 0), sys_getsr())
                                                      && end_->setSeconds(pos.floatAt(1, -1), sys_getsr()); });

        p->setListCheckFn([](const AtomList& l) -> bool { return l.size() == 2 && l[0].isFloat() && l[1].isFloat(); });
        p->setUnits(PropValueUnits::SEC);
    }

    {
        auto p = createCbListProperty(
            "@select_ms",
            [this]() -> AtomList { return { begin_->ms(sys_getsr()), end_->seconds(sys_getsr()) }; },
            [this](const AtomList& pos) -> bool { return begin_->setMs(pos.floatAt(0, 0), sys_getsr())
                                                      && end_->setMs(pos.floatAt(1, -1), sys_getsr()); });

        p->setListCheckFn([](const AtomList& l) -> bool { return l.size() == 2 && l[0].isFloat() && l[1].isFloat(); });
        p->setUnits(PropValueUnits::MSEC);
    }

    {
        auto p = createCbListProperty(
            "@select_samp",
            [this]() -> AtomList { return { begin_->samples(), end_->samples() }; },
            [this](const AtomList& pos) -> bool { return begin_->setSamples(pos.floatAt(0, 0))
                                                      && end_->setSamples(pos.floatAt(1, -1)); });

        p->setListCheckFn([](const AtomList& l) -> bool { return l.size() == 2 && l[0].isFloat() && l[1].isFloat(); });
        p->setUnits(PropValueUnits::SAMP);
    }

    createSignalOutlet();
    createOutlet();
    createOutlet();
}

void ArrayPlayTilde::onBang()
{
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
    resetPlayPosition();
    command(ACT_PLAY);
}

void ArrayPlayTilde::setupDSP(t_signal** sig)
{
    ArraySoundBase::setupDSP(sig);
    if (checkArray() && cursor_->value() >= array_.size())
        resetPlayPosition();
}

void ArrayPlayTilde::processBlock(const t_sample**, t_sample** out)
{
    const auto BS = blockSize();

    if (state_ != STATE_PLAYING) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = 0;
    } else {
        assert(cursor_->samples() >= 0);

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
                    if (pos_ >= FIRST && pos_ <= LAST) { // need to read one sample after read position
                        if (pos_ <= (LAST - 1))
                            out[0][i] = AMP * readUnsafe1(pos_);
                        else
                            out[0][i] = AMP * readSafe1(pos_);

                        pos_ += SPEED;
                    } else
                        return blockLast(i, BS, out[0]);
                }
            } break;
            case INTERP_CUBIC: {
                for (size_t i = 0; i < BS; i++) {
                    // need to read one sample before and two samples after read position
                    if (pos_ >= FIRST && pos_ <= LAST) {
                        if (pos_ < 1 || pos_ >= (LAST - 2))
                            out[0][i] = AMP * readSafe3(pos_);
                        else
                            out[0][i] = AMP * readUnsafe3(pos_);

                        pos_ += SPEED;
                    } else
                        return blockLast(i, BS, out[0]);
                }
            } break;
            case NO_INTERP:
            default: {
                for (size_t i = 0; i < BS; i++) {
                    if (pos_ >= FIRST && pos_ <= LAST) {
                        out[0][i] = AMP * readUnsafe0(pos_);

                        pos_ += SPEED;
                    } else
                        return blockLast(i, BS, out[0]);
                }
            } break;
            }

            blockDone(done);

        } else if (SPEED < 0) {
            bool done = false;

            switch (interp_->value()) {
            case INTERP_LIN: {
                for (size_t i = 0; i < BS; i++) {
                    if (pos_ >= FIRST && pos_ <= LAST) {
                        if (pos_ <= LAST - 1) // need to read one sample after read position
                            out[0][i] = AMP * readUnsafe1(pos_);
                        else
                            out[0][i] = AMP * readSafe1(pos_);

                        pos_ += SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            case INTERP_CUBIC: {
                for (size_t i = 0; i < BS; i++) {
                    if (pos_ >= FIRST && pos_ <= LAST) {
                        // need to read one sample before and two samples after read position
                        if (pos_ < 1 || pos_ > (LAST - 2))
                            out[0][i] = AMP * readSafe3(pos_);
                        else
                            out[0][i] = AMP * readUnsafe3(pos_);

                        pos_ += SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            case NO_INTERP:
            default: {
                for (size_t i = 0; i < BS; i++) {
                    if (pos_ >= FIRST && pos_ <= LAST) {
                        out[0][i] = AMP * readUnsafe0(pos_);
                        pos_ += SPEED;
                    } else {
                        out[0][i] = 0;
                        done = true;
                    }
                }
            } break;
            }

            blockDone(done);

        } else { // ZERO speed
            for (size_t i = 0; i < BS; i++)
                out[0][i] = 0;
        }
    }
}

void ArrayPlayTilde::m_play(t_symbol*, const AtomListView&)
{
    command(ACT_PLAY);
}

void ArrayPlayTilde::m_pause(t_symbol*, const AtomListView& lv)
{
    if (lv.boolAt(0, true))
        command(ACT_PAUSE);
    else
        command(ACT_PLAY);
}

void ArrayPlayTilde::m_stop(t_symbol*, const AtomListView&)
{
    command(ACT_STOP);
}

void ArrayPlayTilde::m_range(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "usage: BEGIN_unit END_unit. Valid units are: ms/msec/sec/s/min/samp/*/%";
        return;
    }

    if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        begin_->setPhase(lv[0].asT<t_float>());
        end_->setPhase(lv[1].asT<t_float>());
        return;
    }

    if (begin_->set(lv.subView(0, 1)))
        end_->set(lv.subView(1));
}

void ArrayPlayTilde::setBegin()
{
    cursor_->setSamples(begin_->samples());
}

void ArrayPlayTilde::setEnd()
{
    cursor_->setSamples(end_->samples());
}

void ArrayPlayTilde::resetPlayPosition()
{
    if (speed_->value() >= 0)
        setBegin();
    else
        setEnd();

    pos_ = cursor_->samples();
}

bool ArrayPlayTilde::isOk()
{
    if (!checkArray())
        return false;

    if (begin_->samples() > end_->samples()) {
        OBJ_ERR << fmt::format("@begin <= @end expected, got: {} > {}", begin_->samples(), end_->samples());
        return false;
    }

    return true;
}

void ArrayPlayTilde::command(PlayAction act)
{
    state_ = fsm_[state_][act](this);
}

void ArrayPlayTilde::blockLast(size_t i, size_t bs, t_sample* out)
{
    for (; i < bs; i++)
        out[i] = 0;

    done_.delay(0);
}

void ArrayPlayTilde::blockDone(bool done)
{
    if (done) {
        done_.delay(0);
        return;
    }

    // sync cursor position
    cursor_->setValue(pos_);
    const auto BS = blockSize() ? blockSize() : 64;
    const size_t T = (clock_interval_->value() * samplerate()) / BS;
    if (T > 0) {
        block_counter_++;
        if (block_counter_ % T == 0)
            cursor_tick_.delay(0);
    }
}

void setup_array_play_tilde()
{
    SYM_MS = gensym("ms");
    SYM_SEC = gensym("sec");
    SYM_PHASE = gensym("phase");
    SYM_SAMP = gensym("samp");

    SoundExternalFactory<ArrayPlayTilde> obj("array.play~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("array.p~");

    obj.addMethod("play", &ArrayPlayTilde::m_play);
    obj.addMethod("pause", &ArrayPlayTilde::m_pause);
    obj.addMethod("stop", &ArrayPlayTilde::m_stop);
    obj.addMethod("range", &ArrayPlayTilde::m_range);

    obj.setXletsInfo(
        { "bang: start playback" },
        { "audio", "current playing time", "bang on finish" });
}
