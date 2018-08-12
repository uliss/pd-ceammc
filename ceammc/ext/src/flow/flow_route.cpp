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
#include "../data/data_dict.h"
#include "../data/data_mlist.h"
#include "../data/data_set.h"
#include "ceammc_datatypes.h"
#include "ceammc_factory.h"

FlowRoute::FlowRoute(const PdArgs& args)
    : BaseObject(args)
    , n_(args.args.size())
{
    for (size_t i = 0; i < n_ + 1; i++)
        createOutlet();
}

void FlowRoute::parseProperties()
{
    // do not parse properties in args
}

bool FlowRoute::processAnyProps(t_symbol* sel, const AtomList& lst)
{
    return false;
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

void FlowRoute::onAny(t_symbol* s, const AtomList& lst)
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

void FlowRoute::onData(const DataPtr& ptr)
{
    auto type = ptr->type();
    switch (type) {
    case data::DATA_DICT: {
        auto dict = ptr->as<DataTypeDict>();
        if (!dict) {
            OBJ_ERR << "invalid data pointer to dict";
            return;
        }

        auto& slots = args();
        bool matched = false;

        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t IDX = i - 1;
            auto& key = slots[IDX];
            auto value = dict->value(key);
            if (value == boost::none)
                continue;

            if (DataTypeDict::isAtom(value))
                atomTo(IDX, boost::get<Atom>(*value));
            else if (DataTypeDict::isAtomList(value))
                listTo(IDX, boost::get<AtomList>(*value));
            else if (DataTypeDict::isDataAtom(value))
                dataTo(IDX, boost::get<DataAtom>(*value).data());
            else {
                OBJ_ERR << "unsupported value type";
            }

            matched = true;
        }

        // not matched
        if (!matched)
            dataTo(n_, ptr);
    } break;
    case data::DATA_SET: {
        auto set = ptr->as<DataTypeSet>();

        if (!set) {
            OBJ_ERR << "invalid data pointer to set";
            return;
        }

        auto& slots = args();

        bool matched = false;
        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t IDX = i - 1;
            auto& value = slots[IDX];
            if (set->contains(value)) {
                matched = true;
                bangTo(IDX);
            }
        }

        // no match
        if (!matched)
            dataTo(n_, ptr);

    } break;
    case data::DATA_MLIST: {
        auto mlist = ptr->as<DataTypeMList>();

        if (!mlist) {
            OBJ_ERR << "invalid data pointer to mlist";
            return;
        }

        // mpety list check
        if (mlist->size() == 0)
            return;

        // check first element is atom
        auto& first = mlist->at(0);
        if (!first.isAtom())
            return;

        auto atom = first.toAtom();
        auto& slots = args();

        // iterate from end
        for (size_t i = n_; i > 0; i--) {
            const size_t IDX = i - 1;
            auto& value = slots[IDX];
            if (atom == value) {
                DataTypeMList* res = new DataTypeMList;
                DataPtr ptr(res);

                for (size_t i = 1; i < mlist->size(); i++)
                    res->append(mlist->at(i));

                dataTo(IDX, ptr);
                return;
            }
        }

        // no match
        dataTo(n_, ptr);
    } break;
    default:
        OBJ_ERR << "unsupported data type: " << type;
        dataTo(n_, ptr);
        return;
    }
}

void setup_flow_route()
{
    ObjectFactory<FlowRoute> obj("flow.route");
    obj.processData();
}
