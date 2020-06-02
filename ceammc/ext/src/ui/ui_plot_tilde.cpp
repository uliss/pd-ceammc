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

static double fast_pow10(int n)
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
    , sig_min_(0)
    , sig_max_(0)
    , xmin_(0)
    , xmax_(256)
    , yauto_(1)
    , xlog_(1)
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
    PenTransitions pen_fsm_[4];
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

    const t_rgba* colors[] = { &plot_color0_, &plot_color1_, &plot_color2_, &plot_color3_ };

    for (size_t j = 0; j < prop_nins_; j++) {
        fsm.reset();

        p.setColor(*colors[j]);

        for (size_t i = 0; i < buffers_[j].size(); i++) {
            auto x = convert::lin2lin<float>(i, 0, buffers_[j].size() - 1, 0, wd);

            auto v = buffers_[j][i];
            if (!std::isnormal(v))
                v = 0;

            if (std::isinf(v) || std::isnan(v))
                fsm.push(Fsm::PEN_NONE, p, x, 0);
            else if (yauto_) {
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
    char buf[16];
    if (v == int(v))
        snprintf(buf, 16, "%d", int(v));
    else
        snprintf(buf, 16, "%g", v);

    txt_x_.push_back(UITextLayout(font_.font()));
    txt_x_.back().setJustify(align);
    txt_x_.back().setAnchor(anchor);
    txt_x_.back().set(buf, x, y, 20, FONT_SIZE_SMALL);
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

    //    snprintf(buf, 16, "e^%d", n);

    txt_x_.push_back(UITextLayout(font_.font()));
    txt_x_.back().setJustify(align);
    txt_x_.back().setAnchor(anchor);
    txt_x_.back().set(buf, x, y, 20, FONT_SIZE_SMALL);
}

void UIPlotTilde::addYLabel(float v, float x, float y, etextanchor_flags anchor)
{
    char buf[16];
    if (v == int(v))
        snprintf(buf, 16, "%d", int(v));
    else
        snprintf(buf, 16, "%g", v);

    txt_y_.push_back(UITextLayout(font_.font()));
    txt_y_.back().setJustify(ETEXT_JRIGHT);
    txt_y_.back().setAnchor(anchor);
    txt_y_.back().set(buf, x - 2, y, 20, FONT_SIZE_SMALL);
}

void UIPlotTilde::resizeBuffers(size_t n)
{
    for (int i = 0; i < prop_nins_ && prop_nins_ < buffers_.size(); i++)
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
    p.drawRect(xoff, yoff, wd, ht);
    p.stroke();

    const float YMIN = (!yauto_) ? ymin_ : sig_min_;
    const float YMAX = (!yauto_) ? ymax_ : sig_max_;

    if (YMIN != YMAX) {
        const auto tick_step = std::pow(10, std::round(std::log10(std::fabs(YMAX - YMIN)) - 0.5) - 1);
        const auto ytick_base = int(std::trunc(YMIN / tick_step));
        const auto ytick_min = std::trunc(YMIN / tick_step) * tick_step;
        const auto ytick_max = std::trunc(YMAX / tick_step) * tick_step;

        const size_t N = std::fabs(ytick_max - ytick_min) / tick_step;

        txt_y_.clear();
        addYLabel(YMAX, 0, 0, ETEXT_UP_RIGHT);
        p.drawText(txt_y_.back());
        addYLabel(YMIN, 0, ht, ETEXT_DOWN_RIGHT);
        p.drawText(txt_y_.back());

        for (size_t i = 1; i < N; i++) {
            auto v = ytick_min + i * tick_step;
            auto y = convert::lin2lin<float>(v, YMIN, YMAX, ht, 0); // in pixels

            const auto i2 = ytick_base + int(i);

            if (i2 % 10 == 0 && ymaj_ticks_) {
                p.setLineWidth(1);
                p.drawLine(-6, y, 0, y);
            } else if (ymin_ticks_) {
                p.setLineWidth(1);
                p.drawLine(-3, y, 0, y);
            }

            if (i2 % 5 == 0 && ymaj_grid_) {
                // zero
                p.setLineWidth((v == 0) ? 2 : 1);
                p.drawLine(0, y, wd, y);
            }

            if (i2 % 10 == 0 && ylabels_) {
                addYLabel(v, 0, y, ETEXT_DOWN_RIGHT);
                p.drawText(txt_y_.back());
            }
        }
    }

    if (xmax_ != xmin_) {
        txt_x_.clear();
        addXLabel(xmin_, 3, ht + LABEL_YPAD, ETEXT_JLEFT, ETEXT_UP_LEFT);
        p.drawText(txt_x_.back());
        addXLabel(xmax_, wd, ht + LABEL_YPAD, ETEXT_JRIGHT, ETEXT_UP_RIGHT);
        p.drawText(txt_x_.back());

        if (log_base_ == LB_NONE) {
            const auto tick_step = std::pow(10, std::round(std::log10(std::fabs(xmax_ - xmin_)) - 0.5) - 1);
            const auto xtick_base = int(std::trunc(xmin_ / tick_step));
            const auto xtick_min = std::trunc(xmin_ / tick_step) * tick_step;
            const auto xtick_max = std::trunc(xmax_ / tick_step) * tick_step;

            const size_t N = std::fabs(xtick_max - xtick_min) / tick_step;
            for (size_t i = 0; i < N; i++) {
                auto v = xtick_min + i * tick_step;
                auto x = convert::lin2lin<float>(v, xmin_, xmax_, 0, wd);

                const auto i2 = xtick_base + int(i);
                const float tick_h = (i2 % 10 != 0) ? 3 : 6;
                p.setLineWidth(1);
                p.drawLine(x, ht, x, ht + tick_h);

                if (i2 % 5 == 0) {
                    p.setLineWidth((v == 0) ? 2 : 1);
                    p.drawLine(x, 0, x, ht);
                }
            }
        } else if (log_base_ == LB_2) {
            drawLog2X(p, wd, ht);
        } else if (log_base_ == LB_10) {
            drawLog10X(p, wd, ht);
        } else if (log_base_ == LB_E) {
            drawLnX(p, wd, ht);
        }
    }
}

void UIPlotTilde::drawLog2X(UIPainter& p, float wd, float ht)
{
    if (xmin_ <= 0 || xmax_ <= 0) {
        UI_ERR << "positive x-range expected: " << xmin_ << " " << xmax_;
        return;
    }

    const auto lin_a = std::log2(xmin_);
    const auto lin_b = std::log2(xmax_);

    const size_t nbig_ticks = std::floor(lin_b) - std::floor(lin_a);
    size_t N = nbig_ticks;
    if (nbig_ticks == 0)
        N = std::floor(std::log2(xmax_ - xmin_));

    for (size_t i = 0; i <= N; i++) {
        for (size_t j = 0; j < 4; j++) {
            auto log_maj = std::pow(2, i);
            auto log_min = j * (log_maj / 4);
            auto lin_v = std::log2(log_maj + log_min);
            const auto x = convert::lin2lin<float>(lin_v, lin_a, lin_b, 0, wd);

            if (x > wd)
                break;

            // draw ticks
            const bool is_maj = (lin_v == int(lin_v));
            const bool is_min = !is_maj;

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
}

void UIPlotTilde::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    if (!running_)
        return;

    if (phase_ == 0 && yauto_) {
        t_sample v0 = ins[0][0];
        if (!std::isnan(v0) && !std::isinf(v0)) {
            sig_min_ = sig_min_ = ins[0][0];
            sig_max_ = sig_max_ = sig_min_;
        } else {
            sig_max_ = 0;
            sig_min_ = 0;
        }
    }

    auto bphase = phase_;

    for (long j = 0; j < n_ins && j < prop_nins_; j++) {
        //        std::cerr << "j:" << j << "\n";
        if (buffers_[j].size() < total_)
            buffers_[j].resize(total_);

        phase_ = bphase;

        for (long i = 0; i < sampleframes; i++) {
            if (phase_ < total_) {
                t_sample v = ins[j][i];

                //                std::cerr << "v[" << j << "]=" << v << "\n";

                if (yauto_) {
                    if (!std::isnan(v) && !std::isinf(v)) {
                        sig_min_ = std::min(sig_min_, v);
                        sig_max_ = std::max(sig_max_, v);
                    }
                } else {
                    //                if (!std::isnan(v) && !std::isinf(v))
                    //                    v = clip(v, ymin_, ymax_);
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
            //            UI_ERR << fmt::format("requested plot size is too big: {}, max size is {}", N, NMAX);
            return;
        } else if (N > 0) {
            running_ = true;
            phase_ = 0;
            total_ = N;

            if (buffers_.size() != total_) {
                resizeBuffers(total_);
                UI_DBG << "setting number of points: " << total_;
            }

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

float UIPlotTilde::propNumInputs() const
{
    return prop_nins_;
}

void UIPlotTilde::propSetNumInputs(float n)
{
    prop_nins_ = clip<int, 1, 4>(n);
    int dspState = canvas_suspend_dsp();

    eobj_resize_inputs(asEBox(), 0);
    eobj_resize_inputs(asEBox(), prop_nins_);
    eobj_resize_inputs(asEBox(), n + 1, &s_list, gensym("_inlet_2"));

    canvas_update_dsp();
    canvas_resume_dsp(dspState);
}

void UIPlotTilde::setup()
{
    UIObjectFactory<UIPlotTilde> obj("ui.plot~", EBOX_GROWLINK);
    obj.hideLabelInner();

    obj.setDefaultSize(200, 200);
    obj.useBang();

    obj.addBoolProperty(SYM_YAUTO->s_name, _("Auto Y-range"), 0, &UIPlotTilde::yauto_, _("Bounds"));
    obj.addFloatProperty(SYM_YMIN->s_name, _("Minimum Y-value"), -1, &UIPlotTilde::ymin_, _("Bounds"));
    obj.addFloatProperty(SYM_YMAX->s_name, _("Maximum Y-value"), 1, &UIPlotTilde::ymax_, _("Bounds"));
    obj.addBoolProperty("xlog", _("Log scaled x-axis"), 0, &UIPlotTilde::xlog_, _("Bounds"));
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
    obj.setPropertyRange("n", 1, 4);

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
