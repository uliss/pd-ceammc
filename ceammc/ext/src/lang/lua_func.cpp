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

    class LuaStackGuard {
        lua_State* state_;
        int top_;

    public:
        LuaStackGuard(lua_State* l)
            : state_(l)
            , top_(lua_gettop(l))
        {
        }

        ~LuaStackGuard()
        {
            auto current_top = lua_gettop(state_);
            if (current_top > top_) {
                lua_pop(state_, current_top - top_);
            }
        }
    };

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

    void lua_post_stack_idx(lua_State* L, LuaPipe* pipe, SubscriberId id, int i)
    {
        char buf[96];
        snprintf(buf, sizeof(buf), "- %d %s\t", i, luaL_typename(L, i));
        switch (lua_type(L, i)) {
        case LUA_TNUMBER:
            snprintf(buf, sizeof(buf), "%g", lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            snprintf(buf, sizeof(buf), "%s", lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            snprintf(buf, sizeof(buf), "%s", (lua_toboolean(L, i) ? "true" : "false"));
            break;
        case LUA_TNIL:
            snprintf(buf, sizeof(buf), "%s", "nil");
            break;
        default:
            snprintf(buf, sizeof(buf), "#%llx", (uint64_t)lua_topointer(L, i));
            break;
        }

        pipe->try_enqueue({ LUA_CMD_POST, buf });
    }

    struct Context {
        LuaPipe* pipe;
        SubscriberId id;
    };

    Context get_ctx(lua_State* L)
    {
        LuaStackGuard sg(L);

        Context ctx;
        lua_getglobal(L, "_obj");
        ctx.pipe = static_cast<LuaPipe*>(lua_touserdata(L, -1));

        lua_getglobal(L, "_id");
        ctx.id = lua_tointeger(L, -1);

        return ctx;
    }

    int lua_stack_dump(lua_State* L)
    {
        LuaStackGuard sg(L);

        auto ctx = get_ctx(L);
        const int top = lua_gettop(L);

        for (int i = 1; i <= top; i++)
            lua_post_stack_idx(L, ctx.pipe, ctx.id, i);

        return 1;
    }

    LuaAtomList lua_get_args(lua_State* L)
    {
        const int nargs = lua_gettop(L);
        LuaAtomList res(nargs);

        for (int i = 1; i <= nargs; i++) {
            switch (lua_type(L, 1)) {
            case LUA_TNUMBER:
                res.emplace_back(lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                res.emplace_back(lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                res.push_back(LuaDouble(lua_toboolean(L, i) ? 1 : 0));
                break;
            default:
                break;
            }
        }

        return res;
    }

    int lua_pd_send(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);

        if (nargs == 0) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "empty string, usage: pd_send DEST ..." });
            return 0;
        }

        const LuaAtomList args = lua_get_args(L);
        if (!ctx.pipe->try_enqueue({ LUA_CMD_SEND, args }))
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
