/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "list_running_sum.h"
#include "ceammc_factory.h"

#include <boost/container/small_vector.hpp>
using AtomList32 = boost::container::small_vector<Atom, 32>;

ListRunningSum::ListRunningSum(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListRunningSum::onList(const AtomList& lst)
{
    AtomList32 res;

    t_float val = 0;
    for (auto& a : lst) {
        val += a.asFloat();
        res.push_back(Atom(val));
    }

    listTo(0, AtomListView(res.data(), res.size()));
}

void setup_list_runsum()
{
    ObjectFactory<ListRunningSum> obj("list.runsum");
    obj.setXletsInfo({ "list: input" }, { "list: result" });
}
