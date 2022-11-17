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
/* --------------------------------------------------------------------------*/
/*                                                                           */
/* get/set the keyboard light level on Apple Mac OS X                        */
/* Written by Hans-Christoph Steiner <hans@eds.org>                         */
/*                                                                           */
/* Copyright (c) 2008 Free Software Foundation                               */
/*                                                                           */
/* This program is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU General Public License               */
/* as published by the Free Software Foundation; either version 2            */
/* of the License, or (at your option) any later version.                    */
/*                                                                           */
/* See file LICENSE for further informations on licensing terms.             */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software Foundation,   */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA */
/*                                                                           */
/* --------------------------------------------------------------------------*/

// using code from apple/keyboard_light.c
#include "hw_kbd_light.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_stub.h"

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
    createOutlet();

#ifndef __APPLE__
    OBJ_DBG << "now supporting only Apple hardware";
#else

    io_connect_ = 0;
    io_service_ = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleLMUController"));

    if (io_service_) {
        OBJ_LOG << "found AppleLMUController";
    } else {
        OBJ_LOG << "AppleLMUController not found, trying IOI2CDeviceLMU";
        io_service_ = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOI2CDeviceLMU"));
        if (io_service_) {
            OBJ_LOG << "found IOI2CDeviceLMU";
        } else
            OBJ_ERR << "no keyboard controller found";

        return;
    }

    kern_return_t kernResult = IOServiceOpen(io_service_, mach_task_self(), 0, &io_connect_);
    IOObjectRelease(io_service_);

    if (kernResult != KERN_SUCCESS)
        OBJ_ERR << "can't open service, error code is: " << kernResult;

#endif
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

void HwKeyboardLight::onFloat(t_float v)
{
    setLevel(v);
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

bool HwKeyboardLight::setLevel(t_float v)
{
#ifdef __APPLE__
    if (!io_connect_)
        return false;

    const t_float BRIGHTNESS_MAX = 0xfff;
    t_float brightness = clip<t_float>(v * BRIGHTNESS_MAX, 0, BRIGHTNESS_MAX);

    uint64_t inputValues[3];
    uint32_t inputCount = 3;
    uint64_t outputValues[1];
    uint32_t outputCount = 1;
    inputValues[0] = 0;
    inputValues[1] = brightness;
    inputValues[2] = DEFAULT_DELAYTIME;
    kern_return_t kernResult = IOConnectCallScalarMethod(io_connect_,
        kSetLEDFadeID,
        inputValues,
        inputCount,
        outputValues,
        &outputCount);

    if (kernResult != KERN_SUCCESS) {
        if (kernResult == kIOReturnBusy)
            OBJ_ERR << "device busy";
        else
            OBJ_ERR << "could not write to device";

        return false;
    }

    return true;
#else
    return false;
#endif
}

#if !defined(__arm64__) && defined(__APPLE__)
extern "C" void setup_hw_keyboard_light()
{
    ObjectFactory<HwKeyboardLight> obj("hw.kbd_light");
}
#else
CONTROL_OBJECT_STUB(1, 1, "not supported on this platform");
OBJECT_STUB_SETUP("hw.kbd_light", hw_keyboard_light);
#endif
