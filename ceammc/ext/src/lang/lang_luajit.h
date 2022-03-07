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

#include "ceammc_containers.h"
#include "ceammc_editor_object.h"
#include "lua_cmd.h"
#include "lua_interp.h"
#include "poll_dispatcher.h"
#include "pollthread_object.h"

#include "readerwriterqueue.h"

#include <boost/container/small_vector.hpp>

using namespace ceammc;

using LuaCommandQueue = moodycamel::ReaderWriterQueue<lua::LuaCmd>;

using LangLuaBase = EditorObject<PollThreadQueueObject<lua::LuaCmd>>;

class LangLuaJit : public LangLuaBase {
    lua::LuaInterp interp_;
    using FixedAtomList = SmallAtomListN<8>;
    using FixedEditorList = boost::container::small_vector<FixedAtomList, 48>;
    FixedEditorList src_;

public:
    LangLuaJit(const PdArgs& args);
    ~LangLuaJit() override;

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* sel, const AtomListView& lv) override;

    void dump() const override;

    Future createTask() override;
    void processMessage(const lua::LuaCmd& msg) override;

    void m_load(t_symbol* s, const AtomListView& lv);
    void m_eval(t_symbol* s, const AtomListView& lv);
    void m_call(t_symbol* s, const AtomListView& lv);
    void m_restore(t_symbol* s, const AtomListView& lv);

    void onSave(t_binbuf* b);

public:
    void editorClear() final;
    void editorAddLine(t_symbol* sel, const AtomListView& lv) final;
    EditorLineList getContentForEditor() const final;
    EditorTitleString editorTitle() const final { return "Lua"; }
    void editorSync() final;

public:
    lua::LuaInterp& interp() { return interp_; }

private:
    void updateInterpSource();
};

void setup_lang_luajit();

#endif // LANG_LUAJIT_H
