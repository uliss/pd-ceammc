#ifndef G_STYLE_H
#define G_STYLE_H

#include "m_pd.h"

#define STYLE_NORMAL_COLOR 0x444444
#define STYLE_SELECT_COLOR 0x1441E6

#define STYLE_BORDER_WIDTH 1
#define STYLE_BORDER_WIDTH_CLICKED 3
#define STYLE_CORD_AUDIO_WIDTH 2
#define STYLE_CORD_CONTROL_WIDTH 1

#define STYLE_CONTROL_XLET_COLOR 0x555555
#define STYLE_AUDIO_XLET_COLOR 0x2200AA
#define STYLE_BORDER_COLOR 0xBFBFBF
#define STYLE_BORDER_COLOR_CLICKED 0x00AAFF
#define STYLE_FILL_COLOR 0xF5F8F8
#define STYLE_TEXT_NORMAL_COLOR STYLE_NORMAL_COLOR
#define STYLE_TEXT_SELECT_COLOR STYLE_SELECT_COLOR

#define STYLE_CANVAS_GOP_RECT_COLOR 0xFF8080

int style_cord_width_by_type(int issignal);
int style_cord_width(t_canvas* x, t_object* ob, int nout);
int style_border_width(t_glist* glist);
int style_xlet_height(t_glist* glist);

#endif // G_STYLE_H
