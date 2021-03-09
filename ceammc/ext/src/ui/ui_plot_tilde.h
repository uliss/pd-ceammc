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
#ifndef UI_PLOT_TILDE_H
#define UI_PLOT_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

#include <array>
#include <cstdint>
#include <vector>

using namespace ceammc;

enum ScaleBase {
    SB_LIN10,
    SB_LINPI,
    SB_LOG2,
    SB_LN,
    SB_LOG10
};

enum DrawMode {
    DM_LINE,
    DOT,
    BAR
};

struct TickXPoint {
    float v;
    float x;
    bool is_maj;
};

using TextList = std::vector<UITextLayout>;
using XTicks = std::vector<TickXPoint>;

class UIPlotTilde : public UIDspObject {
    using SampleBuffer = std::vector<t_sample>;
    using SampleBufferList = std::array<SampleBuffer, 4>;

    SampleBufferList buffers_;
    XTicks xticks_;
    ClockLambdaFunction clock_;
    t_inlet* in2_;
    size_t phase_;
    size_t total_;
    t_float ymin_, ymax_;
    t_float xmin_, xmax_;
    t_sample sig_min_, sig_max_;
    int yauto_;
    int xmaj_ticks_, xmin_ticks_;
    int xmaj_grid_, xmin_grid_;
    int ymaj_ticks_, ymin_ticks_;
    int ymaj_grid_, ymin_grid_;
    int xlabels_, ylabels_;
    int prop_nins_;
    t_symbol* prop_draw_mode_;
    t_rgba plot_color0_;
    t_rgba plot_color1_;
    t_rgba plot_color2_;
    t_rgba plot_color3_;
    ScaleBase xscale_base_;
    UILayer plot_layer_;
    UILayer border_layer_;
    UIFont font_;
    TextList txt_y_;
    TextList txt_x_;
    bool running_;
    uint8_t plot_show_mask_;

public:
    UIPlotTilde();

    bool okSize(t_rect* newrect);
    void paint();
    void drawPlot();
    void drawBorder();
    void drawLog2X(UIPainter& p, float wd, float ht);
    void drawLog10X(UIPainter& p, float wd, float ht);
    void drawLnX(UIPainter& p, float wd, float ht);
    void drawLinX(UIPainter& p, float wd, float ht);
    void drawLinY(UIPainter& p, float wd, float ht);
    void drawYRangeLabels(UIPainter& p, float from, float to, float wd, float ht);
    void drawXCtrlButtons(UIPainter& p);
    void drawYCtrlButtons(UIPainter& p);
    void drawInCtrlButtons(UIPainter& p);

    void onPropChange(t_symbol* prop_name);

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

    void onInlet(const AtomListView& args);
    void onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long modifiers);

    t_float propNumInputs() const;
    void propSetNumInputs(t_float n);

public:
    static void setup();

private:
    void addXLabel(const char* txt, float x, float y, etextjustify_flags align, etextanchor_flags anchor);
    void addYLabel(float v, float x, float y, etextjustify_flags align, etextanchor_flags anchor);

    void resizeBuffers(size_t n);
    t_rect calcXButton(int n, bool real) const;
    t_rect calcYButton(int n, bool real) const;
    t_rect calcInButton(int n, bool real) const;

    void drawXMajTicks(UIPainter& p, float ht);
    void drawXMinTicks(UIPainter& p, float ht);
    void drawXMajGrid(UIPainter& p, float ht);
    void drawXMinGrid(UIPainter& p, float ht);
    void drawXLabels(UIPainter& p, float wd, float ht);
};

void setup_ui_plot_tilde();

#endif // UI_PLOT_TILDE_H
