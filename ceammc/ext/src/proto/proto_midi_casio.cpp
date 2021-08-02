/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "proto_midi_casio.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include "ceammc_crc32.h"

constexpr const char* PRIVIA_PX160 = "px160";

enum SysExId {
    SYSEX_ID_NON_COMMERCIAL = 0x7D,
    SYSEX_ID_UNIVERSAL_NONRT = 0x7E,
    SYSEX_ID_UNIVERSAL_RT = 0x7F,
};

enum : uint8_t {
    SYSEX_DEVICE_BROADCAST = 0x7F,
    SUBID1_GEN_INFO = 0x06,
    SUBID1_DEV_CONTROL = 0x04,
    SUBID2_ID_REQUEST = 0x1,
    SUBID2_ID_REPLY = 0x2,
    SUBID2_GLOBAL_PARAM = 0x5,
    EOX = 0xF7,
    SYSEX = 0xF0,
};

class PX160 {
public:
    static std::array<uint8_t, 13> setReverbType(int t)
    {
        std::array<uint8_t, 13> res {
            SYSEX,
            SYSEX_ID_UNIVERSAL_RT,
            SYSEX_DEVICE_BROADCAST,
            SUBID1_DEV_CONTROL,
            SUBID2_GLOBAL_PARAM,
            0x1,
            0x1,
            0x1,
            0x1,
            0x1,
            0x0,
            0x0,
            EOX,
        };

        auto* p = &res[11];

        switch (t) {
        case 1:
            *p = 0x1;
            break;
        case 4:
            *p = 0xC;
            break;
        case 3:
            *p = 0x4;
            break;
        case 2:
        default:
            *p = 0x2;
            break;
        }

        return res;
    }

    static std::array<uint8_t, 13> setReverbTime(float f)
    {
        std::array<uint8_t, 13> res {
            SYSEX,
            SYSEX_ID_UNIVERSAL_RT,
            SYSEX_DEVICE_BROADCAST,
            SUBID1_DEV_CONTROL,
            SUBID2_GLOBAL_PARAM,
            0x1,
            0x1,
            0x1,
            0x1,
            0x1,
            0x1,
            0x0,
            EOX,
        };

        auto* p = &res[11];
        *p = static_cast<uint8_t>(convert::lin2lin_clip<t_float, 0, 1>(f, 0, 127));

        return res;
    }

    static std::array<uint8_t, 13> chorusParam(uint8_t p)
    {
        return {
            SYSEX,
            SYSEX_ID_UNIVERSAL_RT,
            SYSEX_DEVICE_BROADCAST,
            SUBID1_DEV_CONTROL,
            SUBID2_GLOBAL_PARAM,
            0x1,
            0x1,
            0x1,
            0x1,
            0x2,
            p,
            0xBB,
            EOX,
        };
    }

    static std::array<uint8_t, 13> setChorusType(int t)
    {
        auto res = chorusParam(0x0);
        auto* p = &res[11];

        switch (t) {
        case 1:
            *p = 0x0;
            break;
        case 2:
            *p = 0x2;
            break;
        case 3:
            *p = 0xA;
            break;
        case 4:
            *p = 0xB;
            break;
        default:
            *p = 0x0;
            break;
        }

        return res;
    }

    static std::array<uint8_t, 13> setChorusRate(float f)
    {
        auto res = chorusParam(0x1);
        res[11] = static_cast<uint8_t>(convert::lin2lin_clip<t_float, 0, 1>(f, 0, 127));
        return res;
    }

    static std::array<uint8_t, 13> setChorusDepth(float f)
    {
        auto res = chorusParam(0x2);
        res[11] = static_cast<uint8_t>(convert::lin2lin_clip<t_float, 0, 1>(f, 0, 127));
        return res;
    }

    static std::array<uint8_t, 13> setChorusFeedback(float f)
    {
        auto res = chorusParam(0x3);
        res[11] = static_cast<uint8_t>(convert::lin2lin_clip<t_float, 0, 1>(f, 0, 127));
        return res;
    }

    static std::array<uint8_t, 13> setChorusSend(float f)
    {
        auto res = chorusParam(0x4);
        res[11] = static_cast<uint8_t>(convert::lin2lin_clip<t_float, 0, 1>(f, 0, 127));
        return res;
    }
};

ProtoMidiCasio::ProtoMidiCasio(const PdArgs& args)
    : BaseObject(args)
    , model_(nullptr)
{
    createOutlet();

    model_ = new SymbolEnumProperty("@model", { PRIVIA_PX160 });
    addProperty(model_);
}

void ProtoMidiCasio::m_reverb_type(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT)) {
        METHOD_ERR(s) << "reverb type expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setReverbType(lv[0].asInt());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_reverb_time(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "reverb time expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setReverbTime(lv[0].asFloat());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_chorus_type(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT)) {
        METHOD_ERR(s) << "chorus type expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setChorusType(lv[0].asInt());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_chorus_rate(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "chorus rate expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setChorusRate(lv[0].asFloat());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_chorus_depth(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "chorus depth expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setChorusDepth(lv[0].asFloat());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_chorus_fb(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "chorus feedback expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setChorusFeedback(lv[0].asFloat());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_chorus_send(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "chorus reverb send expected, got: " << lv;
        return;
    }

    if (model_->value() == gensym(PRIVIA_PX160)) {
        const auto data = PX160::setChorusSend(lv[0].asFloat());
        output(data);
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::m_instr(t_symbol* s, const AtomListView& lv)
{
    if (model_->value() == gensym(PRIVIA_PX160)) {
        int chan = 0;
        t_symbol* instr = &s_;
        uint32_t id = 0;

        if (lv.size() == 3 && lv[0].isInteger() && lv[1].isSymbol()) {
            chan = lv[0].asT<int>();
            instr = lv[1].asSymbol();
            id = lv[2].isFloat() ? lv[2].asT<int>() : crc32_hash(lv[2].asT<t_symbol*>()->s_name);
        } else if (lv.size() == 2 && lv[0].isSymbol()) {
            instr = lv[0].asSymbol();
            id = lv[1].isFloat() ? lv[1].asT<int>() : crc32_hash(lv[1].asT<t_symbol*>()->s_name);
        } else {
            METHOD_ERR(s) << "CHAN[0..15]? CATEGORY NAME expected, got: " << lv;
            return;
        }

        switch (crc32_hash(instr->s_name)) {
        case "piano"_hash:
            switch (id) {
            case "concert"_hash:
            case 0:
                bankSelect(chan, 0, 0);
                progChange(chan, 0);
                break;
            case "modern"_hash:
            case 1:
                bankSelect(chan, 1, 0);
                progChange(chan, 0);
                break;
            case "classic"_hash:
            case 2:
                bankSelect(chan, 2, 0);
                progChange(chan, 0);
                break;
            case "mellow"_hash:
            case 3:
                bankSelect(chan, 3, 0);
                progChange(chan, 0);
                break;
            case "bright"_hash:
            case 4:
                bankSelect(chan, 4, 0);
                progChange(chan, 0);
                break;
            default:
                METHOD_ERR(s) << "unknown piano name, expected (concert|modern|classic|mellow|bright|0..4), got: " << lv[1];
                return;
            }
            break;
        case "epiano"_hash:
            switch (id) {
            case 0:
                bankSelect(chan, 0, 0);
                progChange(chan, 4);
                break;
            case 1:
                bankSelect(chan, 1, 0);
                progChange(chan, 4);
                break;
            case 2:
            case "fm"_hash:
                bankSelect(chan, 0, 0);
                progChange(chan, 5);
                break;
            case 3:
            case 60:
                bankSelect(chan, 2, 0);
                progChange(chan, 4);
                break;
            default:
                METHOD_ERR(s) << "unknown electric piano name, expected (0|1|fm|2|3|60), got: " << lv[1];
                return;
            }
            break;
        case "strings"_hash:
            switch (id) {
            case 0:
                bankSelect(chan, 0, 0);
                progChange(chan, 0x31);
                break;
            case 1:
                bankSelect(chan, 0, 0);
                progChange(chan, 0x30);
                break;
            default:
                METHOD_ERR(s) << "unknown strings name, expected (0|1), got: " << lv[1];
                return;
            }
            break;
        case "bass"_hash:
            bankSelect(chan, 0, 0);
            progChange(chan, 0x20);
            break;
        case "organ"_hash:
            switch (id) {
            case 0:
            case "pipe"_hash:
                bankSelect(chan, 0, 0);
                progChange(chan, 0x13);
                break;
            case 1:
            case "jazz"_hash:
                bankSelect(chan, 0, 0);
                progChange(chan, 0x11);
                break;
            case 2:
            case "elec1"_hash:
                bankSelect(chan, 1, 0);
                progChange(chan, 0x10);
                break;
            case 3:
            case "elec2"_hash:
                bankSelect(chan, 0, 0);
                progChange(chan, 0x10);
                break;
            }
            break;
        case "vibr"_hash:
            bankSelect(chan, 0, 0);
            progChange(chan, 0x0B);
            break;
        case "haprs"_hash:
            bankSelect(chan, 0, 0);
            progChange(chan, 0x06);
            break;
        default:
            METHOD_ERR(s) << "unknown instrument category: " << instr;
            return;
        }
    } else
        METHOD_ERR(s) << "model not supported: " << model_->value();
}

void ProtoMidiCasio::bankSelect(int chan, int msb, int lsb)
{
    std::array<int, 6> msg {
        0xB0 | (chan & 0x7),
        0,
        msb & 0x7F,
        0xB0 | (chan & 0x7),
        0x20,
        lsb & 0x7F,
    };

    output(msg);
}

void ProtoMidiCasio::progChange(int chan, int val)
{
    std::array<int, 2> msg { 0xC0 | (chan & 0x7), val & 0x7F };

    output(msg);
}

void setup_proto_midi_casio()
{
    ObjectFactory<ProtoMidiCasio> obj("proto.midi.casio");

    obj.addMethod("rev_type", &ProtoMidiCasio::m_reverb_type);
    obj.addMethod("rev_time", &ProtoMidiCasio::m_reverb_time);

    obj.addMethod("chorus_type", &ProtoMidiCasio::m_chorus_type);
    obj.addMethod("chorus_rate", &ProtoMidiCasio::m_chorus_rate);
    obj.addMethod("chorus_depth", &ProtoMidiCasio::m_chorus_depth);
    obj.addMethod("chorus_fb", &ProtoMidiCasio::m_chorus_fb);
    obj.addMethod("chorus_send", &ProtoMidiCasio::m_chorus_send);

    obj.addMethod("instr", &ProtoMidiCasio::m_instr);
}
