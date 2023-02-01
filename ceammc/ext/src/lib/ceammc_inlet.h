/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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

#ifndef CEAMMC_INLET_H
#define CEAMMC_INLET_H

#include "m_pd.h"

namespace ceammc {
namespace util {
    size_t inlet_count(t_object* x);
    t_inlet* inlet_next(t_inlet* x);
    t_object* inlet_owner(t_inlet* x);
    t_pd* inlet_get_dest(t_inlet* x);
    void inlet_set_dest(t_inlet* x, t_pd* dest);
    t_pd* inlet_object(t_inlet* x);
    bool inlet_is_signal(t_inlet* x);
    void inlet_set_signal(t_inlet* x, bool v);
}

class InletIterator {
    const t_object* obj_;
    t_inlet* inlet_;

public:
    InletIterator(const t_object* obj);

    bool isSignal() const;
    void setSignal(bool v);
    void reset();
    InletIterator& next();
    operator bool();

    t_inlet* inlet() { return inlet_; }
    t_pd* asObject();

    t_pd* destination();
    void setDestination(t_pd* dest);
};

}

#endif // CEAMMC_INLET_H
