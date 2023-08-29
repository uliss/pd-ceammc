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
#include "ceammc_function.h"
#include "ceammc_convert.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_platform.h"
#include "ceammc_rtree.h"
#include "fmt/core.h"
#include "muParser.h"

#include <algorithm>
#include <cmath>

constexpr size_t MAX_LIST_LEN = 256;

namespace {

using namespace ceammc;

AtomList fn_seq(const AtomListView& a)
{
    constexpr int MAXN = 1024;

    const char* usage = "usage: seq(FROM TO [STEP])";

    if (a.empty()) {
        LIB_ERR << fmt::format("seq(): empty arguments, {}", usage);
        return {};
    } else if (a.size() == 1 && a[0].isFloat())
        return a[0];
    else if (a.size() == 2 && a[0].isFloat() && a[1].isFloat()) { // seq(FROM TO)
        const auto from = a[0].asT<t_float>();
        const auto to = a[1].asT<t_float>();
        const int n = std::abs(from - to) + 1;

        if (n == 1)
            return { from };
        else if (n > MAXN) {
            LIB_ERR << fmt::format("seq(): generated list size is too big: {}, max size is: {}", n, MAXN);
            return { from, to };
        } else {
            AtomList res;
            res.reserve(n);

            if (from < to) { // seq(-10 10)
                for (int i = 0; i < n; i++)
                    res.append(from + i);
            } else {
                for (int i = 0; i < n; i++)
                    res.append(from - i);
            }

            return res;
        }
    } else if (a.size() == 3 && a[0].isFloat() && a[1].isFloat() && a[2].isFloat()) { // seq(FROM TO STEP)
        const auto from = a[0].asT<t_float>();
        const auto to = a[1].asT<t_float>();
        const auto step = a[2].asT<t_float>();
        if (step <= 0) {
            LIB_ERR << fmt::format("seq(): invalid step value: {}, should be >0", step);
            return {};
        }

        const int n = (std::abs(from - to) / step) + 1;

        AtomList res;
        res.reserve(n);

        if (n == 1)
            return a[0];
        else if (from < to) {
            for (int i = 0; i < n; i++)
                res.append(from + i * step);
        } else {
            for (int i = 0; i < n; i++)
                res.append(from - i * step);
        }

        return res;

    } else {
        LIB_ERR << fmt::format("seq(): invalid arguments: '{}', {}", to_string(a), usage);
        return {};
    }
}

AtomList fn_db2amp(const AtomListView& args)
{
    if (!args.isFloat()) {
        LIB_ERR << fmt::format("db2amp(): float expected, got: '{}'", to_string(args));
        return { 0. };
    }

    return { convert::dbfs2amp(args[0].asT<t_float>()) };
}

AtomList fn_amp2db(const AtomListView& args)
{
    if (!args.isFloat()) {
        LIB_ERR << fmt::format("amp2db(): float expected, got: '{}'", to_string(args));
        return { -144 };
    }

    const auto f = args[0].asT<t_float>();
    if (f <= 0) {
        LIB_ERR << fmt::format("amp2db(): invalid amplitude value: '{}', should be >0", f);
        return { -144 };
    }

    return { convert::amp2dbfs(f) };
}

AtomList fn_repeat(const AtomListView& args)
{
    constexpr size_t MAX_RESULT_LEN = 1024;

    if (args.size() < 2) {
        LIB_ERR << fmt::format("usage: repeat(N, args...)");
        return {};
    }

    const auto N = args.intAt(0, 0);
    if (N <= 0) {
        LIB_ERR << fmt::format("repeat(): positive number of repeats expected, got: {}", to_string(args[0]));
        return {};
    }

    const auto RESULT_N = args.size() * N;
    if (RESULT_N > MAX_RESULT_LEN) {
        LIB_ERR << fmt::format("repeat():result argument list is too long: {}, max length is {}", RESULT_N, MAX_RESULT_LEN);
        return {};
    }

    return list::repeat(args.subView(1), N);
}

AtomList fn_reverse(const AtomListView& args)
{
    AtomList res(args);
    res.reverse();
    return res;
}

#define FN_RTREE "rtree"
#define FN_EUCLID "euclid"
#define FN_HEXBEAT "hexbeat"
#define FN_ROTATE "rotate"
#define FN_ONES "ones"
#define FN_ZEROS "zeros"

AtomList fn_rhythm_tree(const AtomListView& args)
{
    const bool ok = args.size() == 2 && args[0].isFloat() && args[1].isDataType(DataTypeMList::staticType());
    if (!ok) {
        LIB_ERR << fmt::format(FN_RTREE "(): invalid arguments, usage: " FN_RTREE "(DUR MList), got: {}", to_string(args));
        return {};
    }

    const auto len = args[0].asT<t_float>();
    auto ml = args[1].asD<DataTypeMList>();
    return rtree::rhythm_tree(len, ml, FN_RTREE "(): ", LIB_ERR);
}

AtomList fn_euclid(const AtomListView& args)
{
    const bool ok = (args.size() >= 1 && args.size() <= 3)
        && args[0].isInteger();

    if (!ok) {
        LIB_ERR << fmt::format(FN_EUCLID "(): usage " FN_EUCLID "(BEATS N=8? ROTATE=0?)");
        return {};
    }

    const auto onsets = args[0].asT<int>();
    const auto pulses = clip_max<int>(args.intAt(1, 8), MAX_LIST_LEN);
    const auto rotate = args.intAt(2, 0);

    if (onsets < 0) {
        LIB_ERR << fmt::format(FN_EUCLID "(): number of onsets should be >0, got {}", onsets);
        return {};
    }

    if (pulses < 1) {
        LIB_ERR << fmt::format(FN_EUCLID "(): pattern length should be >1, got {}", pulses);
        return {};
    }

    if (onsets > pulses) {
        LIB_ERR << fmt::format(FN_EUCLID "(): number of pulses should be <={}, got {}", pulses, onsets);
        return {};
    }

    if (!rotate)
        return list::bresenham(onsets, pulses);
    else
        return list::rotate(list::bresenham(onsets, pulses), -rotate);
}

AtomList fn_hexbeat(const AtomListView& args)
{
    if (!args.isSymbol()) {
        LIB_ERR << fmt::format(FN_HEXBEAT "(): usage " FN_HEXBEAT "(HEX)");
        return {};
    }

    return list::hexbeat_bin(args[0].asT<t_symbol*>()->s_name);
}

AtomList fn_rotate(const AtomListView& args)
{
    const auto ok = args.size() > 1 && args[0].isFloat();
    if (!ok) {
        LIB_ERR << fmt::format(FN_ROTATE "(): usage " FN_ROTATE "(N args...)");
        return {};
    }

    return list::rotate(args.subView(1), args[0].asFloat());
}

AtomList fn_ones(const AtomListView& args)
{
    const auto ok = (args.size() == 1)
        && args[0].isInteger()
        && (args[0].asInt() >= 0);

    if (!ok) {
        LIB_ERR << fmt::format(FN_ONES "(): usage " FN_ONES "(N)");
        return {};
    }

    return AtomList::ones(clip_max<int>(args[0].asInt(), MAX_LIST_LEN));
}

AtomList fn_zeros(const AtomListView& args)
{
    const auto ok = (args.size() == 1)
        && args[0].isInteger()
        && (args[0].asInt() >= 0);

    if (!ok) {
        LIB_ERR << fmt::format(FN_ZEROS "(): usage " FN_ZEROS "(N)");
        return {};
    }

    return AtomList::zeroes(clip_max<int>(args[0].asInt(), MAX_LIST_LEN));
}

}

namespace ceammc {

BuiltinFunctionMap::BuiltinFunctionMap()
    : math_parser_(new mu::Parser)
{
    using MuParserFn = double (*)(double);
    static const t_float m_pi = std::acos(t_float(-1));
    static const t_float m_exp = std::exp(t_float(1));

    math_parser_->DefineNameChars("$0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    //    math_parser_->SetArgSep(' ');
    math_parser_->DefineConst("sr", sys_getsr());
    math_parser_->DefineConst("bs", sys_getblksize());
    math_parser_->DefineConst("e", m_exp);
    math_parser_->DefineConst("pi", m_pi);
    math_parser_->DefineFun("floor", static_cast<MuParserFn>(std::floor));
    math_parser_->DefineFun("ceil", static_cast<MuParserFn>(std::ceil));

    fn_map_.reserve(32);

    registerFn(gensym("seq"), fn_seq);
    registerFn(gensym("expr"),
        [this](const AtomListView& expr) -> AtomList {
            const std::string str = to_string(expr, " ");
            try {
                math_parser_->SetExpr(str);
                const t_float res = math_parser_->Eval();
                return { res };
            } catch (mu::Parser::exception_type& e) {
                LIB_ERR << fmt::format("epxr() error: {} in expression '{}'", e.GetMsg(), e.GetExpr());
                return {};
            }
        });

    registerFn(gensym("env"), [](const AtomListView& name) -> AtomList {
        if (!name.isSymbol()) {
            LIB_ERR << fmt::format("env(): environment variable name expected, got '{}'. usage: env(NAME)", to_string(name));
            return {};
        }

        const auto venv = platform::expandenv(name[0].asT<t_symbol*>()->s_name);
        return { gensym(venv.c_str()) };
    });

    registerFn(gensym("pi"), [](const AtomListView& lv) -> AtomList { return { m_pi * lv.floatAt(0, 1) }; });
    registerFn(gensym("e"), [](const AtomListView&) -> AtomList { return { m_exp }; });
    registerFn(gensym("sr"), [](const AtomListView&) -> AtomList { return { sys_getsr() }; });
    registerFn(gensym("bs"), [](const AtomListView&) -> AtomList { return { (t_float)sys_getblksize() }; });
    registerFn(gensym("mtof"),
        [](const AtomListView& a) -> AtomList {
            AtomList res;
            a.mapFloat([](t_float f) { return convert::midi2freq(f); }, res);
            return res;
        });
    registerFn(gensym("ftom"), [](const AtomListView& a) -> AtomList { return { convert::freq2midi(a.floatAt(0, 0)) }; });
    registerFn(gensym("ms2bpm"), [](const AtomListView& a) -> AtomList { return { t_float(60000 / a.floatAt(0, 1)) }; });
    registerFn(gensym("bpm2ms"), [](const AtomListView& a) -> AtomList { return { t_float(60000 / a.floatAt(0, 1)) }; });
    registerFn(gensym("db2amp"), fn_db2amp);
    registerFn(gensym("amp2db"), fn_amp2db);
    registerFn(gensym("repeat"), fn_repeat);
    registerFn(gensym("reverse"), fn_reverse);
    registerFn(gensym("sort"), [](const AtomListView& lv) -> AtomList {  AtomList res(lv); res.sort(); return res; });
    registerFn(gensym(FN_RTREE), fn_rhythm_tree);
    registerFn(gensym(FN_EUCLID), fn_euclid);
    registerFn(gensym(FN_HEXBEAT), fn_hexbeat);
    registerFn(gensym(FN_ROTATE), fn_rotate);
    registerFn(gensym(FN_ONES), fn_ones);
    registerFn(gensym(FN_ZEROS), fn_zeros);
}

BuiltinFunctionMap::~BuiltinFunctionMap() = default;

BuiltinFunctionMap& BuiltinFunctionMap::instance()
{
    static BuiltinFunctionMap instance_;
    return instance_;
}

AtomList BuiltinFunctionMap::call(t_symbol* name, const AtomListView& args) const
{
    auto it = std::find_if(
        fn_map_.cbegin(),
        fn_map_.cend(),
        [name](const FnPair& p) { return p.first == name; });

    if (it == fn_map_.cend()) {
        LIB_ERR << "function not found: " << name->s_name;
        return {};
    } else
        return it->second(args);
}

bool BuiltinFunctionMap::registerFn(t_symbol* name, BuiltinFunction fn)
{
    auto it = std::find_if(
        fn_map_.cbegin(),
        fn_map_.cend(),
        [name](const FnPair& p) { return p.first == name; });

    if (it != fn_map_.cend()) {
        LIB_ERR << "function already exists: " << name->s_name;
        return false;
    }

    fn_map_.emplace_back(name, fn);
    LIB_LOG << fmt::format("builtin function registered: {}", name->s_name);
    return true;
}

bool BuiltinFunctionMap::exists(t_symbol* name) const
{
    auto it = std::find_if(
        fn_map_.cbegin(),
        fn_map_.cend(),
        [name](const FnPair& p) { return p.first == name; });

    return it != fn_map_.cend();
}

}
