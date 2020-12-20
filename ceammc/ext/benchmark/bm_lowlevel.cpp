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

#include "fmt/include/fmt/format.h"

#include <boost/unordered_map.hpp>
#include <climits>
#include <cstdio>
#include <map>
#include <nonius/nonius.h++>
#include <random>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>

constexpr int NINT = 5;
std::default_random_engine gen;
std::uniform_int_distribution<int> irand(-1000, 1000);
std::uniform_real_distribution<float> frand(-1, 1);
std::uniform_int_distribution<int> str_rand(0, 2);
std::uniform_int_distribution<int> irand2(0, NINT - 1);

static const char* str[] = {
    "test string 1",
    "test string 2",
    "test string 3"
};

static int s_array[NINT] = { 0 };
static std::vector<int*> s_vec;
static std::unordered_map<int*, int> s_umap;
static boost::unordered_map<int*, int> s_bumap;
static std::map<int*, int> s_map;

int* rand_int_ptr() { return &s_array[irand2(gen)]; }

bool init_data()
{
    for (int i = 0; i < NINT; i++)
        s_array[i] = irand2(gen);

    s_vec.assign(NINT, 0);
    for (int i = 0; i < NINT; i++)
        s_vec[i] = rand_int_ptr();

    for (int i = 0; i < NINT; i++) {
        auto x = rand_int_ptr();
        s_umap[x] = *x;
        s_map[x] = *x;
        s_bumap[x] = *x;
    }

    s_umap.rehash(0);
    s_umap.rehash(1);

    return true;
}

const static bool init = init_data();

NONIUS_BENCHMARK("printf @l%d", [] {
    char buf[1024];
    sprintf(buf, "@l%d", irand(gen));
    return 0;
})

NONIUS_BENCHMARK("std::stringstream @l%d", [] {
    std::stringstream ss;
    ss << "@l" << irand(gen);
    return 0;
})

NONIUS_BENCHMARK("fmt::format @l%d", [] {
    fmt::format("@l{}", irand(gen));
    return 0;
})

NONIUS_BENCHMARK("printf @l%f", [] {
    char buf[1024];
    sprintf(buf, "@l%f", frand(gen));
    return 0;
})

NONIUS_BENCHMARK("std::stringstream @l%f", [] {
    std::stringstream ss;
    ss << "@l" << frand(gen);
    return 0;
})

NONIUS_BENCHMARK("fmt::format @l%f", [] {
    fmt::format("@l{}", frand(gen));
    return 0;
})

NONIUS_BENCHMARK("printf @l%s", [] {
    char buf[1024];
    sprintf(buf, "@l%s", str[str_rand(gen)]);
    return 0;
})

NONIUS_BENCHMARK("strcat @l%s", [] {
    char buf[1024];
    strcpy(buf, "@l");
    strcat(buf, str[str_rand(gen)]);
    return 0;
})

NONIUS_BENCHMARK("std::stringstream @l%s", [] {
    std::stringstream ss;
    ss << "@l" << str[str_rand(gen)];
    return 0;
})

NONIUS_BENCHMARK("fmt::format @l%s", [] {
    fmt::format("@l{}", str[str_rand(gen)]);
    return 0;
})

NONIUS_BENCHMARK("small element vector<int*> access", [] {
    auto x = rand_int_ptr();
    std::find(s_vec.begin(), s_vec.end(), x);
    return 0;
})

NONIUS_BENCHMARK("small element std::map<int*, int> access", [] {
    auto x = rand_int_ptr();
    s_map.find(x);
    return 0;
})

NONIUS_BENCHMARK("small element std::unordered_map<int*, int> access", [] {
    auto x = rand_int_ptr();
    s_umap.find(x);
    return 0;
})

NONIUS_BENCHMARK("small element boost::unordered_map<int*, int> access", [] {
    auto x = rand_int_ptr();
    s_bumap.find(x);
    return 0;
})
