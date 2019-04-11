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
#include "hw_kbd_light.h"
#include "ceammc_factory.h"

#ifdef __APPLE__
#include <mach/mach.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <IOKit/IOKitLib.h>

enum {
    kGetSensorReadingID = 0, // getSensorReading(int *, int *)
    kGetLEDBrightnessID = 1, // getLEDBrightness(int, int *)
    kSetLEDBrightnessID = 2, // setLEDBrightness(int, int, int *)
    kSetLEDFadeID = 3, // setLEDFade(int, int, int, int *)
    // other firmware-related functions
    verifyFirmwareID = 4, // verifyFirmware(int *)
    getFirmwareVersionID = 5, // getFirmwareVersion(int *)
    // other flashing-related functions
    // ...
};

#define DEFAULT_DELAYTIME 250

#endif

HwKeyboardLight::HwKeyboardLight(const PdArgs& args)
    : BaseObject(args)
{
#ifndef __APPLE__
    OBJ_DBG << "now supporting only Apple hardware";
#else

    io_connect_ = 0;

    io_service_ = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleLMUController"));
    if (io_service_) {
        OBJ_DBG << "found AppleLMUController";
        //        logpost(x, 4, "[keyboard_light]: found AppleLMUController");
    } else {
        OBJ_DBG << "AppleLMUController not found, trying IOI2CDeviceLMU";
        io_service_ = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOI2CDeviceLMU"));
        if (io_service_) {
            OBJ_DBG << "found IOI2CDeviceLMU";
            //            logpost(x, 4, "[keyboard_light]: found IOI2CDeviceLMU");
        } else
            OBJ_ERR << "no sensor found";
    }

    kern_return_t kernResult = IOServiceOpen(io_service_, mach_task_self(), 0, &io_connect_);

    IOObjectRelease(io_service_);
    if (kernResult != KERN_SUCCESS) {
        OBJ_ERR << "IOServiceOpen(): " << kernResult;
    }
#endif

    createOutlet();
}

HwKeyboardLight::~HwKeyboardLight()
{
#ifdef __APPLE__
    if (io_connect_)
        IOServiceClose(io_connect_);
#endif
}

void HwKeyboardLight::onBang()
{
    floatTo(0, currentLevel());
}

t_float HwKeyboardLight::currentLevel() const
{
#ifdef __APPLE__
    if (!io_connect_) {
        OBJ_ERR << "no device available";
        return 0;
    }

    uint64_t inputValues[1] = { 0 };
    uint32_t inputCount = 1;
    uint64_t outputValues[1] = { 0 };
    uint32_t outputCount = 1;
    kern_return_t kernResult = IOConnectCallScalarMethod(io_connect_,
        kGetLEDBrightnessID,
        inputValues,
        inputCount,
        outputValues,
        &outputCount);

    const t_float BRIGHTNESS_MAX = 0xfff;

    if (kernResult == KERN_SUCCESS)
        return outputValues[0] / BRIGHTNESS_MAX;
    else if (kernResult == kIOReturnBusy)
        OBJ_ERR << "device busy";
    else
        OBJ_ERR << "could not read device";

    return 0;
#else
    return 0;
#endif
}

void setup_hw_keyboard_light()
{
    ObjectFactory<HwKeyboardLight> obj("hw.kbd_light");
}
