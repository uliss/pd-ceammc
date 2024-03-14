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

#include <memory>

#include "ceammc_pollthread_spsc.h"
using namespace ceammc;

struct HwGamepadRequestDevices { };
struct HwGamepadReplyDevice {
    std::string name;
    std::size_t id;
    std::uint16_t vid { 0 }, pid { 0 };
    bool is_connected { false };
};
struct HwGamepadButtonPressed {
    std::size_t id;
    std::uint8_t btn;
};
struct HwGamepadButtonReleased {
    std::size_t id;
    std::uint8_t btn;
};
struct HwGamepadButtonRepeated {
    std::size_t id;
    std::uint8_t btn;
};
struct HwGamepadButtonChanged {
    std::size_t id;
    float val;
    std::uint8_t btn;
};
struct HwGamepadAxisChanged {
    std::size_t id;
    float val;
    std::uint8_t axis;
};
struct HwGamepadConnected {
    std::size_t id;
};
struct HwGamepadDisconnected {
    std::size_t id;
};

using HwGamepadRequest = boost::variant<HwGamepadRequestDevices>;
using HwGamepadReply = boost::variant<HwGamepadReplyDevice,
    HwGamepadConnected,
    HwGamepadDisconnected,
    HwGamepadButtonPressed,
    HwGamepadButtonReleased,
    HwGamepadButtonRepeated,
    HwGamepadButtonChanged,
    HwGamepadAxisChanged>;

using HwGamepadBase = FixedSPSCObject<HwGamepadRequest, HwGamepadReply, BaseObject, 16, 20>;

struct ceammc_rs_hw_gamepad;

class HwGamepad : public HwGamepadBase {
    std::unique_ptr<ceammc_rs_hw_gamepad, void (*)(ceammc_rs_hw_gamepad*)> gp_;

public:
    HwGamepad(const PdArgs& args);
    ~HwGamepad();

    void initDone() final;

    void processRequest(const HwGamepadRequest& req, ResultCallback cb) final;
    void processResult(const HwGamepadReply& res) final;
    void runLoopFor(size_t ms) final;

    void onError(const char* msg);

    void m_devices(t_symbol* s, const AtomListView& lv);
};

void setup_hw_gamepad();

#endif // HW_GAMEPAD_H
