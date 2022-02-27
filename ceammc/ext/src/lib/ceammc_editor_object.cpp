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

void EditorObjectImpl::open(t_canvas* cnv, const AtomListView& data)
{
    if (guiconnect_) {
        sys_vgui("ceammc::texteditor::show .x%lx\n", xowner());
    } else {
        const auto z = glist_getzoom(cnv);
        const auto fsz = sys_hostfontsize(glist_getfont(cnv), z);
        const auto w = w_ * z;
        const auto h = h_ * z;

        sys_vgui("ceammc::texteditor::open .x%lx %dx%d {%s} %d\n",
            xowner(), w, h, name_, fsz);

        char buf[40];
        sprintf(buf, ".x%lx", xowner());
        guiconnect_ = guiconnect_new(&owner_->te_g.g_pd, gensym(buf));

        update(data);
    }
}

void EditorObjectImpl::close(t_symbol* s, const AtomListView& lv)
{
    sys_vgui("ceammc::texteditor::doclose .x%lx\n", xowner());

    if (guiconnect_) {
        guiconnect_notarget((t_guiconnect*)guiconnect_, 1000);
        guiconnect_ = nullptr;
    }
}

void EditorObjectImpl::update(const AtomListView& lv)
{
    sys_vgui("ceammc::texteditor::clear .x%lx\n", xowner());

    for (auto& a : lv) {
        switch (a.type()) {
        case Atom::SYMBOL: {
            auto s = a.asT<t_symbol*>();
            sys_vgui("ceammc::texteditor::append .x%lx {%s\n}\n", xowner(), s->s_name);
        } break;
        case Atom::FLOAT: {
            char buf[32];
            atom_string(&a.atom(), buf, sizeof(buf) - 1);
            sys_vgui("ceammc::texteditor::append .x%lx {%s\n}\n", xowner(), buf);
        } break;
        default:
            continue;
        }
    }

    sys_vgui("ceammc::texteditor::highlight .x%lx\n", xowner());
    sys_vgui("ceammc::texteditor::setdirty .x%lx 0\n", xowner());
}

}
