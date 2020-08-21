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

FlowRoute::FlowRoute(const PdArgs& args)
    : BaseObject(args)
    , n_(args.args.size())
{
    for (auto& a : args.args) {
        routes_.push_back(to_string(a));
        createOutlet();
    }

    createOutlet();
}

void FlowRoute::parseProperties()
{
    // do not parse properties in args
}

void FlowRoute::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    auto& slots = args();
    auto& atom = lst[0];

    // iterate from end
    for (size_t i = n_; i > 0; i--) {
        const size_t IDX = i - 1;
        auto& key = slots[IDX];

        if (atom == key) {
            listTo(IDX, lst.slice(1));
            return;
        }
    }

    // no match
    listTo(n_, lst);
}

void FlowRoute::onAny(t_symbol* s, const AtomListView& lst)
{
    auto& slots = args();

    // iterate from end
    for (size_t i = n_; i > 0; i--) {
        const size_t IDX = i - 1;
        auto& key = slots[IDX];

        if (s == key) {
            listTo(IDX, lst);
            return;
        }
    }

    // no match
    anyTo(n_, s, lst);
}

void FlowRoute::onData(const Atom& data)
{
    if (data.isA<DataTypeDict>()) {
        auto dict = data.asD<DataTypeDict>();
        if (!dict) {
            OBJ_ERR << "invalid data pointer to dict";
            return;
        }

        auto& slots = args();
        bool matched = false;

        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t OUT = i - 1;
            auto key = slots[OUT].asSymbol();

            if (dict->contains(key)) {
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
        auto& slots = args();

        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t OUT = i - 1;
            auto& pattern = slots[OUT];
            if (first == pattern) {
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
    if (n < routes_.size())
        return routes_[n].c_str();
    else
        return "not-matched";
}

void setup_flow_route()
{
    ObjectFactory<FlowRoute> obj("flow.route");
    obj.processData();
    obj.addInletInfo("input messages");
    obj.noPropsDispatch();
}
