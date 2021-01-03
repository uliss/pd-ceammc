#include "mod_ui.h"

#include "ui_arrayview.h"
#include "ui_bang.h"
#include "ui_colorpanel.h"
#include "ui_display.h"
#include "ui_dsp.h"
#include "ui_env.h"
#include "ui_gain.h"
#include "ui_gain2.h"
#include "ui_icon.h"
#include "ui_incdec.h"
#include "ui_keyboard.h"
#include "ui_knob.h"
#include "ui_label.h"
#include "ui_link.h"
#include "ui_matrix.h"
#include "ui_menu.h"
#include "ui_meter.h"
#include "ui_midi.h"
#include "ui_mouse_filter.h"
#include "ui_mouse_route.h"
#include "ui_number.h"
#include "ui_number_tilde.h"
#include "ui_plot_tilde.h"
#include "ui_polar.h"
#include "ui_preset.h"
#include "ui_radio.h"
#include "ui_rslider.h"
#include "ui_scope.h"
#include "ui_slider.h"
#include "ui_slider2d.h"
#include "ui_sliders.h"
#include "ui_spectroscope.h"
#include "ui_tab.h"
#include "ui_toggle.h"

void ceammc_ui_setup()
{
    setup_ui_arrayview();
    setup_ui_bang();
    setup_ui_colorpanel();
    setup_ui_display();
    setup_ui_dsp();
    setup_ui_env();
    setup_ui_gain();
    setup_ui_gain2();
    setup_ui_icon();
    setup_ui_incdec();
    setup_ui_keyboard();
    setup_ui_knob();
    setup_ui_label();
    setup_ui_link();
    setup_ui_matrix();
    setup_ui_menu();
    setup_ui_meter();
    setup_ui_midi();
    setup_ui_mouse_filter();
    setup_ui_mouse_route();
    setup_ui_number();
    setup_ui_number_tilde();
    setup_ui_plot_tilde();
    setup_ui_polar();
    setup_ui_preset();
    setup_ui_radio();
    setup_ui_rslider();
    setup_ui_scope();
    setup_ui_slider2d();
    setup_ui_slider();
    setup_ui_sliders();
    setup_ui_spectroscope();
    setup_ui_tab();
    setup_ui_toggle();
}
