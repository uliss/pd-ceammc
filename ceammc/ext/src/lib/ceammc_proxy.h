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
#ifndef CEAMMC_PROXY_H
#define CEAMMC_PROXY_H

#include "m_pd.h"

namespace ceammc {

template <class Owner>
struct PdProxy {
    t_object obj;
    Owner* owner;

    static t_class* proxy_class;

    static PdProxy* proxy_new()
    {
        PdProxy* x = reinterpret_cast<PdProxy*>(pd_new(PdProxy<Owner>::proxy_class));
        x->owner = 0;
        return x;
    }

    void free()
    {
        pd_free(&obj.te_g.g_pd);
    }

    t_object* object()
    {
        return &obj;
    }
};

template <class Owner>
t_class* PdProxy<Owner>::proxy_class = 0;

template <class Owner>
struct PdFloatProxy : public PdProxy<Owner> {
    static PdFloatProxy* proxy_new()
    {
        return static_cast<PdFloatProxy<Owner>*>(PdProxy<Owner>::proxy_new());
    }

    typedef void (*FloatMethod)(PdFloatProxy* x, t_float v);
    static void class_init(const char* name, FloatMethod func)
    {
        PdProxy<Owner>::proxy_class = class_new(gensym(name),
            reinterpret_cast<t_newmethod>(PdFloatProxy<Owner>::proxy_new),
            0, sizeof(PdFloatProxy<Owner>), CLASS_PD, A_NULL);

        class_addfloat(PdProxy<Owner>::proxy_class, func);
    }
};
}

#endif // CEAMMC_PROXY_H
