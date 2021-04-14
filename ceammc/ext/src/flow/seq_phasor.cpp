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
#include "seq_phasor.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

SeqPhasorBase::SeqPhasorBase(const PdArgs& a)
    : BaseObject(a)
    , freq_hz_(nullptr)
    , steps_(nullptr)
    , on_(nullptr)
    , invert_(nullptr)
    , open_range_(nullptr)
    , clock_([this]() {
        tick();
        if (on_->value()) {
            const auto ms = calcNextTick();
            if (ms > 0)
                clock_.delay(ms);
        }
    })
    , phase_(0)
{
    createInlet();
    createOutlet();
    createOutlet();

    freq_hz_ = new FloatProperty("@freq", 0);
    freq_hz_->setUnits(PropValueUnits::HZ);
    freq_hz_->checkClosedRange(0, 100);
    freq_hz_->setArgIndex(0);
    addProperty(freq_hz_);

    steps_ = new IntProperty("@steps", 128);
    steps_->checkMinEq(3);
    addProperty(steps_);

    on_ = new BoolProperty("@on", false);
    on_->setSuccessFn([this](Property*) {
        if (on_->value() == 0)
            stop();
        else
            start();
    });
    addProperty(on_);

    invert_ = new BoolProperty("@invert", false);
    addProperty(invert_);

    open_range_ = new BoolProperty("@open", false);
    addProperty(open_range_);
}

void SeqPhasorBase::initDone()
{
    if (on_->value())
        start();
}

void SeqPhasorBase::onFloat(t_float f)
{
    on_->set(Atom(f));
}

void SeqPhasorBase::onInlet(size_t n, const AtomListView& l)
{
    resetSequenceCounter();
}

void SeqPhasorBase::tick()
{
    floatTo(0, value());
    next();
}

void SeqPhasorBase::reset()
{
    resetCycleCounter();
    resetSequenceCounter();
    stop();
}

void SeqPhasorBase::stop()
{
    clock_.unset();
}

void SeqPhasorBase::start()
{
    clock_.exec();
}

void SeqPhasorBase::resetSequenceCounter()
{
    const auto N = steps_->value();

    if (invert_->value() && N > 0)
        phase_ = N - 1;
    else
        phase_ = 0;
}

t_float SeqPhasorBase::value() const
{
    if (steps_->value() == 0)
        return 0;

    const int is_closed = !open_range_->value();
    const auto n = steps_->value() - is_closed;

    return t_float(phase_) / n;
}

void SeqPhasorBase::m_phase(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    const int is_closed = !open_range_->value();
    auto ph = lv.floatAt(0, 0);

    if (ph < 0 || ph > 1) {
        METHOD_ERR(s) << "phase value in [0..1" << (is_closed ? ']' : ')') << " range expected, got: " << ph;
        return;
    }

    // wrap open range 1 to 0
    if (!is_closed && ph == 1)
        ph = 0;

    const auto n = steps_->value() - is_closed;
    phase_ = n * ph;
}

void SeqPhasorBase::next()
{
    if (invert_->value()) {
        if (--phase_ < 0) {
            phase_ = steps_->value() - 1;
            bangTo(1);
        }
    } else {
        if (phase_ + 1 < steps_->value()) {
            phase_++;
        } else {
            phase_ = 0;
            bangTo(1);
        }
    }
}

t_float SeqPhasorBase::calcNextTick() const
{
    const auto freq = freq_hz_->value();

    if (freq == 0)
        return 0;

    const auto ms = 1000 / (freq * steps_->value());
    if (ms < 1) {
        OBJ_ERR << "clock resolution is to high, try to decrease frequency or number of steps";
        return 0;
    } else
        return ms;
}

void setup_seq_phasor()
{
    SequencerIFaceFactory<ObjectFactory, SeqPhasor> obj("seq.phasor");
    obj.addMethod("phase", &SeqPhasor::m_phase);

    obj.setXletsInfo(
        { "float: 1|0 - start/stop phasor",
            "bang: reset to start" },
        { "float: value in \\[0, 1) range",
            "bang: at the end of cycle" });
}
