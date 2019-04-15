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
#include "iodisplay.h"
#include "ceammc_log.h"

#include <IOKit/IOKitLib.h>
#include <IOKit/graphics/IOGraphicsLib.h>

using namespace ceammc;

namespace io_display {

const CFStringRef kDisplayBrightness = CFSTR(kIODisplayBrightnessKey);

IODisplay::IODisplay()
    : service_(0)
{
    io_iterator_t iterator;
    kern_return_t result = IOServiceGetMatchingServices(kIOMasterPortDefault,
        IOServiceMatching("IODisplayConnect"),
        &iterator);

    // If we were successful
    if (result == kIOReturnSuccess) {
        service_ = IOIteratorNext(iterator);
        IOObjectRelease(iterator);
    }
}

IODisplay::~IODisplay()
{
    if (service_)
        IOObjectRelease(service_);
}

bool IODisplay::setBrightness(float v)
{
    if (!service_)
        return false;

    kern_return_t result = IODisplaySetFloatParameter(service_, kNilOptions, kDisplayBrightness, v);
    return (result == kIOReturnSuccess);
}

bool IODisplay::getBrightness(float* v) const
{
    if (!service_)
        return false;

    kern_return_t result = IODisplayGetFloatParameter(service_, kNilOptions, kDisplayBrightness, v);
    return (result == kIOReturnSuccess);
}

}
