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

    bool lua_get_atom(lua_State* L, int i, LuaAtom& a)
    {
        switch (lua_type(L, i)) {
        case LUA_TNUMBER:
            a = lua_tonumber(L, i);
            return true;
        case LUA_TSTRING:
            a = lua_tostring(L, i);
            return true;
        case LUA_TBOOLEAN:
            a = LuaDouble(lua_toboolean(L, i) ? 1 : 0);
            return true;
        default:
            return false;
        }
    }

    LuaAtomList lua_get_args(lua_State* L)
    {
        const int nargs = lua_gettop(L);
        LuaAtomList res;
        res.reserve(nargs);

        for (int i = 1; i <= nargs; i++) {
            LuaAtom a;
            if (lua_get_atom(L, i, a))
                res.push_back(a);
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

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    static LuaAtomList get_list_table(lua_State* L, int i)
    {
        LuaStackGuard sg(L);
        LuaAtomList res;

        lua_pushnil(L); /* first key */
        while (lua_next(L, i) != 0) {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            printf("%s - %s\n",
                lua_typename(L, lua_type(L, -2)),
                lua_typename(L, lua_type(L, -1)));

            /* removes 'value'; keeps 'key' for next iteration */
            LuaAtom a;
            if (lua_get_atom(L, -1, a))
                res.push_back(a);

            lua_pop(L, 1);
        }

        return res;
    }

    static LuaAtomList get_param_as_list(lua_State* L, int i)
    {
        LuaStackGuard sg(L);
        LuaAtomList data;

        switch (lua_type(L, i)) {
        case LUA_TNUMBER:
            data.push_back(lua_tonumber(L, i));
            break;
        case LUA_TBOOLEAN:
            data.push_back(LuaAtom(LuaDouble(lua_toboolean(L, i) ? 1 : 0)));
            break;
        case LUA_TSTRING:
            data.push_back(lua_tostring(L, i));
            break;
        case LUA_TTABLE: {
            auto tab = get_list_table(L, i);
            data.insert(data.end(), tab.begin(), tab.end());
        } break;
        default:
            return data;
        }

        return data;
    }

    int lua_output(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);

        LuaStackGuard sg(L);

        if (nargs < 1 || nargs > 2) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: value outlet?" });
            return 0;
        }

        LuaAtomList data;
        LuaInt n = luaL_optinteger(L, 2, 0);
        data.push_back(n);

        const auto args = get_param_as_list(L, 1);
        data.insert(data.end(), args.begin(), args.end());

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_OUTPUT, data)))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_message(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);

        LuaStackGuard sg(L);

        if (nargs < 1 || nargs > 3) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: sel value? outlet?" });
            return 0;
        }

        LuaAtomList data;
        LuaInt n = luaL_optinteger(L, 3, 0);
        data.push_back(n);

        if (!lua_isstring(L, 1)) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "first arg should be string" });
            return 0;
        }

        data.push_back(lua_tostring(L, 1));

        const auto args = get_param_as_list(L, 2);
        data.insert(data.end(), args.begin(), args.end());

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_MESSAGE, data)))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
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
