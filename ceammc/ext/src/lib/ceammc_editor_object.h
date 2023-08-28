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

#include "ceammc_datatypes.h"
#include "ceammc_object.h"

#include <list>
#include <string>

#include "extra/boost_intrusive_pool.hpp"

namespace ceammc {

constexpr size_t EditorTitleMaxLength = 40;
using EditorTitleString = BoostStaticString<EditorTitleMaxLength>;

EditorTitleString makeEditorTitleString(const char* dataName, const char* dataId = "");

class EditorString : public memorypool::boost_intrusive_pool_item {
public:
    BoostStaticString<MAXPDSTRING> str;

    EditorString() { }
    void destroy() final { str.clear(); }

    void append(char ch);
    void append(t_float t);
    void append(const std::string& txt) { append(txt.c_str()); }
    void append(const char* txt);
    void append(t_symbol* s) { append(s->s_name); }
    void append(const Atom& a);
    void append(const AtomListView& lv, const char* delim = " ");
    void append(const char* txt, size_t len);

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

enum class EditorSyntax {
    NONE,
    SELECTOR,
    LUA,
    FAUST,
    DEFAULT
};

enum class EditorEscapeMode {
    DEFAULT,
    LUA,
    DATA
};

/**
 * Unescape string according to mode
 */
bool editor_string_unescape(std::string& str, EditorEscapeMode mode);

bool editor_string_escape(const char* str, AtomList& res, EditorEscapeMode mode);

class EditorObjectImpl {
    t_object* owner_;
    void* guiconnect_;
    EditorEscapeMode esc_mode_;

public:
    EditorObjectImpl(t_object* owner, EditorEscapeMode mode = EditorEscapeMode::DEFAULT);
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
    void setSpecialSymbolEscape(EditorEscapeMode mode);

    /**
     * set owner canvas dirty
     */
    void setDirty(t_canvas* c, bool value);

private:
    unsigned long xowner() const { return reinterpret_cast<std::uintptr_t>(owner_); }
};

template <typename T, EditorSyntax S = EditorSyntax::DEFAULT, EditorEscapeMode M = EditorEscapeMode::DEFAULT>
class EditorObject : public T {
public:
    using EditorObjectT = EditorObject<T, S, M>;

private:
    EditorObjectImpl impl_;
    EditorSyntax syntax_;
    bool line_nums_;

public:
    explicit EditorObject(const PdArgs& args)
        : T(args)
        , impl_(this->owner(), M)
        , line_nums_(true)
        , syntax_(S)
    {
    }

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override
    {
        openEditor(xpos, ypos);
    }

    void openEditor(int x, int y)
    {
        impl_.open(this->canvas(),
            this->getContentForEditor(),
            this->editorTitle(),
            x, y,
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
     * Inplace string unescape
     * O = N
     */
    bool unescapeString(std::string& str) const { return editor_string_unescape(str, M); }

public:
    template <typename Factory>
    static void factoryEditorObjectInit(Factory& f)
    {
        f.useClick();
        f.addMethod(".clear", &EditorObjectT::m_editor_clear);
        f.addMethod(".close", &EditorObjectT::m_editor_close);
        f.addMethod(".addline", &EditorObjectT::m_editor_addline);
        f.addMethod(".sync", &EditorObjectT::m_editor_sync);
    }
};

}

#endif // CEAMMC_EDITOR_OBJECT_H
