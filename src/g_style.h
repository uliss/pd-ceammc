#ifndef G_STYLE_H
#define G_STYLE_H

#include "m_pd.h"
#include "m_imp.h"

#define STYLE_COLOR_SELECTED "#0000FF"
#define STYLE_COLOR_NORMAL   "#444444"

#define STYLE_CORD_SELECTED STYLE_COLOR_SELECTED
#define STYLE_CORD_NORMAL   STYLE_COLOR_NORMAL

static int style_cord_width(t_canvas* x, t_object* ob, int closest) {
    return (obj_issignaloutlet(ob, closest) ? 2 : 1); // * x->gl_zoom;
}

#endif // G_STYLE_H
