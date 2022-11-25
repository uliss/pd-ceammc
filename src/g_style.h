#ifndef G_STYLE_H
#define G_STYLE_H

#include "m_pd.h"

#define STYLE_BORDER_WIDTH 1
#define STYLE_BORDER_WIDTH_CLICKED 3
#define STYLE_CORD_AUDIO_WIDTH 2
#define STYLE_CORD_CONTROL_WIDTH 1

#define STYLE_CANVAS_GOP_RECT_COLOR 0xFF8080

int style_cord_width_by_type(int issignal);
int style_cord_width(t_canvas* x, t_object* ob, int nout);
int style_border_width(t_glist* glist);
int style_xlet_height(t_glist* glist);

#endif // G_STYLE_H
