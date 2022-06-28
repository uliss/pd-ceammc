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

#include <boost/static_string.hpp>
#include <list>

#include "extra/boost_intrusive_pool.hpp"

namespace ceammc {

using EditorTitleString = boost::static_string<32>;

class EditorString : public memorypool::boost_intrusive_pool_item {
public:
    boost::static_string<MAXPDSTRING> str;

    EditorString() { }
    void destroy() final { str.clear(); }

    void append(char ch);
    void append(t_float t);
    void append(const std::string& txt) { append(txt.c_str()); }
    void append(const char* txt);
    void append(t_symbol* s) { append(s->s_name); }
    void append(const Atom& a);
    void append(const AtomList& lst, const char* delim = " ");
    void append(const AtomListView& lv, const char* delim = " ");

    void appendQuoted(const std::string& txt) { appendQuoted(txt.c_str()); }
    void appendQuoted(t_symbol* s) { appendQuoted(s->s_name); }
    void appendQuoted(const char* txt);
    void appendQuoted(const Atom& a);
    void appendQuoted(const AtomListView& lv, const char* delim = " ");

    void pop();

    void clear() { str.clear(); }
    const char* c_str() const { return str.c_str(); }
    size_t length() const { return str.length(); }
    char back() const { return str.back(); }

    void trim();
};

using EditorStringPtr = boost::intrusive_ptr<EditorString>;
using EditorLineList = std::vector<EditorStringPtr>;

class EditorStringPool {
    EditorStringPool() = delete;

public:
    using Pool = memorypool::boost_intrusive_pool<EditorString>;

public:
    static Pool& pool();
    static void dumpMemoryUsage();
};

enum EditorSyntax {
    EDITOR_SYNTAX_NONE = 0,
    EDITOR_SYNTAX_SELECTOR,
    EDITOR_SYNTAX_LUA,
    EDITOR_SYNTAX_DEFAULT
};

class EditorObjectImpl {
    t_object* owner_;
    void* guiconnect_;
    bool escape_specs_;

public:
    EditorObjectImpl(t_object* owner);
    ~EditorObjectImpl();

    /**
     * create and open editor TCL window
     * @param cnv - pointer to onwer canvas
     * @param data - editor content
     * @param nchars - line width in chars
     * @param nlines - number of lines
     * @param lineNumbers - show line numbers
     * @param highlightSyntax - highlight syntax
     * @return true on success, false on error
     */
    void open(t_canvas* cnv, const EditorLineList& data,
        const EditorTitleString& title,
        int x, int y, int nchars, int nlines,
        bool lineNumbers, EditorSyntax syntax);

    /**
     * close TCL editor and stop GUI listening
     */
    void close();

    /**
     * Sync editor with specified data
     * @param lv
     */
    void sync(const EditorLineList& list);

    /**
     * Enable/disable special symbols ("\t,;{}") escaping
     * useful for LUA, for example
     */
    void setSpecialSymbolEscape(bool value);

    /**
     * set owner canvas dirty
     */
    void setDirty(t_canvas* c, bool value);

private:
    unsigned long xowner() const { return reinterpret_cast<unsigned long>(owner_); }
};

template <typename BaseClass>
class EditorObject : public BaseClass {
    EditorObjectImpl impl_;
    EditorSyntax syntax_;
    bool line_nums_;

public:
    EditorObject(const PdArgs& args)
        : BaseClass(args)
        , impl_(this->owner())
        , line_nums_(true)
        , syntax_(EDITOR_SYNTAX_DEFAULT)
    {
    }

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override
    {
        impl_.open(this->canvas(),
            this->getContentForEditor(),
            this->editorTitle(),
            (int)xpos, (int)ypos,
            this->calcEditorChars(),
            this->calcEditorLines(),
            line_nums_, syntax_);
    }

    virtual void editorClear() = 0;
    virtual void editorAddLine(t_symbol* sel, const AtomListView& lv) = 0;
    virtual void editorSync() { impl_.sync(this->getContentForEditor()); }
    virtual EditorLineList getContentForEditor() const = 0;
    virtual EditorTitleString editorTitle() const { return "EDITOR"; }

    virtual int calcEditorLines() const { return 20; }
    virtual int calcEditorChars() const { return 80; }

    void m_editor_clear(t_symbol* s, const AtomListView& lv) { this->editorClear(); }
    void m_editor_close(t_symbol* s, const AtomListView& lv) { impl_.close(); }
    void m_editor_addline(t_symbol* s, const AtomListView& lv) { this->editorAddLine(s, lv); }
    void m_editor_sync(t_symbol*, const AtomListView&)
    {
        impl_.setDirty(this->canvas(), true);
        this->editorSync();
    }

    /**
     * If show line numbers in editor
     */
    bool lineNumbers() const { return line_nums_; }

    /**
     * Set on/off line numbering in editor
     */
    void setLineNumbers(bool value) { line_nums_ = value; }

    /**
     * highlight syntax in editor
     */
    EditorSyntax highlightSyntax() const { return syntax_; }

    /**
     * Set on/off syntax highlighting in editor
     */
    void setHighlightSyntax(EditorSyntax value) { syntax_ = value; }

    /**
     * Enable/disable escaping of special chars
     */
    void setSpecialSymbolEscape(bool value) { impl_.setSpecialSymbolEscape(value); }

public:
    using ThisType = EditorObject<BaseClass>;

    template <typename Factory>
    static void registerMethods(Factory& obj)
    {
        obj.useClick();
        obj.addMethod(".clear", &ThisType::m_editor_clear);
        obj.addMethod(".close", &ThisType::m_editor_close);
        obj.addMethod(".addline", &ThisType::m_editor_addline);
        obj.addMethod(".sync", &ThisType::m_editor_sync);
    }
};

}

#endif // CEAMMC_EDITOR_OBJECT_H
