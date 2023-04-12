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
#include "proto_volca_keys.h"
#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "fmt/core.h"
#include "midi_names.h"
#include "proto_midi_cc.h"
#include "proto_midi_parser.h"

constexpr auto VCO_PORTAMENTO = 5;
constexpr auto VCO_DETUNE = 42;
constexpr auto VCO_EG_INT = 43;
constexpr auto GENERAL_VOICE = 40;
constexpr auto GENERAL_OCTAVE = 41;
constexpr auto DELAY_TIME = 52;
constexpr auto DELAY_FEEDBACK = 53;

constexpr auto GENERAL_OCTAVE_32 = 20;
constexpr auto GENERAL_OCTAVE_16 = 40;
constexpr auto GENERAL_OCTAVE_8 = 60;
constexpr auto GENERAL_OCTAVE_4 = 80;
constexpr auto GENERAL_OCTAVE_2 = 100;
constexpr auto GENERAL_OCTAVE_1 = 120;

constexpr auto GENERAL_VOICE_POLY = 12;
constexpr auto GENERAL_VOICE_UNISON = 37;
constexpr auto GENERAL_VOICE_OCT = 60;
constexpr auto GENERAL_VOICE_FIFTH = 85;
constexpr auto GENERAL_VOICE_RING = 110;
constexpr auto GENERAL_VOICE_POLYRING = 127;

ProtoVolcaKeys::ProtoVolcaKeys(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    chan_ = new IntProperty("@ch", 0);
    chan_->checkClosedRange(0, 15);
    chan_->setArgIndex(0);
    addProperty(chan_);
}

void ProtoVolcaKeys::m_detune(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:i[0,127]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    setCC3(VCO_DETUNE, lv.intAt(0, 0));
}

void ProtoVolcaKeys::m_panic(t_symbol* s, const AtomListView& lv)
{
    setCC3(CC_ALL_NOTES_OFF, 0x7F);
}

void ProtoVolcaKeys::m_octave(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:i=32|16|8|4|2|1");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    switch (lv.intAt(0, 0)) {
    case 32:
        return setCC3(GENERAL_OCTAVE, GENERAL_OCTAVE_32);
    case 16:
        return setCC3(GENERAL_OCTAVE, GENERAL_OCTAVE_16);
    case 8:
        return setCC3(GENERAL_OCTAVE, GENERAL_OCTAVE_8);
    case 4:
        return setCC3(GENERAL_OCTAVE, GENERAL_OCTAVE_4);
    case 2:
        return setCC3(GENERAL_OCTAVE, GENERAL_OCTAVE_2);
    case 1:
        return setCC3(GENERAL_OCTAVE, GENERAL_OCTAVE_1);
    default:
        break;
    }
}

void ProtoVolcaKeys::m_voice(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TYPE:s=poly|unison|octave|fifth|ring|polyring");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto sym = lv.symbolAt(0, &s_);

    switch (crc32_hash(sym)) {
    case "poly"_hash:
        return setCC3(GENERAL_VOICE, 12);
    case "unison"_hash:
        return setCC3(GENERAL_VOICE, 37);
    case "octave"_hash:
        return setCC3(GENERAL_VOICE, 60);
    case "fifth"_hash:
        return setCC3(GENERAL_VOICE, 85);
    case "ring"_hash:
        return setCC3(GENERAL_VOICE, 110);
    case "polyring"_hash:
        return setCC3(GENERAL_VOICE, 127);
    default:
        METHOD_ERR(s) << fmt::format("unknown voice type: '{}'", sym->s_name);
        return;
    }
}

void ProtoVolcaKeys::setCC3(std::uint8_t cc, std::uint8_t data)
{
    floatTo(0, midi::MIDI_CONTROLCHANGE & (0xF & chan_->value()));
    floatTo(0, cc & 0x7F);
    floatTo(0, data & 0x7F);
}

void setup_proto_volca_keys()
{
    ObjectFactory<ProtoVolcaKeys> obj("proto.volca.keys");
    obj.addAlias("volca.keys");

    obj.addMethod(M_PANIC, &ProtoVolcaKeys::m_panic);
    obj.addMethod("detune", &ProtoVolcaKeys::m_detune);
    obj.addMethod("octave", &ProtoVolcaKeys::m_octave);
    obj.addMethod("voice", &ProtoVolcaKeys::m_voice);
}
