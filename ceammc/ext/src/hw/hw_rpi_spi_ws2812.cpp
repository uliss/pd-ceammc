#include "hw_rpi_spi_ws2812.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

#include "hw_rpi_spi_ws2812_args.hpp"

constexpr int DEF_PIXEL_COUNT = 16;
constexpr int MIN_PIXEL_COUNT = 1;
constexpr int MAX_PIXEL_COUNT = 4096;

template <typename T>
void process_rgb(ceammc_hw_color_rgb8& c, const T& args)
{
    if (args.prop_color._count) {
        c.red = args.prop_color.color.red8();
        c.green = args.prop_color.color.green8();
        c.blue = args.prop_color.color.blue8();
    } else if (args.prop_color8._count) {
        c.red = args.prop_color8.red;
        c.green = args.prop_color8.green;
        c.blue = args.prop_color8.blue;
    }
}

template <typename T>
const ceammc_hw_slice* process_slice(ceammc_hw_slice& slice, size_t size, const T& args)
{
    const ceammc_hw_slice* slice_ptr = nullptr;

    if (args.prop_slice._count) {
        slice_ptr = &slice;
        slice.first = args.prop_slice.first;
        slice.last = args.prop_slice.last;
        slice.step = args.prop_slice.step;
    } else if (args.prop_lslice._count) {
        slice_ptr = &slice;
        slice.first = args.prop_lslice.start;
        if (args.prop_lslice.length < 1) {
            slice.last = -1;
        } else {
            slice.last = args.prop_lslice.start + size - 1;
        }

        slice.step = args.prop_lslice.step;
    }

    return slice_ptr;
}

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
    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_spi_ws2812_flush(device());
}

/// @function "set led strip brighness" [{
///  #level int "brightness level" { check: [0..25] }
/// }]
void HwSpiWs2812::m_brightness(t_symbol* s, const AtomListView& lv)
{
    m_brightness_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
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

    if (!check_connected(true, s))
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
    process_rgb(color, args);

    if (!check_connected(true, s))
        return;

    ceammc_hw_spi_ws2812_set_pixel_color(device(), args.pos, color);
}

/// @function "write list of color into the strip" {
///     #colors int + "list of pixel color in int24 format" {}
///     @offset? "start offset" { #value int "value" { default: 0 check: >=0 } }
/// }
void HwSpiWs2812::m_write(t_symbol* s, const AtomListView& lv)
{
    m_write_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    std::vector<std::uint32_t> colors;
    colors.reserve(args.colors.size());
    for (auto& a : args.colors) {
        colors.push_back(a.asInt());
    }

    ceammc_hw_spi_ws2812_write_pixels(device(), colors.data(), colors.size(), args.prop_offset.value);
}

/// @function "fill all pixels in the internal buffer with specified color" {
///  @color  ^(@color8) "RGB color"                         { #color color "fill color" {} }
///  @color8 ^(@color)  "int RGB color in [0..255] range"   {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
///  @lslice !(@slice) "length-based pixel slice" {
///     #start  int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #length int ?   "slice length. If ommitted means position from the end of the buffer"  { default: 0, check: >0 }
///     #step  int ?    "step between pixels"  { default: 1 check: > 0 }
///  }
///  @slice !(@lslice) "range-based pixel slice" {
///     #first int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #last  int ?   "last index, can be negative. If negative: means position from the end of the buffer"  { default: -1 }
///     #step  int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
/// }
void HwSpiWs2812::m_fill(t_symbol* s, const AtomListView& lv)
{
    m_fill_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_color_rgb8 color;
    process_rgb(color, args);

    ceammc_hw_slice slice;
    ceammc_hw_spi_ws2812_fill_slice(device(), color, process_slice(slice, size_->value(), args));
}

/// @function "fill only those pixels in the internal buffer with the specified color for which the corresponding bit in the input list is set to 1" {
///  @bits   "bit list" {
///     #list    bool + "list of 0|1" {}
///  }
///  @offset ? "bit list offset" {
///     #value   int "offset" { default: 0 check: >0 }
///  }
///  @color  ^(@color8) "RGB color"                         { #color color "" {} }
///  @color8 ^(@color)  "int RGB color in [0..255] range"   {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
/// }
void HwSpiWs2812::m_fill_bits(t_symbol* s, const AtomListView& lv)
{
    m_fill_bits_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_color_rgb8 color;
    process_rgb(color, args);

    boost::container::small_vector<std::uint8_t, 128> data;
    for (auto& a : args.prop_bits.list) {
        data.push_back(a.asBool());
    }

    ceammc_hw_bits bits;
    bits.data = data.data();
    bits.size = data.size();
    bits.offset = args.prop_offset.value;

    ceammc_hw_spi_ws2812_fill_bits(device(), color, &bits);
}

/// @function "fill the specified pixels with the specified color" {
///  @indexes "list of pixel positions" {
///     #list int + "pixel index, can be negative. If negative: means position from the end of the buffer" {}
///  }
///  @color  ^(@color8) "RGB color"                         { #color color "color" {} }
///  @color8 ^(@color)  "int RGB color in [0..255] range"   {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
/// }
void HwSpiWs2812::m_fill_pixels(t_symbol* s, const AtomListView& lv)
{
    m_fill_pixels_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_color_rgb8 color { 0, 0, 0 };
    process_rgb(color, args);

    boost::container::small_vector<std::int32_t, 128> data;
    for (auto& a : args.prop_indexes.list) {
        data.push_back(a.asInt());
    }

    ceammc_hw_indexes pixels;
    pixels.data = data.data();
    pixels.size = data.size();

    ceammc_hw_spi_ws2812_fill_pixels(device(), color, &pixels);
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

/// @function "send internal pixel buffer to the device" {
/// }
void HwSpiWs2812::m_flush(t_symbol* s, const AtomListView& lv)
{
    m_flush_args args;
    if (!args.parse_args(lv, this))
        return;

    onBang();
}

/// @function "apply fx to internal buffer" {
///  #fx_name symbol "effect name" {
///      enum: ##("rainbow", "breathe", "bounce", "collision", "cycle", "cylon", "fire", "meteor", "sparkle", "twinkle", "strobe")
///  }
///  @slice !(@lslice) "range-based pixel slice" {
///     #first int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #last  int ?   "last index, can be negative. If negative: means position from the end of the buffer"  { default: -1 }
///     #step  int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
///  @lslice !(@slice) "length-based pixel slice" {
///     #start  int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
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
    ceammc_hw_spi_ws2812_apply_fx(device(), fx, process_slice(slice, size_->value(), args));
}

/// @function "rotate internal pixel buffer" [{
///  #n int "rotation amount" {}
/// }]
void HwSpiWs2812::m_rotate(t_symbol* s, const AtomListView& lv)
{
    m_rotate_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, s))
        return;

    ceammc_hw_spi_ws2812_rotate(device(), lv.intAt(0, 1), nullptr);
}

void setup_hw_rpi_spi_ws2812()
{
    ObjectFactory<HwSpiWs2812> obj("hw.rpi.spi.ws2812");
    obj.addMethod("brightness", &HwSpiWs2812::m_brightness);
    obj.addMethod("clear", &HwSpiWs2812::m_clear);
    obj.addMethod("fill", &HwSpiWs2812::m_fill);
    obj.addMethod("fill_bits", &HwSpiWs2812::m_fill_bits);
    obj.addMethod("fill_pixels", &HwSpiWs2812::m_fill_pixels);
    obj.addMethod("flush", &HwSpiWs2812::m_flush);
    obj.addMethod("fx", &HwSpiWs2812::m_fx);
    obj.addMethod("rotate", &HwSpiWs2812::m_rotate);
    obj.addMethod("set_pixel", &HwSpiWs2812::m_set_pixel);
    obj.addMethod("write", &HwSpiWs2812::m_write);
}
