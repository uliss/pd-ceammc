/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "flow_match.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_regexp.h"
#include "datatype_string.h"
#include "re2/re2.h"

FlowMatch::FlowMatch(const PdArgs& args)
    : BaseObject(args)
    , patterns_(nullptr)
    , cut_(nullptr)
{
    cut_ = new BoolProperty("@cut", false);
    addProperty(cut_);

    patterns_ = new ListProperty("@patterns");
    patterns_->setInitOnly();
    patterns_->setArgIndex(0);
    patterns_->setSuccessFn([this](Property* p) {
        for (auto x : p->get()) {
            const std::string re = regexp::escape(to_string(x));
            re_.emplace_back(new re2::RE2(re));
        }
    });

    addProperty(patterns_);
}

FlowMatch::~FlowMatch()
{
    for (auto* p : re_)
        delete p;
}

void FlowMatch::initDone()
{
    for (size_t i = 0; i < re_.size(); i++) {
        createInlet();
        createOutlet();
    }

    createOutlet();
}

void FlowMatch::onInlet(size_t idx, const AtomListView& lv)
{
    if (idx < re_.size()) {
        delete re_[idx];
        re_[idx] = new re2::RE2(regexp::escape(to_string(lv)));
    }
}

void FlowMatch::onSymbol(t_symbol* s)
{
    const size_t N = re_.size();
    assert(N + 1 == numOutlets());

    for (size_t i = 0; i < N; i++) {
        if (RE2::FullMatch(s->s_name, *re_[i])) {
            if (cut_->value())
                bangTo(i);
            else
                symbolTo(i, s);

            return;
        }
    }

    symbolTo(N, s);
}

void FlowMatch::onAny(t_symbol* s, const AtomListView& lv)
{
    const size_t N = re_.size();
    assert(N + 1 == numOutlets());

    for (size_t i = 0; i < N; i++) {
        if (RE2::FullMatch(s->s_name, *re_[i])) {
            if (cut_->value())
                anyTo(i, lv);
            else
                anyTo(i, s, lv);

            return;
        }
    }

    anyTo(N, s, lv);
}

void FlowMatch::onDataT(const StringAtom& s)
{
    const size_t N = re_.size();
    assert(N + 1 == numOutlets());

    for (size_t i = 0; i < N; i++) {
        if (RE2::FullMatch(s->str(), *re_[i])) {
            atomTo(i, s);
            return;
        }
    }

    atomTo(N, s);
}

const char* FlowMatch::annotateInlet(size_t n) const
{
    if (n == 0)
        return "symbol: input flow\n"
               "any:    messages";
    else if (n <= re_.size())
        return re_[n - 1]->pattern().c_str();
    else
        return nullptr;
}

const char* FlowMatch::annotateOutlet(size_t n) const
{
    if (n < re_.size())
        return re_[n]->pattern().c_str();
    else if (n == re_.size())
        return "unmatched";
    else
        return nullptr;
}

void setup_flow_match()
{
    ObjectFactory<FlowMatch> obj("flow.match");
    obj.processData<DataTypeString>();
    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);

    obj.setDescription("route data flow by regexp match");
    obj.setCategory("flow");
    obj.setKeywords({"regexp"});
}
