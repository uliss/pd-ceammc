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
#ifndef CEAMMC_FUNCTION_H
#define CEAMMC_FUNCTION_H

#include "ceammc_atomlist.h"

#include <functional>
#include <memory>

namespace mu {
class Parser;
}

namespace ceammc {

using BuiltinFunction = std::function<AtomList(const AtomListView& args)>;

class BuiltinFunctionMap {
    using FnPair = std::pair<t_symbol*, BuiltinFunction>;
    using FnMap = std::vector<FnPair>;
    using MathParser = std::unique_ptr<mu::Parser>;

    FnMap fn_map_;
    MathParser math_parser_;

    BuiltinFunctionMap();
    ~BuiltinFunctionMap();
    BuiltinFunctionMap(const BuiltinFunctionMap&) = delete;

public:
    static BuiltinFunctionMap& instance();

    AtomList call(t_symbol* name, const AtomListView& args) const;
    bool exists(t_symbol* name) const;

    bool registerFn(t_symbol* name, BuiltinFunction fn);
};

}

#endif // CEAMMC_FUNCTION_H
