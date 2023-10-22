/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_DATA_H
#define CEAMMC_DATA_H

#include "ceammc_atomlist.h"

#include <string>

namespace ceammc {

class DataTypeMList;
class DataTypeDict;
class DataTypeString;

template <typename T>
class DataAtom : public Atom {
public:
    explicit DataAtom(const Atom& a)
        : Atom(a)
    {
    }

    explicit DataAtom(const T& data)
        : Atom(new T(data))
    {
    }

    explicit DataAtom(T&& data)
        : Atom(new T(std::move(data)))
    {
    }

    explicit DataAtom(Atom&& a)
        : Atom(std::move(a))
    {
    }

    DataAtom(const DataAtom& x) = default;

    template <typename... Args>
    explicit DataAtom(Args... args)
        : Atom(new T(args...))
    {
    }

    operator bool() const noexcept { return isA<T>(); }

    T* operator->() noexcept { return static_cast<T*>(const_cast<AbstractData*>(asData())); }
    const T* operator->() const noexcept { return static_cast<const T*>(asData()); }

    T& operator*() noexcept { return *(operator->()); }
    const T& operator*() const noexcept { return *(operator->()); }
};

using MListAtom = DataAtom<DataTypeMList>;
using DictAtom = DataAtom<DataTypeDict>;
using StringAtom = DataAtom<DataTypeString>;

static_assert(sizeof(MListAtom) == sizeof(Atom), "DataAtom size mismatch");

class DataParseResult {
    AtomList res_;
    std::string err_;
    bool ok_ = { true };

    DataParseResult() = delete;

public:
    DataParseResult(const DataParseResult& r) noexcept;
    DataParseResult(DataParseResult&& r) noexcept;
    DataParseResult& operator=(const DataParseResult& r) noexcept;
    DataParseResult& operator=(DataParseResult&& r) noexcept;

    explicit DataParseResult(const std::string& err)
        : err_(err)
        , ok_(false)
    {
    }

    explicit DataParseResult(const AtomList& l)
        : res_(l)
        , ok_(true)
    {
    }

    explicit DataParseResult(AtomList&& l)
        : res_(std::move(l))
        , ok_(true)
    {
    }

    operator bool() const { return ok_; }

    const std::string& err() const { return err_; }
    const AtomList& result() const { return res_; }
};

/**
 * Parse raw list with data constructor syntax:
 *  - **null**  -> Atom()
 *  - **true**  -> 1.0
 *  - **false** -> 0.0
 *  - 2.345     -> 2.345
 *  - abc       -> 'abc' t_symbol*
 *  - "a b c"   -> 'a b c' t_symbol* with spaces
 *  -
 * @example (1 2 3) -> MListAtom(1, 2, 3)
 * @example 1 2 3 -> AtomList(1, 2, 3)
 * @example [key: 1 2 3] -> DictAtom
 * @param lst - raw input list
 * @param log - error logger, use LogNone to disable logging
 * @return parse result
 */
DataParseResult parseDataList(const AtomList& lst) noexcept;
DataParseResult parseDataList(const AtomListView& view) noexcept;

/**
 * Parse string
 * @return parse result
 */
DataParseResult parseDataString(const char* str) noexcept;

}

#endif // CEAMMC_DATA_H
