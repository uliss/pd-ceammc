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
 * @param lst - raw input list
 * @return parsed list on success or unmodified list if parsing failed
 */
AtomList parseDataList(const AtomList& lst) noexcept;
AtomList parseDataList(const AtomListView& view) noexcept;

/**
 * @throw std::runtime_error on error
 */
AtomList parseDataString(const std::string& str);

}

#endif // CEAMMC_DATA_H
