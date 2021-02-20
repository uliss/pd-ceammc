/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "common.h"

extern "C" {
#include "m_imp.h"
}

namespace ceammc {
namespace ui {

    Xlets::Xlets()
        : size_(0)
    {
        xlets_.fill(0);
    }

    Xlets Xlets::fromInlets(t_object* x)
    {
        Xlets res;
        const int N_OUT = obj_ninlets(x);
        for (int i = 0; i < N_OUT; i++)
            res.append(obj_issignalinlet(x, i) ? XLET_AUDIO : XLET_CONTROL);

        return res;
    }

    Xlets Xlets::fromOutlets(t_object* x)
    {
        Xlets res;
        const int N_OUT = obj_noutlets(x);
        for (int i = 0; i < N_OUT; i++)
            res.append(obj_issignaloutlet(x, i) ? XLET_AUDIO : XLET_CONTROL);

        return res;
    }

    bool Xlets::append(XletType t)
    {
        if ((size_ + 1) < UI_MAX_XLET_NUMBER) {
            xlets_[size_++] = t;
            return true;
        } else
            return false;
    }

}
}
