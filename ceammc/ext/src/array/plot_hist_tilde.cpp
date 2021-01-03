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
#include "plot_hist_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

constexpr size_t NBINS_DEFAULT = 63;

PlotHistTilde::PlotHistTilde(const PdArgs& args)
    : SoundExternal(args)
    , nsamp_(nullptr)
    , nbins_(nullptr)
    , min_(nullptr)
    , max_(nullptr)
    , clip_(nullptr)
    , buf_(NBINS_DEFAULT + 1)
    , clock_([this] {
        state_ = OUTPUT;
        phase_ = 0;
        Atom out[4] = { (t_float)buf_.size(), min_->value(), max_->value(), gensym("hist") };
        listTo(1, AtomListView(out, 4));
    })
    , phase_(0)
{
    createSignalOutlet();
    createOutlet();

    nsamp_ = new IntProperty("@nsamp", 512);
    nsamp_->checkClosedRange(8, 1024 * 32);
    nsamp_->setArgIndex(0);
    addProperty(nsamp_);

    nbins_ = new IntProperty("@nbins", NBINS_DEFAULT);
    nbins_->checkClosedRange(8, 512);
    addProperty(nbins_);

    min_ = new FloatProperty("@min", -1);
    addProperty(min_);

    max_ = new FloatProperty("@max", 1);
    addProperty(max_);

    clip_ = new BoolProperty("@clip", true);
    addProperty(clip_);
}

void PlotHistTilde::onBang()
{
    if (min_->value() >= max_->value()) {
        OBJ_ERR << fmt::format("{} != {} expected, got: {} == {}",
            min_->name()->s_name,
            max_->name()->s_name,
            min_->value(),
            max_->value());

        return;
    }

    phase_ = 0;
    clock_.unset();
    state_ = CALC;
    buf_.assign(nbins_->value(), 0);
}

void PlotHistTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const int NBINS = buf_.size();
    const int NSAMP = nsamp_->value();

    switch (state_) {
    case CALC: {
        const t_sample a = min_->value();
        const t_sample b = max_->value();
        const t_sample r = b - a;
        const bool do_clip = clip_->value();

        for (size_t i = 0; i < BS; i++, phase_++) {
            if (phase_ < NSAMP) {
                auto v = in[0][i];
                out[0][i] = 0; // output always

                if (v < a || v > b) {
                    if (do_clip) // clip sample
                        v = clip(v, a, b);
                    else
                        continue; // skip sample
                }

                if (r <= 0) // avoid division by zero
                    continue;

                auto idx = int((v - a) / (r / NBINS));
                if (idx < 0 || idx >= NBINS)
                    continue;

                buf_[idx]++;
            } else if (phase_ == NSAMP) {
                state_ = OUTPUT;
                clock_.delay(0);
                out[0][i] = 0; // output always
            } else {
                out[0][i] = 0; // output always
            }
        }

        // go to output
    } break;
    case OUTPUT:

        for (size_t i = 0; i < BS; i++, phase_++) {
            if (phase_ < NBINS) {
                out[0][i] = buf_[phase_];
            } else {
                out[0][i] = 0;
                state_ = IDLE;
            }
        }

        break;
    default:

        for (size_t i = 0; i < BS; i++)
            out[0][i] = 0;

        break;
    }
}

void setup_plot_hist_tilde()
{
    SoundExternalFactory<PlotHistTilde> obj("plot.hist~");
    obj.addAlias("hist~");
}
