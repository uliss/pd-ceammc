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
#include "plot_response_tilde.h"
#include "ceammc_factory.h"

#include <cmath>
#include <complex>

PlotResponseTilde::PlotResponseTilde(const PdArgs& a)
    : SoundExternal(a)
    , n_(nullptr)
    , dbfs_(nullptr)
    , use_sr_(nullptr)
    , real_(512)
    , imag_(512)
    , phase_(0)
    , clock_([this]() {
        state_ = OUTPUT;
        phase_ = 0;
        const size_t N = real_.size();
        mayer_fft(N, real_.data(), imag_.data());

        for (size_t i = 0; i < N / 2; i++) {
            std::complex<t_sample> z(real_[i], imag_[i]);

            real_[i] = std::abs(z);
            imag_[i] = std::arg(z);
        }

        if (use_sr_->value()) {
            Atom out[4] = { (t_float)N / 2, 0., sys_getsr() / 2, 0. };
            listTo(2, AtomListView(out, 4));
        } else {
            Atom out[4] = { (t_float)N / 2, 0., std::acos(-1), gensym("pi") };
            listTo(2, AtomListView(out, 4));
        }
    })
{
    n_ = new IntProperty("@n", 512);
    n_->checkClosedRange(32, 8192);
    n_->setSuccessFn([this](Property*) { real_.resize(n_->value()); imag_.resize(real_.size()); });
    n_->setArgIndex(0);
    addProperty(n_);

    dbfs_ = new BoolProperty("@db", false);
    addProperty(dbfs_);

    use_sr_ = new BoolProperty("@sr", false);
    addProperty(use_sr_);

    createInlet();
    createSignalOutlet();
    createSignalOutlet();

    createOutlet();
}

void PlotResponseTilde::onInlet(size_t, const AtomListView&)
{
    clock_.unset();
    phase_ = 0;
    state_ = CALC;
    imag_.assign(real_.size(), 0);
}

void PlotResponseTilde::processBlock(const t_sample** in, t_sample** out)
{
    switch (state_) {
    case IDLE:
        for (size_t i = 0; i < blockSize(); i++) {
            out[0][i] = 0;
            out[1][i] = 0;
        }

        break;

    case CALC:
        for (size_t i = 0; i < blockSize(); i++, phase_++) {
            if (phase_ < real_.size()) {
                real_[phase_] = in[0][i];
            } else {
                clock_.delay(0);
                state_ = OUTPUT;
            }

            out[0][i] = 0;
            out[1][i] = 0;
        }
        break;

    case OUTPUT:
        const bool in_db = dbfs_->value();
        const size_t N = real_.size() / 2;

        for (size_t i = 0; i < blockSize(); i++, phase_++) {
            if (phase_ < N) {
                auto amp = real_[phase_];
                out[0][i] = in_db ? (20 * std::log10(amp)) : amp;
                out[1][i] = imag_[phase_];
            } else {
                state_ = IDLE;
                out[0][i] = 0;
                out[1][i] = 0;
            }
        }
        break;
    }
}

void setup_plot_response_tilde()
{
    SoundExternalFactory<PlotResponseTilde> obj("plot.response~");

    obj.setDescription("output amp/phase freq response to impulse");
    obj.setCategory("array");
    obj.setKeywords({"array", "histogram"});
}
