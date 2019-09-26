/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_EXTRA_H
#define CEAMMC_PROPERTY_EXTRA_H

#include "ceammc_either.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_property.h"
#include "ceammc_property_info.h"

#include <boost/version.hpp>
#if BOOST_VERSION >= 105900
#include <boost/core/demangle.hpp>
#define CEAMMC_DEMANGLE(str) boost::core::demangle(str)
#else
#define CEAMMC_DEMANGLE(str) str
#endif

#include <type_traits>

namespace ceammc {

template <typename V>
class LambdaCheckProperty : public Property {
    typedef Either<V, InvalidValue<V>> EitherValue;
    std::function<EitherValue(V)> check_fn_;
    V value_;
    V default_value_;

public:
    template <typename CheckFn>
    LambdaCheckProperty(const std::string& name, V defaultVal, CheckFn check_fn)
        : Property(PropertyInfo(name, PropertyInfo::toType<V>()), false)
        , check_fn_(check_fn)
        , default_value_(defaultVal)
    {
        if (!valueCheck(defaultVal))
            throw std::logic_error("invalid default value");

        value_ = default_value_;
    }

    bool set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        if (!convertCheck(lst))
            return false;

        if (!lambdaCheck(lst))
            return false;

        return true;
    }

    AtomList get() const
    {
        return Atom(value_);
    }

    V value() const
    {
        return value_;
    }

    void setValue(V v)
    {
        if (!valueCheck(v))
            return;

        value_ = v;
    }

private:
    bool convertCheck(const AtomList& l) const
    {
        if (!list::canConvertToType<V>(l)) {
            LIB_ERR << name() << ": can't convert " << l << " to "
                    << CEAMMC_DEMANGLE(typeid(V).name()) << " type";
            return false;
        }

        return true;
    }

    bool lambdaCheck(const AtomList& lst)
    {
        V v = atomlistToValue<V>(lst, default_value_);
        if (!valueCheck(v))
            return false;

        value_ = v;
        return true;
    }

    bool valueCheck(V v)
    {
        EitherValue res = check_fn_(v);
        if (res.isError()) {
            LIB_ERR << "invalid value " << res.error().value() << " for property " << name()
                    << ": " << res.error().what();
            return false;
        }

        return true;
    }
};

/**
 * Calls lambda function after successfull setting value
 */
template <typename Owner, typename Prop>
class LambdaCallProperty : public Prop {
    std::function<void(Owner*, const AtomList&)> fn_;
    Owner* this_;

public:
    template <typename Fn, typename... Args>
    LambdaCallProperty(Owner* obj, Fn fn, Args... args)
        : Prop(args...)
        , this_(obj)
        , fn_(fn)
    {
        static_assert(std::is_base_of<Property, Prop>::value, "not a Property subclass");
    }

    bool set(const AtomList& l) override
    {
        bool ok = Prop::set(l);

        if (ok)
            fn_(this_, Prop::get());

        return ok;
    }
};

}

#endif // CEAMMC_PROPERTY_EXTRA_H
