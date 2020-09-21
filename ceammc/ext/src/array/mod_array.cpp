#include "mod_array.h"
#include "array_bpm.h"
#include "array_fill.h"
#include "array_hist.h"
#include "array_mean.h"
#include "array_rms.h"
#include "array_set.h"
#include "array_stddev.h"
#include "array_stretch.h"
#include "array_sum.h"
#include "array_sum2.h"
#include "array_variance.h"
#include "array_vline_play.h"
#include "array_window.h"

void setup_array_copy();
void setup_array_do();
void setup_array_each();
void setup_array_grainer();
void setup_array_minmax();
void setup_array_p2s();
void setup_array_plot_tilde();
void setup_array_resample();
void setup_array_s2p();
void setup_plot_geomspace_tilde();
void setup_plot_hist_tilde();
void setup_plot_linspace_tilde();
void setup_plot_logspace_tilde();
void setup_plot_response_tilde();

void ceammc_array_setup()
{
    setup_array0x2eset();
    setup_array_bpm();
    setup_array_copy();
    setup_array_do();
    setup_array_each();
    setup_array_fill();
    setup_array_grainer();
    setup_array_hist();
    setup_array_mean();
    setup_array_minmax();
    setup_array_p2s();
    setup_array_plot_tilde();
    setup_array_resample();
    setup_array_rms();
    setup_array_s2p();
    setup_array_stddev();
    setup_array_stretch();
    setup_array_sum();
    setup_array_sum2();
    setup_array_variance();
    setup_array_vline_play();
    setup_array_window();

    setup_plot_linspace_tilde();
    setup_plot_logspace_tilde();
    setup_plot_geomspace_tilde();
    setup_plot_hist_tilde();
    setup_plot_response_tilde();
}
