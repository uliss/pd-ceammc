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
#include "lua_cmd.h"

namespace ceammc {
namespace lua {
    LuaCmd::LuaCmd()
        : cmd(LUA_CMD_NOP)
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_)
        : cmd(cmd_)
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, const LuaString& s)
        : cmd(cmd_)
        , args { LuaAtom { s } }
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, LuaString&& s)
        : cmd(cmd_)
        , args { LuaAtom { s } }
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, std::initializer_list<LuaAtom>& a)
        : cmd(cmd_)
        , args { a }
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, const LuaAtomList& args_)
        : cmd(cmd_)
        , args(args_)
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, LuaAtomList&& args_)
        : cmd(cmd_)
        , args(std::move(args_))
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, LuaDouble d)
        : cmd(cmd_)
        , args { LuaAtom { d } }
    {
    }

    LuaCmd::LuaCmd(LuaCmdEnum cmd_, LuaInt i)
        : cmd(cmd_)
        , args { LuaAtom { i } }
    {
    }

    void LuaCommandQueue::pushError(SubscriberId id, const std::string& str)
    {
        using namespace ceammc::lua;

        if (!try_enqueue({ LUA_CMD_ERROR, str }))
            return;

        Dispatcher::instance().send({ id, 0 });
    }

    void LuaCommandQueue::pushLog(SubscriberId id, const std::string& str)
    {
        using namespace ceammc::lua;

        if (!try_enqueue({ LUA_CMD_LOG, str }))
            return;

        Dispatcher::instance().send({ id, 0 });
    }
}
}
