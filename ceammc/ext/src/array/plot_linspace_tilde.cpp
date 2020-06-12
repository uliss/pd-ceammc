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
#include "plot_linspace_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

PlotLinTilde::PlotLinTilde(const PdArgs& a)
    : SoundExternal(a)
    , start_(nullptr)
    , stop_(nullptr)
    , num_(nullptr)
    , endpoint_(nullptr)
    , value_(0)
    , incr_(0)
    , phase_(0)
    , clock_([this] { floatTo(1, 0); })
    , running_(false)
{
    start_ = new FloatProperty("@start", 0);
    start_->setArgIndex(0);
    addProperty(start_);

    stop_ = new FloatProperty("@stop", 255);
    stop_->setArgIndex(1);
    addProperty(stop_);

    num_ = new IntProperty("@n", 256);
    num_->checkClosedRange(4, 1024);
    num_->setArgIndex(2);
    addProperty(num_);

    endpoint_ = new BoolProperty("@endpoint", true);
    addProperty(endpoint_);

    createSignalOutlet();
    createOutlet();
}

void PlotLinTilde::onBang()
{
    const int n = (num_->value() - (endpoint_->value() ? 1 : 0));

    clock_.unset();
    phase_ = 0;
    value_ = start_->value();
    incr_ = (stop_->value() - start_->value()) / n;
    running_ = true;

    listTo(1, { (t_float)num_->value(), start_->value(), stop_->value(), 0 });
}

void PlotLinTilde::onFloat(t_float n)
{
    if (!num_->setValue(n))
        return;

    onBang();
}

void PlotLinTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const auto T = num_->value() - 1;
    size_t i = 0;

    if (running_) {
        for (i = 0; i < BS; i++, phase_++) {
            if (phase_ < T) {
                out[0][i] = value_;
                value_ += incr_;
            } else {
                running_ = false;
                clock_.delay(0);
                break;
            }
        }
    }

    // remaining
    for (; i < BS; i++)
        out[0][i] = value_;
}

void setup_plot_linspace_tilde()
{
    SoundExternalFactory<PlotLinTilde> obj("plot.linspace~", OBJECT_FACTORY_DEFAULT);
}
