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
#ifndef HW_RPI_NFC_PN532_H
#define HW_RPI_NFC_PN532_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"

namespace ceammc {
class HwRpiNfcPn532 : public HwRpiDevice<ceammc_hw_nfc_pn532> {
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };

public:
    explicit HwRpiNfcPn532(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;

    //    void m_poll(t_symbol* s, const AtomListView& lv);
    //    void m_address(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

} // namespace ceammc

void setup_hw_rpi_nfc_pn532();

#endif // HW_RPI_NFC_PN532_H
