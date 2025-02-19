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
#ifndef SEQ_BASE_H
#define SEQ_BASE_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <cstdint>

using namespace ceammc;

class RepeatProperty : public IntProperty {
public:
    RepeatProperty(const std::string& name, int defValue = -1);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;

    bool isFinite() const { return value() >= 0; }
    bool isInfinite() const { return value() < 0; }
    bool shouldRepeat(int n) const { return isInfinite() || n < value(); }
};

class SeqTimeGrain : public FloatProperty {
    static const size_t MIN_INTERVAL = 1;
    static const size_t MAX_INTERVAL = 20000;

public:
    SeqTimeGrain(const std::string& name, t_float f = 10);
    bool setList(const AtomListView& lv) override;
    bool setBpm(t_float bpm);
};

class SeqBase : public BaseObject {
    SeqTimeGrain* beat_duration_ { 0 };
    IntProperty* beat_division_ { 0 };

protected:
    RepeatProperty* repeat_ { 0 };
    SymbolEnumProperty* mode_ { 0 };
    FloatProperty* upbeat_ { 0 };
    ClockLambdaFunction clock_;
    size_t num_repeats_ = { 0 };
    size_t sequence_counter_ = { 0 };
    uint8_t inc_ : 1;
    bool r0_ouput_ : 1;

public: // pure virtual
    /**
     * calculates time until next tick in milliseconds
     */
    virtual double calcNextTick() const = 0;

    /**
     * returns sequence size
     */
    virtual size_t sequenceSize() const = 0;

    /**
     * output current sequence element
     */
    virtual void outputTick() = 0;

    /**
     * outputs on every repeat
     */
    virtual void outputRepeat(size_t ridx) = 0;

    /**
     * output when all repeats done
     */
    virtual void outputRepeatDone() = 0;

    /**
     * output when first sequence element is reached
     * @param idx
     */
    virtual void outputSequenceFirst();

    /**
     * output when last sequence element is reached
     * @param idx
     */
    virtual void outputSequenceLast();

    /**
     * start sequence clock
     */
    virtual void clockStart();

    /**
     * stop sequence clock
     */
    virtual void clockStop();

public:
    void initDone() override;

public:
    /**
     * reset sequence counter
     */
    void resetSequenceCounter();

    /**
     * returns sequence counter
     */
    size_t sequenceCounter() const;

    /**
     * output current sequence element
     * @return true if next tick should be scheduled
     */
    bool tick(bool output = true);

    /**
     * check if sequence should be repeated again
     * @return
     */
    bool shouldRepeat() const;

    /**
     * complete sequence reset: reset counters then stop
     */
    void reset();

    /**
     * start sequence
     */
    void start();

    /**
     * stop sequence clock
     */
    void stop();

    /**
     * reset repeat counter
     */
    void resetCycleCounter();

    void setSequenceCounter(size_t);

    void moveSequenceCounter(long n);

    /**
     * number of repeats
     */
    size_t numRepeats() const { return repeat_->value(); }

    /**
     * return beat duration in milliseconds
     */
    t_float beatDuration() const { return beat_duration_->value() / beat_division_->value(); }

    /**
     * set beat duration
     * @return true on success, false on error
     */
    bool setBeatDuration(t_float ms) { return beat_duration_->setValue(ms * beat_division_->value()); }

    void setBeatDurationDefs(t_float ms, int idx);

    /**
     * return beat division
     */
    int beatDivision() const { return beat_division_->value(); }

public:
    SeqBase(const PdArgs& args, t_float defBeatDuration);

private:
    bool tickForward(bool output);
    bool isSeqBegin() const;
};

template <class T>
class SequencerIFace : public T {
public:
    SequencerIFace(const PdArgs& args)
        : T(args)
    {
    }

    void m_start(t_symbol* s, const AtomListView& lv)
    {
        const auto t = lv.boolAt(0, true);
        t ? this->start() : this->stop();
    }

    void m_stop(t_symbol* s, const AtomListView& lv)
    {
        const auto t = lv.boolAt(0, true);
        t ? this->stop() : this->start();
    }

    void m_reset(t_symbol* s, const AtomListView& lv) { this->reset(); }

    void m_tick(t_symbol* s, const AtomListView& lv) { this->tick(); }

    void m_rewind(t_symbol* s, const AtomListView& lv)
    {
        this->resetSequenceCounter();
        this->resetCycleCounter();
    }
};

template <template <typename> class Factory, class T>
class SequencerIFaceFactory : public Factory<T> {
public:
    SequencerIFaceFactory(const char* name)
        : Factory<T>(name)
    {
        this->addMethod("start", &T::m_start);
        this->addMethod("stop", &T::m_stop);
        this->addMethod("reset", &T::m_reset);
        this->addMethod("tick", &T::m_tick);
        this->addMethod("rewind", &T::m_rewind);
    }
};

#endif // SEQ_BASE_H
