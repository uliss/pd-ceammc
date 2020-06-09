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
#include "plot_lin_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

PlotLinTilde::PlotLinTilde(const PdArgs& a)
    : SoundExternal(a)
    , xmin_(nullptr)
    , xmax_(nullptr)
    , steps_(nullptr)
    , value_(0)
    , incr_(0)
    , phase_(0)
    , clock_([this] { floatTo(1, 0); })
    , running_(false)
{
    xmin_ = new FloatProperty("@xmin", 0);
    xmin_->setArgIndex(0);
    addProperty(xmin_);

    xmax_ = new FloatProperty("@xmax", 256);
    xmax_->setArgIndex(1);
    addProperty(xmax_);

    steps_ = new IntProperty("@steps", 256);
    steps_->checkClosedRange(4, 1024);
    steps_->setArgIndex(2);
    addProperty(steps_);

    createSignalOutlet();
    createOutlet();
}

void PlotLinTilde::onBang()
{
    if (xmin_->value() >= xmax_->value()) {
        OBJ_ERR << fmt::format("@xmin < @xmax expected, got: {} >= {}", xmin_->value(), xmax_->value());
        return;
    }

    clock_.unset();
    phase_ = 0;
    value_ = xmin_->value();
    incr_ = (xmax_->value() - xmin_->value()) / steps_->value();
    running_ = true;

    listTo(1, { (t_float)steps_->value() + 1, xmin_->value(), xmax_->value(), 0 });
}

void PlotLinTilde::onFloat(t_float n)
{
    if (!steps_->setValue(n))
        return;

    onBang();
}

void PlotLinTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const size_t T = steps_->value();

    for (size_t i = 0; i < BS; i++) {
        out[0][i] = value_;

        if (!running_)
            continue;
        else if (phase_ <= T) {
            value_ += incr_;
            phase_++;
        } else if (phase_ > T) {
            running_ = false;
            clock_.delay(0);
        }
    }
}

void setup_plot_lin_tilde()
{
    SoundExternalFactory<PlotLinTilde> obj("plot.lin~", OBJECT_FACTORY_DEFAULT);
}
