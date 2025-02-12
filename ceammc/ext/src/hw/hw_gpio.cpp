/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#include "hw_gpio.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwGpio::HwGpio(const PdArgs& args)
    : HwGpioBase(args)
{
    createOutlet();

    gpio_ = ceammc_hw_gpio_new(
        { this, [](void* data, const char* msg) {
             auto obj = static_cast<HwGpio*>(data);
             if (!obj)
                 return;

             Error(obj) << msg;
         } },
        { this, [](void* data, const char* msg) {
             auto obj = static_cast<HwGpio*>(data);
             if (!obj)
                 return;

             Debug(obj) << msg;
         } },
        { size_t(subscriberId()), [](size_t id) { Dispatcher::instance().send(NotifyMessage { id, 0 }); } }, //
        { this, on_pin_value }, //
        { this, [](void* user, const std::uint8_t* data, size_t len) {
             auto obj = static_cast<HwGpio*>(user);
             if (!obj)
                 return;

             AtomList atoms;
             for (size_t i = 0; i < len; i++)
                 atoms.append(Atom(data[i]));

             obj->anyTo(0, gensym("pins"), atoms);
         } } //
    );
}

HwGpio::~HwGpio()
{
    ceammc_hw_gpio_free(gpio_);
}

bool HwGpio::notify(int code)
{
    ceammc_hw_gpio_process_events(gpio_);
    return true;
}

void HwGpio::m_read(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_read_pin(gpio_, lv.intAt(0, 0));
}

void HwGpio::m_write(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b VALUE:B", lv, this))
        return;

    ceammc_hw_gpio_write_pin(gpio_, lv.intAt(0, 0), lv.intAt(1, 1));
}

void HwGpio::m_pull_up(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_set_bias(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_bias::PullUp);
}

void HwGpio::m_pull_down(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_set_bias(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_bias::PullDown);
}

void HwGpio::m_toggle(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_toggle_pin(gpio_, lv.intAt(0, 0));
}

void HwGpio::m_set_pwm(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b PERIOD:f>=0 WIDTH:f>=0", lv, this))
        return;

    ceammc_hw_gpio_set_pwm(gpio_, lv.intAt(0, 0), lv.floatAt(1, 1), lv.floatAt(2, 50));
}

void HwGpio::m_set_pwm_freq(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b FREQ:f>=0 DUTY:f>=0", lv, this))
        return;

    ceammc_hw_gpio_set_pwm_freq(gpio_, lv.intAt(0, 0), lv.floatAt(1, 1), lv.floatAt(2, 50));
}

void HwGpio::m_clear_pwm(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_clear_pwm(gpio_, lv.intAt(0, 0));
}

void HwGpio::m_input(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_set_mode(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_mode::Input);
}

void HwGpio::m_list_pins(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_gpio_list_pins(gpio_);
}

void HwGpio::m_output(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_set_mode(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_mode::Output);
}

void HwGpio::m_reset(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_reset_pin(gpio_, lv.intAt(0, 0));
}

void HwGpio::on_pin_value(void* data, std::uint8_t pin, bool value)
{
    auto obj = static_cast<HwGpio*>(data);
    if (!obj)
        return;

    AtomArray<2> atoms { t_float(pin), t_float(value) };
    obj->anyTo(0, gensym("pin"), atoms.view());
}

void setup_hw_gpio()
{
    ObjectFactory<HwGpio> obj("hw.gpio");
    obj.addMethod("read", &HwGpio::m_read);
    obj.addMethod("write", &HwGpio::m_write);
    obj.addMethod("toggle", &HwGpio::m_toggle);

    obj.addMethod("set_pwm", &HwGpio::m_set_pwm);
    obj.addMethod("set_pwm_freq", &HwGpio::m_set_pwm_freq);
    obj.addMethod("clear_pwm", &HwGpio::m_clear_pwm);

    obj.addMethod("input", &HwGpio::m_input);
    obj.addMethod("output", &HwGpio::m_output);
    obj.addMethod("reset", &HwGpio::m_reset);

    obj.addMethod("list_pins", &HwGpio::m_list_pins);

    obj.addMethod("pull_up", &HwGpio::m_pull_up);
    obj.addMethod("pull_down", &HwGpio::m_pull_down);
}
