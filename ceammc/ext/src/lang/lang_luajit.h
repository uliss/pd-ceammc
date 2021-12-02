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
#ifndef LANG_LUAJIT_H
#define LANG_LUAJIT_H

#include <future>

#include "lua_cmd.h"
#include "poll_dispatcher.h"
#include "pollthread_object.h"

using namespace ceammc;

class LangLuaJit : public ceammc::PollThreadQueueObject<lua::LuaCmd> {
    lua::LuaCmd interp_cmd_;

public:
    LangLuaJit(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onList(const AtomList& lst) override;

    Future createTask() override;
    void processMessage(const lua::LuaCmd& msg) override;

    void m_file(t_symbol* s, const AtomListView& lv);

    //    t_guiconnect* guiconnect;
    //    std::vector<std::string> lines_;
};

void setup_lang_luajit();

#endif // LANG_LUAJIT_H
