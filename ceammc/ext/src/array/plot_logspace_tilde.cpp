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
#include "plot_logspace_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

using PropAlias = AliasProperty<EnumProperty<Atom>>;

PlotLogTilde::PlotLogTilde(const PdArgs& a)
    : SoundExternal(a)
    , start_(nullptr)
    , stop_(nullptr)
    , num_(nullptr)
    , endpoint_(nullptr)
    , phase_(0)
    , running_(false)
    , base_(nullptr)
{
    start_ = new FloatProperty("@start", 0);
    start_->setArgIndex(0);
    addProperty(start_);

    stop_ = new FloatProperty("@stop", 2);
    stop_->setArgIndex(1);
    addProperty(stop_);

    num_ = new IntProperty("@n", 100);
    num_->checkClosedRange(4, 1024);
    num_->setArgIndex(2);
    addProperty(num_);

    base_ = new EnumProperty<Atom>("@base", { Atom(10), Atom(2), Atom(gensym("e")) });
    base_->setArgIndex(3);
    addProperty(base_);

    addProperty(new PropAlias("@10", base_, 10));
    addProperty(new PropAlias("@2", base_, 2));
    addProperty(new PropAlias("@e", base_, gensym("e")));

    endpoint_ = new BoolProperty("@endpoint", true);
    addProperty(endpoint_);

    createSignalOutlet();
    createOutlet();
}

void PlotLogTilde::onBang()
{
    static const t_sample m_e = std::exp(t_sample(1));

    if (start_->value() == stop_->value()) {
        OBJ_ERR << fmt::format("{} != {} expected, got: {} == {}",
            start_->name()->s_name,
            stop_->name()->s_name,
            start_->value(),
            stop_->value());

        return;
    }

    const t_float R = stop_->value() - start_->value();
    value_ = start_->value();
    incr_ = R / (num_->value() - (endpoint_->value() ? 1 : 0));
    phase_ = 0;
    running_ = true;

    if (base_->value() == Atom(2))
        fbase_ = 2;
    else if (base_->value() == Atom(10))
        fbase_ = 10;
    else if (base_->value() == Atom(gensym("e")))
        fbase_ = m_e;

    Atom out[4] = { (t_float)num_->value(), std::pow(fbase_, start_->value()), std::pow(fbase_, stop_->value()), base_->value() };
    listTo(1, AtomListView(out, 4));
}

void PlotLogTilde::onFloat(t_float f)
{
    if (!num_->setValue(f))
        return;

    onBang();
}

void PlotLogTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const int T = num_->value() - 1;
    t_sample vend = std::pow(fbase_, value_);

    for (size_t i = 0; i < BS; i++, phase_++) {
        if (!running_) {
            out[0][i] = vend;
        } else if (phase_ < T) {
            out[0][i] = std::pow(fbase_, value_);
            value_ += incr_;
        } else {
            if (endpoint_->value())
                value_ = stop_->value();

            vend = std::pow(fbase_, value_);
            out[0][i] = vend;
            running_ = false;
        }
    }
}

void setup_plot_logspace_tilde()
{
    SoundExternalFactory<PlotLogTilde> obj("plot.logspace~", OBJECT_FACTORY_DEFAULT);
}
