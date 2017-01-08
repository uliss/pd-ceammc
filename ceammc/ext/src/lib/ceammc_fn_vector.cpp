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
#include "ceammc_fn_vector.h"

#include <algorithm>
#include <cmath>

namespace ceammc {
namespace vector {
    double magnitude(const AtomList& l)
    {
        double res = 0.0;
        if (l.empty())
            return res;

        for (size_t i = 0; i < l.size(); i++) {
            float v = 0.0;
            if (l[i].getFloat(&v))
                res += (v * v);
        }

        return sqrt(res);
    }

    double dotProduct(const AtomList& a, const AtomList& b)
    {
        const size_t len = std::min(a.size(), b.size());
        double res = 0.0;
        for (size_t i = 0; i < len; i++) {
            res += a[i].asFloat(0) * b[i].asFloat(0);
        }

        return res;
    }

    double distance(const AtomList& a, const AtomList& b)
    {
        const size_t len = std::min(a.size(), b.size());
        double res = 0.0;
        for (size_t i = 0; i < len; i++) {
            const double p = a[i].asFloat(0) - b[i].asFloat(0);
            res += p * p;
        }

        return sqrt(res);
    }
}
}
