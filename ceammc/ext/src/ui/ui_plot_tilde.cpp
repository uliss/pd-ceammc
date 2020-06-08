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
#include "ui_plot_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"

#include <cstdio>

constexpr int MIN_INPUTS = 1;
constexpr int MAX_INPUTS = 4;
constexpr float MIN_XOFF = 25;
constexpr float MIN_YOFF = 20;
constexpr float OFF_K = 0.05;

constexpr float LABEL_XPAD = 2;
constexpr float LABEL_YPAD = 5;
constexpr float XTICK_MAJ = 6;
constexpr float XTICK_MIN = 3;
constexpr float XGRID_AVOID = 20;

static t_symbol* SYM_YAUTO;
static t_symbol* SYM_YMIN;
static t_symbol* SYM_YMAX;

constexpr float BTN_SIZE = 12;
constexpr float BTN_PAD = 2;
static const char* BTN_LABELS[] = { "T", "g", "G", "L" };
constexpr size_t N_BTNS = sizeof(BTN_LABELS) / sizeof(BTN_LABELS[0]);

static inline double fast_pow10(int n)
{
    switch (n) {
    case 0:
        return 1;
    case 1:
        return 10;
    case 2:
        return 100;
    case 3:
        return 1000;
    case 4:
        return 10000;
    case 5:
        return 100000;
    case -1:
        return 0.1;
    case -2:
        return 0.01;
    case -3:
        return 0.001;
    case -4:
        return 0.0001;
    default:
        return std::pow(10.0, n);
    }
}

static inline double fast_pow2(int n)
{
    switch (n) {
    case 0:
        return 1;
    case -1:
        return 0.5;
    case -2:
        return 0.25;
    case -3:
        return 0.125;
    case -4:
        return 0.0625;
    case -5:
        return 0.03125;
    default:
        return (n > 0) ? (0x1 << n) : std::pow(2, n);
    }
}

static double fast_log10(int i)
{
    static double log10[10] = {
        0,
        std::log10(2),
        std::log10(3),
        std::log10(4),
        std::log10(5),
        std::log10(6),
        std::log10(7),
        std::log10(8),
        std::log10(9),
    };

    return log10[i - 1];
}

static const char* to_label(float v)
{
    static char buf[32];
    if (v == (int)v)
        snprintf(buf, 32, "%d", (int)v);
    else
        snprintf(buf, 32, "%g", v);

    return buf;
}

UIPlotTilde::UIPlotTilde()
    : clock_([this]() {
        plot_layer_.invalidate();
        if (yauto_)
            border_layer_.invalidate();
        redraw();
    })
    , in2_(nullptr)
    , phase_(0)
    , total_(0)
    , ymin_(0)
    , ymax_(0)
    , xmin_(0)
    , xmax_(256)
    , sig_min_(0)
    , sig_max_(0)
    , yauto_(1)
    , xmaj_ticks_(1)
    , xmin_ticks_(1)
    , xmaj_grid_(1)
    , xmin_grid_(0)
    , prop_nins_(1)
    , log_base_(LB_NONE)
    , plot_layer_(asEBox(), gensym("plot_layer"))
    , border_layer_(asEBox(), gensym("border_layer"))
    , font_(gensym(FONT_FAMILY), FONT_SIZE_SMALL + 1)
    , running_(false)
    , plot_show_mask_(0xff)
{
    buffers_[0].resize(10);
}

bool UIPlotTilde::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 50);
    newrect->height = pd_clip_min(newrect->height, 50);
    return true;
}

void UIPlotTilde::paint()
{
    drawBorder();
    drawPlot();
}

class Fsm {
public:
    enum PenPosition {
        PEN_INIT,
        PEN_IN,
        PEN_OUT,
        PEN_NONE
    };

    using PenTransitionFn = std::function<PenPosition(UIPainter& p, float x, float y)>;
    using PenTransitions = PenTransitionFn[4];

    Fsm()
        : pen_fsm_ {
            {
                // INIT
                nullptr,
                [](UIPainter& p, float x, float y) -> PenPosition { p.moveTo(x, y); return PEN_IN; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_OUT; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_NONE; },
            },
            {
                // IN
                nullptr,
                [](UIPainter& p, float x, float y) -> PenPosition { p.drawLineTo(x, y); return PEN_IN; },
                [](UIPainter& p, float x, float y) -> PenPosition { p.drawLineTo(x, y); return PEN_OUT; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_NONE; },
            },
            {
                // OUT
                nullptr,
                [this](UIPainter& p, float x, float y) -> PenPosition { p.moveTo(pt_.first, pt_.second); p.drawLineTo(x, y); return PEN_IN; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_OUT; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_NONE; },
            },
            {
                // NONE
                nullptr,
                [](UIPainter& p, float x, float y) -> PenPosition { p.moveTo(x, y); return PEN_IN; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_OUT; },
                [](UIPainter& p, float x, float y) -> PenPosition { return PEN_NONE; },
            }
        }
        , pen_position_(PEN_INIT)
    {
    }

    void push(PenPosition pos, UIPainter& p, float x, float y)
    {
        auto fn = pen_fsm_[pen_position_][pos];
        pen_position_ = fn(p, x, y);
        pt_ = std::make_pair(x, y);
    }

    PenPosition pos() const { return pen_position_; }

    void reset()
    {
        pen_position_ = PEN_INIT;
    }

private:
    PenTransitions pen_fsm_[MAX_INPUTS];
    PenPosition pen_position_;
    std::pair<float, float> pt_;
};

void UIPlotTilde::drawPlot()
{
    static Fsm fsm;

    const t_rect r = rect();
    UIPainter p = plot_layer_.painter(r);
    if (!p)
        return;

    const float xoff = std::max(MIN_XOFF, width() * OFF_K);
    const float yoff = std::max(MIN_YOFF, height() * OFF_K);
    const float wd = width() - (xoff + MIN_XOFF);
    const float ht = height() - 2 * yoff;

    p.setMatrix({ 1, 0, 0, 1, xoff, yoff });
    p.preAllocObjects(prop_nins_);

    const t_rgba* colors[] = { &plot_color0_, &plot_color1_, &plot_color2_, &plot_color3_ };

    for (size_t j = 0; j < prop_nins_; j++) {
        if (!(plot_show_mask_ & (1 << j)))
            continue;

        fsm.reset();

        p.setColor(*colors[j]);
        p.preAllocPoints(3 * buffers_[j].size());

        for (size_t i = 0; i < buffers_[j].size(); i++) {
            auto x = convert::lin2lin<float>(i, 0, buffers_[j].size() - 1, 0, wd);

            auto v = buffers_[j][i];

            if (std::isinf(v) || std::isnan(v)) {
                fsm.push(Fsm::PEN_NONE, p, x, 0);
            } else if (yauto_) {
                auto y = convert::lin2lin<float>(v, sig_min_, sig_max_, ht, 0);
                fsm.push(Fsm::PEN_IN, p, x, y);
            } else {
                auto y = convert::lin2lin<float>(v, ymin_, ymax_, ht, 0);
                if (v < ymin_ || v > ymax_)
                    fsm.push(Fsm::PEN_OUT, p, x, y);
                else
                    fsm.push(Fsm::PEN_IN, p, x, y);
            }
        }

        p.stroke();
    }
}

void UIPlotTilde::addXLabel(float v, float x, float y, etextjustify_flags align, etextanchor_flags anchor)
{
    txt_x_.push_back(UITextLayout(font_.font()));
    txt_x_.back().setJustify(align);
    txt_x_.back().setAnchor(anchor);
    txt_x_.back().set(to_label(v), x, y, 20, FONT_SIZE_SMALL);
}

void UIPlotTilde::addXLabelLn(int n, float x, float y, etextjustify_flags align, etextanchor_flags anchor)
{
    static const char* digits[] = {
        "\u2070", "\u00B9", "\u00B2", "\u00B3", "\u2074", "\u2075", "\u2076", "\u2077", "\u2078", "\u2079"
    };

    char buf[16] = "";
    if (n < 0)
        strcat(buf, "e\u207B"); // minus superscript
    else
        strcat(buf, "e");

    n = std::abs(n);
    for (int i = 30; n && i; --i, n /= 10)
        strcat(buf, digits[n % 10]);

    txt_x_.push_back(UITextLayout(font_.font()));
    txt_x_.back().setJustify(align);
    txt_x_.back().setAnchor(anchor);
    txt_x_.back().set(buf, x, y, 20, FONT_SIZE_SMALL);
}

void UIPlotTilde::addYLabel(float v, float x, float y, etextjustify_flags align, etextanchor_flags anchor)
{
    txt_y_.push_back(UITextLayout(font_.font()));
    txt_y_.back().setJustify(align);
    txt_y_.back().setAnchor(anchor);
    txt_y_.back().set(to_label(v), x - 2, y, 20, FONT_SIZE_SMALL);
}

void UIPlotTilde::resizeBuffers(size_t n)
{
    for (int i = 0; i < prop_nins_ && prop_nins_ <= buffers_.size(); i++)
        buffers_[i].resize(n);
}

void UIPlotTilde::drawBorder()
{
    const t_rect r = rect();

    UIPainter p = border_layer_.painter(r);
    if (!p)
        return;

    const float xoff = std::max(MIN_XOFF, width() * OFF_K);
    const float yoff = std::max(MIN_YOFF, height() * OFF_K);
    const float wd = width() - (xoff + MIN_XOFF);
    const float ht = height() - 2 * yoff;

    p.setMatrix({ 1, 0, 0, 1, xoff, yoff });
    p.setColor(prop_color_border);
    // draw plot frame
    p.drawRect(0, 0, wd, ht);
    p.stroke();

    const float YMIN = (!yauto_) ? ymin_ : sig_min_;
    const float YMAX = (!yauto_) ? ymax_ : sig_max_;

    if (YMIN != YMAX) {
        drawYRangeLabels(p, YMIN, YMAX, wd, ht);
        drawLinY(p, wd, ht);
    }

    if (xmax_ != xmin_) {
        if (xlabels_) {
            txt_x_.clear();
            addXLabel(xmin_, 3, ht + LABEL_YPAD, ETEXT_JLEFT, ETEXT_UP_LEFT);
            p.drawText(txt_x_.back());
            addXLabel(xmax_, wd, ht + LABEL_YPAD, ETEXT_JRIGHT, ETEXT_UP_RIGHT);
            p.drawText(txt_x_.back());
        }

        if (log_base_ == LB_NONE) {
            drawLinX(p, wd, ht);
        } else if (log_base_ == LB_2) {
            drawLog2X(p, wd, ht);
        } else if (log_base_ == LB_10) {
            drawLog10X(p, wd, ht);
        } else if (log_base_ == LB_E) {
            drawLnX(p, wd, ht);
        }
    }

    if (ht > N_BTNS * (BTN_SIZE + BTN_PAD))
        drawXCtrlButtons(p);

    if (wd > N_BTNS * (BTN_SIZE + BTN_PAD))
        drawYCtrlButtons(p);

    if (prop_nins_ > 1 && wd > (N_BTNS + prop_nins_) * (BTN_SIZE + BTN_PAD))
        drawInCtrlButtons(p);
}

void UIPlotTilde::drawLog2X(UIPainter& p, float wd, float ht)
{
    if (xmin_ <= 0 || xmax_ <= 0) {
        UI_ERR << "positive x-range expected: " << xmin_ << " " << xmax_;
        return;
    }

    const auto lin_a = std::log2(xmin_);
    const auto lin_b = std::log2(xmax_);
    const int lin_ai = std::floor(lin_a);
    const int lin_bi = std::ceil(lin_b);

    for (int i = lin_ai; i <= lin_bi; i++) {
        for (int j = 0; j < 4; j++) {
            auto log_maj = fast_pow2(i);
            auto log_min = j * (log_maj / 4);
            auto lin_v = std::log2(log_maj + log_min);
            const auto x = convert::lin2lin<float>(lin_v, lin_a, lin_b, 0, wd);

            if (x <= 0)
                continue;
            if (x >= wd)
                break;

            const bool is_maj = (j == 0);
            const bool is_min = !is_maj;

            // draw ticks
            if (is_maj && xmaj_ticks_)
                p.drawLine(x, ht, x, ht + XTICK_MAJ);
            else if (is_min && xmin_ticks_)
                p.drawLine(x, ht, x, ht + XTICK_MIN);

            // draw grid
            if (is_maj && xmaj_grid_)
                p.drawLine(x, 0, x, ht);
            else if (is_min && xmin_grid_)
                p.drawLine(x, 0, x, ht);

            // too close to the border
            if (x < XGRID_AVOID || x > (wd - XGRID_AVOID))
                continue;

            // draw labels
            if (is_maj && xlabels_) {
                addXLabel(log_maj, x + LABEL_XPAD, ht + LABEL_YPAD, ETEXT_JLEFT, ETEXT_UP_LEFT);
                p.drawText(txt_x_.back());
            }
        }
    }
}

void UIPlotTilde::drawLog10X(UIPainter& p, float wd, float ht)
{
    if (xmin_ <= 0 || xmax_ <= 0) {
        UI_ERR << "positive x-range expected: " << xmin_ << " " << xmax_;
        return;
    }

    const auto lin_a = std::log10(xmin_);
    const auto lin_b = std::log10(xmax_);
    const int lin_ai = std::floor(lin_a);
    const int lin_bi = std::ceil(lin_b);

    for (int i = lin_ai; i <= lin_bi; i++) {
        for (int j = 0; j < 10; j++) {
            auto log_maj = fast_pow10(i);
            auto lin_v = fast_log10(1 + j) + i;
            const auto x = convert::lin2lin<float>(lin_v, lin_a, lin_b, 0, wd);

            if (x <= 0)
                continue;
            if (x >= wd)
                break;

            const bool is_maj = (j == 0);
            const bool is_min = !is_maj;

            // draw ticks
            if (is_maj && xmaj_ticks_)
                p.drawLine(x, ht, x, ht + XTICK_MAJ);
            else if (is_min && xmin_ticks_)
                p.drawLine(x, ht, x, ht + XTICK_MIN);

            // draw grid
            if (is_maj && xmaj_grid_)
                p.drawLine(x, 0, x, ht);
            else if (is_min && xmin_grid_)
                p.drawLine(x, 0, x, ht);

            // too close to the border
            if (x < XGRID_AVOID || x > (wd - XGRID_AVOID))
                continue;

            // draw labels
            if (is_maj && xlabels_) {
                addXLabel(log_maj, x + LABEL_XPAD, ht + LABEL_YPAD, ETEXT_JLEFT, ETEXT_UP_LEFT);
                p.drawText(txt_x_.back());
            }
        }
    }
}

void UIPlotTilde::drawLnX(UIPainter& p, float wd, float ht)
{
    if (xmin_ <= 0 || xmax_ <= 0) {
        UI_ERR << "positive x-range expected: " << xmin_ << " " << xmax_;
        return;
    }

    const auto lin_a = std::log(xmin_);
    const auto lin_b = std::log(xmax_);
    const int lin_ai = std::floor(lin_a);
    const int lin_bi = std::ceil(lin_b);

    for (int i = lin_ai; i <= lin_bi; i++) {
        auto log_v = std::exp(i);
        if (log_v <= xmin_)
            continue;
        if (log_v >= xmax_)
            break;

        const auto x = convert::lin2lin<float>(i, lin_a, lin_b, 0, wd);

        // draw ticks
        if (xmaj_ticks_)
            p.drawLine(x, ht, x, ht + XTICK_MAJ);

        // draw grid
        if (xmaj_grid_)
            p.drawLine(x, 0, x, ht);

        // too close to the border
        if (x < XGRID_AVOID || x > (wd - XGRID_AVOID))
            continue;

        // draw labels
        if (xlabels_) {
            addXLabelLn(i, x + LABEL_XPAD, ht + LABEL_YPAD, ETEXT_JLEFT, ETEXT_UP_LEFT);
            p.drawText(txt_x_.back());
        }
    }
}

void UIPlotTilde::drawLinX(UIPainter& p, float wd, float ht)
{
    const auto tick_step = std::pow(10, std::trunc(std::log10(std::fabs(xmax_ - xmin_)) + 0.3) - 1);
    const int xtick_min = std::ceil(xmin_ / tick_step);
    const int xtick_max = std::floor(xmax_ / tick_step);

    for (int i = xtick_min; i <= xtick_max; i++) {
        auto x = convert::lin2lin<float>(i * tick_step, xmin_, xmax_, 0, wd);

        const bool is_maj = (i % 10 == 0);
        const bool is_min = !is_maj;

        // draw ticks
        if (is_maj && xmaj_ticks_)
            p.drawLine(x, ht, x, ht + XTICK_MAJ);
        else if (is_min && xmin_ticks_)
            p.drawLine(x, ht, x, ht + XTICK_MIN);

        if (is_maj && xmaj_grid_)
            p.drawLine(x, 0, x, ht);
        else if (is_min && xmin_grid_)
            p.drawLine(x, 0, x, ht);

        // too close to the border
        if (x < XGRID_AVOID || x > (wd - XGRID_AVOID))
            continue;

        // draw labels
        if (is_maj && xlabels_) {
            addXLabel(i * tick_step, x + LABEL_XPAD, ht + LABEL_YPAD, ETEXT_JLEFT, ETEXT_UP_LEFT);
            p.drawText(txt_x_.back());
        }
    }
}

void UIPlotTilde::drawLinY(UIPainter& p, float wd, float ht)
{
    const float YMIN = (!yauto_) ? ymin_ : sig_min_;
    const float YMAX = (!yauto_) ? ymax_ : sig_max_;

    const auto tick_step = std::pow(10, std::floor(std::log10(std::fabs(YMAX - YMIN)) - 0.05) - 1);
    const auto ytick_base = int(std::trunc(YMIN / tick_step));
    const auto ytick_min = std::trunc(YMIN / tick_step) * tick_step;
    const auto ytick_max = std::trunc(YMAX / tick_step) * tick_step;

    const size_t N = std::fabs(ytick_max - ytick_min) / tick_step;
    p.setColor(prop_color_border);

    const size_t NMAJ_LINES = N / 10 * (ymaj_grid_ + ymaj_ticks_);
    const size_t NMIN_LINES = (0.9 * N) * (ymin_grid_ + ymin_ticks_);

    p.preAllocPoints(4 * (NMAJ_LINES + NMIN_LINES));

    for (size_t i = 1; i <= N; i++) {
        auto v = ytick_min + i * tick_step;
        auto y = convert::lin2lin<float>(v, YMIN, YMAX, ht, 0); // in pixels

        const auto i2 = ytick_base + int(i);
        const bool is_maj = (i2 % 10 == 0);
        const bool is_min = !is_maj;

        // ticks
        if (is_maj && ymaj_ticks_) {
            p.moveTo(-XTICK_MAJ, y);
            p.drawLineTo(0, y);
        } else if (is_min && ymin_ticks_) {
            p.moveTo(-XTICK_MIN, y);
            p.drawLineTo(0, y);
        }

        // grid
        if (is_maj && ymaj_grid_) {
            p.moveTo(0, y);
            p.drawLineTo(wd, y);
        } else if (is_min && ymin_grid_) {
            p.moveTo(0, y);
            p.drawLineTo(wd, y);
        }
    }

    p.stroke();

    if (ylabels_) {
        p.preAllocObjects(N / 10);

        const auto ts10 = 10 * tick_step;
        for (size_t i = 0; i <= (N / 10); i++) {
            auto v = std::trunc(YMIN / ts10) * ts10 + i * ts10;
            auto y = convert::lin2lin<float>(v, YMIN, YMAX, ht, 0); // in pixels

            // too close to the border
            if (y < XGRID_AVOID || y > (ht - XGRID_AVOID))
                continue;

            addYLabel(v, 0, y, ETEXT_JLEFT, ETEXT_DOWN_RIGHT);
            p.drawText(txt_y_.back());
        }
    }
}

void UIPlotTilde::drawYRangeLabels(UIPainter& p, float from, float to, float wd, float ht)
{
    if (!ylabels_)
        return;

    txt_y_.clear();

    if (strlen(to_label(to)) > 4) // long label
        addYLabel(to, -MIN_XOFF + 4, 0, ETEXT_JLEFT, ETEXT_UP_LEFT);
    else
        addYLabel(to, 0, 0, ETEXT_JRIGHT, ETEXT_UP_RIGHT);

    p.drawText(txt_y_.back());

    if (strlen(to_label(from)) > 4) // long label
        addYLabel(from, -MIN_XOFF + 4, ht, ETEXT_JLEFT, ETEXT_DOWN_LEFT);
    else
        addYLabel(from, 0, ht, ETEXT_JRIGHT, ETEXT_DOWN_RIGHT);

    p.drawText(txt_y_.back());
}

t_rect UIPlotTilde::calcXButton(int n, bool real) const
{
    if (n < 0 || n > N_BTNS)
        return { 0 };

    const float xoff = std::max(MIN_XOFF, width() * OFF_K);
    const float yoff = std::max(MIN_YOFF, height() * OFF_K);
    const float wd = width() - (xoff + MIN_XOFF);
    const float ht = height() - 2 * yoff;

    // location - right bottom plot side
    float x = wd + BTN_PAD;
    float y = ht;
    auto yy = y - (n + 1) * (BTN_SIZE + BTN_PAD);

    if (real) {
        x += xoff;
        yy += yoff;
    }

    return { x, yy, BTN_SIZE, BTN_SIZE };
}

t_rect UIPlotTilde::calcYButton(int n, bool real) const
{
    if (n < 0 || n > N_BTNS)
        return { 0 };

    const float xoff = std::max(MIN_XOFF, width() * OFF_K);
    const float yoff = std::max(MIN_YOFF, height() * OFF_K);

    float x = n * (BTN_SIZE + BTN_PAD);
    float y = -(BTN_SIZE + BTN_PAD);

    if (real) {
        x += xoff;
        y += yoff;
    }

    return { x, y, BTN_SIZE, BTN_SIZE };
}

t_rect UIPlotTilde::calcInButton(int n, bool real) const
{
    if (n < 0 || n > N_BTNS)
        return { 0 };

    const float xoff = std::max(MIN_XOFF, width() * OFF_K);
    const float yoff = std::max(MIN_YOFF, height() * OFF_K);
    const float wd = width() - (xoff + MIN_XOFF);

    float x = wd - (n + 1) * (BTN_SIZE + BTN_PAD);
    float y = -(BTN_SIZE + BTN_PAD);

    if (real) {
        x += xoff;
        y += yoff;
    }

    return { x, y, BTN_SIZE, BTN_SIZE };
}

void UIPlotTilde::drawXCtrlButtons(UIPainter& p)
{
    const int props[] = { xmin_ticks_ || xmaj_ticks_, xmin_grid_, xmaj_grid_, xlabels_ };
    constexpr size_t N = sizeof(props) / sizeof(props[0]);
    static_assert(N == N_BTNS, "size mismatch");

    // 4 rects and 4 labels
    p.preAllocObjects(N_BTNS * 2);

    for (int i = 0; i < N_BTNS; i++) {
        const t_rect r = calcXButton(i, false);
        p.drawRect(r.x, r.y, r.width, r.height);
        if (props[i]) {
            p.setColor(prop_color_border);
            p.fillPreserve();
        }

        p.setColor(rgba_black);
        p.stroke();

        txt_x_.push_back(UITextLayout(font_.font()));
        txt_x_.back().setJustify(ETEXT_JCENTER);
        txt_x_.back().setAnchor(ETEXT_CENTER);

        auto xc = r.x + r.width / 2 + 1; // button x-center
        auto yc = r.y + r.height / 2 + 1; // button y-center
        txt_x_.back().set(BTN_LABELS[i], xc, yc, r.width, FONT_SIZE_SMALL);
        p.drawText(txt_x_.back());
    }
}

void UIPlotTilde::drawYCtrlButtons(UIPainter& p)
{
    const int props[] = { ymin_ticks_ || ymaj_ticks_, ymin_grid_, ymaj_grid_, ylabels_ };
    constexpr size_t N = sizeof(props) / sizeof(props[0]);
    static_assert(N == N_BTNS, "size mismatch");

    // 4 rects and 4 labels
    p.preAllocObjects(N_BTNS * 2);

    for (int i = 0; i < N_BTNS; i++) {
        int btn_idx = N_BTNS - (i + 1);
        const t_rect r = calcYButton(i, false);
        p.drawRect(r.x, r.y, r.width, r.height);
        if (props[btn_idx]) {
            p.setColor(prop_color_border);
            p.fillPreserve();
        }

        p.setColor(rgba_black);
        p.stroke();

        txt_y_.push_back(UITextLayout(font_.font()));
        txt_y_.back().setJustify(ETEXT_JCENTER);
        txt_y_.back().setAnchor(ETEXT_CENTER);

        auto xc = r.x + r.width / 2 + 1; // button x-center
        auto yc = r.y + r.height / 2 + 1; // button y-center
        txt_y_.back().set(BTN_LABELS[btn_idx], xc, yc, r.width, FONT_SIZE_SMALL);
        p.drawText(txt_y_.back());
    }
}

void UIPlotTilde::drawInCtrlButtons(UIPainter& p)
{
    const char* txt[] = { "1", "2", "3", "4" };
    const t_rgba* colors[] = { &plot_color0_, &plot_color1_, &plot_color2_, &plot_color3_ };

    p.preAllocObjects(prop_nins_ * 2);

    for (int i = 0; i < prop_nins_; i++) {
        const int btn_idx = prop_nins_ - (i + 1);
        const t_rect r = calcInButton(btn_idx, false);
        p.drawRect(r.x, r.y, r.width, r.height);
        if (plot_show_mask_ & (1 << i)) {
            p.setColor(*colors[i]);
            p.fillPreserve();
        }

        p.setColor(rgba_black);
        p.stroke();

        txt_y_.push_back(UITextLayout(font_.font()));
        txt_y_.back().setJustify(ETEXT_JCENTER);
        txt_y_.back().setAnchor(ETEXT_CENTER);

        auto xc = r.x + r.width / 2 + 1; // button x-center
        auto yc = r.y + r.height / 2 + 1; // button y-center
        txt_y_.back().set(txt[i], xc, yc, r.width, FONT_SIZE_SMALL);
        p.drawText(txt_y_.back());
    }
}

void UIPlotTilde::onPropChange(t_symbol* prop_name)
{
    plot_layer_.invalidate();
    border_layer_.invalidate();
    redraw();
}

void UIPlotTilde::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIDspObject::init(name, args, false);
    dspSetup(1, 0);

    sig_min_ = ymin_;
    sig_max_ = ymax_;
}

void UIPlotTilde::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    if (!running_)
        return;

    if (phase_ == 0 && yauto_) {
        t_sample v0 = ins[0][0];
        if (!std::isnan(v0) && !std::isinf(v0)) {
            sig_min_ = ins[0][0];
            sig_max_ = sig_min_;
        } else {
            sig_max_ = 0;
            sig_min_ = 0;
        }
    }

    auto bphase = phase_;

    for (long j = 0; j < n_ins && j < prop_nins_; j++) {
        if (buffers_[j].size() < total_)
            buffers_[j].resize(total_);

        phase_ = bphase;

        for (long i = 0; i < sampleframes; i++) {
            if (phase_ < total_) {
                t_sample v = ins[j][i];

                if (yauto_) {
                    if (!std::isnan(v) && !std::isinf(v)) {
                        sig_min_ = std::min(sig_min_, v);
                        sig_max_ = std::max(sig_max_, v);
                    }
                }

                buffers_[j][phase_++] = v;

            } else {
                running_ = false;
                if (total_ != 0)
                    clock_.delay(0);

                break;
            }
        }
    }

    if (!yauto_) {
        sig_max_ = ymax_;
        sig_min_ = ymin_;
    }
}

void UIPlotTilde::onInlet(const AtomList& args)
{
    constexpr int NMAX = 1024;

    if (args.empty()) {
        running_ = true;
        phase_ = 0;
        total_ = width() * 0.9;
        xmin_ = 0;
        xmax_ = total_;
        resizeBuffers(total_);
        return;
    } else {
        if (!args[0].isInteger()) {
            UI_ERR << "invalid args: " << args;
            return;
        }

        auto N = args[0].toT<int>(0);
        if (N > NMAX) {
            running_ = false;
            UI_ERR << "requested plot size is too big: " << N << ", max size is " << NMAX;
            return;
        } else if (N > 0) {
            running_ = true;
            phase_ = 0;
            total_ = N;
            resizeBuffers(total_);

            xmin_ = args.floatAt(1, 0);
            xmax_ = args.floatAt(2, total_);

            auto sym_lb = args.symbolAt(3, &s_);

            if (sym_lb == gensym("log10"))
                log_base_ = LB_10;
            else if (sym_lb == gensym("log2"))
                log_base_ = LB_2;
            else if (sym_lb == gensym("ln"))
                log_base_ = LB_E;
            else
                log_base_ = LB_NONE;

            UI_DBG << "logbase: " << sym_lb << "; lb: " << log_base_;

        } else {
            running_ = false;
            phase_ = 0;
            total_ = 0;
        }
    }
}

static bool inRect(const t_pt& p, const t_rect& r)
{
    if (p.x < r.x || p.x > (r.x + r.width))
        return false;
    else if (p.y < r.y || p.y > (r.y + r.height))
        return false;
    else
        return true;
}

void UIPlotTilde::onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    for (int i = 0; i < 4; i++) {
        if (inRect(pt, calcXButton(i, true))) {
            switch (i) {
            case 0:
                xmaj_ticks_ ^= 0x1;
                xmin_ticks_ = xmaj_ticks_;
                border_layer_.invalidate();
                redraw();
                break;
            case 2:
                xmaj_grid_ ^= 0x1;
                border_layer_.invalidate();
                redraw();
                break;
            case 1:
                xmin_grid_ ^= 0x1;
                border_layer_.invalidate();
                redraw();
                break;
            case 3:
                xlabels_ ^= 0x1;
                border_layer_.invalidate();
                redraw();
                break;
            }
            return;
        }

        if (inRect(pt, calcYButton(i, true))) {
            switch (i) {
            case 3:
                ymaj_ticks_ ^= 0x1;
                ymin_ticks_ = ymaj_ticks_;
                border_layer_.invalidate();
                redraw();
                break;
            case 1:
                ymaj_grid_ ^= 0x1;
                border_layer_.invalidate();
                redraw();
                break;
            case 2:
                ymin_grid_ ^= 0x1;
                border_layer_.invalidate();
                redraw();
                break;
            case 0:
                ylabels_ ^= 0x1;
                border_layer_.invalidate();
                redraw();
                break;
            }
            return;
        }
    }

    for (int i = 0; i < prop_nins_; i++) {
        if (inRect(pt, calcInButton(prop_nins_ - (i + 1), true))) {
            plot_show_mask_ ^= (0x1 << i);
            border_layer_.invalidate();
            plot_layer_.invalidate();
            redraw();
            return;
        }
    }
}

float UIPlotTilde::propNumInputs() const
{
    return prop_nins_;
}

void UIPlotTilde::propSetNumInputs(float n)
{
    prop_nins_ = clip<int, MIN_INPUTS, MAX_INPUTS>(n);
    int dspState = canvas_suspend_dsp();

    eobj_resize_inputs(asEBox(), 0);
    eobj_resize_inputs(asEBox(), prop_nins_);
    eobj_resize_inputs(asEBox(), n + 1, &s_list, gensym("_inlet_2"));

    canvas_update_dsp();
    canvas_resume_dsp(dspState);
}

void UIPlotTilde::setup()
{
    UIObjectFactory<UIPlotTilde> obj("ui.plot~");
    obj.hideLabelInner();

    obj.setDefaultSize(200, 200);
    obj.useBang();
    obj.useMouseEvents(UI_MOUSE_DOWN);

    obj.addBoolProperty(SYM_YAUTO->s_name, _("Auto Y-range"), 0, &UIPlotTilde::yauto_, _("Bounds"));
    obj.addFloatProperty(SYM_YMIN->s_name, _("Minimum Y-value"), -1, &UIPlotTilde::ymin_, _("Bounds"));
    obj.addFloatProperty(SYM_YMAX->s_name, _("Maximum Y-value"), 1, &UIPlotTilde::ymax_, _("Bounds"));
    obj.addBoolProperty("xmaj_ticks", _("Major ticks on x-axis"), 1, &UIPlotTilde::xmaj_ticks_, _("Bounds"));
    obj.addBoolProperty("xmin_ticks", _("Minor ticks on x-axis"), 1, &UIPlotTilde::xmin_ticks_, _("Bounds"));
    obj.addBoolProperty("xmaj_grid", _("Major grid on x-axis"), 1, &UIPlotTilde::xmaj_grid_, _("Bounds"));
    obj.addBoolProperty("xmin_grid", _("Minor grid on x-axis"), 0, &UIPlotTilde::xmin_grid_, _("Bounds"));
    obj.addBoolProperty("xlabels", _("Labels on x-axis"), 0, &UIPlotTilde::xlabels_, _("Bounds"));
    obj.addBoolProperty("ymaj_ticks", _("Major ticks on y-axis"), 1, &UIPlotTilde::ymaj_ticks_, _("Bounds"));
    obj.addBoolProperty("ymin_ticks", _("Minor ticks on y-axis"), 1, &UIPlotTilde::ymin_ticks_, _("Bounds"));
    obj.addBoolProperty("ymaj_grid", _("Major grid on y-axis"), 1, &UIPlotTilde::ymaj_grid_, _("Bounds"));
    obj.addBoolProperty("ymin_grid", _("Minor grid on y-axis"), 0, &UIPlotTilde::ymin_grid_, _("Bounds"));
    obj.addBoolProperty("ylabels", _("Labels on y-axis"), 0, &UIPlotTilde::ylabels_, _("Bounds"));

    obj.addIntProperty("n", _("Number of inputs"), 1, &UIPlotTilde::prop_nins_, "Main");
    obj.setPropertyAccessor("n", &UIPlotTilde::propNumInputs, &UIPlotTilde::propSetNumInputs);
    obj.setPropertyRange("n", MIN_INPUTS, MAX_INPUTS);

    obj.addColorProperty("plot_color0", _("First plot color"), DEFAULT_ACTIVE_COLOR, &UIPlotTilde::plot_color0_);
    obj.addColorProperty("plot_color1", _("Second plot color"), "0.75 0 1 1", &UIPlotTilde::plot_color1_);
    obj.addColorProperty("plot_color2", _("Third plot color"), "1 0 0.75 1", &UIPlotTilde::plot_color2_);
    obj.addColorProperty("plot_color3", _("Fourth plot color"), "1 0.75 0 1", &UIPlotTilde::plot_color3_);

    obj.addMethod("_inlet_2", &UIPlotTilde::onInlet);
    obj.addMethod("plot", &UIPlotTilde::onInlet);
}

void setup_ui_plot_tilde()
{
    SYM_YAUTO = gensym("yauto");
    SYM_YMIN = gensym("ymin");
    SYM_YMAX = gensym("ymax");

    UIPlotTilde::setup();
}
