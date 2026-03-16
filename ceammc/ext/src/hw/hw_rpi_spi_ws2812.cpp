#include "hw_rpi_spi_ws2812.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "datatype_color.h"

#include "hw_rpi_spi_ws2812_args.hpp"

constexpr int DEF_PIXEL_COUNT = 16;
constexpr int MIN_PIXEL_COUNT = 1;
constexpr int MAX_PIXEL_COUNT = 4096;

HwSpiWs2812::HwSpiWs2812(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_spi_ws2812>(&ceammc_hw_spi_ws2812_free, args)
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

bool HwSpiWs2812::notify(int code)
{
    ceammc_hw_spi_ws2812_process_reply(device());
    return true;
}

void HwSpiWs2812::onBang()
{
    if (!check_connected(true))
        return;

    ceammc_hw_spi_ws2812_flush(device());
}

/// @function "set led strip brighness" [{
///  #level byte "brightness level" {}
/// }]
void HwSpiWs2812::m_brightness(t_symbol* s, const AtomListView& lv)
{
    m_brightness_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true))
        return;

    ceammc_hw_spi_ws2812_set_brightness(device(), args.level);
}

/// @function "clear the internal pixel buffer and (optionally) flush it to the strip" [{
///  #flush bool ? "do flush" { default: true }
/// }]
void HwSpiWs2812::m_clear(t_symbol* s, const AtomListView& lv)
{
    m_clear_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true))
        return;

    ceammc_hw_spi_ws2812_clear(device());

    if (args.flush)
        ceammc_hw_spi_ws2812_flush(device());
}

/// @function "set pixel color in the internal buffer" {
///  #pos     int  "pixel position" { check: >= 0 }
///  @color8  ^@color "RGB int pixel color in range [0..255]" {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
///  @color   ^@color8 "float RGB color list, hex, named color or color datatype" {
///     #color color "pixel color" {}
///  }
/// }
void HwSpiWs2812::m_set_pixel(t_symbol* s, const AtomListView& lv)
{
    m_set_pixel_args args;
    if (!args.parse_args(lv, this))
        return;

    ceammc_hw_color_rgb8 color;
    if (args.prop_color._count) {
        color.red = args.prop_color.color.red8();
        color.green = args.prop_color.color.green8();
        color.blue = args.prop_color.color.blue8();
    } else if(args.prop_color8._count) {
        color.red = args.prop_color8.red;
        color.green = args.prop_color8.green;
        color.blue = args.prop_color8.blue;
    }

    if (!check_connected(true))
        return;

    ceammc_hw_spi_ws2812_set_pixel_color(device(), args.pos, color);
}

/// @function "fill all pixels in the internal buffer with specified color" {
///  @color  ^(@color8) "RGB color"                         { #color color "pixel color" {} }
///  @color8 ^(@color)  "int RGB color in [0..255] range"   {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
/// }
void HwSpiWs2812::m_fill(t_symbol* s, const AtomListView& lv)
{
    m_fill_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true))
        return;

    ceammc_hw_color_rgb8 color;
    if (args.prop_color._count) {
        color.red = args.prop_color.color.red8();
        color.green = args.prop_color.color.green8();
        color.blue = args.prop_color.color.blue8();
    } else if (args.prop_color8._count) {
        color.red = args.prop_color8.red;
        color.green = args.prop_color8.green;
        color.blue = args.prop_color8.blue;
    }

    ceammc_hw_spi_ws2812_fill_slice(device(), color, nullptr);
}

/// @function "fill the range of pixels in the internal buffer with specified color" {
///  @slice "range-based pixel slice" {
///     #first int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #last  int ?   "last index, can be negative. If negative: means position from the end of the buffer"  { default: -1 }
///     #step  int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
///  @color  ^(@color8) "RGB color"                         { #color color "" {} }
///  @color8 ^(@color)  "int RGB color in [0..255] range"   {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
/// }
void HwSpiWs2812::m_fill_slice(t_symbol* s, const AtomListView& lv)
{
    m_fill_slice_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true))
        return;

    ceammc_hw_color_rgb8 color;
    if (args.prop_color._count) {
        color.red = args.prop_color.color.red8();
        color.green = args.prop_color.color.green8();
        color.blue = args.prop_color.color.blue8();
    } else if (args.prop_color8._count) {
        color.red = args.prop_color8.red;
        color.green = args.prop_color8.green;
        color.blue = args.prop_color8.blue;
    }

    ceammc_hw_slice slice;
    const ceammc_hw_slice* slice_ptr = &slice;
    slice.first = args.prop_slice.first;
    slice.last = args.prop_slice.last;
    slice.step = args.prop_slice.step;

    ceammc_hw_spi_ws2812_fill_slice(device(), color, slice_ptr);
}

HwSpiWs2812::HwRpiDevice::Device HwSpiWs2812::createDevice()
{
    if (spi_->isNone(false)) {
        OBJ_ERR << "spi is not set";
        return nullDevice();
    }

    return Device(ceammc_hw_spi_ws2812_new( //
                      spi_->bus(),
                      cs_->pin(),
                      size_->value(),
                      on_notify(),
                      on_message(),
                      clear_on_exit_->value()),
        freeDeviceFn());
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
    } else if (lv.getProperty(gensym("@color8"), res)) {
        rgb.red = clip<int, 0, 255>(res.intAt(0, 0));
        rgb.green = clip<int, 0, 255>(res.intAt(1, 0));
        rgb.blue = clip<int, 0, 255>(res.intAt(2, 0));
        return true;
    }

    OBJ_ERR << "color property expected:"
               "\n  - @color R[0..1] G[0..1] B[0..1] or"
               "\n  - @color #RRGGBB or name or"
               "\n. - @color Color(...) or"
               "\n. - @color8 R[0..255] G[0..255] B[0..255]";

    return false;
}

bool HwSpiWs2812::parse_slice_property(ceammc_hw_slice& slice, const AtomListView& lv) const
{
    AtomListView res;

    auto this_ = const_cast<HwSpiWs2812*>(this);

    if (lv.getProperty(gensym("@slice"), res)) {
        static const args::ArgChecker rslice_chk("FIRST:i LAST:i? STEP:i>0?");
        if (!rslice_chk.check(res, this_, gensym("@slice")))
            return false;

        slice.first = res.intAt(0, 0);
        slice.last = res.intAt(1, -1);
        slice.step = res.intAt(2, 1);

        return true;
    } else if (lv.getProperty(gensym("@lslice"), res)) {
        static const args::ArgChecker lslice_chk("FIRST:i LENGTH:i>0? STEP:i>0?");
        if (!lslice_chk.check(res, this_, gensym("@lslice")))
            return false;

        slice.first = res.intAt(0, 0);
        slice.last = slice.first + res.intAt(1, size_->value()) - 1;
        slice.step = res.intAt(2, 1);

        return true;
    } else if (lv.getProperty(gensym("@indexes"), res)) {
        static const args::ArgChecker idx_chk("INDEXES:i+");
        if (!idx_chk.check(res, this_, gensym("@indexes")))
            return false;

        return false;
    }

    return false;
}

/**
 * Parse bits into static buffer
 */
bool HwSpiWs2812::parse_bits_property(ceammc_hw_bits& bits, std::uint8_t* const& buf, size_t buf_size, const AtomListView& lv) const
{
    AtomListView res;
    auto this_ = const_cast<HwSpiWs2812*>(this);

    bits.offset = 0;

    if (lv.getProperty(gensym("@offset"), res))
        bits.offset = res.asInt();

    if (lv.getProperty(gensym("@bits"), res)) {
        static const args::ArgChecker bits_chk("BITS:b+");
        if (!bits_chk.check(res, this_, gensym("@bits")))
            return false;

        auto N = std::min(buf_size, res.size());
        for (auto i = 0; i < N; i++)
            buf[i] = res[i].asBool();

        bits.data = &buf[0];
        bits.size = N;
        return true;
    }

    return false;
}

void HwSpiWs2812::m_flush(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

/// @function "apply fx to internal buffer" {
///  #fx_name symbol "effect name" {
///      enum: ##("rainbow", "breathe", "bounce", "collision", "cycle", "cylon", "fire", "meteor", "sparkle", "twinkle", "strobe")
///  }
///  @slice ^(@lslice) "range-based pixel slice" {
///     #first int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #last  int ?   "last index, can be negative. If negative: means position from the end of the buffer"  { default: -1 }
///     #step  int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
///  @lslice ^(@slice) "length-based pixel slice" {
///     #first  int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #length int ?   "length. If 0 or not specified: apply to all pixels"  { default: 0 check: >= 0 }
///     #step   int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
/// }
void HwSpiWs2812::m_fx(t_symbol* s, const AtomListView& lv)
{
    m_fx_args args;
    if (!args.parse_args(lv, this))
        return;

    ceammc_hw_led_fx fx {};

    switch (crc32_hash(args.fx_name)) {
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
        return;
    }

    ceammc_hw_slice slice;
    const ceammc_hw_slice* slice_ptr = &slice;
    if (!parse_slice_property(slice, lv))
        slice_ptr = nullptr;

    if (!check_connected(true))
        return;

    ceammc_hw_spi_ws2812_apply_fx(device(), fx, slice_ptr);
}

/// @function "rotate internal pixel buffer" [{
///  #n int "rotation amount" {}
/// }]
void HwSpiWs2812::m_rotate(t_symbol* s, const AtomListView& lv)
{
    m_rotate_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true))
        return;

    ceammc_hw_spi_ws2812_rotate(device(), lv.intAt(0, 1), nullptr);
}

void setup_hw_rpi_spi_ws2812()
{
    ObjectFactory<HwSpiWs2812> obj("hw.rpi.spi.ws2812");
    obj.addMethod("brightness", &HwSpiWs2812::m_brightness);
    obj.addMethod("clear", &HwSpiWs2812::m_clear);
    obj.addMethod("fill_slice", &HwSpiWs2812::m_fill_slice);
    obj.addMethod("fill", &HwSpiWs2812::m_fill);
    obj.addMethod("flush", &HwSpiWs2812::m_flush);
    obj.addMethod("fx", &HwSpiWs2812::m_fx);
    obj.addMethod("rotate", &HwSpiWs2812::m_rotate);
    obj.addMethod("set_pixel", &HwSpiWs2812::m_set_pixel);
}
