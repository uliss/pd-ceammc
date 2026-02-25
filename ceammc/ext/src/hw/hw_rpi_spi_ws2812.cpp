#include "hw_rpi_spi_ws2812.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_HASH(breathe)
CEAMMC_DEFINE_HASH(bounce)
CEAMMC_DEFINE_HASH(collision)
CEAMMC_DEFINE_HASH(cycle)
CEAMMC_DEFINE_HASH(rainbow)

HwSpiWs2812::HwSpiWs2812(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    spi_ = addSpiBusProperty();
    cs_ = addSpiCsProperty();

    size_ = new IntProperty("@size", 16);
    size_->setInitOnly();
    size_->checkClosedRange(1, 4096);
    addProperty(size_);

    clear_on_exit_ = new BoolProperty("@clear", true);
    addProperty(clear_on_exit_);
}

HwSpiWs2812::~HwSpiWs2812()
{
    ceammc_hw_spi_ws2812_free(ws_);
}

void HwSpiWs2812::initDone()
{
    if (spi_->isNone())
        return;

    ws_ = ceammc_hw_spi_ws2812_new( //
        spi_->bus(),
        cs_->pin(),
        size_->value(),
        on_notify(),
        on_message(),
        clear_on_exit_->value());
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
    static const args::ArgChecker chk("FX:s=rainbow|breathe ARG:f[0,1]? START:i? LEN:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_led_fx fx {};

    switch (crc32_hash(lv.symbolAt(0, &s_))) {
    case hash_rainbow:
        fx = ceammc_hw_led_fx::Rainbow;
        break;
    case hash_breathe:
        fx = ceammc_hw_led_fx::Breathe;
        break;
    case hash_bounce:
        fx = ceammc_hw_led_fx::Bounce;
        break;
    case hash_collision:
        fx = ceammc_hw_led_fx::Collision;
        break;
    case hash_cycle:
        fx = ceammc_hw_led_fx::Cycle;
        break;
    default:
        break;
    }

    auto arg = lv.floatAt(1, 0);
    auto start = lv.intAt(2, 0);
    auto len = lv.intAt(3, size_->value());

    ceammc_hw_spi_ws2812_apply_fx(ws_, start, len, fx, arg);
}

void HwSpiWs2812::m_fx_next(t_symbol *s, const AtomListView &lv)
{
    ceammc_hw_spi_ws2812_fx_next(ws_);
}

void HwSpiWs2812::m_rotate(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("i");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_spi_ws2812_rotate(ws_, lv.intAt(0, 0));
}

void setup_hw_rpi_spi_ws2812()
{
    ObjectFactory<HwSpiWs2812> obj("hw.rpi.spi.ws2812");
    obj.addMethod("brightness", &HwSpiWs2812::m_brightness);
    obj.addMethod("clear", &HwSpiWs2812::m_clear);
    obj.addMethod("fill", &HwSpiWs2812::m_fill);
    obj.addMethod("flush", &HwSpiWs2812::m_flush);
    obj.addMethod("fx", &HwSpiWs2812::m_fx);
    obj.addMethod("fx_next", &HwSpiWs2812::m_fx_next);
    obj.addMethod("rotate", &HwSpiWs2812::m_rotate);
    obj.addMethod("set", &HwSpiWs2812::m_set);
    obj.addMethod("set_range", &HwSpiWs2812::m_set_range);
    obj.addMethod("set_slice", &HwSpiWs2812::m_set_slice);
}
