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
#include "seq_nbangs.h"
#include "ceammc_factory.h"

static t_symbol* SYM_DONE;

SeqNBangs::SeqNBangs(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
    , interval_(nullptr)
    , counter_(0)
    , clock_([this]() {
        if (tick())
            clock_.delay(interval_->value());
    })
{
    createInlet();
    createOutlet();
    createOutlet();

    n_ = new IntProperty("@n", 0);
    n_->checkNonNegative();
    n_->setArgIndex(0);
    addProperty(n_);

    interval_ = new SeqTimeGrain("@t", 0);
    interval_->setArgIndex(1);
    addProperty(interval_);

    createCbFloatProperty(
        "@dur",
        [this]() -> t_float { return n_->value() * interval_->value(); },
        [this](t_float f) -> bool {
            const auto N = n_->value();
            if (N == 0) {
                OBJ_ERR << "empty sequence";
                return false;
            }

            return interval_->setValue(f / N);
        })
        ->checkNonNegative();
}

void SeqNBangs::onBang()
{
    reset();
    clock_.exec();
}

void SeqNBangs::onFloat(t_float f)
{
    reset();

    if (n_->setValue(f))
        clock_.exec();
}

void SeqNBangs::onList(const AtomList& l)
{
    if (l.empty())
        return onBang();
    else if (l.size() == 1)
        return onFloat(l[0].asFloat());
    else if (l.size() == 2) {
        reset();
        if (n_->set(l.view(0, 1)) && interval_->set(l.view(1, 1)))
            clock_.exec();
    } else
        OBJ_ERR << "usage: NUM INTERVAL";
}

void SeqNBangs::onInlet(size_t n, const AtomList& lv)
{
    reset();
}

void SeqNBangs::start()
{
    reset();
    clock_.exec();
}

void SeqNBangs::stop()
{
    reset();
}

void SeqNBangs::reset()
{
    counter_ = 0;
    clock_.unset();
}

bool SeqNBangs::tick()
{
    if ((int)counter_ >= n_->value()) {
        anyTo(1, SYM_DONE, AtomListView());
        return false;
    } else {
        floatTo(1, counter_);
        bangTo(0);

        counter_++;
        return true;
    }
}

void setup_seq_nbangs()
{
    SYM_DONE = gensym("done");

    SequencerIFaceFactory<ObjectFactory, SeqNBangsT> obj("seq.nbangs");
    obj.addAlias("seq.nb");

    obj.setXletsInfo({ "bang: start\n"
                       "float: set number of bangs then start\n"
                       "list: NUM INTERVAL set number and interval then start\n"
                       "start 1|0: start/stop sequence\n"
                       "stop 1|0:  stop strt sequence",
                         "bang: stop sequence output and reset" },
        { "bang", "float: bang index or 'done' after last bang" });
}
