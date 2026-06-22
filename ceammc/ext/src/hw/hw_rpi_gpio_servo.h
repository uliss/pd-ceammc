/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#ifndef HW_RPI_GPIO_SERVO_H
#define HW_RPI_GPIO_SERVO_H

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rpi_device_props.h"
#include "hw_rust.hpp"

namespace ceammc {

class HwRpiGpioServo : public HwRpiDevice<ceammc_hw_gpio> {
    FloatProperty* min_pulse_ { nullptr };
    FloatProperty* max_pulse_ { nullptr };
    FloatProperty* freq_ { nullptr };
    GpioPinProperty* pin_ { nullptr };
    BoolProperty* smooth_traj_ { nullptr };
    FloatProperty* max_vel_ { nullptr };
    FloatProperty* max_acc_ { nullptr };
    FloatProperty* max_jerk_ { nullptr };
    t_float angle_ { 0 };

    using TrajPtr = std::unique_ptr<ceammc_hw_trajectory, decltype(&ceammc_hw_trajectory_free)>;
    TrajPtr traj_;
    ClockLambdaFunction traj_clock_;

public:
    explicit HwRpiGpioServo(const PdArgs& args);

    Device createDevice() override;
    bool notify(int) override;

    void m_angle_deg(t_symbol* sel, const AtomListView& lv);
    void m_angle_rad(t_symbol* sel, const AtomListView& lv);
    void m_angle_phase(t_symbol* sel, const AtomListView& lv);
    void m_rotate_deg(t_symbol* sel, const AtomListView& lv);
    void m_rotate_rad(t_symbol* sel, const AtomListView& lv);
    void m_rotate_phase(t_symbol* sel, const AtomListView& lv);

private:
    void setAngle(t_float angle_deg);
    void rotate(t_float angle_deg);
    t_float pulseValue() const;
    t_float pulsePeriod() const;
};

void setup_hw_rpi_gpio_servo();

} // namespace ceammc

#endif // HW_RPI_GPIO_SERVO_H
