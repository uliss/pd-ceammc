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
#include "list_running_diff.h"
#include "ceammc_factory.h"

#include <boost/container/small_vector.hpp>
using AtomList32 = boost::container::small_vector<Atom, 32>;

ListRunningDiff::ListRunningDiff(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListRunningDiff::onList(const AtomList& lst)
{
    AtomList32 res;
    res.reserve(lst.size());

    auto it = lst.begin();
    auto last = lst.end();

    if (it != last) {
        t_float val, prev;
        prev = it->asFloat();
        res.push_back(Atom(prev));

        while (++it != last) {
            val = it->asFloat();
            res.push_back(val - prev);
            prev = val;
        }
    }

    listTo(0, AtomListView(res.data(), res.size()));
}

void setup_list_rundiff()
{
    ObjectFactory<ListRunningDiff> obj("list.rundiff");
    obj.setXletsInfo({ "list: input" }, { "list: result" });
}
