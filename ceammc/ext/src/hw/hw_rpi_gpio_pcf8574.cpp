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
#include "hw_rpi_gpio_pcf8574.h"
#include "ceammc_factory.h"
#include "hw_rpi_gpio_pcf8574_args.hpp"

namespace ceammc {

HwRpiGpioPcf8574::HwRpiGpioPcf8574(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_pcf8574>(&ceammc_hw_pcf8674_free, args)
    , clk_ {
        ClockLambdaFunction { [this]() { impulse_off(0); } },
        ClockLambdaFunction { [this]() { impulse_off(1); } },
        ClockLambdaFunction { [this]() { impulse_off(2); } },
        ClockLambdaFunction { [this]() { impulse_off(3); } },
        ClockLambdaFunction { [this]() { impulse_off(4); } },
        ClockLambdaFunction { [this]() { impulse_off(5); } },
        ClockLambdaFunction { [this]() { impulse_off(6); } },
        ClockLambdaFunction { [this]() { impulse_off(7); } },
    }
{
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();
    irq_ = addGpioPinProperty("@irq");

    clear_on_close_ = new BoolProperty("@clear", true);
    addProperty(clear_on_close_);

    inverted_ = new BoolProperty("@inverted", true);
    addProperty(inverted_);
}

void HwRpiGpioPcf8574::impulse_off(std::uint8_t pin)
{
    ceammc_hw_pcf8674_write_pin(device(), pin, false);
}

HwRpiGpioPcf8574::HwRpiDevice::Device HwRpiGpioPcf8574::createDevice()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus)) {
        OBJ_ERR << "i2c bus is not set";
        return nullDevice();
    }

    std::uint8_t pin = irq_->value();
    auto pin_ptr = irq_->isNone() ? nullptr : &pin;

    return {
        ceammc_hw_pcf8574_new(
            bus,
            i2c_addr_->value(),
            pin_ptr,
            clear_on_close_->value(),
            on_notify(),
            on_message(),
            { this,
                [](void* user, std::uint8_t mask, std::uint8_t state) {
                    auto obj = static_cast<HwRpiGpioPcf8574*>(user);
                    if (obj) {
                        if (obj->inverted_->value())
                            state = ~state;

                        AtomArray<8> data;
                        for (int i = 0; i < 8; i++) {
                            if (((0x1 << i) & mask)) {
                                data[i] = Atom((0x1 << i) & state);
                            } else
                                data[i] = 0.0;
                        }
                        obj->anyTo(0, gensym("pins"), data.view());
                    }
                },
                [](void* user, std::uint8_t pin, bool state) {
                    auto obj = static_cast<HwRpiGpioPcf8574*>(user);
                    if (obj) {
                        if (obj->inverted_->value())
                            state = ~state;

                        AtomArray<2> data;
                        data[0] = pin;
                        data[1] = state;
                        obj->anyTo(0, gensym("pin"), data.view());
                    }
                } }),
        &ceammc_hw_pcf8674_free,
    };
}

bool HwRpiGpioPcf8574::notify(int /*code*/)
{
    return ceammc_hw_pcf8574_process_reply(device());
}

/// @function "send impulse of pecified length to pin" {
///     #pin    int   "pin index" { check: [0..7] }
///     #length float "pulse length" { check: [0.01..100] }
/// }
void HwRpiGpioPcf8574::m_impulse(t_symbol* s, const AtomListView& lv)
{
    m_impulse_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    if (ceammc_hw_pcf8674_write_pin(device(), args.pin, true)) {
        clk_[args.pin].delay(args.length);
    }
}

/// @function "write value to all pins configured for output" {
///     #value byte "pins output value" {}
/// }
void HwRpiGpioPcf8574::m_write_all(t_symbol* s, const AtomListView& lv)
{
    m_write_all_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_pcf8674_write_all(device(), args.value);
}

/// @function "write pin value" {
///     #pin   int  "pin index" { check: [0..7] }
///     #value bool "new value" {}
/// }
void HwRpiGpioPcf8574::m_write(t_symbol* s, const AtomListView& lv)
{
    m_write_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_pcf8674_write_pin(device(), args.pin, args.value);
}

/// @function "configure pin for input" {
///     #pin int "pin index" { check: [0..7] }
/// }
void HwRpiGpioPcf8574::m_input(t_symbol* s, const AtomListView& lv)
{
    m_input_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_pcf8674_config_pin(device(), args.pin, ceammc_hw_gpio_mode::Input);
}

/// @function "configure pin for output" {
///     #pin int "pin index" { check: [0..7] }
/// }
void HwRpiGpioPcf8574::m_output(t_symbol* s, const AtomListView& lv)
{
    m_output_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_pcf8674_config_pin(device(), args.pin, ceammc_hw_gpio_mode::Output)
        && ceammc_hw_pcf8674_write_pin(device(), args.pin, false);
}

/// @function "read all pins configured for input" {
/// }
void HwRpiGpioPcf8574::m_read_all(t_symbol* s, const AtomListView& lv)
{
    m_read_all_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_pcf8674_read_all(device());
}

/// @function "read pin value" {
///     #pin int "pin index" { check: [0..7] }
/// }
void HwRpiGpioPcf8574::m_read(t_symbol* s, const AtomListView& lv)
{
    m_read_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_pcf8674_read_pin(device(), args.pin);
}

void setup_hw_rpi_gpio_pcf8574()
{
    ObjectFactory<HwRpiGpioPcf8574> obj("hw.rpi.gpio.pcf8574");

    obj.addMethod("impulse", &HwRpiGpioPcf8574::m_impulse);
    obj.addMethod("input", &HwRpiGpioPcf8574::m_input);
    obj.addMethod("read", &HwRpiGpioPcf8574::m_read);
    obj.addMethod("read_all", &HwRpiGpioPcf8574::m_read_all);
    obj.addMethod("output", &HwRpiGpioPcf8574::m_output);
    obj.addMethod("write_all", &HwRpiGpioPcf8574::m_write_all);
    obj.addMethod("write", &HwRpiGpioPcf8574::m_write);
}

} // namespace ceammc
