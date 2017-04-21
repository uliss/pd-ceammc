/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_pd.h"

extern "C" {
#include "m_imp.h"
}

typedef t_object* (*t_newgimme)(t_symbol* s, int argc, t_atom* argv);

using namespace ceammc;

pd::External::External(const char* name)
    : obj_(0)
{
    t_symbol* OBJ_NAME = gensym(name);
    t_methodentry* m = pd_objectmaker->c_methods;
    for (int i = 0; i < pd_objectmaker->c_nmethod; i++) {
        if (m[i].me_name == OBJ_NAME)
            obj_ = (*((t_newgimme)(m[i].me_fun)))(OBJ_NAME, 0, 0);
    }
}

pd::External::~External()
{
    if (!obj_) {
        pd_free(&obj_->te_g.g_pd);
    }
}

bool pd::External::isNull() const
{
    return obj_ == 0;
}

bool pd::External::connectTo(int outn, t_object* dest, int inln)
{
    if (!obj_)
        return false;

    if (obj_ninlets(obj_) < 1)
        return false;

    obj_connect(obj_, outn, dest, inln);
    return true;
}

t_object* pd::External::object()
{
    return obj_;
}

void pd::External::bang()
{
    if (!obj_)
        return;

    pd_bang(&obj_->te_g.g_pd);
}
