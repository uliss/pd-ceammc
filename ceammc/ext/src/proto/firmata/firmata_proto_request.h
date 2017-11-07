#ifndef FIRMATA_PROTO_REQUEST_H
#define FIRMATA_PROTO_REQUEST_H

#include "firmata_proto.h"

#include <stdint.h>
#include <string>

namespace firmata {

class ProtoRequest {
    ProtoRequest();

public:
    static std::string setDigitalPinMode(uint8_t pin, FirmataPinMode mode);
    static std::string setDigitalPinValue(uint8_t pin, uint8_t value);
    static std::string setAnalogPinValue(uint8_t pin, uint32_t value);
    static std::string setSamplingInterval(uint32_t ms);

    static std::string queryFirmware();
    static std::string queryCapabilities();
    static std::string queryPinState(uint8_t pin);

    /**
     * Analog messages are numbered 0 to 15, which traditionally refer to the
     * Arduino pins labeled A0, A1, A2, etc.
     *
     * However, these pis are actually configured using "normal" pin numbers in the pin mode message,
     * and when those pins are used for non-analog functions. The analog mapping query provides
     * the information about which pins (as used with Firmata's pin mode message) correspond
     * to the analog channels.
     */
    static std::string queryAnalogMapping();

    static std::string enableAnalogReporting(uint8_t analogPin, bool value = true);
    static std::string enableDigitalReporting(uint8_t port, bool value = true);

    static std::string uintToByte7(uint32_t v);
};
}

#endif // FIRMATA_PROTO_REQUEST_H
