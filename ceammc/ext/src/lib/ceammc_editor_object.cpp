/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "ceammc_editor_object.h"
#include "ceammc_canvas.h"
#include "fmt/core.h"

extern "C" {
#include "g_canvas.h"
}

namespace ceammc {

namespace {
    const char* editorSyntaxStr(EditorSyntax s)
    {
        switch (s) {
        case EditorSyntax::SELECTOR:
            return "selector";
        case EditorSyntax::DEFAULT:
            return "default";
        case EditorSyntax::LUA:
            return "lua";
        case EditorSyntax::NONE:
        default:
            return "none";
        }
    }

    const char* escapeMode(EditorEscapeMode m)
    {
        switch (m) {
        case EditorEscapeMode::LUA:
            return "lua";
        case EditorEscapeMode::DATA:
            return "data";
        case EditorEscapeMode::DEFAULT:
        default:
            return "default";
        }
    }
}

void EditorString::append(char ch)
{
    try {
        str.push_back(ch);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
    }
}

void EditorString::append(t_float t)
{
    try {
        fmt::format_to(std::back_inserter(str), "{:g}", t);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
    }
}

void EditorString::append(const char* txt)
{
    try {
        str.append(txt);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
    }
}

void EditorString::append(const char* txt, size_t len)
{
    try {
        str.append(txt, len);
    } catch (std::exception& e) {
        LIB_ERR << e.what();
    }
}

void EditorString::append(const Atom& a)
{
    if (a.isFloat())
        return append(a.asT<t_float>());
    else if (a.isSymbol())
        return append(a.asT<t_symbol*>());
    else
        LIB_ERR << "unsupported atom type: " << a.type();
}

void EditorString::append(const AtomListView& lv, const char* delim)
{
    for (size_t i = 0; i < lv.size(); i++) {
        if (i > 0)
            append(delim);

        append(lv[i]);
    }
}

void EditorString::appendQuoted(const char* txt)
{
    try {

        str.append(1, '"');
        const auto N = strlen(txt);
        for (size_t i = 0; i < N; i++) {
            auto c = txt[i];
            if (c == '"') {
                str.append(1, '`');
                str.append(1, '"');
            } else if (c == '`') {
                str.append(2, '`');
            } else
                str.append(1, c);
        }

        str.append(1, '"');
    } catch (std::exception& e) {
        LIB_ERR << e.what();
    }
}

void EditorString::appendQuoted(const Atom& a)
{
    if (a.isFloat())
        return append(a.asT<t_float>());
    else if (a.isSymbol())
        return appendQuoted(a.asT<t_symbol*>()->s_name);
}

void EditorString::appendQuoted(const AtomListView& lv, const char* delim)
{
    for (size_t i = 0; i < lv.size(); i++) {
        if (i > 0)
            append(delim);

        appendQuoted(lv[i]);
    }
}

void EditorString::pop()
{
    if (str.size() > 0)
        str.pop_back();
}

void EditorString::trim()
{
    while (str.size() > 0 && str.back() == ' ')
        str.pop_back();
}

EditorObjectImpl::EditorObjectImpl(t_object* owner, EditorEscapeMode mode)
    : owner_(owner)
    , guiconnect_(nullptr)
    , esc_mode_(mode)
{
}

EditorObjectImpl::~EditorObjectImpl()
{
    if (guiconnect_) {
        sys_vgui("destroy .x%lx\n", xowner());
        guiconnect_notarget((t_guiconnect*)guiconnect_, 1000);
        guiconnect_ = nullptr;
    }
}

void EditorObjectImpl::open(t_canvas* cnv, const EditorLineList& data, const EditorTitleString& title, int x, int y, int nchars, int nlines, bool lineNumbers, EditorSyntax syntax)
{
    if (guiconnect_) {
        sys_vgui("ceammc::texteditor::show .x%lx\n", xowner());
    } else {
        const auto z = glist_getzoom(cnv);
        const auto ft = glist_getfont(cnv);
        const auto fsz = sys_hostfontsize(ft, z);
        const auto brect = canvas_info_rect(canvas_getrootfor(cnv));

        const auto w = std::min(800, sys_zoomfontwidth(ft, z, 0) * nchars);
        const auto h = std::min(600, sys_zoomfontheight(fsz, z, 0) * nlines);

        sys_vgui("ceammc::texteditor::open .x%lx %dx%d+%d+%d {%s} %d %d %s\n",
            xowner(), w, h, brect.x + x, brect.y + y, title.c_str(), fsz, (int)lineNumbers, editorSyntaxStr(syntax));

        sys_vgui("ceammc::texteditor::set_escape .x%lx %s\n", xowner(), escapeMode(esc_mode_));

        char buf[40];
        sprintf(buf, ".x%lx", xowner());
        guiconnect_ = guiconnect_new(&owner_->te_g.g_pd, gensym(buf));

        sync(data);
    }
}

void EditorObjectImpl::close()
{
    sys_vgui("ceammc::texteditor::doclose .x%lx\n", xowner());

    if (guiconnect_) {
        guiconnect_notarget((t_guiconnect*)guiconnect_, 1000);
        guiconnect_ = nullptr;
    }

#ifndef NDEBUG
    EditorStringPool::dumpMemoryUsage();
#endif
}

void EditorObjectImpl::sync(const EditorLineList& list)
{
    sys_vgui("ceammc::texteditor::setundo .x%lx 0\n", xowner());
    sys_vgui("ceammc::texteditor::clear .x%lx\n", xowner());

    for (auto& str : list)
        sys_vgui("ceammc::texteditor::append .x%lx {%s\n}\n", xowner(), str->str.c_str());

    sys_vgui("ceammc::texteditor::highlight .x%lx\n", xowner());
    sys_vgui("ceammc::texteditor::setdirty .x%lx 0\n", xowner());
    sys_vgui("ceammc::texteditor::setundo .x%lx 1\n", xowner());
}

void EditorObjectImpl::setSpecialSymbolEscape(EditorEscapeMode mode)
{
    esc_mode_ = mode;
}

void EditorObjectImpl::setDirty(t_canvas* c, bool value)
{
    canvas_dirty(c, value ? 1 : 0);
}

EditorStringPool::Pool& EditorStringPool::pool()
{
    static Pool instance_(64, 64, 0, memorypool::RECYCLE_METHOD_DESTROY_FUNCTION);
    return instance_;
}

void EditorStringPool::dumpMemoryUsage()
{
    LIB_DBG << "editor string pool memory usage:\n"
               "\tsize="
            << pool().max_size() << "\n"
                                    "\tinuse="
            << pool().inuse_count() << "\n"
                                       "\tunused="
            << pool().unused_count();
}

EditorTitleString makeEditorTitleString(const char* dataName, const char* dataId)
{
    EditorTitleString res;
    const bool with_id = dataId && dataId[0];

    try {
        if (with_id)
            fmt::format_to(std::back_inserter(res), "{} ({})", dataName, dataId);
        else
            res.append(dataName);

    } catch (std::exception& e) {
        if (res.size() >= (res.max_size() - 1)) {
            res.resize(res.max_size() - 5);
            res.append("...");
            if (with_id)
                res.push_back(')');
        }
    }

    return res;
}

}
