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
#include "seq_sequencer.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

static t_symbol* SYM_IOTA;
static t_symbol* SYM_DONE;

SeqSequencerBase::SeqSequencerBase(const PdArgs& args)
    : SeqBase(args)
    , values_(nullptr)
    , interval_(nullptr)
    , counter_(0)
{
    values_ = new ListProperty("@v");
    values_->setArgIndex(1);
    values_->setSuccessFn([this](Property*) { resetSequence(); });
    addProperty(values_);

    interval_ = new SeqTimeGrain("@t", values_->value(), 20);
    interval_->setArgIndex(0);
    addProperty(interval_);

    createInlet();
    createOutlet();
    createOutlet();
}

void SeqSequencerBase::onBang()
{
    start();
}

void SeqSequencerBase::onFloat(t_float f)
{
    if (f == 0)
        stop();
    else
        start();
}

void SeqSequencerBase::onInlet(size_t n, const AtomList& l)
{
    if (!l.empty())
        values_->setList(l);
}

void SeqSequencerBase::outputTick()
{
    const auto& v = values_->value();
    if (counter_ >= v.size())
        return;

    floatTo(1, counter_);

    const auto& a = v[counter_];
    if (a.isDataType(DataTypeMList::dataType))
        listTo(0, a.asD<DataTypeMList>()->data());
    else
        atomTo(0, a);
}

void SeqSequencerBase::outputSequenceBegin()
{
    anyTo(1, SYM_IOTA, Atom(cycle_counter_));
}

void SeqSequencerBase::outputSequenceEnd()
{
}

void SeqSequencerBase::outputCycleBegin()
{
}

void SeqSequencerBase::outputCycleEnd()
{
    anyTo(1, SYM_DONE, AtomListView());
}

void SeqSequencerBase::start()
{
    reset();
    clock_.exec();
}

void SeqSequencerBase::stop()
{
    reset();
    clock_.unset();
}

void SeqSequencerBase::reset()
{
    counter_ = 0;
    //    SeqBase::reset
}

void setup_seq_sequencer()
{
    SYM_IOTA = gensym("i");
    SYM_DONE = gensym("done");

    SequencerIFaceFactory<ObjectFactory, SeqSequencer> obj("seq");
}
