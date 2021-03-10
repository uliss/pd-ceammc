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
#include "obj_info.h"
#include "ceammc_externals.h"
#include "ceammc_factory.h"

extern "C" {
#include "m_imp.h"
}

ObjectInfo::ObjectInfo(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void ObjectInfo::onBang()
{
    m_props(gensym("props"), AtomList());
}

void ObjectInfo::m_props(t_symbol* s, const AtomListView&)
{
    AtomList res;
    for (t_object* o : connected()) {
        if (!is_ceammc(o))
            continue;

        std::vector<PropertyInfo> props;

        if (is_ceammc_base(o))
            props = ceammc_base_properties(o);
        if (is_ceammc_ui(o))
            props = ceammc_ui_properties(o);

        for (const PropertyInfo& p : props)
            res.append(p.name());
    }

    listTo(0, res);
}

std::vector<t_object*> ObjectInfo::connected()
{
    t_object* obj = owner();
    t_outlet* outlet = nullptr;
    auto conn = obj_starttraverseoutlet(obj, &outlet, 1);
    if (!conn)
        return {};

    std::vector<t_object*> res;
    while (conn) {
        t_object* dest;
        t_inlet* inletp;
        int whichp;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);
        res.push_back(dest);
    }

    return res;
}

void setup_obj_info()
{
    ObjectFactory<ObjectInfo> obj("obj.info");
    obj.addMethod("properties", &ObjectInfo::m_props);
}
