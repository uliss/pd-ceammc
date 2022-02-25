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
#ifndef CEAMMC_EDITOR_OBJECT_H
#define CEAMMC_EDITOR_OBJECT_H

#include "ceammc_object.h"

namespace ceammc {

class EditorObjectImpl {
    t_object* owner_;
    void* guiconnect_;
    const char* name_;
    int w_, h_;

public:
    enum SyncMode {
        SYNC_YES,
        SYNC_NO
    };

public:
    EditorObjectImpl(t_object* owner, const char* name = "DATA", int w = 200, int h = 100);

    SyncMode click(t_canvas* cnv);
    void close(t_symbol* sel, const AtomListView& lv);
    void update(const AtomListView& lv);

private:
    unsigned long xowner() const { return reinterpret_cast<unsigned long>(owner_); }
};

template <typename BaseClass>
class EditorObject : public BaseClass {
    EditorObjectImpl impl_;

public:
    EditorObject(const PdArgs& args, const char* name = "DATA", int w = 200, int h = 100)
        : BaseClass(args)
        , impl_(this->owner(), name, w, h)
    {
    }

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override
    {
        if (impl_.click(this->canvas()) == EditorObjectImpl::SYNC_YES)
            this->editorSyncData();
    }

    virtual void editorAddLine(t_symbol* sel, const AtomListView& lv) = 0;
    virtual AtomListView getContentForEditor() const = 0;

    void m_close(t_symbol* s, const AtomListView& lv) { impl_.close(s, lv); }
    void m_addline(t_symbol* s, const AtomListView& lv) { this->editorAddLine(s, lv); }
    void m_notify(t_symbol*, const AtomListView&) { this->editorSyncData(); }

private:
    void editorSyncData()
    {
        impl_.update(this->getContentForEditor());
    }

public:
    using ThisType = EditorObject<BaseClass>;

    template <typename Factory>
    static void registerMethods(Factory& obj)
    {
        obj.useClick();
        obj.addMethod("close", &ThisType::m_close);
        obj.addMethod("addline", &ThisType::m_addline);
        obj.addMethod("notify", &ThisType::m_notify);
    }
};

}

#endif // CEAMMC_EDITOR_OBJECT_H
