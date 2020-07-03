/*****************************************************************************
 *
 * Created: 17.10.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_H
#define CEAMMC_H

#include "m_pd.h"

#include <stdexcept>
#include <string>

namespace ceammc {

struct OutletIdx {
    unsigned long long n;

    operator unsigned long long() const { return n; }
};

struct InletIdx {
    unsigned long long n;

    operator unsigned long long() const { return n; }
};

namespace literals {
    inline OutletIdx operator"" _out(unsigned long long n) { return { n }; }
    inline InletIdx operator"" _in(unsigned long long n) { return { n }; }
}

struct Exception : public std::runtime_error {
public:
    Exception(const char* msg)
        : std::runtime_error(msg)
    {
    }

    Exception(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
};

class InvalidOutlet : public Exception {
    OutletIdx n_;

public:
    InvalidOutlet(OutletIdx n) noexcept;
    const char* what() const noexcept;
};

}

#endif // CEAMMC_H
