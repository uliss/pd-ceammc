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
#include "ui_spectroscope.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"
#include "ceammc_window.h"

#include <algorithm>
#include <cmath>

static const size_t TXT_DB_COUNT = 10;
static const char* TXT_DB[TXT_DB_COUNT] = {
    "", " -6", "-12", "-18", "-24", "-30", "-36", "-42", "-50", "-54"
};

const size_t UISpectroscope::WINDOW_SIZE = 4096;
const size_t UISpectroscope::N_BINS = UISpectroscope::WINDOW_SIZE / 2;
const size_t UISpectroscope::DB_SCALE_RANGE = 70;
const size_t UISpectroscope::DB_SCALE_STEP = 10;
const size_t UISpectroscope::DB_SCALE_N = UISpectroscope::DB_SCALE_RANGE / UISpectroscope::DB_SCALE_STEP;
static t_sample hann_window[UISpectroscope::WINDOW_SIZE] = { 0 };

static bool init_hann_window()
{
    return window::fill(hann_window, hann_window + UISpectroscope::WINDOW_SIZE, window::hann<float>);
}

UISpectroscope::UISpectroscope()
    : clock_(this, &UISpectroscope::updateFFT)
    , prop_color_active(rgba_blue)
    , prop_color_scale(rgba_black)
    , graph_layer_(asEBox(), gensym("graph_layer"))
    , font_(gensym(FONT_FAMILY), FONT_SIZE_SMALL - 1)
    , grid_color_main_(rgba_greylight)
    , grid_color_thick_(rgba_greylight)
    , last_redraw_time_(0)
    , counter_(0)
    , prop_refresh(100)
    , prop_log_scale(0)
{

    initPopupMenu("ss_log", { { _("log scale"), [this](const t_pt&) {
                                   prop_log_scale = true;
                                   bg_layer_.invalidate();
                                   graph_layer_.invalidate();
                                   redraw();
                               } } });
    initPopupMenu("ss_lin", { { _("linear scale"), [this](const t_pt&) {
                                   prop_log_scale = false;
                                   bg_layer_.invalidate();
                                   graph_layer_.invalidate();
                                   redraw();
                               } } });
}

UISpectroscope::~UISpectroscope()
{
    freeLabels();
}

void UISpectroscope::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIDspObject::init(name, args, usePresets);
    dspSetup(1, 0);
    initLabels();
    updateLabelColors();
    buffer_.reset(new t_sample[WINDOW_SIZE]);
    spectre_.reset(new float[N_BINS]);
    std::fill(&buffer_[0], &buffer_[WINDOW_SIZE], 0);
    std::fill(&spectre_[0], &spectre_[N_BINS], 0.f);
}

void UISpectroscope::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30);
    newrect->height = pd_clip_min(newrect->height, 30);
}

void UISpectroscope::onPropChange(t_symbol* prop_name)
{
    grid_color_main_ = rgba_addContrast(prop_color_background, -0.12);
    grid_color_thick_ = rgba_addContrast(prop_color_background, -0.05);
    updateLabelColors();

    bg_layer_.invalidate();
    graph_layer_.invalidate();
    redraw();
}

void UISpectroscope::paint()
{
    drawBackground();
    drawGraph();
}

static int roundGridFreq(int freq)
{
    static int f[] = { 100, 250, 500, 1000, 2500, 5000, 10000 };

    int* end = f + sizeof(f) / sizeof(f[0]);
    int* p = std::lower_bound(f, end, freq);
    return (p == end) ? 1000 : *p;
}

void UISpectroscope::drawBackground()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    // draw grid
    if (prop_log_scale) {
        const int MIN_FREQ = 10;
        const int MAX_FREQ = samplerate() / 2;
        const float FULL_F_RANGE = log10f(MAX_FREQ / float(MIN_FREQ));

        for (int i = MIN_FREQ, lbl_idx = 1; i < MAX_FREQ; i *= 10, lbl_idx++) {
            for (int j = 1; j < 10; j++) {
                const float f = i * j;
                const float lf = log10f(MAX_FREQ / f);
                const float x = convert::lin2lin<float>(lf, FULL_F_RANGE, 0, 0, r.width);

                // minimal frequency is special case
                if (f == MIN_FREQ) {
                    // draw label
                    y_labels_[0]->set("10", x + 3, r.height - 2, 0, 0);
                    p.drawText(*y_labels_[0]);
                    // but skip line
                    continue;
                }

                p.setColor(j == 1 ? grid_color_main_ : grid_color_thick_);
                p.drawLine(x, 0, x, r.height);

                if (j == 1) {
                    // draw label
                    char buf[16];
                    sprintf(buf, "%i", int(f));
                    y_labels_[lbl_idx]->set(buf, x + 3, r.height - 2, 0, 0);
                    p.drawText(*y_labels_[lbl_idx]);
                }
            }
        }
    } else {
        const int MAX_FREQ = samplerate() / 2;
        const int GRID_STEP_FREQ = roundGridFreq(MAX_FREQ / (r.width / 8));
        const int MAIN_GRID_STEP = (GRID_STEP_FREQ < 500) ? 1000 : 5000;

        // draw thick lines
        p.setColor(grid_color_thick_);
        for (int f = GRID_STEP_FREQ; f < MAX_FREQ; f += GRID_STEP_FREQ) {
            // skip bold lines
            if (f % MAIN_GRID_STEP == 0)
                continue;

            float x = convert::lin2lin<float>(f, 0, MAX_FREQ, 0, r.width);
            p.drawLine(x, 0, x, r.height);
        }

        // draw fat lines and labels
        p.setColor(grid_color_main_);
        for (int f = MAIN_GRID_STEP; f < MAX_FREQ; f += MAIN_GRID_STEP) {

            float x = convert::lin2lin<float>(f, 0, MAX_FREQ, 0, r.width);
            p.drawLine(x, 0, x, r.height);

            // calc label index
            size_t idx = f / MAIN_GRID_STEP;
            if (idx >= y_labels_.size())
                continue;

            // draw label
            char buf[16];
            sprintf(buf, "%ik", f / 1000);
            y_labels_[idx]->set(buf, x + 3, r.height - 2, 0, 0);
            p.drawText(*y_labels_[idx]);
        }
    }

    drawHGrid(p);
}

void UISpectroscope::drawGraph()
{
    const t_rect r = rect();

    UIPainter p = graph_layer_.painter(r);
    if (!p)
        return;

    t_sample* real = &buffer_[0];
    t_sample imag[WINDOW_SIZE];
    std::fill(imag, imag + WINDOW_SIZE, 0);

    mayer_fft(WINDOW_SIZE, real, imag);

    for (size_t i = 0; i < N_BINS; i++) {
        t_sample re = real[i];
        t_sample im = imag[i];

        spectre_[i] = convert::amp2dbfs(sqrtf(re * re + im * im) / N_BINS);
    }

    if (prop_log_scale)
        drawGraphLog(p);
    else
        drawGraphLinear(p);
}

void UISpectroscope::drawGraphLinear(UIPainter& p)
{
    const t_rect r = rect();

    int outside_counter = 0;

    p.setColor(prop_color_active);
    p.moveTo(0, r.height);
    const size_t x_step = std::max<size_t>(1, N_BINS / r.width);

    for (size_t i = 0; i < N_BINS; i += x_step) {
        if (i + x_step >= N_BINS)
            break;

        float* element = &spectre_[i];
        float x = convert::lin2lin<float>(i, 0, N_BINS, 0, r.width);
        float v = *std::max_element(element, element + x_step);
        float y = convert::lin2lin<float>(v, 0, -float(DB_SCALE_RANGE), 0, r.height);

        // outside
        if (y > r.height) {
            outside_counter++;
            // first time
            if (outside_counter == 1) {
                p.drawLineTo(x, r.height);
            }
        } else {
            if (outside_counter > 0) {
                outside_counter = 0;
                p.drawLineTo(x, r.height);
            }
        }

        p.drawLineTo(x, y);
    }

    p.stroke();
}

static inline float approx_lin(float v, float* buf)
{
    int idx = int(v);
    float mix = v - idx;
    return (1 - mix) * buf[idx] + mix * buf[idx + 1];
}

#define pow10f(x) (expf(2.302585092994046f * x))

void UISpectroscope::drawGraphLog(UIPainter& p)
{
    static const int MIN_FREQ = 10;
    const t_rect r = rect();

    const int MAX_FREQ = samplerate() / 2;
    const float k = float(MAX_FREQ) / N_BINS;
    const float FULL_FREQ_RANGE = log10f(MAX_FREQ / MIN_FREQ);

    p.setColor(prop_color_active);
    p.moveTo(0, r.height);

    int prev_bin = -1;
    int out_of_range_counter = 0;
    for (int x = 0; x < r.width; x++) {
        // pixel->pixel_freq
        float f = convert::lin2lin<float>(x, 0, r.width, FULL_FREQ_RANGE, 0);
        const float pix_freq = MAX_FREQ / pow10f(f);
        // pixel_freq->#n_bin
        float f_bin = pix_freq / k;
        int i_bin = int(f_bin);

        // skip last bin
        if (i_bin >= N_BINS)
            break;

        if (prev_bin != i_bin) {
            // not exact precision but seems to be enough
            const size_t next_bin = std::min<size_t>(N_BINS, i_bin + i_bin - prev_bin);
            float v = *std::max_element(&spectre_[i_bin], &spectre_[next_bin]);
            float y = convert::lin2lin<float>(v, 0, -float(DB_SCALE_RANGE), 0, r.height);

            if (y > r.height) {
                out_of_range_counter++;

                // first time is out of range
                if (out_of_range_counter == 1) {
                    // draw to bottom
                    p.drawLineTo(x, r.height);
                } else {
                    // do nothing
                }
            } else {
                // first return in range
                if (out_of_range_counter) {
                    out_of_range_counter = 0;

                    // draw to point
                    p.drawLineTo(x, r.height);
                }

                p.drawLineTo(x, y);
            }

            // update previous
            prev_bin = i_bin;
        }
    }

    p.stroke();
}

void UISpectroscope::drawHGrid(UIPainter& p)
{
    const t_rect r = rect();

    // draw horizontal lines
    int v_step = roundf(r.height / DB_SCALE_N);
    int db_step = (v_step < 20) ? ((20 / v_step) + 1) * v_step : v_step;

    p.setColor(grid_color_main_);
    for (int y = v_step; y < r.height; y += v_step) {
        p.drawLine(0, y, r.width, y);

        if (y % db_step == 0) {
            size_t idx = y / v_step;
            // skip last label
            if (idx == DB_SCALE_N)
                continue;

            // skip last-1 label if too low height
            if ((idx == DB_SCALE_N - 1) && r.height < 160)
                continue;

            // draw label
            char buf[16];
            sprintf(buf, "%i", int(-idx * DB_SCALE_STEP));
            x_labels_[idx]->set(buf, r.width - 2, y + 1, 0, 0);
            p.drawText(*x_labels_[idx]);
        }
    }

    // draw 0db
    x_labels_[0]->set("0 db", r.width - 2, 2, 0, 0);
    p.drawText(*x_labels_[0]);
}

void UISpectroscope::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    if (prop_log_scale)
        showPopupMenu("ss_lin", pt, abs_pt);
    else
        showPopupMenu("ss_log", pt, abs_pt);
}

void UISpectroscope::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    t_sample* in = ins[0];
    t_sample* out = buffer_.get() + counter_;

    long n = sampleframes;
    while (n--) {
        // apply Hann window
        *out++ = (*in++) * hann_window[counter_];

        counter_++;

        if (counter_ >= WINDOW_SIZE) {
            counter_ = 0;
            clock_.delay(0);
            return;
        }
    }
}

void UISpectroscope::dspOn(double samplerate, long blocksize)
{
    UIDspObject::dspOn(samplerate, blocksize);
    counter_ = 0;

    // if samplerate changes - redraw background layer, grid and freq labels
    bg_layer_.invalidate();
    graph_layer_.invalidate();
}

void UISpectroscope::setup()
{
    static const bool init = init_hann_window();

    UIObjectFactory<UISpectroscope> obj("ui.spectroscope~", EBOX_GROWINDI);
    obj.usePopup();
    obj.addAlias("ui.ssc~");
    obj.setDefaultSize(150, 100);

    obj.addColorProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UISpectroscope::prop_color_active);
    obj.addColorProperty("scale_color", _("Scale Color"), "0.6 0.6 0.6 1", &UISpectroscope::prop_color_scale);
    obj.addIntProperty("refresh", _("Refresh time (ms)"), 100, &UISpectroscope::prop_refresh, "Main");
    obj.setPropertyRange("refresh", 20, 1000);
    obj.setPropertyUnits("refresh", "msec");
    obj.addBoolProperty("log_scale", _("Log scale"), false, &UISpectroscope::prop_log_scale, "Main");

    obj.hideProperty("send");
}

void UISpectroscope::updateFFT()
{
    double t = clock_gettimesince(last_redraw_time_);
    if (t > prop_refresh) {
        last_redraw_time_ = clock_getlogicaltime();
        graph_layer_.invalidate();
        redraw();
    }
}

void UISpectroscope::initLabels()
{
    static const size_t MAX_LABELS = 48;
    y_labels_.reserve(MAX_LABELS);
    for (size_t i = 0; i < MAX_LABELS; i++) {
        UITextLayout* l = new UITextLayout(font_.font(), ColorRGBA::black(),
            ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);

        y_labels_.push_back(l);
    }

    x_labels_.reserve(10);
    for (size_t i = 0; i < 10; i++) {
        UITextLayout* l = new UITextLayout(font_.font(), ColorRGBA::black(),
            ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP);

        x_labels_.push_back(l);
    }
}

void UISpectroscope::freeLabels()
{
    for (size_t i = 0; i < x_labels_.size(); i++)
        delete x_labels_[i];

    for (size_t i = 0; i < y_labels_.size(); i++)
        delete y_labels_[i];
}

void UISpectroscope::updateLabelColors()
{
    for (size_t i = 0; i < x_labels_.size(); i++)
        x_labels_[i]->setColor(prop_color_scale);

    for (size_t i = 0; i < y_labels_.size(); i++)
        y_labels_[i]->setColor(prop_color_scale);
}

void setup_ui_spectroscope()
{
    UISpectroscope::setup();
}
