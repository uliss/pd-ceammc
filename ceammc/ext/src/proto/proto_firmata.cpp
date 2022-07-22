#include "proto_firmata.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "firmata/firmata_proto.h"
#include "firmata/firmata_proto_request.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>

using byte = uint8_t;

using namespace firmata;

namespace {

CEAMMC_DEFINE_SYM_HASH(INPUT);
CEAMMC_DEFINE_SYM_HASH(OUTPUT);
CEAMMC_DEFINE_SYM_HASH(ANALOG);
CEAMMC_DEFINE_SYM_HASH(PWM);
CEAMMC_DEFINE_SYM_HASH(SERVO);
CEAMMC_DEFINE_SYM_HASH(SHIFT);
CEAMMC_DEFINE_SYM_HASH(I2C);
CEAMMC_DEFINE_SYM_HASH(ONEWIRE);
CEAMMC_DEFINE_SYM_HASH(STEPPER);
CEAMMC_DEFINE_SYM_HASH(ENCODER);
CEAMMC_DEFINE_SYM_HASH(SERIAL);
CEAMMC_DEFINE_SYM_HASH(PULLUP);

using PinEntry = std::pair<const char*, uint32_t>;
const std::array<PinEntry, 12> pin_modes {
    PinEntry { str_INPUT, hash_INPUT },
    PinEntry { str_OUTPUT, hash_OUTPUT },
    PinEntry { str_ANALOG, hash_ANALOG },
    PinEntry { str_PWM, hash_PWM },
    PinEntry { str_SERVO, hash_SERVO },
    PinEntry { str_SHIFT, hash_SHIFT },
    PinEntry { str_I2C, hash_I2C },
    PinEntry { str_ONEWIRE, hash_ONEWIRE },
    PinEntry { str_STEPPER, hash_STEPPER },
    PinEntry { str_ENCODER, hash_ENCODER },
    PinEntry { str_SERIAL, hash_SERIAL },
    PinEntry { str_PULLUP, hash_PULLUP },
};

const std::string F_QUERY_FIRMWARE = "firmware?";
const std::string F_QUERY_VERSION = "version?";
const std::string F_QUERY_ANALOG_MAPPING = "apin_mapping?";
const std::string F_QUERY_PIN_STATE = "pin_state?";
const std::string F_QUERY_CAPABILITIES = "capabilities?";
const std::string F_RESPONSE_VERSION = "version";
const std::string F_RESPONSE_FIRMWARE = "firmware";
const std::string F_RESPONSE_ANALOG_MAPPING = "apin_mapping";
const std::string F_RESPONSE_PIN_STATE = "pin_state";
const std::string F_RESPONSE_PIN_INFO = "pin_info";
const std::string F_RESPONSE_ANALOG_PIN = "apin";
const std::string F_RESPONSE_DIGITAL_PORT = "dport";
const std::string F_RESPONSE_STRING = "string";
const std::string F_SET_SAMPLING_INTERVAL = "samp_interval";
const std::string F_SET_PIN_MODE = "pin_mode";
const std::string F_WRITE_DIGITAL_PIN = "write_dpin";
const std::string F_WRITE_PWM_PIN = "write_pwm";
const std::string F_REPORT_ANALOG_PIN = "report_apin";
const std::string F_REPORT_DIGITAL_PORT = "report_dport";

inline bool operator==(t_symbol* s, const std::string& str)
{
    return str == s->s_name;
}

inline t_symbol* gensym(const std::string& str)
{
    return ::gensym(str.c_str());
}

}

Firmata::Firmata(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void Firmata::onFloat(t_float f)
{
    byte v = static_cast<byte>(roundf(f));

    parser_ << v;

    if (parser_.isDone()) {
        processFirmataInput(parser_.msg().command);
        parser_.reset();
    }

    if (parser_.isError()) {
        OBJ_ERR << "parsing error";
        parser_.reset();
    }
}

void Firmata::onAny(t_symbol* s, const AtomListView& lv)
{
    if (s == F_QUERY_VERSION) { // query firmata protocol version (answer is list like: [2 5])
        return floatTo(0, PROTO_PROTOCOL_VERSION);
    } else if (s == F_WRITE_DIGITAL_PIN) {
        if (!checkArgs(lv, ARG_BYTE, ARG_BOOL, s))
            return;

        return toArduino(ProtoRequest::setDigitalPinValue(lv[0].asInt(), lv[1].asInt()));

    } else if (s == F_WRITE_PWM_PIN) {
        if (!checkArgs(lv, ARG_BYTE, ARG_NATURAL, s))
            return;

        return toArduino(ProtoRequest::setAnalogPinValue(lv[0].asInt(), lv[1].asInt()));
    } else if (s == F_SET_PIN_MODE) { // set pin mode
        if (!checkArgs(lv, ARG_BYTE, ARG_SYMBOL, s))
            return;

        int mode = symToPinMode(lv[1].asSymbol());
        if (mode < 0) {
            OBJ_ERR << "invalid pin mode: " << lv[1];
            OBJ_DBG << "supported values are: ";
            for (auto& m : pin_modes) {
                OBJ_DBG << "    " << m.first;
            }
            return;
        }

        return toArduino(ProtoRequest::setDigitalPinMode(lv[0].asInt(), static_cast<FirmataPinMode>(mode)));
    } else if (s == F_REPORT_ANALOG_PIN) {
        if (!checkArgs(lv, ARG_BYTE, ARG_BOOL, s))
            return;

        return toArduino(ProtoRequest::enableAnalogReporting(lv[0].asInt(), lv[1].asInt()));
    } else if (s == F_REPORT_DIGITAL_PORT) {
        if (!checkArgs(lv, ARG_BYTE, ARG_BOOL, s))
            return;

        return toArduino(ProtoRequest::enableDigitalReporting(lv[0].asInt(), lv[1].asInt()));
    } else if (s == F_QUERY_ANALOG_MAPPING) {
        return toArduino(ProtoRequest::queryAnalogMapping());
    } else if (s == F_QUERY_CAPABILITIES) {
        return toArduino(ProtoRequest::queryCapabilities());
    } else if (s == F_QUERY_FIRMWARE) {
        // query firmata firmware version like list [StandartFirmata.ino 2 5]
        return toArduino(ProtoRequest::queryFirmware());
    } else if (s == F_SET_SAMPLING_INTERVAL) {
        if (!checkArgs(lv, ARG_FLOAT, s))
            return;

        int ms = lv[0].asInt();
        if (ms < 10 || ms > 10000) {
            OBJ_ERR << "sampling interval should be in range: [10-10000]";
            return;
        }

        return toArduino(ProtoRequest::setSamplingInterval(ms));
    } else if (s == F_QUERY_PIN_STATE) { // query pin state
        if (!checkArgs(lv, ARG_BYTE, s))
            return;

        return toArduino(ProtoRequest::queryPinState(lv[0].asInt()));
    } else {
        OBJ_ERR << "unknown message: " << s->s_name;
    }
}

void Firmata::processFirmataInput(int cmd)
{
    using AtomList3 = StaticAtomList<3>;

    switch (cmd) {
    case PROTO_PROTOCOL_VERSION: {
        AtomList3 res { parser_.versionMajor(), parser_.versionMinor() };
        anyTo(0, gensym(F_RESPONSE_VERSION), res.view());
    } break;
    case PROTO_ANALOG_IO_MESSAGE: {
        AtomList3 res { parser_.msg().pin, parser_.value() };
        anyTo(0, gensym(F_RESPONSE_ANALOG_PIN), res.view());
    } break;
    case PROTO_DIGITAL_IO_MESSAGE: {
        AtomList3 res { parser_.msg().pin, parser_.value() };
        anyTo(0, gensym(F_RESPONSE_DIGITAL_PORT), res.view());
    } break;
    case PROTO_START_SYSEX: {
        switch (parser_.msg().sysex_id) {
        case PROTO_SYSEX_ANALOG_MAPPING_RESPONSE: {

            for (int i = 0; i < parser_.msg().cur_pin_map_idx; i++) {
                int pin = parser_.msg().analog_pin_map[i];
                if (pin < 0)
                    continue;

                char buf[8];
                snprintf(buf, sizeof(buf) - 1, "A%i", pin);
                AtomList3 res { gensym(buf), i };
                anyTo(0, gensym(F_RESPONSE_ANALOG_MAPPING), res.view());
            }
        } break;
        case PROTO_SYSEX_CAPABILITY_RESPONSE: {
            for (int pin = 0; pin < parser_.msg().pin_cap_idx; pin++) {
                const FirmataPinCapabilities& caps = parser_.msg().capabilities[pin];

                for (int i = 0; i < caps.num; i++) {
                    const FirmataPinCapability& c = caps.list[i];

                    // [pinInfo $PIN OUTPUT $resolution(
                    AtomList3 res(pin, pinModeToSym(c.mode), c.resolution);
                    anyTo(0, gensym(F_RESPONSE_PIN_INFO), res.view());
                }
            }
        } break;
        case PROTO_SYSEX_REPORT_FIRMWARE: {
            AtomList3 res { gensym(parser_.str()), parser_.versionMajor(), parser_.versionMinor() };
            anyTo(0, gensym(F_RESPONSE_FIRMWARE), res.view());
        } break;
        case PROTO_SYSEX_PIN_STATE_RESPONSE: {
            AtomList3 res { parser_.msg().pin, pinModeToSym(parser_.msg().pin_mode), parser_.msg().pin_state.value };
            anyTo(0, gensym(F_RESPONSE_PIN_STATE), res.view());
        } break;
        case PROTO_SYSEX_STRING_DATA: {
            OBJ_ERR << parser_.str();
        } break;
        default:
            OBJ_ERR << "unsupported sysex: " << parser_.msg().sysex_id;
            break;
        }
    } break;
    default:
        OBJ_ERR << "unsupported action: " << cmd;
        break;
    }
}

void Firmata::toArduino(const std::string& cmd)
{
    for (size_t i = 0; i < cmd.size(); i++) {
        auto ch = static_cast<uint8_t>(cmd[i]);
        floatTo(0, ch);
    }
}

t_symbol* Firmata::pinModeToSym(int mode)
{
    if (mode < 0 || mode >= pin_modes.size())
        return &s_;

    return gensym(pin_modes[mode].first);
}

int Firmata::symToPinMode(t_symbol* s)
{
    auto hash = crc32_hash(s);
    auto it = std::find_if(pin_modes.begin(), pin_modes.end(),
        [hash](const PinEntry& e) { return e.second == hash; });

    if (it == pin_modes.end())
        return -1;

    return std::distance(pin_modes.begin(), it);
}

void setup_proto_firmata()
{
    ObjectFactory<Firmata> obj("proto.firmata");
}
