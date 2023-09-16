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
#include "ceammc_node_external.h"
#include "ceammc_externals.h"

extern "C" {
#include "m_imp.h"
}

using namespace ceammc;

NodeExternal::NodeExternal(const PdArgs& args)
    : BaseObject(args)
{
}

ObjectList NodeExternal::connectedObjects() const
{
    t_object* obj = owner();
    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(obj, &outlet, 1);
    if (!conn)
        return {};

    ObjectList res;
    while (conn) {
        t_object* dest;
        t_inlet* inletp;
        int whichp;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);
        res.push_back(dest);
    }

    return res;
}

void NodeExternal::forEach(PdObjectCb cb)
{
    auto obj = owner();
    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(obj, &outlet, 1);
    if (!conn)
        return;

    while (conn) {
        t_object* dest = nullptr;
        t_inlet* inletp = nullptr;
        int whichp = 0;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);

        if (!cb(dest))
            return;
    }
}

void NodeExternal::forEach(BaseObjectCb cb)
{
    auto obj = owner();
    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(obj, &outlet, 1);
    if (!conn)
        return;

    while (conn) {
        t_object* dest = nullptr;
        t_inlet* inletp = nullptr;
        int whichp = 0;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);

        auto base_obj = ceammc_to_base_object(dest, true);
        if (base_obj && !cb(base_obj))
            return;
    }
}
