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
#ifndef CEAMMC_OUTLET_H
#define CEAMMC_OUTLET_H

#include "m_pd.h"

namespace ceammc {
namespace util {
    bool outlet_is_signal(t_outlet* x);
    void outlet_set_signal(t_outlet* x, bool v);
}

class OutletIterator {
    const t_object* obj_;
    t_outlet* outlet_;

public:
    OutletIterator(const t_object* obj);

    bool isSignal() const;
    void setSignal(bool v);
    void reset();
    OutletIterator& next();
    operator bool();

    t_outlet* outlet() { return outlet_; }
};

class ConnectionIterator {
    const t_object* obj_;
    t_outlet* src_outlet_;
    t_outconnect* conn_;
    t_object* dest_;
    t_inlet* dest_inlet_;
    int dest_idx_;

public:
    ConnectionIterator(const t_object* obj);

    void reset();
    ConnectionIterator& next();

    operator bool() { return conn_; }

    t_outlet* srcOutlet() { return src_outlet_; }

    t_object* dest() { return dest_; }
    t_inlet* destInlet() { return dest_inlet_; }
    int destIndex() const { return dest_idx_; }

    bool isSignal() const;
};

}

#endif // CEAMMC_OUTLET_H
