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
#include "hw_rpi_spi.h"
#include "ceammc_factory.h"

namespace ceammc {

HwSpi::HwSpi(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_spi>(&ceammc_hw_spi_free, args)
{
    createOutlet();

    spi_ = addSpiBusProperty();
    cs_ = addSpiCsProperty();
}

bool HwSpi::notify(int code)
{
    return ceammc_hw_spi_process_reply(device());
}

HwSpi::HwRpiDevice::Device HwSpi::createDevice()
{
    return Device(ceammc_hw_spi_new(spi_->bus(),
                      cs_->pin(),
                      on_notify(),
                      on_message(),
                      {
                          static_cast<void*>(this),
                          [](void* user, bool result) {
                          },
                          [](void* user, const std::uint8_t* data, size_t size) {

                          },
                      }),
        &ceammc_hw_spi_free);
}

void setup_hw_rpi_spi()
{
    ObjectFactory<HwSpi> obj("hw.rpi.spi");
    obj.addAlias("hw.spi");
}

} // namespace ceammc
