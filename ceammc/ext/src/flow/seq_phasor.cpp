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

SeqPhasor::SeqPhasor(const PdArgs& a)
    : BaseObject(a)
    , freq_hz_(nullptr)
    , precision_(nullptr)
    , on_(nullptr)
    , invert_(nullptr)
    , clock_([this]() {
        const size_t nsteps = 1 / precision_->value();
        const t_float value = (invert_->value()) ? 1 - t_float(index_) / nsteps
                                                 : t_float(index_) / nsteps;

        if (freq_hz_->value() == 0) { // const output
            floatTo(0, value);
        } else {
            const auto delay_ms = 1000 / (freq_hz_->value() * nsteps);
            clock_.delay(delay_ms);
            floatTo(0, value);

            if (++index_ >= nsteps) {
                index_ = 0;
                bangTo(1);
            }
        }
    })
    , index_(0)
{
    createInlet();
    createOutlet();
    createOutlet();

    freq_hz_ = new FloatProperty("@freq", 0);
    freq_hz_->setUnits(PropValueUnits::HZ);
    freq_hz_->checkClosedRange(0, 100);
    freq_hz_->setArgIndex(0);
    addProperty(freq_hz_);

    precision_ = new FloatProperty("@precision", 1. / 128);
    precision_->checkClosedRange(1. / 1024, 1. / 2);
    addProperty(precision_);

    on_ = new BoolProperty("@on", false);
    on_->setSuccessFn([this](Property*) {
        if (on_->value()) {
            if (!clock_.isActive())
                clock_.exec();
        } else
            clock_.unset();
    });
    on_->setArgIndex(1);
    addProperty(on_);

    invert_ = new BoolProperty("@invert", false);
    addProperty(invert_);
}

void SeqPhasor::initDone()
{
    if (on_->value())
        clock_.exec();
}

void SeqPhasor::onFloat(t_float f)
{
    Atom a(f);
    on_->set(AtomListView(a));
}

void SeqPhasor::onInlet(size_t n, const AtomList& l)
{
    index_ = 0;
}

void SeqPhasor::m_set(t_symbol* s, const AtomListView& lv)
{
    if (!lv.isFloat()) {
        METHOD_ERR(s) << "float is expected, got: " << lv;
        return;
    }

    // v in [0, 1) range
    const auto v = wrapFloatMax<t_float>(lv[0].asT<t_float>(), 1);
    // n >= 2
    const size_t nsteps = 1 / precision_->value();
    index_ = size_t(v * nsteps);
}

void setup_seq_phasor()
{
    ObjectFactory<SeqPhasor> obj("seq.phasor");
    obj.addMethod("set", &SeqPhasor::m_set);
    obj.setXletsInfo(
        { "float: 1|0 - start/stop phasor",
            "bang: reset to start" },
        { "float: value in \\[0, 1) range",
            "bang: at the end of cycle" });
}
