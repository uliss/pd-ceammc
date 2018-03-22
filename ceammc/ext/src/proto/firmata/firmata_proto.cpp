#include "firmata_proto.h"

void firmata_message_append(FirmataMessage* msg, const char* wchar, unsigned char n)
{
    if ((n + msg->str_len) > FIRMATA_MAX_STRING) {
        fprintf(stderr, "[firmata] only %d max characters allowed in string\n", FIRMATA_MAX_STRING);
        return;
    }

    memcpy(msg->str + msg->str_len, wchar, n);
    msg->str_len += n;
}

void firmata_message_append_pin_capability(FirmataMessage* msg, int mode, int resolution)
{
    const int pin = msg->pin_cap_idx;
    if (pin >= MAX_PINS)
        return;

    const int cap_idx = msg->capabilities[pin].num;
    if (cap_idx >= MAX_PIN_CAPS)
        return;

    msg->capabilities[pin].list[cap_idx].mode = (enum FirmataPinMode)mode;
    msg->capabilities[pin].list[cap_idx].resolution = resolution;
    msg->capabilities[pin].num++;
}

void firmata_message_append_analog_mapping(FirmataMessage* msg, int port)
{
    if (msg->cur_pin_map_idx >= MAX_PINS)
        return;

    msg->analog_pin_map[msg->cur_pin_map_idx] = (port == 127) ? -1 : port;

    if (port != 127)
        printf("map pin #%d => A%d\n", msg->cur_pin_map_idx, port);

    msg->cur_pin_map_idx++;
}

const char* firmata_pin_mode_str(FirmataPinMode m)
{
    static const char* names[12] = {
        "INPUT",
        "OUTPUT",
        "ANALOG",
        "PWM",
        "SERVO",
        "SHIFT",
        "I2C",
        "ONEWIRE",
        "STEPPER",
        "ENCODER",
        "SERIAL",
        "PULLUP"
    };

    return names[m];
}
