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

void setup_proto_midi_casio()
{
    ObjectFactory<ProtoMidiCasio> obj("proto.midi.casio");

    obj.addMethod("revtype", &ProtoMidiCasio::m_reverb_type);
    obj.addMethod("revtime", &ProtoMidiCasio::m_reverb_time);
}
