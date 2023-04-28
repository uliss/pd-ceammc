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
#include "fmt/core.h"

#include "ceammc_crc32.h"

CEAMMC_DEFINE_SYM(done)
CEAMMC_DEFINE_SYM(ed)
CEAMMC_DEFINE_SYM(ri)
CEAMMC_DEFINE_SYM(i)

SeqBangsBase::SeqBangsBase(const PdArgs& args)
    : SeqBase(args, 100)
    , pattern_(nullptr)
{
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
                return total * beatDuration();
            },
            [this](t_float f) -> bool {
                const auto total = pattern_->value().sum().get_value_or(0);
                if (total <= 0) {
                    OBJ_ERR << "empty sequence";
                    return false;
                }

                return setBeatDuration(f / total);
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

void SeqBangsBase::onInlet(size_t n, const AtomListView& l)
{
    pattern_->set(l);
}

double SeqBangsBase::calcNextTick() const
{
    const auto i = beatDuration();
    const auto N = pattern_->value().size();
    if (N == 0)
        return i;

    const auto idx = sequenceCounter() % N;
    return i * pattern_->value()[idx].asFloat(0);
}

void SeqBangsBase::outputTick()
{
    Atom l[2] = { sequenceCounter(), sequenceSize() };
    anyTo(1, sym_i(), AtomListView(l, 2));
    anyTo(1, sym_ed(), Atom(calcNextTick()));

    bangTo(0);
}

void SeqBangsBase::outputRepeat(size_t ridx)
{
    Atom l[2] = { ridx, numRepeats() };
    anyTo(1, sym_ri(), AtomListView(l, 2));
}

void SeqBangsBase::outputRepeatDone()
{
    anyTo(1, sym_done(), AtomListView());
}

void setup_seq_bangs()
{
    SequencerIFaceFactory<ObjectFactory, SeqBangs> obj("seq.bangs");
    obj.addAlias("seq.b");

    obj.setXletsInfo({ "bang:  start playing sequence\n"
                       "stop:  stop sequencer\n",
                         "list: set new pattern" },
        { "bang: output pattern", "\\[i IDX N( - sequence iteration\n"
                                  "\\[ri IDX N( - repeat iteration\n"
                                  "\\[ed MS( - event duration\n"
                                  "\\[done( - when done" });

    obj.setDescription("bang sequencer");
    obj.setCategory("seq");
    obj.setKeywords({ "seq", "sequencer", "bang", "pattern", "rhythm" });
}
