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

#include <cassert>
#include <string.h>

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
    typedef void (T::*ProxyFnPtr)(float);

    PdFloatProxy(T* o, ProxyFnPtr p)
        : owner_(o)
        , method_ptr_(p)
    {
        if (proxy_class_ == 0)
            initClass();

        assert(proxy_class_);

        t_object* tmp = (t_object*)pd_new(proxy_class_);
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
        class_addfloat(PdFloatProxy::proxy_class_, PdFloatProxy::on_float);
    }

private:
    T* owner_;
    ProxyFnPtr method_ptr_;

protected:
    static t_class* proxy_class_;
};

template <class T>
t_class* PdFloatProxy<T>::proxy_class_ = 0;

template <class T>
class PdListProxy : public PdBindObject {
public:
    typedef void (T::*ProxyFnPtr)(const AtomList&);

    PdListProxy(T* o, ProxyFnPtr p)
        : owner_(o)
        , method_ptr_(p)
    {
        if (proxy_class_ == 0)
            initClass();

        assert(proxy_class_);

        t_object* tmp = (t_object*)pd_new(proxy_class_);
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
        class_addlist(PdListProxy::proxy_class_, PdListProxy::on_list);
    }

private:
    T* owner_;
    ProxyFnPtr method_ptr_;

protected:
    static t_class* proxy_class_;
};

template <class T>
t_class* PdListProxy<T>::proxy_class_ = 0;
}

#endif // CEAMMC_PROXY_H
