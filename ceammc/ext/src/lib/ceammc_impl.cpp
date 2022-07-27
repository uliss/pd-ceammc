/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_impl.h"
#include "m_pd.h"

#include <cstdlib>

extern "C" {
#include "m_imp.h"
}

/// 0.51 t_methodentry
typedef struct _methodentry51 {
    t_symbol* me_name;
    t_gotfn me_fun;
    t_atomtype me_arg[MAXPDARG + 1];
} t_methodentry51;

/// 0.52 t_methodentry
typedef struct _methodentry52 {
    t_symbol* me_name;
    t_gotfn me_fun;
    unsigned char me_arg[MAXPDARG + 1];
} t_methodentry52;

namespace {

template <typename T>
T* class_get_method(t_class* c, int n)
{
#ifdef PDINSTANCE
    return reinterpret_cast<T*>(c->c_methods[pd_this->pd_instanceno]) + n;
#else
    return reinterpret_cast<T*>(c->c_methods) + n;
#endif
}

template <typename T>
t_symbol* class_get_method_name(t_class* c, int n)
{
    return class_get_method<T>(c, n)->me_name;
}
}

namespace ceammc {

PdFloatFunction default_pd_float_fn(t_class* c)
{
    return reinterpret_cast<PdFloatFunction>(c->c_floatmethod);
}

PdBangFunction default_pd_bang_fn(t_class* c)
{
    return reinterpret_cast<PdBangFunction>(c->c_bangmethod);
}

t_symbol* class_method_name(t_class* c, int method_idx)
{
    if (!c)
        return nullptr;

    if (method_idx < 0 || method_idx >= c->c_nmethod)
        return nullptr;

    if (is_ceammc_distribution())
        return class_get_method_name<t_methodentry>(c, method_idx);
    else {
        int maj, min, bug;
        sys_getversion(&maj, &min, nullptr);
        int int_vers = maj * 100 + min;
        if (int_vers < 52)
            return class_get_method_name<t_methodentry51>(c, method_idx);
        else
            return class_get_method_name<t_methodentry52>(c, method_idx);
    }
}

bool is_ceammc_distribution()
{
    static const bool is_ceammc = (getenv("is_ceammc") != nullptr);
    return is_ceammc;
}

bool class_method_set_fn(t_class* c, int method_idx, void* fn)
{
    if (!c)
        return false;

    if (method_idx < 0 || method_idx >= c->c_nmethod)
        return false;

    if (is_ceammc_distribution()) {
        auto m = class_get_method<t_methodentry>(c, method_idx);
        m->me_fun = reinterpret_cast<t_gotfn>(fn);
        return true;
    } else {
        int maj, min, bug;
        sys_getversion(&maj, &min, nullptr);
        int int_vers = maj * 100 + min;
        if (int_vers < 52) {
            auto m = class_get_method<t_methodentry51>(c, method_idx);
            m->me_fun = reinterpret_cast<t_gotfn>(fn);
        } else {
            auto m = class_get_method<t_methodentry52>(c, method_idx);
            m->me_fun = reinterpret_cast<t_gotfn>(fn);
        }

        return true;
    }
}

} // namespace ceammc
