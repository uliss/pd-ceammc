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

class DataTypeTree;
class DataTypeMList;
class DataTypeDict;
class DataTypeString;

template <typename T>
class TypeAtom : public Atom {
public:
    template <typename... Args>
    TypeAtom(Args... args)
        : Atom(new T(args...))
    {
    }

    T* operator->() { return static_cast<T*>(const_cast<AbstractData*>(asData())); }
    const T* operator->() const { return static_cast<const T*>(asData()); }

    T& operator*() { return *(operator->()); }
    const T& operator*() const { return *(operator->()); }
};

using MListAtom = TypeAtom<DataTypeMList>;
using DictAtom = TypeAtom<DataTypeDict>;
using TreeAtom = TypeAtom<DataTypeTree>;
using StringAtom = TypeAtom<DataTypeString>;

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

/**
 * @throw std::runtime_error on error
 */
AtomList parseDataString(const std::string& str);

}

#endif // CEAMMC_DATA_H
