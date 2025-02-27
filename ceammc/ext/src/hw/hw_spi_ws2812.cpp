#include "hw_spi_ws2812.h"
#include "ceammc_factory.h"

HwSpiWs2812::HwSpiWs2812(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();

    spi_ = new IntProperty("@spi", static_cast<int>(ceammc_hw_spi_bus::SPI0));
    spi_->setInitOnly();
    spi_->checkClosedRange(static_cast<int>(ceammc_hw_spi_bus::SPI0), static_cast<int>(ceammc_hw_spi_bus::SPI6));
    addProperty(spi_);

    cs_ = new IntProperty("@cs", static_cast<int>(ceammc_hw_spi_cs::CS0));
    cs_->setInitOnly();
    cs_->checkClosedRange(static_cast<int>(ceammc_hw_spi_cs::CS0), static_cast<int>(ceammc_hw_spi_cs::CS3));
    addProperty(cs_);
}

HwSpiWs2812::~HwSpiWs2812()
{
    ceammc_hw_spi_ws2812_free(ws_);
}

void HwSpiWs2812::initDone()
{
    ws_ = ceammc_hw_spi_ws2812_new( //
        static_cast<ceammc_hw_spi_bus>(spi_->value()),
        static_cast<ceammc_hw_spi_cs>(cs_->value()),
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }, //
        { this, [](void* user, const char* msg) {
             auto* obj = static_cast<HwSpiWs2812*>(user);
             Error(obj) << msg;
         } });
}

bool HwSpiWs2812::notify(int code)
{
    return true;
}

void HwSpiWs2812::m_write(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_spi_ws2812_write(ws_, 255, 0, 125);
}

void setup_hw_spi_ws2812()
{
    ObjectFactory<HwSpiWs2812> obj("hw.spi.ws2812");
}
