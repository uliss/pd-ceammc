/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef UI_SPECTROSCOPE_H
#define UI_SPECTROSCOPE_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

#include <boost/scoped_array.hpp>

using namespace ceammc;

typedef boost::scoped_array<t_sample> SampleArray;
typedef boost::scoped_array<float> SpectreArray;

class UISpectroscope : public UIDspObject {
public:
    static const size_t WINDOW_SIZE;
    static const size_t N_BINS;
    static const size_t DB_SCALE_RANGE;
    static const size_t DB_SCALE_STEP;
    static const size_t DB_SCALE_N;

private:
    ClockMemberFunction<UISpectroscope> clock_;
    t_rgba prop_color_active;
    t_rgba prop_color_scale;
    UILayer graph_layer_;
    UIFont font_;
    std::vector<UITextLayout*> x_labels_;
    std::vector<UITextLayout*> y_labels_;
    t_rgba grid_color_main_;
    t_rgba grid_color_thick_;
    SampleArray buffer_;
    SpectreArray spectre_;
    double last_redraw_time_;
    int counter_;
    int prop_refresh;
    int prop_log_scale;

public:
    UISpectroscope();
    ~UISpectroscope();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void okSize(t_rect* newrect);
    void onPropChange(t_symbol* prop_name);
    void paint();
    void drawBackground();
    void drawGraph();
    void drawGraphLinear(UIPainter& p);
    void drawGraphLog(UIPainter& p);
    void drawHGrid(UIPainter& p);

    void showPopup(const t_pt& pt, const t_pt& abs_pt);

    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);
    void dspOn(double samplerate, long blocksize);

public:
    static void setup();
    void updateFFT();

private:
    void initLabels();
    void freeLabels();
    void updateLabelColors();
};

void setup_ui_spectroscope();

#endif // UI_SPECTROSCOPE_H
