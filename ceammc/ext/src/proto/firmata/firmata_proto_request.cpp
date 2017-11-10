#include "firmata_proto_request.h"

static inline uint8_t b7(uint8_t v)
{
    return 0x7F & v;
}

std::string firmata::ProtoRequest::setDigitalPinMode(uint8_t pin, FirmataPinMode mode)
{
    char buf[3];
    buf[0] = PROTO_SET_PIN_MODE;
    buf[1] = b7(pin);
    buf[2] = b7(mode);

    return std::string(buf, 3);
}

std::string firmata::ProtoRequest::setDigitalPinValue(uint8_t pin, uint8_t value)
{
    char buf[3];
    buf[0] = PROTO_SET_DIGITAL_PIN_VALUE;
    buf[1] = b7(pin);
    buf[2] = value ? 1 : 0;

    return std::string(buf, 3);
}

std::string firmata::ProtoRequest::setAnalogPinValue(uint8_t pin, uint32_t value)
{
    static const unsigned int BYTE7 = 0x7F;

    char buf[3];
    buf[0] = PROTO_ANALOG_IO_MESSAGE | (0xF & pin);
    buf[1] = BYTE7 & value;
    buf[2] = BYTE7 & (value >> 7);

    return std::string(buf, 3);
}

std::string firmata::ProtoRequest::setSamplingInterval(uint32_t ms)
{
    char buf[5];
    buf[0] = PROTO_START_SYSEX;
    buf[1] = PROTO_SYSEX_SAMPLING_INTERVAL;
    buf[2] = b7(ms);
    buf[3] = b7(ms >> 7);
    buf[4] = PROTO_SYSEX_END;

    return std::string(buf, 5);
}

std::string firmata::ProtoRequest::queryFirmware()
{
    char buf[3];
    buf[0] = PROTO_START_SYSEX;
    buf[1] = PROTO_SYSEX_REPORT_FIRMWARE;
    buf[2] = PROTO_SYSEX_END;

    return std::string(buf, 3);
}

std::string firmata::ProtoRequest::queryCapabilities()
{
    char buf[3];
    buf[0] = PROTO_START_SYSEX;
    buf[1] = PROTO_SYSEX_CAPABILITY_QUERY;
    buf[2] = PROTO_SYSEX_END;

    return std::string(buf, 3);
}

std::string firmata::ProtoRequest::queryPinState(uint8_t pin)
{
    char buf[4];
    buf[0] = PROTO_START_SYSEX;
    buf[1] = PROTO_SYSEX_PIN_STATE_QUERY;
    buf[2] = pin;
    buf[3] = PROTO_SYSEX_END;

    return std::string(buf, 4);
}

std::string firmata::ProtoRequest::queryAnalogMapping()
{
    char buf[3];
    buf[0] = PROTO_START_SYSEX;
    buf[1] = PROTO_SYSEX_ANALOG_MAPPING_QUERY;
    buf[2] = PROTO_SYSEX_END;

    return std::string(buf, 3);
}

std::string firmata::ProtoRequest::enableAnalogReporting(uint8_t analogPin, bool value)
{
    char buf[2];
    buf[0] = PROTO_REPORT_ANALOG_PIN | (0x0F & analogPin);
    buf[1] = value ? 1 : 0;

    return std::string(buf, 2);
}

std::string firmata::ProtoRequest::enableDigitalReporting(uint8_t port, bool value)
{
    char buf[2];
    buf[0] = PROTO_REPORT_DIGITAL_PORT | (0x0F & port);
    buf[1] = value ? 1 : 0;

    return std::string(buf, 2);
}

std::string firmata::ProtoRequest::uintToByte7(uint32_t v)
{
    static const unsigned int BYTE7 = 0x7F;
    static const unsigned int BYTE14 = 0x3FFF;
    static const unsigned int BYTE21 = 0x1FFFFF;
    static const unsigned int BYTE28 = 0xFFFFFFF;

    std::string res;
    if (v <= BYTE7) {
        res.push_back(v);
    } else if (v <= BYTE14) {
        res.push_back(b7(v));
        res.push_back(b7(v >> 7));
    } else if (v <= BYTE21) {
        res.push_back(b7(v));
        res.push_back(b7(v >> 7));
        res.push_back(b7(v >> 14));
    } else if (v <= BYTE28) {
        res.push_back(b7(v));
        res.push_back(b7(v >> 7));
        res.push_back(b7(v >> 14));
        res.push_back(b7(v >> 21));
    }

    return res;
}
