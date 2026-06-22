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
#include "hw_rpi_gpio_servo.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "hw_rpi_gpio_servo_args.hpp"

namespace {
constexpr t_float MIN_PULSE_MIN = 0.5;
constexpr t_float MIN_PULSE_DEF = 0.5125;
constexpr t_float MIN_PULSE_MAX = 1.5;
constexpr t_float MAX_PULSE_MIN = 1.5;
constexpr t_float MAX_PULSE_DEF = 2.125;
constexpr t_float MAX_PULSE_MAX = 3.0;

constexpr t_float MIN_FREQ = 30;
constexpr t_float MAX_FREQ = 100;
constexpr t_float DEF_FREQ = 50;

constexpr t_float TRAJECTORY_CALC_STEP = 20; // msec
constexpr t_float TRAJECTORY_VEL_DEF = 600;
constexpr t_float TRAJECTORY_VEL_MIN = 10;
constexpr t_float TRAJECTORY_ACC_DEF = 6000;
constexpr t_float TRAJECTORY_ACC_MIN = 10;
constexpr t_float TRAJECTORY_JERK_DEF = 60000;
constexpr t_float TRAJECTORY_JERK_MIN = 10;

constexpr int SERVO_RANGE = 180;
} // namespace

namespace ceammc {

HwRpiGpioServo::HwRpiGpioServo(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_gpio>(&ceammc_hw_gpio_free, args)
    , traj_(nullptr, &ceammc_hw_trajectory_free)
    , traj_clock_([this]() {
        if (!traj_)
            return;

        ceammc_hw_trajectory_set_target_pos(traj_.get(), angle_);

        switch (ceammc_hw_trajectory_update(traj_.get())) {
        case ceammc_hw_trajectory_result::Working: {
            double pos = 0, vel = 0, accel = 0, jerk = 0;
            if (ceammc_hw_trajectory_new_output(traj_.get(), &pos, &vel, &accel, &jerk)) {
                angle_ = pos;
                ceammc_hw_gpio_set_pwm(device(), pin_->value(), pulsePeriod(), pulseValue());

                OBJ_DBG << fmt::format("traj: pos = {}, vel = {}, accel = {}, jerk = {}", pos, vel, accel, jerk);
                traj_clock_.delay(TRAJECTORY_CALC_STEP);
            }
        } break;
        case ceammc_hw_trajectory_result::Finished: {
            double pos = 0, vel = 0, accel = 0;
            if (ceammc_hw_trajectory_current_input(traj_.get(), &pos, &vel, &accel)) {
                OBJ_DBG << fmt::format("traj done: pos = {}, vel = {}, accel = {}", pos, vel, accel);
            }
        } break;
        case ceammc_hw_trajectory_result::Error:
            OBJ_ERR << "traj calc error";
            break;
        case ceammc_hw_trajectory_result::NullPtr:
            OBJ_ERR << "null ptr";
            break;
        }
    })
{
    createOutlet();

    min_pulse_ = new FloatProperty("@min_pulse", MIN_PULSE_DEF);
    min_pulse_->checkClosedRange(MIN_PULSE_MIN, MIN_PULSE_MAX);
    min_pulse_->setUnitsMs();
    addProperty(min_pulse_);

    max_pulse_ = new FloatProperty("@max_pulse", MAX_PULSE_DEF);
    max_pulse_->checkClosedRange(MAX_PULSE_MIN, MAX_PULSE_MAX);
    max_pulse_->setUnitsMs();
    addProperty(max_pulse_);

    freq_ = new FloatProperty("@freq", DEF_FREQ);
    freq_->checkClosedRange(MIN_FREQ, MAX_FREQ);
    freq_->setUnitsHz();
    addProperty(freq_);

    pin_ = addGpioPinProperty("@pin");
    pin_->setArgIndex(0);

    smooth_traj_ = new BoolProperty("@smooth", true);
    addProperty(smooth_traj_);

    max_vel_ = new FloatProperty("@max_vel", TRAJECTORY_VEL_DEF);
    max_vel_->checkMinEq(TRAJECTORY_VEL_MIN);
    max_vel_->setSuccessFn([this](Property*) {
        ceammc_hw_trajectory_set_max_velocity(traj_.get(), max_vel_->value());
    });
    addProperty(max_vel_);

    max_acc_ = new FloatProperty("@max_accel", TRAJECTORY_ACC_DEF);
    max_acc_->checkMinEq(TRAJECTORY_ACC_MIN);
    max_acc_->setSuccessFn([this](Property*) {
        ceammc_hw_trajectory_set_max_accel(traj_.get(), max_acc_->value());
    });
    addProperty(max_acc_);

    max_jerk_ = new FloatProperty("@max_jerk", TRAJECTORY_JERK_DEF);
    max_jerk_->checkMinEq(TRAJECTORY_JERK_MIN);
    max_jerk_->setSuccessFn([this](Property*) {
        ceammc_hw_trajectory_set_max_jerk(traj_.get(), max_jerk_->value());
    });
    addProperty(max_jerk_);

    traj_.reset(ceammc_hw_trajectory_new(TRAJECTORY_CALC_STEP));
    ceammc_hw_trajectory_set_max_velocity(traj_.get(), TRAJECTORY_VEL_DEF);
    ceammc_hw_trajectory_set_max_accel(traj_.get(), TRAJECTORY_ACC_DEF);
    ceammc_hw_trajectory_set_max_jerk(traj_.get(), TRAJECTORY_JERK_DEF);
}

HwRpiGpioServo::Device HwRpiGpioServo::createDevice()
{
    if (pin_->isNone()) {
        OBJ_ERR << "pin is not specified";
        return nullDevice();
    }

    HwRpiGpioServo::Device dev {
        ceammc_hw_gpio_new(
            on_message(),
            on_notify(), //
            { this, nullptr }, //
            { this, nullptr }, //
            { subscriberId(), nullptr }),
        &ceammc_hw_gpio_free
    };

    auto gpio = dev.get();

    if (ceammc_hw_gpio_set_mode(gpio, pin_->value(), ceammc_hw_gpio_mode::Output))
        ceammc_hw_gpio_set_pwm(gpio, pin_->value(), pulsePeriod(), pulseValue());

    return dev;
}

bool HwRpiGpioServo::notify(int)
{
    ceammc_hw_gpio_process_events(device());
    return true;
}

/// @function "set absolute servo angle" {
///     #angle float "angle in degrees" { check: [0..180] }
/// }
void HwRpiGpioServo::m_angle_deg(t_symbol* sel, const AtomListView& lv)
{
    if (!check_connected(true, sel))
        return;

    m_angle_deg_args args;
    if (!args.parse_args(lv, this))
        return;

    setAngle(args.angle);
}

/// @function "set absolute servo angle" {
///     #angle float "angle in radians" { check: [0..3.1415926] }
/// }
void HwRpiGpioServo::m_angle_rad(t_symbol* sel, const AtomListView& lv)
{
    if (!check_connected(true, sel))
        return;

    m_angle_rad_args args;
    if (!args.parse_args(lv, this))
        return;

    setAngle(convert::rad2degree(args.angle));
}

/// @function "set absolute servo angle" {
///     #phase float "phase value" { check: [0..1] }
/// }
void HwRpiGpioServo::m_angle_phase(t_symbol* sel, const AtomListView& lv)
{
    if (!check_connected(true, sel))
        return;

    m_angle_phase_args args;
    if (!args.parse_args(lv, this))
        return;

    setAngle(convert::lin2lin_clip<t_float>(args.phase, 0, 1, 0, SERVO_RANGE));
}

/// @function "rotate current servo position" {
///     #angle float "angle in degrees" { check: [-180..180] }
/// }
void HwRpiGpioServo::m_rotate_deg(t_symbol* sel, const AtomListView& lv)
{
    if (!check_connected(true, sel))
        return;

    m_rotate_deg_args args;
    if (!args.parse_args(lv, this))
        return;

    rotate(args.angle);
}

/// @function "rotate current servo position" {
///     #angle float "angle in degrees" { check: [-3.1415926..3.1415926] }
/// }
void HwRpiGpioServo::m_rotate_rad(t_symbol* sel, const AtomListView& lv)
{
    if (!check_connected(true, sel))
        return;

    m_rotate_rad_args args;
    if (!args.parse_args(lv, this))
        return;

    rotate(convert::rad2degree(args.angle));
}

/// @function "rotate current servo position" {
///     #phase float "angle in degrees" { check: [-1..1] }
/// }
void HwRpiGpioServo::m_rotate_phase(t_symbol* sel, const AtomListView& lv)
{
    if (!check_connected(true, sel))
        return;

    m_rotate_phase_args args;
    if (!args.parse_args(lv, this))
        return;

    rotate(convert::lin2lin_clip<t_float>(args.phase, -1, 1, -SERVO_RANGE, SERVO_RANGE));
}

void HwRpiGpioServo::setAngle(t_float angle_deg)
{
    angle_ = angle_deg;

    if (smooth_traj_->value()) {
        traj_clock_.delay(TRAJECTORY_CALC_STEP);
    } else {
        ceammc_hw_gpio_set_pwm(device(), pin_->value(), pulsePeriod(), pulseValue());
    }
}

void HwRpiGpioServo::rotate(t_float angle_deg)
{
    setAngle(clip<t_float, 0, SERVO_RANGE>(angle_ + angle_deg));
}

t_float HwRpiGpioServo::pulseValue() const
{
    return convert::lin2lin_clip<t_float>(angle_, 0, SERVO_RANGE, min_pulse_->value(), max_pulse_->value());
}

t_float HwRpiGpioServo::pulsePeriod() const
{
    return 1000 / freq_->value();
}

void setup_hw_rpi_gpio_servo()
{
    ObjectFactory<HwRpiGpioServo> obj("hw.rpi.gpio.servo");
    obj.addAlias("hw.rpi.servo");

    obj.addMethod("angle", &HwRpiGpioServo::m_angle_deg);
    obj.addMethod("angle_rad", &HwRpiGpioServo::m_angle_rad);
    obj.addMethod("angle_phase", &HwRpiGpioServo::m_angle_phase);

    obj.addMethod("rotate", &HwRpiGpioServo::m_rotate_deg);
    obj.addMethod("rotate_rad", &HwRpiGpioServo::m_rotate_rad);
    obj.addMethod("rotate_phase", &HwRpiGpioServo::m_rotate_phase);
}

} // namespace ceammc
