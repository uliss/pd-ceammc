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
#ifndef LUA_CMD_H
#define LUA_CMD_H

#include <boost/variant.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace ceammc {
namespace lua {
    enum LuaCmdEnum {
        LUA_CMD_NOP = 0,
        LUA_CMD_ERROR,
        LUA_CMD_OUTPUT,
        LUA_CMD_POST,
        LUA_CMD_SEND,
        LUA_INTERP_EVAL,
        LUA_INTERP_LOAD
    };

    using LuaInt = int64_t;
    using LuaDouble = double;
    using LuaString = std::string;
    using LuaAtom = boost::variant<LuaInt, LuaDouble, LuaString>;

    using LuaAtomList = std::vector<LuaAtom>;

    class LuaCmd {
    public:
        LuaCmd();
        LuaCmd(LuaCmdEnum cmd_);
        LuaCmd(LuaCmdEnum cmd_, LuaDouble d);
        LuaCmd(LuaCmdEnum cmd_, LuaInt i);
        LuaCmd(LuaCmdEnum cmd_, LuaString&& s);
        LuaCmd(LuaCmdEnum cmd_, const LuaString& s);
        LuaCmd(LuaCmdEnum cmd_, std::initializer_list<LuaAtom>& a);
        LuaCmd(LuaCmdEnum cmd_, const LuaAtomList& args);
        LuaCmd(LuaCmdEnum cmd_, LuaAtomList&& args);

        void appendArg(LuaAtom&& a)
        {
            args.push_back(a);
        }

        LuaCmdEnum cmd { LUA_CMD_NOP };
        LuaAtomList args;
    };
}
}

#endif // LUA_CMD_H
