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
#include "route_cycle.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_proxy.h"
#include "fmt/core.h"

constexpr int MIN_STEPS = 1;
constexpr int MAX_STEPS = 64;
constexpr int DEF_STEPS = 2;

using OutletInfo = char[24];
static std::array<OutletInfo, MAX_STEPS> OUTLET_INFO {};

RouteCycle::RouteCycle(const PdArgs& args)
    : BaseObject(args)
    , inlet1_(this)
{
    bindProxyInlet(inlet1_, 0);

    n_ = new IntProperty("@n", DEF_STEPS, PropValueAccess::INITONLY);
    n_->checkClosedRange(MIN_STEPS, MAX_STEPS);
    n_->setArgIndex(0);
    addProperty(n_);

    offset_ = new IntProperty("@offset", 0);
    offset_->setArgIndex(1);
    offset_->checkClosedRange(-MAX_STEPS, MAX_STEPS);
    addProperty(offset_);

    step_ = new IntProperty("@step", 1);
    step_->setArgIndex(2);
    addProperty(step_);

    createCbIntProperty(
        "@index",
        [this]() -> int {
            return calcCurrentOutlet();
        },
        [this](int x) -> bool {
            cur_idx_ = clip<int, -MAX_STEPS, MAX_STEPS>(x - offset_->value());
            return true;
        });
}

void RouteCycle::initDone()
{
    for (int i = 0; i < n_->value(); i++)
        createOutlet();

    cur_idx_ = 0;
}

void RouteCycle::onBang()
{
    bangTo(calcCurrentOutlet());
    nextIndex();
}

void RouteCycle::onFloat(t_float f)
{
    floatTo(calcCurrentOutlet(), f);
    nextIndex();
}

void RouteCycle::onSymbol(t_symbol* s)
{
    symbolTo(calcCurrentOutlet(), s);
    nextIndex();
}

void RouteCycle::onList(const AtomListView& lv)
{
    listTo(calcCurrentOutlet(), lv);
    nextIndex();
}

void RouteCycle::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(calcCurrentOutlet(), s, lv);
    nextIndex();
}

void RouteCycle::onProxyBang(int idx)
{
    cur_idx_ = 0;
}

const char* RouteCycle::annotateOutlet(size_t n) const
{
    return n < OUTLET_INFO.size() ? OUTLET_INFO[n] : "??";
}

size_t RouteCycle::calcCurrentOutlet() const
{
    auto idx = (cur_idx_ + offset_->value());
    const auto N = n_->value();
    while (idx < 0)
        idx += N;

    while (idx >= N)
        idx -= N;

    return idx;
}

void RouteCycle::nextIndex()
{
    cur_idx_ += step_->value();
    normalizeIndex();
}

void RouteCycle::normalizeIndex()
{
    const auto N = n_->value();
    while (cur_idx_ < 0)
        cur_idx_ += N;

    while (cur_idx_ >= N)
        cur_idx_ -= N;
}

void setup_route_cycle()
{
    for (size_t i = 0; i < OUTLET_INFO.size(); i++)
        fmt::format_to(OUTLET_INFO[i], "outlet \\[{}\\]\0", i);

    ObjectFactory<RouteCycle> obj("route.cycle");
    obj.setXletsInfo({ "any: input stream", "bang: reset counter" }, {});
    obj.noPropsDispatch();
    obj.noInletsDispatch();

    obj.useProxyBang();
}
