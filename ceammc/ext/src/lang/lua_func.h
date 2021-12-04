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
#ifndef LUA_FUNC_H
#define LUA_FUNC_H

#include "lua.hpp"

namespace ceammc {
namespace lua {
    int lua_mtof(lua_State* L);
    int lua_output(lua_State* L);
    int lua_pd_error(lua_State* L);
    int lua_pd_post(lua_State* L);
    int lua_pd_send(lua_State* L);
    int lua_stack_dump(lua_State* L);
    int lua_sleep(lua_State* L);
}
}

#endif // LUA_FUNC_H
