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
    t_object* obj = owner();

    t_outlet* outlet;
    auto conn = obj_starttraverseoutlet(obj, &outlet, 1);
    if (!conn) {
        OBJ_ERR << "can't connect";
        return;
    }

    while (conn) {
        t_object* dest;
        t_inlet* inletp;
        int whichp;
        conn = obj_nexttraverseoutlet(conn, &dest, &inletp, &whichp);

        t_class* c = dest->te_g.g_pd;
        t_symbol* name = c->c_name;
        OBJ_DBG << "class name: " << name;

        auto it = base_external_set().find(c);
        if (it != base_external_set().end()) {
            OBJ_DBG << "BaseObject";
            typedef ObjectFactory<ObjectInfo>::ObjectProxy Proxy;
            Proxy* p = (Proxy*)dest;
            for (auto& prop : p->impl->properties())
                OBJ_DBG << prop.second->name();

            return;
        }

        it = ui_external_set().find(c);
        if (it != ui_external_set().end()) {
            OBJ_DBG << "UIObject";
            for (int i = 0; i < c->c_nmethod; i++) {
                t_methodentry m = c->c_methods[i];
                t_symbol* s = m.me_name;
                if (s->s_name[0] != '@')
                    continue;

                size_t len = strlen(s->s_name);
                if (len < 1)
                    continue;

                if (s->s_name[len - 1] == '?')
                    continue;

                OBJ_DBG << s;
            }
        }
    }
}

void setup_obj_info()
{
    ObjectFactory<ObjectInfo> obj("obj.info");
}
