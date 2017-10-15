#ifndef G_STYLE_H
#define G_STYLE_H

#include "m_pd.h"
#include "m_imp.h"

#define STYLE_NORMAL_COLOR  "#444444"
#define STYLE_SELECT_COLOR  "#1441E6"
#define STYLE_EDIT_COLOR    "#F62459"

#define STYLE_BORDER_WIDTH 1
#define STYLE_BORDER_WIDTH_CLICKED 3
#define STYLE_SLIDER_KNOB_WIDTH 3
#define STYLE_XLET_WIDTH 7

#define STYLE_CONTROL_XLET_COLOR "#000000"
#define STYLE_AUDIO_XLET_COLOR "#2200AA"
#define STYLE_BORDER_COLOR "#BFBFBF"
#define STYLE_BORDER_COLOR_CLICKED "#00AAFF"
#define STYLE_FILL_COLOR  "#F5F8F8"
#define STYLE_TEXT_NORMAL_COLOR STYLE_NORMAL_COLOR
#define STYLE_TEXT_SELECT_COLOR STYLE_SELECT_COLOR

#define STYLE_IEM_BORDER_COLOR "#000000"

#define STYLE_CORD_SELECTED STYLE_SELECT_COLOR
#define STYLE_CORD_NORMAL   STYLE_NORMAL_COLOR

#define STYLE_CANVAS_GOP_RECT_COLOR "#ff8080"

static inline int style_cord_width(t_canvas* x, t_object* ob, int closest) {
    return (obj_issignaloutlet(ob, closest) ? 2 : 1) * x->gl_zoom;
}

static inline int style_border_width(t_glist * glist) {
//    glist->gl_zoom
    return STYLE_BORDER_WIDTH;
}

static inline int style_xlet_height(t_glist * glist) {
//    return 1 - 2*glist->gl_zoom;
    return 1 + 1 ;// EXTRAPIX;
}

#endif // G_STYLE_H
