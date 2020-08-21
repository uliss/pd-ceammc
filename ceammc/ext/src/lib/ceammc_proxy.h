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

#include "ceammc_atomlist.h"

#include <algorithm>
#include <cassert>
#include <string.h>
#include <vector>

namespace ceammc {

class PdBindObject {
protected:
    t_object obj_;
    t_symbol* bind_name_;

public:
    PdBindObject()
        : bind_name_(&s_)
    {
    }

    ~PdBindObject()
    {
        unbind();
    }

    void bind(t_symbol* name)
    {
        assert(name);

        if (bind_name_ == name)
            return;

        unbind();
        pd_bind(pd(), name);
        bind_name_ = name;
    }

    void unbind()
    {
        if (bind_name_ != &s_) {
            pd_unbind(pd(), bind_name_);
            bind_name_ = &s_;
        }
    }

    bool isBinded() const { return bind_name_ != &s_; }

    t_object* object() { return &obj_; }
    t_pd* pd() { return &obj_.te_g.g_pd; }
};

template <class T>
class PdFloatProxy : public PdBindObject {
public:
    typedef void (T::*ProxyFnPtr)(t_float);

    PdFloatProxy(T* o, ProxyFnPtr p)
        : owner_(o)
        , method_ptr_(p)
    {
        if (proxy_class_ == nullptr)
            initClass();

        assert(proxy_class_);

        t_object* tmp = reinterpret_cast<t_object*>(pd_new(proxy_class_));
        memcpy(&obj_, tmp, sizeof(t_object));
        pd_free((t_pd*)tmp);
    }

    void onFloat(t_float f)
    {
        (owner_->*method_ptr_)(f);
    }

public:
    static void on_float(PdFloatProxy* p, t_float f)
    {
        p->onFloat(f);
    }

    static void initClass()
    {
        PdFloatProxy::proxy_class_ = class_new(gensym("proxy float"), 0, 0, sizeof(PdFloatProxy), CLASS_PD, A_NULL);
        class_doaddfloat(PdFloatProxy::proxy_class_, reinterpret_cast<t_method>(PdFloatProxy::on_float));
    }

private:
    T* owner_;
    ProxyFnPtr method_ptr_;

protected:
    static t_class* proxy_class_;
};

template <class T>
t_class* PdFloatProxy<T>::proxy_class_ = nullptr;

template <class T>
class PdListProxy : public PdBindObject {
public:
    typedef void (T::*ProxyFnPtr)(const AtomList&);

    PdListProxy(T* o, ProxyFnPtr p)
        : owner_(o)
        , method_ptr_(p)
    {
        if (proxy_class_ == nullptr)
            initClass();

        assert(proxy_class_);

        t_object* tmp = reinterpret_cast<t_object*>(pd_new(proxy_class_));
        memcpy(&obj_, tmp, sizeof(t_object));
        pd_free((t_pd*)tmp);
    }

    void onList(const AtomList& l)
    {
        (owner_->*method_ptr_)(l);
    }

public:
    static void on_list(PdListProxy* p, t_symbol*, int argc, t_atom* argv)
    {
        p->onList(AtomList(argc, argv));
    }

    static void initClass()
    {
        PdListProxy::proxy_class_ = class_new(gensym("proxy list"), 0, 0, sizeof(PdListProxy), CLASS_PD, A_NULL);
        class_addlist(PdListProxy::proxy_class_, reinterpret_cast<t_method>(PdListProxy::on_list));
    }

private:
    T* owner_;
    ProxyFnPtr method_ptr_;

protected:
    static t_class* proxy_class_;
};

template <class T>
t_class* PdListProxy<T>::proxy_class_ = nullptr;

template <typename T>
struct InletProxy {
public:
    using BangMethodPtr = void (T::*)();
    using FloatMethodPtr = void (T::*)(t_float);
    using AnyMethodPtr = void (T::*)(InletProxy* this_, t_symbol* s, const AtomListView& args);
    using MethodPtr = void (T::*)(const AtomListView& args);
    using MethodEntry = std::pair<t_symbol*, MethodPtr>;
    using MethodList = std::vector<MethodEntry>;

    t_pd x_obj;
    T* dest_;

public:
    InletProxy(T* dest)
        : dest_(dest)
    {
        x_obj = inlet_proxy_class;
    }

    static void init()
    {
        inlet_proxy_class = class_new(gensym("inlet_proxy"), 0, 0, sizeof(InletProxy), CLASS_PD, A_NULL);
    }

    static void on_bang(InletProxy* x)
    {
        auto obj = x->dest_;
        (obj->*bang_)();
    }

    static void on_float(InletProxy* x, t_float f)
    {
        auto obj = x->dest_;
        (obj->*float_)(f);
    }

    static void on_any(InletProxy* x, t_symbol* s, int argc, t_atom* argv)
    {
        auto obj = x->dest_;
        (obj->*any_)(x, s, AtomListView(argv, argc));
    }

    static void on_method(InletProxy* x, t_symbol* s, int argc, t_atom* argv)
    {
        auto it = std::find_if(methods_.begin(), methods_.end(), [s](const MethodEntry& me) { return me.first == s; });
        if (it == methods_.end())
            return;

        auto obj = x->dest_;
        auto mfn = it->second;

        (obj->*mfn)(AtomListView(argv, argc));
    }

    static void set_bang_callback(BangMethodPtr bang_mem_fn)
    {
        bang_ = bang_mem_fn;
        class_addbang(inlet_proxy_class, reinterpret_cast<t_method>(InletProxy::on_bang));
    }

    static void set_float_callback(FloatMethodPtr float_mem_fn)
    {
        float_ = float_mem_fn;
        class_doaddfloat(inlet_proxy_class, reinterpret_cast<t_method>(InletProxy::on_float));
    }

    static void set_any_callback(AnyMethodPtr any_mem_fn)
    {
        any_ = any_mem_fn;
        class_addanything(inlet_proxy_class, reinterpret_cast<t_method>(InletProxy::on_any));
    }

    static void set_method_callback(t_symbol* m, MethodPtr mem_fn)
    {
        methods_.emplace_back(m, mem_fn);
        class_addmethod(inlet_proxy_class, reinterpret_cast<t_method>(InletProxy::on_method), m, A_GIMME, 0);
    }

private:
    static t_class* inlet_proxy_class;
    static BangMethodPtr bang_;
    static FloatMethodPtr float_;
    static AnyMethodPtr any_;
    static MethodList methods_;
};

template <typename T>
t_class* InletProxy<T>::inlet_proxy_class = nullptr;
template <typename T>
typename InletProxy<T>::BangMethodPtr InletProxy<T>::bang_;
template <typename T>
typename InletProxy<T>::FloatMethodPtr InletProxy<T>::float_;
template <typename T>
typename InletProxy<T>::AnyMethodPtr InletProxy<T>::any_;
template <typename T>
typename InletProxy<T>::MethodList InletProxy<T>::methods_;

}

#endif // CEAMMC_PROXY_H
