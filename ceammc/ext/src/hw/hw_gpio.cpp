/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#include "hw_gpio.h"
#include "ceammc_factory.h"

HwGpio::HwGpio(const PdArgs& args)
    : HwGpioBase(args)
{
    gpio_ = ceammc_hw_gpio_new();
}

HwGpio::~HwGpio()
{
    ceammc_hw_gpio_free(gpio_);
}

bool HwGpio::notify(int code)
{
    return true;
}

void setup_hw_gpio()
{
    ObjectFactory<HwGpio> obj("hw.gpio");
    // obj.addMethod("devices", &HwGpio::m_devices);
    // obj.setXletsInfo(
    //     { "control inlet" },
    //     {
    //         "button: list ID BTN STATE VAL",
    //         "axis: list ID AXIS VAL",
    //         "any: connected ID STATE\n"
    //         "any: device INFO",
    //     });
}
