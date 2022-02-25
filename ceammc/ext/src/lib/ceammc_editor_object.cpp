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

#include <boost/static_string/static_string.hpp>

extern "C" {
#include "g_canvas.h"
}

namespace ceammc {

EditorObjectImpl::EditorObjectImpl(t_object* owner, const char* name, int w, int h)
    : owner_(owner)
    , guiconnect_(nullptr)
    , name_(name)
    , w_(w)
    , h_(h)
{
}

EditorObjectImpl::SyncMode EditorObjectImpl::click(t_canvas* cnv)
{
    if (guiconnect_) {
        sys_vgui("wm deiconify .x%lx\n", xowner());
        sys_vgui("raise .x%lx\n", xowner());
        sys_vgui("focus .x%lx.text\n", xowner());
        return SYNC_NO;
    } else {
        char buf[40];
        sys_vgui("pdtk_textwindow_open .x%lx %dx%d {%s} %d\n",
            xowner(), w_, h_, name_,
            sys_hostfontsize(glist_getfont(cnv),
                glist_getzoom(cnv)));

        sprintf(buf, ".x%lx", xowner());
        guiconnect_ = guiconnect_new(&owner_->te_g.g_pd, gensym(buf));
        return SYNC_YES;
    }
}

void EditorObjectImpl::close(t_symbol* s, const AtomListView& lv)
{
    sys_vgui("pdtk_textwindow_doclose .x%lx\n", xowner());

    if (guiconnect_) {
        guiconnect_notarget((t_guiconnect*)guiconnect_, 1000);
        guiconnect_ = nullptr;
    }
}

void EditorObjectImpl::update(const AtomListView& lv)
{
    constexpr size_t MAXSTR_LEN = 80;
    boost::static_string<256> buf;

    sys_vgui("pdtk_textwindow_clear .x%lx\n", xowner());
    for (auto& a : lv) {
        switch (a.type()) {
        case Atom::SYMBOL: {
            auto s = a.asT<t_symbol*>();
            const auto len = strlen(s->s_name);

            if (len >= MAXSTR_LEN) {
                sys_vgui("pdtk_textwindow_append .x%lx {%s\n}\n", xowner(), s->s_name);
                buf.clear();
            } else {
                buf.append(s->s_name);
                buf.push_back(' ');
                if (buf.length() >= MAXSTR_LEN) {
                    sys_vgui("pdtk_textwindow_append .x%lx {%s\n}\n", xowner(), buf.c_str());
                    buf.clear();
                }
            }
        } break;
        case Atom::FLOAT: {
            char buf2[32];
            atom_string(&a.atom(), buf2, sizeof(buf2) - 1);
            buf.append(buf2);
            buf.push_back(' ');
            if (buf.length() >= MAXSTR_LEN) {
                sys_vgui("pdtk_textwindow_append .x%lx {%s\n}\n", xowner(), buf.c_str());
                buf.clear();
            }
        } break;
        default:
            continue;
        }
    }

    if (!buf.empty()) {
        sys_vgui("pdtk_textwindow_append .x%lx {%s\n}\n", xowner(), buf.c_str());
    }

    sys_vgui("pdtk_textwindow_setdirty .x%lx 0\n", xowner());
}

}
