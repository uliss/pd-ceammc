/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "ceammc_table_editor.h"
#include "ceammc_canvas.h"
#include "ceammc_format.h"
#include "fmt/core.h"

extern "C" {
#include "g_canvas.h"
}

using namespace ceammc;

TableObjectImpl::TableObjectImpl(t_object* owner)
    : owner_(owner)
    , guiconnect_(nullptr)
{
}

TableObjectImpl::~TableObjectImpl()
{
    if (guiconnect_) {
        sys_vgui("destroy .x%lx\n", xowner());
        guiconnect_notarget((t_guiconnect*)guiconnect_, 1000);
        guiconnect_ = nullptr;
    }
}

void TableObjectImpl::open(t_canvas* cnv, const TableLineList& data, const EditorTitleString& title, int x, int y, int nchars, int nlines, bool lineNumbers)
{
    if (guiconnect_) {
        sys_vgui("ceammc::tableeditor::show .x%lx\n", xowner());
    } else {
        const auto root = canvas_root(cnv);
        const auto z = canvas_info_zoom(root);
        const auto ft = canvas_info_font(root);
        const auto fsz = sys_hostfontsize(ft, z);
        const auto brect = canvas_info_rect(canvas_root(cnv));

        //        const auto w = std::min(800, sys_zoomfontwidth(ft, z, 0) * nchars);
        const auto w = 400;
        const auto h = std::min(600, sys_zoomfontheight(fsz, z, 0) * nlines);

        char buf[MAXPDSTRING];
        auto pbuf = fmt::format_to(buf, "{}x{}+{}+{} {{{}}} {} {:d}",
            w, h, brect.x + x, brect.y + y,
            title.c_str(), fsz, lineNumbers);
        *pbuf = '\0';
        sys_vgui("ceammc::tableeditor::open .x%lx %s\n", xowner(), buf);

        sprintf(buf, ".x%lx", xowner());
        guiconnect_ = guiconnect_new(&owner_->te_g.g_pd, gensym(buf));

        sync(data);
    }
}

void TableObjectImpl::close()
{
    sys_vgui("ceammc::tableeditor::doclose .x%lx\n", xowner());

    if (guiconnect_) {
        guiconnect_notarget((t_guiconnect*)guiconnect_, 1000);
        guiconnect_ = nullptr;
    }

#ifndef NDEBUG
    EditorStringPool::dumpMemoryUsage();
#endif
}

void TableObjectImpl::sync(const TableLineList& lines)
{
    sys_vgui("ceammc::tableeditor::clear .x%lx\n", xowner());

    for (auto& ln : lines)
        sys_vgui("ceammc::tableeditor::append .x%lx [list %s]\n", xowner(), ceammc::to_string(ln).c_str());

    sys_vgui("ceammc::tableeditor::setdirty .x%lx 0\n", xowner());
}

void TableObjectImpl::setDirty(t_canvas* c, bool value)
{
    canvas_mark_dirty(c, value);
}
