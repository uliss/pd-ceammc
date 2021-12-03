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
#ifndef LUA_INTERP_H
#define LUA_INTERP_H

#include "lua_cmd.h"
#include "pollthread_object.h"

extern "C" {
#include "lua.h"
}

#include <string>

namespace ceammc {

namespace lua {
    using LuaPipe = PollThreadQueue<LuaCmd>;

    class LuaInterp {
    private:
        lua_State* lua_;
        LuaPipe* pipe_ { nullptr };
        const bool* quit_ { nullptr };
        SubscriberId id_ { 0 };

        LuaInterp(const LuaInterp&)
            = delete;
        LuaInterp& operator=(const LuaInterp&) = delete;

    public:
        LuaInterp(LuaPipe* pipe, SubscriberId id, const bool* quit);
        virtual ~LuaInterp();

        void run(const LuaCmd& cmd);
    };

}
}

#endif // LUA_INTERP_H
