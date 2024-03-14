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
#include "hw_gamepad.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "hw_rust.h"

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

static void err_cb(void* data, const char* err)
{
    if (!data || !err)
        return;

    auto hw = static_cast<HwGamepad*>(data);
    hw->onError(err);
}

static t_symbol* btn2sym(std::uint8_t btn)
{
    using b = ceammc_rs_hw_gamepad_btn;

    switch (static_cast<ceammc_rs_hw_gamepad_btn>(btn)) {
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

static t_symbol* axis2sym(std::uint8_t axis)
{
    using a = ceammc_rs_hw_gamepad_event_axis;

    switch (static_cast<ceammc_rs_hw_gamepad_event_axis>(axis)) {
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

constexpr int OUT_BUTTON = 0;
constexpr int OUT_AXIS = 1;
constexpr int OUT_INFO = 2;

HwGamepad::HwGamepad(const PdArgs& args)
    : HwGamepadBase(args)
    , gp_(nullptr, ceammc_rs_hw_gamepad_free)
{
    createOutlet();
    createOutlet();
    createOutlet();

    if (!runTask()) {
        OBJ_ERR << "can't run worker process";
    }
}

void HwGamepad::initDone()
{
    gp_.reset(ceammc_rs_hw_gamepad_new(this, err_cb));
}

void HwGamepad::processRequest(const HwGamepadRequest& req, ResultCallback cb)
{
    if (!gp_) {
        workerThreadError("invalid handle");
        return;
    }

    if (req.type() == typeid(HwGamepadRequestDevices)) {
        auto rc = ceammc_rs_hw_gamepad_list(
            gp_.get(),
            [](void* data, const char* name, std::size_t id, //
                std::uint16_t vid, std::uint16_t pid, bool is_connected, //
                const ceammc_rs_hw_gamepad_powerinfo* pwinfo) {
                auto cb = static_cast<ResultCallback*>(data);
                if (!cb || !*cb)
                    return;

                HwGamepadReplyDevice dev;
                dev.vid = vid;
                dev.pid = pid;
                dev.name = name;
                dev.is_connected = is_connected;
                dev.id = id;
                (*cb)(dev);
            },
            &cb);

        if (rc != ceammc_rs_hw_gamepad_rc::Ok)
            workerThreadError("ceammc_rs_hw_gamepad_list() failed");
    }
}

HwGamepad::~HwGamepad() = default;

void HwGamepad::processResult(const HwGamepadReply& res)
{
    auto& type = res.type();
    if (type == typeid(HwGamepadReplyDevice)) {
        auto& dev = boost::get<HwGamepadReplyDevice>(res);
        AtomArray<5> data { dev.id, dev.vid, dev.pid, gensym(dev.name.c_str()), dev.is_connected };
        anyTo(OUT_INFO, sym_device(), data.view());
    } else if (type == typeid(HwGamepadButtonPressed)) {
        auto& ev = boost::get<HwGamepadButtonPressed>(res);
        AtomArray<4> data { ev.id, btn2sym(ev.btn), sym_pressed(), 1 };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(HwGamepadButtonReleased)) {
        auto& ev = boost::get<HwGamepadButtonReleased>(res);
        AtomArray<4> data { ev.id, btn2sym(ev.btn), sym_released(), 0. };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(HwGamepadButtonRepeated)) {
        auto& ev = boost::get<HwGamepadButtonRepeated>(res);
        AtomArray<4> data { ev.id, btn2sym(ev.btn), sym_repeated(), 0. };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(HwGamepadButtonChanged)) {
        auto& ev = boost::get<HwGamepadButtonChanged>(res);
        AtomArray<4> data { ev.id, btn2sym(ev.btn), sym_changed(), ev.val };
        listTo(OUT_BUTTON, data.view());
    } else if (type == typeid(HwGamepadConnected)) {
        auto& ev = boost::get<HwGamepadConnected>(res);
        AtomArray<2> data { ev.id, 1 };
        anyTo(OUT_INFO, sym_connected(), data.view());
    } else if (type == typeid(HwGamepadDisconnected)) {
        auto& ev = boost::get<HwGamepadDisconnected>(res);
        AtomArray<2> data { ev.id, 0. };
        anyTo(OUT_INFO, sym_connected(), data.view());
    } else if (type == typeid(HwGamepadAxisChanged)) {
        auto& ev = boost::get<HwGamepadAxisChanged>(res);
        AtomArray<3> data { ev.id, axis2sym(ev.axis), ev.val };
        listTo(OUT_AXIS, data.view());
    } else {
        OBJ_ERR << "unknown typeid: " << type.name();
    }
}

void HwGamepad::runLoopFor(size_t ms)
{
    if (!gp_)
        return;

    ceammc_rs_hw_gamepad_runloop(
        gp_.get(), [](void* user, const ceammc_rs_hw_gamepad_event* ev) {
            if (!ev || !user)
                return;

            auto this_ = static_cast<HwGamepad*>(user);

            switch (ev->event) {
            case ceammc_rs_hw_gamepad_event_type::ButtonPressed:
                this_->addReply(HwGamepadButtonPressed { ev->id, (std::uint8_t)ev->button });
                break;
            case ceammc_rs_hw_gamepad_event_type::ButtonRepeated:
                this_->addReply(HwGamepadButtonRepeated { ev->id, (std::uint8_t)ev->button });
                break;
            case ceammc_rs_hw_gamepad_event_type::ButtonReleased:
                this_->addReply(HwGamepadButtonReleased { ev->id, (std::uint8_t)ev->button });
                break;
            case ceammc_rs_hw_gamepad_event_type::ButtonChanged:
                this_->addReply(HwGamepadButtonChanged { ev->id, ev->value, (std::uint8_t)ev->button });
                break;
            case ceammc_rs_hw_gamepad_event_type::AxisChanged:
                this_->addReply(HwGamepadAxisChanged { ev->id, ev->value, (std::uint8_t)ev->axis });
                break;
            case ceammc_rs_hw_gamepad_event_type::Connected:
                this_->addReply(HwGamepadConnected {});
                break;
            case ceammc_rs_hw_gamepad_event_type::Disconnected:
                this_->addReply(HwGamepadDisconnected {});
                break;
            default:
                break;
            }
        },
        this);
}

void HwGamepad::onError(const char* msg)
{
    OBJ_ERR << msg;
}

void HwGamepad::m_devices(t_symbol* s, const AtomListView& lv)
{
    addRequest(HwGamepadRequestDevices {});
}

void setup_hw_gamepad()
{
    ObjectFactory<HwGamepad> obj("hw.gamepad");
    obj.addMethod("devices", &HwGamepad::m_devices);
    obj.setXletsInfo({ "control inlet" }, { "button: list ID BTN STATE VAL", "axis: list ID AXIS VAL", "info: connected, dis" });
}
