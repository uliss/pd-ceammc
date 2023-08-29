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
#include "ceammc_rtree.h"
#include "fmt/core.h"

#include <iostream>

namespace ceammc {
namespace rtree {

    AtomList rhythm_tree(t_float duration, const DataTypeMList* ml, const char* errPrefix, std::ostream& err)
    {
        if (!ml || ml->empty())
            return { duration };
        else {
            t_float sum = 0;
            for (auto& a : *ml) {
                if (a.isFloat()) {
                    auto f = a.asT<t_float>();
                    if (f <= 0) {
                        err << errPrefix << fmt::format("invalid beat length: {} in '{}', should be >0", f, ml->toString());
                        return {};
                    }

                    sum += a.asT<t_float>();
                }
            }

            if (sum <= 0)
                return {};

            AtomList res;
            const auto N = ml->size();
            for (size_t i = 0; i < N; i++) {
                const auto& a = ml->at(i);
                if (a.isFloat()) {
                    const auto f = a.asT<t_float>();
                    const auto subdur = (duration * f) / sum;
                    // number followed by mlist
                    if ((i + 1 < N) && ml->at(i + 1).isDataType(DataTypeMList::staticType())) {
                        res.append(rhythm_tree(subdur, ml->at(i + 1).asDataT<DataTypeMList>(), errPrefix, err));
                        i += 1;
                    } else {
                        res.append(subdur);
                    }
                }
            }
            return res;
        }
    }
}

}
