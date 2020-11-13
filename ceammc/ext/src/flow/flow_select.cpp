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
#include "lex/select.lexer.h"
#include "lex/select.parser.hpp"

#include <algorithm>
#include <cmath>

class SelectMatch {
public:
    using Data = SelectLexer::MatchData;
    std::vector<Data> data;
};

FlowSelect::FlowSelect(const PdArgs& args)
    : BaseObject(args)
    , patterns_(new SelectMatch())
    , keep_value_(nullptr)
{
    try {
        keep_value_ = new BoolProperty("@keep_value", false);
        addProperty(keep_value_);

        addProperty(new AliasProperty<BoolProperty>("@v", keep_value_, true));

        SelectLexer l(to_string(unparsedPosArgs()));
        SelectParser p(l);

        auto err = p.parse();
        if (err) {
            OBJ_ERR << "parse error: ";
        } else {
            reserveOutlets(l.numMatches());
            outlet_toolips_.reserve(l.numMatches());

            const auto& margs = unparsedPosArgs();
            const Atom empty(&s_);

            for (size_t i = 0; i < l.numMatches(); i++) {
                auto& m = l[i];
                patterns_->data.push_back(m);

                const auto& a = (i < margs.size()) ? margs.at(i) : empty;
                outlet_toolips_.push_back(to_string(a));

                createOutlet();
            }

            createOutlet();
        }

    } catch (std::exception& e) {
        OBJ_ERR << "parser error: " << e.what();
    }
}

// for unique_ptr
FlowSelect::~FlowSelect() { }

void FlowSelect::onFloat(t_float v)
{
    if (patterns_->data.empty())
        return floatTo(0, v);

    auto it = patterns_->data.crbegin();
    auto end = patterns_->data.crend();

    using l = SelectLexer;
    const size_t LAST = numOutlets() - 1;
    bool matched = false;

    auto output = [this, &matched](t_float v, size_t idx) {
        matched = true;
        if (keep_value_->value())
            floatTo(idx, v);
        else
            bangTo(idx);
    };

    for (size_t idx = LAST - 1; it != end; ++it, --idx) {
        switch (it->type()) {
        case l::MATCH_EQUAL:
            if (it->at(0) == Atom(v))
                output(v, idx);

            break;
        case l::MATCH_GREATER:
            if (v > it->at(0).asT<t_float>())
                output(v, idx);

            break;
        case l::MATCH_GREATER_EQ:
            if (v >= it->at(0).asT<t_float>())
                output(v, idx);

            break;
        case l::MATCH_LESS:
            if (v < it->at(0).asT<t_float>())
                output(v, idx);

            break;
        case l::MATCH_LESS_EQ:
            if (v <= it->at(0).asT<t_float>())
                output(v, idx);

            break;
        case l::MATCH_RANGE_CC: {
            const auto a = it->at(0).asT<t_float>();
            const auto b = it->at(1).asT<t_float>();

            if (a <= v && v <= b)
                output(v, idx);

        } break;
        case l::MATCH_RANGE_CO: {
            const auto a = it->at(0).asT<t_float>();
            const auto b = it->at(1).asT<t_float>();

            if (a <= v && v < b)
                output(v, idx);

        } break;
        case l::MATCH_RANGE_OO: {
            const auto a = it->at(0).asT<t_float>();
            const auto b = it->at(1).asT<t_float>();

            if (a < v && v < b)
                output(v, idx);

        } break;
        case l::MATCH_RANGE_OC: {
            const auto a = it->at(0).asT<t_float>();
            const auto b = it->at(1).asT<t_float>();

            if (a < v && v <= b)
                output(v, idx);

        } break;
        case l::MATCH_EPSILON: {
            const auto x = it->at(0).asT<t_float>();
            const auto e = it->at(1).asT<t_float>();

            if (std::fabs(x - v) < e)
                output(v, idx);

        } break;
        case l::MATCH_SET: {
            auto mit = std::find_if(it->begin(), it->end(), [v](const Atom& a) { return a == Atom(v); });
            if (mit != it->end())
                output(v, idx);

        } break;
        default:
            break;
        }
    }

    if (!matched)
        floatTo(LAST, v);
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
            kv ? symbolTo(idx, s) : bangTo(idx);
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
            kv ? anyTo(idx, s, lv) : bangTo(idx);
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

void setup_flow_select()
{
    ObjectFactory<FlowSelect> obj("flow.select");
    obj.addAlias("flow.sel");
    obj.noArgsDataParsing();
    obj.useDefaultPdListFn();
    obj.noPropsDispatch();

    obj.addInletInfo("input flow");
}
