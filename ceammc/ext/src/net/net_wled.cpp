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
#include "net_wled.h"
#include "ceammc_factory.h"
#include "ceammc_ranges.h"
#include "net_wled_args.hpp"

constexpr auto LED_SIZE_MIN = 1;
constexpr auto LED_SIZE_DEF = 16;
constexpr auto LED_SIZE_MAX = 490;
constexpr auto LED_SIZE_WARLS_MAX = 255;

namespace ceammc {

template <typename T>
bool process_rgb(ceammc_wled_color& c, const T& args)
{
    if (args.prop_color) {
        c.red = args.prop_color.color.red8();
        c.green = args.prop_color.color.green8();
        c.blue = args.prop_color.color.blue8();
        return true;
    } else if (args.prop_color8) {
        c.red = args.prop_color8.red;
        c.green = args.prop_color8.green;
        c.blue = args.prop_color8.blue;
        return true;
    } else
        return false;
}

using RelSlice = RelativeIndexSlice<t_int>;
using RelLSlice = RelativeLengthSlice<t_int>;

template <typename T>
bool process_slice(RelSlice::AbsSlice& slice, const T& args, size_t len, const BaseObject* obj = nullptr)
{
    if (args.prop_slice) {
        const RelSlice rel_slice(args.prop_slice.first, args.prop_slice.last, args.prop_slice.step);
        auto st = rel_slice.get(slice, len);
        if (st == RelativeSliceStatus::Ok) {
            return true;
        } else {
            Error(obj) << to_err_string(st, args.prop_slice.first, args.prop_slice.last, args.prop_slice.step, len);
            return false;
        }
    } else if (args.prop_lslice) {
        const RelLSlice rel_slice(args.prop_lslice.start, args.prop_lslice.length, args.prop_lslice.step);
        auto st = rel_slice.get(slice, len);
        if (st == RelativeSliceStatus::Ok) {
            return true;
        } else {
            Error(obj) << to_err_string(st, args.prop_lslice.start, args.prop_lslice.length, args.prop_lslice.step, len);
            return false;
        }
        return true;
    } else
        return false;
}

NetWledUdp::NetWledUdp(const PdArgs& args)
    : RustFfiObject<BaseObject, ceammc_wled_udp>(&ceammc_wled_udp_free, args)
{
    createOutlet();

    addr_ = new SymbolProperty("@addr", &s_);
    addProperty(addr_);

    port_ = new IntProperty("@port", ceammc_WLED_UDP_DEFAULT_PORT);
    addProperty(port_);

    size_ = new IntProperty("@size", LED_SIZE_DEF);
    size_->checkClosedRange(LED_SIZE_MIN, LED_SIZE_MAX);
    addProperty(size_);
}

bool NetWledUdp::notify(int code)
{
    return true;
}

void NetWledUdp::m_clear(t_symbol* s, const AtomListView& lv)
{
    if (!checkFfiObject(true, s))
        return;

    const ceammc_wled_color color {};
    ceammc_wled_udp_fill(ffiObject(), color, 0, size_->value(), 2);
}

/// @function "fill pixels with specified color" {
///  @color  ^(@color8) "RGB color"                         { #color color "fill color" {} }
///  @color8 ^(@color)  "int RGB color in [0..255] range"   {
///     #red   byte "red color component"   {}
///     #green byte "green color component" {}
///     #blue  byte "blue color component"  {}
///  }
///  @lslice !(@slice) "length-based pixel slice" {
///     #start  int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #length int ?   "slice length. If ommitted means position from the end of the buffer"  { default: 0, check: >0 }
///     #step   int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
///  @slice !(@lslice) "range-based pixel slice" {
///     #first int [1] "start index, can be negative. If negative: means position from the end of the buffer" {}
///     #last  int ?   "last index, can be negative. If negative: means position from the end of the buffer"  { default: -1 }
///     #step  int ?   "step between pixels"  { default: 1 check: > 0 }
///  }
/// }
void NetWledUdp::m_fill(t_symbol* s, const AtomListView& lv)
{
    m_fill_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!checkFfiObject(true, s))
        return;

    ceammc_wled_color color {};
    process_rgb(color, args);

    RelSlice::AbsSlice slice;
    if (process_slice(slice, args, size_->value(), this)) {
        if (slice.step == 1) {
            ceammc_wled_udp_fill(ffiObject(), color, slice.begin, slice.length, 2);
        } else {
            ceammc_wled_pixel_color pix {};
            pix.color.red = color.red;
            pix.color.green = color.green;
            pix.color.blue = color.blue;

            std::vector<ceammc_wled_pixel_color> pixels;
            pixels.reserve(slice.count());

            slice.foreach ([&pix, &pixels](size_t i) {
                pix.index = i;
                pixels.push_back(pix);
            });

            ceammc_wled_udp_set_pixels(ffiObject(), pixels.data(), pixels.size(), 2);
        }
    }

    if (!args.prop_lslice && !args.prop_slice) { // fill all
        ceammc_wled_udp_fill(ffiObject(), color, 0, size_->value(), 2);
    }
}

/// @function "set pixel color" {
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
void NetWledUdp::m_set_pixel(t_symbol* s, const AtomListView& lv)
{
    m_set_pixel_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!checkFfiObject(false, s)) {
        auto dev = ceammc_wled_udp_new(addr_->cstr(), port_->value(), on_notify(), on_message());
        if (dev)
            createObject(dev);
    }

    if (!checkFfiObject(true, s))
        return;

    ceammc_wled_pixel_color pixel {};
    pixel.index = args.pos;
    process_rgb(pixel.color, args);

    ceammc_wled_udp_set_pixels(ffiObject(), &pixel, 1, 2);
}

void setup_net_wled_udp()
{
    ObjectFactory<NetWledUdp> obj("net.wled");
    obj.addMethod("clear", &NetWledUdp::m_clear);
    obj.addMethod("fill", &NetWledUdp::m_fill);
    obj.addMethod("set_pixel", &NetWledUdp::m_set_pixel);
}

} // namespace ceammc
