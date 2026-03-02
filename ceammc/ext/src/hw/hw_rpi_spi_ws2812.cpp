#include "hw_rpi_spi_ws2812.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_color.h"
#include "fmt/color.h"

CEAMMC_DEFINE_HASH(bounce)
CEAMMC_DEFINE_HASH(breathe)
CEAMMC_DEFINE_HASH(collision)
CEAMMC_DEFINE_HASH(cycle)
CEAMMC_DEFINE_HASH(cylon)
CEAMMC_DEFINE_HASH(fire)
CEAMMC_DEFINE_HASH(meteor)
CEAMMC_DEFINE_HASH(rainbow)
CEAMMC_DEFINE_HASH(sparkle)
CEAMMC_DEFINE_HASH(strobe)
CEAMMC_DEFINE_HASH(twinkle)

constexpr int DEF_PIXEL_COUNT = 16;
constexpr int MIN_PIXEL_COUNT = 1;
constexpr int MAX_PIXEL_COUNT = 4096;

HwSpiWs2812::HwSpiWs2812(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    spi_ = addSpiBusProperty();
    cs_ = addSpiCsProperty();

    size_ = new IntProperty("@size", DEF_PIXEL_COUNT);
    size_->setInitOnly();
    size_->checkClosedRange(MIN_PIXEL_COUNT, MAX_PIXEL_COUNT);
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
    if (spi_->isNone(false))
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
    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_flush(ws_);
}

void HwSpiWs2812::m_brightness(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("BRIGHT:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_set_brightness(ws_, lv.intAt(0, 0));
}

void HwSpiWs2812::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FLUSH:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_clear(ws_);

    if (lv.boolAt(0, true))
        ceammc_hw_spi_ws2812_flush(ws_);
}

void HwSpiWs2812::m_fill(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_color_rgb8 color;
    if (!parse_color_property(color, lv)) {
        METHOD_ERR(s) << "@color property not found in list: " << lv;
        return;
    }

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_set_slice_color(ws_, color, nullptr);
}

void HwSpiWs2812::m_set(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_color_rgb8 color;
    if (!parse_color_property(color, lv)) {
        METHOD_ERR(s) << "@color property not found in list: " << lv;
        return;
    }

    size_t idx = 0;
    if (!parse_pixel_index(idx, lv)) {
        METHOD_ERR(s) << "valid pixel index not found in list: " << lv;
        return;
    }

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_set_pixel_color(ws_, idx, color);
}

void HwSpiWs2812::m_set_slice(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_color_rgb8 color;
    if (!parse_color_property(color, lv)) {
        METHOD_ERR(s) << "@color property not found in list: " << lv;
        return;
    }

    ceammc_hw_slice slice;
    const ceammc_hw_slice* slice_ptr = &slice;
    if (!parse_slice_property(slice, lv))
        slice_ptr = nullptr;

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_set_slice_color(ws_, color, slice_ptr);
}

bool HwSpiWs2812::parse_color_property(ceammc_hw_color_rgb8& rgb, const AtomListView& lv) const
{
    AtomListView res;
    DataTypeColor c;
    if (lv.getProperty(gensym("@color"), res)
        && DataTypeColor::parseFromList(res, c)) {

        rgb.red = c.red8();
        rgb.green = c.green8();
        rgb.blue = c.blue8();

        return true;
    }

    OBJ_ERR << "color property expected:"
               "\n  - @color R[0..1] G[0..1] B[0..1] or"
               "\n  - @color #RRGGBB or name"
               "\n. - @color Color(...)";

    return false;
}

bool HwSpiWs2812::parse_slice_property(ceammc_hw_slice& slice, const AtomListView& lv) const
{
    AtomListView res;

    if (lv.getProperty(gensym("@slice"), res) || lv.getProperty(gensym("@rslice"), res)) {
        static const args::ArgChecker rslice_chk("FIRST:i LAST:i? STEP:i>0?");
        if (!rslice_chk.check(res, const_cast<HwSpiWs2812*>(this)))
            return false;

        slice.first = res.intAt(0, 0);
        slice.last = res.intAt(1, -1);
        slice.step = res.intAt(2, 1);

        return true;
    } else if (lv.getProperty(gensym("@lslice"), res)) {
        static const args::ArgChecker lslice_chk("FIRST:i LENGTH:i>0? STEP:i>0?");
        if (!lslice_chk.check(res, const_cast<HwSpiWs2812*>(this)))
            return false;

        slice.first = res.intAt(0, 0);
        slice.last = slice.first + res.intAt(1, size_->value());
        slice.step = res.intAt(2, 1);
    }

    return false;
}

bool HwSpiWs2812::parse_pixel_index(size_t& idx, const AtomListView& lv)
{
    if (lv.empty() || !lv[0].isIntGreaterEqual(0))
        return false;

    idx = lv[0].asInt();
    return true;
}

bool HwSpiWs2812::is_null_device(bool print_err) const
{
    if (spi_->isNone(print_err))
        return true;

    if (!ws_) {
        if (print_err)
            OBJ_ERR << "device is not initialized";

        return true;
    } else
        return false;
}

void HwSpiWs2812::m_flush(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

void HwSpiWs2812::m_fx(t_symbol* s, const AtomListView& lv)
{
    auto fx_name = lv.symbolAt(0, &s_);
    ceammc_hw_led_fx fx {};

    switch (crc32_hash(fx_name)) {
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
    case hash_cylon:
        fx = ceammc_hw_led_fx::Cylon;
        break;
    case hash_fire:
        fx = ceammc_hw_led_fx::Fire;
        break;
    case hash_meteor:
        fx = ceammc_hw_led_fx::Meteor;
        break;
    case hash_sparkle:
        fx = ceammc_hw_led_fx::SnowSparkle;
        break;
    case hash_twinkle:
        fx = ceammc_hw_led_fx::Twinkle;
        break;
    case hash_strobe:
        fx = ceammc_hw_led_fx::Strobe;
        break;
    default:
        METHOD_ERR(s) << fmt::format("unknown fx name: '{}'", fx_name->s_name);
        return;
    }

    ceammc_hw_slice slice;
    const ceammc_hw_slice* slice_ptr = &slice;
    if (!parse_slice_property(slice, lv))
        slice_ptr = nullptr;

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_apply_fx(ws_, fx, slice_ptr);
}

void HwSpiWs2812::m_rotate(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_slice slice;
    const ceammc_hw_slice* slice_ptr = &slice;
    if (!parse_slice_property(slice, lv))
        slice_ptr = nullptr;

    if (is_null_device(true))
        return;

    ceammc_hw_spi_ws2812_rotate(ws_, lv.intAt(0, 1), slice_ptr);
}

void setup_hw_rpi_spi_ws2812()
{
    ObjectFactory<HwSpiWs2812> obj("hw.rpi.spi.ws2812");
    obj.addMethod("brightness", &HwSpiWs2812::m_brightness);
    obj.addMethod("clear", &HwSpiWs2812::m_clear);
    obj.addMethod("fill", &HwSpiWs2812::m_fill);
    obj.addMethod("flush", &HwSpiWs2812::m_flush);
    obj.addMethod("fx", &HwSpiWs2812::m_fx);
    obj.addMethod("rotate", &HwSpiWs2812::m_rotate);
    obj.addMethod("set", &HwSpiWs2812::m_set);
    obj.addMethod("set_slice", &HwSpiWs2812::m_set_slice);
}
