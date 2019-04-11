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
#include "hw_apple_sms.h"
#include "ceammc_factory.h"

#ifdef __APPLE__
#include <mach/mach.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <IOKit/IOKitLib.h>
#endif

struct data {
    char x;
    char y;
    char z;
    char pad[57];
};

HwAppleSMS::HwAppleSMS(const PdArgs& args)
    : BaseObject(args)
{
#ifndef __APPLE__
    OBJ_DBG << "supported only on MacOS";
#endif

    createOutlet();
}

void HwAppleSMS::onBang()
{
    auto lst = getXYZ();
    if (!lst.empty())
        listTo(0, lst);
}

AtomList HwAppleSMS::getXYZ() const
{
#ifndef __APPLE__
    AtomList res(0.f, 0.f);
    res.append(0.f);
    return res;
#else
    const char* name = "";

    io_connect_t io_connect;
    int kernel_function, sizeof_data;

    struct data inputStructure;
    struct data outputStructure;

    mach_port_t mach_port;
    kern_return_t kern_return = IOMasterPort(MACH_PORT_NULL, &mach_port);
    if (kern_return != KERN_SUCCESS) {
        OBJ_ERR << "cannot get mach_port";
        return AtomList();
    }

    io_iterator_t io_iterator;
    //
    kern_return = IOServiceGetMatchingServices(mach_port, IOServiceMatching("PMUMotionSensor"), &io_iterator);
    if (kern_return == KERN_SUCCESS && io_iterator != 0) {
        name = "PMUMotionSensor";
        sizeof_data = 60;
        kernel_function = 21;
        goto FOUND_SENSOR;
    }
    // MacBook Pro
    kern_return = IOServiceGetMatchingServices(mach_port, IOServiceMatching("SMCMotionSensor"), &io_iterator);
    if (kern_return == KERN_SUCCESS && io_iterator != 0) {
        name = "SMCMotionSensor";
        sizeof_data = 40;
        kernel_function = 5;
        goto FOUND_SENSOR;
    }

    {
        OBJ_ERR << "cannot find motionsensor";
        return AtomList();
    }

FOUND_SENSOR:
    io_object_t io_object = IOIteratorNext(io_iterator);
    IOObjectRelease(io_iterator);

    if (!io_object) {
        OBJ_ERR << "No motion sensor available";
        return AtomList();
    }

    kern_return = IOServiceOpen(io_object, mach_task_self(), 0, &io_connect);
    IOObjectRelease(io_object);

    if (kern_return != KERN_SUCCESS) {
        OBJ_ERR << "Could not open motion sensor device";
        return AtomList();
    }

    size_t structureInputSize = sizeof_data; //sizeof(struct data);
    size_t structureOutputSize = sizeof_data; //sizeof(struct data);

    memset(&inputStructure, 0, sizeof(inputStructure));
    memset(&outputStructure, 0, sizeof(outputStructure));

#ifdef AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER
    kern_return = IOConnectCallStructMethod(
        io_connect, // an io_connect_t returned from IOServiceOpen().
        kernel_function, // selector of the function to be called via the user client.
        &inputStructure, // pointer to the input struct parameter.
        structureInputSize, // the size of the input structure parameter.
        &outputStructure, // pointer to the output struct parameter.
        &structureOutputSize // pointer to the size of the output structure parameter.
    );
#endif

    if (kern_return == KERN_SUCCESS) {
        AtomList lst(outputStructure.x, outputStructure.y);
        lst.append(outputStructure.z);
        return lst;
    } else if (kern_return == kIOReturnBusy)
        OBJ_ERR << "device busy";
    else
        OBJ_ERR << "could not read device";

    IOServiceClose(io_connect);

    return AtomList();
#endif
}

void setup_hw_apple_sms()
{
    ObjectFactory<HwAppleSMS> obj("hw.apple_sms");
}
