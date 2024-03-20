/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef WITH_GAMEPAD
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(HwGamepad, 1, 3, "compiled without gamepad support");
OBJECT_STUB_SETUP(HwGamepad, hw_gamepad, "hw.gamepad");
#else
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "hw_gamepad.h"
#include "hw_rust.h"

#include "fmt/core.h"

CEAMMC_DEFINE_SYM(south)
CEAMMC_DEFINE_SYM(east)
CEAMMC_DEFINE_SYM(north)
CEAMMC_DEFINE_SYM(west)
CEAMMC_DEFINE_SYM(c)
CEAMMC_DEFINE_SYM(z)
CEAMMC_DEFINE_SYM(ltrig)
CEAMMC_DEFINE_SYM(ltrig2)
CEAMMC_DEFINE_SYM(rtrig)
CEAMMC_DEFINE_SYM(rtrig2)
CEAMMC_DEFINE_SYM(select)
CEAMMC_DEFINE_SYM(start)
CEAMMC_DEFINE_SYM(mode)
CEAMMC_DEFINE_SYM(lthumb)
CEAMMC_DEFINE_SYM(rthumb)
CEAMMC_DEFINE_SYM(up)
CEAMMC_DEFINE_SYM(down)
CEAMMC_DEFINE_SYM(left)
CEAMMC_DEFINE_SYM(right)
CEAMMC_DEFINE_SYM(unknown)

CEAMMC_DEFINE_SYM(pressed)
CEAMMC_DEFINE_SYM(released)
CEAMMC_DEFINE_SYM(repeated)
CEAMMC_DEFINE_SYM(changed)
CEAMMC_DEFINE_SYM(connected)
CEAMMC_DEFINE_SYM(disconnected)
CEAMMC_DEFINE_SYM(device)

CEAMMC_DEFINE_SYM(lstickx)
CEAMMC_DEFINE_SYM(lsticky)
CEAMMC_DEFINE_SYM(rstickx)
CEAMMC_DEFINE_SYM(rsticky)
CEAMMC_DEFINE_SYM(leftz)
CEAMMC_DEFINE_SYM(rightz)
CEAMMC_DEFINE_SYM(x)
CEAMMC_DEFINE_SYM(y)

CEAMMC_DEFINE_SYM(button)
CEAMMC_DEFINE_SYM(axis)

CEAMMC_DEFINE_SYM(charged)
CEAMMC_DEFINE_SYM(charging)
CEAMMC_DEFINE_SYM(discharging)
CEAMMC_DEFINE_SYM(wired)

using namespace ceammc::gp;
using namespace ceammc::gp::reply;
using namespace ceammc::gp::req;

static t_symbol* btn2sym(ceammc_hw_gamepad_btn btn)
{
    using b = ceammc_hw_gamepad_btn;

    switch (btn) {
    case b::South:
        return sym_south();
    case b::East:
        return sym_east();
    case b::North:
        return sym_north();
    case b::West:
        return sym_west();
    case b::C:
        return sym_c();
    case b::Z:
        return sym_z();
    case b::LeftTrigger:
        return sym_ltrig();
    case b::LeftTrigger2:
        return sym_ltrig2();
    case b::RightTrigger:
        return sym_rtrig();
    case b::RightTrigger2:
        return sym_rtrig2();
    case b::Select:
        return sym_select();
    case b::Start:
        return sym_start();
    case b::Mode:
        return sym_mode();
    case b::LeftThumb:
        return sym_lthumb();
    case b::RightThumb:
        return sym_rthumb();
    case b::DPadUp:
        return sym_up();
    case b::DPadDown:
        return sym_down();
    case b::DPadLeft:
        return sym_left();
    case b::DPadRight:
        return sym_right();
    default:
        return sym_unknown();
    }
}

static t_symbol* axis2sym(ceammc_hw_gamepad_event_axis axis)
{
    using a = ceammc_hw_gamepad_event_axis;

    switch (axis) {
    case a::LeftStickX:
        return sym_lstickx();
    case a::LeftStickY:
        return sym_lsticky();
    case a::LeftZ:
        return sym_leftz();
    case a::RightStickX:
        return sym_rstickx();
    case a::RightStickY:
        return sym_rsticky();
    case a::RightZ:
        return sym_rightz();
    case a::DPadX:
        return sym_x();
    case a::DPadY:
        return sym_y();
    default:
        return sym_unknown();
    }
}

static t_symbol* power2sym(ceammc_hw_gamepad_powerstate power)
{
    using p = ceammc_hw_gamepad_powerstate;

    switch (power) {
    case p::Charged:
        return sym_charged();
    case p::Charging:
        return sym_charging();
    case p::Discharging:
        return sym_discharging();
    case p::Wired:
        return sym_wired();
    default:
        return sym_unknown();
    }
}

constexpr int OUT_BUTTON = 0;
constexpr int OUT_AXIS = 1;
constexpr int OUT_INFO = 2;

using MutexLock = std::lock_guard<std::mutex>;

GamepadImpl::GamepadImpl()
{
    MutexLock lock(mtx_);
    gp_ = ceammc_hw_gamepad_new(
        { this, on_error },
        { this, on_event },
        { this, on_devlist });
}

GamepadImpl::~GamepadImpl()
{
    MutexLock lock(mtx_);
    if (gp_)
        ceammc_hw_gamepad_free(gp_);
}

void GamepadImpl::process(const req::ListDevices& ev)
{
    MutexLock lock(mtx_);
    if (gp_) {
        auto rc = ceammc_hw_gamepad_list(gp_);

        if (rc != ceammc_hw_gamepad_rc::Ok && cb_err)
            cb_err("ceammc_hw_gamepad_list() failed");
    }
}

void GamepadImpl::processEvents(std::uint16_t ms)
{
    MutexLock lock(mtx_);
    if (gp_)
        ceammc_hw_gamepad_process_events(gp_, ms);
}

void GamepadImpl::on_error(void* user, const char* msg)
{
    auto this_ = static_cast<GamepadImpl*>(user);
    if (this_ && this_->cb_err)
        this_->cb_err(msg);
}

void GamepadImpl::on_event(void* user, const ceammc_hw_gamepad_event* ev)
{
    auto this_ = static_cast<GamepadImpl*>(user);
    if (this_ && this_->cb_event)
        this_->cb_event(*ev);
}

void GamepadImpl::on_devlist(void* user, const ceammc_gamepad_dev_info* info)
{
    auto this_ = static_cast<GamepadImpl*>(user);
    if (this_ && this_->cb_devlist) {
        Device dev;
        dev.vid = info->vid;
        dev.pid = info->pid;
        dev.name = info->name;
        dev.os_name = info->os_name;
        dev.id = info->id;
        dev.power = info->power;
        dev.connected = info->is_connected;
        dev.force_feedback = info->has_ff;
        this_->cb_devlist(dev);
    }
}

HwGamepad::HwGamepad(const PdArgs& args)
    : HwGamepadBase(args)
{
    createOutlet();
    createOutlet();
    createOutlet();

    gp_.reset(new GamepadImpl);
    gp_->cb_err = [this](const char* msg) { workerThreadError(msg); };
    gp_->cb_devlist = [this](const Device& dev) { addReply(dev); };
    gp_->cb_event = [this](const ceammc_hw_gamepad_event& ev) {
        using event = ceammc_hw_gamepad_event_type;

        if (waitForOutputAvailable() == WorkerProcess::QUIT)
            return;

        switch (ev.event) {
        case event::ButtonPressed:
            addReply(ButtonPressed { ev.id, ev.button });
            break;
        case event::ButtonRepeated:
            addReply(ButtonRepeated { ev.id, ev.button });
            break;
        case event::ButtonReleased:
            addReply(ButtonReleased { ev.id, ev.button });
            break;
        case event::ButtonChanged:
            addReply(ButtonChanged { ev.id, ev.value, ev.button });
            break;
        case event::AxisChanged:
            addReply(AxisChanged { ev.id, ev.value, ev.axis });
            break;
        case event::Connected:
            addReply(Connected {});
            break;
        case event::Disconnected:
            addReply(Disconnected {});
            break;
        default:
            break;
        }
    };
}

void HwGamepad::processRequest(const Request& req, ResultCallback cb)
{
    if (!gp_) {
        workerThreadError("invalid handle");
        return;
    }

    if (req.type() == typeid(ListDevices)) {
        gp_->process(boost::get<ListDevices>(req));
    } else {
        workerThreadError(fmt::format("unknown request: {}", req.type().name()));
    }
}

void HwGamepad::processResult(const Reply& res)
{
    auto& type = res.type();
    if (type == typeid(Device)) {
        auto& dev = boost::get<Device>(res);
        DictAtom info;
        info->insert("id", dev.id);
        info->insert("vendor", dev.vid);
        info->insert("product", dev.pid);
        info->insert("name", gensym(dev.name.c_str()));
        info->insert("os_name", gensym(dev.os_name.c_str()));
        info->insert("power", power2sym(dev.power.state));
        info->insert("power_value", dev.power.data);
        info->insert("connected", dev.connected);
        info->insert("force_feedback", dev.force_feedback);

        anyTo(OUT_INFO, sym_device(), info);
    } else if (type == typeid(ButtonPressed)) {
        auto& ev = boost::get<ButtonPressed>(res);
        AtomArray<4> data {
            ev.id,
            btn2sym(ev.btn),
            sym_pressed(),
            1,
        };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(ButtonReleased)) {
        auto& ev = boost::get<ButtonReleased>(res);
        AtomArray<4> data {
            ev.id,
            btn2sym(ev.btn),
            sym_released(),
            0.,
        };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(ButtonRepeated)) {
        auto& ev = boost::get<ButtonRepeated>(res);
        AtomArray<4> data {
            ev.id,
            btn2sym(ev.btn),
            sym_repeated(),
            0.,
        };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(ButtonChanged)) {
        auto& ev = boost::get<ButtonChanged>(res);
        AtomArray<4> data {
            ev.id,
            btn2sym(ev.btn),
            sym_changed(),
            ev.val,
        };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(Connected)) {
        auto& ev = boost::get<Connected>(res);
        anyTo(OUT_INFO, sym_connected(), ev.id);
    } else if (type == typeid(Disconnected)) {
        auto& ev = boost::get<Disconnected>(res);
        anyTo(OUT_INFO, sym_disconnected(), ev.id);
    } else if (type == typeid(AxisChanged)) {
        auto& ev = boost::get<AxisChanged>(res);
        AtomArray<3> data {
            ev.id,
            axis2sym(ev.axis),
            ev.val,
        };
        listTo(OUT_AXIS, data.view());
    } else {
        OBJ_ERR << "unknown typeid: " << type.name();
    }
}

void HwGamepad::processEvents()
{
    if (gp_)
        gp_->processEvents(POLL_TIME_MS);
}

void HwGamepad::m_devices(t_symbol* s, const AtomListView& lv)
{
    addRequest(ListDevices {});
}

void setup_hw_gamepad()
{
    ObjectFactory<HwGamepad> obj("hw.gamepad");
    obj.addMethod("devices", &HwGamepad::m_devices);
    obj.setXletsInfo(
        { "control inlet" },
        {
            "button: list ID BTN STATE VAL",
            "axis: list ID AXIS VAL",
            "any: connected ID STATE\n"
            "any: device INFO",
        });
}
#endif
