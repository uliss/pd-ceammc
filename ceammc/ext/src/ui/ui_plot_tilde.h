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

#include <vector>

using namespace ceammc;

enum LogBase {
    LB_NONE,
    LB_2,
    LB_E,
    LB_10
};

using TextList = std::vector<UITextLayout>;

class UIPlotTilde : public UIDspObject {
    std::vector<t_sample> buf_;
    ClockLambdaFunction clock_;
    t_inlet* in2_;
    size_t phase_;
    size_t total_;
    float ymin_, ymax_;
    float xmin_, xmax_;
    t_sample sig_min_, sig_max_;
    int yauto_;
    int xlog_;
    int xmaj_ticks_, xmin_ticks_;
    int xmaj_grid_, xmin_grid_;
    LogBase log_base_;
    UILayer plot_layer_;
    UILayer border_layer_;
    UIFont font_;
    TextList txt_y_;
    TextList txt_x_;
    bool running_;

public:
    UIPlotTilde();

    bool okSize(t_rect* newrect);
    void paint();
    void drawPlot();
    void drawBorder();

    void onPropChange(t_symbol* prop_name);

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

    void onInlet(const AtomList& args);

public:
    static void setup();

private:
    void addXLabel(float v, float x, float y, etextjustify_flags align, etextanchor_flags anchor);
    void addYLabel(float v, float x, float y, etextanchor_flags anchor);
};

void setup_ui_plot_tilde();

#endif // UI_PLOT_TILDE_H
