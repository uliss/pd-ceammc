/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_CREAM_LIBRARY
#define DEF_CREAM_LIBRARY

extern "C" {
#include "cicm_wrapper.h"
}

#define CREAM_MAXITEMS 256

// gettext extract
#ifndef _
#define _(msg) msg
#endif

// clang-format off

extern "C" void cream_setup(void);
extern "C" void Cream_setup(void);
extern "C" void libpd_loadcream(void);

//extern "C" void setup_c0x2egain_tilde(void);


//CEAMMC
extern "C" void setup_ui0x2ebang(void);
extern "C" void setup_ui0x2ecolorpanel(void);
extern "C" void setup_ui0x2edsp_tilde(void);
extern "C" void setup_ui0x2eincdec(void);
extern "C" void setup_ui0x2enumber_tilde(void);
extern "C" void setup_ui0x2enumber(void);
extern "C" void setup_ui0x2ematrix(void);
extern "C" void setup_ui0x2emenu(void);
extern "C" void setup_ui0x2emeter_tilde(void);
extern "C" void setup_ui0x2epreset(void);
extern "C" void setup_ui0x2eradio(void);
extern "C" void setup_ui0x2erslider(void);
extern "C" void setup_ui0x2eslider(void);
extern "C" void setup_ui0x2etab(void);
extern "C" void setup_ui0x2etoggle(void);

static t_symbol* cream_sym_goppos               = gensym("goppos");
static t_symbol* cream_sym_gopsize              = gensym("gopsize");
static t_symbol* cream_sym_select               = gensym("select");
static t_symbol* cream_sym_delete               = gensym("delete");
static t_symbol* cream_sym_past                 = gensym("paste");

static t_symbol* cream_sym_attr_modified        = gensym("attr_modified");

static t_symbol* cream_sym_background_layer     = gensym("background_layer");
static t_symbol* cream_sym_text_layer           = gensym("text_layer");
static t_symbol* cream_sym_selection_layer      = gensym("selection_layer");
static t_symbol* cream_sym_points_layer         = gensym("points_layer");
static t_symbol* cream_sym_value_layer          = gensym("value_layer");
static t_symbol* cream_sym_knob_layer           = gensym("knob_layer");
static t_symbol* cream_sym_signal_layer         = gensym("signal_layer");
static t_symbol* cream_sym_leds_layer           = gensym("leds_layer");
static t_symbol* cream_sym_items_layer          = gensym("items_layer");
static t_symbol* cream_sym_needle_layer         = gensym("needle_layer");

static t_symbol* cream_sym_bacolor              = gensym("bacolor");
static t_symbol* cream_sym_bgcolor              = gensym("bgcolor");
static t_symbol* cream_sym_bdcolor 		= gensym("bdcolor");
static t_symbol* cream_sym_crcolor 		= gensym("crcolor");
static t_symbol* cream_sym_btcolor              = gensym("btcolor");
static t_symbol* cream_sym_textcolor            = gensym("textcolor");
static t_symbol* cream_sym_ptcolor              = gensym("ptcolor");
static t_symbol* cream_sym_licolor              = gensym("licolor");
static t_symbol* cream_sym_coldcolor            = gensym("coldcolor");
static t_symbol* cream_sym_tepidcolor           = gensym("tepidcolor");
static t_symbol* cream_sym_warmcolor            = gensym("warmcolor");
static t_symbol* cream_sym_hotcolor             = gensym("hotcolor");
static t_symbol* cream_sym_overcolor            = gensym("overcolor");
static t_symbol* cream_sym_wkeycolor            = gensym("wkeycolor");
static t_symbol* cream_sym_bkeycolor            = gensym("bkeycolor");
static t_symbol* cream_sym_skeycolor            = gensym("skeycolor");
static t_symbol* cream_sym_kncolor              = gensym("kncolor");
static t_symbol* cream_sym_itcolor              = gensym("itcolor");
static t_symbol* cream_sym_secolor              = gensym("secolor");
static t_symbol* cream_sym_hocolor              = gensym("hocolor");
static t_symbol* cream_sym_necolor              = gensym("necolor");

static t_symbol* cream_sym_fontsize             = gensym("fontsize");
static t_symbol* cream_sym_fontname             = gensym("fontname");
static t_symbol* cream_sym_fontweight           = gensym("fontweight");
static t_symbol* cream_sym_fontslant            = gensym("fontslant");

static t_symbol* cream_sym_endless              = gensym("endless");
static t_symbol* cream_sym_ptsize               = gensym("ptsize");
static t_symbol* cream_sym_preset               = gensym("preset");
static t_symbol* cream_sym_atpreset             = gensym("@preset");
static t_symbol* cream_sym_atindex              = gensym("@index");
static t_symbol* cream_sym_interpolate          = gensym("interpolate");
static t_symbol* cream_sym_absrange             = gensym("absrange");
static t_symbol* cream_sym_ordrange             = gensym("ordrange");
static t_symbol* cream_sym_atpoints             = gensym("@points");
static t_symbol* cream_sym_function             = gensym("function");
static t_symbol* cream_sym_Linear               = gensym("Linear");
static t_symbol* cream_sym_Cosine               = gensym("Cosine");
static t_symbol* cream_sym_Cubic                = gensym("Cubic");
static t_symbol* cream_sym_outline              = gensym("outline");
static t_symbol* cream_sym_items                = gensym("items");
static t_symbol* cream_sym_orientation          = gensym("orientation");
static t_symbol* cream_sym_lowkey               = gensym("lowkey");
static t_symbol* cream_sym_highkey              = gensym("highkey");

static t_symbol* cream_sym_nothing              = gensym("''");
static t_symbol* cream_sym_left_bracket         = gensym("[");
static t_symbol* cream_sym_right_bracket        = gensym("]");

// clang-format on

static const char* DEFAULT_ACTIVE_COLOR = "0. 0.75 1. 1.";
static const char* DEFAULT_BORDER_COLOR = "0.6 0.6 0.6 1.";
static const char* DEFAULT_BACKGROUND_COLOR = "0.93 0.93 0.93 1.";
static const char* DEFAULT_TEXT_COLOR = "0. 0. 0. 1.";

static const char* PROP_ACTIVE_COLOR = "active_color";
static const char* PROP_BACKGROUND_COLOR = "background_color";
static const char* PROP_BORDER_COLOR = "border_color";
static const char* PROP_TEXT_COLOR = "text_color";

#define ATTR_DEFAULT_COLOR_BORDER(c, struct_name) { \
    CLASS_ATTR_RGBA                 (c, PROP_BORDER_COLOR, 0, struct_name, color_border);\
    CLASS_ATTR_LABEL                (c, PROP_BORDER_COLOR, 0, _("Border Color"));\
    CLASS_ATTR_ORDER                (c, PROP_BORDER_COLOR, 0, "1");\
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, PROP_BORDER_COLOR, 0, DEFAULT_BORDER_COLOR);\
    CLASS_ATTR_STYLE                (c, PROP_BORDER_COLOR, 0, "color");\
}

#define ATTR_DEFAULT_COLOR_BACKGROUND(c, struct_name) { \
    CLASS_ATTR_RGBA                 (c, PROP_BACKGROUND_COLOR, 0, struct_name, color_background);\
    CLASS_ATTR_LABEL                (c, PROP_BACKGROUND_COLOR, 0, _("Background Color"));\
    CLASS_ATTR_ORDER                (c, PROP_BACKGROUND_COLOR, 0, "2");\
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, PROP_BACKGROUND_COLOR, 0, DEFAULT_BACKGROUND_COLOR);\
    CLASS_ATTR_STYLE                (c, PROP_BACKGROUND_COLOR, 0, "color");\
}

#define ATTR_DEFAULT_COLOR_ACTIVE(c, struct_name) { \
    CLASS_ATTR_RGBA                 (c, PROP_ACTIVE_COLOR, 0, struct_name, color_active);\
    CLASS_ATTR_LABEL                (c, PROP_ACTIVE_COLOR, 0, _("Active Color"));\
    CLASS_ATTR_ORDER                (c, PROP_ACTIVE_COLOR, 0, "3");\
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, PROP_ACTIVE_COLOR, 0, DEFAULT_ACTIVE_COLOR);\
    CLASS_ATTR_STYLE                (c, PROP_ACTIVE_COLOR, 0, "color");\
}

#define ATTR_DEFAULT_COLOR_ACTIVE(c, struct_name) { \
    CLASS_ATTR_RGBA                 (c, PROP_ACTIVE_COLOR, 0, struct_name, color_active);\
    CLASS_ATTR_LABEL                (c, PROP_ACTIVE_COLOR, 0, _("Active Color"));\
    CLASS_ATTR_ORDER                (c, PROP_ACTIVE_COLOR, 0, "3");\
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, PROP_ACTIVE_COLOR, 0, DEFAULT_ACTIVE_COLOR);\
    CLASS_ATTR_STYLE                (c, PROP_ACTIVE_COLOR, 0, "color");\
}

#define ATTR_DEFAULT_COLOR_TEXT(c, struct_name) { \
    CLASS_ATTR_RGBA                 (c, PROP_TEXT_COLOR, 0, struct_name, color_text);\
    CLASS_ATTR_LABEL                (c, PROP_TEXT_COLOR, 0, _("Text Color"));\
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, PROP_TEXT_COLOR, 0, DEFAULT_TEXT_COLOR);\
    CLASS_ATTR_STYLE                (c, PROP_TEXT_COLOR, 0, "color");\
}

#define CREAM_DEFAULT_DRAW_PARAMS() {\
    params->d_borderthickness = 1;\
    params->d_cornersize      = 0;\
    params->d_bordercolor     = x->color_border;\
    params->d_boxfillcolor    = x->color_background;\
}

#endif
