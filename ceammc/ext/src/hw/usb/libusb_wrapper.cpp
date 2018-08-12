/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "libusb_wrapper.h"
#include "ceammc_log.h"

#include <libusb.h>

using namespace ceammc;

static int hotplug_callback(libusb_context* ctx, libusb_device* device, libusb_hotplug_event event, void* user_data)
{
    LibUSB* ptr = static_cast<LibUSB*>(user_data);
    ptr->hotplugEvent(device, event);
    return 0;
}

LibUSB::LibUSB()
    : callback_handle_(0)
    , valid_(false)
{
    int r = libusb_init(nullptr);
    valid_ = (r == 0);

    if (r < 0) {
        LIB_ERR << "Failed to initialise libusb";
    }
}

LibUSB::~LibUSB()
{
    libusb_hotplug_deregister_callback(NULL, callback_handle_);
    libusb_exit(nullptr);
}

LibUSB& LibUSB::instance()
{
    static LibUSB instance_;
    return instance_;
}

DeviceInfoList LibUSB::listDevices()
{
    DeviceInfoList res;
    if (!valid_) {
        LIB_ERR << "invalid handle";
        return res;
    }

    libusb_device** devs;
    ssize_t cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return res;

    libusb_device* dev;
    int i = 0;

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            LIB_ERR << "failed to get device descriptor";
            return res;
        }

        libusb_device_handle* handle;
        int err = libusb_open(dev, &handle);
        if (err)
            continue;

        DeviceInfo info;
        info["bus"] = libusb_get_bus_number(dev);
        info["addr"] = libusb_get_device_address(dev);
        info["vendor"] = desc.idVendor;
        info["product"] = desc.idProduct;

        if (desc.iManufacturer) {
            char string[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, (unsigned char*)string, sizeof(string));
            if (ret > 0)
                info["manufacturer"] = string;
        }

        if (desc.iProduct) {
            char string[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, (unsigned char*)string, sizeof(string));
            if (ret > 0)
                info["description"] = string;
        }

        if (desc.iSerialNumber) {
            char string[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, (unsigned char*)string, sizeof(string));
            if (ret > 0)
                info["serial"] = string;
        }

        res.push_back(info);
        libusb_close(handle);
    }

    libusb_free_device_list(devs, 1);
    return res;
}

bool LibUSB::watch(int vid, int pid, int dev_class)
{
    if (!valid_) {
        LIB_ERR << "invalid handle";
        return false;
    }

    WatchInfo dev(vid, pid, dev_class);
    auto it = std::find(std::begin(wlist_), std::end(wlist_), dev);

    if (it != wlist_.end()) {
        LIB_ERR << "already watching device with vid=" << vid
                << ", pid=" << pid
                << ", dev_class=" << dev_class;

        return false;
    }

    // deregister previous
    libusb_hotplug_deregister_callback(NULL, callback_handle_);

    int rc = libusb_hotplug_register_callback(NULL,
        libusb_hotplug_event(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
        LIBUSB_HOTPLUG_NO_FLAGS,
        vid, pid, dev_class,
        hotplug_callback, this, &callback_handle_);

    if (rc != LIBUSB_SUCCESS) {
        LIB_ERR << "libusb: Error creating a hotplug callback - " << libusb_error_name(rc);
        return false;
    }

    wlist_.push_back(dev);
    return true;
}

void LibUSB::hotplugEvent(void* device, int event)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::cerr << "event: " << event << "\n";

    libusb_device* dev = static_cast<libusb_device*>(device);
    libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0)
        return;

    EventInfo ev;
    ev.info.vid = desc.idVendor;
    ev.info.pid = desc.idProduct;
    ev.info.bus = libusb_get_bus_number(dev);
    ev.info.addr = libusb_get_device_address(dev);

    switch (event) {
    case LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED: {
        ev.action = DEVICE_ADDED;

        // fill additional info
        libusb_device_handle* handle;
        int rc = libusb_open(dev, &handle);
        if (rc != LIBUSB_SUCCESS)
            break;

        if (desc.iManufacturer) {
            char string[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, (unsigned char*)string, sizeof(string));
            if (ret > 0)
                ev.info.manufaturer = string;
        }

        if (desc.iProduct) {
            char string[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, (unsigned char*)string, sizeof(string));
            if (ret > 0)
                ev.info.description = string;
        }

        if (desc.iSerialNumber) {
            char string[256];
            int ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, (unsigned char*)string, sizeof(string));
            if (ret > 0)
                ev.info.serial = string;
        }

        libusb_close(handle);

    } break;
    case LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT:
        ev.action = DEVICE_REMOVED;
        break;
    default:
        LIB_ERR << "unknown event: " << event;
        return;
    }

    event_list_.push_back(ev);
}

bool LibUSB::hasEvents() const
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    libusb_handle_events_timeout(NULL, &tv);
    return !event_list_.empty();
}

EventList LibUSB::popEvents()
{
    std::lock_guard<std::mutex> lock(mutex_);
    EventList res;
    res.swap(event_list_);
    return res;
}
