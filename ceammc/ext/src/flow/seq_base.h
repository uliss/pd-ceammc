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

using namespace ceammc;

class RepeatProperty : public IntProperty {
public:
    RepeatProperty(const std::string& name, int defValue = -1);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;

    bool isFinite() const { return value() >= 0; }
};

class SeqTimeGrain : public FloatProperty {
    static const size_t MIN_INTERVAL = 1;
    static const size_t MAX_INTERVAL = 20000;
    AtomList& sequence_;

public:
    SeqTimeGrain(const std::string& name, AtomList& seq, t_float f = 10);
    bool setList(const AtomListView& lv) override;
    bool setBpm(t_float bpm);
    bool setFreq(t_float hz);
};

class SeqBase : public BaseObject {
protected:
    RepeatProperty* repeat_;
    ClockLambdaFunction clock_;
    size_t cycle_counter_ = { 0 };

    /**
     * calculates time until next tick in milliseconds
     */
    virtual double calcNextTick() const = 0;

    /**
     * Check if current sequence element is first
     */
    virtual bool isSequenceBegin() const = 0;

    /**
     * checks if current sequence cycle is finished
     */
    virtual bool isSequenceEnd() const = 0;

    /**
     * output current sequence element
     */
    virtual void outputTick() = 0;

    /**
     * outputs when sequence is on first element
     */
    virtual void outputCycleBegin() = 0;

    /**
     * outputs when cycle is finished
     */
    virtual void outputCycleEnd() = 0;

    /**
     * Outputs when sequence begins
     */
    virtual void outputSequenceBegin() = 0;

    /**
     * output when whoel sequence is finished
     */
    virtual void outputSequenceEnd() = 0;

    /**
     * reset sequence cycle
     */
    virtual void resetSequence() = 0;

    virtual void sequenceNext() = 0;

    /**
     * output current sequence element
     * @return true if next tick should be scheduled
     */
    bool tick();

    // start/stop/continue at any cycle point
    //    virtual void start() = 0;
    //    virtual void stop() = 0;

    //    // call when last cycle finished
    //    virtual void done() = 0;
    //    virtual void reset();

    /**
     * check if sequence should be repeated again
     * @return
     */
    bool shouldRepeat() const;

public:
    SeqBase(const PdArgs& args);

    void m_start(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
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
        t ? T::start() : T::stop();
    }

    void m_stop(t_symbol* s, const AtomListView& lv)
    {
        const auto t = lv.boolAt(0, true);
        t ? T::stop() : T::start();
    }

    void m_reset(t_symbol* s, const AtomListView& lv) { T::reset(); }

    void m_tick(t_symbol* s, const AtomListView& lv) { T::tick(); }
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
    }
};

#endif // SEQ_BASE_H
