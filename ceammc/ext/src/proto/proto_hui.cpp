/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "proto_hui.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cstdint>

constexpr int MAX_CHAN = 0x07;
constexpr int MAX_ZONE = 0x1D;
constexpr int MAX_PORT = 0x07;

constexpr int SIDE_LEFT = 0;
constexpr int SIDE_RIGHT = 1;

constexpr int HUI_SIDE_RIGHT = 0x10;
constexpr int HUI_SIDE_LEFT = 0x00;

ProtoHUI::ProtoHUI(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ProtoHUI::onFloat(t_float f)
{
    if (f < 0 || f > 255 || (std::round(f) != f)) {
        OBJ_ERR << "integer in range 0..255 is expected, got: " << f;
        return;
    }
}

void ProtoHUI::m_vu(t_symbol* s, const AtomListView& lv)
{
    static const std::array<int8_t, 12> db_arr { -60, -50, -40, -30, -20, -14, -10, -8, -6, -4, -2, 0 };

    if (lv.size() < 2) {
        METHOD_ERR(s) << "usage: CHANNEL(0..7) DBFS [SIDE(0|1)]";
        return;
    }

    const auto chan = lv.intAt(0, -1);
    if (chan < 0 || chan > MAX_CHAN) {
        METHOD_ERR(s) << "expected channel in [0.." << MAX_CHAN << "] range, got: " << chan;
        return;
    }

    const int side = lv.intAt(2, SIDE_LEFT);
    if (side != SIDE_LEFT && side != SIDE_RIGHT) {
        METHOD_ERR(s) << "expected side is 0(left) or 1(right), got: " << side;
        return;
    }

    const auto val = lv.intAt(1, -144);
    const auto db_it = std::find_if(db_arr.begin(), db_arr.end(), [val](int8_t x) -> bool { return val < x; });
    const size_t idx = std::distance(db_arr.begin(), db_it);

    Atom args[3];
    args[0].setFloat(0xA0, true);
    args[1].setFloat(chan, true);

    const auto vuval = (side == SIDE_LEFT ? HUI_SIDE_LEFT : HUI_SIDE_RIGHT) | idx;
    args[2].setFloat(vuval, true);
    listTo(0, AtomListView { &args[0].atom(), 3 });
}

void ProtoHUI::m_vpot(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 2) {
        METHOD_ERR(s) << "usage: CHANNEL(0..7) VALUE(0..15) [MODE(0|1|2|3)] [ACTIVE(0|1)]";
        return;
    }

    const auto chan = lv.intAt(0, -1);
    if (chan < 0 || chan > MAX_CHAN) {
        METHOD_ERR(s) << "expected channel in [0.." << MAX_CHAN << "] range, got: " << chan;
        return;
    }

    const int val = lv.intAt(1, -1);
    if (val < 1 || val > 11) {
        METHOD_ERR(s) << "expected value in [1..11] range, got: " << val;
        return;
    }

    const auto mode = lv.intAt(2, 0);
    if (mode < 0 || mode > 3) {
        METHOD_ERR(s) << "expected mode in [0..3] range, got: " << mode;
        return;
    }

    const auto active = lv.intAt(3, 0);
    if (active < 0 || active > 1) {
        METHOD_ERR(s) << "expected active 0 or 1, got: " << active;
        return;
    }

    Atom args[3];
    args[0].setFloat(0xB0, true);
    args[1].setFloat(0x10 | chan, true);
    args[2].setFloat((active * 0x40) | (mode * 0x10) | val, true);
    listTo(0, AtomListView { &args[0].atom(), 3 });
}

void ProtoHUI::m_led(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 2) {
        METHOD_ERR(s) << "usage: ZONE(0..1D) VAL(0|1)";
        return;
    }

    led_switch(lv.intAt(0, -1), lv.intAt(1, -1), lv.boolAt(2, false));
}

bool ProtoHUI::select_zone(int zone)
{
    if (zone < 0 || zone > MAX_ZONE) {
        OBJ_ERR << "expected zone in [0.." << MAX_ZONE << "] range, got: " << zone;
        return false;
    }

    Atom args[3];
    args[0].setFloat(0xB0, true);
    args[1].setFloat(0x0C, true);
    args[2].setFloat(0xFF & zone, true);
    listTo(0, AtomListView { &args[0].atom(), 3 });
    return true;
}

bool ProtoHUI::port_switch(int port, bool on)
{
    if (port < 0 || port > MAX_PORT) {
        OBJ_ERR << "expected port in [0.." << MAX_PORT << "] range, got: " << port;
        return false;
    }

    Atom args[3];
    args[0].setFloat(0xB0, true);
    args[1].setFloat(0x2C, true);
    args[2].setFloat((on ? 0x40 : 0x00) | port, true);
    listTo(0, AtomListView { &args[0].atom(), 3 });
    return true;
}

bool ProtoHUI::led_switch(int zone, int port, bool on)
{
    if (!select_zone(zone))
        return false;

    return port_switch(port, on);
}

void ProtoHUI::m_rec(t_symbol* s, const AtomListView& lv)
{
    led_switch(lv.intAt(0, -1), 7, lv.boolAt(1, false));
}

void ProtoHUI::m_solo(t_symbol* s, const AtomListView& lv)
{
    led_switch(lv.intAt(0, -1), 3, lv.boolAt(1, false));
}

void ProtoHUI::m_mute(t_symbol* s, const AtomListView& lv)
{
    led_switch(lv.intAt(0, -1), 2, lv.boolAt(1, false));
}

void ProtoHUI::m_select(t_symbol* s, const AtomListView& lv)
{
    led_switch(lv.intAt(0, -1), 1, lv.boolAt(1, false));
}

void ProtoHUI::m_fader(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 2) {
        METHOD_ERR(s) << "usage: ZONE(0..7) VAL(0..1)";
        return;
    }

    const auto zone = lv.intAt(0, -1);
    if (zone < 0 || zone > MAX_ZONE) {
        METHOD_ERR(s) << "expected zone in [0.." << MAX_ZONE << "] range, got: " << zone;
        return;
    }

    const int val = convert::lin2lin_clip<t_float>(lv.floatAt(1, 0), 0, 1, 0, 0x3fff);

    //    b0 0z hi 2z lo
    Atom args[5];
    args[0].setFloat(0xB0, true);
    args[1].setFloat(0x00 | zone, true);
    args[2].setFloat(0x7f & (val >> 7), true);
    args[3].setFloat(0x20 | zone, true);
    args[4].setFloat(0x7f & val, true);
    listTo(0, AtomListView { &args[0].atom(), 5 });
}

void setup_proto_hui()
{
    ObjectFactory<ProtoHUI> obj("proto.hui");
    obj.addMethod("vu", &ProtoHUI::m_vu);
    obj.addMethod("vpot", &ProtoHUI::m_vpot);
    obj.addMethod("led", &ProtoHUI::m_led);
    obj.addMethod("rec", &ProtoHUI::m_rec);
    obj.addMethod("solo", &ProtoHUI::m_solo);
    obj.addMethod("mute", &ProtoHUI::m_mute);
    obj.addMethod("select", &ProtoHUI::m_select);
    obj.addMethod("fader", &ProtoHUI::m_fader);
}
