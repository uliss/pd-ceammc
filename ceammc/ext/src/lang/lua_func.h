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
#include "lua_cmd.h"

#include <cstdint>
#include <string>

namespace ceammc {
namespace lua {
    int lua_mtof(lua_State* L);
    int lua_pd_error(lua_State* L);
    int lua_pd_post(lua_State* L);
    int lua_pd_send(lua_State* L);
    int lua_stack_dump(lua_State* L);
    int lua_sleep(lua_State* L);
    // output
    int lua_any_to(lua_State* L);
    int lua_bang_to(lua_State* L);
    int lua_float_to(lua_State* L);
    int lua_symbol_to(lua_State* L);
    int lua_list_to(lua_State* L);
    // send
    int lua_send_bang(lua_State* L);
    int lua_send_float(lua_State* L);
    int lua_send_symbol(lua_State* L);

    class FunctionCall {
        lua_State* lua_;
        const int16_t nargs_;
        int16_t npushed_;
        std::string name_;

    public:
        FunctionCall(lua_State* L, int8_t nargs, const std::string& name);

        FunctionCall& operator<<(bool v);
        FunctionCall& operator<<(float v) { return this->operator<<(double(v)); }
        FunctionCall& operator<<(double v);
        FunctionCall& operator<<(int32_t v) { return this->operator<<(int64_t(v)); }
        FunctionCall& operator<<(int64_t v);
        FunctionCall& operator<<(const char* s);
        FunctionCall& operator<<(const std::string& s) { return this->operator<<(s.c_str()); }
        FunctionCall& operator<<(const LuaAtom& a);

        FunctionCall& pushTable(size_t argc, const LuaAtom* argv);

        operator bool() const { return lua_ != nullptr; }
        bool operator()();

    private:
        bool checkArgs() const;
        bool isDefined() const;
    };
}
}

#endif // LUA_FUNC_H
