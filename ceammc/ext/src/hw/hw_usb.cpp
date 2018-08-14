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
#include "hw_usb.h"
#include "../data/datatype_dict.h"
#include "ceammc_factory.h"
#include "usb/libusb_wrapper.h"

static t_symbol* SYM_SEP = gensym(":");
static t_symbol* SYM_OPEN = gensym("(");
static t_symbol* SYM_CLOSE = gensym(")");
static t_symbol* SYM_CONNECTED = gensym("connected");
static t_symbol* SYM_DISCONNECTED = gensym("disconnected");
static char QUOTE = '`';

static int POLL_TIME_MS = 100;

HwUsb::HwUsb(const PdArgs& args)
    : BaseObject(args)
    , clock_(this, &HwUsb::tick)
{
    createOutlet();
}

void HwUsb::m_list(t_symbol* s, const AtomList& lst)
{
    auto devs = LibUSB::instance().listDevices();

    DataTypeDict dict;

    for (auto& dev : devs) {
        for (auto& kv : dev) {

            if (kv.second.type() == typeid(std::string)) {
                auto& str = boost::get<std::string>(kv.second);
                dict.insert(kv.first, str);
            } else if (kv.second.type() == typeid(int)) {
                auto num = boost::get<int>(kv.second);
                dict.insert(kv.first, t_float(num));
            }
        }
    }

    dataTo(0, DataPtr(dict.clone()));
}

void HwUsb::m_watch(t_symbol* s, const AtomList& lst)
{
    int vendor_id = lst.intAt(0, -1);
    int product_id = lst.intAt(1, -1);
    int class_id = lst.intAt(2, -1);
    LibUSB::instance().watch(vendor_id, product_id, class_id);

    clock_.delay(POLL_TIME_MS);
}

static AtomList toList(const EventInfo& ev)
{
    AtomList lst;
    lst.append(ev.info.bus);
    lst.append(ev.info.addr);
    lst.append(ev.info.vid);
    lst.append(ev.info.pid);

    if (!ev.info.manufaturer.empty())
        lst.append(gensym(ev.info.manufaturer.c_str()));

    if (!ev.info.description.empty())
        lst.append(gensym(ev.info.description.c_str()));

    if (!ev.info.serial.empty())
        lst.append(gensym(ev.info.serial.c_str()));

    return lst;
}

void HwUsb::tick()
{
    clock_.delay(POLL_TIME_MS);

    auto& lib = LibUSB::instance();
    if (!lib.hasEvents())
        return;

    auto info = lib.popEvents();
    for (auto& event : info) {
        switch (event.action) {
        case DEVICE_ADDED: {
            anyTo(0, SYM_CONNECTED, toList(event));
        } break;
        case DEVICE_REMOVED: {
            anyTo(0, SYM_DISCONNECTED, toList(event));
        } break;
        }
    }
}

void setup_hw_usb()
{
    ObjectFactory<HwUsb> obj("hw.usb");
    obj.addMethod("list", &HwUsb::m_list);
    obj.addMethod("watch", &HwUsb::m_watch);
}
