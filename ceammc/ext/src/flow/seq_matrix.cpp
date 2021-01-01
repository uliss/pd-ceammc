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
#include "seq_matrix.h"
#include "ceammc_factory.h"

static t_symbol* PROP_CURRENT_COL;
static t_symbol* SYM_COL;
static t_symbol* SYM_DONE;
static t_symbol* SYM_GET;
static t_symbol* SYM_IDX;
static t_symbol* SYM_REPEAT_IDX;

SeqMatrixBase::SeqMatrixBase(const PdArgs& args)
    : SeqBase(args)
    , n_(nullptr)
    , interval_(nullptr)
{
    createInlet();
    createOutlet();
    createOutlet();

    n_ = new IntProperty("@n", 16);
    n_->checkNonNegative();
    n_->setArgIndex(0);
    addProperty(n_);

    interval_ = new SeqTimeGrain("@t", 250);
    interval_->setArgIndex(1);
    addProperty(interval_);

    {
        auto p = createCbFloatProperty(
            "@dur",
            [this]() -> t_float { return n_->value() * interval_->value(); },
            [this](t_float f) -> bool {
                const auto N = n_->value();
                if (N == 0) {
                    OBJ_ERR << "empty sequence";
                    return false;
                }

                return interval_->setValue(f / N);
            });

        p->setUnits(PropValueUnits::MSEC);
        p->checkNonNegative();
    }

    {
        auto p = property(gensym("@r"));
        if (p) {
            p->infoT().setDefault(-1);
            p->set({ -1 });
        }
    }
}

void SeqMatrixBase::onBang()
{
    reset();
    start();
}

void SeqMatrixBase::onFloat(t_float f)
{
    if (f == 0) {
        stop();
    } else {
        reset();
        start();
    }
}

void SeqMatrixBase::onInlet(size_t n, const AtomListView& l)
{
    if (l.empty())
        reset();
    else
        n_->set(l);
}

void SeqMatrixBase::outputTick()
{
    Atom l0[2] = { sequenceCounter(), sequenceSize() };
    anyTo(1, SYM_IDX, AtomListView(&l0->atom(), 2));

    anyTo(0, PROP_CURRENT_COL, Atom(sequenceCounter()));

    Atom l1[2] = { SYM_COL, sequenceCounter() };
    anyTo(0, SYM_GET, AtomListView(&l1->atom(), 2));
}

void SeqMatrixBase::outputRepeat(size_t ridx)
{
    Atom l[2] = { ridx, numRepeats() };
    anyTo(1, SYM_REPEAT_IDX, AtomListView(&l->atom(), 2));
}

void SeqMatrixBase::outputRepeatDone()
{
    anyTo(1, SYM_DONE, AtomListView());
}

void setup_seq_matrix()
{
    SYM_GET = gensym("get");
    SYM_COL = gensym("col");
    SYM_DONE = gensym("done");
    SYM_IDX = gensym("i");
    SYM_REPEAT_IDX = gensym("ri");
    PROP_CURRENT_COL = gensym("@current_col");

    SequencerIFaceFactory<ObjectFactory, SeqMatrix> obj("seq.matrix");

    obj.setXletsInfo({ "bang:  reset and start sequencer\n"
                       "start: start sequencer\n"
                       "stop:  stop sequencer\n"
                       "float (1|0):  reset and start/stop sequencer\n",
                         "float: set number of steps" },
        { "any: output for ui.matrix", "\\[i IDX N( - sequence iteration\n"
                                       "\\[ri IDX N( - repeat iteration\n"
                                       "\\[done( - when done" });
}
