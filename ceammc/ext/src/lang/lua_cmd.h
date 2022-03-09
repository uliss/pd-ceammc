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

#include "m_pd.h"

#include <boost/variant.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace ceammc {
namespace lua {
    enum LuaCmdEnum {
        LUA_CMD_NOP = 0,
        LUA_CMD_ERROR,
        LUA_CMD_POST,
        LUA_CMD_LOG,
        LUA_CMD_BANG_TO,
        LUA_CMD_FLOAT_TO,
        LUA_CMD_SYMBOL_TO,
        LUA_CMD_LIST_TO,
        LUA_CMD_ANY_TO,
        LUA_CMD_SEND,
        LUA_CMD_SEND_BANG,
        LUA_CMD_SEND_FLOAT,
        LUA_CMD_SEND_SYMBOL,
        LUA_CMD_SEND_LIST,
        LUA_CMD_SEND_ANY,
        LUA_INTERP_EVAL,
        LUA_INTERP_LOAD,
        LUA_INTERP_BANG,
        LUA_INTERP_FLOAT,
        LUA_INTERP_SYMBOL,
        LUA_INTERP_LIST,
        LUA_INTERP_ANY,
        LUA_INTERP_CALL,
        LUA_INTERP_PROPERTY,
        LUA_INTERP_EVAL_BEGIN,
        LUA_INTERP_EVAL_APPEND,
        LUA_INTERP_EVAL_END
    };

    using LuaInt = int64_t;
    using LuaDouble = double;
    using LuaString = std::string;

    class LuaAtom {
        boost::variant<LuaInt, LuaDouble, LuaString> val_;

    public:
        LuaAtom()
            : val_(LuaInt(0))
        {
        }

        explicit LuaAtom(bool b)
            : val_(LuaInt(b ? 1 : 0))
        {
        }

        explicit LuaAtom(LuaInt i)
            : val_(i)
        {
        }

        explicit LuaAtom(LuaDouble d)
            : val_(d)
        {
        }

        explicit LuaAtom(const LuaString& str)
            : val_(str)
        {
        }

        explicit LuaAtom(const char* str)
            : val_(str)
        {
        }

        explicit LuaAtom(t_symbol* s)
            : val_(s->s_name)
        {
        }

        bool isInt() const { return val_.which() == 0; }
        bool isDouble() const { return val_.which() == 1; }
        bool isString() const { return val_.which() == 2; }

        LuaInt getInt() const { return boost::get<LuaInt>(val_); }
        LuaDouble getDouble() const { return boost::get<LuaDouble>(val_); }
        const LuaString& getString() const { return boost::get<LuaString>(val_); }

        template <typename V>
        typename V::result_type applyVisitor() const { return boost::apply_visitor(V(), val_); }
    };

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
