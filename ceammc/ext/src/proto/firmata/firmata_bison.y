%{
#include <stdio.h>

#include "firmata_proto.h"

void yyerror(FirmataMessage* msg, const char* s) {
    msg->state = STATE_ERROR;
}

#define YYMAXDEPTH 500

unsigned char decode_wchar(int ch, char* out)
{
    if (ch <= 0x7f) {
        if(out) out[0] = ch;
        return 1;
    }
    else if (ch <= 0x7ff)
    {
        if(out) {
            out[0] = (0xc0 | ((ch >> 6) & 0x1f));
            out[1] = (0x80 | (ch & 0x3f));
        }
        return 2;
    }

    return 0;
}

%}

%union {
    FirmataMessage* m;
    int integer;
}

//%define api.value.type {FirmataMessage*}
%defines

%define api.pure full
%define api.push-pull push
%define api.token.prefix {FIRMATA_TOKEN_}
%define api.prefix {firmata_}
%parse-param {FirmataMessage *result}
%token
    ANALOG_IO
    DIGITAL_IO
    PROTOCOL_VERSION
    SET_PIN_MODE
    SET_DIGITAL_PIN_VALUE
    START_SYSEX
    STOP_SYSEX
    ERROR_RESET
    BYTE7
    SYSEX_ID_EXTENDED
    SYSEX_ID_RESERVED
    SYSEX_ID_ANALOG_MAPPING_QUERY
    SYSEX_ID_ANALOG_MAPPING_RESPONSE
    SYSEX_ID_CAPABILITY_QUERY
    SYSEX_ID_CAPABILITY_RESPONSE
    SYSEX_ID_PIN_STATE_QUERY
    SYSEX_ID_PIN_STATE_RESPONSE
    SYSEX_ID_EXTENDED_ANALOG
    SYSEX_ID_STRING_DATA
    SYSEX_ID_REPORT_FIRMWARE
    SYSEX_ID_SAMPLING_INTERVAL
    SYSEX_ID_SYSEX_NON_REALTIME
    SYSEX_ID_SYSEX_REALTIME
    SYSEX_ID_PIN_CAPABILITY_END
;

%type <m> program;
%type <m> message;
%type <m> sysex;
%type <integer> byte7;
%type <integer> pin;
%type <integer> value14;
%type <integer> BYTE7;
%type <integer> sysex_body;
%type <integer> sysex_query_firmware;
%type <integer> sysex_string;
%type <integer> pin_mode;
%type <integer> pin_resolution;
%type <integer> pin_capability;

%%

program     :  message         {
                    result->state = STATE_DONE; }
            |  program message {
                    result->state = STATE_DONE; }
;

message     : ANALOG_IO pin value14 {
                    result->command = PROTO_ANALOG_IO_MESSAGE;
                    result->pin = $2;
                    result->value = $3;
                }
            | DIGITAL_IO pin value14 {
                    result->command = PROTO_DIGITAL_IO_MESSAGE;
                    result->pin = $2;
                    result->value = $3;
                }
            | PROTOCOL_VERSION byte7 byte7 {
                    result->command = PROTO_PROTOCOL_VERSION;
                    result->proto_version.major = $2;
                    result->proto_version.minor = $3;
                }
            | SET_PIN_MODE pin byte7 {
                    result->command = PROTO_SET_PIN_MODE;
                    result->pin = $2;

                    switch($3) {
                    case PROTO_PIN_MODE_INPUT:
                    case PROTO_PIN_MODE_OUTPUT:
                    case PROTO_PIN_MODE_ANALOG:
                    case PROTO_PIN_MODE_PWM:
                    case PROTO_PIN_MODE_SERVO:
                    case PROTO_PIN_MODE_I2C:
                    case PROTO_PIN_MODE_ONEWIRE:
                    case PROTO_PIN_MODE_STEPPER:
                    case PROTO_PIN_MODE_ENCODER:
                    case PROTO_PIN_MODE_SERIAL:
                    case PROTO_PIN_MODE_PULLUP:
                        result->pin_mode = $3;
                        break;
                    default:
                        fprintf(stderr, "invalid pin mode value: %d\n", $3);
                        result->pin_mode = PROTO_PIN_MODE_OUTPUT;
                        break;
                    }
                }
            | SET_DIGITAL_PIN_VALUE pin byte7 {
                    result->command = PROTO_SET_DIGITAL_PIN_VALUE;
                    result->pin = $2;
                    result->value = ($3) ? PROTO_PIN_HIGH : PROTO_PIN_LOW;
                }
            | sysex
            | ERROR_RESET   { result->state = STATE_IN_PROCESS; }
;

pin         : byte7         { $$ = (0xF & $1); }
value14     : byte7 byte7   { $$ = ($1) | ($2 << 7); }
byte7       : BYTE7         { $$ = (0x7F & $1); }

sysex       : START_SYSEX sysex_body STOP_SYSEX {
                    result->command = PROTO_START_SYSEX;
                 }
;

sysex_body  : SYSEX_ID_EXTENDED byte7 sysex_paylod                     { $$ = 0; }
            | SYSEX_ID_RESERVED sysex_paylod                           { $$ = 0; }
            | SYSEX_ID_ANALOG_MAPPING_QUERY                            { $$ = 0; }
            | SYSEX_ID_ANALOG_MAPPING_RESPONSE analog_mapping_response {
                    result->sysex_id = PROTO_SYSEX_ANALOG_MAPPING_RESPONSE;
                }
            | SYSEX_ID_CAPABILITY_QUERY                                { $$ = 0; }
            | SYSEX_ID_CAPABILITY_RESPONSE    capability_response      {
                    result->sysex_id = PROTO_SYSEX_CAPABILITY_RESPONSE;
                }
            | SYSEX_ID_PIN_STATE_QUERY        pin                      { $$ = 0; }
            | SYSEX_ID_PIN_STATE_RESPONSE     pin_state_respone        {
                    result->sysex_id = PROTO_SYSEX_PIN_STATE_RESPONSE;
                }
            | SYSEX_ID_EXTENDED_ANALOG                                 { $$ = 0; }
            | SYSEX_ID_STRING_DATA sysex_string                        {
                    result->sysex_id = PROTO_SYSEX_STRING_DATA;
                }
            | SYSEX_ID_REPORT_FIRMWARE sysex_query_firmware            { $$ = 0; }
            | SYSEX_ID_SAMPLING_INTERVAL sysex_sampling_interval       { $$ = 0; }
            | SYSEX_ID_SYSEX_NON_REALTIME                              { $$ = 0; }
            | SYSEX_ID_SYSEX_REALTIME                                  { $$ = 0; }
;

sysex_paylod: byte7
            | sysex_paylod byte7;

sysex_query_firmware: byte7 byte7 sysex_string {
    result->sysex_id = PROTO_SYSEX_REPORT_FIRMWARE;
    result->proto_version.major = $1;
    result->proto_version.minor = $2;
};

sysex_string: value14  {
                    char buf[2];
                    size_t n = decode_wchar($1, buf);
                    firmata_message_append(result, buf, n);
                }
            | sysex_string value14 {
                    char buf[2];
                    size_t n = decode_wchar($2, buf);
                    firmata_message_append(result, buf, n);
                }
;

sysex_sampling_interval: value14 {
    result->sysex_id = PROTO_SYSEX_SAMPLING_INTERVAL;
    result->value = $1;
};


capability_response :   pin_capability_list
                    |   capability_response  pin_capability_list;

pin_capability_list :   SYSEX_ID_PIN_CAPABILITY_END {
                        result->pin_cap_idx++;
                    }
                    |   pin_capability_chain SYSEX_ID_PIN_CAPABILITY_END {
                        result->pin_cap_idx++;
};

pin_capability_chain  : pin_capability
                      | pin_capability_chain pin_capability;

pin_capability     :    pin_mode pin_resolution  {
                        int n = result->capabilities[result->pin_cap_idx].num;
                        firmata_message_append_pin_capability(result, $1, $2);
};

pin_mode           :    byte7   { $$ = 0x0F & $1; };
pin_resolution     :    byte7   { $$ = $1; };

analog_mapping_response : byte7 {
                            firmata_message_append_analog_mapping(result, $1);
                        }
                        | analog_mapping_response byte7 {
                            firmata_message_append_analog_mapping(result, $2);
                        }
;

pin_state_respone :     pin pin_mode pin_state_bytes7 {
                            result->pin = $1;
                            result->pin_mode = $2;
                        }
;

pin_state_bytes7  :     byte7   {
                            result->pin_state.value = (0x7F & $1);
                            result->pin_state.num_bytes = 1;
                        }
                  |     pin_state_bytes7 byte7 {
                            long v = (((long)(0x7F & $2)) << (result->pin_state.num_bytes * 7));
                            result->pin_state.value += v;
                            result->pin_state.num_bytes++;
                        }
;



