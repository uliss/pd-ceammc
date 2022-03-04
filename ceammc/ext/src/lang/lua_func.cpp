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
#include "fmt/format.h"
#include "lua_cmd.h"
#include "lua_stack_guard.h"
#include "pollthread_object.h"

#include <chrono>
#include <thread>

constexpr size_t MAX_TABLE_LEN = 256;

namespace ceammc {
namespace lua {

    using LuaPipe = PollThreadQueue<LuaCmd>;

    struct Context {
        LuaPipe* pipe;
        SubscriberId id;
    };

    static const char* type_to_string(int t)
    {
        switch (t) {
        case LUA_TNONE:
            return "none";
        case LUA_TNIL:
            return "nil";
        case LUA_TBOOLEAN:
            return "boolean";
        case LUA_TLIGHTUSERDATA:
            return "lightuserdata";
        case LUA_TNUMBER:
            return "number";
        case LUA_TSTRING:
            return "string";
        case LUA_TTABLE:
            return "table";
        case LUA_TFUNCTION:
            return "function";
        case LUA_TUSERDATA:
            return "userdata";
        case LUA_TTHREAD:
            return "thread";
        default:
            return "unknown";
        }
    }

    static void check_arg_type(const Context& ctx, const char* name, int expected_type, lua_State* lua, int idx)
    {
        auto real_type = lua_type(lua, idx);
        if (real_type != expected_type && ctx.pipe) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR,
                fmt::format("'{}' expected to be '{}', got '{}'", name, type_to_string(expected_type), type_to_string(real_type)) });
        }
    }

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
            a = LuaAtom(lua_tonumber(L, i));
            return true;
        case LUA_TSTRING:
            a = LuaAtom(lua_tostring(L, i));
            return true;
        case LUA_TBOOLEAN:
            a = LuaAtom(lua_toboolean(L, i) ? true : false);
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
            data.emplace_back(lua_tonumber(L, i));
            break;
        case LUA_TBOOLEAN:
            data.emplace_back(lua_toboolean(L, i) ? true : false);
            break;
        case LUA_TSTRING:
            data.emplace_back(lua_tostring(L, i));
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

    int lua_bang_to(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const auto nargs = lua_gettop(L);

        LuaStackGuard sg(L);

        if (nargs != 1) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: bang_to(outlet)" });
            return 0;
        }

        const LuaInt n = luaL_optinteger(L, 1, 0);
        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_BANG_TO, n)))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_float_to(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const auto nargs = lua_gettop(L);

        if (nargs != 2) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: float_to(outlet, value)" });
            return 0;
        }

        LuaStackGuard sg(L);

        const LuaInt n = luaL_optinteger(L, 1, 0);
        const LuaDouble f = luaL_optnumber(L, 2, 0);

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_FLOAT_TO, LuaAtomList { LuaAtom { n }, LuaAtom { f } })))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_symbol_to(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const auto nargs = lua_gettop(L);

        if (nargs != 2) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: symbol_to(outlet, value)" });
            return 0;
        }

        LuaStackGuard sg(L);

        const LuaInt n = luaL_optinteger(L, 1, 0);
        const char* str = luaL_optstring(L, 2, "");

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_SYMBOL_TO, LuaAtomList { LuaAtom { n }, LuaAtom { str } })))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_list_to(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const auto nargs = lua_gettop(L);
        const auto targ2 = lua_type(L, 2);

        const bool is_list_arg = (nargs == 2 && targ2 == LUA_TTABLE);
        const bool is_tuple_arg = (nargs >= 2 && targ2 != LUA_TTABLE);

        if (!is_list_arg && !is_tuple_arg) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: list_to(outlet, list? or values?)" });
            return 0;
        }

        LuaStackGuard sg(L);

        const LuaInt n = luaL_optinteger(L, 1, 0);
        LuaAtomList data;
        data.reserve(nargs);
        data.emplace_back(n);

        if (is_list_arg) {
            auto tab = get_list_table(L, 2);
            data.insert(data.end(), tab.begin(), tab.end());
        } else if (is_tuple_arg) {
            for (int i = 2; i <= nargs; i++) {
                switch (lua_type(L, i)) {
                case LUA_TNUMBER:
                    data.emplace_back(lua_tonumber(L, i));
                    break;
                case LUA_TBOOLEAN:
                    data.emplace_back(lua_toboolean(L, i) ? true : false);
                    break;
                case LUA_TSTRING:
                    data.emplace_back(lua_tostring(L, i));
                    break;
                default:
                    continue;
                }
            }
        }

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_LIST_TO, data)))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_any_to(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);
        const auto targ3 = lua_type(L, 3);

        LuaStackGuard sg(L);

        const bool is_list_arg = (nargs == 3 && targ3 == LUA_TTABLE);
        const bool is_tuple_arg = (nargs >= 2 && targ3 != LUA_TTABLE);

        if (!is_list_arg && !is_tuple_arg) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: any_to(outlet, sel, list? or values?)" });
            return 0;
        }

        // target outlet
        LuaInt n = luaL_optinteger(L, 1, 0);

        LuaAtomList data;
        data.reserve(nargs);
        data.emplace_back(n);

        // message selector
        if (!lua_isstring(L, 2)) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "selector should be the string" });
            return 0;
        }
        data.emplace_back(lua_tostring(L, 2));

        // message arguments
        if (is_list_arg) {
            auto tab = get_list_table(L, 3);
            data.insert(data.end(), tab.begin(), tab.end());
        } else if (is_tuple_arg) {
            for (int i = 3; i <= nargs; i++) {
                switch (lua_type(L, i)) {
                case LUA_TNUMBER:
                    data.emplace_back(lua_tonumber(L, i));
                    break;
                case LUA_TBOOLEAN:
                    data.emplace_back(lua_toboolean(L, i) ? true : false);
                    break;
                case LUA_TSTRING:
                    data.emplace_back(lua_tostring(L, i));
                    break;
                default:
                    continue;
                }
            }
        }

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_ANY_TO, data)))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_send_bang(lua_State* L)
    {
        const auto ctx = get_ctx(L);

        LuaStackGuard sg(L);

        if (!lua_isstring(L, 1)) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: send_bang(dest)" });
            check_arg_type(ctx, "dest", LUA_TSTRING, L, 1);
            return 0;
        }

        // destination
        LuaString dest = luaL_optstring(L, 1, "");

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_SEND_BANG, dest)))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_send_float(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);

        LuaStackGuard sg(L);

        if (!(nargs == 2 && lua_isstring(L, 1) && lua_isnumber(L, 2))) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: send_float(dest, val)" });

            check_arg_type(ctx, "dest", LUA_TSTRING, L, 1);
            check_arg_type(ctx, "val", LUA_TNUMBER, L, 2);

            return 0;
        }

        // destination
        auto dest = LuaAtom(luaL_optstring(L, 1, ""));
        auto val = LuaAtom(luaL_optnumber(L, 2, 0));

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_SEND_FLOAT, LuaAtomList { dest, val })))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_send_symbol(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);

        LuaStackGuard sg(L);

        if (!(nargs == 2 && lua_isstring(L, 1) && lua_isstring(L, 2))) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: send_symbol(dest, val)" });

            check_arg_type(ctx, "dest", LUA_TSTRING, L, 1);
            check_arg_type(ctx, "val", LUA_TSTRING, L, 2);

            return 0;
        }

        // destination
        auto dest = LuaAtom(luaL_optstring(L, 1, ""));
        auto val = LuaAtom(luaL_optstring(L, 2, ""));

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_SEND_SYMBOL, LuaAtomList { dest, val })))
            return 0;

        if (!Dispatcher::instance().send({ ctx.id, NOTIFY_UPDATE }))
            return 0;

        return 1;
    }

    int lua_send_list(lua_State* L)
    {
        const auto ctx = get_ctx(L);
        const int nargs = lua_gettop(L);

        LuaStackGuard sg(L);

        if (!(nargs == 2 && lua_isstring(L, 1) && lua_istable(L, 2))) {
            ctx.pipe->try_enqueue({ LUA_CMD_ERROR, "usage: send_symbol(dest, val)" });

            check_arg_type(ctx, "dest", LUA_TSTRING, L, 1);
            check_arg_type(ctx, "val", LUA_TSTRING, L, 2);

            return 0;
        }

        // destination
        auto dest = LuaAtom(luaL_optstring(L, 1, ""));
        auto val = LuaAtom(luaL_optstring(L, 2, ""));

        if (!ctx.pipe->try_enqueue(LuaCmd(LUA_CMD_SEND_SYMBOL, LuaAtomList { dest, val })))
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

    FunctionCall::FunctionCall(lua_State* L, int8_t nargs, const std::string& name)
        : lua_(L)
        , nargs_(nargs)
        , npushed_(0)
        , name_(name)
    {
        if (!lua_checkstack(lua_, nargs + 1)) {
            std::cerr << "no space on stack before calling \"" << name_ << '"' << std::endl;
            lua_ = nullptr;
            return;
        }

        lua_getglobal(L, name_.c_str());
        if (!lua_isfunction(L, -1)) {
            std::cerr << "function '" << name_ << "' is undefined" << std::endl;
            lua_pop(L, 1);
            lua_ = nullptr;
            return;
        }
    }

    bool FunctionCall::checkArgs() const
    {
        if (npushed_ > nargs_) {
            std::cerr << "too many arguments, expected: " << nargs_ << std::endl;
            return false;
        }

        return true;
    }

    bool FunctionCall::isDefined() const
    {
        if (!lua_) {
            std::cerr << "undefined function: " << name_ << std::endl;
            return false;
        }

        return true;
    }

    FunctionCall& FunctionCall::operator<<(bool v)
    {
        if (!isDefined() || !checkArgs())
            return *this;

        lua_pushboolean(lua_, v);
        npushed_++;

        return *this;
    }

    FunctionCall& FunctionCall::operator<<(double v)
    {
        if (!isDefined() || !checkArgs())
            return *this;

        lua_pushnumber(lua_, v);
        npushed_++;

        return *this;
    }

    FunctionCall& FunctionCall::operator<<(int64_t v)
    {
        if (!isDefined() || !checkArgs())
            return *this;

        lua_pushinteger(lua_, v);
        npushed_++;

        return *this;
    }

    FunctionCall& FunctionCall::operator<<(const char* s)
    {
        if (!isDefined() || !checkArgs())
            return *this;

        lua_pushstring(lua_, s);
        npushed_++;

        return *this;
    }

    FunctionCall& FunctionCall::operator<<(const LuaAtom& a)
    {
        if (a.isDouble())
            return this->operator<<(a.getDouble());
        else if (a.isInt())
            return this->operator<<(a.getInt());
        else if (a.isString())
            return this->operator<<(a.getString().c_str());
        else {
            std::cerr << "unsupported LuaAtom type" << std::endl;
            return *this;
        }
    }

    FunctionCall& FunctionCall::pushTable(size_t argc, const LuaAtom* argv)
    {
        if (!isDefined())
            return *this;

        if (argc > MAX_TABLE_LEN) {
            std::cerr << "list is too long: " << argc << ", max allowed length is: " << MAX_TABLE_LEN << std::endl;
            return *this;
        }

        if (!lua_checkstack(lua_, argc)) {
            std::cerr << "no space for " << argc << " element left on the stack" << std::endl;
            return *this;
        }

        // push new table
        lua_createtable(lua_, argc, 0);

        for (size_t i = 0; i < argc; i++) {
            // push insert index
            lua_pushinteger(lua_, i + 1);

            const LuaAtom& a = argv[i];

            if (a.isDouble())
                lua_pushnumber(lua_, a.getDouble());
            else if (a.isInt())
                lua_pushnumber(lua_, a.getInt());
            else if (a.isString())
                lua_pushstring(lua_, a.getString().c_str());
            else {
                // pop index
                lua_pop(lua_, 1);
                continue;
            }

            lua_settable(lua_, -3);
        }

        npushed_++;

        return *this;
    }

    bool FunctionCall::operator()()
    {
        if (!isDefined())
            return false;

        if (npushed_ < nargs_) {
            std::cerr << "not all arguments pushed for function " << name_ << ". " << nargs_ << " expected, got: " << npushed_ << std::endl;
            return false;
        } else if (npushed_ > nargs_) {
            std::cerr << "too many arguments pushed for function " << name_ << ". " << nargs_ << " expected, got: " << npushed_ << std::endl;
            return false;
        }

        if (lua_pcall(lua_, nargs_, 0, 0) != LUA_OK) {
            if (lua_isstring(lua_, 1))
                std::cerr << "error: " << lua_tostring(lua_, -1);

            lua_pop(lua_, 1);
            return false;
        }

        return true;
    }
}
}
