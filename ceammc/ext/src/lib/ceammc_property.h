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
#include "ceammc_controlvalue.h"
#include <m_pd.h>

namespace ceammc {

template <typename T>
struct PdAnyFunction {
    typedef void (*type)(T*, t_symbol*, int, t_atom*);
};

template <typename T>
struct PdListFunction {
    typedef void (*type)(T*, t_symbol*, int, t_atom*);
};

template <typename T>
struct PdBangFunction {
    typedef void (*type)(T*);
};

template <typename T>
struct PdFloatFunction {
    typedef void (*type)(T*, t_float);
};

template <typename T>
struct PdSymbolFunction {
    typedef void (*type)(T*, t_float);
};

template <typename V>
class Property {
    V prop_;
    t_symbol* name_;

public:
    Property(t_symbol* name)
        : name_(name)
    {
    }

    t_symbol* name() { return name_; }
    const t_symbol* name() const { return name_; }

    V& get() { return prop_; }
    const V& get() const { return prop_; }
    void set(const V& p) { prop_ = p; }

    void setFromPd(t_symbol* sel, int argc, t_atom* argv)
    {
        if (name_ != sel)
            return;

        if (argc < 1)
            return;

        propertyValueSet(AtomList(argc, argv));
    }

    void output(t_outlet* x)
    {
        ControlValue cv;
        controlValueSet(cv, &prop_);
        cv.output(x);
    }

    void controlValueSet(ControlValue&, V*) {}
    void propertyValueSet(const AtomList&) {}
};

template <>
void Property<t_float>::controlValueSet(ControlValue& cv, t_float* f) { cv.setFloat(*f); }

template <>
void Property<int>::controlValueSet(ControlValue& cv, int* i) { cv.setFloat(*i); }

template <>
void Property<size_t>::controlValueSet(ControlValue& cv, size_t* sz) { cv.setFloat(*sz); }

template <>
void Property<t_symbol*>::controlValueSet(ControlValue& cv, t_symbol** s) { cv.setSymbol(*s); }

template <>
void Property<Atom>::controlValueSet(ControlValue& cv, Atom* a) { cv.setAtom(*a); }

template <>
void Property<AtomList>::controlValueSet(ControlValue& cv, AtomList* l) { cv.setList(*l); }

template <>
void Property<t_float>::propertyValueSet(const AtomList& lst)
{
    lst.first()->getFloat(&prop_);
}

template <>
void Property<int>::propertyValueSet(const AtomList& lst)
{
    t_float f;
    if (lst.first()->getFloat(&f))
        prop_ = static_cast<int>(f);
}

template <>
void Property<size_t>::propertyValueSet(const AtomList& lst)
{
    t_float f;
    if (lst.first()->getFloat(&f))
        prop_ = static_cast<size_t>(f);
}

template <>
void Property<t_symbol*>::propertyValueSet(const AtomList& lst)
{
    lst.first()->getSymbol(&prop_);
}

typedef Property<t_float> FloatProperty;
typedef Property<int> IntProperty;
typedef Property<size_t> SizeProperty;
typedef Property<t_symbol*> SymbolProperty;

template <typename T, typename V>
struct PropertyFieldPtr {
    Property<V>* T::*prop_member_ptr;
    PdAnyFunction<T> pd_call_func_;

    PropertyFieldPtr(Property<V>* T::*p, PdAnyFunction<T> fn)
        : prop_member_ptr(p)
        , pd_call_func_(fn)
    {
    }

    void process_rw(T* x, t_symbol* sel, int argc, t_atom* argv)
    {
        if (argc < 1)
            (x->*prop_member_ptr)->output(x->x_obj.te_outlet);
        else
            (x->*prop_member_ptr)->setFromPd(sel, argc, argv);
    }
};

//template <typename T, typename V>
//void class_addproperty_rw(t_class* c, Property<V>* T::*prop, t_symbol* s)
//{
//    static std::function<void(PropertyFieldPtr<T, V>&, T * x, t_symbol * sel, int argc, t_atom* argv)> func = &PropertyFieldPtr<T, V>::process_rw;

//    //    auto f = std::bind(&p::process_rw);
//    class_addmethod(c,
//        reinterpret_cast<t_method>(func), s, A_GIMME, A_NULL);
//}

}

#endif // CEAMMC_PROPERTY_H
