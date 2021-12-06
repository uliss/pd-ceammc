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
#ifndef LUA_STACK_GUARD_H
#define LUA_STACK_GUARD_H

#include "lua.hpp"

#include <iostream>

namespace ceammc {
namespace lua {

    class LuaStackGuard {
        lua_State* state_;
        int top_;
        bool debug_;

    public:
        LuaStackGuard(lua_State* l, bool debug = false)
            : state_(l)
            , top_(lua_gettop(l))
            , debug_(debug)
        {
            if (debug_)
                std::cerr << "[ LuaStackGuard] stack size: " << top_ << std::endl;
        }

        ~LuaStackGuard()
        {
            if (debug_)
                std::cerr << "[~LuaStackGuard] stack size: " << top_ << std::endl;

            auto current_top = lua_gettop(state_);
            if (current_top > top_) {
                lua_pop(state_, current_top - top_);
            }
        }
    };

}
}

#endif // LUA_STACK_GUARD_H
