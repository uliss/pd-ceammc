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
#include "lua_func.h"
#include "ceammc_convert.h"
#include "lua_cmd.h"
#include "pollthread_object.h"

namespace ceammc {
namespace lua {
    using LuaPipe = PollThreadQueue<LuaCmd>;

    int lua_mtof(lua_State* L)
    {
        const auto m = luaL_checknumber(L, 1);
        const auto baseA = luaL_optnumber(L, 2, 440);
        const auto freq = convert::midi2freq(m, baseA);
        lua_pushnumber(L, freq);

        return 1;
    }

    int lua_pd_post(lua_State* L)
    {
        const char* str = luaL_checkstring(L, 1);

        lua_getglobal(L, "_obj");
        if (lua_islightuserdata(L, -1) != 1)
            return 0;

        LuaPipe* pipe = static_cast<LuaPipe*>(lua_touserdata(L, -1));
        if (!pipe)
            return 0;

        lua_getglobal(L, "_id");
        SubscriberId id = luaL_checkinteger(L, -1);

        if (!pipe->try_enqueue({ LUA_CMD_POST, str }))
            return 0;

        if (!Dispatcher::instance().send({ id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_pd_error(lua_State* L)
    {
        const char* str = luaL_checkstring(L, 1);

        lua_getglobal(L, "_obj");
        if (lua_islightuserdata(L, -1) != 1)
            return 0;

        LuaPipe* pipe = static_cast<LuaPipe*>(lua_touserdata(L, -1));
        if (!pipe)
            return 0;

        lua_getglobal(L, "_id");
        SubscriberId id = luaL_checkinteger(L, -1);

        if (!pipe->try_enqueue({ LUA_CMD_ERROR, str }))
            return 0;

        if (!Dispatcher::instance().send({ id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_output(lua_State* L)
    {
        auto f = luaL_checknumber(L, 1);
        LuaInt n = luaL_optinteger(L, 2, 0);

        lua_getglobal(L, "_obj");
        if (lua_islightuserdata(L, -1) != 1)
            return 0;

        LuaPipe* pipe = static_cast<LuaPipe*>(lua_touserdata(L, -1));
        if (!pipe)
            return 0;

        lua_getglobal(L, "_id");
        SubscriberId id = luaL_checkinteger(L, -1);

        LuaCmd cmd(LUA_CMD_OUTPUT, n);
        cmd.appendArg(f);

        if (!pipe->try_enqueue(cmd))
            return 0;

        if (!Dispatcher::instance().send({ id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_sleep(lua_State* L)
    {
        auto n = luaL_checkinteger(L, 1);
        if (n < 1)
            return 0;

        std::this_thread::sleep_for(std::chrono::milliseconds(n));
        return 1;
    }
}
}
