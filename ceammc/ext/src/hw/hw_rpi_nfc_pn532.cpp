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
#include "hw_rpi_nfc_pn532.h"
#include "ceammc_factory.h"

using namespace ceammc;

HwRpiNfcPn532::HwRpiNfcPn532(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_nfc_pn532>(&ceammc_hw_nfc_pn532_free, args)
{
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();
}

bool HwRpiNfcPn532::notify(int code)
{
    return ceammc_hw_nfc_pn532_proc_reply(device());
}

void HwRpiNfcPn532::onBang()
{
    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_nfc_pn532_readall(device());
}

HwRpiNfcPn532::HwRpiDevice::Device HwRpiNfcPn532::createDevice()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus)) {
        OBJ_ERR << "i2c bus is not specified";
        return nullDevice();
    }

    return Device(ceammc_hw_pn532_new(bus,
                      i2c_addr_->value(),
                      on_notify(),
                      on_message(),
                      ceammc_hw_nfc_pn532_cb {
                          this,
                          [](void* user, std::uint16_t state) {
                              auto obj = static_cast<HwRpiNfcPn532*>(user);
                              if (obj)
                                  obj->floatTo(0, state);
                          },
                      }),
        freeDeviceFn());
}

void setup_hw_rpi_nfc_pn532()
{
    ObjectFactory<HwRpiNfcPn532> obj("hw.rpi.nfc.pn532");

    //    obj.addMethod("poll", &HwRpiSensorMpr121::m_poll);
    //    obj.addMethod("address", &HwRpiSensorMpr121::m_address);
}
