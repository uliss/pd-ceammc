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

#include "ceammc_atomlist_view.h"

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

    void bind(const char* name) { bind(gensym(name)); }

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
            if (bind_name_->s_thing)
                pd_unbind(pd(), bind_name_);

            bind_name_ = &s_;
        }
    }

    t_symbol* bindName() const { return bind_name_; }

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
    typedef void (T::*ProxyFnPtr)(const AtomListView&);

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

    void onList(const AtomListView& lv)
    {
        (owner_->*method_ptr_)(lv);
    }

public:
    static void on_list(PdListProxy* p, t_symbol*, int argc, t_atom* argv)
    {
        p->onList(AtomListView(argv, argc));
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
    using BangMethodPtr = void (T::*)(int);
    using FloatMethodPtr = void (T::*)(int, t_float);
    using SymbolMethodPtr = void (T::*)(int, t_symbol*);
    using ListMethodPtr = void (T::*)(int, const AtomListView&);
    using AnyMethodPtr = void (T::*)(int, t_symbol*, const AtomListView&);
    using MethodPtr = void (T::*)(const AtomListView& args);
    using MethodEntry = std::pair<t_symbol*, MethodPtr>;
    using MethodList = std::vector<MethodEntry>;

    t_pd x_obj;
    T* dest_;
    int idx_;

public:
    InletProxy(T* dest, int idx = -1)
        : dest_(dest)
        , idx_(idx)
    {
        x_obj = inlet_proxy_class;
    }

    t_pd* target() { return &x_obj; }
    int index() const { return idx_; }
    void setIndex(int idx) { idx_ = idx; }

    static void init()
    {
        if (!inlet_proxy_class)
            inlet_proxy_class = class_new(gensym("inlet_proxy"), 0, 0, sizeof(InletProxy), CLASS_PD, A_NULL);
    }

    static void on_bang(InletProxy* x)
    {
        auto obj = x->dest_;
        (obj->*bang_)(x->idx_);
    }

    static void on_float(InletProxy* x, t_float f)
    {
        auto obj = x->dest_;
        (obj->*float_)(x->idx_, f);
    }

    static void on_symbol(InletProxy* x, t_symbol* s)
    {
        auto obj = x->dest_;
        (obj->*symbol_)(x->idx_, s);
    }

    static void on_list(InletProxy* x, t_symbol* /*s*/, int argc, t_atom* argv)
    {
        auto obj = x->dest_;
        (obj->*list_)(x->idx_, AtomListView(argv, argc));
    }

    static void on_any(InletProxy* x, t_symbol* s, int argc, t_atom* argv)
    {
        auto obj = x->dest_;
        (obj->*any_)(x->idx_, s, AtomListView(argv, argc));
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

    static void set_symbol_callback(SymbolMethodPtr symbol_mem_fn)
    {
        symbol_ = symbol_mem_fn;
        class_addsymbol(inlet_proxy_class, reinterpret_cast<t_method>(InletProxy::on_symbol));
    }

    static void set_list_callback(ListMethodPtr list_mem_fn)
    {
        list_ = list_mem_fn;
        class_addlist(inlet_proxy_class, reinterpret_cast<t_method>(InletProxy::on_list));
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
    static SymbolMethodPtr symbol_;
    static ListMethodPtr list_;
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
typename InletProxy<T>::SymbolMethodPtr InletProxy<T>::symbol_;
template <typename T>
typename InletProxy<T>::ListMethodPtr InletProxy<T>::list_;
template <typename T>
typename InletProxy<T>::AnyMethodPtr InletProxy<T>::any_;
template <typename T>
typename InletProxy<T>::MethodList InletProxy<T>::methods_;

template <typename T, typename Data>
class DataProxy {
    t_object obj_;
    Data data_;

public:
    DataProxy(T* o, const Data& data)
        : data_(data)
        , owner_(o)
    {
        if (data_proxy_class_ == nullptr)
            initClass();

        assert(data_proxy_class_);

        t_object* tmp = reinterpret_cast<t_object*>(pd_new(data_proxy_class_));
        memcpy(&obj_, tmp, sizeof(t_object));
        pd_free((t_pd*)tmp);
    }

    void onBang() { (owner_->*bang_method)(data_); }
    void onFloat(t_float f) { (owner_->*float_method)(data_, f); }
    void onSymbol(t_symbol* s) { (owner_->*symbol_method)(data_, s); }
    void onList(const AtomListView& lv) { (owner_->*list_method)(data_, lv); }
    void onAny(t_symbol* s, const AtomListView& lv) { (owner_->*any_method)(data_, s, lv); }

    t_object* object() { return &obj_; }
    T* owner() { return owner_; }
    const Data& data() const { return data_; }

public:
    static void on_bang(DataProxy* p) { p->onBang(); }
    static void on_float(DataProxy* p, t_float f) { p->onFloat(f); }
    static void on_symbol(DataProxy* p, t_symbol* s) { p->onSymbol(s); }
    static void on_list(DataProxy* p, t_symbol*, int argc, t_atom* argv) { p->onList(AtomListView(argv, argc)); }
    static void on_any(DataProxy* p, t_symbol* s, int argc, t_atom* argv) { p->onAny(s, AtomListView(argv, argc)); }

    static void initClass()
    {
        DataProxy::data_proxy_class_ = class_new(gensym("proxy data"), 0, 0, sizeof(DataProxy), CLASS_PD, A_NULL);

        auto cls = DataProxy::data_proxy_class_;
        class_addbang(cls, reinterpret_cast<t_method>(DataProxy::on_bang));
        class_doaddfloat(cls, reinterpret_cast<t_method>(DataProxy::on_float));
        class_addsymbol(cls, reinterpret_cast<t_method>(DataProxy::on_symbol));
        class_addlist(cls, reinterpret_cast<t_method>(DataProxy::on_list));
        class_addanything(cls, reinterpret_cast<t_method>(DataProxy::on_any));

        bang_method = &T::data_proxy_bang;
        float_method = &T::data_proxy_float;
        symbol_method = &T::data_proxy_symbol;
        list_method = &T::data_proxy_list;
        any_method = &T::data_proxy_any;
    }

private:
    T* owner_;

private:
    static t_class* data_proxy_class_;

    using BangMethod = void (T::*)(const Data&);
    using FloatMethod = void (T::*)(const Data&, t_float);
    using SymbolMethod = void (T::*)(const Data&, t_symbol*);
    using ListMethod = void (T::*)(const Data&, const AtomListView&);
    using AnyMethod = void (T::*)(const Data&, t_symbol*, const AtomListView&);

    static BangMethod bang_method;
    static FloatMethod float_method;
    static SymbolMethod symbol_method;
    static ListMethod list_method;
    static AnyMethod any_method;
};

template <typename T, typename D>
t_class* DataProxy<T, D>::data_proxy_class_ = nullptr;
template <typename T, typename D>
typename DataProxy<T, D>::BangMethod DataProxy<T, D>::bang_method = nullptr;
template <typename T, typename D>
typename DataProxy<T, D>::FloatMethod DataProxy<T, D>::float_method = nullptr;
template <typename T, typename D>
typename DataProxy<T, D>::SymbolMethod DataProxy<T, D>::symbol_method = nullptr;
template <typename T, typename D>
typename DataProxy<T, D>::ListMethod DataProxy<T, D>::list_method = nullptr;
template <typename T, typename D>
typename DataProxy<T, D>::AnyMethod DataProxy<T, D>::any_method = nullptr;
}

#endif // CEAMMC_PROXY_H
