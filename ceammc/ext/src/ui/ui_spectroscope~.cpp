//
//  ui_spectroscope~.cpp
//  pd_ext
//
//  Created by Alex on 19/12/16.
//
//

#include <algorithm>
#include <cmath>
#include <stdio.h>

#include "ceammc_gui.h"

static const size_t TXT_DB_COUNT = 10;
static const char* TXT_DB[TXT_DB_COUNT] = {
    "", " -6", "-12", "-18", "-24", "-30", "-36", "-42", "-50", "-54"
};
static const size_t TXT_FREQ_COUNT = 10;
static const char* TXT_FREQ[TXT_DB_COUNT] = {
    " 1k", " 5k", "10k", "15k", "20k", "25k", "30k", "35k", "40k", "45k"
};

static const size_t WINDOW_SIZE = 4096;
static const size_t BUFSIZE = 8192;
static t_sample hann_window[WINDOW_SIZE] = { 0 };
static t_symbol* GRID_LAYER = gensym("grid_layer");

static void init_hann_window()
{
    for (size_t i = 0; i < WINDOW_SIZE; i++) {
        hann_window[i] = (1 - cosf((2 * 3.1415926f * i) / (WINDOW_SIZE - 1))) / 2;
    }
}

struct ui_spectroscope : public ceammc_gui::BaseSoundGuiStruct {
    t_sample input;
    t_sample buf[BUFSIZE];

    size_t counter;
    t_rgba fg_color;

    t_etext* txt_db[TXT_DB_COUNT];
    t_etext* txt_freq[TXT_FREQ_COUNT];
    t_efont* txt_font;

    // this flag used to redraw spectral only on DSP update
    // not when resizing etc.
    char redraw_spectral;
};

namespace ceammc_gui {

#pragma mark dsp

static void ui_spectroscope_perform(ui_spectroscope* x,
    t_object* /*dsp64*/,
    t_sample** ins,
    long /*numins*/,
    t_sample** /*outs*/,
    long /*numouts*/,
    long sampleframes,
    long /*flags*/,
    void* /*userparam*/)
{
    t_sample* in = ins[0];
    t_sample* out = x->buf + x->counter;

    long n = sampleframes;
    while (n--) {
        *out++ = *in++;
        x->counter++;
        if (x->counter >= WINDOW_SIZE)
            break;
    }

    if (x->counter >= WINDOW_SIZE) {
        x->counter = 0;
        x->redraw_spectral = 1;
        GuiFactory<ui_spectroscope>::ws_redraw(x);
        x->redraw_spectral = 0;
    }
}

void ui_spectroscope_dsp(ui_spectroscope* x,
    t_object* dsp,
    short* /*count*/,
    double /*samplerate*/,
    long /*maxvectorsize*/,
    long /*flags*/)
{
    x->counter = 0;
    object_method(dsp, gensym("dsp_add"), x, (method)ui_spectroscope_perform, 0, NULL);
}

#pragma mark ui

UI_fun(ui_spectroscope)::wx_paint(ui_spectroscope* zx, t_object* view)
{
    t_rect rect;
    zx->getRect(&rect);

    // grid
    t_elayer* grid = ebox_start_layer(asBox(zx), GRID_LAYER, rect.width, rect.height);
    if (grid) {
        egraphics_set_line_width(grid, 1);
        egraphics_set_color_hex(grid, gensym("#DDDDDD"));

        // draw vertical lines
        {
            bool vgrid_linear = true;
            if (vgrid_linear) {
                const int STEP = (rect.width < 200) ? 2000 : 1000;
                const t_float SR = sys_getsr();

                for (t_float i = STEP; i < SR / 2; i += STEP) {
                    float x = rect.width * i * 2 / SR;
                    egraphics_line(grid, x, 0, x, rect.height);

                    // calc text index
                    const size_t FREQ_IDX = size_t(i) / 5000;
                    // skip repeats
                    if (int(i) % 5000 != 0 && int(i) != 1000)
                        continue;

                    if (FREQ_IDX >= TXT_FREQ_COUNT)
                        continue;

                    etext_layout_set(zx->txt_freq[FREQ_IDX], TXT_FREQ[FREQ_IDX],
                        zx->txt_font, x - 13, rect.height, 50, 20,
                        ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
                    etext_layout_draw(zx->txt_freq[FREQ_IDX], grid);
                }

                egraphics_stroke(grid);
            }
        }

        // draw horizontal lines
        {
            const size_t DENS = std::max<size_t>(1, 120 / size_t(rect.height));

            for (size_t i = 1; i < TXT_DB_COUNT; i++) {
                float y = rect.height * i / float(TXT_DB_COUNT - 1);
                egraphics_line(grid, 0, y, rect.width, y);

                // skip some lines if too narrow
                if (i % DENS != 0)
                    continue;

                etext_layout_set(zx->txt_db[i], TXT_DB[i], zx->txt_font,
                    rect.width - 14, y, 50, 20,
                    ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
                etext_layout_draw(zx->txt_db[i], grid);
            }

            egraphics_stroke(grid);
        }

        ebox_end_layer(asBox(zx), GRID_LAYER);
    }

    ebox_paint_layer(asBox(zx), GRID_LAYER, 0., 0.);

    if (zx->redraw_spectral) {
        // spectral
        t_elayer* l_spectral = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);
        if (l_spectral) {
            egraphics_set_line_width(l_spectral, 1);
            egraphics_set_color_rgba(l_spectral, &zx->fg_color);

            // clear imag part
            t_sample imag[BUFSIZE];
            for (size_t i = 0; i < WINDOW_SIZE; i++)
                imag[i] = 0;

            // apply hann window
            t_sample* real = zx->buf;
            for (size_t i = 0; i < WINDOW_SIZE; i++)
                real[i] *= hann_window[i];

            mayer_fft(WINDOW_SIZE, real, imag);

            t_sample out_buf[BUFSIZE];
            t_sample* pout = out_buf;

            const size_t N = WINDOW_SIZE / 2;

            for (size_t i = 1; i < N - 1; i++) {
                t_sample f_r = real[i];
                t_sample f_i = imag[i];

                pout[i] = 2 * sqrtf(f_r * f_r + f_i * f_i) / N;
                pout[i] = 10 * log10f(pout[i]);
            }

            egraphics_move_to(l_spectral, 0, rect.height);

            const size_t step = std::max<size_t>(1, N / rect.width);
            for (size_t i = 1; i < N; i += step) {
                if (i + step >= N)
                    break;

                float xx = float(i + 1) / N * rect.width;
                float val = *std::max_element(pout + i, pout + i + step);
                float yy = (-val / 54) * rect.height;

                egraphics_line_to(l_spectral, xx, yy);
            }

            egraphics_stroke(l_spectral);
            ebox_end_layer(asBox(zx), BG_LAYER);
        }
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0., 0.);
}

#pragma mark setup

UI_fun(ui_spectroscope)::new_ext(ui_spectroscope* zx, t_symbol* s, int argcl, t_atom* argv)
{
    eobj_dspsetup(zx, 1, 0);

    for (size_t i = 0; i < TXT_DB_COUNT; i++)
        zx->txt_db[i] = etext_layout_create();

    for (size_t i = 0; i < TXT_FREQ_COUNT; i++)
        zx->txt_freq[i] = etext_layout_create();

    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE_SMALL);
}

UI_fun(ui_spectroscope)::free_ext(ui_spectroscope* zx)
{
    for (size_t i = 0; i < TXT_DB_COUNT; i++)
        etext_layout_destroy(zx->txt_db[i]);

    for (size_t i = 0; i < TXT_FREQ_COUNT; i++)
        etext_layout_destroy(zx->txt_freq[i]);

    efont_destroy(zx->txt_font);
}

static void ui_sps_getdrawparams(ui_spectroscope* x, t_object* patcherview, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

UI_fun(ui_spectroscope)::init_ext(t_eclass* z)
{
    eclass_addmethod(z, (method)ui_spectroscope_dsp, "dsp", A_CANT, 0);

    // clang-format off
    CLASS_ATTR_DEFAULT        (z, "size", 0, "150. 100.");
    CLASS_ATTR_INVISIBLE      (z, "send", 0);

    CLASS_ATTR_RGBA                 (z, "fgcolor", 0, ui_spectroscope, fg_color);
    CLASS_ATTR_LABEL                (z, "fgcolor", 0, _("Foreground color"));
    CLASS_ATTR_STYLE                (z, "fgcolor", 0, "color");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "fgcolor", 0, DEFAULT_ACTIVE_COLOR);
    // clang-format on

    eclass_addmethod(z, (method)ui_sps_getdrawparams, "getdrawparams", A_NULL, 0);
}

UI_fun(ui_spectroscope)::wx_oksize(ui_spectroscope*, t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30);
    newrect->height = pd_clip_min(newrect->height, 30);
}
}

extern "C" void setup_ui0x2espectroscope_tilde()
{
    init_hann_window();

    ceammc_gui::GuiFactory<ui_spectroscope> class1;
    class1.setup_dsp("ui.spectroscope~");
}
