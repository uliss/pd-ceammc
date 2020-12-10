#include "../proto/firmata/firmata_lexer.h"
#include "../proto/firmata/firmata_parser.h"
#include "../proto/firmata/firmata_proto.h"
#include "../proto/firmata/firmata_proto_request.h"

extern "C" {
#include "../proto/firmata/firmata_bison.h"
}

#include "catch.hpp"

#include <boost/static_assert.hpp>
#include <cstring>
#include <iostream>

using namespace firmata;

TEST_CASE("firmata_parser", "[firmata]")
{
    SECTION("bison")
    {
        FirmataParser p;
        REQUIRE_FALSE(p.isDone());

        // protocol version
        p << PROTO_PROTOCOL_VERSION << 2 << 5;
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        REQUIRE(p.msg().proto_version.major == 2);
        REQUIRE(p.msg().proto_version.minor == 5);
        REQUIRE(p.isDone());

        // we should reset parser
        p << PROTO_ANALOG_IO_MESSAGE << 2 << 6;
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        p.reset();

        // analog value
        p << (PROTO_ANALOG_IO_MESSAGE | 4) << '\x02' << '\x06';
        REQUIRE(p.msg().command == PROTO_ANALOG_IO_MESSAGE);
        REQUIRE(p.msg().pin == 4);
        REQUIRE(p.value() == 770);
        REQUIRE(p.isDone());
        p.reset();

        // STACK overflow test
        // should not overflow because of left recursion used
        // default stack size == 500
        int n = 1000;
        while (n-- > 0) {
            // digital pin value
            p << (PROTO_DIGITAL_IO_MESSAGE | 2) << '\x03' << '\x06';
            REQUIRE(p.msg().command == PROTO_DIGITAL_IO_MESSAGE);
            REQUIRE(p.msg().pin == 2);
            REQUIRE(p.value() == 771);
            REQUIRE(p.isDone());
            p.reset();
            REQUIRE_FALSE(p.isDone());
        }

        // error test
        p << 'e' << 'r' << 'r' << 'o' << 'r';
        REQUIRE(p.isError());
        REQUIRE_FALSE(p.isDone());

        p.reset();
        REQUIRE_FALSE(p.isError());

        p << PROTO_PROTOCOL_VERSION << '\x02' << '\x05';
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        REQUIRE(p.msg().proto_version.major == 2);
        REQUIRE(p.msg().proto_version.minor == 5);
        REQUIRE_FALSE(p.isError());

        p.reset();

        // set pin mode
        p << PROTO_SET_PIN_MODE << 8 << PROTO_PIN_MODE_ONEWIRE;
        REQUIRE(p.isDone());
        REQUIRE(p.msg().pin == 8);
        REQUIRE(p.msg().pin_mode == PROTO_PIN_MODE_ONEWIRE);

        p.reset();

        // set pin value
        p << PROTO_SET_DIGITAL_PIN_VALUE << 10 << PROTO_PIN_HIGH;
        REQUIRE(p.isDone());
        REQUIRE(p.msg().pin == 10);
        REQUIRE(p.msg().value == PROTO_PIN_HIGH);

        p.reset();

        // sysex

        // sysex firmware
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_REPORT_FIRMWARE << 3 << 19 << 'A' << 0 << 'B' << 0 << 'C' << 0
          << PROTO_SYSEX_END;

        REQUIRE(p.isSysex());
        REQUIRE(p.msg().sysex_id == PROTO_SYSEX_REPORT_FIRMWARE);
        REQUIRE(p.msg().proto_version.major == 3);
        REQUIRE(p.msg().proto_version.minor == 19);
        REQUIRE(p.str() == "ABC");
        REQUIRE(p.isDone());

        p.reset();

        // sysex string data
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_STRING_DATA << 'D' << 0 << 'E' << 0 << 'F' << 0
          << PROTO_SYSEX_END;

        REQUIRE(p.isDone());
        REQUIRE(p.str() == "DEF");

        p.reset();

        // overflow check
        REQUIRE(p.str() == "");

        p << PROTO_START_SYSEX << PROTO_SYSEX_STRING_DATA;
        for (int i = 0; i < 40; i++) {
            p << 'A' << 0;
        }
        p << PROTO_SYSEX_END;

        REQUIRE(p.str() == std::string(32, 'A'));

        p.reset();

        // sysex sampling interval
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_SAMPLING_INTERVAL << 23 << 32 << PROTO_SYSEX_END;
        REQUIRE(p.isDone());
        REQUIRE(p.msg().sysex_id == PROTO_SYSEX_SAMPLING_INTERVAL);
        REQUIRE(p.value() == 4119);

        p.reset();

        // sysex capabilities
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_CAPABILITY_RESPONSE
          << 0 << 2 << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // sysex capabilities
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_CAPABILITY_RESPONSE
          << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // sysex capabilities
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_CAPABILITY_RESPONSE
          << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // sysex capabilities
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_CAPABILITY_RESPONSE
          << 0 << 2 << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // sysex capabilities
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_CAPABILITY_RESPONSE
          << 0 << 2 << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << 0 << 2 << 3 << 8 << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // sysex capabilities
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_CAPABILITY_RESPONSE
          << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << 0 << 2 << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << 3 << 0 << 2 << 1 << PROTO_SYSEX_CAPABILITY_RESPONSE_END
          << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // analog mapping
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_ANALOG_MAPPING_RESPONSE
          << 1 << 2 << 127 << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();

        // pin state query
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_PIN_STATE_QUERY
          << 12 << PROTO_SYSEX_END;
        REQUIRE(p.isDone());

        p.reset();
        // pin state response
        p << PROTO_START_SYSEX
          << PROTO_SYSEX_PIN_STATE_RESPONSE
          << 8 << PROTO_PIN_MODE_I2C << 1 << 2 << 4 << PROTO_SYSEX_END;
        REQUIRE(p.isDone());
        REQUIRE(p.msg().sysex_id == PROTO_SYSEX_PIN_STATE_RESPONSE);
        REQUIRE(p.msg().pin == 8);
        REQUIRE(p.msg().pin_mode == PROTO_PIN_MODE_I2C);
        REQUIRE(p.msg().pin_state.value == (1 + (2 << 7) + (4 << 14)));
    }

    SECTION("lexer")
    {
        LexerMode m = LEXER_MODE_NORMAL;
        LexerToken t = lexer_token(m, PROTO_START_SYSEX);

        REQUIRE(t.first.type == FIRMATA_TOKEN_START_SYSEX);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_SYSEX);

        t = lexer_token(m, PROTO_SYSEX_CAPABILITY_RESPONSE);
        REQUIRE(t.first.type == FIRMATA_TOKEN_SYSEX_ID_CAPABILITY_RESPONSE);

        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_CAPABILITIES);

        t = lexer_token(m, PROTO_SYSEX_CAPABILITY_RESPONSE_END);
        REQUIRE(t.first.type == FIRMATA_TOKEN_SYSEX_ID_PIN_CAPABILITY_END);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_CAPABILITIES);

        t = lexer_token(m, PROTO_SYSEX_CAPABILITY_RESPONSE_END);
        REQUIRE(t.first.type == FIRMATA_TOKEN_SYSEX_ID_PIN_CAPABILITY_END);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_CAPABILITIES);

        t = lexer_token(m, 0x16);
        REQUIRE(t.first.type == FIRMATA_TOKEN_BYTE7);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_CAPABILITIES);

        t = lexer_token(m, 0x1);
        REQUIRE(t.first.type == FIRMATA_TOKEN_BYTE7);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_CAPABILITIES);

        t = lexer_token(m, PROTO_SYSEX_CAPABILITY_RESPONSE_END);
        REQUIRE(t.first.type == FIRMATA_TOKEN_SYSEX_ID_PIN_CAPABILITY_END);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_CAPABILITIES);

        t = lexer_token(m, PROTO_SYSEX_END);
        REQUIRE(t.first.type == FIRMATA_TOKEN_STOP_SYSEX);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_NORMAL);

        t = lexer_token(LEXER_MODE_SYSEX, PROTO_SYSEX_ANALOG_MAPPING_RESPONSE);
        REQUIRE(t.first.type == FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_RESPONSE);
        m = lexer_next_mode(LEXER_MODE_SYSEX, t);
        REQUIRE(m == LEXER_MODE_SYSEX_RAW);

        t = lexer_token(m, PROTO_SYSEX_END);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_NORMAL);
    }

    SECTION("pin_mode_str")
    {
        BOOST_STATIC_ASSERT(PROTO_PIN_MODE_INPUT < PROTO_PIN_MODE_PULLUP);

#define REQUIRE_PIN_MODE(mode)                                                                      \
    {                                                                                               \
        REQUIRE(std::string(firmata_pin_mode_str((FirmataPinMode)PROTO_PIN_MODE_##mode)) == #mode); \
    }

        REQUIRE_PIN_MODE(INPUT);
        REQUIRE_PIN_MODE(OUTPUT);
        REQUIRE_PIN_MODE(ANALOG);
        REQUIRE_PIN_MODE(PWM);
        REQUIRE_PIN_MODE(SERVO);
        REQUIRE_PIN_MODE(SHIFT);
        REQUIRE_PIN_MODE(I2C);
        REQUIRE_PIN_MODE(ONEWIRE);
        REQUIRE_PIN_MODE(STEPPER);
        REQUIRE_PIN_MODE(ENCODER);
        REQUIRE_PIN_MODE(SERIAL);
        REQUIRE_PIN_MODE(PULLUP);

        for (int i = PROTO_PIN_MODE_INPUT; i <= PROTO_PIN_MODE_PULLUP; i++) {
            REQUIRE(strlen(firmata_pin_mode_str((FirmataPinMode)i)) != 0);
        }
    }

    SECTION("pin state query")
    {
        LexerMode m = LEXER_MODE_NORMAL;
        LexerToken t = lexer_token(m, PROTO_START_SYSEX);

        REQUIRE(t.first.type == FIRMATA_TOKEN_START_SYSEX);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_SYSEX);

        t = lexer_token(m, PROTO_SYSEX_PIN_STATE_RESPONSE);
        REQUIRE(t.first.type == FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_RESPONSE);

        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_SYSEX_RAW);

        t = lexer_token(m, PROTO_SYSEX_END);
        m = lexer_next_mode(m, t);
        REQUIRE(m == LEXER_MODE_NORMAL);
    }

    SECTION("proto request")
    {
        SECTION("uintToByte7")
        {
            unsigned int v = 0;
            REQUIRE(ProtoRequest::uintToByte7(0x0) == std::string(1, '\x00'));
            REQUIRE(ProtoRequest::uintToByte7(0x01) == "\x01");
            REQUIRE(ProtoRequest::uintToByte7(0x10) == "\x10");
            REQUIRE(ProtoRequest::uintToByte7(0x7D) == "\x7D");
            v = 0x7f;
            REQUIRE(ProtoRequest::uintToByte7(v) == "\x7F");
            REQUIRE(ProtoRequest::uintToByte7(v + 1) == std::string("\x00\x01", 2));

            REQUIRE(ProtoRequest::uintToByte7(0x81) == std::string("\x01\x01", 2));
            REQUIRE(ProtoRequest::uintToByte7(0x82) == std::string("\x02\x01", 2));

            v = 0x3fff;
            REQUIRE(ProtoRequest::uintToByte7(v - 1) == std::string("\x7E\x7F", 2));
            REQUIRE(ProtoRequest::uintToByte7(v + 0) == std::string("\x7F\x7F", 2));
            REQUIRE(ProtoRequest::uintToByte7(v + 1) == std::string("\x00\x00\x01", 3));

            v = 0x1fffff;
            REQUIRE(ProtoRequest::uintToByte7(v - 1) == std::string("\x7E\x7F\x7F", 3));
            REQUIRE(ProtoRequest::uintToByte7(v + 0) == std::string("\x7F\x7F\x7F", 3));
            REQUIRE(ProtoRequest::uintToByte7(v + 1) == std::string("\x00\x00\x00\x01", 4));

            v = 0xfffffff;
            REQUIRE(ProtoRequest::uintToByte7(v - 1) == std::string("\x7E\x7F\x7F\x7F", 4));
            REQUIRE(ProtoRequest::uintToByte7(v + 0) == std::string("\x7F\x7F\x7F\x7F", 4));
            REQUIRE(ProtoRequest::uintToByte7(v + 1) == std::string(""));
        }
    }
}
