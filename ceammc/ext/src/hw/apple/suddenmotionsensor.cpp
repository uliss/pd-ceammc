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
#include "suddenmotionsensor.h"

#include <IOKit/IOKitLib.h>
#include <cassert>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unordered_map>

#include "ceammc_log.h"
using namespace ceammc;

namespace apple_sms {

struct SensorModelSpec {
    std::string sensor_name;
    SensorSpec spec;
};

typedef std::unordered_map<std::string, SensorSpec> SensorMap;
typedef std::unordered_map<std::string, SensorModelSpec> SensorModelMap;

// clang-format off
// data used from SMSLib
/*
 * smslib.m
 *
 * SMSLib Sudden Motion Sensor Access Library
 * Copyright (c) 2010 Suitable Systems
 * All rights reserved.
 *
 * Developed by: Daniel Griscom
 *               Suitable Systems
 *               http://www.suitable.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal with the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimers.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimers in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the names of Suitable Systems nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this Software without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
 *
 * For more information about SMSLib, see
 *		<http://www.suitable.com/tools/smslib.html>
 * or contact
 *		Daniel Griscom
 *		Suitable Systems
 *		1 Centre Street, Suite 204
 *		Wakefield, MA 01880
 *		(781) 665-0053
 *
 */
static const SensorModelMap sensors_model_info = {
    // The PowerBook5,6 is one of the G4 models that seems to lose
    // SMS access until the next reboot.
    {"PowerBook5,6",
        { "IOI2CMotionSensor", { 21, 60, {
            {true, 0, 1, 0,  51.5},
            {true, 1, 1, 0, -51.5},
            {true, 2, 1, 0, -51.5}
        }}}
    },
    // The PowerBook5,7 is one of the G4 models that seems to lose
    // SMS access until the next reboot.
    {"PowerBook5,7",
        { "IOI2CMotionSensor", { 21, 60, {
            {true, 0, 1, 0,  51.5},
            {true, 1, 1, 0,  51.5},
            {true, 2, 1, 0,  51.5}
        }}}
    },
    // Access seems to be reliable on the PowerBook5,8
    {"PowerBook5,8",
        { "PMUMotionSensor", { 21, 60, {
            {true, 0, 1, 0, -51.5},
            {true, 1, 1, 0,  51.5},
            {true, 2, 1, 0, -51.5}
        }}}
    },
    // Access seems to be reliable on the PowerBook5,9
    {"PowerBook5,9",
        { "PMUMotionSensor", { 21, 60, {
            {true, 0, 1, 0,  51.5},
            {true, 1, 1, 0, -51.5},
            {true, 2, 1, 0, -51.5}
        }}}
    },
    // The PowerBook6,7 is one of the G4 models that seems to lose
    // SMS access until the next reboot.
    {"PowerBook6,7",
        { "IOI2CMotionSensor", { 21, 60, {
            {true, 0, 1, 0,  51.5},
            {true, 1, 1, 0,  51.5},
            {true, 2, 1, 0,  51.5}
        }}}
    },
    // The PowerBook6,8 is one of the G4 models that seems to lose
    // SMS access until the next reboot.
    {"PowerBook6,8",
        { "IOI2CMotionSensor", { 21, 60, {
            {true, 0, 1, 0,  51.5},
            {true, 1, 1, 0,  51.5},
            {true, 2, 1, 0,  51.5}
        }}}
    },
    // MacBook Pro Core 2 Duo 17". Note the reversed Y and Z axes.
    {"MacBookPro2,1",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0,  251},
            {true, 2, 2, 0, -251},
            {true, 4, 2, 0, -251}
        }}}
    },
    // MacBook Pro Core 2 Duo 15" AND 17" with LED backlight, introduced June '07.
    // NOTE! The 17" machines have the signs of their X and Y axes reversed
    // from this calibration, but there's no clear way to discriminate between
    // the two machines.
    {"MacBookPro3,1",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, -251},
            {true, 2, 2, 0,  251},
            {true, 4, 2, 0, -251}
        }}}
    },
    // ... specs?
    {"MacBook5,2",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, -251},
            {true, 2, 2, 0,  251},
            {true, 4, 2, 0, -251}
        }}}
    },
    // ... specs?
    {"MacBookPro5,1",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, -251},
            {true, 2, 2, 0, -251},
            {true, 4, 2, 0,  251}
        }}}
    },
    // ... specs?
    {"MacBookPro5,2",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, -251},
            {true, 2, 2, 0, -251},
            {true, 4, 2, 0,  251}
        }}}
    },
    // This is speculative, based on a single user's report. Looks like the X and Y axes
    // are swapped. This is true for no other known Appple laptop.
    {"MacBookPro5,3",
        { "SMCMotionSensor", { 5, 40, {
            {true, 2, 2, 0, -251},
            {true, 0, 2, 0, -251},
            {true, 4, 2, 0, -251}
        }}}
    },
    // ... specs?
    {"MacBookPro5,4",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, -251},
            {true, 2, 2, 0, -251},
            {true, 4, 2, 0,  251}
        }}}
    },
    {"MacBookPro8,1",
        { "SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, 251},
            {true, 2, 2, 0, 251},
            {true, 4, 2, 264, -251}
        }}}
    }
};

static const SensorMap sensors_info = {
    {"IOI2CMotionSensor",  { 24, 60, {
            {true, 0, 1, 0, 51.5},
            {true, 1, 1, 0, 51.5},
            {true, 2, 1, 0, 51.5}
        }}
    },
    // PowerBook5,6 , PowerBook5,7 , PowerBook6,7 , PowerBook6,8
    // under OS X 10.4.*
    {"IOI2CMotionSensor", { 21, 60, {
            {true, 0, 1, 0, 51.5},
            {true, 1, 1, 0, 51.5},
            {true, 2, 1, 0, 51.5}
        }}
    },
    // PowerBook5,8 , PowerBook5,9 under OS X 10.4.*
    {"PMUMotionSensor", { 21, 60, {
            // Each has two out of three gains negative, but it's different
            // for the different models. So, this will be right in two out
            // of three axis for either model.
            {true, 0, 1,  0, -51.5},
            {true, 1, 1, -6, -51.5},
            {true, 2, 1,  0, -51.5}
        }}
    },
    // All MacBook, MacBookPro models. Hardware (at least on early MacBookPro 15")
    // is Kionix KXM52-1050 three-axis accelerometer chip. Data is at
    // http://kionix.com/Product-Index/product-index.htm. Specific MB and MBP models
    // that use this are:
    //		MacBook1,1
    //		MacBook2,1
    //		MacBook3,1
    //		MacBook4,1
    //		MacBook5,1
    //		MacBook6,1
    //		MacBookAir1,1
    //		MacBookPro1,1
    //		MacBookPro1,2
    //		MacBookPro4,1
    //		MacBookPro5,5
    {"SMCMotionSensor", { 5, 40, {
            {true, 0, 2, 0, 251},
            {true, 2, 2, 0, 251},
            {true, 4, 2, 264,  -251}
        }}
    }
};
// clang-format on

SuddenMotionSensor::SuddenMotionSensor()
    : model_name_(getModelName())
    , connection_(0)
    , spec_({ 0, 0, { { false, 0, 0, 0 }, { false, 0, 0, 0 }, { false, 0, 0, 0 } } })
{
    data_in_.fill(0);
    init();
}

SuddenMotionSensor::~SuddenMotionSensor()
{
    IOServiceClose(connection_);
}

static float getAxis(const AxisStruct& a, const char* data)
{
    int value = 0;

    // Although the values in the returned record should have the proper
    // endianness, we still have to get it into the proper end of value.
#if (BYTE_ORDER == BIG_ENDIAN)
    // On PowerPC processors
    memcpy(((char*)&value) + (sizeof(int) - a.nbytes), data + a.index, a.nbytes);
#endif
#if (BYTE_ORDER == LITTLE_ENDIAN)
    // On Intel processors
    memcpy(&value, data + a.index, a.nbytes);
#endif

    // Extend sign
    switch (a.nbytes) {
    case 1:
        if (value & 0x00000080)
            value |= 0xffffff00;
        break;
    case 2:
        if (value & 0x00008000)
            value |= 0xffff0000;
        break;
    case 3:
        if (value & 0x00800000)
            value |= 0xff000000;
        break;
    }
    return value;
}

AccelData SuddenMotionSensor::getRawAccel() const
{
    AccelData data = { 0, 0, 0 };
    if (!connection_) {
        LIB_ERR << "can't get connection";
        return data;
    }

#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1050
    const size_t in_size = spec_.record_size;
    size_t out_size = spec_.record_size;

    assert(in_size < data_in_.size());
    assert(out_size < data_out_.size());

    kern_return_t result = IOConnectCallStructMethod(connection_,
        spec_.kernel_fn, // magic kernel function number
        (const void*)data_in_.data(),
        in_size,
        (void*)data_out_.data(),
        &out_size);
#else
#error "MAC_OS_X_VERSION_MIN_REQUIRED >= 1050"
#endif // __MAC_OS_X_VERSION_MIN_REQUIRED 1050

    if (result != KERN_SUCCESS) {
        LIB_ERR << "can't read accel data with return code: " << result;
        return data;
    }

    std::get<0>(data) = getAxis(spec_.axes[0], data_out_.data());
    std::get<1>(data) = getAxis(spec_.axes[1], data_out_.data());
    std::get<2>(data) = getAxis(spec_.axes[2], data_out_.data());
    return data;
}

AccelData SuddenMotionSensor::getNormalizedAccel() const
{
    AccelData data = { 0, 0, 0 };
    if (!connection_) {
        LIB_ERR << "can't get connection";
        return data;
    }

    data = getRawAccel();
    std::get<0>(data) = (std::get<0>(data) - spec_.axes[0].zero_gravity) / spec_.axes[0].one_gravity;
    std::get<1>(data) = (std::get<1>(data) - spec_.axes[1].zero_gravity) / spec_.axes[1].one_gravity;
    std::get<2>(data) = (std::get<2>(data) - spec_.axes[2].zero_gravity) / spec_.axes[2].one_gravity;

    return data;
}

bool SuddenMotionSensor::isConnected() const
{
    return connection_ != 0;
}

void SuddenMotionSensor::init()
{
    // first try to find model info
    auto model_it = sensors_model_info.find(model_name_);
    if (model_it != sensors_model_info.end()) {
        if (connectTo(model_it->second.sensor_name)) {
            LIB_LOG << "connected to " << model_it->second.sensor_name << " on device " << model_it->first;
            spec_ = model_it->second.spec;
            return;
        }
    }

    // use common info
    for (auto& kv : sensors_info) {
        if (connectTo(kv.first)) {
            LIB_LOG << "connected to " << kv.first;
            spec_ = kv.second;
            return;
        }
    }

    LIB_ERR << "can't connect to sudden motion sensor";
}

bool SuddenMotionSensor::connectTo(const std::string& serviceName)
{
    io_iterator_t iterator;
    io_object_t device;

    CFMutableDictionaryRef dict = IOServiceMatching(serviceName.c_str());
    if (!dict) {
        LIB_LOG << "can't create matching dict for: " << serviceName;
        return false;
    }

    kern_return_t res = IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &iterator);
    if (res != KERN_SUCCESS) {
        LIB_LOG << "can't match service " << serviceName << " with return value: " << res;
        return false;
    }

    // first device in list
    device = IOIteratorNext(iterator);
    if (device == 0) {
        LIB_LOG << "can't get device for service: " << serviceName;
        IOObjectRelease(iterator);
        return false;
    }

    res = IOServiceOpen(device, mach_task_self(), 0, &connection_);
    IOObjectRelease(device);
    IOObjectRelease(iterator);

    if (res != KERN_SUCCESS) {
        LIB_LOG << "can't open device with return value: " << res;
        return false;
    } else if (connection_ == 0) {
        LIB_LOG << "device opened, but can't get connection";
        return false;
    } else
        return true;
}

std::string SuddenMotionSensor::getModelName()
{
    char model[32] = { 0 };
    size_t len = sizeof(model);
    int name[2] = { CTL_HW, HW_MODEL };

    int err = sysctl(name, 2, &model, &len, NULL, 0);
    if (err)
        LIB_ERR << "can't get model";

    return model;
}

}
