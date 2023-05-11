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
#include "flow_select.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_output.h"
#include "lex/select.lexer.h"
#include "lex/select.parser.hpp"

#include <algorithm>
#include <cmath>

using namespace ceammc::select;

class SelectMatch {
public:
    using Data = SelectLexer::MatchData;
    std::vector<Data> data;
};

FlowSelect::FlowSelect(const PdArgs& args)
    : BaseObject(args)
    , patterns_(new SelectMatch())
{
    try {
        keep_value_ = new BoolProperty("@keep_value", false);
        addProperty(keep_value_);

        addProperty(new AliasProperty<BoolProperty>("@v", keep_value_, true));

        msg_ = new ListProperty("@msg");
        addProperty(msg_);

        const std::string str = to_string(parsedPosArgs());
        SelectLexer l(str);
        SelectParser p(l);

        auto err = p.parse();
        if (err) {
            OBJ_ERR << "parse error: ";
        } else {
            const size_t NM = l.numMatches();
            OBJ_LOG << "num args: " << parsedPosArgs().size();
            OBJ_LOG << "num matches: " << NM;

            reserveOutlets(NM);
            outlet_toolips_.reserve(NM);

            const auto& margs = parsedPosArgs();
            const Atom empty(&s_);

            for (size_t i = 0; i < NM; i++) {
                auto& m = l[i];
                patterns_->data.push_back(m);

                const auto& a = (i < margs.size()) ? margs.at(i) : empty;
                outlet_toolips_.push_back(to_string(a));

                createOutlet();
            }

            createOutlet();
        }

        const auto prop_idx = args.args.findPos(isProperty);
        if (prop_idx >= 0) {
            for (auto& plist : args.args.properties()) {
                if (plist.size() > 0 && plist[0].isSymbol())
                    setProperty(plist[0].asT<t_symbol*>(), plist.view(1));
            }
        }

    } catch (std::exception& e) {
        OBJ_ERR << "parser error: " << e.what();
    }
}

// for unique_ptr
FlowSelect::~FlowSelect() = default;

void FlowSelect::onFloat(t_float v)
{
    const size_t N = patterns_->data.size();

    bool res[N];
    std::fill(res, res + N, false);

    if (!match(v, &res[0]))
        return floatTo(N, v);

    const bool kv = keep_value_->value();

    for (size_t i = N; i > 0; i--) {
        const auto idx = i - 1;

        if (res[idx])
            kv ? floatTo(idx, v) : outputTo(idx);
    }
}

void FlowSelect::onSymbol(t_symbol* s)
{
    const size_t N = patterns_->data.size();

    bool res[N];
    std::fill(res, res + N, false);

    if (!match(s, &res[0]))
        return symbolTo(N, s);

    const bool kv = keep_value_->value();

    for (size_t i = N; i > 0; i--) {
        const auto idx = i - 1;

        if (res[idx])
            kv ? symbolTo(idx, s) : outputTo(idx);
    }
}

void FlowSelect::onList(const AtomListView& lv)
{
    const size_t N = patterns_->data.size();
    bool res[N];
    std::fill(res, res + N, false);

    if (lv.empty())
        return;
    else if (lv.isFloat())
        return onFloat(lv[0].asT<t_float>());
    else if (lv.isSymbol())
        return onSymbol(lv[0].asT<t_symbol*>());
    else {
        if (!match(lv[0], &res[0])) {
            OBJ_ERR << "no match: " << N << " L" << lv;
            return listTo(N, lv);
        }

        const bool kv = keep_value_->value();

        for (size_t i = N; i > 0; i--) {
            const auto idx = i - 1;

            if (res[idx])
                kv ? listTo(idx, lv) : outputTo(idx);
        }
    }
}

void FlowSelect::onAny(t_symbol* s, const AtomListView& lv)
{
    const size_t N = patterns_->data.size();

    bool res[N];
    std::fill(res, res + N, false);

    if (!match(s, &res[0]))
        return anyTo(N, s, lv);

    const bool kv = keep_value_->value();

    for (size_t i = N; i > 0; i--) {
        const auto idx = i - 1;

        if (res[idx])
            kv ? anyTo(idx, s, lv) : outputTo(idx);
    }
}

const char* FlowSelect::annotateOutlet(size_t idx) const
{
    if (idx >= numOutlets() || idx >= outlet_toolips_.size())
        return "unmatched";

    return outlet_toolips_[idx].c_str();
}

size_t FlowSelect::match(t_symbol* s, bool* result) const
{
    using l = SelectLexer;

    if (patterns_->data.empty())
        return 0;

    auto it = patterns_->data.cbegin();
    auto end = patterns_->data.cend();
    size_t nmatches = 0;

    for (size_t pos = 0; it != end; ++it, ++pos) {
        switch (it->type()) {
        case l::MATCH_EQUAL:
            if (it->at(0) == Atom(s)) {
                result[pos] = true;
                nmatches++;
            }

            break;
        case l::MATCH_SET: {
            auto mit = std::find_if(it->begin(), it->end(), [s](const Atom& a) { return a == Atom(s); });
            if (mit != it->end()) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        default:
            break;
        }
    }

    return nmatches;
}

size_t FlowSelect::match(const Atom& a, bool* result) const
{
    if (patterns_->data.empty())
        return 0;

    auto it = patterns_->data.cbegin();
    auto end = patterns_->data.cend();

    using l = SelectLexer;
    size_t nmatches = 0;

    for (size_t pos = 0; it != end; ++it, ++pos) {
        switch (it->type()) {
        case l::MATCH_EQUAL:
            if (it->at(0) == Atom(a)) {
                result[pos] = true;
                nmatches++;
            }

            break;
        case l::MATCH_GREATER:
            if (a.isFloat() && a.asT<t_float>() > it->at(0).asT<t_float>()) {
                result[pos] = true;
                nmatches++;
            }

            break;
        case l::MATCH_GREATER_EQ:
            if (a.isFloat() && a.asT<t_float>() >= it->at(0).asT<t_float>()) {
                result[pos] = true;
                nmatches++;
            }

            break;
        case l::MATCH_LESS:
            if (a.isFloat() && a.asT<t_float>() < it->at(0).asT<t_float>()) {
                result[pos] = true;
                nmatches++;
            }

            break;
        case l::MATCH_LESS_EQ:
            if (a.isFloat() && a.asT<t_float>() <= it->at(0).asT<t_float>()) {
                result[pos] = true;
                nmatches++;
            }

            break;
        case l::MATCH_RANGE_CC: {
            if (!a.isFloat())
                break;

            const auto v = a.asT<t_float>();
            const auto m0 = it->at(0).asT<t_float>();
            const auto m1 = it->at(1).asT<t_float>();

            if (m0 <= v && v <= m1) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        case l::MATCH_RANGE_CO: {
            if (!a.isFloat())
                break;

            const auto v = a.asT<t_float>();
            const auto m0 = it->at(0).asT<t_float>();
            const auto m1 = it->at(1).asT<t_float>();

            if (m0 <= v && v < m1) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        case l::MATCH_RANGE_OO: {
            if (!a.isFloat())
                break;

            const auto v = a.asT<t_float>();
            const auto m0 = it->at(0).asT<t_float>();
            const auto m1 = it->at(1).asT<t_float>();

            if (m0 < v && v < m1) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        case l::MATCH_RANGE_OC: {
            if (!a.isFloat())
                break;

            const auto v = a.asT<t_float>();
            const auto m0 = it->at(0).asT<t_float>();
            const auto m1 = it->at(1).asT<t_float>();

            if (m0 < v && v <= m1) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        case l::MATCH_EPSILON: {
            if (!a.isFloat())
                break;

            const auto v = a.asT<t_float>();
            const auto x = it->at(0).asT<t_float>();
            const auto e = it->at(1).asT<t_float>();

            if (std::fabs(x - v) < e) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        case l::MATCH_SET: {
            auto mit = std::find_if(it->begin(), it->end(), [a](const Atom& a0) { return a0 == Atom(a); });
            if (mit != it->end()) {
                result[pos] = true;
                nmatches++;
            }

        } break;
        default:
            break;
        }
    }

    return nmatches;
}

void FlowSelect::outputTo(size_t idx)
{
    if (msg_->value().empty()) {
        bangTo(idx);
    } else {
        if (msg_->value()[0].isSymbol())
            anyTo(idx, msg_->value());
        else
            outletAtomList(outletAt(idx), msg_->value(), true);
    }
}

void setup_flow_select()
{
    ObjectFactory<FlowSelect> obj("flow.select");
    obj.addAlias("flow.sel");

    obj.parseArgs(true);
    obj.parseProps(true);
    obj.parsePosProps(false);
    obj.parseArgsMode(PdArgs::PARSE_COPY);
    obj.parsePropsMode(PdArgs::PARSE_COPY);

    obj.noPropsDispatch();

    obj.addInletInfo("input flow");

    obj.setDescription("vanilla flow on steroids");
    obj.setCategory("flow");
    obj.setKeywords({ "select" });
}
