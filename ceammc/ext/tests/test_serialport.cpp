#include "serial/serial.h"

#include "catch.hpp"

#include <iostream>
#include <string>

#include "config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace serial;

TEST_CASE("serial", "[serial]")
{
    SECTION("list_ports")
    {
        std::vector<PortInfo> lst = list_ports();

        for (size_t i = 0; i < lst.size(); i++) {
            PortInfo pi = lst[i];
            REQUIRE(pi.hardware_id != "");
            REQUIRE(pi.description != "");
            REQUIRE(pi.port != "");

            std::cerr << "\tport:        " << pi.port << "\n";
            std::cerr << "\tdescription: " << pi.description << "\n";
            std::cerr << "\tid:          " << pi.hardware_id << "\n";
        }
    }

    SECTION("wine")
    {
#ifdef _WIN32
        auto l = list_ports();

        if(!l.empty()) {
            Serial p(l[0].port);
            REQUIRE(p.isOpen());
        }
#endif
    }
}
