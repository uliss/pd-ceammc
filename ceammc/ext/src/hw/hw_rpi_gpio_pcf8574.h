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
#ifndef HW_RPI_GPIO_PCF8574_H
#define HW_RPI_GPIO_PCF8574_H

#include "hw_rpi_device.h"

namespace ceammc {

class HwRpiGpioPcf8574 : public HwRpiDevice<ceammc_hw_pcf8574> {
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };

public:
    explicit HwRpiGpioPcf8574(const PdArgs& args);
    Device createDevice() final;
    bool notify(int code) final;

    void m_input(t_symbol* s, const AtomListView& lv);
    void m_output(t_symbol* s, const AtomListView& lv);
    void m_read_all(t_symbol* s, const AtomListView& lv);
    void m_read(t_symbol* s, const AtomListView& lv);
    void m_write_all(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_gpio_pcf8574();

} // namespace ceammc

#endif // HW_RPI_GPIO_PCF8574_H
