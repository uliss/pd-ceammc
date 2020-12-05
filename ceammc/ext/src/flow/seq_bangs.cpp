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
#include "seq_bangs.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/format.h"

static t_symbol* SYM_DONE;
static t_symbol* SYM_EVENT_DUR;
static t_symbol* SYM_REPEAT_IDX;
static t_symbol* SYM_IDX;

SeqBangsBase::SeqBangsBase(const PdArgs& args)
    : SeqBase(args)
    , pattern_(nullptr)
    , interval_(nullptr)
{
    interval_ = new SeqTimeGrain("@t", 100);
    addProperty(interval_);

    pattern_ = new ListProperty("@p");
    pattern_->setArgIndex(0);
    pattern_->setFilterAtomFn([this](const Atom& a) -> bool {
        if (!a.isFloat() || a.asT<t_float>() < 0) {
            OBJ_ERR << fmt::format("invalid value in pattern: \"{}\", skipping", to_string(a));
            return false;
        }

        return true;
    });
    addProperty(pattern_);

    {
        auto p = createCbFloatProperty(
            "@dur",
            [this]() -> t_float {
                const auto total = pattern_->value().sum().get_value_or(0);
                return total * interval_->value();
            },
            [this](t_float f) -> bool {
                const auto total = pattern_->value().sum().get_value_or(0);
                if (total <= 0) {
                    OBJ_ERR << "empty sequence";
                    return false;
                }

                return interval_->setValue(f / total);
            });

        p->setUnits(PropValueUnits::MSEC);
        p->checkNonNegative();
    }

    createInlet();
    createOutlet();
    createOutlet();
}

void SeqBangsBase::onBang()
{
    reset();
    start();
}

void SeqBangsBase::onInlet(size_t n, const AtomList& l)
{
    pattern_->set(l);
}

double SeqBangsBase::calcNextTick() const
{
    const auto i = interval_->value();
    const auto N = pattern_->value().size();
    if (N == 0)
        return i;

    const auto idx = current_ % N;
    return i * pattern_->value()[idx].asFloat(0);
}

void SeqBangsBase::outputTick()
{
    anyTo(1, SYM_IDX, Atom(current_));
    anyTo(1, SYM_EVENT_DUR, Atom(calcNextTick()));

    bangTo(0);
}

void SeqBangsBase::outputSequenceBegin()
{
    anyTo(1, SYM_REPEAT_IDX, Atom(cycle_counter_));
}

void SeqBangsBase::outputSequenceEnd()
{
}

void SeqBangsBase::outputCycleBegin()
{
}

void SeqBangsBase::outputCycleEnd()
{
    anyTo(1, SYM_DONE, AtomListView());
}

void setup_seq_bangs()
{
    SYM_DONE = gensym("done");
    SYM_EVENT_DUR = gensym("ed");
    SYM_REPEAT_IDX = gensym("ri");
    SYM_IDX = gensym("i");

    SequencerIFaceFactory<ObjectFactory, SeqBangs> obj("seq.bangs");
    obj.addAlias("seq.b");

    obj.setXletsInfo({ "bang:  start playing sequence\n"
                       "stop:  stop sequencer\n",
                         "list: set new pattern" },
        { "bang: output pattern", "done: when done" });
}
