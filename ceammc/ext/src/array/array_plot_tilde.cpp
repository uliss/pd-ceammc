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
#include "array_plot_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include <cmath>
#include <cstdint>
#include <limits>

constexpr size_t NMAX = 2048;
constexpr int FMIN = std::numeric_limits<int16_t>::lowest();
constexpr int FMAX = std::numeric_limits<int16_t>::max();
constexpr int PLOT_YMAX = 1024;
constexpr int PLOT_YMIN = -1024;

static t_float round_fixed(t_float x, size_t ndigits)
{
    const auto n = std::pow(10, ndigits);
    return std::round(x * n) / n;
}

ArrayPlotTilde::ArrayPlotTilde(const PdArgs& a)
    : SoundExternal(a)
    , array_name_(&s_)
    , clock_([this]() { updateScale(); })
    , ymin_(nullptr)
    , ymax_(nullptr)
    , yauto_(nullptr)
    , phase_(0)
    , running_(false)
    , total_(0)
    , min_(0)
    , max_(0)
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
    yauto_ = new BoolProperty("@yauto", false);
    addProperty(yauto_);

    createInlet();
    createOutlet();
}

void ArrayPlotTilde::onInlet(size_t n, const AtomListView& l)
{
    if (n != 1)
        return;

    if (!checkArray(true))
        return;

    if (l.empty()) {
        running_ = true;
        phase_ = 0;
        total_ = array_.size();
        return;
    } else {
        if (!l[0].isInteger()) {
            OBJ_ERR << "invalid args: " << l;
            return;
        }

        auto N = l[0].toT<int>(0);
        if (N > NMAX) {
            running_ = false;
            OBJ_ERR << fmt::format("requested plot size is too big: {}, max size is {}", N, NMAX);
            return;
        } else if (N > 0) {
            running_ = true;
            phase_ = 0;
            total_ = N;

            if (array_.size() != total_) {
                array_.resize(static_cast<size_t>(total_));
                OBJ_LOG << "setting number of points: " << total_;
            }

        } else {
            running_ = false;
            phase_ = 0;
            total_ = 0;
        }
    }
}

void ArrayPlotTilde::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    if (!checkArray(true))
        running_ = false;
}

void ArrayPlotTilde::processBlock(const t_sample** in, t_sample** out)
{
    if (!running_)
        return;

    const bool yauto = yauto_->value();
    const auto ymin = ymin_->value();
    const auto ymax = ymax_->value();

    if (phase_ == 0 && yauto) {
        min_ = in[0][0];
        max_ = min_;
    }

    const auto BS = blockSize();
    for (size_t i = 0; i < BS; i++, phase_++) {
        if (phase_ < total_) {
            t_sample v = in[0][i];

            if (yauto) {
                if (std::isnan(v))
                    v = FMAX;
                else if (std::isinf(v))
                    v = FMAX;

                v = clip<t_sample, FMIN, FMAX>(v);
                min_ = std::min(min_, v);
                max_ = std::max(max_, v);
            } else {
                if (std::isnan(v))
                    v = ymax;
                else if (std::isinf(v))
                    v = ymax;

                v = clip(v, ymin, ymax);
            }

            array_[phase_] = v;

        } else {
            running_ = false;
            if (total_ != 0)
                clock_.delay(0);

            break;
        }
    }
}

bool ArrayPlotTilde::setArray(t_symbol* s)
{
    array_name_ = s;
    if (!array_.open(array_name_)) {
        OBJ_ERR << "array not found: " << s->s_name;
        return false;
    }

    array_.useInDSP();

    return true;
}

bool ArrayPlotTilde::checkArray(bool log)
{
    if (array_name_ == nullptr || !array_.open(array_name_)) {
        if (log && array_name_ != &s_) {
            OBJ_ERR << "invalid array: " << array_.name();
        }

        return false;
    }

    return true;
}

void ArrayPlotTilde::updateScale()
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

    const auto step = std::pow(10, std::trunc(std::log10(dist)) - 1);

    if (!array_.setYTicks(a0, step, 5))
        OBJ_ERR << fmt::format("can't set yticks for array '{}'", array_.name()->s_name);

    array_.redraw();
}

void setup_array_plot_tilde()
{
    SoundExternalFactory<ArrayPlotTilde> obj("array.plot~");

    obj.setXletsInfo({ "signal: signal to plot",
                         "bang: plot samples\n"
                         "int: resize array and plot samples" },
        { "" });

    obj.setDescription("plot signal on specified array");
    obj.setCategory("array");
}
