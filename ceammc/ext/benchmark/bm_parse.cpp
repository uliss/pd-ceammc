/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "m_pd.h"
#include "net/parser_osc.h"
#include "nonius/nonius.h++"

#include <ctime>
#include <random>

extern "C" void pd_init();

using namespace ceammc;
using namespace ceammc::parser;

constexpr auto URL_COUNT = 5;
const char* URL_LIST[URL_COUNT] = {
    "osc.tcp://:1000",
    "osc://:2000",
    "osc.tcp://hostname.com:3000",
    "osc://hostname.com:4000",
    "osc://127.0.0.1",
};

std::default_random_engine gen(std::time(nullptr));
std::uniform_int_distribution<int8_t> dist_url(0, URL_COUNT - 1);

namespace {
char* lo_url_get_port2(const char* url)
{
    char* port = (char*)malloc(strlen(url));

    if (sscanf(url, "osc://%*[^:]:%[0-9]", port) > 0) {
        return port;
    }
    if (sscanf(url, "osc.%*[^:]://%*[^:]:%[0-9]", port) > 0) {
        return port;
    }
    if (sscanf(url, "osc://[%*[^]]]:%[0-9]", port) > 0) {
        return port;
    }
    if (sscanf(url, "osc.%*[^:]://[%*[^]]]:%[0-9]", port) > 0) {
        return port;
    }
    if (sscanf(url, "osc://:%[0-9]", port) > 0) {
        return port;
    }
    if (sscanf(url, "osc.%*[^:]://:%[0-9]", port) > 0) {
        return port;
    }

    /* doesnt look like an OSC URL with port number */
    free(port);

    return NULL;
}

t_symbol* lo_url_get_port(const char* url)
{
    auto port = lo_url_get_port2(url);
    if (port) {
        auto sym = gensym(port);
        free(port);
        return sym;
    } else
        return &s_;
}

inline const char* random_url()
{
    return URL_LIST[dist_url(gen)];
}

const bool init = []() { pd_init(); return true; }();
}

NONIUS_BENCHMARK("lo_url_get_port (sscanf)", [] {
    lo_url_get_port(random_url());
})

NONIUS_BENCHMARK("parse::parse_osc_url (ragel)", [] {
    t_symbol* proto;
    t_symbol* host;
    t_symbol* port;
    parse_osc_url(random_url(), proto, host, port);
})
