#include "firmata/firmata_pin.h"

#include "test_proto_base.h"

#include <iostream>
#include <sstream>

using namespace firmata;

TEST_CASE("firmata::Pin", "[firmata]")
{
    SECTION("base")
    {
        Pin p(10);
        REQUIRE(p.number() == 10);
        REQUIRE_FALSE(p.isAnalog());
        REQUIRE(p.analogNumber() == -1);
        REQUIRE(p.capabilities().empty());
        REQUIRE(p.mode() == PROTO_PIN_MODE_INPUT);
        REQUIRE_FALSE(p.isSupportedMode(PROTO_PIN_MODE_INPUT));

        p.setNumber(11);
        REQUIRE(p.number() == 11);
        p.setAnalogNumber(15);
        REQUIRE(p.analogNumber() == 15);

        FirmataPinCapability c;
        c.mode = PROTO_PIN_MODE_PWM;
        c.resolution = 14;
        p.addCapability(c);
        REQUIRE(p.isSupportedMode(PROTO_PIN_MODE_PWM));
        REQUIRE(p.mode() == PROTO_PIN_MODE_INPUT);

        p.setValue(-1000);
        REQUIRE(p.value() == -1000);

        REQUIRE(p.setMode(PROTO_PIN_MODE_PWM));
        REQUIRE(p.bitResolution() == 14);

        REQUIRE_FALSE(p.setMode(PROTO_PIN_MODE_ENCODER));
    }

    SECTION("<<")
    {
        std::ostringstream ss;
        ss << Pin(10);

        REQUIRE(ss.str() == "pin #10:\n"
                            "  supported modes:\n");
    }
}
