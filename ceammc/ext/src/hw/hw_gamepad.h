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
#ifndef HW_GAMEPAD_H
#define HW_GAMEPAD_H

#include <boost/variant.hpp>
#include <functional>
#include <memory>

#include "ceammc_pollthread_spsc.h"
#include "hw_rust.h"
using namespace ceammc;

namespace ceammc {
namespace gp {
    namespace req {
        struct ListDevices { };
        using Request = boost::variant<ListDevices>;
    }
    namespace reply {
        struct Device {
            std::string name, os_name;
            std::size_t id;
            std::uint16_t vid { 0 }, pid { 0 };
            ceammc_hw_gamepad_powerinfo power { ceammc_hw_gamepad_powerstate::Unknown, 0 };
            bool connected { false }, force_feedback { false };
        };
        struct ButtonPressed {
            std::size_t id;
            ceammc_hw_gamepad_btn btn;
        };
        struct ButtonReleased {
            std::size_t id;
            ceammc_hw_gamepad_btn btn;
        };
        struct ButtonRepeated {
            std::size_t id;
            ceammc_hw_gamepad_btn btn;
        };
        struct ButtonChanged {
            std::size_t id;
            float val;
            ceammc_hw_gamepad_btn btn;
        };
        struct AxisChanged {
            std::size_t id;
            float val;
            ceammc_hw_gamepad_event_axis axis;
        };
        struct Connected {
            std::size_t id;
        };
        struct Disconnected {
            std::size_t id;
        };

        using Reply = boost::variant<Device,
            Connected,
            Disconnected,
            ButtonPressed,
            ButtonReleased,
            ButtonRepeated,
            ButtonChanged,
            AxisChanged>;
    }

    struct GamepadImpl {
        std::mutex mtx_;
        ceammc_hw_gamepad* gp_ { nullptr };
        std::function<void(const char*)> cb_err;
        std::function<void(const ceammc_hw_gamepad_event&)> cb_event;
        std::function<void(const reply::Device&)> cb_devlist;

        GamepadImpl();
        ~GamepadImpl();

        void process(const req::ListDevices& ev);
        void processEvents(std::uint16_t ms);

        static void on_error(void* user, const char* msg);
        static void on_event(void* user, const ceammc_hw_gamepad_event* ev);
        static void on_devlist(void* user, const ceammc_gamepad_dev_info* info);
    };
}
}

using HwGamepadBase = FixedSPSCObject<gp::req::Request, gp::reply::Reply, BaseObject, 32, 20>;

class HwGamepad : public HwGamepadBase {
    std::unique_ptr<gp::GamepadImpl> gp_;

public:
    HwGamepad(const PdArgs& args);

    void processRequest(const gp::req::Request& req, ResultCallback cb) final;
    void processResult(const gp::reply::Reply& res) final;
    void processEvents() final;

    void m_devices(t_symbol* s, const AtomListView& lv);
};

void setup_hw_gamepad();

#endif // HW_GAMEPAD_H
