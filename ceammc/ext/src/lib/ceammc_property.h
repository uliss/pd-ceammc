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
#ifndef CEAMMC_PROPERTY_H
#define CEAMMC_PROPERTY_H

#include "ceammc_atomlist.h"
#include <m_pd.h>

namespace ceammc {

template <typename V>
void property_atom_get(V*, const Atom&) {}

template <>
void property_atom_get(t_float* v, const Atom& a) { a.getFloat(v); }

template <>
void property_atom_get(int* v, const Atom& a)
{
    if (a.isFloat())
        *v = static_cast<int>(a.asFloat());
}

template <>
void property_atom_get(size_t* v, const Atom& a)
{
    if (a.isFloat())
        *v = static_cast<size_t>(a.asFloat());
}

template <>
void property_atom_get(t_symbol* s, const Atom& a) { a.getSymbol(&s); }

template <typename V>
void property_atom_set(Atom&, V*) {}

template <>
void property_atom_set(Atom& a, t_float* v) { a.setFloat(*v, true); }

template <>
void property_atom_set(Atom& a, int* v) { a.setFloat(*v, true); }

template <>
void property_atom_set(Atom& a, size_t* v) { a.setFloat(*v, true); }

template <>
void property_atom_set(Atom& a, t_symbol* s) { a.setSymbol(s, true); }

template <>
void property_atom_set(Atom& a, Atom* v) { a = *v; }

template <typename T, typename V>
static void property_get(T* x, t_symbol* sel, V T::*prop)
{
    Atom a;
    property_atom_set(a, &(x->*prop));
    a.outputAsAny(x->x_obj.te_outlet, sel);
}

template <typename T, typename V, V T::*prop>
static void property_set(T* x, const Atom& a)
{
    property_atom_get(&(x->*prop), a);
}

template <typename T, typename V, V T::*prop>
static void property_set_list(T* x, const AtomList& l)
{
    (x->*prop) = l;
}

template <typename T, typename V, V T::*prop>
static void process_property_rw(T* x, t_symbol* sel, int argc, t_atom* argv)
{
    if (argc < 1)
        property_get(x, sel, prop);
    else if (argc == 1)
        property_set<T, V, prop>(x, Atom(argv[0]));
}

template <typename T, typename V, V T::*prop>
static void process_property_ro(T* x, t_symbol* sel)
{
    property_get(x, sel, prop);
}

template <typename T, typename V, V T::*prop>
struct PropertyCallbackRW {
    typedef void (*type)(T*, t_symbol* sel, int argc, t_atom* argv);
};

template <typename T, typename V, V T::*prop>
void class_addproperty_rw(t_class* c, t_symbol* s)
{
    class_addmethod(c, reinterpret_cast<t_method>(&process_property_rw<T, V, prop>), s, A_GIMME, A_NULL);
}

template <typename T, typename V, V T::*prop>
void class_addproperty_ro(t_class* c, t_symbol* s)
{
    class_addmethod(c, reinterpret_cast<t_method>(&process_property_ro<T, V, prop>), s, A_GIMME, A_NULL);
}

}

#endif // CEAMMC_PROPERTY_H
