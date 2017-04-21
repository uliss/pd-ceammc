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
#ifndef CEAMMC_PD_H
#define CEAMMC_PD_H

#include "m_pd.h"

namespace ceammc {
namespace pd {
    class External {
    private:
        t_object* obj_;

    public:
        External(const char* name);
        ~External();

        bool isNull() const;
        bool connectTo(int outn, t_object* dest, int inln);

        t_object* object();

        void bang();
    };
}
}

#endif // CEAMMC_PD_H
