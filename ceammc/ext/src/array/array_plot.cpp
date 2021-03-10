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
#include "array_plot.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>

constexpr size_t NMAX = 2048;
constexpr int FMIN = std::numeric_limits<int16_t>::lowest();
constexpr int FMAX = std::numeric_limits<int16_t>::max();
constexpr int PLOT_YMAX = 2048;
constexpr int PLOT_YMIN = -2048;

static t_float round_fixed(t_float x, size_t ndigits)
{
    const auto n = std::pow(10, ndigits);
    return std::round(x * n) / n;
}

ArrayPlot::ArrayPlot(const PdArgs& a)
    : BaseObject(a)
    , array_name_(&s_)
    , ymin_(nullptr)
    , ymax_(nullptr)
    , yauto_(nullptr)
    , nan_(nullptr)
    , phase_(0)
    , total_(0)
    , min_(0)
    , max_(0)
    , running_(false)
{
    createCbSymbolProperty(
        "@array",
        [this]() -> t_symbol* { return array_name_; },
        [this](t_symbol* s) -> bool { setArray(s); return true; })
        ->setArgIndex(0);

    ymin_ = new FloatProperty("@ymin", -1);
    ymin_->checkClosedRange(PLOT_YMIN, PLOT_YMAX);
    ymin_->setArgIndex(1);
    addProperty(ymin_);
    ymax_ = new FloatProperty("@ymax", 1);
    ymax_->checkClosedRange(PLOT_YMIN, PLOT_YMAX);
    ymax_->setArgIndex(2);
    addProperty(ymax_);

    yauto_ = new BoolProperty("@yauto", true);
    addProperty(yauto_);

    nan_ = new FloatProperty("@nan", std::numeric_limits<t_float>::max());
    addProperty(nan_);

    createInlet();
    createOutlet();
}

void ArrayPlot::onInlet(size_t n, const AtomListView& l)
{
    reset();

    if (n != 1)
        return;

    if (!checkArray(true))
        return;

    if (l.empty()) {
        phase_ = 0;
        total_ = array_.size();
        running_ = true;
    } else {
        if (!l[0].isInteger()) {
            OBJ_ERR << "invalid args: " << l;
            return;
        }

        auto N = l[0].toT<int>(0);
        if (N < 0) {
            OBJ_ERR << "invalid plot size: " << N;
            return;
        }

        resizeArray(N);
    }
}

void ArrayPlot::onFloat(t_float sample)
{
    if (!running_)
        return;

    if (phase_ < total_) {
        const bool yauto = yauto_->value();
        const auto ymin = ymin_->value();
        const auto ymax = ymax_->value();

        if (!array_.update()) {
            OBJ_ERR << fmt::format("array '{}' access error", array_.name()->s_name);
            return reset();
        }

        const auto N = array_.size();
        if (total_ > N) {
            OBJ_ERR << fmt::format("invalid plot size: {} for array '{}'", total_, array_.name()->s_name);
            return reset();
        }

        plotSample(processSample(sample, ymin, ymax, yauto));
    } else
        done();
}

void ArrayPlot::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    if (!checkArray(true))
        return;

    const size_t N = lst.size();
    if (!resizeArray(N))
        return;

    const bool yauto = yauto_->value();
    const auto ymin = ymin_->value();
    const auto ymax = ymax_->value();

    for (auto& a : lst) {
        auto s = processSample(a.asFloat(), ymin, ymax, yauto);
        plotSample(s);
    }

    reset();
}

bool ArrayPlot::setArray(t_symbol* s)
{
    array_name_ = s;
    if (!array_.open(array_name_)) {
        OBJ_ERR << "array not found: " << s->s_name;
        return false;
    }

    return true;
}

bool ArrayPlot::checkArray(bool log)
{
    if (array_name_ == nullptr || !array_.open(array_name_)) {
        if (log && array_name_ != &s_) {
            OBJ_ERR << "invalid array: " << array_.name();
        }

        return false;
    }

    return true;
}

void ArrayPlot::updateScale()
{
    constexpr t_float MIN_RANGE = 0.1;

    t_float a, b;
    if (!yauto_->value()) {
        a = ymin_->value();
        b = ymax_->value();
    } else {
        a = clip_min<decltype(min_), PLOT_YMIN>(min_);
        b = clip_max<decltype(max_), PLOT_YMAX>(max_);
    }

    auto is_odd = [](t_float v) { return v == int(v) && (int(v) & 0x1) && int(v) > 1; };

    // zero values
    if (b == a && a == 0) {
        b = 1;
        a = 0;
    }

    float a0 = round_fixed(a, 2);
    float b0 = round_fixed(b, 2);
    float dist = std::fabs(b0 - a0);

    if (dist < MIN_RANGE) {
        a0 = b0 - MIN_RANGE;
        dist = MIN_RANGE;
    }

    dist += is_odd(dist);

    const float m0 = round_fixed(a0 + (dist / 2), 2);
    array_.setYBounds(a0, b0);

    if (!array_.setYLabels({ a0, m0, b0 }))
        OBJ_ERR << fmt::format("can't set ylabels for array '{}'", array_.name()->s_name);

    auto step = std::pow(10, std::trunc(std::log10(dist)) - 1);
    const int nsteps = dist / step;
    if (nsteps > 40)
        step *= 10;

    auto pt = a0;
    if (int(a0) != a0 && int(b0) == b0)
        pt = b0;

    if (!array_.setYTicks(pt, step, 5))
        OBJ_ERR << fmt::format("can't set yticks for array '{}'", array_.name()->s_name);

    array_.redraw();
}

void ArrayPlot::done()
{
    if (total_ != 0)
        updateScale();

    reset();
    bangTo(0);
}

void ArrayPlot::reset()
{
    running_ = false;
    phase_ = 0;
    total_ = 0;
    min_ = 0;
    max_ = 0;
}

bool ArrayPlot::resizeArray(size_t n)
{
    if (n > 0 && n <= NMAX) {
        if (array_.size() != n) {
            if (!array_.resize(n)) {
                OBJ_ERR << fmt::format("can't resize array '{}' to {}", array_.name()->s_name, n);
                return false;
            }

            OBJ_LOG << fmt::format("resizing array '{}' to {}", array_.name()->s_name, n);
        }

        phase_ = 0;
        total_ = n;
        running_ = true;
        return true;
    } else {
        OBJ_ERR << fmt::format("invalid array size: {}, expected value in 1..{} range", n, NMAX);
        reset();
        return false;
    }
}

t_float ArrayPlot::processSample(t_float sample, t_float ymin, t_float ymax, bool auto_range)
{
    if (auto_range) {
        if (std::isnan(sample) || std::isinf(sample))
            sample = nan_->value();

        sample = clip<t_float, PLOT_YMIN, PLOT_YMAX>(sample);

        if (phase_ == 0) {
            min_ = sample;
            max_ = sample;
        } else {
            min_ = std::min(min_, sample);
            max_ = std::max(max_, sample);
        }
    } else {
        if (std::isnan(sample) || std::isinf(sample))
            sample = ymax;

        sample = clip(sample, ymin, ymax);
    }

    return sample;
}

void ArrayPlot::plotSample(t_float sample)
{
    assert(total_ > 0);
    const auto last = total_ - 1;

    if (phase_ >= last) {
        if (phase_ == last) // write last sample
            array_[phase_] = sample;

        done();
    } else
        array_[phase_++] = sample;
}

void setup_array_plot()
{
    ObjectFactory<ArrayPlot> obj("array.plot");

    obj.setXletsInfo({ "float: floats to plot\n"
                       "list: samples to plot",
                         "bang: plot samples\n"
                         "int: resize array and plot samples" },
        { "bang: on done" });
}
