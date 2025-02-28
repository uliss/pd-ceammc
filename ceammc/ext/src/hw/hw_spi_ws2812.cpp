#include "hw_spi_ws2812.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_HASH(rainbow)

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
    ceammc_hw_spi_ws2812_process_reply(ws_);
    return true;
}

void HwSpiWs2812::onBang()
{
    ceammc_hw_spi_ws2812_flush(ws_);
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

void HwSpiWs2812::m_set_range(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("START:i LEN:i>0 R:b G:b B:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_set_range(ws_, lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0), lv.intAt(3, 0), lv.intAt(4, 0));
}

void HwSpiWs2812::m_set_slice(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("START:i END:i STEP:i>0 R:b G:b B:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_set_slice(ws_, lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0), lv.intAt(3, 0), lv.intAt(4, 0), lv.intAt(5, 0));
}

void HwSpiWs2812::m_flush(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

void HwSpiWs2812::m_fx(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FX:s=rainbow ARG:f[0,1]? START:i? LEN:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_led_fx fx {};

    switch (crc32_hash(lv.symbolAt(0, &s_))) {
    case hash_rainbow:
        fx = ceammc_hw_led_fx::Rainbow;
        break;
    default:
        break;
    }

    auto arg = lv.floatAt(1, 0);
    auto start = lv.intAt(2, 0);
    auto len = lv.intAt(3, size_->value());

    ceammc_hw_spi_ws2812_apply_rx(ws_, start, len, fx, arg);
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
    obj.addMethod("fx", &HwSpiWs2812::m_fx);
    obj.addMethod("rotate", &HwSpiWs2812::m_rotate);
    obj.addMethod("set", &HwSpiWs2812::m_set);
    obj.addMethod("set_range", &HwSpiWs2812::m_set_range);
    obj.addMethod("set_slice", &HwSpiWs2812::m_set_slice);
}
