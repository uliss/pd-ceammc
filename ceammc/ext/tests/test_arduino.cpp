#include "../hw/arduino/arduino.h"
#include "../hw/arduino/arduino_thread.h"

#include "ceammc_platform.h"

#include "catch.hpp"
#include "test_base.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace ceammc::hw;
using namespace ceammc::platform;

inline void sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

TEST_CASE("hw::Arduino", "[hw]")
{
    SECTION("init")
    {
        Arduino a;
        REQUIRE(a.port().empty());
        REQUIRE(a.usbSerial().empty());
        REQUIRE(a.vendorId() == 0);
        REQUIRE(a.productId() == 0);

        REQUIRE(a.start());
        // start twice
        REQUIRE_FALSE(a.start());

        sleep_ms(4000);

        if (!a.isConnected())
            return;

        REQUIRE(a.isConnected());
        REQUIRE(a.vendorId() != 0);
        REQUIRE(a.productId() != 0);
        REQUIRE(a.usbSerial() != "");

        ArduinoData data;
        a.read(data);
        sleep_ms(100);
        a.write("\xF9");
        sleep_ms(100);

        data.clear();
        a.read(data);

        REQUIRE(data.size() == 3);
        REQUIRE(data[0] == 0xF9);
        REQUIRE(data[1] == 0x2);
        REQUIRE(data[2] == 0x5);

        a.stop();
        sleep_ms(100);
    }

    SECTION("set")
    {
        Arduino a;
        REQUIRE(a.port().empty());
        REQUIRE(a.usbSerial().empty());
        REQUIRE(a.vendorId() == 0);
        REQUIRE(a.productId() == 0);

        a.setPort("abc");
        REQUIRE(a.port() == "abc");

        a.setUsbSerial("123");
        REQUIRE(a.usbSerial() == "123");

        a.setVendorId(1000);
        REQUIRE(a.vendorId() == 1000);

        a.setProductId(2000);
        REQUIRE(a.productId() == 2000);
    }

    SECTION("messages")
    {
        Arduino a;
        REQUIRE_FALSE(a.hasMessages());
        a.pushError("error");
        a.pushDebug("debug");
        REQUIRE(a.hasMessages());
        Messages lst = a.popMessages();
        REQUIRE_FALSE(a.hasMessages());
        REQUIRE(lst.size() == 2);
        REQUIRE(lst[0].first == MSG_ERROR);
        REQUIRE(lst[0].second == "error");
        REQUIRE(lst[1].first == MSG_DEBUG);
        REQUIRE(lst[1].second == "debug");
    }
}
