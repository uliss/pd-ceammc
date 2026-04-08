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
    : HwRpiDevice<ceammc_hw_pcf_8574>(&ceammc_hw_pcf8674_free, args)
{
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();
}

HwRpiGpioPcf8574::HwRpiDevice::Device HwRpiGpioPcf8574::createDevice()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus)) {
        OBJ_ERR << "i2c bus is not set";
        return nullDevice();
    }

    return {
        ceammc_hw_pcf8574_new(
            bus,
            i2c_addr_->value(),
            on_notify(),
            on_message(),
            {
                this,
                [](void* user, std::uint8_t mask, std::uint8_t state) {
                    auto obj = static_cast<HwRpiGpioPcf8574*>(user);
                    if (obj) {
                        auto sel = gensym("pin");
                        for (int i = 0; i < 8; i++) {
                            if (((0x1 << i) & mask))
                                obj->anyTo(0, sel, Atom((0x1 << i) & state));
                        }
                    }
                },
            }),
        &ceammc_hw_pcf8674_free,
    };
}

bool HwRpiGpioPcf8574::notify(int /*code*/)
{
    return ceammc_hw_pcf8574_process_reply(device());
}

/// @function "set all expander pins output value" {
///     #value byte "pins output value" {}
/// }
void HwRpiGpioPcf8574::m_set_all(t_symbol* s, const AtomListView& lv)
{
    m_set_all_args args;
    if (!args.parse_args(lv, this))
        return;

    // ceammc_hw_pcf_123 sd;
}

void setup_hw_rpi_gpio_pcf8574()
{
    ObjectFactory<HwRpiGpioPcf8574> obj("hw.rpi.gpio.pcf8574");
    obj.addMethod("set_all", &HwRpiGpioPcf8574::m_set_all);
}

} // namespace ceammc
