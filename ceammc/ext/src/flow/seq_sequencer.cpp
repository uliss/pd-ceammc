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

static t_symbol* SYM_REPEAT_IDX;
static t_symbol* SYM_IDX;
static t_symbol* SYM_DONE;

SeqSequencerBase::SeqSequencerBase(const PdArgs& args)
    : SeqBase(args)
    , values_(nullptr)
    , interval_(nullptr)
{
    values_ = new ListProperty("@v");
    values_->setArgIndex(0);
    values_->setSuccessFn([this](Property*) { resetSequenceCounter(); });
    addProperty(values_);

    interval_ = new SeqTimeGrain("@t", 20);
    addProperty(interval_);

    createCbFloatProperty(
        "@dur",
        [this]() -> t_float { return sequenceSize() * interval_->value(); },
        [this](t_float f) -> bool {
            const auto N = values_->value().size();
            if (N == 0) {
                OBJ_ERR << "empty sequence";
                return false;
            }

            return interval_->setValue(f / N);
        })
        ->checkNonNegative();

    createInlet();
    createOutlet();
    createOutlet();
}

void SeqSequencerBase::onBang()
{
    reset();
    start();
}

void SeqSequencerBase::onFloat(t_float f)
{
    reset();

    if (f == 0)
        stop();
    else
        start();
}

void SeqSequencerBase::onInlet(size_t n, const AtomListView& l)
{
    if (!l.empty())
        values_->setList(l);
}

void SeqSequencerBase::outputTick()
{
    const auto& v = values_->value();
    const auto i = sequenceCounter();
    if (i >= v.size())
        return;

    Atom l[2] = { i, sequenceSize() };
    anyTo(1, SYM_IDX, AtomListView(l, 2));

    const auto& a = v[i];
    if (a.isDataType(DataTypeMList::dataType))
        listTo(0, a.asD<DataTypeMList>()->data());
    else
        atomTo(0, a);
}

void SeqSequencerBase::outputRepeat(size_t ridx)
{
    Atom l[2] = { ridx, numRepeats() };
    anyTo(1, SYM_REPEAT_IDX, AtomListView(l, 2));
}

void SeqSequencerBase::outputRepeatDone()
{
    anyTo(1, SYM_DONE, AtomListView());
}

void setup_seq_sequencer()
{
    SYM_REPEAT_IDX = gensym("ri");
    SYM_IDX = gensym("i");
    SYM_DONE = gensym("done");

    SequencerIFaceFactory<ObjectFactory, SeqSequencer> obj("sequencer");
    obj.addAlias("seq");
    obj.setXletsInfo(
        { "bang: start sequence\n"
          "float: 1|0 - start/stop sequence",
            "list: set sequence" },
        { "atom or list output", "\\[i IDX N( - sequence iteration\n"
                                 "\\[ri IDX N( - repeat iteration\n"
                                 "\\[done( - when done" });

    obj.setDescription("basic sequence player");
    obj.setCategory("seq");
    obj.setKeywords({"sequencer"});
}
