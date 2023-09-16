/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef HW_DISPLAY_H
#define HW_DISPLAY_H

#include "ceammc_object.h"
using namespace ceammc;

#ifdef WITH_IODISPLAY
#include "apple/iodisplay.h"
#endif

#ifdef WITH_X11DISPLAY
#include "linux/x11display.h"
#endif

class HwDisplay : public BaseObject {
#ifdef WITH_IODISPLAY
    io_display::IODisplay display_;
#endif
#ifdef WITH_X11DISPLAY
    x11_display::X11Display display_;
#endif
public:
    HwDisplay(const PdArgs& args);

    t_float propBrightness() const;
    bool setPropBrightness(t_float v);
};

extern "C" void setup_hw_display();

#endif // HW_DISPLAY_H
