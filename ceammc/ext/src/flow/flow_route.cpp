/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "flow_route.h"
#include "ceammc_datatypes.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"

constexpr char KEEP_SELECTOR_CHAR = '*';

FlowRoute::FlowRoute(const PdArgs& args)
    : BaseObject(args)
    , n_(args.args.size())
{
    routes_s_.reserve(args.args.size());
    routes_.reserve(args.args.size());

    for (auto& a : args.args) {
        if (a.isSymbol()) {
            const char* str = a.asT<t_symbol*>()->s_name;
            if (str[0] == KEEP_SELECTOR_CHAR && str[1] != '\0') { // check for *XXX
                auto lst = AtomList::parseString(str + 1);
                if (lst.size() == 1) {
                    routes_s_.emplace_back(str + 1);
                    routes_.push_back({ lst[0], true });
                } else {
                    OBJ_ERR << "parse error: " << str;
                    routes_s_.emplace_back(str);
                    routes_.push_back({ a, true });
                }
            } else {
                routes_s_.emplace_back(str);
                routes_.push_back({ a, false });
            }
        } else {
            routes_s_.emplace_back(to_string(a));
            routes_.push_back({ a, false });
        }

        createOutlet();
    }

    createOutlet();
}

void FlowRoute::onList(const AtomListView& lv)
{
    if (lv.empty())
        return;

    auto& atom = lv[0];

    // iterate from end
    for (size_t i = n_; i > 0; i--) {
        const size_t IDX = i - 1;
        auto& r = routes_[IDX];

        if (atom == r.sel) {
            if (r.keep)
                listTo(IDX, lv);
            else
                listTo(IDX, lv.subView(1));

            return;
        }
    }

    // no match
    listTo(n_, lv);
}

void FlowRoute::onAny(t_symbol* s, const AtomListView& lv)
{
    // iterate from end
    for (size_t i = n_; i > 0; i--) {
        const size_t IDX = i - 1;
        auto& r = routes_[IDX];

        if (s == r.sel) {
            if (r.keep)
                anyTo(IDX, s, lv);
            else {
                if (!lv.empty() && lv[0].isSymbol())
                    anyTo(IDX, lv[0].asT<t_symbol*>(), lv.subView(1));
                else
                    listTo(IDX, lv);
            }

            return;
        }
    }

    // no match
    anyTo(n_, s, lv);
}

void FlowRoute::onData(const Atom& data)
{
    if (data.isA<DataTypeDict>()) {
        auto dict = data.asD<DataTypeDict>();
        if (!dict) {
            OBJ_ERR << "invalid data pointer to dict";
            return;
        }
        bool matched = false;

        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t OUT = i - 1;
            auto& r = routes_[OUT];
            auto key = r.sel.asSymbol();

            if (dict->contains(key)) {
                if (r.keep)
                    anyTo(OUT, key, dict->at(key));
                else
                    listTo(OUT, dict->at(key));

                matched = true;
            }
        }

        // not matched: output to last outlet
        if (!matched)
            atomTo(n_, data);

    } else if (data.isA<DataTypeMList>()) {
        auto mlist = data.asD<DataTypeMList>();

        if (!mlist) {
            OBJ_ERR << "invalid data pointer to mlist";
            return;
        }

        // empty list check
        if (mlist->size() == 0)
            return;

        auto& first = mlist->at(0);

        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t OUT = i - 1;
            auto& r = routes_[OUT];
            if (first == r.sel) {
                if (r.keep)
                    atomTo(OUT, data);
                else
                    atomTo(OUT, MListAtom(mlist->slice(1)));

                return;
            }
        }

        // no match
        atomTo(n_, data);
    } else {
        OBJ_ERR << "unsupported data type: " << data;
        atomTo(n_, data);
    }
}

const char* FlowRoute::annotateOutlet(size_t n) const
{
    if (n < routes_s_.size())
        return routes_s_[n].c_str();
    else
        return "not-matched";
}

void setup_flow_route()
{
    ObjectFactory<FlowRoute> obj("flow.route");
    obj.processData();
    obj.addInletInfo("input messages");
    obj.noPropsDispatch();
    obj.noArgsAndPropsParse();
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdSymbolFn();
}
