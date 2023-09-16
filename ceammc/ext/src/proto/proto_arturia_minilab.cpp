/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "proto_arturia_minilab.h"
#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/format.h"

#include <utility>

// https://forum.arturia.com/index.php?topic=93714.msg152899#msg152899
// F0 00 20 6B 7F 42 02 00 10 7n cc F7
// where:
//  n is the pad number, 0 to F, corresponding to Pad1 to Pad16
//  cc is the color:
//    00 - black
//    01 - red
//    04 - green
//    05 - yellow
//    10 - blue
//    11 - magenta
//    14 - cyan
//    7F - white

CEAMMC_DEFINE_HASH(black)
CEAMMC_DEFINE_HASH(red)
CEAMMC_DEFINE_HASH(green)
CEAMMC_DEFINE_HASH(yellow)
CEAMMC_DEFINE_HASH(blue)
CEAMMC_DEFINE_HASH(magenta)
CEAMMC_DEFINE_HASH(cyan)
CEAMMC_DEFINE_HASH(white)

enum PadColor {
    PAD_BLACK,
    PAD_RED,
    PAD_GREEN,
    PAD_YELLOW,
    PAD_BLUE,
    PAD_MAGENTA,
    PAD_CYAN,
    PAD_WHITE,
    PAD_NONE = -1
};

// constexpr int
using PadArray = std::array<std::uint8_t, MINILAB_KNOB_COUNT>;

constexpr const std::uint8_t PAD_COLORS[8] = { 0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x7f };
constexpr const char* PAD_COLOR_NAMES[8] = {
    str_black,
    str_red,
    str_green,
    str_yellow,
    str_blue,
    str_magenta,
    str_cyan,
    str_white,
};

static const PadArray KNOB_SYSEX_IDX {
    0x30, 0x01, // knob 1-2
    0x02, 0x09,
    0x0B, 0x0C,
    0x0D, 0x0E,
    0x33, 0x03,
    0x04, 0x0A,
    0x05, 0x06,
    0x07, 0x08,
    0x32, 0x35, // shift + knob 1 and 9
};

static inline t_float cc2float(midi::Byte v) { return convert::lin2lin<t_float, 0, 127>(v, 0, 1); }
static inline midi::Byte float2cc(t_float v) { return std::round(convert::lin2lin_clip<t_float, 0, 1>(v, 0, 127)); }
static std::uint8_t color2midi(PadColor c) { return PAD_COLORS[c]; }

static PadColor atomToColor(const Atom& a)
{
    if (a.isSymbol()) {
        switch (crc32_hash(a.asT<t_symbol*>())) {
        case hash_black:
            return PAD_BLACK;
        case hash_blue:
            return PAD_BLUE;
        case hash_red:
            return PAD_RED;
        case hash_green:
            return PAD_GREEN;
        case hash_yellow:
            return PAD_YELLOW;
        case hash_magenta:
            return PAD_MAGENTA;
        case hash_cyan:
            return PAD_CYAN;
        case hash_white:
            return PAD_WHITE;
        default:
            break;
        }
    } else if (a.isInteger()) {
        auto x = a.asInt();
        if (x >= PAD_BLACK && x <= PAD_WHITE)
            return static_cast<PadColor>(x);
    }

    LIB_ERR << fmt::format("unknown color: '{}', expected values are: {} or 0,1,2,3,4,5,6,7", to_string(a), fmt::join(PAD_COLOR_NAMES, ","));
    return PAD_NONE;
}

static int findKnobByCC(midi::Byte cc)
{
    static const PadArray CC_TO_KNOB {
        112, 74,
        71, 76,
        77, 93,
        73, 75,
        114, 18,
        19, 16,
        17, 91,
        79, 72,
        7, 116
    };

    auto data = CC_TO_KNOB.data();
    auto x = static_cast<const std::uint8_t*>(memchr(data, cc, MINILAB_KNOB_COUNT));
    return (x == nullptr) ? -1 : (x - data);
}

static int findKnobBySysIdx(midi::Byte sys_idx)
{
    auto data = KNOB_SYSEX_IDX.data();
    auto x = static_cast<const std::uint8_t*>(memchr(data, sys_idx, MINILAB_KNOB_COUNT));
    return (x == nullptr) ? -1 : (x - data);
}

// kn - Knob in [0..17] range
static inline int findSysIdxByKnob(midi::Byte kn)
{
    return (kn < 0 || kn >= KNOB_SYSEX_IDX.size()) ? -1 : KNOB_SYSEX_IDX[kn];
}

static AtomArray<9> arturia_param_get(midi::Byte param)
{
    return AtomArray<9> { 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x01, 0x00, 0x00, param };
}

// To set the CC initial value send this SYSEX message:
//   F0 00 20 6B 7F 42 02 00 00 cc vv F7
// where:
//   vv is the CC initial value
//   cc is the controller index:
static AtomArray<10> arturia_param_set(midi::Byte param, midi::Byte value)
{
    return AtomArray<10> { 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x00, param, value };
}

static inline bool is_arturia_knob_btn(midi::Byte cc) { return cc == 113 || cc == 115; }

ProtoArturiaMinilab::ProtoArturiaMinilab(const PdArgs& args)
    : ProtoMidi(args)
{
    using midi::Byte;

    parser_.setControlChangeFn([this](Byte b, Byte c, Byte v) {
        auto kn = findKnobByCC(c);
        if (kn >= 0) {
            knobs_[kn] = v;
            AtomArray<2> data { kn, cc2float(v) };
            msgTo(gensym("knob"), data.view());
        } else if (is_arturia_knob_btn(c)) {
            AtomArray<2> data { (c == 115), (v > 0) };
            msgTo(gensym("button"), data.view());
        } else
            msgCC(b, c, v);
    });

    parser_.setSysExFn([this](size_t n, const Byte* data) {
        if (n == 12 && std::memcmp(data, "\xF0\x00\x20\x6B\x7F\x42\x02\x00\x00", 9) == 0) { // knob request answer
            const auto k = findKnobBySysIdx(data[9]);
            if (k < 0 || k >= MINILAB_KNOB_COUNT)
                return;

            auto v = data[10];
            knobs_[k] = v;
            AtomArray<2> kc { k, cc2float(v) };
            msgTo(gensym("knob?"), kc.view());
        }
    });
}

void ProtoArturiaMinilab::m_pad_color(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PAD:i[0,15]? COLOR:a");

    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (lv.size() == 2) {
        std::uint8_t pad = lv.intAt(0, 0);
        PadColor c = atomToColor(lv[1]);
        if (c == PAD_NONE)
            return;

        AtomArray<10> data { 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x10, 0x70 | pad, color2midi(c) };
        m_sysex(s, data.view());
    } else if(lv.size() == 1) {
        PadColor c = atomToColor(lv[0]);

        AtomArray<10> data { 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x10, 0x70, color2midi(c) };
        for (int i = 0; i < MINILAB_PAD_COUNT; i++) {
            data[8] = 0x70 | i;
            m_sysex(s, data.view());
        }
    }
}

void ProtoArturiaMinilab::m_channel(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,15]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    // F0 00 20 6B 7F 42 02 00 40 06 chan F7
    AtomArray<10> data { 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x40, 0x06, lv[0].asInt() };
    m_sysex(s, data.view());
}

void ProtoArturiaMinilab::m_knob(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("KNOB:i[0,17] VAL:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto knob = lv[0].asInt();
    const auto v = float2cc(lv[1].asFloat());
    knobs_[knob] = v;
    const auto cc = findSysIdxByKnob(knob);
    if (cc < 0)
        return;

    m_sysex(s, arturia_param_set(cc, v).view());
}

void ProtoArturiaMinilab::m_knob_req(t_symbol* s, const AtomListView& lv)
{
    const args::ArgChecker chk("KNOB:i[0,17]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto knob = lv[0].asInt();
    const auto cc = findSysIdxByKnob(knob);
    if (cc < 0)
        return;

    m_sysex(s, arturia_param_get(cc).view());
}

void ProtoArturiaMinilab::m_knob_req_all(t_symbol* s, const AtomListView& lv)
{
    for (int i = 0; i < MINILAB_KNOB_COUNT; i++)
        m_sysex(s, arturia_param_get(findSysIdxByKnob(i)).view());
}

void ProtoArturiaMinilab::m_pad_backlight(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_BOOL, s))
        return;

    // F0 00 20 6B 7F 42 02 00 40 1E on/off F7
    AtomArray<10> data { 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x40, 0x1E, lv[0].asBool() * 0x7F };
    m_sysex(s, data.view());
}

void setup_proto_arturia_minilab()
{
    ObjectFactory<ProtoArturiaMinilab> obj("proto.arturia.minilab");
    obj.addMethod("pad_color", &ProtoArturiaMinilab::m_pad_color);
    obj.addMethod("knob", &ProtoArturiaMinilab::m_knob);
    obj.addMethod("knob?", &ProtoArturiaMinilab::m_knob_req);
    obj.addMethod("knobs?", &ProtoArturiaMinilab::m_knob_req_all);
    obj.addMethod("backlight", &ProtoArturiaMinilab::m_pad_backlight);
    obj.addMethod("channel", &ProtoArturiaMinilab::m_channel);
}
