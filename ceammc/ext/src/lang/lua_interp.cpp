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
#include "lua_interp.h"
#include "fmt/core.h"
#include "lua_func.h"
#include "lua_stack_guard.h"

#include "lua.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace ceammc {

namespace lua {

    static void line_hook_fn(lua_State* L, lua_Debug* ar)
    {
        if (ar->event == LUA_HOOKLINE) {
            lua_getglobal(L, "_quit");
            if (lua_islightuserdata(L, -1) != 1)
                return;

            auto pquit = static_cast<bool**>(lua_touserdata(L, -1));
            if (pquit && *pquit && **pquit)
                luaL_error(L, "QUIT");
        }
    }

    LuaInterp::LuaInterp(LuaCommandQueue* pipe, SubscriberId id, const std::atomic_bool* quit)
        : lua_(nullptr)
        , pipe_(pipe)
        , quit_(quit)
        , id_(id)
    {
        /* initialize Lua */
        lua_ = luaL_newstate();
        if (!lua_) {
            LIB_ERR << "can't allocate lua interperter";
            return;
        }

        luaopen_base(lua_);
        luaopen_math(lua_);
        luaopen_string(lua_);
        luaopen_table(lua_);

        luaopen_bit(lua_);
        luaopen_jit(lua_);
        // luaopen_io(lua_);
        // luaopen_os(lua_);
        // luaopen_package(lua_);
        // luaopen_debug(lua_);
        // luaopen_ffi(lua_);

        lua_pushcfunction(lua_, lua_pd_post);
        lua_setglobal(lua_, "pd_post");

        lua_pushcfunction(lua_, lua_pd_error);
        lua_setglobal(lua_, "pd_error");

        // output
        lua_pushcfunction(lua_, lua_bang_to);
        lua_setglobal(lua_, "bang_to");
        lua_pushcfunction(lua_, lua_float_to);
        lua_setglobal(lua_, "float_to");
        lua_pushcfunction(lua_, lua_symbol_to);
        lua_setglobal(lua_, "symbol_to");
        lua_pushcfunction(lua_, lua_list_to);
        lua_setglobal(lua_, "list_to");
        lua_pushcfunction(lua_, lua_any_to);
        lua_setglobal(lua_, "any_to");

        // send
        lua_pushcfunction(lua_, lua_send_bang);
        lua_setglobal(lua_, "send_bang");
        lua_pushcfunction(lua_, lua_send_float);
        lua_setglobal(lua_, "send_float");
        lua_pushcfunction(lua_, lua_send_symbol);
        lua_setglobal(lua_, "send_symbol");
        lua_pushcfunction(lua_, lua_send_list);
        lua_setglobal(lua_, "send_list");

        lua_pushcfunction(lua_, lua_sleep);
        lua_setglobal(lua_, "sleep");

        lua_pushcfunction(lua_, lua_mtof);
        lua_setglobal(lua_, "mtof");

        lua_pushcfunction(lua_, lua_pd_send);
        lua_setglobal(lua_, "pd_send");

        lua_pushcfunction(lua_, lua_stack_dump);
        lua_setglobal(lua_, "stack_dump");

        lua_pushlightuserdata(lua_, pipe);
        lua_setglobal(lua_, "_obj");

        lua_pushinteger(lua_, id);
        lua_setglobal(lua_, "_id");

        // note: pointer to pointer
        // lua_pushlightuserdata(lua_, &quit_);
        // lua_setglobal(lua_, "_quit");

        // Initialisation code
        lua_sethook(lua_, &line_hook_fn, LUA_MASKLINE, 0);

        luaL_dostring(lua_, R"(
-- convert list to string
function str(var, delim)
    if type(var) ~= "table" then
        return tostring(var)
    end

    if delim == nil then delim = " " end
    local res = ""
    for i=1,#var do
    if i ~= 1 then
        res = res .. delim
    end

        res = res .. var[i]
    end

    return res
end
)");
    }

    LuaInterp::~LuaInterp()
    {
        if (lua_)
            lua_close(lua_);
    }

    void LuaInterp::run(const LuaCmd& cmd)
    {
        if (quit_ && *quit_)
            return;

        try {
            lua::LuaStackGuard sg(lua_, true);

            switch (cmd.cmd) {
            case LUA_INTERP_EVAL: {
                if (cmd.args.size() == 1 && cmd.args[0].isString()) {
                    const auto str = cmd.args[0].getString();

                    if (luaL_dostring(lua_, str.c_str()) != LUA_OK) {
                        error(fmt::format("eval error:\n\t{}", lua_tostring(lua_, lua_gettop(lua_))));
                        lua_pop(lua_, lua_gettop(lua_));
                    } else
                        log(fmt::format("eval: '{}'", str));
                }
            } break;
            case LUA_INTERP_LOAD: {
                if (cmd.args.size() == 1 && cmd.args[0].isString()) {
                    const auto str = cmd.args[0].getString();

                    if (luaL_dofile(lua_, str.c_str()) != LUA_OK) {
                        error(fmt::format("eval error:\n\t{}", lua_tostring(lua_, lua_gettop(lua_))));
                        lua_pop(lua_, lua_gettop(lua_));
                    } else
                        log(fmt::format("load file: '{}'", str));
                } else
                    error("invalid arguments: path expected");
            } break;
            case LUA_INTERP_BANG: {
                if (cmd.args.size() == 1 && cmd.args[0].isInt()) {
                    FunctionCall fn(lua_, 1, "on_bang");
                    if (!fn)
                        return;

                    fn << cmd.args[0];

                    fn();
                }
            } break;
            case LUA_INTERP_FLOAT: {
                // args: INLET_IDX FLOAT
                if (cmd.args.size() == 2 && cmd.args[0].isInt() && cmd.args[1].isDouble()) {
                    FunctionCall fn(lua_, 2, "on_float");
                    if (!fn)
                        return;

                    fn << cmd.args[0];
                    fn << cmd.args[1];

                    fn();
                }
            } break;
            case LUA_INTERP_SYMBOL: {
                // args: INLET_IDX SYMBOL
                if (cmd.args.size() == 2 && cmd.args[0].isInt() && cmd.args[1].isString()) {
                    FunctionCall fn(lua_, 2, "on_symbol");
                    if (!fn)
                        return;

                    fn << cmd.args[0];
                    fn << cmd.args[1];

                    fn();
                }
            } break;
            case LUA_INTERP_LIST: {
                // args: INLET_IDX VALUES?...
                if (cmd.args.size() > 1 && cmd.args[0].isInt()) {
                    FunctionCall fn(lua_, 2, "on_list");
                    if (!fn)
                        return;

                    fn << cmd.args[0];
                    fn.pushTable(cmd.args.size() - 1, cmd.args.data() + 1);

                    fn();
                }
            } break;
            case LUA_INTERP_ANY: {
                // args: SELECTOR VALUES?...
                if (cmd.args.size() >= 1 && cmd.args[0].isString()) {
                    FunctionCall fn(lua_, 2, "on_any");
                    if (!fn)
                        return;

                    fn << cmd.args[0];
                    fn.pushTable(cmd.args.size() - 1, cmd.args.data() + 1);

                    fn();
                } else {
                }
            } break;
            case LUA_INTERP_CALL: {
                // args: METHOD ARGS?...
                if (cmd.args.size() >= 1 && cmd.args[0].isString()) {
                    FunctionCall fn(lua_, cmd.args.size() - 1, cmd.args[0].getString());
                    if (!fn)
                        return;

                    for (size_t i = 1; i < cmd.args.size(); i++)
                        fn << cmd.args[i];

                    fn();
                } else {
                }
            } break;
            case LUA_INTERP_EVAL_BEGIN: {
                eval_string_.clear();
            } break;
            case LUA_INTERP_EVAL_APPEND: {
                if (cmd.args.size() == 1 && cmd.args[0].isString()) {
                    auto str = cmd.args[0].getString();
                    boost::replace_all(str, "\\x09", "\t");
                    boost::replace_all(str, "\\x2c", ",");
                    boost::replace_all(str, "\\x7b", "{");
                    boost::replace_all(str, "\\x7d", "}");
                    boost::replace_all(str, "\\x3b", ";");
                    boost::replace_all(str, "\\x5c", "\\");

                    eval_string_.append(str);
                    eval_string_.append("\n");
                }
            } break;
            case LUA_INTERP_EVAL_END: {
                if (!eval_string_.empty()) {
                    if (luaL_dostring(lua_, eval_string_.c_str()) != LUA_OK) {
                        error(fmt::format("source error:\n\t{}", lua_tostring(lua_, lua_gettop(lua_))));
                        lua_pop(lua_, lua_gettop(lua_));
                    }
                } else
                    error("empty eval string");
            } break;
            case LUA_CMD_NOP: // ignore silently
                break;
            default:
                error(fmt::format("unsupported command: {}", (int)cmd.cmd));
                break;
            }
        } catch (std::exception& e) {
            error(fmt::format("exception: {}", e.what()));
        }
    }

    void LuaInterp::error(const std::string& str)
    {
        std::cerr << str << std::endl;
        pipe_->pushError(id_, str);
    }

    void LuaInterp::log(const std::string& str)
    {
        std::cerr << "[debug] " << str << std::endl;
        pipe_->pushLog(id_, str);
    }
}
}
