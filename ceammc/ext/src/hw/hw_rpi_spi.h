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
#ifndef HW_RPI_SPI_H
#define HW_RPI_SPI_H

#include "hw_rpi_device.h"

namespace ceammc {

class HwSpi : public HwRpiDevice<ceammc_hw_spi> {
    SpiBusProperty* spi_ { nullptr };
    SpiCsPinProperty* cs_ { nullptr };

public:
    explicit HwSpi(const PdArgs& args);
    bool notify(int code) final;
    Device createDevice() final;

    void m_transfer(t_symbol* s, const AtomListView& lv);
    void m_loopback_test(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_spi();

} // namespace ceammc

#endif // HW_RPI_SPI_H
