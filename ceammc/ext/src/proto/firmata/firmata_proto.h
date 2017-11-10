#ifndef FIRMATA_PROTO_H
#define FIRMATA_PROTO_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

enum FirmataProtoCommand {
    PROTO_ANALOG_IO_MESSAGE = 0xE0,
    PROTO_DIGITAL_IO_MESSAGE = 0x90,
    PROTO_REPORT_ANALOG_PIN = 0xC0,
    PROTO_REPORT_DIGITAL_PORT = 0xD0,
    PROTO_START_SYSEX = 0xF0,
    PROTO_SET_PIN_MODE = 0xF4,
    PROTO_SET_DIGITAL_PIN_VALUE = 0xF5,
    PROTO_SYSEX_END = 0xF7,
    PROTO_PROTOCOL_VERSION = 0xF9,
    PROTO_SYSTEM_RESET = 0xFF
};

enum FirmataProtoSysex {
    PROTO_SYSEX_REPORT_FIRMWARE = 0x79,
    PROTO_SYSEX_STRING_DATA = 0x71,
    PROTO_SYSEX_SAMPLING_INTERVAL = 0x7A,
    PROTO_SYSEX_CAPABILITY_QUERY = 0x6B,
    PROTO_SYSEX_CAPABILITY_RESPONSE = 0x6C,
    PROTO_SYSEX_CAPABILITY_RESPONSE_END = 0x7F,
    PROTO_SYSEX_ANALOG_MAPPING_QUERY = 0x69,
    PROTO_SYSEX_ANALOG_MAPPING_RESPONSE = 0x6A,
    /*
     * The pin state is any data written to the pin (it is important to note that pin state != pin value).
     * For output modes (digital output, PWM, and Servo), the state is any value that has been
     * previously written to the pin.
     * For input modes, typically the state is zero.
     * However, for digital inputs, the state is the status of the pull-up resistor which is 1 if enabled,
     * 0 if disabled.
     *
     * The pin state query can also be used as a verification after sending pin modes or data messages.
     */
    PROTO_SYSEX_PIN_STATE_QUERY = 0x6D,
    PROTO_SYSEX_PIN_STATE_RESPONSE = 0x6E
};

enum FirmataPinMode {
    PROTO_PIN_MODE_INPUT = 0,
    PROTO_PIN_MODE_OUTPUT = 1,
    PROTO_PIN_MODE_ANALOG = 2,
    PROTO_PIN_MODE_PWM = 3,
    PROTO_PIN_MODE_SERVO = 4,
    PROTO_PIN_MODE_SHIFT = 5,
    PROTO_PIN_MODE_I2C = 6,
    PROTO_PIN_MODE_ONEWIRE = 7,
    PROTO_PIN_MODE_STEPPER = 8,
    PROTO_PIN_MODE_ENCODER = 9,
    PROTO_PIN_MODE_SERIAL = 10,
    PROTO_PIN_MODE_PULLUP = 11
};

enum {
    MAX_PINS = 70, // Arduino MEGA 2560
    MAX_PIN_CAPS = 16
};

enum FirmataDigitalPinState {
    PROTO_PIN_LOW = 0,
    PROTO_PIN_HIGH = 1
};

enum FirmataParseState {
    STATE_IN_PROCESS = 0,
    STATE_DONE,
    STATE_ERROR
};

typedef struct FirmataProtoVersion {
    unsigned char major;
    unsigned char minor;
} FirmataProtoVersion;

typedef struct FirmataPinCapability {
    enum FirmataPinMode mode;
    unsigned char resolution;
} FirmataPinCapability;

typedef struct FirmataPinCapabilities {
    FirmataPinCapability list[MAX_PIN_CAPS];
    int num;
} FirmataPinCapabilities;

enum {
    FIRMATA_MAX_STRING = 32
};

typedef struct FirmataPinState {
    long value;
    int num_bytes;
} FirmataPinState;

typedef struct FirmataMessage {
    int value;
    int type;
    FirmataProtoVersion proto_version;
    enum FirmataProtoCommand command;
    enum FirmataPinMode pin_mode;
    enum FirmataProtoSysex sysex_id;
    unsigned char pin;
    unsigned char byte;
    unsigned char state;
    unsigned char str_len;
    char str[FIRMATA_MAX_STRING];
    FirmataPinCapabilities capabilities[MAX_PINS];
    int pin_cap_idx;
    signed char analog_pin_map[MAX_PINS];
    int cur_pin_map_idx;
    FirmataPinState pin_state;
} FirmataMessage;

void firmata_message_append(FirmataMessage* msg, const char* wchar, unsigned char n);
void firmata_message_append_pin_capability(FirmataMessage* msg, int mode, int resolution);
void firmata_message_append_analog_mapping(FirmataMessage* msg, int port);

const char* firmata_pin_mode_str(enum FirmataPinMode m);

#ifdef __cplusplus
}
#endif

#endif // FIRMATA_PROTO_H
