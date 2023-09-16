/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "base_gate_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

constexpr int DEF_XLET = 1;
constexpr int MIN_XLET = 1;
constexpr int MAX_XLET = 64;

static char INLET_INFO[MAX_XLET][16];
static char OUTLET_INFO[MAX_XLET][16];

static void initXletInfo()
{
    for (int i = 0; i < MAX_XLET; i++)
        fmt::format_to(INLET_INFO[i], "input: {}\0", i + 1);

    for (int i = 0; i < MAX_XLET; i++)
        fmt::format_to(OUTLET_INFO[i], "outlet: {}\0", i + 1);
}

BaseGateTilde::BaseGateTilde(const PdArgs& args)
    : SoundExternal(args)
{
    n_ = new IntProperty("@n", DEF_XLET, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_XLET, MAX_XLET);
    n_->setArgIndex(0);
    addProperty(n_);

    init_ = new BoolProperty("@init", true, PropValueAccess::INITONLY);
    init_->setArgIndex(1);
    addProperty(init_);

    smooth_ = new FloatProperty("@smooth", 50);
    smooth_->checkClosedRange(0, 1000);
    smooth_->setUnitsMs();
    smooth_->setSuccessFn([this](Property*) {
        using namespace convert;
        auto tau = msec2tau(smooth_->value());
        smooth_pole_ = tau2pole<double>(tau, samplerate());
    });
    addProperty(smooth_);
}

void BaseGateTilde::initDone()
{
    for (int i = 1; i < n_->value(); i++)
        createSignalInlet();

    createInlet();

    smoo_.reserve(n_->value());
    t_sample init_value = init_->value();
    for (int i = 0; i < n_->value(); i++) {
        createSignalOutlet();
        smoo_.emplace_back(0, init_value);
    }

    smooth_->callSuccessFn();
}

void BaseGateTilde::onInlet(size_t n, const AtomListView& lv)
{
    if (n != n_->value())
        return;

    if (lv.isBool()) {
        auto x = lv.boolAt(0, false);
        for (auto& s : smoo_)
            s.setTarget(x);
    } else {
        auto N = std::min<size_t>(lv.size(), smoo_.size());
        for (size_t i = 0; i < N; i++)
            smoo_[i].setTarget(lv.boolAt(i, false));
    }
}

void BaseGateTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t N = smoo_.size();
    const auto BS = blockSize();

    t_sample buf[N];

    for (size_t k = 0; k < BS; k++) {
        for (size_t i = 0; i < N; i++)
            buf[i] = in[i][k] * smoo_[i].tick(smooth_pole_);

        for (size_t i = 0; i < N; i++)
            out[i][k] = buf[i];
    }
}

const char* BaseGateTilde::annotateInlet(size_t n) const
{
    return (n < n_->value()) ? INLET_INFO[n] : "bool or list of 1 and 0";
}

const char* BaseGateTilde::annotateOutlet(size_t n) const
{
    return (n < MAX_XLET) ? OUTLET_INFO[n] : "";
}

void setup_base_gate_tilde()
{
    initXletInfo();
    SoundExternalFactory<BaseGateTilde> obj("gate~");
}
