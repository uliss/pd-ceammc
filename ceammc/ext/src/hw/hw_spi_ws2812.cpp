#include "hw_spi_ws2812.h"
#include "args/argcheck.h"
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

    size_ = new IntProperty("@size", 16);
    size_->checkClosedRange(1, 4096);
    addProperty(size_);
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
        size_->value(),
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

void HwSpiWs2812::m_brightness(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("BRIGHT:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_set_brightness(ws_, lv.intAt(0, 0));
}

void HwSpiWs2812::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FLUSH:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_clear(ws_);
    if (lv.boolAt(0, true))
        ceammc_hw_spi_ws2812_flush(ws_);
}

void HwSpiWs2812::m_fill(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("R:b G:b B:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_fill(ws_, lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0));
}

void HwSpiWs2812::m_set(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("IDX:i>=0 R:b G:b B:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_set_color(ws_, lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0), lv.intAt(3, 0));
}

void HwSpiWs2812::m_flush(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_spi_ws2812_flush(ws_);
}

void HwSpiWs2812::m_rotate(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("i");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_rotate(ws_, lv.intAt(0, 0));
}

void setup_hw_spi_ws2812()
{
    ObjectFactory<HwSpiWs2812> obj("hw.spi.ws2812");
    obj.addMethod("brightness", &HwSpiWs2812::m_brightness);
    obj.addMethod("clear", &HwSpiWs2812::m_clear);
    obj.addMethod("fill", &HwSpiWs2812::m_fill);
    obj.addMethod("flush", &HwSpiWs2812::m_flush);
    obj.addMethod("rotate", &HwSpiWs2812::m_rotate);
    obj.addMethod("set", &HwSpiWs2812::m_set);
}
