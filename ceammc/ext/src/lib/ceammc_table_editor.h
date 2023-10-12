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
#ifndef CEAMMC_TABLE_EDITOR_H
#define CEAMMC_TABLE_EDITOR_H

#include "ceammc_editor_object.h"

#include <cstdint>

namespace ceammc {

using TableLineList = std::vector<AtomList>;

enum TableColumnAlign : std::int8_t {
    TABLE_COL_ALIGN_LEFT,
    TABLE_COL_ALIGN_RIGHT,
    TABLE_COL_ALIGN_CENTER,
};

enum TableColumnEditor : std::int8_t {
    TABLE_COL_EDIT_ENTRY,
    TABLE_COL_EDIT_SPINBOX,
    TABLE_COL_EDIT_COMBOBOX,
    TABLE_COL_EDIT_CHECKBOX,
    TABLE_COL_EDIT_MENU,
};

struct TableColumnParam {
    std::string name;
    int width;
    bool readonly;
    TableColumnAlign align { TABLE_COL_ALIGN_LEFT };
    TableColumnEditor edit { TABLE_COL_EDIT_ENTRY };

    TableColumnParam(const std::string& name_, bool readOnly, int width_ = 0)
        : name(name_)
        , readonly(readOnly)
        , width(width_)
    {
    }

    TableColumnParam& setAlign(TableColumnAlign a)
    {
        align = a;
        return *this;
    }

    TableColumnParam& alignRight()
    {
        return setAlign(TABLE_COL_ALIGN_RIGHT);
    }

    TableColumnParam& setEdit(TableColumnEditor e)
    {
        edit = e;
        return *this;
    }
};

class TableObjectImpl {
    t_object* owner_;
    void* guiconnect_;

public:
    TableObjectImpl(t_object* owner);
    ~TableObjectImpl();

    /**
     * create and open table editor TCL window
     * @param cnv - pointer to onwer canvas
     * @param data - editor content
     * @param x - x screen coord
     * @param y - y screen coord
     * @param w - editor window width
     * @param h - editor window height
     * @param cols - column titles
     * @return true on success, false on error
     */
    void open(t_canvas* cnv, const TableLineList& data,
        const EditorTitleString& title, int x, int y, int w, int h, const std::vector<TableColumnParam>& cols);

    /**
     * close TCL editor and stop GUI listening
     */
    void close();

    /**
     * Sync editor with specified data
     * @param lv
     */
    void sync(const TableLineList& lines);

    /**
     * set owner canvas dirty
     */
    void setDirty(t_canvas* c, bool value);

private:
    unsigned long xowner() const { return reinterpret_cast<std::uintptr_t>(owner_); }
};

template <typename T>
class TableObject : public T {
public:
    using TableObjectT = EditorObject<T>;

private:
    TableObjectImpl impl_;
    std::vector<TableColumnParam> cols_;
    int w_ { 400 }, h_ { 300 };

public:
    explicit TableObject(const PdArgs& args)
        : T(args)
        , impl_(this->owner())
    {
    }

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override
    {
        openEditor(xpos, ypos);
    }

    void openEditor(int x, int y)
    {
        impl_.open(this->canvas(),
            this->getTableContentForEditor(),
            this->editorTitle(),
            x, y, w_, h_, cols_);
    }

    virtual void tableEditorClear() = 0;
    virtual void tableEditorAddLine(t_symbol* sel, const AtomListView& lv) = 0;
    virtual void tableEditorSync() { impl_.sync(this->getTableContentForEditor()); }
    virtual TableLineList getTableContentForEditor() const = 0;
    virtual EditorTitleString editorTitle() const { return "Table Editor"; }

    void m_table_clear(t_symbol* s, const AtomListView& lv) { this->tableEditorClear(); }
    void m_table_close(t_symbol* s, const AtomListView& lv) { impl_.close(); }
    void m_table_addline(t_symbol* s, const AtomListView& lv) { this->tableEditorAddLine(s, lv); }
    void m_table_sync(t_symbol*, const AtomListView&)
    {
        impl_.setDirty(this->canvas(), true);
        this->tableEditorSync();
    }

    void setTableColumns(const std::vector<TableColumnParam>& cols) { cols_ = cols; }
    void addTableColumn(const TableColumnParam& col) { cols_.push_back(col); }

    void setTableEditorSize(int w, int h)
    {
        w_ = w;
        h_ = h;
    }

public:
    template <typename Factory>
    static void factoryTableObjectInit(Factory& f)
    {
        f.useClick();
        f.addMethod(".clear", &TableObject::m_table_clear);
        f.addMethod(".close", &TableObject::m_table_close);
        f.addMethod(".addline", &TableObject::m_table_addline);
        f.addMethod(".sync", &TableObject::m_table_sync);
    }
};

}

#endif // CEAMMC_TABLE_EDITOR_H
