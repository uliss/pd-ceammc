#ifndef X11DISPLAY_H
#define X11DISPLAY_H

#include <stdio.h>
#include <stdlib.h>

#include <xcb/randr.h>
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>

#include <ctype.h>
#include <string.h>
#include <unistd.h>

namespace x11_display {

class X11Display {
    xcb_connection_t* conn_;
    xcb_atom_t backlight_new, backlight_legacy;

public:
    X11Display();
    ~X11Display();

//    bool setBrightness(float v);
    bool getBrightness(float* v) const;
};

} // namespace x11_display

#endif // X11DISPLAY_H
