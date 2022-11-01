#include "proto_moppy.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "midi_names.h"

#include "fmt/core.h"

#include <array>

constexpr int MOPPY_PROTO_V1 = 1;
constexpr int MOPPY_PROTO_V2 = 2;
constexpr int MOPPY_MAGICK = 0x4d;
constexpr int MOPPY_NOTE_ON = 0x09;
constexpr int MOPPY_NOTE_OFF = 0x08;
constexpr int MOPPY_PITCHBEND = 0x0e;
constexpr int MOPPY_ALL_NOTES_OFF = 0x0;
constexpr int MOPPY_PING = 0x80;
constexpr int MOPPY_RESET = 0xff;
constexpr int MOPPY_SYSTEM_ADDR = 0;
constexpr int MOPPY_CMD0 = 1;
constexpr int MOPPY_CMD1 = 2;
constexpr int MOPPY_CMD2 = 3;

/**
 * Resolution of the Arduino code in microSeconds.
 */
constexpr int ARDUINO_RESOLUTION = 40;

template <size_t N>
using AtomArray = std::array<Atom, N>;
using MoppyCmd0 = AtomArray<5>;
using MoppyCmd1 = AtomArray<6>;
using MoppyCmd2 = AtomArray<7>;

// v2 protocol:
// https://github.com/Sammy1Am/Moppy2/wiki/Design-Docs

ProtoMoppy::ProtoMoppy(const PdArgs& args)
    : BaseObject(args)
    , vers_(nullptr)
    , device_addr_(nullptr)
    , floppy_addr_(nullptr)
{
    createOutlet();

    vers_ = new IntEnumProperty("@v", { MOPPY_PROTO_V2, MOPPY_PROTO_V1 });
    addProperty(vers_);

    device_addr_ = new IntProperty("@device", 1);
    device_addr_->checkClosedRange(1, 255);
    addProperty(device_addr_);

    floppy_addr_ = new IntProperty("@floppy", 1);
    floppy_addr_->checkClosedRange(1, 16);
    addProperty(floppy_addr_);
}

void ProtoMoppy::m_note_on(t_symbol* s, const AtomListView& lv)
{
    auto floppy_id = floppy_addr_->value();
    std::uint8_t note = 0;
    std::uint8_t vel = 0;

    if (checkArgBytesN(3, lv)) {
        floppy_id = lv[0].asT<int>();
        note = lv[1].asT<int>();
        vel = lv[2].asT<int>();
    } else if (checkArgBytesN(2, lv)) {
        note = lv[0].asT<int>();
        vel = lv[1].asT<int>();
    } else {
        METHOD_ERR(s) << "usage: CHAN? NOTE VEL";
        return;
    }

    switch (vers_->value()) {
    case MOPPY_PROTO_V2: {
        if (vel > 0) {
            MoppyCmd2 msg {
                MOPPY_MAGICK,
                device_addr_->value(),
                floppy_id,
                MOPPY_CMD2,
                MOPPY_NOTE_ON,
                note,
                vel,
            };
            listTo(0, AtomListView(msg.data(), msg.size()));
        } else {
            MoppyCmd1 msg {
                MOPPY_MAGICK,
                device_addr_->value(),
                floppy_id,
                MOPPY_CMD1,
                MOPPY_NOTE_OFF,
                note,
            };
            listTo(0, AtomListView(msg.data(), msg.size()));
        }
    } break;
    case MOPPY_PROTO_V1: {
        const std::uint8_t pin = floppy_id * 2;

        if (vel > 0) {
            auto freq = convert::midi2freq(static_cast<double>(note));
            // After looking up the period, devide by (the Arduino resolution * 2).
            // The Arduino's timer will only tick once per X microseconds based on the
            // resolution.  And each tick will only turn the pin on or off.  So a full
            // on-off cycle (one step on the floppy) is two periods.
            int t = std::round((1000000.0 / freq) / (ARDUINO_RESOLUTION * 2));
            std::uint8_t msb = (t >> 8) & 0xff;
            std::uint8_t lsb = t & 0xff;

            AtomArray<3> msg { pin, msb, lsb };
            listTo(0, AtomListView(msg.data(), msg.size()));
        } else {
            AtomArray<3> msg { pin, 0., 0. };
            listTo(0, AtomListView(msg.data(), msg.size()));
        }
    } break;
    default:
        METHOD_ERR(s) << fmt::format("protocol version not supported: {}", vers_->value());
        break;
    }
}

void ProtoMoppy::m_note_off(t_symbol* s, const AtomListView& lv)
{
    auto floppy_id = floppy_addr_->value();
    std::uint8_t note = 0;

    if (checkArgBytesN(2, lv)) {
        floppy_id = lv[0].asT<int>();
        note = lv[1].asT<int>();
    } else if (checkArgBytesN(1, lv)) {
        note = lv[0].asT<int>();
    } else {
        METHOD_ERR(s) << "usage: CHAN? NOTE";
        return;
    }

    switch (vers_->value()) {
    case MOPPY_PROTO_V2: {
        MoppyCmd1 msg {
            MOPPY_MAGICK,
            device_addr_->value(),
            floppy_id,
            MOPPY_CMD1,
            MOPPY_NOTE_OFF,
            note,
        };
        listTo(0, AtomListView(msg.data(), msg.size()));
    } break;
    case MOPPY_PROTO_V1: {
        std::uint8_t pin = floppy_addr_->value() * 2;
        AtomArray<3> msg { pin, 0., 0. };
        listTo(0, AtomListView(msg.data(), msg.size()));
    } break;
    default:
        METHOD_ERR(s) << fmt::format("protocol version not supported: {}", vers_->value());
        break;
    }
}

void ProtoMoppy::m_ping(t_symbol* s, const AtomListView& lv)
{
    switch (vers_->value()) {
    case MOPPY_PROTO_V2: {
        MoppyCmd0 msg {
            MOPPY_MAGICK,
            MOPPY_SYSTEM_ADDR,
            MOPPY_SYSTEM_ADDR,
            MOPPY_CMD0,
            MOPPY_PING,
        };
        listTo(0, AtomListView(&msg.front(), msg.size()));
    } break;
    default:
        METHOD_ERR(s) << fmt::format("protocol version not supported: {}", vers_->value());
        break;
    }
}

void ProtoMoppy::m_reset(t_symbol* s, const AtomListView& lv)
{
    switch (vers_->value()) {
    case MOPPY_PROTO_V2: {
        if (s == gensym("all")) {
            MoppyCmd0 msg {
                MOPPY_MAGICK,
                MOPPY_SYSTEM_ADDR,
                MOPPY_SYSTEM_ADDR,
                MOPPY_CMD0,
                MOPPY_RESET,
            };
            listTo(0, AtomListView(msg.data(), msg.size()));
        } else {
            MoppyCmd0 msg {
                MOPPY_MAGICK,
                device_addr_->value(),
                floppy_addr_->value(),
                MOPPY_CMD0,
                MOPPY_ALL_NOTES_OFF,
            };
            listTo(0, AtomListView(msg.data(), msg.size()));
        }
    } break;
    case MOPPY_PROTO_V1: {
        AtomArray<3> msg { 100, 0., 0. };
        listTo(0, AtomListView(msg.data(), msg.size()));
    } break;
    default:
        METHOD_ERR(s) << fmt::format("protocol version not supported: {}", vers_->value());
        break;
    }
}

void ProtoMoppy::m_bendf(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    switch (vers_->value()) {
    case MOPPY_PROTO_V2: {
        auto vali = static_cast<uint16_t>(std::round(convert::lin2lin_clip<t_float, -1, 1>(
            lv[0].asT<t_float>(),
            std::numeric_limits<int16_t>::lowest(),
            std::numeric_limits<int16_t>::max())));

        MoppyCmd2 msg {
            MOPPY_MAGICK,
            device_addr_->value(),
            floppy_addr_->value(),
            MOPPY_CMD2,
            MOPPY_PITCHBEND,
            (vali >> 8),
            (vali & 0xff),
        };
        listTo(0, AtomListView(msg.data(), msg.size()));
    } break;
    default:
        METHOD_ERR(s) << fmt::format("protocol version not supported: {}", vers_->value());
        break;
    }
}

bool ProtoMoppy::checkArgBytesN(uint8_t n, const AtomListView& lv)
{
    if (lv.size() != n)
        return false;

    for (uint8_t i = 0; i < n; i++) {
        auto& a = lv[i];
        bool ok = a.isInteger() && 0 <= a.asT<int>() && a.asT<int>() <= 255;
        if (!ok)
            return false;
    }

    return true;
}

void setup_proto_moppy()
{
    ObjectFactory<ProtoMoppy> obj("proto.moppy");

    obj.addMethod("note", &ProtoMoppy::m_note_on);
    obj.addMethod("noteon", &ProtoMoppy::m_note_on);
    obj.addMethod("noteoff", &ProtoMoppy::m_note_off);
    obj.addMethod("ping", &ProtoMoppy::m_ping);
    obj.addMethod("reset", &ProtoMoppy::m_reset);
    obj.addMethod(M_BEND_FLOAT, &ProtoMoppy::m_bendf);

    obj.setXletsInfo({ "note, noteoff, bend:f, reset, ping" }, { "list: byte list" });
}
