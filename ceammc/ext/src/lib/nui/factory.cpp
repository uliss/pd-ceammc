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
#include "nui/factory.h"
#include "nui/tk_view_impl.h"

namespace ceammc {
namespace ui {

    SymTable::SymTable()
    {
        mouseenter = gensym("mouseenter");
        mouseleave = gensym("mouseleave");
        mousemove = gensym("mousemove");

        tcl_nui_init();
    }

    const SymTable& SymTable::instance()
    {
        static SymTable tab_;
        return tab_;
    }

    namespace utils {
        uint32_t platform_modifier(uint32_t mod)
        {
#ifdef __APPLE__
#elif __WIN32
#else
            if (mod == 24) //right click
                mod = KEY_MOD_CMD;
            else if (mod & KEY_MOD_CMD) {
                mod ^= KEY_MOD_CMD;
                mod |= KEY_MOD_ALT;
            }
#endif
            //            post("MOD : %ld", mod);
            return mod;
        }
    }

}
}
