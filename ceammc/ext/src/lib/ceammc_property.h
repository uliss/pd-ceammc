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

#include <string>

namespace ceammc {

class Property {
    std::string name_;
    bool readonly_;

public:
    Property(const std::string& name, bool readonly = false);
    virtual ~Property();

    std::string name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

    bool readonly() const { return readonly_; }

    virtual bool set(const AtomList&) = 0;
    virtual AtomList get() const = 0;

protected:
    bool readonlyCheck() const;
    bool emptyValueCheck(const AtomList& v) const;
};

class AtomProperty : public Property {
    Atom v_;

public:
    AtomProperty(const std::string& name, const Atom& a, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    Atom value() const { return v_; }
    void setValue(const Atom& v) { v_ = v; }
};

class ListProperty : public Property {
    AtomList lst_;

public:
    ListProperty(const std::string& name, const AtomList& l = AtomList(), bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;
};

class FloatProperty : public Property {
    float v_;

public:
    FloatProperty(const std::string& name, float init = 0, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    float value() const { return v_; }
    void setValue(float v) { v_ = v; }
};

class BoolProperty : public Property {
    bool v_;

public:
    BoolProperty(const std::string& name, bool init, bool readonly = false);
    bool set(const AtomList& lst);
    AtomList get() const;

    bool value() const { return v_; }
    void setValue(bool v) { v_ = v; }
};

template <typename T>
class CallbackProperty : public Property {
public:
    typedef AtomList (T::*GetterFn)() const;
    typedef void (T::*SetterFn)(const AtomList& v);

public:
    CallbackProperty(const std::string& name, T* obj, GetterFn gf, SetterFn sf = 0)
        : Property(name, sf == 0 ? true : false)
        , obj_(obj)
        , getter_(gf)
        , setter_(sf)
    {
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        if (!emptyValueCheck(lst))
            return false;

        (obj_->*setter_)(lst);
        return true;
    }

    AtomList get() const
    {
        return (obj_->*getter_)();
    }

protected:
    T* obj_;

private:
    GetterFn getter_;
    SetterFn setter_;
};

template <typename T, typename B>
class TypedCbProperty : public CallbackProperty<TypedCbProperty<T, B> > {
    typedef T (B::*TGetterFn)() const;
    typedef void (B::*SGetterFn)(const T&);

public:
    TypedCbProperty(const std::string& name, B* obj, TGetterFn gf, SGetterFn sf = 0)
        : CallbackProperty<TypedCbProperty<T, B> >(name,
              this,
              &TypedCbProperty::defGetter, sf == 0 ? 0 : &TypedCbProperty::defSetter)
        , bobj_(obj)
        , tgetter_(gf)
        , tsetter_(sf)

    {
    }

private:
    AtomList defGetter() const
    {
        T v = (this->bobj_->*tgetter_)();
        return listFrom(v);
    }

    void defSetter(const AtomList& l)
    {
        T v = atomlistToValue<T>(l, T());
        (this->bobj_->*tsetter_)(v);
    }

private:
    B* bobj_;
    TGetterFn tgetter_;
    SGetterFn tsetter_;
};
}

#endif // CEAMMC_PROPERTY_H
