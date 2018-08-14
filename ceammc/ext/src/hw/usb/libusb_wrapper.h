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
#ifndef LIBUSB_WRAPPER_H
#define LIBUSB_WRAPPER_H

#include "../data/datatype_mlist.h"
#include "ceammc_atomlist.h"
#include <boost/variant.hpp>
#include <mutex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

typedef boost::variant<int, std::string> MapValue;
typedef std::unordered_map<std::string, MapValue> DeviceInfo;
typedef std::vector<DeviceInfo> DeviceInfoList;
typedef std::tuple<int, int, int> WatchInfo;
typedef std::vector<WatchInfo> WatcherList;

enum DeviceActionType {
    DEVICE_ADDED = 0,
    DEVICE_REMOVED
};

struct UsbInfo {
    int vid, pid, dev_class, bus, addr;
    std::string serial, description, manufaturer;
};

struct EventInfo {
    UsbInfo info;
    DeviceActionType action;
};

typedef std::vector<EventInfo> EventList;

namespace ceammc {
class LibUSB {
    WatcherList wlist_;
    EventList event_list_;
    std::mutex mutex_;
    int callback_handle_;
    bool valid_;

private:
    LibUSB();
    ~LibUSB();
    LibUSB(const LibUSB&);

public:
    static LibUSB& instance();

    DeviceInfoList listDevices();
    bool watch(int vid, int pid, int dev_class);
    void hotplugEvent(void* device, int event);
    bool hasEvents() const;
    EventList popEvents();
};
}

#endif // LIBUSB_WRAPPER_H
