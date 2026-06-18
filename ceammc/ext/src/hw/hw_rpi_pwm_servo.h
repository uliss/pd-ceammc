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
#ifndef HW_RPI_PWM_SERVO_H
#define HW_RPI_PWM_SERVO_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rpi_device_props.h"
#include "hw_rust.hpp"

namespace ceammc {

class HwPvmServo : public HwRpiDevice<ceammc_hw_gpio> {
    FloatProperty* min_pulse_ { nullptr };
    FloatProperty* max_pulse_ { nullptr };
    FloatProperty* freq_ { nullptr };
    GpioPinProperty* pin_ { nullptr };
    t_float angle_ { 0 };

public:
    explicit HwPvmServo(const PdArgs& args);

    Device createDevice() override;
    bool notify(int) override;

    void m_angle(t_symbol* sel, const AtomListView& lv);
    void m_rotate(t_symbol* sel, const AtomListView& lv);

private:
    void setAngle(t_float angle_deg);
    void rotate(t_float angle_deg);
    t_float pulseValue() const;
    t_float pulsePeriod() const;
};

void setup_hw_rpi_pwm_servo();

} // namespace ceammc

#endif // HW_RPI_PWM_SERVO_H
