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

HwRpiGpio::HwRpiGpio(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();

    gpio_ = ceammc_hw_gpio_new(
        { this, [](void* data, const char* msg) {
             auto obj = static_cast<HwRpiGpio*>(data);
             if (!obj)
                 return;

             Error(obj) << msg;
         } },
        { this, [](void* data, const char* msg) {
             auto obj = static_cast<HwRpiGpio*>(data);
             if (!obj)
                 return;

             Debug(obj) << msg;
         } },
        { size_t(subscriberId()), [](size_t id) { Dispatcher::instance().send(NotifyMessage { id, 0 }); } }, //
        { this, on_pin_value }, //
        { this, [](void* user, const std::uint8_t* data, size_t len) {
             auto obj = static_cast<HwRpiGpio*>(user);
             if (!obj)
                 return;

             AtomList atoms;
             for (size_t i = 0; i < len; i++)
                 atoms.append(Atom(data[i]));

             obj->anyTo(0, gensym("pins"), atoms);
         } }, //
        { size_t(subscriberId()), [](size_t id, int data) { Dispatcher::instance().send(NotifyMessage { id, data }); } });
}

HwRpiGpio::~HwRpiGpio()
{
    ceammc_hw_gpio_free(gpio_);
}

bool HwRpiGpio::notify(int code)
{
    if (code != 0) {
        int pin = (code & 0xff);
        auto event = static_cast<ceammc_hw_gpio_trigger>((code & 0xf00) >> 8);

        AtomArray<2> atoms { static_cast<t_float>(pin), 0.0 };

        switch (event) {
        case ceammc_hw_gpio_trigger::RisingEdge:
            atoms[1] = t_float(1);
            break;
        default:
            atoms[1] = t_float(0);
            break;
        }

        anyTo(0, gensym("pin"), atoms.view());
        return true;
    }

    ceammc_hw_gpio_process_events(gpio_);
    return true;
}

void HwRpiGpio::m_read(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_read_pin(gpio_, lv.intAt(0, 0));
}

void HwRpiGpio::m_write(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b VALUE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_write_pin(gpio_, lv.intAt(0, 0), lv.intAt(1, 1));
}

void HwRpiGpio::m_pull_up(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_set_bias(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_bias::PullUp);
}

void HwRpiGpio::m_pull_down(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_set_bias(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_bias::PullDown);
}

void HwRpiGpio::m_start_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b DEBOUNCE:f>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_set_poll(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_trigger::Both, lv.floatAt(1, 10));
}

void HwRpiGpio::m_stop_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_clear_poll(gpio_, lv.intAt(0, 0));
}

void HwRpiGpio::m_toggle(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_toggle_pin(gpio_, lv.intAt(0, 0));
}

void HwRpiGpio::m_set_pwm(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b PERIOD:f>=0 WIDTH:f>=0");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_set_pwm(gpio_, lv.intAt(0, 0), lv.floatAt(1, 1), lv.floatAt(2, 50));
}

void HwRpiGpio::m_set_pwm_freq(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b FREQ:f>=0 DUTY:f>=0", lv, this))
        return;

    ceammc_hw_gpio_set_pwm_freq(gpio_, lv.intAt(0, 0), lv.floatAt(1, 1), lv.floatAt(2, 50));
}

void HwRpiGpio::m_clear_pwm(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_clear_pwm(gpio_, lv.intAt(0, 0));
}

void HwRpiGpio::m_input(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_set_mode(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_mode::Input);
}

void HwRpiGpio::m_list_pins(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_gpio_list_pins(gpio_);
}

void HwRpiGpio::m_output(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_set_mode(gpio_, lv.intAt(0, 0), ceammc_hw_gpio_mode::Output);
}

void HwRpiGpio::m_reset(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PIN:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_reset_pin(gpio_, lv.intAt(0, 0));
}

void HwRpiGpio::on_pin_value(void* data, std::uint8_t pin, bool value)
{
    auto obj = static_cast<HwRpiGpio*>(data);
    if (!obj)
        return;

    AtomArray<2> atoms { static_cast<t_float>(pin), static_cast<t_float>(value) };
    obj->anyTo(0, gensym("pin"), atoms.view());
}

void setup_hw_rpi_gpio()
{
    ObjectFactory<HwRpiGpio> obj("hw.gpio");
    obj.addMethod("read", &HwRpiGpio::m_read);
    obj.addMethod("write", &HwRpiGpio::m_write);
    obj.addMethod("toggle", &HwRpiGpio::m_toggle);

    obj.addMethod("set_pwm", &HwRpiGpio::m_set_pwm);
    obj.addMethod("set_pwm_freq", &HwRpiGpio::m_set_pwm_freq);
    obj.addMethod("clear_pwm", &HwRpiGpio::m_clear_pwm);

    obj.addMethod("input", &HwRpiGpio::m_input);
    obj.addMethod("output", &HwRpiGpio::m_output);
    obj.addMethod("reset", &HwRpiGpio::m_reset);

    obj.addMethod("list_pins", &HwRpiGpio::m_list_pins);

    obj.addMethod("pull_up", &HwRpiGpio::m_pull_up);
    obj.addMethod("pull_down", &HwRpiGpio::m_pull_down);

    obj.addMethod("start_poll", &HwRpiGpio::m_start_poll);
    obj.addMethod("stop_poll", &HwRpiGpio::m_stop_poll);
}
