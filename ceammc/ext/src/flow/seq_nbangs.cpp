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

SeqNBangs::SeqNBangs(const PdArgs& args)
    : BaseObject(args)
    , n_(nullptr)
    , interval_ms_(nullptr)
    , clock_([this]() {
        if (counter_ < n_->value()) {
            counter_++;
            clock_.delay(interval_ms_->value());
            bangTo(0);

            if (counter_ == n_->value())
                bangTo(1);
        }
    })
    , counter_(0)
{
    createInlet();
    createOutlet();
    createOutlet();

    n_ = new IntProperty("@n", 0);
    n_->checkNonNegative();
    n_->setArgIndex(0);
    addProperty(n_);

    interval_ms_ = new FloatProperty("@t", 0);
    interval_ms_->checkNonNegative();
    interval_ms_->setUnits(PropValueUnits::MSEC);
    interval_ms_->setArgIndex(1);
    addProperty(interval_ms_);
}

void SeqNBangs::onBang()
{
    counter_ = 0;
    clock_.exec();
}

void SeqNBangs::onFloat(t_float f)
{
    counter_ = 0;
    if (n_->setValue(f))
        clock_.exec();
}

void SeqNBangs::onInlet(size_t n, const AtomList& lv)
{
    m_reset(&s_, lv);
}

void SeqNBangs::m_reset(t_symbol* s, const AtomListView&)
{
    clock_.unset();
    counter_ = 0;
}

void SeqNBangs::m_start(t_symbol* s, const AtomListView& lv)
{
    const auto on = lv.boolAt(0, true);

    if (on)
        clock_.exec();
    else
        clock_.unset();
}

void SeqNBangs::m_stop(t_symbol* s, const AtomListView&)
{
    clock_.unset();
}

void setup_seq_nbangs()
{
    ObjectFactory<SeqNBangs> obj("seq.nbangs");
    obj.addAlias("seq.nb");
    obj.addMethod("reset", &SeqNBangs::m_reset);
    obj.addMethod("start", &SeqNBangs::m_start);
    obj.addMethod("stop", &SeqNBangs::m_stop);

    obj.useDefaultPdListFn();

    obj.setXletsInfo({ "bang: start\n"
                       "float: set number of bangs then start\n"
                       "list: NUM INTERVAL set number and interval then start\n"
                       "start 1|0:  start/stop sequence\n"
                       "stop: sequence output",
                         "bang: stop sequence output and reset" },
        { "bang", "bang after last sequence output" });
}
