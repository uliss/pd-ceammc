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
#include "ceammc_args.h"
#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_crc32.h"

#include "re2/re2.h"
#include "reflex/matcher.h"

#include <cstdint>
#include <functional>
#include <nonius/nonius.h++>
#include <random>
#include <regex>

using namespace ceammc;

std::default_random_engine engine;
static std::uniform_int_distribution<int8_t> sym_int(0, 7);
static std::uniform_int_distribution<int8_t> list_int(0, 7);

const char* SYM_TABLE[] = {
    "A",
    "ABCDE",
    "asd",
    "a",
    "simple",
    "string",
    "longstring",
    "veryveryveryverylongstring"
};
const AtomList LIST_TABLE[] = {
    { 1, 2, 3 }, { 1 }, {}, { 4, 3, 2, 1, 6 },
    { 6, 7, 4, 3, 7, 8 }, { 1, 1, 1 }, { 2, 3, 4 }, { 1, 9, 6, 4, 3 }
};

static const char* re_pattern = "[a-z].{2,4}";
static re2::RE2 re2_re_precompiled(re_pattern);
static std::regex std_re_precompiled(re_pattern);

extern "C" void pd_init();
bool init() { return pd_init(), true; }
static bool init_done = init();

static ArgChecker arg_checker0("s=test f f");

static bool is_odd0(int n) { return n % 2 == 1; }
static auto is_odd1 = [](int n) { return n % 2 == 1; };
static std::function<bool(int)> is_odd2 = is_odd0;
static std::function<bool(int)> is_odd3 = is_odd1;

NONIUS_BENCHMARK("gensym (x1000)", [] {
    for (size_t i = 0; i < 1000; i++)
        gensym(SYM_TABLE[sym_int(engine)]);
})

NONIUS_BENCHMARK("crc32 (x1000)", [] {
    for (size_t i = 0; i < 1000; i++)
        crc32_hash(SYM_TABLE[sym_int(engine)]);
})

NONIUS_BENCHMARK("std::regex", [] {
    return std::regex_match(SYM_TABLE[sym_int(engine)], std::regex(re_pattern));
})

NONIUS_BENCHMARK("std::regex precompiled", [] {
    return std::regex_match(SYM_TABLE[sym_int(engine)], std_re_precompiled);
})

NONIUS_BENCHMARK("re2", [] {
    return re2::RE2::FullMatch(SYM_TABLE[sym_int(engine)], re2::RE2(re_pattern));
})

NONIUS_BENCHMARK("re2 precompiled", [] {
    return re2::RE2::FullMatch(SYM_TABLE[sym_int(engine)], re2_re_precompiled);
})

NONIUS_BENCHMARK("AtomList slice", [] {
    auto idx = list_int(engine);
    auto from = list_int(engine);
    auto to = list_int(engine);
    return LIST_TABLE[idx].slice(from, to);
})

NONIUS_BENCHMARK("AtomList view", [] {
    auto idx = list_int(engine);
    auto from = list_int(engine);
    auto len = from + list_int(engine);
    AtomListView v(LIST_TABLE[idx]);
    return v.subView(from, len);
})

NONIUS_BENCHMARK("ArgCheck ", [] {
    return arg_checker0.check(AtomList({ 1, 2, 3 }));
})

NONIUS_BENCHMARK("is_odd0: function call", [] {
    bool res = true;
    for (size_t i = 0; i < 1000; i++)
        res ^= is_odd0(sym_int(engine));

    return res;
})

NONIUS_BENCHMARK("is_odd1: lambda call", [] {
    bool res = true;
    for (size_t i = 0; i < 1000; i++)
        res ^= is_odd1(sym_int(engine));

    return res;
})

NONIUS_BENCHMARK("is_odd2: std::function call with function pointer", [] {
    bool res = true;
    for (size_t i = 0; i < 1000; i++)
        res ^= is_odd2(sym_int(engine));

    return res;
})

NONIUS_BENCHMARK("is_odd3: std::function lambda call", [] {
    bool res = true;
    for (size_t i = 0; i < 1000; i++)
        res ^= is_odd3(sym_int(engine));

    return res;
})
