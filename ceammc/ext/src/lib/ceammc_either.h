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
#ifndef CEAMMC_EITHER_H
#define CEAMMC_EITHER_H

#include <boost/variant.hpp>
#include <string>

#include "ceammc_atomlist.h"

namespace ceammc {

class RuntimeError {
    std::string msg_;

public:
    RuntimeError(const char* msg = "")
        : msg_(msg)
    {
    }

    RuntimeError(const std::string& msg)
        : msg_(msg)
    {
    }

    virtual ~RuntimeError() {}

    const std::string& what() const { return msg_; }
};

template <typename V>
class InvalidValue : public RuntimeError {
    V v_;

public:
    InvalidValue(V v, const char* msg)
        : RuntimeError(msg)
        , v_(v)
    {
    }

    InvalidValue(V v, const std::string& msg)
        : RuntimeError(msg)
        , v_(v)
    {
    }

    const V& value() const { return v_; }
};

template <typename T, typename Error = RuntimeError>
struct Either : public boost::variant<T, Error> {
    typedef Error ErrorType;

    Either(const Error& err)
        : boost::variant<T, Error>(err)
    {
    }

    Either(const T& value)
        : boost::variant<T, Error>(value)
    {
    }

    bool isError() const
    {
        return !isOk();
    }

    bool isOk() const
    {
        return boost::variant<T, Error>::which() == 0;
    }

    bool matchError(Error& err) const
    {
        if (!isError())
            return false;

        err = boost::get<Error>(*this);
        return true;
    }

    bool matchValue(T& value) const
    {
        if (!isOk())
            return false;

        value = boost::get<T>(*this);
        return true;
    }

    template <class U>
    Either<U, Error> call(std::function<U(const T&)> fn)
    {
        if (isError())
            return Either<U, Error>(error());
        else
            return Either<U, Error>(fn(value()));
    }

    template <class U>
    Either<U, Error> call(U (*fn)(const T&))
    {
        if (isError())
            return Either<U, Error>(error());
        else
            return Either<U, Error>(fn(value()));
    }

    T& value()
    {
        return boost::get<T>(*this);
    }

    const T& value() const
    {
        return boost::get<T>(*this);
    }

    const Error& error() const
    {
        return boost::get<Error>(*this);
    }

    template <typename... Args>
    static Either makeError(Args... args)
    {
        return Either(ErrorType(args...));
    }
};

typedef Either<bool, InvalidValue<bool>> EitherBool;
typedef Either<t_float, InvalidValue<t_float>> EitherFloat;
typedef Either<t_symbol*, InvalidValue<t_symbol*>> EitherSymbol;
typedef Either<int, InvalidValue<int>> EitherInt;
typedef Either<size_t, InvalidValue<size_t>> EitherSizeT;
typedef Either<Atom, InvalidValue<Atom>> EitherAtom;
typedef Either<AtomList, InvalidValue<AtomList>> EitherAtomList;

}

#endif // CEAMMC_EITHER_H
