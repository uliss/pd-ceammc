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

static t_symbol* SYM_BASE_10;
static t_symbol* SYM_BASE_2;
static t_symbol* SYM_BASE_E;

PlotLogTilde::PlotLogTilde(const PdArgs& a)
    : SoundExternal(a)
    , xmin_(nullptr)
    , xmax_(nullptr)
    , steps_(nullptr)
    , running_(false)
    , clock_([this]() { floatTo(1, 0); })
    , base_(nullptr)
{
    xmin_ = new FloatProperty("@xmin", 0);
    xmin_->setArgIndex(0);
    addProperty(xmin_);

    xmax_ = new FloatProperty("@xmax", 2);
    xmax_->setArgIndex(1);
    addProperty(xmax_);

    steps_ = new IntProperty("@steps", 256);
    steps_->checkClosedRange(4, 1024);
    steps_->setArgIndex(2);
    addProperty(steps_);

    base_ = new SymbolEnumProperty("@base", { SYM_BASE_10, SYM_BASE_2, SYM_BASE_E });
    addProperty(base_);

    addProperty(new SymbolEnumAlias("@10", base_, SYM_BASE_10));
    addProperty(new SymbolEnumAlias("@2", base_, SYM_BASE_2));
    addProperty(new SymbolEnumAlias("@e", base_, SYM_BASE_E));

    createSignalOutlet();
    createOutlet();
}

void PlotLogTilde::onBang()
{
    if (xmin_->value() == xmax_->value()) {
        OBJ_ERR << fmt::format("{} != {} expected, got: {} == {}",
            xmin_->name()->s_name,
            xmax_->name()->s_name,
            xmin_->value(),
            xmax_->value());

        return;
    }

    clock_.unset();

    const t_float R = xmax_->value() - xmin_->value();
    value_ = xmin_->value();
    incr_ = R / steps_->value();
    phase_ = 0;
    running_ = true;

    if (base_->value() == SYM_BASE_2)
        fbase_ = 2;
    else if (base_->value() == SYM_BASE_E)
        fbase_ = std::exp(t_sample(1));
    else if (base_->value() == SYM_BASE_10)
        fbase_ = 10;

    listTo(1, { (t_float)steps_->value() + 1, std::pow(fbase_, xmin_->value()), std::pow(fbase_, xmax_->value()), base_->value() });
}

void PlotLogTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const size_t T = steps_->value();
    const auto v1 = xmax_->value();

    for (size_t i = 0; i < BS; i++) {

        if (!running_) {
            out[0][i] = 0;
        } else if (phase_ < T) {
            out[0][i] = std::pow(fbase_, value_);
            value_ += incr_;
            phase_++;
        } else if (phase_ == T) {
            out[0][i] = std::pow(fbase_, v1);
            clock_.delay(0);
            running_ = false;
        }
    }
}

void setup_plot_log_tilde()
{
    SYM_BASE_10 = gensym("log10");
    SYM_BASE_2 = gensym("log2");
    SYM_BASE_E = gensym("ln");

    SoundExternalFactory<PlotLogTilde> obj("plot.logspace~", OBJECT_FACTORY_DEFAULT);
    obj.useDefaultPdFloatFn();
}
