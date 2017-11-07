#include "firmata_lexer.h"
#include "firmata_proto.h"

#include <iostream>

extern "C" {
struct FirmataMessage;
#include "firmata_bison.h"
}

using namespace firmata;

LexerToken firmata::lexer_token_normal(uint8_t c)
{
    LexerToken t;
    t.first.type = -1;
    t.first.value = -1;
    t.second.type = -1;
    t.second.value = -1;

    switch (c) {
    case PROTO_PROTOCOL_VERSION:
        t.first.type = FIRMATA_TOKEN_PROTOCOL_VERSION;
        return t;
    case PROTO_SET_PIN_MODE:
        t.first.type = FIRMATA_TOKEN_SET_PIN_MODE;
        return t;
    case PROTO_SET_DIGITAL_PIN_VALUE:
        t.first.type = FIRMATA_TOKEN_SET_DIGITAL_PIN_VALUE;
        return t;
    case PROTO_START_SYSEX:
        t.first.type = FIRMATA_TOKEN_START_SYSEX;
        return t;
    case PROTO_SYSEX_END:
        t.first.type = FIRMATA_TOKEN_STOP_SYSEX;
        return t;
    // mask types
    default: {
        switch (0xF0 & c) {
        case PROTO_ANALOG_IO_MESSAGE:
            t.first.type = FIRMATA_TOKEN_ANALOG_IO;
            t.second.type = FIRMATA_TOKEN_BYTE7;
            t.second.value = (0x0F & c);
            return t;
        case PROTO_DIGITAL_IO_MESSAGE:
            t.first.type = FIRMATA_TOKEN_DIGITAL_IO;
            t.second.type = FIRMATA_TOKEN_BYTE7;
            t.second.value = 0x0F & c;
            return t;
        default:
            t.first.type = FIRMATA_TOKEN_BYTE7;
            t.first.value = c;
            return t;
        }
    }
    }
}

LexerToken firmata::lexer_token_sysex(uint8_t c)
{
    LexerToken t;
    t.first.type = -1;
    t.first.value = -1;
    t.second.type = -1;
    t.second.value = -1;

    switch (c) {
    case PROTO_SYSEX_REPORT_FIRMWARE:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_REPORT_FIRMWARE;
        return t;
    case PROTO_SYSEX_END:
        t.first.type = FIRMATA_TOKEN_STOP_SYSEX;
        return t;
    case PROTO_SYSEX_STRING_DATA:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_STRING_DATA;
        return t;
    case PROTO_SYSEX_SAMPLING_INTERVAL:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_SAMPLING_INTERVAL;
        return t;
    case PROTO_SYSEX_CAPABILITY_QUERY:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_CAPABILITY_QUERY;
        return t;
    case PROTO_SYSEX_CAPABILITY_RESPONSE:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_CAPABILITY_RESPONSE;
        return t;
    case PROTO_SYSEX_ANALOG_MAPPING_QUERY:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_QUERY;
        return t;
    case PROTO_SYSEX_ANALOG_MAPPING_RESPONSE:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_RESPONSE;
        return t;
    case PROTO_SYSEX_PIN_STATE_QUERY:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_QUERY;
        return t;
    case PROTO_SYSEX_PIN_STATE_RESPONSE:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_RESPONSE;
        return t;
    default:
        t.first.type = FIRMATA_TOKEN_BYTE7;
        t.first.value = c;
        return t;
    }
}

LexerToken firmata::lexer_token_capabilities(uint8_t c)
{
    LexerToken t;
    t.first.type = -1;
    t.first.value = -1;
    t.second.type = -1;
    t.second.value = -1;

    switch (c) {
    case PROTO_SYSEX_CAPABILITY_RESPONSE_END:
        t.first.type = FIRMATA_TOKEN_SYSEX_ID_PIN_CAPABILITY_END;
        return t;
    case PROTO_SYSEX_END:
        t.first.type = FIRMATA_TOKEN_STOP_SYSEX;
        return t;
    default:
        t.first.type = FIRMATA_TOKEN_BYTE7;
        t.first.value = c;
        return t;
    }
}

LexerToken firmata::lexer_token(LexerMode mode, uint8_t c)
{
    // printf("0x%02X ", (int)c);

    switch (mode) {
    case LEXER_MODE_NORMAL:
        return lexer_token_normal(c);
    case LEXER_MODE_SYSEX:
        return lexer_token_sysex(c);
    case LEXER_MODE_CAPABILITIES:
        return lexer_token_capabilities(c);
    case LEXER_MODE_SYSEX_RAW:
        return lexer_token_sysex_raw(c);
    }
}

LexerMode firmata::lexer_next_mode(LexerMode mode, const LexerToken& tok)
{
    // switch normal => sysex
    if (mode == LEXER_MODE_NORMAL && tok.first.type == FIRMATA_TOKEN_START_SYSEX)
        return LEXER_MODE_SYSEX;

    // sysex => normal
    if (mode == LEXER_MODE_SYSEX && tok.first.type == FIRMATA_TOKEN_STOP_SYSEX)
        return LEXER_MODE_NORMAL;

    // sysex => capabilities
    if (mode == LEXER_MODE_SYSEX && tok.first.type == FIRMATA_TOKEN_SYSEX_ID_CAPABILITY_RESPONSE)
        return LEXER_MODE_CAPABILITIES;

    // capabilities => normal
    if (mode == LEXER_MODE_CAPABILITIES && tok.first.type == FIRMATA_TOKEN_STOP_SYSEX)
        return LEXER_MODE_NORMAL;

    // sysex => raw
    if (mode == LEXER_MODE_SYSEX) {
        switch (tok.first.type) {
        case FIRMATA_TOKEN_STOP_SYSEX:
            return LEXER_MODE_NORMAL;
        case FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_RESPONSE:
        case FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_QUERY:
        case FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_QUERY:
        case FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_RESPONSE:
        default:
            return LEXER_MODE_SYSEX_RAW;
        }
    }

    // raw => normal
    if (mode == LEXER_MODE_SYSEX_RAW && tok.first.type == FIRMATA_TOKEN_STOP_SYSEX)
        return LEXER_MODE_NORMAL;

    return mode;
}

SingleLexerToken firmata::lexer_token_reset()
{
    SingleLexerToken t;
    t.type = FIRMATA_TOKEN_ERROR_RESET;
    t.value = -1;
    return t;
}

LexerToken firmata::lexer_token_sysex_raw(uint8_t c)
{
    LexerToken t;
    t.first.type = -1;
    t.first.value = -1;
    t.second.type = -1;
    t.second.value = -1;

    switch (c) {
    case PROTO_SYSEX_END:
        t.first.type = FIRMATA_TOKEN_STOP_SYSEX;
        return t;
    default:
        t.first.type = FIRMATA_TOKEN_BYTE7;
        t.first.value = c;
        return t;
    }
}
