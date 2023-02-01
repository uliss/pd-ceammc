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
#include "hw_display.h"
#include "ceammc_factory.h"
#include "ceammc_stub.h"

HwDisplay::HwDisplay(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createCbFloatProperty(
        "@brightness",
        [this]() -> t_float { return propBrightness(); },
        [this](t_float f) -> bool { return setPropBrightness(f); });
}

t_float HwDisplay::propBrightness() const
{
#ifdef WITH_IODISPLAY
    float v = 0;
    if (!display_.getBrightness(&v))
        OBJ_ERR << "can't get brightness";

    return v;
#endif

#ifdef WITH_X11DISPLAY
    float v = 0;
    if (!display_.getBrightness(&v))
        OBJ_ERR << "can't get brightness";

    return v;
#endif

    return -1;
}

bool HwDisplay::setPropBrightness(t_float v)
{
#ifdef WITH_IODISPLAY
    if (!display_.setBrightness(v)) {
        OBJ_ERR << "can't set brightness";
        return false;
    } else
        return true;
#endif

#ifdef WITH_X11DISPLAY
    if (!display_.setBrightness(v)) {
        OBJ_ERR << "can't set brightness";
        return false;
    } else
        return true;
#endif

    return false;
}

#if (!defined(__arm64__) && defined(WITH_IODISPLAY)) || defined(WITH_X11DISPLAY)
extern "C" void setup_hw_display()
{
    ObjectFactory<HwDisplay> obj("hw.display");
}
#else
CONTROL_OBJECT_STUB(1, 1, "not supported on this platform");
OBJECT_STUB_SETUP("hw.display", hw_display);
#endif
