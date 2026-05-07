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
#include "net_wled_args.hpp"

constexpr auto LED_SIZE_MIN = 1;
constexpr auto LED_SIZE_DEF = 16;
constexpr auto LED_SIZE_MAX = 490;
constexpr auto LED_SIZE_WARLS_MAX = 255;

namespace ceammc {

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

    ceammc_wled_color color {};
    color.red = 255;

    ceammc_wled_udp_fill(ffiObject(), color, 0, size_->value(), 2);
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
    if (args.prop_color) {
        pixel.color.red = args.prop_color.color.red8();
        pixel.color.green = args.prop_color.color.green8();
        pixel.color.blue = args.prop_color.color.blue8();
    } else if (args.prop_color8) {
        pixel.color.red = args.prop_color8.red;
        pixel.color.red = args.prop_color8.green;
        pixel.color.red = args.prop_color8.blue;
    }

    ceammc_wled_udp_set_pixels(ffiObject(), &pixel, 1, 2);
}

void setup_net_wled_udp()
{
    ObjectFactory<NetWledUdp> obj("net.wled");
    obj.addMethod("clear", &NetWledUdp::m_clear);
    obj.addMethod("set_pixel", &NetWledUdp::m_set_pixel);
}

} // namespace ceammc
