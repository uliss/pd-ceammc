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
#include "proto_midi_sysex.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "proto_midi_parser.h"

constexpr const char* MASTER_BALANCE_FLOAT = "mbal:f";

constexpr const char* MASTER_TUNING_COARSE = "mtune~";
constexpr const char* MASTER_TUNING_FINE = "mtune.";
constexpr const char* MASTER_TUNING = "mtune";

enum SysExId {
    SYSEX_ID_NON_COMMERCIAL = 0x7D,
    SYSEX_ID_UNIVERSAL_NONRT = 0x7E,
    SYSEX_ID_UNIVERSAL_RT = 0x7F,
};

enum SysExMan {
    SYSEX_MAN_BYTE3 = 0x00,
};

enum {
    SYSEX_DEVICE_BROADCAST = 0x7F,
    SUBID1_GEN_INFO = 0x06,
    SUBID1_DEV_CONTROL = 0x04,
    SUBID2_ID_REQUEST = 0x1,
    SUBID2_ID_REPLY = 0x2,
    SUBID2_MASTER_VOLUME = 0x1,
    SUBID2_MASTER_BALANCE = 0x2,
    SUBID2_MASTER_TUNE_COARSE = 0x4,
    SUBID2_MASTER_TUNE_FINE = 0x3,
    EOX = 0xF7,
    SYSEX = 0xF0,
};

static inline bool isSysRealtime(uint8_t b) { return b > midi::MIDI_SYSEXEND; }
static inline bool isStatusByte(uint8_t b) { return b >= 0x80; }

ProtoMidiSysex::ProtoMidiSysex(const PdArgs& args)
    : BaseObject(args)
    , n_(0)
    , state_(STATE_INIT)
{
    createOutlet();
}

void ProtoMidiSysex::onFloat(t_float f)
{
    if (f < 0 || f > 0xff) {
        OBJ_ERR << "byte value in [0..255] range expected, got: " << f;
        return;
    }

    const Byte byte = f;
    if (isSysRealtime(byte))
        return;

    switch (byte) {
    case midi::MIDI_SYSEX: {
        if (state_ == STATE_INIT) {
            reset();
            appendByte(byte);
            state_ = STATE_SYSEX;
        }
    } break;
    case midi::MIDI_SYSEXEND: {
        if (!appendByte(byte)) {
            OBJ_ERR << "buffer overflow";
            return reset();
        }

        output();
        reset();
    } break;
    default:
        if (isStatusByte(byte)) {
            if (state_ == STATE_SYSEX) {
                if (!appendByte(byte)) {
                    OBJ_ERR << "buffer overflow";
                    return reset();
                }

                output();
                reset();
            }
        } else {
            if (state_ == STATE_SYSEX) {
                if (!appendByte(byte)) {
                    OBJ_ERR << "buffer overflow";
                    return reset();
                }
            }
        }
    }
}

void ProtoMidiSysex::onList(const AtomList& lst)
{
    for (auto& a : lst) {
        if (a.isFloat())
            onFloat(a.asT<t_float>());
    }
}

bool ProtoMidiSysex::appendByte(ProtoMidiSysex::Byte b)
{
    if (n_ >= N)
        return false;

    buf_[n_++] = b;
    return true;
}

void ProtoMidiSysex::output()
{
    using ByteList = std::initializer_list<Byte>;
    static const ByteList id_reply { SYSEX, SYSEX_ID_UNIVERSAL_NONRT, SYSEX_DEVICE_BROADCAST, SUBID1_GEN_INFO, SUBID2_ID_REPLY };

    if (startsWith(id_reply)) {
        auto pos = id_reply.size();
        if (buf_[pos] == SYSEX_MAN_BYTE3) {
            Short id = (buf_[pos + 1] << 7) | buf_[pos + 2];
            anyTo(0, gensym("man"), Atom(id));
            pos += 3;
        } else {
            anyTo(0, gensym("man"), Atom(buf_[pos]));
            pos += 1;
        }

        Short dev = (buf_[pos + 1] << 7) | buf_[pos];
        pos += 2;
        anyTo(0, gensym("dev"), Atom(dev));

        Short dev_model = (buf_[pos + 1] << 7) | buf_[pos];
        pos += 2;

        anyTo(0, gensym("dev_model"), Atom(dev_model));

        Atom soft[4];
        for (size_t i = 0; i < 4; i++)
            soft[i] = buf_[pos + i];

        anyTo(0, gensym("soft"), AtomListView(soft, 4));
    } else {
        for (size_t i = 0; i < n_; i++) {
            OBJ_ERR << i << ": " << (int)buf_[i];
        }
    }
}

void ProtoMidiSysex::reset()
{
    state_ = STATE_INIT;
    n_ = 0;
}

bool ProtoMidiSysex::startsWith(std::initializer_list<Byte> l) const
{
    if (n_ < l.size())
        return false;

    for (size_t i = 0; i < l.size(); i++) {
        if (buf_[i] != *(l.begin() + i))
            return false;
    }

    return true;
}

void ProtoMidiSysex::output_mtune_coarse(int v)
{
    const t_float semi = 0x7F & (clip<int, -64, 63>(v) + 64);
    const t_float msg[8] = {
        SYSEX,
        SYSEX_ID_UNIVERSAL_RT,
        SYSEX_DEVICE_BROADCAST,
        SUBID1_DEV_CONTROL,
        SUBID2_MASTER_TUNE_COARSE,
        0,
        semi,
        EOX,
    };

    for (size_t i = 0; i < 8; i++)
        floatTo(0, msg[i]);
}

void ProtoMidiSysex::output_mtune_fine(t_float v)
{
    auto f = convert::lin2lin_clip<t_float, -100, 100>(v, 0x000, 0x3fff);
    const t_float msb = 0x7F & ((int)f >> 7);
    const t_float lsb = 0x7F & ((int)f);
    const t_float msg[8] = {
        SYSEX,
        SYSEX_ID_UNIVERSAL_RT,
        SYSEX_DEVICE_BROADCAST,
        SUBID1_DEV_CONTROL,
        SUBID2_MASTER_TUNE_FINE,
        lsb,
        msb,
        EOX,
    };

    for (size_t i = 0; i < 8; i++)
        floatTo(0, msg[i]);
}

void ProtoMidiSysex::m_id_request(t_symbol*, const AtomListView&)
{
    const Byte msg[6] = {
        SYSEX,
        SYSEX_ID_UNIVERSAL_NONRT,
        SYSEX_DEVICE_BROADCAST,
        SUBID1_GEN_INFO,
        SUBID2_ID_REQUEST,
        EOX,
    };

    for (size_t i = 0; i < 6; i++)
        floatTo(0, msg[i]);
}

void ProtoMidiSysex::m_id_reply(t_symbol*, const AtomListView&)
{
}

void ProtoMidiSysex::m_mvolume(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "float volume in [0..1] range expected, got: " << lv;
        return;
    }

    const auto f = lv[0].asT<t_float>();
    if (f < 0 || f > 1) {
        METHOD_ERR(s) << "float volume in [0..1] range expected, got: " << lv;
        return;
    }

    const uint16_t volume = convert::lin2lin<t_float, 0, 1>(f, 0x0000, 0x3fff);
    const t_float msb = 0x7F & (volume >> 7);
    const t_float lsb = 0x7F & volume;

    const t_float msg[8] = {
        SYSEX,
        SYSEX_ID_UNIVERSAL_RT,
        SYSEX_DEVICE_BROADCAST,
        SUBID1_DEV_CONTROL,
        SUBID2_MASTER_VOLUME,
        lsb,
        msb,
        EOX,
    };

    for (size_t i = 0; i < 8; i++)
        floatTo(0, msg[i]);
}

void ProtoMidiSysex::m_mbalance_float(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "float balance in [-1..1] range expected, got: " << lv;
        return;
    }

    const auto f = lv[0].asT<t_float>();
    if (f < -1 || f > 1) {
        METHOD_ERR(s) << "float balance in [-1..1] range expected, got: " << lv;
        return;
    }

    const uint16_t bal = convert::lin2lin<t_float, -1, 1>(f, 0x0000, 0x3fff);
    const t_float msb = 0x7F & (bal >> 7);
    const t_float lsb = 0x7F & bal;

    const t_float msg[8] = {
        SYSEX,
        SYSEX_ID_UNIVERSAL_RT,
        SYSEX_DEVICE_BROADCAST,
        SUBID1_DEV_CONTROL,
        SUBID2_MASTER_BALANCE,
        lsb,
        msb,
        EOX,
    };

    for (size_t i = 0; i < 8; i++)
        floatTo(0, msg[i]);
}

void ProtoMidiSysex::m_mtune_coarse(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT)) {
        METHOD_ERR(s) << "tuning (in semitones) expected, got: " << lv;
        return;
    }

    output_mtune_coarse(lv[0].asT<int>());
}

void ProtoMidiSysex::m_mtune(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "tuning (in semitones) expected, got: " << lv;
        return;
    }

    const t_float tune = clip<t_float, -64, 63>(lv[0].asFloat());

    t_float coarse = 0;
    const t_float fine = std::modf(tune, &coarse);

    output_mtune_coarse(coarse);
    output_mtune_fine(fine * 100);
}

void ProtoMidiSysex::m_mtune_fine(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "float fine tuning (in cents) expected, got: " << lv;
        return;
    }

    output_mtune_fine(lv[0].asT<t_float>());
}

void setup_proto_midi_sysex()
{
    ObjectFactory<ProtoMidiSysex> obj("proto.midi.sysex");
    obj.addMethod("id_request", &ProtoMidiSysex::m_id_request);
    obj.addMethod("id_reply", &ProtoMidiSysex::m_id_reply);

    obj.addMethod("mvolume", &ProtoMidiSysex::m_mvolume);
    obj.addMethod(MASTER_BALANCE_FLOAT, &ProtoMidiSysex::m_mbalance_float);

    obj.addMethod(MASTER_TUNING_FINE, &ProtoMidiSysex::m_mtune_fine);
    obj.addMethod(MASTER_TUNING_COARSE, &ProtoMidiSysex::m_mtune_coarse);
    obj.addMethod(MASTER_TUNING, &ProtoMidiSysex::m_mtune);
}
