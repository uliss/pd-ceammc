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
#include "lua_func.h"

#include "lua.hpp"

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

    LuaInterp::LuaInterp(LuaPipe* pipe, SubscriberId id, const bool* quit)
        : lua_(nullptr)
        , pipe_(pipe)
        , quit_(quit)
        , id_(id)
    {
        /* initialize Lua */
        lua_ = luaL_newstate();

        //        luaopen_base(L);
        //        luaopen_io(L);
        //        luaopen_string(L);
        //        luaopen_utf8(L);
        //        luaopen_table(L);
        //        luaopen_math(L);
        //        luaopen_bit32(L);
        luaL_openlibs(lua_);

        lua_pushcfunction(lua_, lua_pd_post);
        lua_setglobal(lua_, "pd_post");

        lua_pushcfunction(lua_, lua_pd_error);
        lua_setglobal(lua_, "pd_error");

        lua_pushcfunction(lua_, lua_output);
        lua_setglobal(lua_, "output_float");

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
        lua_pushlightuserdata(lua_, &quit_);
        lua_setglobal(lua_, "_quit");

        // Initialisation code
        lua_sethook(lua_, &line_hook_fn, LUA_MASKLINE, 0);
    }

    LuaInterp::~LuaInterp()
    {
        lua_close(lua_);
    }

    void LuaInterp::run(const LuaCmd& cmd)
    {
        if (quit_ && *quit_)
            return;

        switch (cmd.cmd) {
        case LUA_INTERP_EVAL: {
            if (cmd.args.size() == 1 && cmd.args[0].type() == typeid(LuaString)) {
                const auto str = boost::get<std::string>(cmd.args[0]);
                std::cerr << "[lua] eval " << str << "\n";

                if (luaL_dostring(lua_, str.c_str()) != LUA_OK) {
                    std::cerr << "[lua] " << lua_tostring(lua_, lua_gettop(lua_)) << "\n";
                    lua_pop(lua_, lua_gettop(lua_));
                }
            }
        } break;
        case LUA_INTERP_LOAD: {
            if (cmd.args.size() == 1 && cmd.args[0].type() == typeid(LuaString)) {
                const auto str = boost::get<std::string>(cmd.args[0]);
                std::cerr << "[lua] load " << str << "\n";

                if (luaL_dofile(lua_, str.c_str()) != LUA_OK) {
                    std::cerr << "[lua] " << lua_tostring(lua_, lua_gettop(lua_)) << "\n";
                    lua_pop(lua_, lua_gettop(lua_));
                }
            } else
                std::cerr << "invalid arguments: path expected";
        } break;
        case LUA_CMD_NOP: // ignore silently
            break;
        default:
            std::cerr << "unsupported command: " << cmd.cmd << "\n";
            break;
        }
    }
}
}
